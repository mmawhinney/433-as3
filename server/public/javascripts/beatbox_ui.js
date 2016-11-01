var socket = io.connect();
$(document).ready(function() {
	sendRequest('beatone');
	sendRequest('vol');
	sendRequest('tempo');
	var requestTimer = setInterval(function() {
		sendRequest('uptime');
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
	
	socket.on('beatReply', function(result) {
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
		$('#tempoId').val(result);
	});
	
	socket.on('volumeReply', function(result) {
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
	socket.emit('command', cmd);
}

