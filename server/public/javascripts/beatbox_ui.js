var socket = io.connect();
var serverErrorTimer;
$(document).ready(function() {
	sendRequest('update');
	
	var requestTimer = setInterval(function() {
		socket.emit('uptime', '');
		sendRequest('update');
	}, 1000);
	
	socket.on('errorReply', function(result) {
		clearTimeout(serverErrorTimer);
		$('#error-text').text(result.toString('utf8'));
		$('#error-box').show();
		var errorTimer = setTimeout(function() {
			$('#error-box').hide();
		}, 10000);
	});

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
	
	socket.on('heartBeat', function(result) {
		clearTimeout(serverErrorTimer);
		sendRequest('vol');
		sendRequest('tempo');
		sendRequest('beat');
	});
	
	socket.on('beatReply', function(result) {
		clearTimeout(serverErrorTimer);
		var res = result[result.length-1];
		if(res == '0') {
			$('#modeid').html("None");
		} else if (res == '1') {
			$('#modeid').html("Beat #1");
		} else if (res == '2') {
			$('#modeid').html("Beat #2");
		}
	});
	

	socket.on('tempoReply', function(result) {
		clearTimeout(serverErrorTimer);
		$('#tempoId').val(result);
	});
	
	socket.on('volumeReply', function(result) {
		clearTimeout(serverErrorTimer);
		$('#volumeid').val(result);
	});
	
	$('#modeNone').click(function() {
		sendRequest('beatnone');
	});
	
	$('#modeRock1').click(function() {
		sendRequest('beatone');
	});
	
	$('#modeRock2').click(function() {
		sendRequest('beattwo');
	});
	
	$('#volumeDown').click(function() {
		sendRequest('vdown');
	});
	
	$('#volumeUp').click(function() {
		sendRequest('vup');
	});
	
	$('#tempoDown').click(function() {
		sendRequest('tdown');
	});
	
	$('#tempoUp').click(function() {
		sendRequest('tup');
	});

	$('#bass').click(function() {
		sendRequest('bass');
	});

	$('#snare').click(function() {
		sendRequest('snare');
	});

	$('#hihat').click(function() {
		sendRequest('hihat');
	});
});


function sendRequest(cmd) {
	console.log("send request");
	socket.emit('command', cmd);
	if(serverErrorTimer) {
		clearTimeout(serverErrorTimer);
	} 
	serverErrorTimer = setTimeout(function() {
	$('#error-box').show();
	$('#error-text').text('Server Error: No response received from server. Is it running?');
		var errorTimer = setTimeout(function() {
			$('#error-box').hide();
		}, 10000);
	}, 7000);
}

