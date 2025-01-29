#include <iostream>
#include <string>
#include <algorithm>
#include <unistd.h>
#include <vector>
#include <ctime>

using namespace std;

// =====================
// Structure Definitions
// =====================

struct Stats {
    string name;
    string value;
};

struct Environment {
    string name;
    vector<string> stats;
};

struct Beast {
    string race;
    vector<string> attributes;
    vector<Stats> individualStats;
};

struct Enemy {
    string race;
    string skillClass;
    int levels;
    vector<Stats> individualStats;
};

struct Settlement {
    string name;
    string type;       // Added missing member
    string size;
    int enemyCount;
    float x, y;
    vector<string> specialResources;
};

struct Skill {
    string name;
    int energyCost;
    int resourceCost;
    string description;
    bool purchased;
};

// =================
// Global Variables
// =================
int totalKills = 0;
int totalEnergy = 0;
vector<string> collectedResources;

// Spawning mechanics
int spawnAmount = 3;
int spawnRadius = 10;
int spawnEnergyCost = 50;
float beastStatMultiplier = 1.0;

// =================
// Shop System
// =================
struct Shop {
    vector<Skill> availableSkills;
    
    void displayShop() {
        cout << "Shop:\n";
        for (size_t i = 0; i < availableSkills.size(); ++i) {
            if (!availableSkills[i].purchased) {
                cout << i + 1 << ". " << availableSkills[i].name 
                     << " - Energy: " << availableSkills[i].energyCost
                     << ", Resources: " << availableSkills[i].resourceCost
                     << "\n   " << availableSkills[i].description << "\n";
            }
        }
    }

    bool buySkill(int skillIndex, int& playerEnergy, vector<string>& playerResources) {
        if (skillIndex < 0 || skillIndex >= static_cast<int>(availableSkills.size())) {
            cout << "Invalid skill index.\n";
            return false;
        }

        Skill& skill = availableSkills[skillIndex];
        if (playerEnergy >= skill.energyCost && playerResources.size() >= static_cast<size_t>(skill.resourceCost)) {
            playerEnergy -= skill.energyCost;
            for (int i = 0; i < skill.resourceCost; ++i) {
                if (!playerResources.empty()) playerResources.pop_back();
            }
            skill.purchased = true;
            cout << "Purchased: " << skill.name << "!\n";
            return true;
        }
        cout << "Insufficient resources!\n";
        return false;
    }
};

// ================
// Function Prototypes
// ================
void startScreen();
void gameFunction();
void killEnemy(const Enemy& enemy);
void killEnemy(const string& race, int level);
void generateSettlements(vector<Settlement>& settlements, int numSettlements, const string& environment);
void attackSettlement(Settlement& settlement);
void assignSpecialResources(Settlement& settlement, const string& environment);
void collectResources(const Settlement& settlement);
void initializeGameWorld(vector<Environment>& environments, vector<Beast>& beasts, vector<Enemy>& enemies);
void printEnvironments(const vector<Environment>& environments);
void printBeasts(const vector<Beast>& beasts);
void printEnemies(const vector<Enemy>& enemies);

// ================
// Core Functions
// ================

int main() {
    srand(time(0));
    vector<Environment> environments;
    vector<Beast> beasts;
    vector<Enemy> enemies;

    initializeGameWorld(environments, beasts, enemies);
    startScreen();

    // Environment selection
    gameFunction();
    
    cout << "\nGame Over!\nTotal kills: " << totalKills 
         << "\nTotal energy: " << totalEnergy << endl;
    return 0;
}

void startScreen() {
    cout << "=============\nInfinity Beast Horde\n=============\n";
    sleep(1);
    cout << "Survive as long as possible!\n";
    sleep(1);
}

void initializeGameWorld(vector<Environment>& environments, vector<Beast>& beasts, vector<Enemy>& enemies) {
    // Initialize environments
    environments = {
        {"Plains", {"Open", "High Visibility", "+5 Speed", "-10 Sneak", "Special Resource: Wind Spirit"}},
        {"Mountains", {"Good Visibility", "Easy to attack from", "+10 Intimidation", "-20 Surprise", "Special Resource: Rock Spirit"}},
        {"Forests", {"Relatively hidden", "Scary", "+10 Sneak", "-20 Response delay", "Special Resource: Nature Spirit"}}
    };

    // Initialize beasts
    beasts = {
        {"Rats", {"Size: Tiny", "Type: Horde", "Type: Ground"},
         {{"Attack", "0.1"}, {"Defense", "0.5"}, {"Speed", "1"}}}
    };

    // Initialize enemies
    enemies = {
        {"Humans", "Class: Commoner", 1, {{"Strength", "5.0"}, {"Endurance", "5"}}},
        {"Elves", "Class: Apprentice", 1, {{"Strength", "2.5"}, {"Agility", "10"}}},
        {"Dwarfs", "Class: Blacksmith", 1, {{"Strength", "20.0"}, {"Endurance", "15"}}}
    };
}

void killEnemy(const Enemy& enemy) {
    totalEnergy += enemy.levels;
    totalKills++;
    cout << "Killed " << enemy.race << " (+" << enemy.levels << " energy)\n";
}

void killEnemy(const string& race, int level) {
    totalEnergy += level;
    totalKills++;
    cout << "Killed " << race << " (Level " << level << ")\n";
}

void gameFunction() {
    vector<Settlement> settlements;
    string environment;
    
    cout << "\nChoose environment (Plains, Forests, Mountains): ";
    cin >> environment;
    transform(environment.begin(), environment.end(), environment.begin(), ::tolower);

    generateSettlements(settlements, 3, environment);

    for (auto& settlement : settlements) {
        attackSettlement(settlement);
    }
}

void generateSettlements(vector<Settlement>& settlements, int numSettlements, const string& environment) {
    vector<string> names = {"Outpost", "Village", "Town"};
    vector<string> types = {"Military", "Trade", "Residential"};
    
    for (int i = 0; i < numSettlements; ++i) {
        Settlement s;
        s.name = names[rand() % names.size()];
        s.type = types[rand() % types.size()];
        s.size = (rand() % 2 == 0) ? "Small" : "Medium";
        s.enemyCount = rand() % 10 + 5;
        s.x = static_cast<float>(rand() % 20 - 10);
        s.y = static_cast<float>(rand() % 20 - 10);
        assignSpecialResources(s, environment);
        
        settlements.push_back(s);
        cout << "Discovered " << s.type << " " << s.name << " at ("
             << s.x << ", " << s.y << ")\n";
    }
}

void attackSettlement(Settlement& settlement) {
    cout << "\nAttacking " << settlement.name << "!\nEnemies: " 
         << settlement.enemyCount << "\n";
    
    while (settlement.enemyCount > 0) {
        // Create temporary enemy
        killEnemy("Bandit", rand() % 3 + 1);
        settlement.enemyCount--;
    }
    
    collectResources(settlement);
    cout << settlement.name << " destroyed!\n";
}

void assignSpecialResources(Settlement& settlement, const string& environment) {
    if (environment == "plains") {
        settlement.specialResources = {"Wind Spirit", "Earth Essence"};
    } else if (environment == "forests") {
        settlement.specialResources = {"Nature Essence", "Sacred Wood"};
    } else {
        settlement.specialResources = {"Stone Core", "Mountain Ore"};
    }
}

void collectResources(const Settlement& settlement) {
    for (const auto& res : settlement.specialResources) {
        collectedResources.push_back(res);
        cout << "Acquired: " << res << "\n";
    }
}   