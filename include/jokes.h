#include <Arduino.h>

struct jokes_on_you{
    String joke1;
    String joke2;
};


void funny(jokes_on_you *joke){

    randomSeed(analogRead(A5));

    unsigned char los = random(5);

    switch(los){
        case(0):
        joke->joke1 = "Robert Rauch";
        break;

        case(1):
        joke->joke1 = "Gizmo";
        break;

        case(2):
        joke->joke1 = "Kuba Miroslawski";
        break;
        
        case(3):
        joke->joke1 = "Macij Repelowski";
        break;

        case(4):
        joke->joke1 = "Przemo Kaluza";
        break;
    }

    
    los = random(10);
    
    switch(los){
        case(0):
        joke->joke2 += "laminuje";
        break;

        case(1):
        joke->joke2 = "zamyka worek";
        break;

        case(2):
        joke->joke2 = "zamyka morde";
        break;
        
        case(3):
        joke->joke2 = "klei takitejpa";
        break;

        case(4):
        joke->joke2 = "udaje ze pracuje";
        break;

        case(5):
        joke->joke2 = "odsysa worek";
        break;

        case(6):
        joke->joke2 = "szlifuje rdzen";
        break;

        case(7):
        joke->joke2 = "stawia browary";
        break;

        case(8):
        joke->joke2 = "jest zly";
        break;

        case(9):
        joke->joke2 = "daje fula";
        break;
    }

}