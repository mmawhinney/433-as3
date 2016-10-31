#include "udp.h"
#include "control.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>
#include <linux/kernel.h>
#include <sys/sysinfo.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>
#include <pthread.h>

// Taking from guide provided in student links
#define BUFLEN 512
#define NPACK 10
#define SRV_IP "0.0.0.0"

_Bool isListening = false;
pthread_t udpThreadId;
int sock_port, *sock;
struct sockaddr_in si_me, si_other;

void udp_err(char *err) {
	perror(err);
	exit(1);
}

// Start UDP socket on given port
void udp_startSocket(int port) {
	sock_port = port;
	isListening = true;

	pthread_create(&udpThreadId, NULL, udp_socket, NULL);
}

// UDP Socket to handle calls
void* udp_socket(void* arg) {

	// Init variables
	sock = malloc(sizeof(int));
	socklen_t slen = sizeof(si_other);
	char buf[BUFLEN];

	// Create a socket
	if ((*sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
		udp_err("Socket");
	}

	memset((char *) &si_me, 0, sizeof(si_me));
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(sock_port);
	si_me.sin_addr.s_addr = INADDR_ANY;

	if (bind(*sock, (struct sockaddr *) &si_me, sizeof(si_me)) == -1) {
		udp_err("Bind");
	}

	// Listen for calls
	while (isListening) {
		if (recvfrom(*sock, buf, BUFLEN, 0, (struct sockaddr *) &si_other,
				&slen) == -1) {
			udp_err("RecvFrom");
		}
		if (strcmp(buf, "") != 0) {
			//trim string
			char* command = trimStr(buf);
			printf("%s\n", command);
			// process command
			udp_command(command);

			// clear buffer
			memset(buf, 0, sizeof(buf));
		}
	}
	// Exit nicely
	pthread_exit(0);
}

// Process what command was sent
void udp_command(char* cmd) {
	if (strncmp(cmd, "uptime", 6) == 0) {
		udp_uptime();
	} else if (strncmp(cmd, "vdown", 5) == 0) {
		udp_volume_down();
	} else if (strncmp(cmd, "vup", 3) == 0) {
		udp_volume_up();
	} else if (strncmp(cmd, "tdown", 5) == 0) {
		udp_tempo_down();
	} else if (strncmp(cmd, "tup", 3) == 0) {
		udp_tempo_up();
	} else if (strncmp(cmd, "beatnone", 8) == 0) {
		udp_play_beat(0);
	} else if (strncmp(cmd, "beatone", 7) == 0) {
		udp_play_beat(1);
	} else if (strncmp(cmd, "beattwo", 7) == 0) {
		udp_play_beat(2);
	}
//		udp_count();
//		// if the command contains get
//	} else if (strstr(cmd, "get") != NULL) {
//		char* separate;
//		// check if first word is get
//		separate = strtok(cmd, " ");
//		if (strncmp(separate, "get", 3) == 0) {
//			separate = strtok(NULL, " ");
//			// next one should be an int
//			if (separate != NULL)
//				udp_get(atoi(separate));
//			else
//				udp_error(0);
//		} else {
//			udp_error(0);
//		}
//	} else if (strstr(cmd, "first") != NULL) {
//		char* separate;
//		// check if first word is first
//		separate = strtok(cmd, " ");
//		if (strncmp(separate, "first", 5) == 0) {
//			separate = strtok(NULL, " ");
//			// next one should be an int
//			if (separate != NULL)
//				udp_first(atoi(separate));
//			else
//				udp_first(1);
//		} else {
//			udp_error(0);
//		}
//	} else if (strstr(cmd, "last") != NULL) {
//		char* separate;
//		// check if first word is last
//		separate = strtok(cmd, " ");
//		if (strncmp(separate, "last", 4) == 0) {
//			separate = strtok(NULL, " ");
//			// next one should be an int
//			if (separate != NULL) {
//				udp_last(atoi(separate));
//			} else
//				udp_last(1);
//		} else {
//			udp_error(0);
//		}
//	} else if (strcmp(cmd, "stop") == 0) {
//		udp_stop();
//	} else {
//		udp_error(0);
//	}
}

void udp_volume_up() {
	increaseVolume();
}

void udp_volume_down() {
	decreaseVolume();
}

void udp_tempo_up() {
	increaseTempo();
}

void udp_tempo_down() {
	decreaseTempo();
}

void udp_play_beat(int beat) {
	setBeat(beat);
	char res[BUFLEN];
	sprintf(res, "beat%d", beat);
	udp_sendback(res);
}

void udp_uptime() {
	struct sysinfo s_info;
	int error = sysinfo(&s_info);
	if (error != 0) {
		printf("code error = %d\n", error);
	}
	char res[BUFLEN];
	sprintf(res, "%ld", s_info.uptime);
	udp_sendback(res);
}

// ABORT!
void udp_stop(void) {
	udp_sendback("Stopping program...\n");
	isListening = false;
	pthread_join(udpThreadId, NULL);
	close(*sock);
	free(sock);
}

// Decide between errroring unknown commands or out of bounds
void udp_error(int opt) {
	switch (opt) {
	case 0: {
		udp_sendback("error\n");
		break;
	}
	}
}

// Send UDP packet back to caller
void udp_sendback(char* res) {
	socklen_t slen = sizeof(si_other);

	if (sendto(*sock, res, strlen(res), 0, (struct sockaddr *) &si_other, slen)
			== -1) {
		udp_err("sendto\n");
	}
}

// stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way
char* trimStr(char* str) {
	char *end;

	while (isspace(*str))
		str++;

	if (*str == 0)
		return str;

	end = str + strlen(str) - 1;
	while (end > str && isspace(*end))
		end--;
	*(end + 1) = 0;
	return str;
}
