#include "AudioQueueNotificator.h"
#include "Audio.h"

void audio_info(const char *info) {
    //Serial.print("info        ");
   // Serial.println(info);
}

void audio_eof_speech(const char *info){
  //  Serial.print("eof_speech  ");
   // Serial.println(info);
}

AudioQueueNotificator::AudioQueueNotificator(int i2s_blck, int i2s_lrc, int i2s_dout) {
    audio = new Audio();
    audio->setPinout(i2s_blck, i2s_lrc, i2s_dout);
    audio->setVolume(21);
    queue = xQueueCreate(10, sizeof(msg));
}

void AudioQueueNotificator::notify(char &msg) {
    xQueueSend(queue, &msg, portMAX_DELAY);
}

void AudioQueueNotificator::run() {
    xQueueReceive(queue, &msg, portMAX_DELAY);
    audio->connecttospeech(msg, "ru");
    while (true) {
        audio->loop();
        if (!audio->isRunning()) {
            break;
        }
    }
}