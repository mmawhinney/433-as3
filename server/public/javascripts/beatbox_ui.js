var socket = io.connect();
$(document).ready(function() {
	sendRequest();
	var requestTimer = setInterval(function() {
		sendRequest();
	}, 1000);
	socket.on('uptimeReply', function(result) {
		var time = result.split(" ");
		var intTime = parseInt(time[0]);
		var days = intTime / 60 / 60 / 24;
		var hours = (intTime / 60 / 60) % 24;
		var minutes = (intTime / 60) % 60;
		var seconds = intTime % 60;
		var humanReadable = parseInt(hours)+":"+parseInt(minutes)+":"+parseInt(seconds)+" (H:M:S)";
		$('#status').html(humanReadable);
	});
});


function sendRequest() {
	socket.emit('uptime', 'test');
}
