#include <config.h>

#if AUDIO_METHOD == PULSE
#include <pulse/simple.h>
typedef pa_simple * Speaker;
typedef pa_simple * Microphone;
#else
#error "Invalid audio method"
#endif

typedef int16_t Moment[2];

Speaker createSpeaker(char *name, int rate, int channels);
Microphone createMicrophone(char *name, int rate, int channels);
int sendSpeaker(Speaker speaker, Moment *buff, long len);
int recvMicrophone(Microphone mic, Moment *buff, long  len);
