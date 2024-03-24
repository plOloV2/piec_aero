#include <WString.h>

/*
W pliku piec.h zmienić wartości w linijkach 64 oraz 66 tak by odpowiadały ilości możliwości odjąć jeden
*/

String joke_row1(unsigned char los){

    switch(los){
        case(0):
        return "Kuba Ruchala";

        case(1):
        return "Nazari";

        case(2):
        return "Zuber";
        
        default:
        return "Michal Dowgiallo";
    }

}

String joke_row2(unsigned char los){
    
    switch(los){
        case(0):
        return "laminuje";

        case(1):
        return "zamyka worek";

        case(2):
        return "zamyka morde";
        
        case(3):
        return "klei takitejpa";

        case(4):
        return "udaje ze pracuje";

        case(5):
        return "daje fula";

        case(6):
        return "leci w chuja";

        default:
        return "stawia browary";
    }

}