## Connecting arduino to Node.js

Connecting [Arduino](http://www.arduino.cc/) and [node.js](https://nodejs.org/) is very powerful. It allows you do build internet connected electronics and html cross device interfaces that run in the browser. You can even build games using something like [three.js](http://threejs.org/) and control them with your own circuits!

Upload firmware to your arduino so long that prints information over serial.

Make sure you have [node.js](https://nodejs.org/) installed.

In your project folder:

    npm install serialport

This will download and compile the serialport node module. If this looks like it's not working you might have to [follow the instructions here](https://github.com/voodootikigod/node-serialport).

Let's test if it is working. Make a text file called **test.js** and paste this in there.

```
var SerialPort = require("serialport");

SerialPort.list( function (err, ports) {
  console.log(ports);
});
```

Using terminal in the same folder run it:

    node test.js

If its working correctly you should see that it detects your arduino as one of the devices.

![Imgur](http://i.imgur.com/rzzQL0n.png)

## Reading data

Add this line to your program. You'll have to make sure the first parameter has the correct comName. On windows it will be 'COM4' or similar.

    var arduino = new SerialPort.SerialPort('/dev/cu.usbmodem1411', {baudrate: 9600});

Add the event listener that will handle the incoming data.

```
arduino.on('data', function(datain) {
	console.log(datain.toString());
});
```

You'll see that the buffer does not handle end lines nicely. It's just a stream of raw data. What we'll need to do is parse the incoming stream and seperate out the data nicely.

## Using JSON

You'll want to change the output on your arduino to JSON format. So for instance if you want the value of A0 and A1 both:

```
Serial.print("{ \"A0\" : \"");
Serial.print(valueA0);
Serial.print("\", \"A1\" : \"");
Serial.print(valueA1);
Serial.print("\", \"A2\" : \"");
Serial.print(valueA2);
Serial.println("\"}");
```

The backslash **\** operator escapes the output and allows you to print a **"**

Your output should look something like this:

```
{"A0":"1023", "A1":"123", "A2":"422"}
{"A0":"1021", "A1":"125", "A2":"425"}
```

Then on the terminal install:

    npm install json-scrape

Use it like this:

```
var SerialPort = require("serialport");
var scraper = require('json-scrape')();

SerialPort.list( function (err, ports) { console.log(ports); });

var arduino = new SerialPort.SerialPort('/dev/cu.usbmodem1411', {baudrate: 9600}); //you must set the port and baudrate

arduino.on('data', function (indata) {
	//console.log(indata.toString());
	scraper.write(indata.toString());  
});

scraper.on('data', function (cleandata) {
	console.log(cleandata);
    //console.log(cleandata.A0);
});

```

## Writing to the arduino

You can use plain strings.

    arduino.write("hello");

But you'll probably want to send JSON data to the arduino and have it seperate the values for you. To do this it is best to install and use the [aJson library](https://github.com/interactive-matter/aJson).

```
#include <aJSON.h>
aJsonStream serial_stream(&Serial);

void setup(void) {
 Serial.begin(9600);
 pinMode(9, OUTPUT);
}

void loop(void) {

  //recieve commands
  while (serial_stream.available())
  {
    aJsonObject *msg = aJson.parse(&serial_stream);
    processMessage(msg);  //see api.ino
    aJson.deleteItem(msg);  
  }

 int sensorvalue = analogRead(A0);

 Serial.print("{\"light\" : \"");
 Serial.print(sensorvalue);
 Serial.println("\" }");

 delay(16); //just here to slow down the output so it is easier to read
}


/* ==================================================================

 Expects something like this over serial:

 {"led": "1"}

*/

void processMessage(aJsonObject *msg)
{

  aJsonObject *led = aJson.getObjectItem(msg, "led");
  if (led) {
    char* ledvaluestring = led->valuestring;
    float ledvaluefloat = atof(ledvaluestring);
    int ledvalueint = ledvaluefloat;
    digitalWrite(9, ledvalueint);
  }

}

```

Ofcourse to make all of this useful we'll need to have nodejs provide us with a nice user interface. Have a look at [express](http://expressjs.com/) and [socket.io](http://socket.io/) so long. Here is also nice working [server code](https://github.com/DigitalArtsWITS/2014IOT/blob/master/part2.2_arduino/_2_2_5_webardu/server.js) and [firmware](https://github.com/DigitalArtsWITS/2014IOT/blob/master/part2.2_arduino/_2_2_5_ardufirmware/arduinoNodeFirmware/arduinoNodeFirmware.ino) for you on github.
