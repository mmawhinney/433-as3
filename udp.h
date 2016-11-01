#ifndef UDP_H_
#define UDP_H_

#define BUFLEN 512

// Helper error function
void udp_err(char *err);

// Start UDP socket on given port
void udp_startSocket(int port);

// Threaded
void* udp_socket(void* arg);

// Process what command was sent
void udp_command(char* cmd);

void udp_volume_up();
void udp_volume_down();
void udp_tempo_up();
void udp_tempo_down();
void udp_play_beat(int beat);
void udp_uptime();
void udp_play_bass();
void udp_play_snare();
void udp_play_hihat();

// ABORT!
void udp_stop(void);

// Error helper
void udp_error(int opt);

// Sendback to netclient
void udp_sendback(char* res);

// Trim string
char* trimStr(char* str);

#endif /* UDP_H_ */
