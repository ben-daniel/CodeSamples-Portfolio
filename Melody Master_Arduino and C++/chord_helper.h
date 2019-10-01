#ifndef _CHORD_HELPER_H
#define _CHORD_HELPER_H

#include <Arduino.h>
#include <chord.h>
#include <SD.h>

Chord my_chord;
File myFile;
const String file_name = "chord.txt";

void displayChord(String chord) {
  my_chord.display_LEDs();
//  for (uint16_t i = 0; i < CHORD_LIST_SIZE; i++) {
//    if (chord == chord_list[i].get_chord_name()) {
//      chord_list[i].display_LEDs();
//      break;
//    }
//  }
}

bool readChord(String my_chord_name) {
  uint8_t counter = 0;
  String chord_name = "";
  bool match = false;

  // Check to see if the file exists:
  if (SD.exists(file_name)) {
//    Serial.println(file_name + " exists.");
  } else {
    Serial.println(file_name + " doesn't exist.");
  }

  myFile = SD.open(file_name);
  if (myFile) {
//    Serial.println(file_name + ":");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      uint8_t value = myFile.read();
      if(match && counter == 0) {
        break;
      }

      // chord name
      if(counter == 0 && value != '|') {
//        if(!match) {
//          chord_name += (char)value;
//        }
        chord_name += (char)value;
//        Serial.write(value);
      }
      // comma
      else if(value == '|') {
        counter++;
        if(counter == 1) {
          if(chord_name == my_chord_name) {
//            Serial.print("*MATCH*");
            match = true;
          }
  //        else if(!match){
          else {
            chord_name = "";
          }
        }
//        Serial.write(value);
      }
      // e fret
      else if(value != '|' && value != (char)32
          && counter == 1) {
        my_chord.set_e_fret(value + chord_shift);
//        Serial.print(value);
      }
      // B fret
      else if(value != '|' && value != (char)32
          && counter == 2) {
        my_chord.set_B_fret(value + chord_shift);
//        Serial.print(value);
      }
      // G fret
      else if(value != '|' && value != (char)32
          && counter == 3) {
        my_chord.set_G_fret(value + chord_shift);
//        Serial.print(value);
      }
      // D fret
      else if(value != '|' && value != (char)32
          && counter == 4) {
        my_chord.set_D_fret(value + chord_shift);
//        Serial.print(value);
      }
      // A fret
      else if(value != '|' && value != (char)32
          && counter == 5) {
        my_chord.set_A_fret(value + chord_shift);
//        Serial.print(value);
      }
      // E fret
      else if(value != '|' && value != (char)32
          && counter == 6) {
        my_chord.set_E_fret(value + chord_shift);
//        Serial.print(value);
      }
      // e color
      else if(value != '|' && value != (char)32
          && counter == 7) {
        my_chord.set_e_color(value);
//        Serial.print(value);
      }
      // B color
      else if(value != '|' && value != (char)32
          && counter == 8) {
        my_chord.set_B_color(value);
//        Serial.print(value);
      }
      // G color
      else if(value != '|' && value != (char)32
          && counter == 9) {
        my_chord.set_G_color(value);
//        Serial.print(value);
      }
      // D color
      else if(value != '|' && value != (char)32
          && counter == 10) {
        my_chord.set_D_color(value);
//        Serial.print(value);
      }
      // A color
      else if(value != '|' && value != (char)32
          && counter == 11) {
        my_chord.set_A_color(value);
//        Serial.print(value);
      }
      // E color
      else if(value != '|' && value != (char)32
          && counter == 12) {
        my_chord.set_E_color(value);
//        Serial.print(value);
      }
      // reset counter upon newline
      else if(value == (char)32) {
        counter = 0;
//        Serial.write('\n');
      }
      // other case?
      else {
//        Serial.write(value);
      }
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening " + file_name);
  }
  return match;
}

String readChordInput() {
  String chord = "";
  while (true) {
//    Serial.println("Chord Options:");
//    for(uint16_t i = 0; i < CHORD_LIST_SIZE; i++) {
//      chord_list[i].print_chord();
//    }

    Serial.println("Which chord?");
    while (Serial.available() == 0)
    {
    }
    if (Serial.available() > 0) {
      chord = Serial.readString();
      Serial.print("Input: ");
      Serial.println(chord);
    }
    if(readChord(chord)) {
      return chord;
    }
//    // Check for valid entry. If valid return. Else get new input.
//    for (uint16_t i = 0; i < CHORD_LIST_SIZE; i++)
//    {
//      if (chord == chord_list[i].get_chord_name()) {
//        return chord;
//      }
//    }
  }
}

#endif
