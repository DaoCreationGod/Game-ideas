#include <iostream>
#include <string>
#include <algorithm>
#include <unistd.h>
#include <vector>

using namespace std;
//global variables
int totalKills = 0;
int earnedEnergy = 0;
int totalEnergy = 0;
vector<string> collectedResources;


// Struct Definitions
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

struct Settlement{

    string name;
    string size;
    int enemyCount;
    float x, y;
    vector<string> specialResources;

};

struct Skill {
    string name;      // Name of the skill
    int energyCost;   // Cost in energy
    int resourceCost; // Cost in special resources (if applicable)
    string description; // Description of the skill
    bool purchased;   // Whether the skill has been purchased
};

// Structure to hold the shop and skills
struct Shop {
    vector<Skill> availableSkills;
    
    // Display the shop's available skills
    void displayShop() {
        cout << "Shop:\n";
        for (int i = 0; i < availableSkills.size(); ++i) {
            if (!availableSkills[i].purchased) {
                cout << i + 1 << ". " << availableSkills[i].name 
                     << " - Energy: " << availableSkills[i].energyCost
                     << ", Resources: " << availableSkills[i].resourceCost
                     << "\n";
                cout << "   " << availableSkills[i].description << "\n";
            }
        }
    }
    
    // Buy a skill from the shop
    bool buySkill(int skillIndex, int& playerEnergy, vector<string>& playerResources) {
        if (skillIndex < 0 || skillIndex >= availableSkills.size()) {
            cout << "Invalid skill index.\n";
            return false;
        }

        Skill& skill = availableSkills[skillIndex];
        
        // Check if the player has enough resources
        if (playerEnergy >= skill.energyCost && canAffordResources(skill.resourceCost, playerResources)) {
            playerEnergy -= skill.energyCost;
            deductResources(skill.resourceCost, playerResources);
            skill.purchased = true;
            cout << "You bought the " << skill.name << " skill!\n";
            applySkillEffect(skill);
            return true;
        } else {
            cout << "Not enough resources or energy.\n";
            return false;
        }
    }

    // Check if the player can afford the resources
    bool canAffordResources(int resourceCost, const vector<string>& playerResources) {
        return (playerResources.size() >= resourceCost);  // For simplicity, checking if we have enough resources
    }

    // Deduct resources after purchasing
    void deductResources(int resourceCost, vector<string>& playerResources) {
        while (resourceCost > 0 && !playerResources.empty()) {
            playerResources.pop_back();  // Deduct one resource for simplicity
            --resourceCost;
        }
    }

    // Apply skill effects to the game (adjusting spawn mechanics and more)
    void applySkillEffect(const Skill& skill) {
        if (skill.name == "Bigger Spawn Amount") {
            // Increase spawn amount (increases number of beasts spawned at a time)
            spawnAmount += 2;  // For example, increase by 2
            cout << "Spawn amount increased! Now you spawn " << spawnAmount << " beasts.\n";
        } else if (skill.name == "Bigger Spawning Radius") {
            // Increase spawn radius (further distance from settlement)
            spawnRadius += 10;  // For example, increase radius by 10 units
            cout << "Spawning radius increased! Now beasts spawn " << spawnRadius << " units away.\n";
        } else if (skill.name == "Cheaper Cost") {
            // Decrease energy cost to spawn beasts
            spawnEnergyCost = max(1, spawnEnergyCost - 20);  // Prevent going negative
            cout << "Energy cost reduced! Now it costs " << spawnEnergyCost << " energy to spawn.\n";
        } else if (skill.name == "Stat Strengthening") {
            // Strengthen a specific stat of a beast (e.g., health or damage)
            beastStatMultiplier *= 1.2;  // For example, increase stats by 20%
            cout << "Beast stats strengthened! Now your beasts are stronger.\n";
        }
    }
};
Shop initializeShop() {
    Shop shop;
    
    // Initialize skills
    shop.availableSkills.push_back({"Bigger Spawn Amount", 100, 0, "Increases the amount of beasts spawned at a time."});
    shop.availableSkills.push_back({"Bigger Spawning Radius", 80, 0, "Increases the radius within which beasts can spawn."});
    shop.availableSkills.push_back({"Cheaper Cost", 120, 0, "Reduces the energy cost to spawn beasts."});
    shop.availableSkills.push_back({"Stat Strengthening", 150, 0, "Increases the stats (e.g., damage or health) of your beasts."});
    
    return shop;
}
// Global variables
int spawnAmount = 3;   // Default spawn amount per settlement
int spawnRadius = 10;   // Default spawning radius in units
int spawnEnergyCost = 50;  // Default energy cost to spawn beasts
float beastStatMultiplier = 1.0;  // Default beast stat multiplier (1.0 means no stat change)

// Function to spawn beasts (after skills are applied)
void spawnBeasts(int playerEnergy) {
    if (playerEnergy < spawnEnergyCost) {
        cout << "Not enough energy to spawn beasts.\n";
        return;
    }
    
    playerEnergy -= spawnEnergyCost;  // Deduct energy
    cout << "Spawning " << spawnAmount << " beasts within " << spawnRadius << " units of the settlement.\n";
    
    // Use beastStatMultiplier to adjust stats
    cout << "Beasts have " << (beastStatMultiplier * 100) << "% of their original stats!\n";
    
    // Handle spawn logic here (e.g., create beasts, handle movement, etc.)
}


// Function Prototypes
void startScreen();
void environmentFunction(vector<Environment>& environments);
void beastsFunction(vector<Beast>& beasts);
void enemyFunction(vector<Enemy>& enemies);
void shopFunction();
void gameFunction();
void killEnemy();
void generateSettlements();
void attackSettlement();
void assignSpecialResources();

void printEnvironments(const vector<Environment>& environments);
void printBeasts(const vector<Beast>& beasts);
void printEnemies(const vector<Enemy>& enemies);

// Main Function
int main() {
    string answer;
    vector<Environment> environments;
    vector<Beast> beasts;
    vector<Enemy> enemies;

    environmentFunction(environments);
    beastsFunction(beasts);
    enemyFunction(enemies);

    startScreen();

    cout << "\nBefore we start, do you want to know the most basic of info about your enemies? Yes(y) or no(n).";
    cin >> answer;
    transform(answer.begin(), answer.end(), answer.begin(), ::tolower);
    if (answer == "yes" || answer == "y") {
        printEnemies(enemies);
    }

    cout << "\nBefore we start, do you want to know the most basic of info about the available environments? Yes(y) or no(n).";
    cin >> answer;
    transform(answer.begin(), answer.end(), answer.begin(), ::tolower);
    if (answer == "yes" || answer == "y") {
        printEnvironments(environments);
    }

    cout << "\nBefore we start, do you want to know the most basic of info about your available beasts? Yes(y) or no(n).";
    cin >> answer;
    transform(answer.begin(), answer.end(), answer.begin(), ::tolower);
    if (answer == "yes" || answer == "y") {
        printBeasts(beasts);
    }

    gameFunction();
    cout << "\nBack to main";
    return 0;
}

// Start Screen Function
void startScreen() {
    string answer;
    cout << "Hello and welcome to Infinity Beast Horde\n";

    while (true) {
        cout << "\nDo you want to play, yes (y) or no (n)?: ";
        cin >> answer;

        transform(answer.begin(), answer.end(), answer.begin(), ::tolower);

        if (answer == "yes" || answer == "y") {
            cout << "\nWe are so happy you want to play Infinity Beast Horde.";
            sleep(4);
            cout << "\nYou are the source of a calamity, an infinite beast horde calamity.";
            sleep(3);
            cout << "\nYour goal is to survive for as long as possible.";
            sleep(2);
            cout << "\nYou lose when all of your beasts are exterminated and you get killed.";
            sleep(1);
            break;
        } else if (answer == "no" || answer == "n") {
            cout << "\nOkay, bye and have a good morbin";
            break;
        } else {
            cout << "Invalid input, please type 'yes' or 'no'.\n";
        }
    }
}

// Print Functions
void printEnvironments(const vector<Environment>& environments) {
    cout << "Environments:\n";
    for (const auto& env : environments) {
        cout << "Name: " << env.name << "\nStats:\n";
        for (const auto& stat : env.stats) {
            cout << "  - " << stat << "\n";
        }
        cout << "\n";
    }
}

void printBeasts(const vector<Beast>& beasts) {
    cout << "Beasts:\n";
    for (const auto& beast : beasts) {
        cout << "Race: " << beast.race << "\nAttributes:\n";
        for (const auto& attr : beast.attributes) {
            cout << "  - " << attr << "\n";
        }
        cout << "Individual Stats:\n";
        for (const auto& stat : beast.individualStats) {
            cout << "  " << stat.name << ": " << stat.value << "\n";
        }
        cout << "\n";
    }
}

void printEnemies(const vector<Enemy>& enemies) {
    cout << "Enemies:\n";
    for (const auto& enemy : enemies) {
        cout << "Race: " << enemy.race << "\n";
        cout << "Skill Class: " << enemy.skillClass << "\n";
        cout << "Level: " << enemy.levels << "\n";
        cout << "Individual Stats:\n";
        for (const auto& stat : enemy.individualStats) {
            cout << "  " << stat.name << ": " << stat.value << "\n";
        }
        cout << "\n";
    }
}

// Environment Setup Function
void environmentFunction(vector<Environment>& environments) {
    environments = {
        {"Plains", {"Open", "High Visibility", "+5 Speed", "-10 Sneak", "Special Resource: Wind Spirit", "Special Resource: Earth Spirit", "Note: Primarily Human enemies"}},
        {"Mountains", {"Good Visibility", "Easy to attack from", "+10 Intimidation", "-20 Surprise", "Special Resource: Rock Spirit", "Special Resource: Rare Metals", "Note: Primarily Dwarf and Birdfolk Enemies"}},
        {"Forests", {"Relatively hidden", "Scary", "+10 Sneak", "-20 Response delay", "Special Resource: Rare Woods", "Special Resource: Nature Spirit", "Note: Primarily Elf and Beastmen Enemies"}},
        {"Lakes", {"Water", "Relatively easy to find", "+10 Defense", "-15 Attack", "Special Resource: Water Spirit", "Special Resource: Rare Waters", "Note: Primarily Fishfolk and Frogpeople Enemies"}},
        {"Oceans", {"Deep Water", "Well hidden", "+25 Sneak", "-15 Defense", "Special Resource: Special Fires", "Special Resource: Rare Waters", "Note: Primarily Mermen and Fishfolk Enemies"}},
        {"Deserts", {"Extreme weather", "Open", "+30 Personal Defense", "-25 Enemies", "Special Resource: Rare Fires", "Special Resource: Unstable Spirits", "Note: Primarily Fay Enemies"}},
        {"Caves", {"Tight", "Easy to defend", "+30 Enemies", "-35 Weak enemies", "Special Resource: Rare Metals", "Special Resource: Fossils", "Note: Primarily Dwarf and Fay Enemies"}}
    };
}

// Beasts Setup Function
void beastsFunction(vector<Beast>& beasts) {
    beasts = {
        {"Rats", {"Size: Tiny", "Type: Horde", "Type: Ground", "Type: Night", "Type: Underground"},
         {{"Attack", "0.1"}, {"Defense", "0.5"}, {"Speed", "1"}}}
    };
}

// Enemies Setup Function
void enemyFunction(vector<Enemy>& enemies) {
    enemies = {
        {"Humans", "Class: Commoner", 1, {{"Strength", "5.0"}, {"Endurance", "5"}, {"Agility", "5"}, {"Wisdom", "5"}, {"Intelligence", "5"}, {"Spirit", "5"}}},
        {"Elves", "Class: Apprentice Gatherer", 1, {{"Strength", "2.5"}, {"Endurance", "6"}, {"Agility", "10"}, {"Wisdom", "5"}, {"Intelligence", "5"}, {"Spirit", "20"}}},
        {"Dwarfs", "Class: Apprentice Blacksmith", 1, {{"Strength", "20.0"}, {"Endurance", "15"}, {"Agility", "1.5"}, {"Wisdom", "3"}, {"Intelligence", "10"}, {"Spirit", "7"}}},
        {"Beastmen", "Class: Apprentice Hunter", 1, {{"Strength", "5.0"}, {"Endurance", "15"}, {"Agility", "15"}, {"Wisdom", "1"}, {"Intelligence", "2"}, {"Spirit", "9"}}},
        {"Fays", "Class: Trickster", 1, {{"Strength", "0.1"}, {"Endurance", "0.1"}, {"Agility", "30"}, {"Wisdom", "15"}, {"Intelligence", "15"}, {"Spirit", "15"}}},
        {"Mermen", "Class: Slave", 1, {{"Strength", "1.0"}, {"Endurance", "2"}, {"Agility", "10"}, {"Wisdom", "2"}, {"Intelligence", "3"}, {"Spirit", "3"}}},
        {"Birdfolk", "Class: Pray", 1, {{"Strength", "2.0"}, {"Endurance", "15"}, {"Agility", "15"}, {"Wisdom", "2"}, {"Intelligence", "6"}, {"Spirit", "6"}}},
        {"Fishfolk", "Class: Sardine", 1, {{"Strength", "0.5"}, {"Endurance", "2"}, {"Agility", "15"}, {"Wisdom", "1"}, {"Intelligence", "2"}, {"Spirit", "2"}}},
        {"Frogpeople", "Class: Freshwater Frog", 1, {{"Strength", "5.0"}, {"Endurance", "5"}, {"Agility", "15"}, {"Wisdom", "3"}, {"Intelligence", "3"}, {"Spirit", "3"}}}
    };
}

// Placeholder Functions
void shopFunction() {
    // Functionality to be added
}

void gameFunction() {
    srand(time(0)); // Seed random number generator

    string answer;
    vector<Settlement> settlements;
    vector<Beast> beasts;
    vector<Enemy> enemies;

    // Choose Environment
    cout << "Choose an environment (Plains, Forests, Caves) or type 'random': ";
    cin >> answer;
    transform(answer.begin(), answer.end(), answer.begin(), ::tolower);

    string chosenEnvironment;
    if (answer == "random") {
        vector<string> availableEnvironments = {"Plains", "Forests", "Caves"};
        chosenEnvironment = availableEnvironments[rand() % availableEnvironments.size()];
    } else {
        chosenEnvironment = answer;
    }

    cout << "\nEnvironment chosen: " << chosenEnvironment << "\n";

    // Generate Settlements
    int settlementCount = rand() % 5 + 1; // Random 1-5 settlements
    for (int i = 0; i < settlementCount; ++i) {
        Settlement settlement;
        settlement.type = "Small Village";
        settlement.size = "Small";
        settlement.enemyCount = rand() % 20 + 10; // Random 10-30 enemies
        settlement.x = (rand() % 20 - 10) * 1.0; // Random x position (-10 to 10 km)
        settlement.y = (rand() % 20 - 10) * 1.0; // Random y position (-10 to 10 km)

        settlements.push_back(settlement);
        cout << "Generated settlement at (" << settlement.x << ", " << settlement.y << ") with " << settlement.enemyCount << " enemies.\n";
    }

    // Spawn Beasts
    beastsFunction(beasts);
    cout << "Spawning beasts (Rats) near core...\n";

    // Simulate Beast Attack on Settlements
    for (auto& settlement : settlements) {
        cout << "Rats attacking settlement at (" << settlement.x << ", " << settlement.y << ")!\n";

        while (settlement.enemyCount > 0 && !beasts.empty()) {
            settlement.enemyCount -= 1; // One enemy killed per beast attack
            beasts.pop_back(); // One beast dies in the attack
            cout << "Enemy killed! Remaining enemies: " << settlement.enemyCount << "\n";
        }

        if (settlement.enemyCount == 0) {
            cout << "Settlement destroyed!\n";
        } else if (beasts.empty()) {
            cout << "All beasts have been exterminated! Settlement survives.\n";
        }
    }

    // Check if Core is Vulnerable
    if (beasts.empty()) {
        cout << "Core is vulnerable! Enemies can attack now.\n";
    } else {
        cout << "Core is protected by remaining beasts.\n";
    }
}

void killEnemy(const Enemy& enemy){

    int earnedEnergy = enemy.levels;
    totalEnergy += earnedEnergy;
    totalKills++;
};

void generateSettlements(vector<Settlement>& settlements, int numSettlements, const string& environment) {
    vector<string> settlementNames = {"Outpost", "Village", "Town", "City"};
    vector<string> settlementSizes = {"Small", "Medium", "Large"};
    
    for (int i = 0; i < numSettlements; ++i) {
        Settlement settlement;
        settlement.name = settlementNames[rand() % settlementNames.size()];
        settlement.size = settlementSizes[rand() % settlementSizes.size()];
        settlement.enemyCount = rand() % 20 + 10;  // Random enemy count between 10-30
        settlement.x = (rand() % 20 - 10) * 1.0;    // Random X (-10 to 10)
        settlement.y = (rand() % 20 - 10) * 1.0;    // Random Y (-10 to 10)
        
        // Assign special resources based on environment
        assignSpecialResources(settlement, environment);
        
        settlements.push_back(settlement);
    }
}


void attackSettlement(Settlement& settlement) {
    cout << "Attacking settlement: " << settlement.name << " (" << settlement.size << ")\n";
    cout << "Enemy count: " << settlement.enemyCount << "\n";
    
    // Simulate killing enemies in the settlement
    for (int i = 0; i < settlement.enemyCount; ++i) {
        string enemyRace = "Enemy_" + to_string(i + 1);  // Simple placeholder
        int enemyLevel = rand() % 3 + 1;  // Random level between 1-3
        killEnemy(enemyRace, enemyLevel);  // Kill the enemy
    }

    cout << "Settlement " << settlement.name << " destroyed!\n";

    // Collect special resources from the destroyed settlement
    collectResources(settlement);
}


void assignSpecialResources(Settlement& settlement, const string& environment) {
    if (environment == "Plains") {
        settlement.specialResources = {"Wind Spirit", "Earth Spirit"};
    } else if (environment == "Mountains") {
        settlement.specialResources = {"Rock Spirit", "Rare Metals"};
    } else if (environment == "Forests") {
        settlement.specialResources = {"Nature Spirit", "Rare Woods"};
    } else if (environment == "Lakes") {
        settlement.specialResources = {"Water Spirit", "Rare Waters"};
    } else if (environment == "Oceans") {
        settlement.specialResources = {"Fire Spirit", "Rare Waters"};
    } else if (environment == "Deserts") {
        settlement.specialResources = {"Fire Spirits", "Unstable Spirits"};
    } else if (environment == "Caves") {
        settlement.specialResources = {"Rare Metals", "Fossils"};
    }
}

void collectResources(const Settlement& settlement) {
    cout << "Collecting special resources from " << settlement.name << "...\n";
    for (const auto& resource : settlement.specialResources) {
        collectedResources.push_back(resource);
        cout << "Collected: " << resource << "\n";
    }
}

