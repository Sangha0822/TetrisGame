#ifndef GAMESONG_H
#define GAMESONG_H

#include "timerISR.h"
#include "helper.h"
#include "periph.h"
#include "irAVR.h"
#include "serialATmega.h"
#include "spiAVR.h"
#include "MAX7219.h"
#include "time.h"

#define NOTE_C4  261
#define NOTE_D4  294
#define NOTE_E4  329
#define NOTE_G4  392
#define NOTE_C5  523
#define NOTE_C3  131
#define NOTE_F3  175
#define NOTE_A3  220
#define NOTE_F2  87
#define NOTE_A2  110

#define SILENCE  0

unsigned int maryNotes[] = {
    NOTE_E4, NOTE_D4, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_E4, NOTE_E4,
    NOTE_D4, NOTE_D4, NOTE_D4, NOTE_E4, NOTE_G4, NOTE_G4,
    NOTE_E4, NOTE_D4, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_E4, NOTE_E4,
    NOTE_D4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_C4
};

unsigned int maryDurations[] = {
    8, 8, 8, 8, 8, 8, 4,
    8, 8, 4, 8, 8, 4,
    8, 8, 8, 8, 8, 8, 4,
    8, 8, 8, 8, 4
};

unsigned int maryLength = sizeof(maryNotes) / sizeof(maryNotes[0]);

unsigned int scoreEffectNotes[] = {
    NOTE_C5, NOTE_E4, NOTE_G4
};

unsigned int scoreEffectDurations[] = {
    8, 8, 4
};

unsigned int scoreEffectLength = sizeof(scoreEffectNotes) / sizeof(scoreEffectNotes[0]);

unsigned int loseEffectNotes[] = {
    NOTE_C4, NOTE_A3, NOTE_F3, NOTE_C3, NOTE_F2
};

unsigned int loseEffectDurations[] = {
    8, 8, 8, 8, 8, 8
};

unsigned int loseEffectLength = sizeof(loseEffectNotes) / sizeof(loseEffectNotes[0]);

unsigned int winEffectNotes[] = {
    NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5, NOTE_G4, NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5, NOTE_G4
};

unsigned int winEffectDurations[] = {
    8, 8, 8, 4, 4, 8, 8, 8, 4, 4
};

unsigned int winEffectLength = sizeof(winEffectNotes) / sizeof(winEffectNotes[0]);

void playTone(unsigned int frequency) {
    if (frequency == 0) {
        OCR1A = 0;
    } else {
        ICR1 = (16000000 / (8 * frequency)) - 1;
        OCR1A = ICR1 / 2;
    }
}

void playMelody(const unsigned int *notes, const unsigned int *durations, unsigned int length) {
    for (unsigned int i = 0; i < length; i++) {
        if (notes[i] == SILENCE) {
            OCR1A = 0;
        } else {
            ICR1 = (16000000 / (8 * notes[i])) - 1;
            OCR1A = ICR1 / 2;
        }
        unsigned int noteDuration = durations[i] * 30;
        while (noteDuration--) {
            _delay_ms(1);
        }
    }
    OCR1A = 0;
}

#endif
