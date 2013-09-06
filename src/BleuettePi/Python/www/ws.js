
//var HOST = '192.168.2.17';
var HOST = 'localhost';

var keymap = {
    37: 'left',
    38: 'forward',
    39: 'right',
    40: 'reverse'
};

(function (exports, $) {

    exports.Bleuette = function() {
    
        var self = this;

        return {

            CMD_SET: 'set',
            CMD_DRIVE: 'drive',
            CMD_CONFIG: 'config',
            CMD_CONTROL: 'control',
            CMD_SEQUENCE: 'sequence',

            config: (function() {

                return {
                    received: function(data) {
                        alert('received!');
                    }
                }
            })(),

            sendCmd: function(cmd, data) {
                data = data || {};
                data['cmd'] = cmd;
                ws.send(JSON.stringify(data));
            }
        }
        
        return self;
    };

})(this, jQuery);

var B = new Bleuette;
var ws = null;

$(document).ready(function(event) {

    $('.slider.trimh').slider({
        min: -20,
        max: 20,
        step: 1,
        value: 0,
        orientation: 'horizontal'
    }).on('slide', function(ev) {
        B.sendCmd(B.CMD_SET, { type: 'trim', servo: $(ev.currentTarget).data('servo'), value: ev.value });
    });

    $('.slider.trimv').slider({
        min: -20,
        max: 20,
        step: 1,
        value: 0,
        orientation: 'vertical'
    }).on('slide', function(ev) {
        B.sendCmd(B.CMD_SET, { type: 'trim', servo: $(ev.currentTarget).data('servo'), value: ev.value });
    });

    $('.slider.moveh').slider({
        min: 0,
        max: 255,
        step: 1,
        value: 128,
        orientation: 'horizontal'
    }).on('slide', function(ev) {
        B.sendCmd(B.CMD_SET, { type: 'position', servo: $(ev.currentTarget).data('servo'), value: ev.value });
    });

    $('.slider.movev').slider({
        min: 0,
        max: 255,
        step: 1,
        value: 128,
        orientation: 'vertical'
    }).on('slide', function(ev) {
        B.sendCmd(B.CMD_SET, { type: 'position', servo: $(ev.currentTarget).data('servo'), value: ev.value });
    });

    $('.slider.limith').slider({
        min: 0,
        max: 254,
        step: 1,
        value: [ 64, 192 ],
        orientation: 'horizontal'
    }).on('slide', function(ev) {
        B.sendCmd(B.CMD_SET, { type: 'limit', servo: $(ev.currentTarget).data('servo'), min: ev.value[0], max: ev.value[1] });
    });

    $('.slider.limitv').slider({
        min: 0,
        max: 254,
        step: 1,
        value: [ 64, 192 ],
        orientation: 'vertical'
    }).on('slide', function(ev) {
        B.sendCmd(B.CMD_SET, { type: 'limit', servo: $(ev.currentTarget).data('servo'), min: ev.value[0], max: ev.value[1] });
    });

    $('#speed-slide').slider({
        min: 0,
        max: 16,
        step: 1,
        value: 16,
        orientation: 'horizontal'
    }).on('slide', function(ev) {
        B.sendCmd(B.CMD_SET, { type: 'speed', value: ev.value });
    });

    $('#tab').tab();

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
        B.sendCmd(B.CMD_SET, { type: 'loglevel', level: $(this).val() });
    });

    $('.btn-action').click(function() {
        switch ($(this).data('action')) {
            case 'save':
                B.sendCmd(B.CMD_CONFIG, { action: 'save' });
                break;
            case 'reset':
                B.sendCmd(B.CMD_CONTROL, { action: 'reset' });
                break;
        }
    });

    $('#btn-pushup').click(function() {
        B.sendCmd(B.CMD_SEQUENCE, { name: 'pushup' });
    });

    $('#btn-middle').click(function() {
        B.sendCmd(B.CMD_SEQUENCE, { name: 'middle' });
    });

    $('#btn-standby').click(function() {
        B.sendCmd(B.CMD_SEQUENCE, { name: 'standby' });
    });

    //ws = new WebSocket('ws://192.168.2.17:8888/ws');
    ws = new WebSocket('ws://' + HOST + ':8888/ws');
    var $message = $('#message');

    ws.onopen = function(){
        $message.attr("class", 'label label-success');
        $message.text('open');

        B.sendCmd(B.CMD_CONFIG, { action: 'get' });
    };

    ws.onmessage = function(ev){
        json = JSON.parse(ev.data);
        switch (json.type) {
            case B.CMD_CONFIG:
                for (i = 0; i < json.data.trims.length; i++) {
                    $('#trim-slide' + i).slider('setValue', json.data.trims[i]);
                }

                for (i = 0; i < json.data.limits.length; i++) {
                    $('#limit-slide' + i).slider('setValue', json.data.limits[i]);
                }
                break;

            case 'position':
                console.info(json.data);
                for (i = 0; i < json.data.servos.length; i++) {
                    console.info("Servo " + i + " - " + json.data.servos[i]);
                    $('#move-slide' + i).slider('setValue', json.data.servos[i]);
                }
                break;
            default:
                console.info(json);
        }

/*
        $message.attr("class", 'label label-info');
        $message.hide();
        $message.fadeIn("slow");
        $message.text('recieved message');

        var json = JSON.parse(ev.data);
        //console.info(json);

        $('#acc-x').text(json.sensors.accelerometer[0]);
        $('#acc-y').text(json.sensors.accelerometer[1]);
        $('#acc-z').text(json.sensors.accelerometer[2]);


        for (i = 0; i < 6; i++) {
            if (json.sensors.ground[i]) {
                $('#ground-' + i).css('background-color', 'red');
            } else {
                $('#ground-' + i).css('background-color', 'green');
            }
        }

        $('#current').text(json.sensors.current);

      //var json = JSON.parse(ev.data);
      $('#' + json.id).hide();
      $('#' + json.id).fadeIn("slow");
      $('#' + json.id).text(json.value);

*/
    /*
      var $rowid = $('#row' + json.id);
      if(json.value > 500){
        $rowid.attr("class", "error");
      }
      else if(json.value > 200){
        $rowid.attr("class", "warning");
      }
      else{
        $rowid.attr("class", "");
      }
      */
    };

    ws.onclose = function(ev){
        $message.attr("class", 'label label-important');
        $message.text('closed');
    };

    ws.onerror = function(ev){
        $message.attr("class", 'label label-warning');
        $message.text('error occurred');
    };
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

