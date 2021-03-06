cordova.define('cordova/plugin_list', function(require, exports, module) {
module.exports = [
    {
        "file": "plugins/cordova-plugin-camera/www/CameraConstants.js",
        "id": "cordova-plugin-camera.Camera",
        "clobbers": [
            "Camera"
        ]
    },
    {
        "file": "plugins/cordova-plugin-camera/www/CameraPopoverOptions.js",
        "id": "cordova-plugin-camera.CameraPopoverOptions",
        "clobbers": [
            "CameraPopoverOptions"
        ]
    },
    {
        "file": "plugins/cordova-plugin-camera/www/Camera.js",
        "id": "cordova-plugin-camera.camera",
        "clobbers": [
            "navigator.camera"
        ]
    },
    {
        "file": "plugins/cordova-plugin-camera/src/browser/CameraProxy.js",
        "id": "cordova-plugin-camera.CameraProxy",
        "runs": true
    },
    {
        "file": "plugins/cordova-plugin-device-motion/www/Acceleration.js",
        "id": "cordova-plugin-device-motion.Acceleration",
        "clobbers": [
            "Acceleration"
        ]
    },
    {
        "file": "plugins/cordova-plugin-device-motion/www/accelerometer.js",
        "id": "cordova-plugin-device-motion.accelerometer",
        "clobbers": [
            "navigator.accelerometer"
        ]
    },
    {
        "file": "plugins/cordova-plugin-device-motion/src/browser/AccelerometerProxy.js",
        "id": "cordova-plugin-device-motion.AccelerometerProxy",
        "runs": true
    },
    {
        "file": "plugins/cordova-plugin-device-motion/src/browser/accelerometer.js",
        "id": "cordova-plugin-device-motion.accelerometer",
        "merges": [
            "navigator.accelerometer"
        ]
    },
    {
        "file": "plugins/cordova-plugin-device-orientation/www/CompassError.js",
        "id": "cordova-plugin-device-orientation.CompassError",
        "clobbers": [
            "CompassError"
        ]
    },
    {
        "file": "plugins/cordova-plugin-device-orientation/www/CompassHeading.js",
        "id": "cordova-plugin-device-orientation.CompassHeading",
        "clobbers": [
            "CompassHeading"
        ]
    },
    {
        "file": "plugins/cordova-plugin-device-orientation/www/compass.js",
        "id": "cordova-plugin-device-orientation.compass",
        "clobbers": [
            "navigator.compass"
        ]
    },
    {
        "file": "plugins/cordova-plugin-device-orientation/src/browser/CompassProxy.js",
        "id": "cordova-plugin-device-orientation.CompassProxy",
        "runs": true
    },
    {
        "file": "plugins/cordova-plugin-device-orientation/src/browser/compass.js",
        "id": "cordova-plugin-device-orientation.compass",
        "merges": [
            "navigator.compass"
        ]
    },
    {
        "file": "plugins/com.moonware.cameraplus/www/CameraPlus.js",
        "id": "com.moonware.cameraplus.CameraPlus",
        "clobbers": [
            "cordova.plugins.CameraPlus"
        ]
    },
    {
        "file": "plugins/com.moonware.cameraserver/www/CameraServer.js",
        "id": "com.moonware.cameraserver.CameraServer",
        "clobbers": [
            "cordova.plugins.CameraServer"
        ]
    },
    {
        "file": "plugins/com.keith.cordova.plugin.canvascamera/www/CanvasCamera.js",
        "id": "com.keith.cordova.plugin.canvascamera.CanvasCamera",
        "clobbers": [
            "plugin.CanvasCamera",
            "CanvasCamera"
        ]
    },
    {
        "file": "plugins/com.manueldeveloper.volume-buttons/www/volumebuttons.js",
        "id": "com.manueldeveloper.volume-buttons.volumebuttons",
        "clobbers": [
            "navigator.volumebuttons"
        ]
    }
];
module.exports.metadata = 
// TOP OF METADATA
{
    "cordova-plugin-camera": "1.2.0",
    "cordova-plugin-device-motion": "1.1.1",
    "cordova-plugin-device-orientation": "1.0.1",
    "cordova-plugin-geolocation": "1.0.1",
    "cordova-plugin-whitelist": "1.1.0",
    "com.moonware.cameraplus": "0.2.2",
    "com.moonware.cameraserver": "0.2.0",
    "com.keith.cordova.plugin.canvascamera": "1.0.0dev",
    "com.manueldeveloper.volume-buttons": "0.0.1"
}
// BOTTOM OF METADATA
});