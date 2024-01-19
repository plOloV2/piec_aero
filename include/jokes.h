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

        // case(4):
        // joke->joke1 = "";
        // break;

        // case(5):
        // joke->joke1 = "";
        // break;
        
        // case(6):
        // joke->joke1 = "";
        // break;

        // case(7):
        // joke->joke1 = "";
        // break;

        // case(8):
        // joke->joke1 = "";
        // break;
        
        // case(9):
        // joke->joke1 = "";
        // break;
    }

    
    los = random(27);
    
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
        
        // case(3):
        // joke->joke2 = "klei takitejpa";
        // break;

        // case(4):
        // joke->joke2 = "udaje że pracuje";
        // break;

        // case(5):
        // joke->joke2 = "odsysa worek";
        // break;

        // case(6):
        // joke->joke2 = "szlifuje rdzen";
        // break;

        // case(7):
        // joke->joke2 = "stawia browary";
        // break;

        // case(8):
        // joke->joke2 = "tnie worek";
        // break;

        // case(9):
        // joke->joke2 = "daje fula";
        // break;

        // case(10):
        // joke->joke2 = "jest głodny/a";
        // break;

        // case(11):
        // joke->joke2 = "jest zly/a";
        // break;

        // case(12):
        // joke->joke2 = "chce do domu";
        // break;

        // case(13):
        // joke->joke2 = "tnie wykroje";
        // break;

        // case(14):
        // joke->joke2 = "masuje uszczelke";
        // break;

        // case(15):
        // joke->joke2 = "szlifuje formy";
        // break;

        // case(16):
        // joke->joke2 = "wchodzi do pieca";
        // break;

        // case(17):
        // joke->joke2 = "idzie do pirmu";
        // break;

        // case(18):
        // joke->joke2 = "gwintuje inserty";
        // break;

        // case(19):
        // joke->joke2 = "zlamal endplate";
        // break;

        // case(20):
        // joke->joke2 = "wyrwal wklejke";
        // break;

        // case(21):
        // joke->joke2 = "depnal na ub";
        // break;

        // case(22):
        // joke->joke2 = "udaje greka";
        // break;

        // case(23):
        // joke->joke2 = "ma wszy";
        // break;

        // case(24):
        // joke->joke2 = "nic nie rozumie";
        // break;

        // case(25):
        // joke->joke2 = "jest szalony";
        // break;

        // case(26):
        // joke->joke2 = "leci w chuja";
        // break;
    }
}