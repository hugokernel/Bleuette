
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

                self.ws.onopen = function(){
                    $message.attr("class", 'label label-success');
                    $message.text('open');

                    B.sendCmd(B.CMD_SERVO, { action: 'get' });
                };

                self.ws.onmessage = function(ev){
                    json = JSON.parse(ev.data);
                    switch (json.type) {
                        case B.CMD_SERVO:
                            Events.emit('servo.received', json.data);
                            break;

                        case 'position':
                            Events.emit('position.received', json.data.servos);
                            break;
                        case 'sensors':
                            Events.emit('sensors.received', json.sensors);
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
                                    $.pnotify({
                                        title: 'Error !',
                                        text: '[' + json.data.levelname + '] ' + json.data.msg,
                                        type: 'error'
                                    });
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

                        // Value = 0, Skip !
                        if (!value) {
                            return;
                        }

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
                            if (cad) {
                                cad.leg.setH(servo, value);
                            }
                        } else {
                            //value = -value;
                            max = 1;
                            coeff = max / 128;

                            //console.info("Servo:" + servo + ", value: " + value + ", sent:" + (coeff * value - max));
                            value = coeff * value + max + 2;

                            if (cad) {
                                cad.leg.setV(servo, value);
                            }
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


