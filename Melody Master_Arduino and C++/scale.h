#ifndef _SCALE_H
#define _SCALE_H

#include <global_declarations.h>
#include <mm_functions.h>
#include <note.h>

const uint16_t MAX_NOTES = 12;

class Scale {
private:
    Note notes[MAX_NOTES];
    // tracker variable to keep track of how many notes in this scale:
    uint16_t num_notes;
    String scale_name;
public:
    // constructor:
    Scale(String scale_name_new, Note notes_new[], uint16_t num_notes_new) {
        scale_name = scale_name_new;
        num_notes = num_notes_new;
        for (uint16_t i = 0; i < num_notes; i++)
        {
            notes[i] = notes_new[i];
        }
    }
    // void shift(uint16_t shift_value);

    void display_LEDs(uint16_t col) {
        clearAll(NUM_FRETS, tlc);
        for(uint16_t i=0; i<num_notes; i++) {
            for(uint16_t j=0; j<notes[i].get_note_len(); j++) {
                color(col);
                tlc.setLED(notes[i].get_note(j), r, g, b);
            }
        }
        tlc.write();
    }

    String get_scale_name() {
      return scale_name;
    }
};

#endif
