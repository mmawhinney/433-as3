var http = require('http');
var server = http.createServer(function(request, response) {
	var filePath = false;
	if(request.url == '/') {
		filePath = 'public/index.html';
	} else {
		filePath = 'public' + request.url;
	}
	var absPath = './' + filePath;
	serveStatic(response, absPath);
});

var PORT = 8088;
server.listen(PORT, function() {
	console.log("Server listening on port " + PORT);
});

var fs = require('fs');
function serveStatic(response, absPath) {
	fs.exists(absPath, function(exists) {
		if(exists) {
			fs.readFile(absPath, function(err, data) {
				if(err) {
					send404(response);
				} else {
					sendFile(response, absPath, data);
				}
			});
		} else {
			send404(response);
		}
	});
}

function send404(response) {
	response.writeHead(404, {'Content-Type': 'text/plain'});
	response.write('Error 404: resource not found');
	response.end();
}

var mime = require('mime');
var path = require('path');
function sendFile(response, filePath, fileContents) {
	response.writeHead(
		200,
		{"Content-Type": mime.lookup(path.basename(filePath))}
	);
	response.end(fileContents);
}

var udp = require('./lib/udp');
udp.listen(server);
