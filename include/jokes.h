#include <Arduino.h>

String fun1(){

    randomSeed(analogRead(A5));

    String joke = "";
    
    unsigned char los = random(16);

    switch(los){
        case(0):
        joke = "Robert Rauch";
        break;

        case(1):
        joke = "Gizmo";
        break;

        case(2):
        joke = "Kuba Mirosławski";
        break;
        
        case(3):
        joke = "Macij Repelowski";
        break;

        case(4):
        joke = "Przemo Kałuza";
        break;

        case(5):
        joke = "Adam Małysz";
        break;

        case(6):
        joke = "Filip Szwedo";
        break;

        case(7):
        joke = "Łukasz Spyt";
        break;

        case(8):
        joke = "Marcin Tkaczyk";
        break;

        case(9):
        joke = "Oskar Wegrzyn";
        break;

        case(10):
        joke = "Daniel Ceron";
        break;

        case(11):
        joke = "Twoja babka";
        break;

        case(12):
        joke = "Dział Chassis";
        break;

        case(13):
        joke = "Baba z portierni";
        break;

        case(14):
        joke = "Marketing";
        break;

        case(15):
        joke = "Suspension";
        break;
    }

    while(joke.length()<16)  
        joke += " ";

    los = random(26);
    
    switch(los){
        case(0):
        joke += "laminuje";
        break;

        case(1):
        joke = "zamyka worek";
        break;

        case(2):
        joke = "zamyka morde";
        break;
        
        case(3):
        joke = "klei takitejpa";
        break;

        case(4):
        joke = "udaje że pracuje";
        break;

        case(5):
        joke = "odsysa worek";
        break;

        case(6):
        joke = "szlifuje rdzen";
        break;

        case(7):
        joke = "stawia browary";
        break;

        case(8):
        joke = "tnie worek";
        break;

        case(9):
        joke = "daje fula";
        break;

        case(10):
        joke = "jest głodny/a";
        break;

        case(11):
        joke = "jest zly/a";
        break;

        case(12):
        joke = "chce do domu";
        break;

        case(13):
        joke = "tnie wykroje";
        break;

        case(14):
        joke = "masuje uszczelke";
        break;

        case(15):
        joke = "szlifuje formy";
        break;

        case(16):
        joke = "wchodzi do pieca";
        break;

        case(17):
        joke = "idzie do pirmu";
        break;

        case(18):
        joke = "gwintuje inserty";
        break;

        case(19):
        joke = "zlamal endplate";
        break;

        case(20):
        joke = "wyrwal wklejke";
        break;

        case(21):
        joke = "depnal na ub";
        break;

        case(22):
        joke = "udaje greka";
        break;

        case(23):
        joke = "ma wszy";
        break;

        case(24):
        joke = "nic nie rozumie";
        break;

        case(25):
        joke = "jest szalony";
        break;
    }

    while(joke.length()<32)  
        joke += " ";

  return joke;
}


String fun2(){

    randomSeed(analogRead(A5));

    String joke = "";
    
    unsigned char los = random(10);

    switch(los){
        case(0):
        joke += "Jak nazywa sie  ";
        joke += "najwiekszy diler";
        joke += "dragow w Polsce?";
        joke += "                ";
        joke += "KROL KRAK       ";
        joke += "                ";
        break;

        case(1):
        joke += "Dlaczego lod    ";
        joke += "sie topi?       ";
        joke += "Bo nie          ";
        joke += "umie plywac     ";
        break;

        case(2):
        joke += "Czemu w Afryce  ";
        joke += "nie ma tabletek?";
        joke += "bo nie mozna    ";
        joke += "ich brac na     ";
        joke += "pusty zoladek   ";
        joke += "                ";
        break;

        case(3):
        joke += "Czemu w Afryce  ";
        joke += "nie ma tabletek?";
        joke += "bo trzeba je    ";
        joke += "popic woda      ";
        break;

        case(4):
        joke += "Co bialego      ";
        joke += "ma murzyn?      ";
        joke += "Sluchac         ";
        joke += "                ";
        break;

        case(5):
        joke += "Czym sie rozni  ";
        joke += "serce od murzyna";
        joke += "Serce nie sluga ";
        joke += "                ";
        break;

        case(6):
        joke += "Czego rudy nie  ";
        joke += "ma w szkole?    ";
        joke += "Zycia           ";
        joke += "                ";
        break;

        case(7):
        joke += "Czego nie ma na ";
        joke += "dobrej imprezie?";
        joke += "Rudego          ";
        joke += "                ";
        break;

        case(8):
        joke += "Kto jest        ";
        joke += "patronem ELEC?  ";
        joke += "Ewa, bo pierwsza";
        joke += "ciagnela druta  ";
        break;

        case(9):
        joke += "Nie do rymu     ";
        joke += "nie do taktu    ";
        joke += "wsadz se chuja  ";
        joke += "do kontaktu     ";
        break;

    }

    return joke;
}