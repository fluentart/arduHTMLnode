var SerialPort = require("serialport"); // so we can access the serial port

//LIST DEVICES
SerialPort.list( function (err, ports) {
	for (var num in ports) {
		console.log(ports[num])
	}
});

//CONNECT
var arduino = new SerialPort.SerialPort('/dev/cu.usbmodem1411', {baudrate: 9600}); //you must set the port and baudrate

arduino.on('data', function(datain) {
	console.log(datain.toString());
})
