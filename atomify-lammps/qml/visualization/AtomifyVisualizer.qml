import QtQuick 2.5
import QtQuick.Controls 1.4
import Atomify 1.0
import SimVis 1.0

Item {
    id: atomifyVisualizerRoot
    property Visualizer visualizer: visualizer
    property AtomifySimulator simulator
    property real scale: 0.23
    property Light light: light
    property Slice slice: mySlice
    property Camera camera: camera
    property bool addPeriodicCopies: false

    Visualizer {
        id: visualizer
        property Spheres spheres: spheres
        width: parent.width
        height: parent.height
        simulator: atomifyVisualizerRoot.simulator
        camera: camera
        backgroundColor: "#111"
        navigator: navigator
        onTouched: {
            atomifyVisualizerRoot.focus = true
        }

//        SkyBox {
//            id: skybox
//            camera: camera
//            texture: ":/1024.png"
//        }

        TrackballNavigator {
            id: navigator
            anchors.fill: parent
            camera: camera
        }
//        FlyModeNavigator {
//            id: navigator
//            anchors.fill: parent
//            camera: camera
//        }

        Spheres {
            id: spheres
            scale: atomifyVisualizerRoot.scale
            color: "white"
            visible: parent.visible

            Light {
                id: light
                ambientColor: spheres.color
                specularColor: "white"
                diffuseColor: spheres.color
                ambientIntensity: 0.05
                diffuseIntensity: 1.0
                specularIntensity: 2.0
                specular: true
                shininess: 30.0
                attenuation: 0.1/(Math.pow(simulator.cameraToSystemCenterDistance,1.75))
                position: camera.position
            }

            PeriodicCopies {
                id: periodicCopies
                systemSize: simulator.systemSize
                enabled: addPeriodicCopies
            }

            Slice {
                id: mySlice
                systemSize: simulator.systemSize
                origo: Qt.vector3d(0,0,0)
                distance: 0
                normal: Qt.vector3d(1.0, 0.0, 0.0)
                width: 5
                enabled: false
            }

//            SkyBoxReflection {
//                id: reflection
//                skybox: skybox
//                reflectivity: 0.2
//            }
        }
    }

    Camera {
        id: camera
        nearPlane: 0.1
        farPlane: 100000
    }
}
