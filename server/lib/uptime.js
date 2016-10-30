
var socketio = require('socket.io');
var io;

exports.listen = function(server) {
	io = socketio.listen(server);
	io.sockets.on('connection', function(socket) {
		handleCommand(socket);
	});
}

var fs = require('fs');
function handleCommand(socket) {
	socket.on('uptime', function(data) {
		var path = "/proc/uptime";
		var message;
		fs.readFile(path, function(err, data) {
			if(err) {
				message = "Connection lost";
			} else {
				message = data.toString('utf8');
			}
			socket.emit('uptimeReply', message);
		});
		
	});
}