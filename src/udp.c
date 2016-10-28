#include "udp.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
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
//	if (strncmp(cmd, "help", 4) == 0) {
//		udp_help();
//	} else if (strncmp(cmd, "count", 5) == 0) {
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

//// Print help screen
//void udp_help(void) {
//	// sendback
//	char* buf = "Accepted command examples:\n"
//			"count   -- show # primes found\n"
//			"get 10  -- display prime # 10.\n"
//			"last 2  -- display the last 2 primes found.\n"
//			"first 5 -- display the first 5 primes found.\n"
//			"stop    -- cause the server program to end.\n";
//	udp_sendback(buf);
//}
//
//// Returns count of primes
//void udp_count(void) {
//	char buffer[BUFLEN];
//	snprintf(buffer, sizeof(buffer), "Number of primes found = %d\n",
//			PrimeFinder_getNumPrimesFound());
//	udp_sendback(buffer);
//}
//
//// Get prime at index
//void udp_get(int num) {
//	// Index in array
//	int idx = (num - 1);
//	// Must fall between 0 and array size
//	if (idx > -1 && idx < primesFound) {
//		unsigned long long prime = PrimeFinder_getPrimeByIndex(idx);
//		char buffer[BUFLEN];
//		snprintf(buffer, sizeof(buffer), "Prime %d = %lld\n", num, prime);
//		udp_sendback(buffer);
//	} else {
//		udp_error(1);
//	}
//}
//
void udp_mode(int mode) {
}
void udp_volume_up() {
}
void udp_volume_down() {
}
void udp_tempo_up() {
}
void udp_tempo_down() {
}
void udp_play_beat(int beat) {
}
//// Get last n primes
//void udp_last(int num) {
//	int curPrimes = PrimeFinder_getNumPrimesFound() - 1;
//	if (num > curPrimes || num < 1) {
//		udp_error(1);
//	} else {
//		char buffer[BUFLEN * 2];
//		snprintf(buffer, sizeof(buffer), "Last %d primes = \n", num);
//		int printCount = 0;
//		if (num == 1) {
//			char buffy[BUFLEN];
//			snprintf(buffy, sizeof(buffy), "%lld\n",
//					PrimeFinder_getPrimeByIndex(curPrimes - num));
//			strcat(buffer, buffy);
//		} else {
//			char warning[BUFLEN];
//			if (num > 50) {
//				num = 50;
//				snprintf(warning, sizeof(warning),
//						"Only returned last %d primes due to memory constraints\n",
//						num);
//			}
//			for (int i = curPrimes - num; i < curPrimes; i++) {
//				// last elem in line
//				char buffy[BUFLEN];
//				if (printCount == 0) {
//					snprintf(buffy, sizeof(buffy), "%lld, ",
//							PrimeFinder_getPrimeByIndex(i));
//				} else if (printCount == num - 1) {
//					snprintf(buffy, sizeof(buffy), "%lld\n",
//							PrimeFinder_getPrimeByIndex(i));
//				} else if ((printCount + 1) % 4 == 0) {
//					snprintf(buffy, sizeof(buffy), "%lld,\n",
//							PrimeFinder_getPrimeByIndex(i));
//				} else {
//					snprintf(buffy, sizeof(buffy), "%lld, ",
//							PrimeFinder_getPrimeByIndex(i));
//				}
//				strcat(buffer, buffy);
//				printCount++;
//			}
//			if (strlen(warning) > 0)
//				strcat(buffer, warning);
//		}
//		udp_sendback(buffer);
//	}
//}
//
//// Get first n primes
//void udp_first(int num) {
//	int curPrimes = PrimeFinder_getNumPrimesFound() - 1;
//	if (num > curPrimes || num < 1) {
//		udp_error(1);
//	} else {
//		char buffer[BUFLEN * 2];
//		snprintf(buffer, sizeof(buffer), "First %d primes = \n", num);
//		int printCount = 0;
//		if (num == 1) {
//			char buffy[BUFLEN];
//			snprintf(buffy, sizeof(buffy), "%lld\n",
//					PrimeFinder_getPrimeByIndex(0));
//			strcat(buffer, buffy);
//		} else {
//			char warning[BUFLEN];
//			if (num > 50) {
//				num = 50;
//				snprintf(warning, sizeof(warning),
//						"Only returned first %d primes due to memory constraints\n",
//						num);
//			}
//			for (int i = 0; i < num; i++) {
//				// last elem in line
//				char buffy[BUFLEN];
//				if (printCount == 0) {
//					snprintf(buffy, sizeof(buffy), "%lld, ",
//							PrimeFinder_getPrimeByIndex(i));
//				} else if (printCount == num - 1) {
//					snprintf(buffy, sizeof(buffy), "%lld\n",
//							PrimeFinder_getPrimeByIndex(i));
//				} else if ((printCount + 1) % 4 == 0) {
//					snprintf(buffy, sizeof(buffy), "%lld,\n",
//							PrimeFinder_getPrimeByIndex(i));
//				} else {
//					snprintf(buffy, sizeof(buffy), "%lld, ",
//							PrimeFinder_getPrimeByIndex(i));
//				}
//				strcat(buffer, buffy);
//				printCount++;
//			}
//			if (strlen(warning) > 0)
//				strcat(buffer, warning);
//		}
//		udp_sendback(buffer);
//	}
//}

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
