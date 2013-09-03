
var HOST = '192.168.2.17';

var ws = null;
$(document).ready(function(event) {

    $('#btn-left').click(function() {
        ws.send('left');
    });

    $('#btn-forward').click(function() {
        ws.send('forward');
    });

    $('#btn-right').click(function() {
        ws.send('right');
    });

    $('#btn-reverse').click(function() {
        ws.send('reverse');
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
    };

    ws.onmessage = function(ev){
        console.info(ev.data);

        $message.attr("class", 'label label-info');
        $message.hide();
        $message.fadeIn("slow");
        $message.text('recieved message');

        var json = JSON.parse(ev.data);
        console.info(json);
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

$(document).keydown(function(event) {
    switch (event.keyCode) {
        case 37:
            ws.send("left");
            break
        case 38:
            ws.send("forward");
            break;
        case 39:
            ws.send("right");
            break;
        case 40:
            ws.send("reverse");
            break;
    }
});

$(document).keyup(function(event) {
    ws.send("release");
});

