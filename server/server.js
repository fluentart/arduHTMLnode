var config = {
  port: 8000,
  serialbaudrate: 115200
}

var express = require('express');
var app = express();

var serveStatic = require('serve-static');

app.use(serveStatic(__dirname + '/public', {'index': ['default.html', 'default.htm']}))

app.use(function(req, res, next){
  console.log(req.method + req.url);
  next();
});




app.get('/', function (req, res) {
  res.writeHead(200, {'Content-Type': 'text/html'});
  res.end('go to <a href="game.htm">index.htm</a>');
});


app.use(errorHandler);
  function errorHandler(err, req, res, next) {
  console.error(err.stack);
  res.status(500);
  res.send(500, 'Something broke! '+err);
}




var server = app.listen(config.port, function() {
	  console.log(server.address())
    console.log('Listening on port %d', server.address().port);
});


var io = require('socket.io').listen(server, {log: false});

io.sockets.on('connection', function (socket) {
	console.log("SOCKET New Connection")

  //talk
 socket.on("ourmessage", function (frombrowser) {
    console.log("New incoming message");    
    console.log(frombrowser);
    io.sockets.emit("messagefromserver", frombrowser)
  });

 socket.on("arducontrol", function (frombrowser) {
    console.log(frombrowser);
    frombrowser = JSON.stringify(frombrowser);
    arduino.write(frombrowser)
    console.log(frombrowser);
  });


});


/* ====================================== */

var SerialPort = require("serialport"); // so we can access the serial port
var scraper = require('json-scrape')(); // cleans messy serial messages.

var arduino; 

//AUTODETECT
SerialPort.list( function (err, ports) {
  for (var num in ports) {
    //console.log(ports[num])

    if (ports[num].manufacturer.slice(2,7) == "duino") {
      console.log("Arduino detected!");
      arduino = new SerialPort.SerialPort(ports[num].comName, {baudrate: config.serialbaudrate});
      arduConnect(arduino);
    }

  }
});



var arduConnect = function (device) {
  device.on("data", datahandler);
}

var datahandler = function (data) {
  scraper.write(data); 
}

scraper.on('data', function (data) {
  console.log(data);
  io.sockets.emit("arduino", data)
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







