#include <pthread.h>

static pthread_mutex_t audioMutex = PTHREAD_MUTEX_INITIALIZER;

void lockAudio() {
	pthread_mutex_lock(&audioMutex);
}

void unlockAudio() {
	pthread_mutex_unlock(&audioMutex);
}

