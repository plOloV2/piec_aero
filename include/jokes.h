#include <Arduino.h>

struct jokes_on_you{
    String joke1;
    String joke2;
};


void funny(jokes_on_you *joke){

    randomSeed(analogRead(A5));

    unsigned char los = random(4);

    switch(los){
        case(0):
        joke->joke1 = "Kuba Ruchala";
        break;

        case(1):
        joke->joke1 = "Nazari Honcharen";
        break;

        case(2):
        joke->joke1 = "Zuber";
        break;
        
        case(3):
        joke->joke1 = "Michal Dowgiallo";
        break;
    }

    
    los = random(8);
    
    switch(los){
        case(0):
        joke->joke2 = "laminuje";
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
        joke->joke2 = "daje fula";
        break;

        case(6):
        joke->joke2 = "leci w chuja";
        break;

        case(7):
        joke->joke2 = "stawia browary";
        break;
    }

}