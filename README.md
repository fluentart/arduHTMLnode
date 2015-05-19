ARDUINO DUE GYRO TO 3D

Here I'll show you how to connect multiple i2c sensors to the browser for easy visualisation. The firmware contains custom i2c code to handle any pin on the DUE. I hope you find this useful!

![Imgur](http://i.imgur.com/45LmN5Y.jpg)

We connect two gyro sensors to two 3D cubes in the browser and control them in realtime.

## Download

Sourcecode: [https://github.com/fluentart/arduHTMLnode](https://github.com/fluentart/arduHTMLnode)

## How to 

![Imgur](http://i.imgur.com/5CQfA8g.jpg)

**Step1** Solder **SA0** to **GND** on your secondary gyro.

This will give it a different address on the I2C bus, avoiding conflict with the primary. You can only have two of these specific sensors on a single I2C bus, but you can have many I2C busses.

**Step2** Complete the wiring.

![Imgur](http://i.imgur.com/peCnty7.jpg)

```
Arduino pin 8 to IMU SCL (both of your gyro sensors)
Arduino pin 9 to IMU SDA (both of your gyro sensors)
Arduino GND to IMU GND (both of your gyro sensors)
Arduino 3.3V to IMU VIN (both of your gyro sensors)
4.7kOhm resistor connecting SCL and 3.3V
4.7kOhm resistor connecting SDA and 3.3V
```

The resistors can be as low as 1k Ohm, they are called pullup resistors in this case, and help to keep the data lines HIGH. This is part of I2C spec.

Any 5Volt on the VDD of your sensors will damage it.

**Step3** Upload **/arduino/due_i2c** sketch to your arduino.

**Step4** In the **/server** folder:

    npm install express
    npm install serve-static
    npm install socket.io
    npm install serialport
    npm install json-scrape


This will download and compile the serialport node module. Make sure you have [node.js](https://nodejs.org/) installed. If this looks like it's not working you might have to [follow the instructions here](https://github.com/voodootikigod/node-serialport).

**Step 5** RUN IT!

    node server.js

Open chrome on [http://localhost:8000/game.htm](http://localhost:8000/game.htm)
