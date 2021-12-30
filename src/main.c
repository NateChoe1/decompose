#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sound.h>
#include <config.h>

typedef struct {
	double left;
	double right;
} Component;

Component getComponent(Moment *sound, int len, int rate, int component) {
	Component integral = {0};
	for (int i = 0; i < len; i++) {
		const double scale = cos(-component * i * M_PI * 2 / rate);
		integral.left += sound[i][0] * scale;
		integral.right += sound[i][1] * scale;
	}
	integral.left /= len;
	integral.right /= len;
	return integral;
}

void modify(Moment *sound, int len, int rate) {
	Component *newsound = calloc(len, sizeof(Component));
	for (int i = -268; i <= 268; i++) {
		double freq = 440 * pow(2, i / 96.0);
		double newfreq = 440 * pow(2, (i) / 96.0);
		Component comp = getComponent(sound, len, rate, freq);
		for (int j = 0; j < len; j++) {
			const double base = cos(newfreq*j*M_PI*2/rate);
			newsound[j].left += base * comp.left;
			newsound[j].right += base * comp.right;
		}
	}
	for (int i = 0; i < len; i++) {
		sound[i][0] = newsound[i].left;
		sound[i][1] = newsound[i].right;
	}
	free(newsound);
}

int main(int argc, char **argv) {
	Microphone mic = createMicrophone(argv[0], 44100, 2);
	Speaker speaker = createSpeaker(argv[0], 44100, 2);

	const int MOMENTS = 1000;

	Moment *buff = malloc(sizeof(Moment) * MOMENTS);

	for (;;) {
		if (recvMicrophone(mic, buff, MOMENTS))
			break;
		modify(buff, MOMENTS, 44100);
		if (sendSpeaker(speaker, buff, MOMENTS))
			break;
	}
	return 0;
}
