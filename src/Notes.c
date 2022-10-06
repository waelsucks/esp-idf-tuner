#include <math.h>
#include <stdio.h>

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