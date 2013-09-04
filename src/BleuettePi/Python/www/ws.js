
//var HOST = '192.168.2.17';
var HOST = 'localhost';

(function (exports, $) {

    exports.Bleuette = function() {
    
        var self = this;

        return {
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
        B.sendCmd('set', { type: 'trim', servo: $(ev.currentTarget).data('servo'), value: ev.value });
    });

    $('.slider.trimv').slider({
        min: -20,
        max: 20,
        step: 1,
        value: 0,
        orientation: 'vertical'
    }).on('slide', function(ev) {
        B.sendCmd('set', { type: 'trim', servo: $(ev.currentTarget).data('servo'), value: ev.value });
    });

    $('.slider.moveh').slider({
        min: 0,
        max: 255,
        step: 1,
        value: 128,
        orientation: 'horizontal'
    }).on('slide', function(ev) {
        B.sendCmd('set', { type: 'position', servo: $(ev.currentTarget).data('servo'), value: ev.value });
    });

    $('.slider.movev').slider({
        min: 0,
        max: 255,
        step: 1,
        value: 128,
        orientation: 'vertical'
    }).on('slide', function(ev) {
        B.sendCmd('set', { type: 'position', servo: $(ev.currentTarget).data('servo'), value: ev.value });
    });

    $('.slider.limith').slider({
        min: 0,
        max: 254,
        step: 1,
        value: [ 64, 192 ],
        orientation: 'horizontal'
    }).on('slide', function(ev) {
        B.sendCmd('set', { type: 'limit', servo: $(ev.currentTarget).data('servo'), min: ev.value[0], max: ev.value[1] });
    });

    $('.slider.limitv').slider({
        min: 0,
        max: 254,
        step: 1,
        value: [ 64, 192 ],
        orientation: 'vertical'
    }).on('slide', function(ev) {
        B.sendCmd('set', { type: 'limit', servo: $(ev.currentTarget).data('servo'), min: ev.value[0], max: ev.value[1] });
    });


    $('#tab').tab();

/*
    $('#btn-left').click(function() {
        sendCmd('drive', { direction: 'left' });
    });

    $('#btn-forward').click(function() {
        sendCmd('drive', { direction: 'forward' });
    });

    $('#btn-right').click(function() {
        sendCmd('drive', { direction: 'right' });
    });

    $('#btn-reverse').click(function() {
        sendCmd('drive', { direction: 'reverse' });
    });
*/

    $('.btn-save').click(function() {
        B.sendCmd('config', { action: 'save' });
    });

    $('#btn-pushup').click(function() {
        ws.send('pushup');
    });

    $('#btn-middle').click(function() {
        ws.send('middle');
    });

    //ws = new WebSocket('ws://192.168.2.17:8888/ws');
    ws = new WebSocket('ws://' + HOST + ':8888/ws');
    var $message = $('#message');

    ws.onopen = function(){
        $message.attr("class", 'label label-success');
        $message.text('open');

        B.sendCmd('config', { action: 'get' });
    };

    ws.onmessage = function(ev){
        console.info(ev.data);

        json = JSON.parse(ev.data);

        switch (json.type) {
            case 'config':
                for (i = 0; i < json.data.trims.length; i++) {
                    $('#trim-slide' + i).slider('setValue', json.data.trims[i]);
                }
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

var keymap = {
    37: 'left',
    38: 'forward',
    39: 'right',
    40: 'reverse'
};

$(document).keydown(function(event) {
    switch (event.keyCode) {
        case 37:
        case 38:
        case 39:
        case 40:
            B.sendCmd('drive', { status: 'begin', direction: keymap[event.keyCode] });
            break;
    }
});

$(document).keyup(function(event) {
    switch (event.keyCode) {
        case 37:
        case 38:
        case 39:
        case 40:
            B.sendCmd('drive', { status: 'end', direction: keymap[event.keyCode] });
            break;
    }
});

