#include "Notificator.h"
#include "Audio.h"

#ifndef DISPLAY_AUDIOQUEUENOTIFICATOR_H
#define DISPLAY_AUDIOQUEUENOTIFICATOR_H
class AudioQueueNotificator: public Notificator {
private:
    Audio *audio;
    char msg[200];
    QueueHandle_t queue;
public:
    AudioQueueNotificator(int i2s_blck, int i2s_lrc, int i2s_dout);
    void notify(char &msg);
    void run();
};
#endif //DISPLAY_AUDIOQUEUENOTIFICATOR_H
