var SerialPort = require("serialport"); 
var scraper = require('json-scrape')(); 

SerialPort.list( function (err, ports) { console.log(ports); });

var arduino = new SerialPort.SerialPort('/dev/cu.usbmodem1411', {baudrate: 9600}); //you must set the port and baudrate

arduino.on('data', function (indata) {
	//console.log(indata.toString());
	scraper.write(indata.toString());
});

scraper.on('data', function (cleandata) {
	console.log(cleandata.g1x)		
});

/*
// TO SEND TO ARDUINO
// ideally this will be called from the browser event with socket io. 
// more in a later class on this.
	var data = { brightness: "0.1" }
	var test = JSON.stringify(data)    // turns into string to send
	console.log(test)
	arduino.write(test)
*/

