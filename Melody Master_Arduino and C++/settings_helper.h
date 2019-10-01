#ifndef _SETTINGS_HELPER_H
#define _SETTINGS_HELPER_H

#include <Arduino.h>
#include <global_declarations.h>


// Bad input : returns false, capo = 0
// Good input : returns true, capo is entered value
bool setCapo() {
    Serial.println("Which fret is your capo on? (0 for no capo)");
    while (Serial.available() == 0)
    {
    }
    if (Serial.available() > 0) {
        capo = Serial.parseInt();
        if(capo >= 0 && capo <+ MAX_CAPO) {
            Serial.print("Input: ");
            Serial.println(capo);
            chord_shift = 8*capo;
            return true;
        }
        else {
            Serial.print("Invalid");
            capo = 0;
            return false;
        }
    }
}

//
bool SettingsMenu() {
    String selection = "";
    bool valid_input = false;

    Serial.println("GENERAL SETTINGS MENU");
    // Serial.println("led Brightness // Mic sensitivity");

    while(!valid_input) {
        valid_input = true;
        Serial.println("Capo position // eXit setttings");
        Serial.println("Enter Selection:");

        while (Serial.available() == 0)
        {
        }
        if (Serial.available() > 0) {
          selection = Serial.readString();
          Serial.print("Input: ");
          Serial.println(selection);
        }
        if(selection == "c" || selection == "C") {
            valid_input = false;
            while(!valid_input) {
                valid_input = setCapo();
            }
        }
        if(selection == "x" || selection == "X") {
            return false; // exit settings menu
        }
    }
    return true; //stay in settings menu
}

#endif
