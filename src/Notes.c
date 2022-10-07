#include <math.h>
#include <stdio.h>
#include <esp_log.h>

#include "Notes.h"

char * notes[] = {
    "A",
    "A#",
    "B",
    "C",
    "C#",
    "D",
    "D#",
    "E",
    "F",
    "F#",
    "G",
    "G#"
};

void freq_to_note(float frequency, char * string_buffer) {

    // Convert frequency to note and change string_buffer to that note

    int semitones = round(12 * log2(frequency / 440));

    semitones = semitones % 12;

    if (semitones < 0) {
        semitones += 12;
    }

    sprintf(string_buffer, "%s", notes[semitones]);

}

int diff_to_note(char note_target, char note_current) {

    int i_target;
    int i_current;

    for (int i = 0; i < 12; i++)
    {
        if (note_target == notes[i][0])
            i_target = i;
        if (note_current == notes[i][0])
            i_current = i;
    }
    
    return i_current - i_target;

}