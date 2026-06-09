#include <iostream>
#include <string>
#include <random>
using namespace std;

bool running = true;

//Direction Memory
string direction;
int directionNumber = 0;

// Move or Turn Choices
string choice;
string skillChoice;

string movement;
string turn;

// CORDS
int hozDir; // Horziontal Direction
int verDir; // Vertical Direction
int x = 5;
int y = 0;
bool moved;
//Cord Checker
int hozDir2;
int verDir2;
int x2 = 0;
int y2 = 0;

// Checks
bool inputCheck = false;
bool hitWall = false;
bool solidEventTriggered = false; //An action square you can't go past.

// Items and Stuff
float treasureWorth = 0;

// Player Info
string classType;
bool classSelectionScreenCheck = false;

int playerStatVIT;
int playerStatSTR;
int playerStatDEF;
int playerStatSPD;

int playerHealth = 0;
int playerMana = 0;
int damageOutput;
int playerMaxMana;

string weaponEquipped;
int weaponATK;
int weaponSPD;

string skillOne, skillTwo, skillThree;
int skill1Cost, skill2Cost, skill3Cost;
int skill1Damage, skill2Damage, skill3Damage;

int healthPotions = 0;
int resetBuffCounter;

//Enemy Info

string currentMonster;

int goblinStatVIT = 10;
int goblinStatATK = 8;
int goblinStatDEF = 5;
int goblinStatSPD = 6;

int goblinHealth = goblinStatVIT * 5;

int batStatVIT = 6;
int batStatATK = 5;
int batStatDEF = 2;
int batStatSPD = 10;

int batHealth = batStatVIT * 4;

int skeletonStatVIT = 12;
int skeletonStatATK = 10;
int skeletonStatDEF = 8;
int skeletonStatSPD = 4;

int skeletonHealth = skeletonStatVIT * 6;

int currentMonsterHealth;

int currentMonsterStatATk;
int currentMonsterStatDEF;
int currentMonsterStatSPD;

// Combat Stuff
int turnCounter = 0;
string combatChoice;
bool inCombat = false;

int hitChanceNumber;
int dodgeChanceNumber;
bool specialSKillUsed = false;
// Other Sections

void classSelectionScreen();
void resetAllUsedVariables();
void mapMovement();
void weaponFoundScreen();
void combatSystem();
void combatSkillScreen();
void specialSkillsSection();
int diceRoll();

//-------------------------------------------------------------------

int main()
{

    direction = "North";

    cout << "You can use shorthand for commands. l for left for example.\n"
        << "It' s always the first letter, lowercase.\n\n";

    cout << "Before beginning your adventure, what class are you?\n\n"
        << " Warrior\n Mage\n Thief\n\n> ";

	classSelectionScreen();

    while (running)
    {
        //Conditional Checks reset
		resetAllUsedVariables();

        cout << "\nWhat do you wish to do? Here are the choices.\n\n";
        cout << " Turn (t) \n Move (m) \n Map (d) \n Inventory (i)\n" << endl << endl << "> ";

        getline(cin, choice); cout << endl;
		mapMovement();

        if (moved)
        {
            // Random Encounters
            int encounterRoll = diceRoll();
            if (encounterRoll <= 6) // 30% chance
            {
                inCombat = true;
                turnCounter = 0;
                // Determine which monster appears
                int monsterRoll = diceRoll();
                if (monsterRoll <= 10) // 50% chance
                {
                    currentMonster = "Goblin";
                    currentMonsterHealth = goblinHealth;
                    currentMonsterStatATk = goblinStatATK;
                    currentMonsterStatDEF = goblinStatDEF;
                    currentMonsterStatSPD = goblinStatSPD;
                }
                else if (monsterRoll <= 16) // 30% chance
                {
                    currentMonster = "Bat";
                    currentMonsterHealth = batHealth;
                    currentMonsterStatATk = batStatATK;
                    currentMonsterStatDEF = batStatDEF;
                    currentMonsterStatSPD = batStatSPD;
                }
                else // 20% chance
                {
                    currentMonster = "Skeleton";
                    currentMonsterHealth = skeletonHealth;
                    currentMonsterStatATk = skeletonStatATK;
                    currentMonsterStatDEF = skeletonStatDEF;
                    currentMonsterStatSPD = skeletonStatSPD;
                }
                cout << "A " << currentMonster << " stands in your way!\n";
                combatSystem();
            }

            if (encounterRoll >= 7 && encounterRoll <= 9) // 10% chance
            {
                int foundTreasure = (diceRoll() * 10);

                cout << "You found a hidden treasure chest.\n";
				int treasureRoll = diceRoll();
                if (treasureRoll <= 10) {
                    healthPotions += 1; treasureWorth += foundTreasure/2;
                    cout << "Inside, you found a health potion and $" << foundTreasure/2 << " worth of treasure!\n";
                }
                else if (treasureRoll > 10 && treasureRoll <= 15) {
					weaponFoundScreen();

					cout << "\nAlongside the weapon, you find a little extra cash.\n";
                    treasureWorth += foundTreasure / 4;
					cout << "You found $" << foundTreasure/4 << " worth of treasure to be exact!\n";
				}
                else {
                    treasureWorth += foundTreasure;
                    cout << "Inside you found $" << foundTreasure << " worth of treasure!\n";
				}
            }

            if (encounterRoll == 10) // 5% chance
            {
                cout << "You found a healing fountain and restored some health!\n";
                int healAmount = playerStatVIT * 5;
                playerHealth += healAmount;
                cout << "You restored " << healAmount << " health points.\n";
            }


        }
	}
    return 0;
}

void classSelectionScreen()
{
    while (!classSelectionScreenCheck)
    {
        getline(cin, classType);
        if (classType == "Warrior" || classType == "warrior" || classType == "w") {
            classType = "Warrior";
            classSelectionScreenCheck = true;
        }
        else if (classType == "Mage" || classType == "mage" || classType == "m") {
            classType = "Mage";
            classSelectionScreenCheck = true;
        }
        else if (classType == "Thief" || classType == "thief" || classType == "t") {
            classType = "Thief";
            classSelectionScreenCheck = true;
        }
        else {
            cout << "\nThat is not a valid class. Please choose again.\n\n> ";
        }
    }

    cout << endl << "I see, so you are a " << classType << ". I wish you the best of luck.\n\n";

    if (classType == "Thief") {//Fast type- Relies on dodging and critcal hits.
        treasureWorth += 50; // Thieves start with some treasure
        playerStatDEF = 4;
        playerStatSPD = 12;
        playerStatSTR = 9;
        playerStatVIT = 5;

        playerHealth = 10 * playerStatVIT;
        playerMana = 25;
        playerMaxMana = playerMana;


        skillOne = "Backstab";
        skillTwo = "Steal";
        skillThree = "Vanish";

        skill1Cost = 5;
        skill2Cost = 8;
        skill3Cost = 12;

		skill1Damage = 15;
		skill2Damage = 0;
		skill3Damage = 0;

        weaponEquipped = "Dagger";
        weaponATK = 5;
        weaponSPD = 3;

        cout << "As a thief, you should be quite fast but a little lacking in physical strength.\n"
            << " And it seems you came in here with about $50 worth of stolen goods.\n\n";
    }
    if (classType == "Mage") {//Glass cannon- Relies on skills.
        playerStatDEF = 3;
        playerStatSPD = 8;
        playerStatSTR = 15;
        playerStatVIT = 3;

        playerHealth = 10 * playerStatVIT;
        playerMana = 50;
        playerMaxMana = playerMana;


        skillOne = "Fireball";
        skillTwo = "Ice Spike";
        skillThree = "Lightning Bolt";

        skill1Cost = 10;
        skill2Cost = 12;
        skill3Cost = 15;

		skill1Damage = 20;
		skill2Damage = 18;
		skill3Damage = 22;

        weaponEquipped = "Staff";
        weaponATK = 3;
        weaponSPD = 1;

        cout << "As a mage, you should be quite wise in dangerous arts known as magic.\n"
            << "Like all mages, you seemed to have neglected your physical training for quite some time. \n";
    }
    if (classType == "Warrior") {// Balanced type- All around stats minus speed.
        playerStatDEF = 10;
        playerStatSPD = 3;
        playerStatSTR = 13;
        playerStatVIT = 8;

        playerHealth = 10 * playerStatVIT;
        playerMana = 30;
        playerMaxMana = playerMana;


        skillOne = "Power Strike";
        skillTwo = "Shield Bash";
        skillThree = "War Cry";

        skill1Cost = 8;
        skill2Cost = 10;
        skill3Cost = 12;

		skill1Damage = 18;
		skill2Damage = 12;
		skill3Damage = 0;

        weaponEquipped = "Sword";
        weaponATK = 7;
        weaponSPD = -1;

        cout << "As a warrior, you are strong and resilient. It seems as if you've kept up in training.\n"
            << "But it seems that frame of your is holding your speed back quite a bit.\n\n";
    }

    playerHealth = 10 * playerStatVIT;
}

void resetAllUsedVariables()
{
    inputCheck = false;
    moved = false;
    hozDir = 0, verDir = 0;
    hozDir2 = 0, verDir2 = 0;
    x2 = x, y2 = y;
    solidEventTriggered = false;
}

void mapMovement()
{
    if (choice == "Turn" || choice == "turn" || choice == "t")
    {
        cout << "Ok, which way? (l, r, a)\n\n> ";
        getline(cin, turn);
        inputCheck = true;
    }

    if (choice == "Move" || choice == "move" || choice == "m")
    {
        cout << "Ok, forward or backwards? (f ,b)\n\n> ";
        getline(cin, movement);
        moved = true;
        inputCheck = true;
    }

    if (choice == "Map" || choice == "map" || choice == "d")
    {
        cout << "You look at your old map and see the layout of the dungeon:\n";
        cout << R"(
                @@@@@@@@@@@@%%@@@@@@@@@@@@
                @@......@@..............@@
                @@..@@......@@@@@@@@@@..@@
                @@..@@..@@..@@......@@..@@
                @@..@@@@@@@@@@@@@@......@@
                @@..@@..@@......@@@@@@..@@
                @@@@@@..@@..@@..@@......@@
                @@@@@@..@@..@@..@@......@@
                @@......@@@@@@......@@@@@@
                @@..@@@@@@..@@..@@@@@@..@@
                @@..@@......@@......@@..@@
                @@..@@..@@..@@..@@..@@..@@
                @@......@@..............@@
                @@@@@@@@@@@@%%@@@@@@@@@@@@
                )";

        cout << "\nIt feels as if you came in from the South.\n";

        inputCheck = true;
    }
    if (choice == "Inventory" || choice == "inventory" || choice == "i")
    {
        cout << "You check your inventory:\n"
            << "- Class Type: " << classType << "\n"
            << "- Health Potions: " << healthPotions << "\n"
            << "- Treasure Worth: $" << treasureWorth << "\n"
            << "- Weapon Equipped: " << weaponEquipped << " (ATK: " << weaponATK << ", SPD: " << weaponSPD << ")\n"
            << "- Stats: STR: " << playerStatSTR << ", DEF: " << playerStatDEF << ", SPD: " << playerStatSPD << ", VIT: " << playerStatVIT << "\n"
            << "- Health: " << playerHealth << "\n"
            << "- Mana: " << playerMana << "\n";
        inputCheck = true;
	}

    if (inputCheck == true) {

        //KEEPS TRACK OF PLAYER DIRECTION

        if (turn == "left" || turn == "l")
        {
            directionNumber = -1 + directionNumber;
        }
        else if (turn == "right" || turn == "r")
        {
            directionNumber = 1 + directionNumber;
        }
        else if ((turn == "turn around" || turn == "Turn around" || turn == "a") && directionNumber <= 0)
        {
            directionNumber = 2 + directionNumber;
        }
        else if ((turn == "turn around" || turn == "Turn around" || turn == "a") && directionNumber > 0)
        {
            directionNumber = -2 + directionNumber;
        }
        else { inputCheck = false; }

        if (directionNumber == 3) { directionNumber = -1; }
        if (directionNumber == -3) { directionNumber = 1; }

        if (directionNumber == 0) { direction = "North"; }
        if (directionNumber == 1) { direction = "East"; }
        if (directionNumber == 2 || directionNumber == -2) { direction = "South"; }
        if (directionNumber == -1) { direction = "West"; }

        // Movement Check (Ghost Cords)

        if (moved)
        {

            if (direction == "North") { verDir2 = 1; }
            if (direction == "East") { hozDir2 = 1; }
            if (direction == "West") { hozDir2 = -1; }
            if (direction == "South") { verDir2 = -1; }

            if (movement == "forward" || movement == "f") {
                x2 = x2 + (hozDir2);
                y2 = y2 + (verDir2);
            }
            else if (movement == "backward" || movement == "b") {
                x2 = x2 - (hozDir2);
                y2 = y2 - (verDir2);
            }
            else { inputCheck = false; }
        }

        // Exit

        if (x2 == 5 && y2 == 12)
        {
            cout << endl << "CONGRATS. You've made it to the end. \n"
                << "Do wish to end the game?\n\n"
                << " Yes\n No\n\n> ";

            getline(cin, choice);

            if (choice == "Yes" || choice == "yes" || choice == "y")
            {
                cout << endl << "You decided to leave.\nYour journey now compelete."
                    << endl << "You made it out alive with $" << treasureWorth << " worth of treasure."
                    << endl << endl;

                running = false;
                exit(0);
                return;

            }

            else if (choice == "No" || choice == "no" || choice == "n")
            {
                cout << endl << "Ok, it seems as if greed stills binds your soul.\n";
                solidEventTriggered = true;
            }

            else {
                cout << endl << "That's not a choice, so I'll take that as a no.\n";
                solidEventTriggered = true;
            }
        }

        // Entrance

        if (x2 == 5 && y2 == -1)
        {
            cout << endl << "Your at the entrance. It's where you came from.\n"
                << "Do want to return? (This will end the game.)\n\n"
                << " Yes\n No\n\n> ";

            getline(cin, choice);

            if (choice == "Yes" || choice == "yes" || choice == "y")
            {
                cout << endl << "You decided to leave.\nYour journey ended before it even started.";

                running = false;
                exit(0);
                return;
            }

            else if (choice == "No" || choice == "no" || choice == "n")
            {
                cout << endl << "Ok, Good Choice.\n";
                solidEventTriggered = true;
            }

            else {
                cout << endl << "That's not a choice, so I'll take that as a no.\n";
                solidEventTriggered = true;
            }
        }

        //WALLS - USES GHOST CORDS
        //Outer Walls
        if ((x2 > 11 || x2 < 0 || y2 > 11 || y2 < 0) && !solidEventTriggered)
        {
            hitWall = true;
        }
        else { hitWall = false; }
        // Maze Walls
        /* Done by hand. Because I'm still learning
        11 x 11 maze. But x starts at 0. And y starts at 1.
        */
        if (!solidEventTriggered) {
            if ((y2 == 11) && (x2 == 3)) { hitWall = true; }
            else if ((y2 == 10) && (x2 == 1 || (x2 >= 5 && x2 <= 9))) { hitWall = true; }
            else if ((y2 == 9) && (x2 == 1 || x2 == 3 || x2 == 5 || x2 == 9)) { hitWall = true; }
            else if ((y2 == 8) && (x2 >= 1 && x2 <= 7)) { hitWall = true; }
            else if ((y2 == 7) && (x2 == 1 || x2 == 3 || (x2 >= 7 && x2 <= 9))) { hitWall = true; }
            else if ((y2 == 6) && (x2 == 0 || x2 == 1 || x2 == 3 || x2 == 5 || x2 == 7)) { hitWall = true; }
            else if ((y2 == 5) && (x2 == 9 || x2 == 10 || (x2 >= 3 && x2 <= 5))) { hitWall = true; }
            else if ((y2 == 4) && (x2 == 5 || (x2 >= 1 && x2 <= 3) || (x2 >= 7 && x2 <= 9))) { hitWall = true; }
            else if ((y2 == 3) && (x2 == 1 || x2 == 5 || x2 == 9)) { hitWall = true; }
            else if ((y2 == 2) && (x2 == 1 || x2 == 3 || x2 == 9 || (x2 >= 5 && x2 <= 7) || (x2 >= 7 && x2 <= 9))) { hitWall = true; }
            else if ((y2 == 1) && (x2 == 3)) { hitWall = true; }
            else { hitWall = false; }
        }



        // MOVEMENT X AND Y


        if (!hitWall && !solidEventTriggered)
        {
            if (moved)
            {

                if (direction == "North") { verDir = 1; }
                if (direction == "East") { hozDir = 1; }
                if (direction == "West") { hozDir = -1; }
                if (direction == "South") { verDir = -1; }

                if (movement == "forward" || movement == "f") {
                    x = x + (hozDir);
                    y = y + (verDir);
                }
                else if (movement == "backward" || movement == "b") {
                    x = x - (hozDir);
                    y = y - (verDir);
                }
                else { inputCheck = false; }
            }
            /*cout << endl << "X Cord is " << x << ". And Y Cord is " << y << endl
                << "Your facing " << direction << "." << endl << endl;*/
            cout << endl << "Your facing " << direction << "." << endl << endl;

            // AI SECTION. Checks around player for open paths. ----------------------------------------------------------
            auto IsBlockedAt = [&](int tx, int ty) -> bool
                {
                    if (solidEventTriggered) return false;

                    // Outer walls (bounds)
                    if ((tx > 11 || tx < 0 || ty > 11 || ty < 0))
                        return true;

                    // Maze Walls
                    if ((ty == 11) && (tx == 3)) { return true; }
                    else if ((ty == 10) && (tx == 1 || (tx >= 5 && tx <= 9))) { return true; }
                    else if ((ty == 9) && (tx == 1 || tx == 3 || tx == 5 || tx == 9)) { return true; }
                    else if ((ty == 8) && (tx >= 1 && tx <= 7)) { return true; }
                    else if ((ty == 7) && (tx == 1 || tx == 3 || (tx >= 7 && tx <= 9))) { return true; }
                    else if ((ty == 6) && (tx == 0 || tx == 1 || tx == 3 || tx == 5 || tx == 7)) { return true; }
                    else if ((ty == 5) && (tx == 9 || tx == 10 || (tx >= 3 && tx <= 5))) { return true; }
                    else if ((ty == 4) && (tx == 5 || (tx >= 1 && tx <= 3) || (tx >= 7 && tx <= 9))) { return true; }
                    else if ((ty == 3) && (tx == 1 || tx == 5 || tx == 9)) { return true; }
                    else if ((ty == 2) && (tx == 1 || tx == 3 || tx == 9 || (tx >= 5 && tx <= 7) || (tx >= 7 && tx <= 9))) { return true; }
                    else if ((ty == 1) && (tx == 3)) { return true; }

                    return false;
                };

            // Ghost coords for each direction
            bool openNorth = !IsBlockedAt(x, y + 1);
            bool openEast = !IsBlockedAt(x + 1, y);
            bool openSouth = !IsBlockedAt(x, y - 1);
            bool openWest = !IsBlockedAt(x - 1, y);

            cout << "Open around you: ";
            bool any = false;
            if (openNorth) { cout << "North "; any = true; }
            if (openEast) { cout << "East ";  any = true; }
            if (openSouth) { cout << "South "; any = true; }
            if (openWest) { cout << "West ";  any = true; }
            if (!any) cout << "None (surrounded)";

            cout << endl << endl;
        }
        //--------------------------------------------------------------------------------------------------
        // End of AI SECTION
        //Comment:
        /* When doing this project I attempted to not use ai as much as possible.
        But due to my faultly logic system,impleting such a feature was going to
        be a pain. Since it wasn't really required, I just let the AI handle it.
        And as you can see, it is based on my eariler coding style. (At least)*/

        else if (hitWall)
        {
            cout << endl << "You've hit a wall, facing " << direction << endl << endl;
        }


    }

    else { cout << endl << "Sorry that isn't a choice." << endl << endl; }
    turn = "";
}

void weaponFoundScreen()
{
	string oldWeaponEquipped = weaponEquipped;
	int oldWeaponATK = weaponATK;
	int oldWeaponSPD = weaponSPD;

	int weaponRoll = diceRoll();
    if (classType == "Warrior") {
        if (weaponRoll <= 5) {
            weaponEquipped = "Iron Sword";
            weaponATK = 6;
            weaponSPD = 0;
        }
        else if (weaponRoll <= 10) {
            weaponEquipped = "Battle Axe";
            weaponATK = 8;
            weaponSPD = -1;
        }
        else if (weaponRoll <= 15) {
            weaponEquipped = "Warhammer";
            weaponATK = 9;
            weaponSPD = -2;
        }
        else if (weaponRoll <= 19) {
            weaponEquipped = "Halberd";
            weaponATK = 11;
            weaponSPD = -2;
        }
        else {
            weaponEquipped = "Divine Greatsword";
            weaponATK = 14;
            weaponSPD = -3;
        }
    }
    else if (classType == "Mage") {
        if (weaponRoll <= 5) {
            weaponEquipped = "Apprentice Staff";
            weaponATK = 5;
            weaponSPD = 0;
        }
        else if (weaponRoll <= 10) {
            weaponEquipped = "Oak Wand";
            weaponATK = 6;
            weaponSPD = 1;
        }
        else if (weaponRoll <= 15) {
            weaponEquipped = "Crystal Rod";
            weaponATK = 7;
            weaponSPD = 0;
        }
        else if (weaponRoll <= 19) {
            weaponEquipped = "Runic Orb";
            weaponATK = 9;
            weaponSPD = 1;
        }
        else {
            weaponEquipped = "Elder Wand";
            weaponATK = 11;
            weaponSPD = 2;
        }
    }
    else if (classType == "Thief") {
        if (weaponRoll <= 5) {
            weaponEquipped = "Dagger";
            weaponATK = 4;
            weaponSPD = 3;
        }
        else if (weaponRoll <= 10) {
            weaponEquipped = "Twin Daggers";
            weaponATK = 5;
            weaponSPD = 4;
        }
        else if (weaponRoll <= 15) {
            weaponEquipped = "Short Sword";
            weaponATK = 6;
            weaponSPD = 2;
        }
        else if (weaponRoll <= 19) {
            weaponEquipped = "Kukri";
            weaponATK = 7;
            weaponSPD = 1;
        }
        else {
            weaponEquipped = "Shadow Fang";
            weaponATK = 9;
            weaponSPD = 4;
        }
    }

	cout << "You found a new weapon: " << weaponEquipped << " (ATK: " << weaponATK << ", SPD: " << weaponSPD << ")\n";
	cout << "Do you want to equip it? (y/n)\n\n> ";

	getline(cin, choice);

    if (choice == "Yes" || choice == "yes" || choice == "y")
    {
        cout << "You equipped the " << weaponEquipped << ".\n";
    }
	else if (choice == "No" || choice == "no" || choice == "n")
    {
        cout << "You decided to leave the " << weaponEquipped << " behind.\n";
		weaponEquipped = oldWeaponEquipped;
		weaponATK = oldWeaponATK;
		weaponSPD = oldWeaponSPD;
	}
}

void combatSystem()
{
    turnCounter = 0;
    int normalSTR = playerStatSTR;
    int normalDEF = playerStatDEF;
    int normalSPD = playerStatSPD;
    int normalMana = playerMaxMana; // use true max
    resetBuffCounter = 0;

    while (inCombat)
    {
        // snapshot mana at start of this turn
        int manaAtTurnStart = playerMana;

        turnCounter++;

        if (resetBuffCounter == turnCounter) {
            playerStatDEF = normalDEF; // Reset boost
            playerStatSPD = normalSPD;
            playerStatSTR = normalSTR;
        }
        cout << "\n--- Turn " << turnCounter << " ---\n";
        cout << "Your Health: " << playerHealth << " | " << currentMonster << " Health: " << currentMonsterHealth << "\n\n";
        cout << "What will you do?\n"
            << " Attack (a)\n Defend (d)\n Use Skill (s)\n Use Potion (p)\n Flee (f)\n\n> ";
        getline(cin, combatChoice);
        if (combatChoice == "Attack" || combatChoice == "attack" || combatChoice == "a")
        {
            // Player attacks
            int attackRoll = diceRoll();

            cout << "\nYou attack the " << currentMonster << " and roll a " << attackRoll << ".\n";

            int damageDealt = playerStatSTR + weaponATK + attackRoll - currentMonsterStatDEF;

            int enemyDodgeRoll = diceRoll();
            if (enemyDodgeRoll + currentMonsterStatSPD > playerStatSPD + 10) {
                cout << "The " << currentMonster << " was able to dodge your attack!\n";
                damageDealt = 0;
            }
            else {
                if (damageDealt < 0) damageDealt = 0;
                currentMonsterHealth -= damageDealt;
                cout << "You deal " << damageDealt << " damage!\n";
                if (currentMonsterHealth <= 0)
                {
                    cout << "You have defeated the " << currentMonster << "!\n";
                    inCombat = false;
                    break;
                }
            }

        }
        else if (combatChoice == "Defend" || combatChoice == "defend" || combatChoice == "d")
        {
            cout << "\nYou brace yourself for the next attack, increasing your defense this turn.\n";
            playerStatDEF += 5; // Temporary defense boost
            resetBuffCounter = turnCounter + 1;
        }
        else if (combatChoice == "Use Skill" || combatChoice == "use skill" || combatChoice == "s")
        {
            cout << "\nYou prepare to use a skill.\n You have " << playerMana << " mana remaining.\n";
            combatSkillScreen();
            if (currentMonsterHealth <= 0)
            {
                cout << "You have defeated the " << currentMonster << "!\n";
                inCombat = false;
                break;
            }
        }
        else if (combatChoice == "Use potion" || combatChoice == "use potion" || combatChoice == "p")
        {
            if (healthPotions > 0)
            {
                cout << "You used a health potion. It healed you by 20 points.\n";
                playerHealth += 20;
                healthPotions -= 1;
                cout << "You have " << healthPotions << " left.";
            }
            else {
                cout << "You don't have any potions.\n";
            }
        }
        else if (combatChoice == "Flee" || combatChoice == "flee" || combatChoice == "f")
        {
            int fleeRoll = diceRoll();
            if (fleeRoll + playerStatSPD > currentMonsterStatSPD + 12) {
                cout << "\nYou successfully fled from the " << currentMonster << "!\n";
                inCombat = false;
                break;
            }
            else {
                cout << "\nYou failed to flee!\n";
            }
        }
        else
        {
            cout << "\nThat's not Choice!\n";
        }

        // Monster's Turn to Attack
        int dodgeRoll = diceRoll();
        if (dodgeRoll + playerStatSPD > currentMonsterStatSPD + 10) {
            cout << "The " << currentMonster << " attempts to attack you. You where able to dodge.\n";
        }
        else {
            // Monster attacks
            int monsterAttackRoll = diceRoll();

            cout << "\nThe " << currentMonster << " attacks you and rolls a " << monsterAttackRoll << ".\n";

            int damageTaken = currentMonsterStatATk + monsterAttackRoll - playerStatDEF;

            if (damageTaken < 0) damageTaken = 0;
            playerHealth -= damageTaken;
            cout << "You take " << damageTaken << " damage!\n";
            if (playerHealth <= 0)
            {
                cout << "You have been defeated by the " << currentMonster << ". Game Over.\n";
                exit(0);
            }
        }

        // regen: only if player didn't spend mana this turn and is below max
        if ((playerMana < normalMana) && (playerMana >= manaAtTurnStart))
        {
            int regen = (normalMana + 4) / 5; // 20% of max rounded up
            playerMana += regen;
            if (playerMana > normalMana) playerMana = normalMana;
        }
    }
}

void combatSkillScreen()
{
    cout << "\nChoose a skill to use:\n"
        << " 1. " << skillOne << " (Cost: " << skill1Cost << " Mana)\n"
        << " 2. " << skillTwo << " (Cost: " << skill2Cost << " Mana)\n"
        << " 3. " << skillThree << " (Cost: " << skill3Cost << " Mana)\n\n> ";

    skillChoice;
    getline(cin, skillChoice);

	specialSKillUsed = false;
	int skillDamageVariation = (diceRoll())/2; // Random variation for skill damage

    if (skillChoice == "1" || skillChoice == skillOne)
    {
        if (playerMana >= skill1Cost)
        {
            playerMana -= skill1Cost;
            int damageDealt = skillDamageVariation + skill1Damage - currentMonsterStatDEF;
            if (damageDealt < 0) damageDealt = 0;
            currentMonsterHealth -= damageDealt;
            cout << "\nYou used " << skillOne << " and dealt " << damageDealt << " damage!\n";
        }
        else
        {
            cout << "\nNot enough mana!\n";
        }
    }
    else if (skillChoice == "2" || skillChoice == skillTwo)
    {
        if (playerMana >= skill2Cost)
        {

            playerMana -= skill2Cost;
            specialSkillsSection();

            int damageDealt = skillDamageVariation + skill2Damage - currentMonsterStatDEF;
            if (damageDealt < 0 && !specialSKillUsed) {
                damageDealt = 0;
                cout << "You used the Skill, " << skillTwo << " but it wasn't effective!\n";

            }
            else {
                currentMonsterHealth -= damageDealt;
                cout << "\nYou used " << skillTwo << " and dealt " << damageDealt << " damage!\n";
            }
        }
        else
        {
            cout << "\nNot enough mana!\n";
        }
    }
    else if (skillChoice == "3" || skillChoice == skillThree)
    {
        if (playerMana >= skill3Cost)
        {
            playerMana -= skill3Cost;
            specialSkillsSection();
            int damageDealt = skillDamageVariation + skill3Damage - currentMonsterStatDEF;
            if (damageDealt < 0 && !specialSKillUsed) {
                damageDealt = 0;
                cout << "You used the Skill, " << skillThree << " but it wasn't effective!\n";

            }
            else {
                currentMonsterHealth -= damageDealt;
                cout << "\nYou used " << skillThree << " and dealt " << damageDealt << " damage!\n";
            }
        }
        else
        {
            cout << "\nNot enough mana!\n";
        }
    }
    else
    {
        cout << "\nThat's not a valid skill!\n";
    }
}

void specialSkillsSection()
{
    if (skillTwo == "Steal" && skillChoice == "2")
    {
		specialSKillUsed = true;
        int stealRoll = diceRoll();
        if (stealRoll > 10)
        {
            float stolenAmount = (diceRoll() * 5);
            treasureWorth += stolenAmount;
            cout << "You successfully stole $" << stolenAmount << " worth of treasure from the " << currentMonster << "!\n";
        }
        else
        {
            cout << "You failed to steal anything from the " << currentMonster << ".\n";
        }
    }
    else if (skillThree == "Vanish" && skillChoice == "3")
    {
        specialSKillUsed = true;
        int vanishRoll = diceRoll();
        if (vanishRoll > 12)
        {
            playerStatSPD += 10; // Temporary speed boost
            cout << "You successfully vanished from sight, increasing your speed for a few turns!\n";
            resetBuffCounter = turnCounter + 3;
        }
        else
        {
            cout << "You failed to vanish!\n";
        }
    }
    else if (skillThree == "War Cry" && skillChoice == "3")
    {
        specialSKillUsed = true;

        cout << "You let out a mighty war cry, boosting your morale!\n";
        playerStatSTR += 5; // Temporary strength boost
		resetBuffCounter = turnCounter + 3;
    }
}
int diceRoll(){
    random_device rd;  // Obtain a random number from hardware
    mt19937 eng(rd()); // Seed the generator
    uniform_int_distribution<> distr(1, 20); // Define the range
    return distr(eng); //The REAL MVP of this Project.
}

//End of Code. Dungeon Keeper.cpp
//Hoorah!
/*Just writing a note at the bottom to reach 1000 lines.
However sketchy or down right horrible this code appears to written,
it's mine, and it's my first. I'm pretty proud of it.
It works and it does so without error, so that's a win.
Especially for someone who just learned C++ not even 3 months ago.
Well, this line 1005. Shout out to my goat "Int diceRoll();" He was a real one.*/

//Update from Current me. Half of the features implemented are bugged and either don't work or act in unintended ways.
