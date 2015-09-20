
var app = {
    // Application Constructor
    initialize: function() {
        this.bindEvents();
        $("#data").text("ready");
        console.log("more console.log");
    },
    // Bind Event Listeners
    //
    // Bind any events that are required on startup. Common events are:
    // 'load', 'deviceready', 'offline', and 'online'.
    bindEvents: function() {
        document.addEventListener('deviceready', this.onDeviceReady, false);
    },
    // deviceready Event Handler
    //
    // The scope of 'this' is the event. In order to call the 'receivedEvent'
    // function, we must explicitly call 'app.receivedEvent(...);'
    onDeviceReady: function() {
        app.receivedEvent('deviceready');
        console.log(navigator.camera);
        console.log("something");
        
    },
    
    // Update DOM on a Received Event
    receivedEvent: function(id) {
        var parentElement = document.getElementById(id);
        var listeningElement = parentElement.querySelector('.listening');
        var receivedElement = parentElement.querySelector('.received');

        listeningElement.setAttribute('style', 'display:none;');
        receivedElement.setAttribute('style', 'display:block;');
        $("#data").text("readyssss");
       // alert(navigator.accelerometer);
        var options = { frequency: 20 }; 
        navigator.accelerometer.watchAcceleration(onSuccess, onError, options);
        $("#data").context = "Hello";
        
        
        function onSuccess(acceleration) {
            //console.log("asd");
            $("#data").text("x: " + acceleration.x + " y: " 
                        + acceleration.y + " z: " 
                        + acceleration.z);
        };
        function onError() {
            alert('onError!');
        };
        
        //brightness plug-in starts here
        function setBrightness(value) {
            console.log("setbr");
            
			// value should be float in range from 0 to 1.
            $("#fakeConsole").text("BRIGHT  " + brightness);
			brightness.setBrightness(value, win, fail);
            
		}
		function getBrightness() {
			// win([-1,0-1]) float 0-1 is a brightness level, -1 represents a system default
			brightness.getBrightness( win, fail);
		}
		function win(status) {
			alert('Message: ' + status);
		}
		function fail(status) {
			alert('Error: ' + status);
		}
        window.addEventListener("volumebuttonslistener", onVolumeButtonsListener, false);

        function onVolumeButtonsListener(info){
            console.log("Button pressed: " + info.signal);
        }
        
        $("#fakeConsole").text("fake  ");
        //window.brightness = cordova.require("cordova.plugin.Brightness.Brightness");
        $("#fakeConsole").text("fakerer ");
        //setBrightness(1);
        $("#fakeConsole").text("really fake ");
        console.log("something");
        
        //brightness.setKeepScreenOn(true);       //so screens can't sleep
        console.log('Received Event: ' + id);
        
        

            
    }
};

//camera quick start server

/*
cordova.plugins.CameraServer.startServer({
    'www_root' : '/',
    'port' : 8080,
    'localhost_only' : false,
    'json_info': []
}, function( url ){
    // if server is up, it will return the url of http://<server ip>:port/
    // the ip is the active network connection
    // if no wifi or no cell, "127.0.0.1" will be returned.
    console.log('CameraServer Started @ ' + url); 
}, function( error ){
    console.log('CameraServer Start failed: ' + error);
});

//camera capture code
cordova.plugins.CameraServer.startCamera(function(){    
      console.log('Capture Started');
  },function( error ){
      console.log('CameraServer StartCapture failed: ' + error);
  });
*/
