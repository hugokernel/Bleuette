
var keymap = {
    37: 'left',
    38: 'forward',
    39: 'right',
    40: 'reverse'
};

var cad = null;

(function (exports, $) {

    exports.Bleuette = function() {

        var self = this;

        return {

            host: '',
            fakeMode: false,

            CMD_SET: 'set',
            CMD_DRIVE: 'drive',
            CMD_CONFIG: 'config',
            CMD_CONTROL: 'control',
            CMD_SEQUENCE: 'sequence',
            CMD_SERVO: 'servo',

            init: function() {

                self.ws = new WebSocket('ws://' + this.host + ':8888/ws');
                var $message = $('#message');

                var agraph = new Dygraph(document.getElementById("accelerometer-graph"), [ [0, 0, 0, 0 ] ], {
                    //drawPoints: true,
                    //showRoller: true,
                    valueRange: [ -5000, 5000],
                    labels: ['Unit', 'z', 'y', 'z']
                });

                var cgraph = new Dygraph(document.getElementById("compass-graph"), [ [0, 0, 0, 0 ] ], {
                    //drawPoints: true,
                    //showRoller: true,
                    valueRange: [ 0, 360],
                    labels: ['Unit', 'Heading']
                });

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

                        get: function() {
                            return this.data;
                        },

                        total: function() {
                            return this._total;
                        }
                    }
                };

                var afifo = new fifo(20);
                var cfifo = new fifo(20);

                self.ws.onopen = function(){
                    $message.attr("class", 'label label-success');
                    $message.text('open');

                    B.sendCmd(B.CMD_CONFIG, { action: 'get' });
                };

                self.ws.onmessage = function(ev){
                    json = JSON.parse(ev.data);
                    switch (json.type) {
                        case B.CMD_CONFIG:
                        /*
                            for (i = 0; i < json.data.trims.length; i++) {
                                $('#trim-slide' + i).slider('setValue', json.data.trims[i]);
                            }

                            for (i = 0; i < json.data.limits.length; i++) {
                                $('#limit-slide' + i).slider('setValue', json.data.limits[i]);
                            }
                            */
                            break;

                        case 'position':
                    //console.info(json.data.servos);
                            for (i = 0; i < 12; i++) {
                                /*
                                $('#move-slide' + i).slider('setValue', json.data.servos[i]);
                                */

                                //if (i == 0) {
                                    B.simulation.setServoPosition(i, json.data.servos[i]);
                                    /*
                                    coeff = 1.4 / 255;
                                    console.info("Coeff: " + coeff);
                                    B.simulation.legs[i].rotation.y = coeff * json.data.servos[i] - 1.4/2;
                                    console.info("Value: " + coeff * json.data.servos[i]);
                                    */
                                //}
//                                leg3.rotation.y -= 0.01;
                            }
                            break;
                        case 'sensors':
                            $('#acc-x').text(json.sensors.accelerometer[0]);
                            $('#acc-y').text(json.sensors.accelerometer[1]);
                            $('#acc-z').text(json.sensors.accelerometer[2]);

                            for (i = 0; i < 6; i++) {
                                if (json.sensors.ground[i]) {
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

                            $('#compass-heading').text(json.sensors.compass[1]);
                            $('#compass-x').text(json.sensors.compass[0][0]);
                            $('#compass-y').text(json.sensors.compass[0][1]);
                            $('#compass-z').text(json.sensors.compass[0][2]);


                            afifo.add([afifo.total(), json.sensors.accelerometer[0], json.sensors.accelerometer[1], json.sensors.accelerometer[2]])
                            agraph.updateOptions({ 'file': afifo.get() });

                            heading = json.sensors.compass[1].split('°');
                            cfifo.add([cfifo.total(), heading[0] ]);
                            cgraph.updateOptions({ 'file': cfifo.get() });
                            break;
                        case 'log':

                            switch(json.data.levelname) {
                                case 'DEBUG':
                                    console.debug('[' + json.data.levelname + '] ' + json.data.msg);
                                    break;
                                case 'WARNING':
                                    console.warn('[' + json.data.levelname + '] ' + json.data.msg);
                                    break;
                                case 'INFO':
                                    console.info('[' + json.data.levelname + '] ' + json.data.msg);
                                    break;
                                case 'ERROR':
                                case 'CRITICAL':
                                    console.error('[' + json.data.levelname + '] ' + json.data.msg);
                                    break;
                                default:
                                    alert('Log level error unknow ' + json.data.levelname + ' !');
                            }

                            break;
                        default:
                            console.info(json);
                    }
                };

                self.ws.onclose = function(ev){
                    $message.attr("class", 'label label-important');
                    $message.text('closed');
                };

                self.ws.onerror = function(ev){
                    $message.attr("class", 'label label-warning');
                    $message.text('error occurred');
                };
            },

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
                self.ws.send(JSON.stringify(data));
            },

            simulation: (function() {

                //var parent, renderer, scene, camera, controls;

                //var bodyTruc, leg0, leg1, leg2, leg3, leg4, leg5;

                return {

                    legs: [],

                    init: function() {

                        // info
                        info = document.createElement( 'div' );
                        info.style.position = 'absolute';
                        info.style.top = '30px';
                        info.style.width = '100%';
                        info.style.textAlign = 'center';
                        info.style.color = '#fff';
                        info.style.fontWeight = 'bold';
                        info.style.backgroundColor = 'transparent';
                        info.style.zIndex = '1';
                        info.style.fontFamily = 'Monospace';
                        //info.innerHTML = 'Drag mouse to rotate camera';
                        document.body.appendChild( info );

/*
                        // renderer
                        //renderer = new THREE.CanvasRenderer();
                        renderer = new THREE.WebGLRenderer();
                        renderer.setSize( window.innerWidth, window.innerHeight );
                        renderer.physicallyBasedShading = true;

                        //document.getElementById('render').appendChild(renderer.domElement);
*/
                        container = document.getElementById( 'canvas' );
                        //document.body.appendChild( container );

                        renderer = new THREE.CanvasRenderer();
                        //renderer = new THREE.WebGLRenderer();
                        renderer.setSize(640, 480);
                        container.appendChild( renderer.domElement );

                        //document.body.appendChild( renderer.domElement );

                        // scene
                        scene = new THREE.Scene();

                        //light = new THREE.AmbientLight(0x00000);
                        //scene.add(light);
                        //var ambientLight = new THREE.AmbientLight(0x0000c1);
                        //scene.add(ambientLight);
                        var light = new THREE.PointLight(0xffffc1);
                        light.position.set(10, 10, 10);
                        light.add(new THREE.AxisHelper(20));
                        scene.add(light);

                        // camera
                        camera = new THREE.PerspectiveCamera(30, window.innerWidth / window.innerHeight, 0.1, 100);
                        camera.position.set(20, 20, 20);

                        // controls
                        controls = new THREE.OrbitControls(camera, container);

                        // axes
                        scene.add(new THREE.AxisHelper(20));

                        // geometry
                        var geometry = new THREE.CubeGeometry(2, 2, 2);

                        // material
                        /*
                        var material = new THREE.MeshBasicMaterial({
                            color: 0xffffff,
                            wireframe: true
                        });
                        */
                        // parent
                        parent = new THREE.Object3D();
                        scene.add(parent);

/*
                        leg = function(pos) {
                            legGeo = new THREE.CubeGeometry(1, 1, 4);
                            legMaterial = new THREE.MeshBasicMaterial({
                                color: 0xff0000,
                                wireframe: false
                            });

                            l = new THREE.Object3D();
                            l.position.x = pos.x;
                            l.position.y = pos.y;
                            l.position.z = pos.z;
                            //l.rotation.x = 5;
                            parent.add(l);

                            m = new THREE.Mesh(legGeo, legMaterial);
                            if (pos.z == 2.5)
                                m.position.z = 2;
                            else
                                m.position.z = -2;
                                l.add(m);

                            return l;
                        };
*/

                        cube = function(w, l, h, parent, color) {
                            color = color || 0x000000;
                            geo = new THREE.CubeGeometry(w, l, h);
                            mat = new THREE.MeshBasicMaterial({
                                color: color,
                                wireframe: false
                            });

                            obj = new THREE.Object3D();
                            parent.add(obj);
                            mesh = new THREE.Mesh(geo, mat);
                            obj.add(mesh)

                            //obj.add(new THREE.AxisHelper(20));

                            return { obj: obj, mesh: mesh };
                        };

                        cad = (function() {
                            return {

                                body: (function() {

                                    return {
                                        make: function() {
                                            geo = new THREE.CubeGeometry(15, 1, 6);
                                            material = new THREE.MeshLambertMaterial({ color: 'blue', ambient: 'blue' });
                                            
                                            /*
                                            material = new THREE.MeshBasicMaterial({
                                                color: 0x00ff00,
                                                wireframe: false
                                            });
                                            */

                                            obj = new THREE.Object3D();
                                            parent.add(obj)

                                            mesh = new THREE.Mesh(geo, material);
                                            obj.add(mesh)

                                            return obj;
                                        },

                                        create: function() {
                                            obj = this.make();
                                            obj.position.y = 0.5;
                                            //obj = this.make();
                                            //obj.position.y = 1;
                                        }
                                    };

                                })(),

                                leg: (function() {

                                    return {

                                        positions: [
                                            [ -5, 0.5, 3, 0 ],
                                            [ -5, 0.5, -3, Math.PI ],

                                            [ 0, 0.5, 3, 0 ],
                                            [ 0, 0.5, -3, Math.PI ],

                                            [ 5, 0.5, 3, 0 ],
                                            [ 5, 0.5, -3, Math.PI ],
                                        ],

                                        data: {},

                                        get: function(index) {
                                            return this.data[index];
                                        },

                                        create: function(index, x, y, z, value) {
                                            armh = cube(0.5, 0.5, 3, parent, 'yellow');
                                            armh.obj.position.x = x;
                                            armh.obj.position.y = y;
                                            armh.obj.position.z = z;

                                            armh.obj.rotation.y = value;
                                            armh.mesh.position.z = 1.5;
                                            this.data[index] = armh;

                                            armv = cube(0.5, 0.5, 3, armh.mesh, 'green');
                                            armv.obj.position.y = 0;
                                            armv.obj.position.z = 1.5;
                                            armv.mesh.position.z = 1.5;

                                            this.data[index + 6] = armv;
                                        },

                                        createAll: function() {
                                            for (i = 0; i < this.positions.length; i++) {
                                                position = this.positions[i];
                                                this.create(i, position[0], position[1], position[2], position[3]);
                                            }
                                        },

                                        setV: function(index, value) {
                                            if (!(index % 1)) {
                                                value += Math.PI;
                                            }
                                            this.data[index].obj.rotation.x = value;
                                        },

                                        setH: function(index, value) {
                                            if (index % 2) {
                                                value -= Math.PI;
                                            }
                                            this.data[index].obj.rotation.y = value;
                                        },

                                        setColor: function(index, color) {
                                            this.data[index].mesh.material.color.setHex(color);
                                        }
                                    }
                                })(),

                            }
                        })();

                        cad.body.create();
                        cad.leg.createAll();
/*
                        leg.create(0, 5, 0, 3, 0);
                        leg.create(1, 0, 0, 3, 0);
                        data = leg.create(2, -5, 0, 3, 0);
                        A = data[0];
                        AA = data[1];

                        leg.create(3, -5, 0, -3, Math.PI);
                        leg.create(4, 0, 0, -3, Math.PI);
                        leg.create(5, 5, 0, -3, Math.PI);
*/

/*
                        A = cube(0.5, 0.5, 3, parent);
                        A[0].position.x = -5;
                        A[0].position.y = 0;
                        A[0].position.z = 3;
                        A[1].position.z = 1.5;

                        AA = cube(0.5, 0.5, 3, A[1]);
                        AA[0].position.y = 0;
                        AA[0].position.z = 1.5;
                        AA[1].position.z = 1.5;
*/


/*
                        this.legs[0] = leg({ x: -3, y: 0, z: 2.5 });
                        this.legs[1] = leg({ x: 0, y: 0, z: 2.5 });
                        this.legs[2] = leg({ x: 3, y: 0, z: 2.5 });

                        this.legs[3] = leg({ x: -3, y: 0, z: -2.5 });
                        this.legs[4] = leg({ x: 0, y: 0, z: -2.5 });
                        this.legs[5] = leg({ x: 3, y: 0, z: -2.5 });
*/
                    },

                    setServoPosition: function(servo, value) {

                        // 0    -> -0.5
                        // 128  -> 0
                        // 255  -> 0.5

                        // 0


                        //console.info("Coeff: " + coeff);

                        // Horizontal
                        if (servo <= 5) {
                            //B.simulation.legs[i].rotation.y = coeff * json.data.servos[i] - 1.4/2;
                            max = 0.9;
                            coeff = max / 128;

                            //console.info("Servo:" + servo + ", value: " + value + ", sent:" + (coeff * value - max));
                            value = coeff * value - max;

                            if (servo % 2) {
                                value = -value;
                            }

                            //if (servo == 1)
                            console.info('h:' + servo + '=' + value);
                            cad.leg.setH(servo, value);
                        } else {
                            //value = -value;
                            max = 1;
                            coeff = max / 128;

                            //console.info("Servo:" + servo + ", value: " + value + ", sent:" + (coeff * value - max));
                            value = coeff * value + max + 2;
                            cad.leg.setV(servo, value);
                            //B.simulation.legs[i - 3].rotation.x = coeff * json.data.servos[i] - 1.4/2;
                        }
                    },

                    animate: function() {

                        requestAnimationFrame(B.simulation.animate);

                        controls.update();

                        renderer.render(scene, camera);
                    }
                };

            })()
        }

        return self;
    };

})(this, jQuery);

$(document).ready(function(event) {

    $('.slide.trim').slider({
        min: -20,
        max: 20,
        step: 1,
        value: 0,
        orientation: 'horizontal',
        slide: function(ev, ui) {
            servo = $(this).data('servo');
            value = ui.value;

            if (servo % 2) {
                value = -value;
            }

            B.sendCmd(B.CMD_SET, { type: 'trim', servo: servo, value: value });
        }
    }).dblclick(function() {
        $(this).slider('value', 0);
        B.sendCmd(B.CMD_SET, { type: 'position', servo: $(this).data('servo'), value: 0 });
    });

    $('.slide.move').slider({
        min: 0,
        max: 254,
        step: 1,
        value: 128,
        orientation: 'horizontal',
        slide: function(ev, ui) {
            servo = $(this).data('servo');
            value = ui.value;

            console.info('before:' + value);
            if (servo == 3 || servo == 10 || servo == 5) {
                if (value > 128) {
                    offset = value - 128;
                    value = 128 - offset;
                } else {
                    offset = 128 - value;
                    value = 128 + offset;
                }
            }
            console.info('after:' + value);

            B.sendCmd(B.CMD_SET, { type: 'position', servo: servo, value: value });
        }
    }).dblclick(function() {
        $(this).slider('value', 128);
        B.sendCmd(B.CMD_SET, { type: 'position', servo: $(this).data('servo'), value: 128 });
    });

    $('.slide.limit').slider({
        range: true,
        min: 0,
        max: 254,
        step: 1,
        values: [ 64, 192 ],
        orientation: 'horizontal',
        slide: function(ev, ui) {
            B.sendCmd(B.CMD_SET, { type: 'limit', servo: $(this).data('servo'), min: ui.values[0], max: ui.values[1] });
        }
    }).dblclick(function() {
        $(this).slider('values', [ 64, 192 ]);
        //B.sendCmd(B.CMD_SET, { type: 'position', servo: $(this).data('servo'), value: 0 });
    });

/*
    $('.slider.trimh').slider({
        min: -20,
        max: 20,
        step: 2,
        value: 0,
        orientation: 'horizontal',
        formater: function(value) {
            return 'Servo ' + $(this.element).data('servo') + ' : ' + value;
        }
    }).on('slide', function(ev) {
        B.sendCmd(B.CMD_SET, { type: 'trim', servo: $(ev.currentTarget).data('servo'), value: ev.value });
    });

    $('.slider.trimv').slider({
        min: -20,
        max: 20,
        step: 2,
        value: 0,
        orientation: 'vertical',
        formater: function(value) {
            return 'Servo ' + $(this.element).data('servo') + ' : ' + value;
        }
    }).on('slide', function(ev) {
        B.sendCmd(B.CMD_SET, { type: 'trim', servo: $(ev.currentTarget).data('servo'), value: ev.value });
    });

    $('.slider.moveh').slider({
        min: 0,
        max: 255,
        step: 1,
        value: 128,
        orientation: 'horizontal',
        formater: function(value) {
            return 'Servo ' + $(this.element).data('servo') + ' : ' + value;
        }
    }).on('slide', function(ev) {
        B.sendCmd(B.CMD_SET, { type: 'position', servo: $(ev.currentTarget).data('servo'), value: ev.value });
    });

    $('.slider.movev').slider({
        min: 0,
        max: 255,
        step: 1,
        value: 128,
        orientation: 'vertical',
        formater: function(value) {
            return 'Servo ' + $(this.element).data('servo') + ' : ' + value;
        }
    }).on('slide', function(ev) {
        B.sendCmd(B.CMD_SET, { type: 'position', servo: $(ev.currentTarget).data('servo'), value: ev.value });
    });

    $('.slider.limith').slider({
        min: 0,
        max: 254,
        step: 1,
        value: [ 64, 192 ],
        orientation: 'horizontal',
        formater: function(value) {
            return 'Servo ' + $(this.element).data('servo') + ' : ' + value;
        }
    }).on('slide', function(ev) {
        B.sendCmd(B.CMD_SET, { type: 'limit', servo: $(ev.currentTarget).data('servo'), min: ev.value[0], max: ev.value[1] });
    });

    $('.slider.limitv').slider({
        min: 0,
        max: 254,
        step: 1,
        value: [ 64, 192 ],
        orientation: 'vertical',
        formater: function(value) {
            return 'Servo ' + $(this.element).data('servo') + ' : ' + value;
        }
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
*/

    $('#tab').tab();

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
                B.sendCmd(B.CMD_CONFIG, { action: 'save' });
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
        B.sendCmd(B.CMD_SERVO, { name: $(this).data('command') });
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

