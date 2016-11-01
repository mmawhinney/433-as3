"use strict";

var socketio = require('socket.io');
var io;

var dgram = require('dgram');

exports.listen = function(server) {
	io = socketio.listen(server);

	io.sockets.on('connection', function(socket) {
		handleCommand(socket);
	});
};

function handleCommand(socket) {
	// Pased string of comamnd to relay	
	var fs = require('fs');
	socket.on('uptime', function(data) {
		var path = "/proc/uptime";
		var message;
		fs.readFile(path, function(err, data) {
			if (err) {
				message = "Connection lost";
			} else {
				message = data.toString('utf8');
			}
			socket.emit('uptimeReply', message);
		});
	});

	socket.on('command', function(data) {
		console.log('udp command: ' + data);

		// Info for connecting to the local process via UDP
		var PORT = 12345;
		var HOST = '127.0.0.1';
		if (data != null) {
			var buffer = new Buffer(data);

			var client = dgram.createSocket('udp4');
			client.send(buffer, 0, buffer.length, PORT, HOST, function(err,
					bytes) {
				if (err)
					throw err;
				console.log('UDP message sent to ' + HOST + ':' + PORT);
			});

			client.on('listening', function() {
				var address = client.address();
				console.log('UDP Client: listening on ' + address.address + ":"
						+ address.port);
			});

			var errorTimer = setTimeout(function() {
				socket.emit('errorReply', 'Application Error: No response recieved from beatbox. Is it running?');
			}, 4000);

			// Handle an incoming message over the UDP from the local
			// application.
			client.on('message', function(message, remote) {
				clearTimeout(errorTimer);
				console.log("UDP Client: message Rx" + remote.address + ':'
						+ remote.port + ' - ' + message);

				var reply = message.toString('utf8')
				if (data == 'beatnone' || data == 'beatone' || data == 'beattwo' || data == 'beat') {
					socket.emit('beatReply', reply);
				} else if (data == 'vup' || data == 'vdown' || data == 'vol') {
					socket.emit('volumeReply', reply);
				} else if (data == 'tup' || data == 'tdown' || data == 'tempo') {
					socket.emit('tempoReply', reply);
				} else if (data == 'update') {
					socket.emit('heartBeat');
				} else {
					socket.emit('commandReply', reply);
				}

				client.close();

			});
			client.on("UDP Client: close", function() {
				console.log("closed");
			});
			client.on("UDP Client: error", function(err) {
				console.log("error: ", err);
			});
		}
	});

};