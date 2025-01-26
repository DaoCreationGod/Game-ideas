#include <iostream>
#include <string>
#include <algorithm>
#include <unistd.h>
#include <vector>
using namespace std;

void startScreen();
void environmentFunction();
void beastsFunction();
void shopfunction();
void gameFunction();

struct environment{

    string name;

    vector<string> stats;

};

struct stats{

    string name;
    string value;

};
struct beast{

    string name;

    vector<string> attributes;
    vector<stats> individualStats;
};

int main(){

    startScreen();

    cout << "\nBack to main";
    
    return 0;
}


void startScreen(){

    string answer;
    cout << "Hello and welcome to Infinity Beast Horde";
    

    while(true){
        cout << "\nDo you want to play, yes (y) or no (n)?: ";
        cin >> answer;

        transform(answer.begin(), answer.end(), answer.begin(), [](unsigned char c){
            return tolower(c);
        });

        if (answer == "yes" || answer == "y"){

            cout << "\nWe are so happy you wanna play Infinity Beast Horde.";
            sleep(4);
            cout << "\nYou are the source of a calamity, an infinite beast horde calamity.";
            sleep(3);
            cout << "\nYour goal is to survive for as long as possible.";
            sleep(2);
            cout << "\nYou loose when all of your beasts are exterminated and you get killed.";
            sleep(1);
            break;

        }else if (answer == "no" || answer == "n"){

            cout << "\n Okey, bye and have a good morbin";
            break;

        }else {

            cout << "Invalid input";
        }
    }
}

void environmentFunction(){
    
    vector<environment> environments;

    environment plains = {"Plains", {"Open", "High Visibility", "+5 Speed", "-10 Sneak", "Special Resource: Wind Spirit", "Special Resource: Earth Spirit", "Note: Primarily Human enemies"}};
    environment mountains = {"Mountains", {"Good Visibility", "Easy to attack from", "+10 Intimidation", "-20 Suprise", "Special Resource: Rock Spirit", "Special Resource: Rare Metals", "Note: Primarily Dwarf and Birdfolk Enemies"}};
    environment forests = {"Forests", {"Relatively hidden", "Scary", "+10 Sneak", "-20 Response delay", "Special Resource: Rare Woods", "Special Resource: Nature Spirit", "Note: Primarily Elf and Beastmen Enemies"}};
    environment lakes = {"Lakes", {"Water", "Relatively easy to find", "+10 Defense", "-15 Attack", "Special Resource: Water Spirit", "Special Resource: Rare Waters", "Note: Primarily Fishfolk and Frogpeople Enemies"}};
    environment oceans = {"Oceans", {"Deep Water","Well hidden", "+25 Sneak", "-15 Defense", "Special Resource: Special Fires", "Special Resource: Rare Waters", "Note: Primarily Mermen and Fishfolk Enemies"}};
    environment deserts = {"Deserts", {"Extreme weather", "Open", "+30 Personal Defense", "-25 Enemies", "Special Resource: Rare Fires", "Special Resource: Unstable Spirits", "Note: Primarily Fay Enemies"}};
    environment caves = {"Caves", {"Tight", "Easy to defend", "+30 Enemies", "-35 Weak enemies", "Special Resource: Rare Metals", "Special Resource: Fossils", "Note: Primarily Dwarf and Fay Enemies"}};
}

void beastsFunction(){

    vector<beast> beasts;

    beast rats = {
        "Rats", // Beast name
        {"Size: Tiny", "Type: Horde", "Type: Ground", "Type: Night", "Type: Underground", "Individual Stats:"},
        {{"Attack", "0.1"}, {"Defense", "0.5"}, {"Speed", "1"}} // Individual stats
    };

}

void shopFunction(){

}

void gameFunction(){

}
