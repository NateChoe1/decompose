#include <stdio.h>
#include <pulse/error.h>
#include <pulse/simple.h>

#include <sound.h>
#include <stdlib.h>
#include <config.h>

static pa_simple *createStream(char *progname, char *name,
                               int rate, int channels, int type) {
	pa_sample_spec spec = {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
		.format = PA_SAMPLE_S16LE,
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
		.format = PA_SAMPLE_S16BE,
#else
#error "__BYTE_ORDER__ can't be parsed."
#endif
		.rate = rate,
		.channels = channels,
	};
	int err;
	pa_simple *ret = pa_simple_new(NULL, progname, type, NULL, name,
	                               &spec, NULL, NULL, &err);
	if (ret == NULL) {
		fprintf(stderr, "pa_simple_new failed: %s\n", pa_strerror(err));
		exit(EXIT_FAILURE);
	}

	return ret;
}

Speaker createSpeaker(char *name, int rate, int channels) {
	return createStream(name, "playback",
	                    rate, channels, PA_STREAM_PLAYBACK);
}

Microphone createMicrophone(char *name, int rate, int channels) {
	return createStream(name, "record",
	                    rate, channels, PA_STREAM_RECORD);
}

int sendSpeaker(Speaker speaker, Moment *buff, long len) {
	int err;
	return pa_simple_write(speaker, buff, len * sizeof(Moment), &err) < 0;
}

int recvMicrophone(Microphone mic, Moment *buff, long len) {
	int err;
	return pa_simple_read(mic, buff, len * sizeof(Moment), &err) < 0;
}
