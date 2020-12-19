import * as THREE from './modules/three.module.js';
import { GLTFLoader } from './modules/GLTFLoader.js';
import { GUI } from './modules/dat.gui.module.js';

var scene, renderer, camera;
var modelID;
var modelLoaded;
var rotations;
var gui;

var use_warning = false;

var joints;

var params = {
    frame: 0,
    apply: function() { exportRotations() },
    auto_assign: function() { automaticJointMap() },
}


var joint_maps = {
    neck: "",
    left_shoulder: "mixamorigLeftShoulder",
    right_shoulder: "mixamorigRightShoulder",
    left_arm: "mixamorigLeftForeArm",
    right_arm: "mixamorigRightForeArm",
    left_leg: "mixamorigLeftUpLeg",
    right_leg: "mixamorigRightUpLeg",
    left_knee: "mixamorigLeftLeg",
    right_knee: "mixamorigRightLeg"
};


var joint_base = {
    neck: [],
    left_shoulder: [],
    right_shoulder: [],
    left_arm: [],
    right_arm: [],
    left_leg: [],
    right_leg: [],
    left_knee: [],
    right_knee: []
};

var direction_maps = {
    neck: "cw",
    left_shoulder: "cw",
    right_shoulder: "cw",
    left_arm: "ccw",
    right_arm: "cw",
    left_leg: "ccw",
    right_leg: "cw",
    left_knee: "cw",
    right_knee: "ccw"
};



function characterInit( sceneRef, rendererRef, cameraRef, rotationsRef, guiRef ) {
    scene = sceneRef;
    renderer = rendererRef;
    camera = cameraRef;
    rotations = rotationsRef;
    gui = guiRef;
}

async function loadModel( modelURL ) {
    const loader = new GLTFLoader();
    var p = new Promise((resolve, reject) => {
        loader.load( modelURL, function ( gltf ) {

            modelID = gltf.scene;


            // model specific params
            modelID.scale.x = 300;
            modelID.scale.y = 300;
            modelID.scale.z = 300;
            modelID.position.y = -300;

            scene.add( modelID );

            modelLoaded = true;

            joints = searchJoints(modelID, []);

            getRotations();
                    
            gui.add(params, 'frame', 0, rotations["frames"].length-1).listen().onChange(function() { applyRotations(params.frame) });
            gui.add(params, 'apply');

            var joint_mapping = gui.addFolder('Joint Mapping');
            joint_mapping.add(params, 'auto_assign');
            for (var key in joint_maps) {
                joint_mapping.add( joint_maps, key , joints).listen();
            }
            joint_mapping.open();

            var direction_mapping = gui.addFolder('Direction Mapping');
            for (var key in direction_maps) {
                direction_mapping.add( direction_maps, key , ["cw", "ccw"]).listen();
            }
            direction_mapping.open();

        } );

  
      } );

}


function automaticJointMap() {
    for (var key in joint_maps) {
        var words = key.split("_");
        var relevant_joints = joints;
        var potential_joints = [];
        
        for(var i = 0; i < words.length; i++) {
            var word = words[i]
            for(var j = 0; j < relevant_joints.length; j++) {
                var joint = relevant_joints[j];
                if( joint.toLowerCase().includes(word) ) {
                    potential_joints.push(joint)
                }
            }
            relevant_joints = potential_joints;
            potential_joints = [];
        }

        if(relevant_joints.length > 0) {
            joint_maps[key] = relevant_joints[0];
        }

    }
}


function exportRotations() {

    if(use_warning) {
        for (var key in joint_maps) {
            if(joint_maps[key] == "") {
                alert("Must assign JointID to each Joint Map")
                return;
            }
        }
    }

    for (var frame = 0; frame < rotations["frames"].length; frame++) {
        applyRotations(frame);
        //saveFrame(frame);
    }
    params.frame = rotations["frames"].length-1;

}


function getRotations() {

    for (var key in joint_maps) {
        var joint = modelID.getObjectByName( joint_maps[key] );
        if(joint) {
            joint_base[key].push(joint.rotation.x)
            joint_base[key].push(joint.rotation.y)
            joint_base[key].push(joint.rotation.z)
        }
    }
    
}


function applyRotations(frame) {

    frame = parseInt(frame)

    for (var joint in rotations["frames"][frame]) {
        var r = rotations["frames"][frame][joint].slice();

        if(direction_maps[joint] == "ccw") {
            r[0] *= -1;
            r[1] *= -1;
            r[2] *= -1;
        }

        rotateJoint(joint_maps[joint], joint, r)
    }

}



function rotateJoint( jointName, key, r ) {

    var joint = modelID.getObjectByName( jointName );
    if(joint) {
        var r0 = joint_base[key];
        joint.rotation.set(r0[0] + r[0], r0[1] + r[1], r0[2] + r[2]);
        return true;
    }
}

function searchJoints( cur, list ) {
    if(cur.type == "Bone") {
       list.push(cur.name);
    }
    for(var i = 0; i < Object.keys(cur.children).length; i++) {
        list = searchJoints( cur.children[i], list);
    }
    return list;
}






function saveFrame(frame) {

    renderer.render( scene, camera );

    var strMime = "image/png";
    var strDownloadMime = "image/octet-stream";
    var imgData = renderer.domElement.toDataURL(strMime);

    saveFile(imgData.replace(strMime, strDownloadMime), "frame_" + frame.toString() + ".png");

}

// https://stackoverflow.com/questions/26193702/three-js-how-can-i-make-a-2d-snapshot-of-a-scene-as-a-jpg-image
function saveFile(strData, filename) {
    var link = document.createElement('a');
    if (typeof link.download === 'string') {
        document.body.appendChild(link);
        link.download = filename;
        link.href = strData;
        link.click();
        document.body.removeChild(link);
    } else {
        location.replace(uri);
    }
}


export { characterInit, loadModel };