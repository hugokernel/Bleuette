
var keymap = {
    37: 'left',
    38: 'forward',
    39: 'right',
    40: 'reverse'
};

var cad = null;

var slide = (function() {

    var timeoutHdl = {};

    var tooltip = function(value0, value1) {
        $(this).children('.ui-slider-handle:first').html('<div class="tooltip top slider-tip"><div class="tooltip-arrow"></div><div class="tooltip-inner">' + value0 + '</div></div>');
        if (value1) {
            $(this).children('.ui-slider-handle:last').html('<div class="tooltip top slider-tip"><div class="tooltip-arrow"></div><div class="tooltip-inner">' + value1 + '</div></div>');
        }
    };

    var set = function(type, index, value) {
        $('#' + type + '-slide' + index).slider((type == 'limit' ? 'values' : 'value'), value);

        if (type == 'limit') {
            $('#limit-' + index).html(value[0] + ', ' + value[1]).css('color', 'red');
        } else {
            $('#' + type + '-' + index).html(value).css('color', 'red');
        }

        clearTimeout(timeoutHdl['#' + type + '-' + index]);
        timeoutHdl['#' + type + '-' + index] = setTimeout(function() {
            $('#' + type + '-' + index).animate({ color: 'black' }, 300);
        }, 700);
    };

    return {

        init: function() {

            $('.slide.move').slider({
                min: 0,
                max: 254,
                step: 1,
                value: 128,
                orientation: 'horizontal',
                slide: function(ev, ui) {
                    //tooltip.call(this, ui.value);
                    servo = $(this).data('servo');

                    value = servoPos(servo, ui.value, 128);

                    set('move', servo, value);

                    B.sendCmd(B.CMD_SERVO, { action: 'position', servo: servo, value: value });
                }
            }).dblclick(function() {
                set('move', $(this).data('servo'), 128);
                B.sendCmd(B.CMD_SERVO, { action: 'position', servo: $(this).data('servo'), value: 128 });
            }).mousewheel(function(e, delta) {
                //tooltip.call(this, $(this).slider('value') + delta);
                set('move', $(this).data('servo'), $(this).slider('value') + delta);
                B.sendCmd(B.CMD_SERVO, { action: 'position', servo: $(this).data('servo'), value: $(this).slider('value') + delta });
                return false;
            });

            $('.slide.trim').slider({
                min: -20,
                max: 20,
                step: 1,
                value: 0,
                orientation: 'horizontal',
                slide: function(ev, ui) {
                    tooltip.call(this, ui.value);

                    servo = $(this).data('servo');
                    value = ui.value;

                    set('trim', servo, value);

                    value = servoPos(servo, value, 0, function() {
                        return value = -value;
                    });

                    B.sendCmd(B.CMD_SERVO, { action: 'trim', servo: servo, value: value });
                }
            }).dblclick(function() {
                set('trim', $(this).data('servo'), 0);
                B.sendCmd(B.CMD_SERVO, { action: 'position', servo: $(this).data('servo'), value: 0 });
            }).mousewheel(function(e, delta) {
                //tooltip.call(this, $(this).slider('value') + delta);
                set('trim', $(this).data('servo'), $(this).slider('value') + delta);
                B.sendCmd(B.CMD_SERVO, { action: 'position', servo: $(this).data('servo'), value: $(this).slider('value') + delta });
                return false;
            });

            $('.slide.limit').slider({
                range: true,
                min: 0,
                max: 254,
                step: 1,
                values: [ 64, 192 ],
                orientation: 'horizontal',
                slide: function(ev, ui) {
                    set('limit', $(this).data('servo'), ui.values);
                    B.sendCmd(B.CMD_SERVO, { action: 'limit', servo: $(this).data('servo'), min: ui.values[0], max: ui.values[1] });
                }
            }).dblclick(function() {
                set('limit', $(this).data('servo'), [ 64, 192 ]);
            });
            /*.mousewheel(function(e, delta) {
                $(this).slider('value', $(this).slider('value') + delta);
                return false;
            });
            */

            $('#speed-slide').slider({
                min: 0,
                max: 16,
                step: 1,
                value: 16,
                orientation: 'horizontal',
                slide: function(ev, ui) {
                    B.sendCmd(B.CMD_SERVO, { action: 'speed', value: ui.value });
                }
            });
        },

        set: set
    };

})();

fifo = function(size) {
    return {
        data: [],
        size: size,
        _total: 0,

        add: function(item) {
            if (this.data.length > this.size) {
                this.data.shift();
            }

            this.data.push(item);
            this._total++;
        },

        get: function(indexed) {
            if (indexed) {
                data = [];
                for (i = 0; i < this.data.length; i++) {
                    data.push([i, this.data[i]]);
                }
                return data;
            } else {
                return this.data;
            }
        },

        total: function() {
            return this._total;
        }
    }
};



var servoPos = function(servo, value, scale, callback) {

    // Disabled mode
    if (1) {
        return value;
    }

    if (servo == 1 || servo == 3 || servo == 10 || servo == 5) {
        if (callback) {
            return callback(value);
        } else {
            if (value > scale) {
                offset = value - scale;
                value = scale - offset;
            } else {
                offset = scale - value;
                value = scale + offset;
            }
        }
    }
    return value;
};

var Events = {};

$(document).ready(function(event) {

    smokesignals.convert(Events);

    Events.on('servo.received', function(data) {
        for (i = 0; i < data.trims.length; i++) {
            slide.set('trim', i, data.trims[i]);
        }

        for (i = 0; i < data.limits.length; i++) {
            slide.set('limit', i, data.limits[i]);
        }
    });

    Events.on('position.received', function(servos) {
        for (i = 0; i < 12; i++) {
            slide.set('move', i, servoPos(i, servos[i], 128));

            B.simulation.setServoPosition(i, servos[i]);
        }
    });

    var sensorsReveived = function(sensors) {

        $('#acc-x').text(sensors.accelerometer[0]);
        $('#acc-y').text(sensors.accelerometer[1]);
        $('#acc-z').text(sensors.accelerometer[2]);

        for (i = 0; i < 6; i++) {
            if (sensors.ground[i]) {
                $('#ground-' + i).css('background-color', 'red');

                if (cad) {
                    cad.leg.setColor(i + 5, 0xff0000);
                }
            } else {
                $('#ground-' + i).css('background-color', 'green');

                if (cad) {
                    cad.leg.setColor(i + 5, 0x00ff00);
                }
            }
        }

        $('#compass-heading').text(sensors.compass[1]);
        $('#compass-x').text(sensors.compass[0][0]);
        $('#compass-y').text(sensors.compass[0][1]);
        $('#compass-z').text(sensors.compass[0][2]);

        $('#current').text(sensors.current);
        $('#batteryVoltage').text(sensors.batteryVoltage);
    };

    Events.on('sensors.received', sensorsReveived);

    Events.on('tab.loaded:sensors', function() {

        series = [{
            label: 'X',
            data: [],
            lines: {
                fill: true
            }
        }, {
            label: 'Y',
            data: [],
            lines: {
                fill: true
            }
        }, {
            label: 'Z',
            data: [],
            lines: {
                fill: true
            }
        }];

        options = {
            grid: {
                borderWidth: 1,
                minBorderMargin: 20,
                labelMargin: 10,
                backgroundColor: {
                    colors: ["#fff", "#e4f4f4"]
                },
                margin: {
                    top: 8,
                    bottom: 20,
                    left: 20
                },
                markings: function(axes) {
                    var markings = [];
                    var xaxis = axes.xaxis;
                    for (var x = Math.floor(xaxis.min); x < xaxis.max; x += xaxis.tickSize * 2) {
                        markings.push({ xaxis: { from: x, to: x + xaxis.tickSize }, color: "rgba(232, 232, 255, 0.2)" });
                    }
                    return markings;
                }
            },
            xaxis: {
                min: 0,
                max: 20
                /*
                tickFormatter: function() {
                    return "";
                }
                */
            },
            yaxis: {
                min: -1000,
                max: 1000
            },
            legend: {
                show: true
            }
        };

        accelOptions = {};
        $.extend(accelOptions, options);
        accelOptions.yaxis = {
            min: -1000,
            max: 1000
        };

        compassOptions = {};
        $.extend(compassOptions, options);
        compassOptions.yaxis = {
            min: -900,
            max: 900
        };

        currentOptions = {};
        $.extend(currentOptions, options);
        currentOptions.yaxis = {
            min: 0,
            max: 5
        };

        voltageOptions = {};
        $.extend(voltageOptions, options);
        voltageOptions.yaxis = {
            min: 0,
            max: 12
        };

        var accelPlot = $.plot($("#accelerometer-graph"), series, accelOptions);
        var compassPlot = $.plot($("#compass-graph"), series, compassOptions);
        var currentPlot = $.plot($("#current-graph"), series, currentOptions);
        var voltagePlot = $.plot($("#voltage-graph"), series, voltageOptions);

        //$("<div class='axisLabel yaxisLabel'></div>").text("Accelerometer").appendTo(container);
        //yaxisLabel.css("margin-top", yaxisLabel.width() / 2 - 20);

        var accelFifo = [ new fifo(20), new fifo(20), new fifo(20) ];
        var compassFifo = [ new fifo(20), new fifo(20), new fifo(20) ];
        var currentFifo = new fifo(20);
        var voltageFifo = new fifo(20);

        Events.on('sensors.received', function(sensors) {

            // Accelerometer
            accelFifo[0].add(json.sensors.accelerometer[0]);
            accelFifo[1].add(json.sensors.accelerometer[1]);
            accelFifo[2].add(json.sensors.accelerometer[2]);

            accelPlot.setData([
                accelFifo[0].get(true),
                accelFifo[1].get(true),
                accelFifo[2].get(true),
            ]);
            accelPlot.draw();

            // Compas
            compassFifo[0].add(json.sensors.compass[0][0]);
            compassFifo[1].add(json.sensors.compass[0][1]);
            compassFifo[2].add(json.sensors.compass[0][2]);

            compassPlot.setData([
                compassFifo[0].get(true),
                compassFifo[1].get(true),
                compassFifo[2].get(true),
            ]);
            compassPlot.draw();

            // Voltage
            voltageFifo.add(json.sensors.batteryVoltage);
            voltagePlot.setData([
                voltageFifo.get(true)
            ]);
            voltagePlot.draw();

            // Current
            currentFifo.add(json.sensors.current);
            currentPlot.setData([
                currentFifo.get(true)
            ]);
            currentPlot.draw();
        });

    }).on('tab.unloaded:sensors', function() {
        Events.off('sensors.received').on('sensors.received', sensorsReveived);
    });

    Events.on('tab.loaded:legs', function() {

        var x = 15;

        series = [];
        for (i = 0; i < 12; i++) {
            series.push({
                label: i,
                data: [],
                lines: {
                    fill: true
                }
            });
        }

/*
        series = [{
            label: '0',
            data: [],
            lines: {
                fill: true
            },
            curvedLines: {
                apply:true
            }
        }, {
            label: '1',
            data: [],
            lines: {
                fill: true
            },
            curvedLines: {
                apply:true
            }
        }, ];
*/

        options = {
            grid: {
                borderWidth: 1,
                minBorderMargin: 20,
                labelMargin: 10,
                backgroundColor: {
                    colors: ["#fff", "#e4f4f4"]
                },
                margin: {
                    top: 8,
                    bottom: 20,
                    left: 20
                },
                markings: function(axes) {
                    var markings = [];
                    var xaxis = axes.xaxis;
                    for (var x = Math.floor(xaxis.min); x < xaxis.max; x += xaxis.tickSize * 2) {
                        markings.push({ xaxis: { from: x, to: x + xaxis.tickSize }, color: "rgba(232, 232, 255, 0.2)" });
                    }
                    return markings;
                }
            },
            xaxis: {
                min: 0,
                max: x
            },
            yaxis: {
                min: 0,
                max: 255
            },
            legend: {
                show: true
            }
        };

        var splot = $.plot($("#legs-graph"), series, options);

        sfifo = [
            new fifo(x), new fifo(x), new fifo(x), new fifo(x),
            new fifo(x), new fifo(x), new fifo(x), new fifo(x),
            new fifo(x), new fifo(x), new fifo(x), new fifo(x),
        ];

        Events.on('position.received', function(servos) {

            data = [];
            for (i = 0; i < 12; i++) {
                if ($.inArray(String(i), $('#legs-selector').val()) > -1) {
                    sfifo[i].add(servos[i]);
                } else {
                    sfifo[i].add(-1);
                }
//                data[i] = sfifo[i].get(true);
            }

/*
            for (ii = 0; ii < 12; ii++) {
                //data.push([sfifo[i].get(true)]);
                data[ii] = sfifo[ii].get(true);
                console.info('i:' + ii);
                console.info(data);
                alert('p[af');
                break;
            }
*/
            data = [
                sfifo[0].get(true),
                sfifo[1].get(true),
                sfifo[2].get(true),
                sfifo[3].get(true),
                sfifo[4].get(true),
                sfifo[5].get(true),
                sfifo[6].get(true),
                sfifo[7].get(true),
                sfifo[8].get(true),
                sfifo[9].get(true),
                sfifo[10].get(true),
                sfifo[11].get(true),
            ];

            console.info(data);

            splot.setData(data);
            splot.draw();
        });

    }).on('tab.unloaded:sensors', function() {
        Events.off('sensors.received').on('sensors.received', sensorsReveived);
    });


    slide.init();

    $.pnotify.defaults.styling = "bootstrap";
    $.pnotify({
        title: 'Hi',
        text: 'Welcome !',
    });

/*
    $('#tab').tab({
        activate: function(event, ui) {
            alert('paf');
        }
    });
*/

    $('ul#tabs a[data-toggle="tab"]').on('shown', function (e) {
        Events.emit('tab.unloaded:' + $(e.relatedTarget).data('name'));
        Events.emit('tab.loaded:' + $(e.target).data('name'));
    })

    var initialized = false;
    //$('a#tab-simulation[data-toggle="tab"]').on('shown', function (e) {
    $('#3dview-handler').click(function() {

        $("#popup-3dview").dialog({
            dialogClass: "no-close",
            resizable: false,
            width: 660,
            create: function() {
                if (!initialized) {
                    B.sendCmd(B.CMD_SET, { type: 'livemode', status: true });
                    B.simulation.init();
                    B.simulation.animate();
                    initialized = true;
                }
            }
        });
    });

    $('#btn-left').mousedown(function() {
        B.sendCmd(B.CMD_DRIVE, { status: 'begin', direction: keymap[37] });
    }).mouseup(function() {
        B.sendCmd(B.CMD_DRIVE, { status: 'end', direction: keymap[37] });
    });

    $('#btn-forward').mousedown(function() {
        B.sendCmd(B.CMD_DRIVE, { status: 'begin', direction: keymap[38] });
    }).mouseup(function() {
        B.sendCmd(B.CMD_DRIVE, { status: 'end', direction: keymap[38] });
    });

    $('#btn-right').mousedown(function() {
        B.sendCmd(B.CMD_DRIVE, { status: 'begin', direction: keymap[39] });
    }).mouseup(function() {
        B.sendCmd(B.CMD_DRIVE, { status: 'end', direction: keymap[39] });
    });

    $('#btn-reverse').mousedown(function() {
        B.sendCmd(B.CMD_DRIVE, { status: 'begin', direction: keymap[40] });
    }).mouseup(function() {
        B.sendCmd(B.CMD_DRIVE, { status: 'end', direction: keymap[40] });
    });

    $('#livemode').change(function() {
        B.sendCmd(B.CMD_SET, { type: 'livemode', status: $(this).is(':checked') });
    });

    $('#loglevel').change(function() {
        B.sendCmd(B.CMD_SET, { type: 'loglevel', which: 'default', level: $(this).val() });
    });

    $('#jsloglevel').change(function() {
        B.sendCmd(B.CMD_SET, { type: 'loglevel', which: 'js', level: $(this).val() });
    });

    $('.btn-action').click(function() {
        switch ($(this).data('action')) {
            case 'save':
                B.sendCmd(B.CMD_SERVO, { action: 'save' });
                break;
            case 'reset':
                B.sendCmd(B.CMD_CONTROL, { action: 'reset' });
                break;
        }
    });

    $('.btn-sequence').click(function() {
        B.sendCmd(B.CMD_SEQUENCE, { name: $(this).data('sequence') });
    });

    $('.btn-servo').click(function() {
        B.sendCmd(B.CMD_SERVO, { action: $(this).data('command') });
    });
});

var released = true;
$(document).keydown(function(event) {
    switch (event.keyCode) {
        case 37:
        case 38:
        case 39:
        case 40:
            if (released) {
                B.sendCmd(B.CMD_DRIVE, { status: 'begin', direction: keymap[event.keyCode] });
                released = false;
            }
            break;
    }
}).keyup(function(event) {
    switch (event.keyCode) {
        case 37:
        case 38:
        case 39:
        case 40:
            released = true;
            B.sendCmd(B.CMD_DRIVE, { status: 'end', direction: keymap[event.keyCode] });
            break;
    }
});

