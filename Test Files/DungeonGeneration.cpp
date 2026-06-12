#include <iostream>
#include <vector>
#include <random>
using namespace std;

int diceRoll(int min, int max);

class Dungeon {
private:
    int rows;
    int cols;
    vector<vector<int> > dungeonLayout;

public:
    Dungeon(int r, int c) : rows(r), cols(c), dungeonLayout(r, vector<int>(c, 99)) {}

    void generateFirstRow();
    void generateExit();
    void generateDungeon();
    void FillGaps();

    const vector<vector<int> >& getLayout() const { return dungeonLayout; }
};

int main() {
    Dungeon dungeon(12, 13);
    dungeon.generateFirstRow();
    dungeon.generateExit();
    dungeon.generateDungeon();
    dungeon.FillGaps();

    const auto& layout = dungeon.getLayout();
    for (int i = 0; i < layout.size(); i++)
    {
        for (int j = 0; j < layout[i].size(); j++)
        {
            cout << layout[i][j] << " ";
        }
        cout << endl;
    }
    system("pause");
    return 0;
}

void Dungeon::generateFirstRow() {
    dungeonLayout[rows-(rows-1)][0] = 9;
    dungeonLayout[rows-(rows-1)][cols-1] = 9;
    //(Test row location) dungeonLayout[rows-(rows-1)][i] = 0;
    int LastWallPlacement = 1; //Kept outside to prevent reset.
    bool UpwardsMobilitySquareGenerated = false; //Check to generate exit.
    bool DownwardsMobilitySquareGenerated = false; //Check to make sure row is accessible
    int VerticalMovementCounter = 0; //To prevent too much vertical movement.
    for (int i = 1; i < ((dungeonLayout[rows-(rows-1)].size()) - 1); i++)
    {
        int LocationValue = 99; //Used to set the location value of the square.
        int PreviousLocationValue = dungeonLayout[rows-(rows-1)][i-1]; //Keeps track of previous location value.

        if (PreviousLocationValue == 0 || PreviousLocationValue == 9)
        {
            if (VerticalMovementCounter >= 3)
            {
                if (!UpwardsMobilitySquareGenerated == false)
                {
                    LocationValue = diceRoll(0,2);
                    if (LocationValue == 2){LocationValue = 9;}
                }
                else
                {
                    LocationValue = diceRoll(0,3);
                }

            }
            else {
                LocationValue = diceRoll(0,1);
                if (LocationValue == 1){LocationValue = 9;}
            }
        }
        else{
            LocationValue = diceRoll(0,1);
            if (LocationValue == 1){LocationValue = 9;}
        }


        if (LocationValue == 3)
        {
            dungeonLayout[rows-(rows-1)][i] = 3;
            UpwardsMobilitySquareGenerated = true;
            VerticalMovementCounter++;
        }
        else if (LocationValue == 2)
        {
            dungeonLayout[rows-(rows-1)][i] = 2;
            UpwardsMobilitySquareGenerated = true;
            VerticalMovementCounter++;
        }

        else if (LocationValue == 9){
            dungeonLayout[rows-(rows-1)][i] = 9;
            int LastWallDistance = (i - LastWallPlacement)-1;//Count Empty Spaces, not including the wall itself.

            if (LastWallDistance == 1)
            {
                if ( UpwardsMobilitySquareGenerated == false )
                {
                    dungeonLayout[rows-(rows-1)][i-1] = 3;
                    UpwardsMobilitySquareGenerated = true;
                }
                else
                {
                    dungeonLayout[rows-(rows-1)][i-1] = 1;
                }
            }
            else if (LastWallDistance > 1){
                for (int j = LastWallPlacement; j < i; j++)
                {
                    dungeonLayout[rows-(rows-1)][j] = 0;
                }
                if (UpwardsMobilitySquareGenerated == false)
                {
                    dungeonLayout[rows-(rows-1)][(i-LastWallPlacement)+1] = 2;
                    UpwardsMobilitySquareGenerated = true;
                }
                dungeonLayout[rows-(rows-1)][i-1] = 1;

            }
            LastWallPlacement = i;
        }

        else if (LocationValue == 1 && VerticalMovementCounter <= 3)
        {
            if (PreviousLocationValue == 1){
                dungeonLayout[rows-(rows-1)][i] = 0;
            }
            else{
            dungeonLayout[rows-(rows-1)][i] = 1;
                DownwardsMobilitySquareGenerated = true;
                VerticalMovementCounter++;
            }
        }
        else if (LocationValue == 0)
        {
            dungeonLayout[rows-(rows-1)][i] = 0;
        }
    }
    //Tackling Air Pockets ------------------------------------------------------
    if (((cols-1)-LastWallPlacement > 1))
    {
        bool HoleInRightAirPocket = false;
        for (int i = LastWallPlacement; i < cols - 2; i++)
        {
            if ((dungeonLayout[rows-(rows-1)][i] == 1) || (dungeonLayout[rows-(rows-1)][i] == 3))
            {
                HoleInRightAirPocket = true;
                break;
            }
        }
        if (!HoleInRightAirPocket)
        {
            dungeonLayout[rows-(rows-1)][cols-2] = 1;
            dungeonLayout[rows-(rows-1)][cols-3] = 0;
        }
    }
    //Left Air Pocket
    int FirstWallLocation = 0;
    for (int i = 1; i < cols-1;i++)
    {
        if (dungeonLayout[rows-(rows-1)][i] == 9)
        {
            FirstWallLocation = i;
            break;
        }
    }
    if (FirstWallLocation >= 2){
        bool HoleInLeftAirPocket = false;
        for (int i = FirstWallLocation; i > 0; i--)
        {
            if ((dungeonLayout[rows-(rows-1)][i] == 1) || (dungeonLayout[rows-(rows-1)][i] == 3))
            {
                HoleInLeftAirPocket = true;
                break;
            }
        }
        if (!HoleInLeftAirPocket)
        {
            dungeonLayout[rows-(rows-1)][1] = 1;
            dungeonLayout[rows-(rows-1)][2] = 0;
        }
    }

    //Ensuring An Exit.
    // First a Check, since the orignal function wasn't correctly activating?
    UpwardsMobilitySquareGenerated = false;
    for (int i = 1; i < cols-1; i++)
    {
        if (dungeonLayout[rows-(rows-1)][i] == 2 || dungeonLayout[rows-(rows-1)][i] == 3)
        {
            UpwardsMobilitySquareGenerated = true;
            break;
        }
    }
    if (!UpwardsMobilitySquareGenerated)
    {
        dungeonLayout[rows-(rows-1)][1] = 3;
        dungeonLayout[rows-(rows-1)][2] = 0;
    }
}
//In some cases this still fails, the exit will be guaranteed to generate when running the mapping algorithm which checks to make the dungeon is possible.

void Dungeon::generateExit()
{
    for (int j = 0; j < cols; j++)
    {
        dungeonLayout[0][j] = 9;
    }

    for (int i = 0; i < cols; i++)
    {
        if (dungeonLayout[rows-(rows-1)][i] == 3 || dungeonLayout[rows-(rows-1)][i] == 2)
        {
            dungeonLayout[0][i] = 8;
        }
    }
}
//Problems for this function.  Walls should be more common. No vertical limiters. The Vertical Generators need to account for previous vertical generators.
void Dungeon::generateDungeon()
{
    //Generate inital Vertical Halls
    for (int i = 1; i < (dungeonLayout[rows-(rows-1)].size()-1); i++)
    {
        if (dungeonLayout[rows-(rows-1)][i] == 3 || dungeonLayout[rows-(rows-1)][i] == 1)
        {
            // Skip if adjacent column already has a vertical hall (prevents adjacent 3s)
            if (i > 1 && (dungeonLayout[rows-(rows-1)][i-1] == 3 || dungeonLayout[rows-(rows-1)][i-1] == 1)) continue;
            for (int m = 2; m < (dungeonLayout.size()-2); m++)
            {
            int WeightedLocationValue = diceRoll(0,15);
            if (WeightedLocationValue >= 7){WeightedLocationValue = 3;}
            else {WeightedLocationValue = 2; dungeonLayout[m+1][i] = 9;}

            dungeonLayout[m][i] = WeightedLocationValue;
            if ((m % 2 == 0) && WeightedLocationValue == 2){dungeonLayout[m][i] = 3; dungeonLayout[m+1][i] = 2; if (dungeonLayout[m+2][i] == 99) dungeonLayout[m+2][i] = 9;}

            if (WeightedLocationValue == 2){m = (dungeonLayout.size());}
            }
        }
    }
    int ChanceToSpawnSideHallway = 7;
    //^ This was bothering me so much. At first it was a coin flip, now it's adjustable. Hopefully this inspires some more side hallways.
    for (int r = 2; r < rows - 1; r++)
    {
        dungeonLayout[r][0] = 9;
        dungeonLayout[r][cols-1] = 9;
        for (int c = 1; c < cols; c++)
        {
            if ((dungeonLayout[r][c] == 3) && (r % 2 != 0))// if odd.
            {
                if (c > 1 && (diceRoll(0,10) <= ChanceToSpawnSideHallway))
                {//Left
                    dungeonLayout[r][c-1] = 0;
                    int VerticalCounter = 0;
                    int VerticalCounterMax = cols/7; //Intergers prevent decimals.
                    for (int i = c-2; (i > 0) && (dungeonLayout[r][i] == 99); i--)
                    {
                        int WeightedLocationValue = diceRoll(0,20);
                        if ((VerticalCounter <= VerticalCounterMax) && (WeightedLocationValue <= 10) && (dungeonLayout[r][i-1] == 0 || dungeonLayout[r][i-1] == 9) && (dungeonLayout[r+1][i] == 99))
                            {
                                VerticalCounter ++;
                                dungeonLayout[r][i] = 1;
                                dungeonLayout[r+1][i] = 3;
                                for (int j = r+2; (j < rows - 3) && (dungeonLayout[j][i] == 99); j++)
                                    {
                                    int VerticalWeightedLocationValue = diceRoll(0,20);
                                    int MiddlePoint = 8; //Quick Tool to edit chances.
                                    if (VerticalWeightedLocationValue <= MiddlePoint)
                                        {
                                        if (dungeonLayout[j][i] == 99) dungeonLayout[j][i] = 2;
                                        if (dungeonLayout[j+1][i] == 99) dungeonLayout[j+1][i] = 9;
                                        if (j % 2 == 0)
                                            {
                                            dungeonLayout[j][i] = 3;
                                            dungeonLayout[j+1][i] = 2;
                                            if (dungeonLayout[j+2][i] == 99) dungeonLayout[j+2][i] = 9;
                                            }
                                        }
                                    else if (VerticalWeightedLocationValue > MiddlePoint)
                                        {
                                            if (dungeonLayout[j][i] == 99) dungeonLayout[j][i] = 3;
                                        }
                                    }
                            }

                        else if (WeightedLocationValue > 10)
                            {
                            dungeonLayout[r][i] = 0;
                            if (dungeonLayout[r+1][i] == 99) dungeonLayout[r+1][i] = 9;
                            }
                        else
                            {
                            dungeonLayout[r][i] = 9;
                            break;
                        }
                    }
                }
                //right
                if (c < cols-1 && (diceRoll(0,10) <= ChanceToSpawnSideHallway))
                {
                    dungeonLayout[r][c+1] = 0;
                    int VerticalCounter = 0;
                    int VerticalCounterMax = cols / 7;
                    for (int i = c+2; (i < cols-1) && (dungeonLayout[r][i] == 99); i++)
                    {
                        int WeightedLocationValue = diceRoll(0,20);
                        if (((VerticalCounter <= VerticalCounterMax) && (WeightedLocationValue <= 10)) && (dungeonLayout[r][i-1] == 0 || dungeonLayout[r][i-1] == 9) && (dungeonLayout[r][i+1] == 99) && (dungeonLayout[r+1][i] == 99) && (dungeonLayout[r][i-1] == 99 || dungeonLayout[r][i-1] == 0 || dungeonLayout[r][i-1] == 9))
                        {
                            VerticalCounter++;
                            dungeonLayout[r][i] = 1;
                            dungeonLayout[r+1][i] = 3;
                            for (int j = r+2; (j < rows - 3) && (dungeonLayout[j][i] == 99); j++)
                                {
                                int VerticalWeightedLocationValue = diceRoll(0,20);
                                int MiddlePoint = 8; //Quick Tool to edit chances.
                                if (VerticalWeightedLocationValue <= MiddlePoint)
                                    {
                                    if (dungeonLayout[j][i] == 99) dungeonLayout[j][i] = 2;
                                    if (dungeonLayout[j+1][i] == 99) dungeonLayout[j+1][i] = 9;
                                    if (j % 2 == 0)
                                        {
                                        dungeonLayout[j][i] = 3;
                                        dungeonLayout[j+1][i] = 2;
                                        if (dungeonLayout[j+2][i] == 99) dungeonLayout[j+2][i] = 9;
                                        }
                                    }
                                else if (VerticalWeightedLocationValue > MiddlePoint)
                                    {
                                        if (dungeonLayout[j][i] == 99) dungeonLayout[j][i] = 3;
                                    }
                                }
                        }
                        else if (WeightedLocationValue > 10)
                            {
                            dungeonLayout[r][i] = 0;
                            if (dungeonLayout[r+1][i] == 99) dungeonLayout[r+1][i] = 9;
                            }
                        else
                            {
                            dungeonLayout[r][i] = 9;
                            break;
                        }
                    }
                }
            } // Stay with me.
            if (dungeonLayout[r][c] == 2)
                {

                bool Acessible = false;

                // Pre-scan: if a 1 already exists nearby on this row, this 2 is accessible
                for (int scan = c-1; scan > 0 && dungeonLayout[r][scan] != 9; scan--)
                    if (dungeonLayout[r][scan] == 1 || dungeonLayout[r][scan] == 3) Acessible = true;
                for (int scan = c+1; scan < cols-1 && dungeonLayout[r][scan] != 9; scan++)
                    if (dungeonLayout[r][scan] == 1 || dungeonLayout[r][scan] == 3) Acessible = true;

                if (!Acessible)
                {

                //Changing the logic to support the fact that every "2" must have an access point.
                int SideToGenerateRoll = diceRoll(0,2);
                //1 is left only, 2 is right only, 0 is both
                bool GenerateBothSides = false;

                if (SideToGenerateRoll == 2){GenerateBothSides = true;}

                if ((SideToGenerateRoll == 0) || GenerateBothSides)
                    {
                        int VerticalCounter = 0;
                        int VerticalCounterMax = cols/7;

                        for(int i = c-1; i > 1; i--)
                            {
                                int LocationValue = diceRoll(0, 2);
                                if (LocationValue == 2)
                                    {
                                    if (diceRoll(0,3) > 1) {LocationValue = 9;} //Just to make them rarer. Increase as needed.
                                    else {LocationValue = 1;}
                                    }

                                if (dungeonLayout[r][i] == 99)
                                    {
                                    if (LocationValue == 0){dungeonLayout[r][i] = 0;}

                                    else if (LocationValue == 1)
                                        {

                                            if ((VerticalCounter <= VerticalCounterMax) && (dungeonLayout[r][i+1] == 0 || dungeonLayout[r][i+1] == 9 || dungeonLayout[r][i+1] == 2) && (dungeonLayout[r+1][i] == 99) && (dungeonLayout[r][i+1] == 99 || dungeonLayout[r][i+1] == 0 || dungeonLayout[r][i+1] == 9 || dungeonLayout[r][i+1] == 2))
                                                {
                                                    dungeonLayout[r][i] = 1;
                                                    dungeonLayout[r+1][i] = 3;
                                                    Acessible = true;
                                                    VerticalCounter ++;
                                                    for (int j = r+2; (j < rows - 3) && (dungeonLayout[j][i] == 99); j++)
                                                        {
                                                        int VerticalWeightedLocationValue = diceRoll(0,20);
                                                        int MiddlePoint = 8; //Quick Tool to edit chances.
                                                        if (VerticalWeightedLocationValue <= MiddlePoint)
                                                            {
                                                            if (dungeonLayout[j][i] == 99) dungeonLayout[j][i] = 2;
                                                            if (dungeonLayout[j+1][i] == 99) dungeonLayout[j+1][i] = 9;
                                                            if (j % 2 == 0)
                                                                {
                                                                dungeonLayout[j][i] = 3;
                                                                dungeonLayout[j+1][i] = 2;
                                                                if (dungeonLayout[j+2][i] == 99) dungeonLayout[j+2][i] = 9;
                                                                }
                                                            }
                                                        else if (VerticalWeightedLocationValue > MiddlePoint)
                                                            {
                                                                if (dungeonLayout[j][i] == 99) dungeonLayout[j][i] = 3;
                                                            }
                                                        }
                                                }
                                            else {dungeonLayout[r][i] = 0; if (dungeonLayout[r+1][i] == 99) dungeonLayout[r+1][i] = 9;}
                                        }

                                    else {
                                        //Quick Fix, in cases where nine spawns first.
                                        dungeonLayout[r][i] = 9;
                                        break;

                                         }
                                    }
                                else
                                    {
                                    break;
                                    }

                            }
                    }
                //Right
                if ((SideToGenerateRoll == 1) || GenerateBothSides)
                    {
                        int VerticalCounter = 0;
                        int VerticalCounterMax = cols/7;

                        for(int i = c+1; i < cols -1; i++)
                            {
                                int LocationValue = diceRoll(0, 2);
                                if (LocationValue == 2)
                                    {
                                    if (diceRoll(0,3) > 1) {LocationValue = 9;} //Just to make them rarer. Increase as needed.
                                    else {LocationValue = 1;}
                                    }

                                if (dungeonLayout[r][i] == 99)
                                    {
                                    if (LocationValue == 0){dungeonLayout[r][i] = 0;}

                                    else if (LocationValue == 1)
                                        {

                                            if ((VerticalCounter <= VerticalCounterMax) && (dungeonLayout[r][i-1] == 0 || dungeonLayout[r][i-1] == 9 || dungeonLayout[r][i-1] == 2) && (dungeonLayout[r+1][i] == 99))
                                                {
                                                    dungeonLayout[r][i] = 1;
                                                    dungeonLayout[r+1][i] = 3;
                                                    Acessible = true;
                                                    VerticalCounter ++;
                                                    for (int j = r+2; (j < rows - 3) && (dungeonLayout[j][i] == 99); j++)
                                                        {
                                                        int VerticalWeightedLocationValue = diceRoll(0,20);
                                                        int MiddlePoint = 8; //Quick Tool to edit chances.
                                                        if (VerticalWeightedLocationValue <= MiddlePoint)
                                                            {
                                                            if (dungeonLayout[j][i] == 99) dungeonLayout[j][i] = 2;
                                                            if (dungeonLayout[j+1][i] == 99) dungeonLayout[j+1][i] = 9;
                                                            if (j % 2 == 0)
                                                                {
                                                                dungeonLayout[j][i] = 3;
                                                                dungeonLayout[j+1][i] = 2;
                                                                if (dungeonLayout[j+2][i] == 99) dungeonLayout[j+2][i] = 9;
                                                                }
                                                            }
                                                        else if (VerticalWeightedLocationValue > MiddlePoint)
                                                            {
                                                                if (dungeonLayout[j][i] == 99) dungeonLayout[j][i] = 3;
                                                            }
                                                        }
                                                }
                                            else {dungeonLayout[r][i] = 0; if (dungeonLayout[r+1][i] == 99) dungeonLayout[r+1][i] = 9;}
                                        }

                                    else {
                                        //Quick Fix, in cases where nine spawns first.
                                        dungeonLayout[r][i] = 9;
                                        break;

                                         }
                                    }
                                else
                                    {
                                    break;
                                    }

                            }
                    }


                // If neither side placed an access point, force one
                if (!Acessible)
                {
                    // Prefer the side that has room and won't create adjacent 1s
                    bool rightOK = (dungeonLayout[r][c+1] == 99 || dungeonLayout[r][c+1] == 0)
                                && dungeonLayout[r][c+2] != 1;
                    bool leftOK  = (dungeonLayout[r][c-1] == 99 || dungeonLayout[r][c-1] == 0)
                                && dungeonLayout[r][c-2] != 1;
                    if (rightOK)
                    {
                        dungeonLayout[r][c+1] = 1;
                        if (dungeonLayout[r+1][c+1] == 99) dungeonLayout[r+1][c+1] = 3;
                        for (int j = r+2; (j < rows - 3) && (dungeonLayout[j][c+1] == 99); j++)
                        {
                            int V = diceRoll(0,20);
                            if (V <= 8) {
                                if (dungeonLayout[j][c+1] == 99) {
                                    dungeonLayout[j][c+1] = 2;
                                    if (dungeonLayout[j+1][c+1] == 99) dungeonLayout[j+1][c+1] = 9;
                                    if (j % 2 == 0) {
                                        dungeonLayout[j][c+1] = 3;
                                        dungeonLayout[j+1][c+1] = 2;
                                        if (dungeonLayout[j+2][c+1] == 99) dungeonLayout[j+2][c+1] = 9;
                                    }
                                }
                            } else {
                                if (dungeonLayout[j][c+1] == 99) dungeonLayout[j][c+1] = 3;
                            }
                        }
                    }
                    else if (leftOK)
                    {
                        dungeonLayout[r][c-1] = 1;
                        if (dungeonLayout[r+1][c-1] == 99) dungeonLayout[r+1][c-1] = 3;
                        for (int j = r+2; (j < rows - 3) && (dungeonLayout[j][c-1] == 99); j++)
                        {
                            int V = diceRoll(0,20);
                            if (V <= 8) {
                                if (dungeonLayout[j][c-1] == 99) {
                                    dungeonLayout[j][c-1] = 2;
                                    if (dungeonLayout[j+1][c-1] == 99) dungeonLayout[j+1][c-1] = 9;
                                    if (j % 2 == 0) {
                                        dungeonLayout[j][c-1] = 3;
                                        dungeonLayout[j+1][c-1] = 2;
                                        if (dungeonLayout[j+2][c-1] == 99) dungeonLayout[j+2][c-1] = 9;
                                    }
                                }
                            } else {
                                if (dungeonLayout[j][c-1] == 99) dungeonLayout[j][c-1] = 3;
                            }
                        }
                    }
                    else
                    {
                        // Both sides blocked: convert 2 to 3+3+2 tower
                        dungeonLayout[r][c] = 3;
                        dungeonLayout[r+1][c] = 3;
                        dungeonLayout[r+2][c] = 2;
                        c = c-1;
                    }
                }

                } // end if (!Acessible) - skip generation if already accessible

            }
        }
    }
    /* First Attmpt, Generated Numbers with some cohesion, failed to prevent number from spawning on side one another.
    int PreviousLocationValue = 9;
    int VerticalScalingLimit = cols / 4;
    for (int i = 2; i < rows-2; i++)
    {   int VerticalLimiterCounter = 0;
        dungeonLayout[i][0] = 9;
        dungeonLayout[i][cols-1] = 9;
        for (int j = 1; j < cols-1; j++)
        {
            //Vertical Generater



            int LocationValue = diceRoll(0,4);
            if (LocationValue == 4){LocationValue = 9;}

            int AboveLocationValue = dungeonLayout[i-1][j];

            if (dungeonLayout[i][j] == 99)
            {

                if (AboveLocationValue == 1 || AboveLocationValue == 3 && (!(PreviousLocationValue == 2) || !(PreviousLocationValue == 3))) //Fix, include the other
                {
                    LocationValue = 2;
                    dungeonLayout[i+1][j] = 9;
                }
                else if (AboveLocationValue == 9 && (!(PreviousLocationValue == 3) || !(PreviousLocationValue == 1)))  //Fix, include the other
                {
                    LocationValue = 1;
                    //Wanna Turn this into a function. Gonna do it later, copy now.
                    for (int m = i; m < (dungeonLayout.size()-2); m++)
                    {
                    int WeightedLocationValue = diceRoll(0,15);
                    if (WeightedLocationValue >= 7){WeightedLocationValue = 3;}
                    else {WeightedLocationValue = 2;}
                    if (dungeonLayout[m][j] == 99){
                    dungeonLayout[m][j] = WeightedLocationValue;}

                    if (WeightedLocationValue == 2){m = (dungeonLayout.size());}
                    }
                }
                // V Useless???
                else if (AboveLocationValue == 3 || AboveLocationValue == 1 && ((!(PreviousLocationValue == 3) || (!(PreviousLocationValue == 1) || (!(PreviousLocationValue == 2))))))
                {
                    LocationValue = 3;

                    for (int m = i; m < (dungeonLayout.size()-2); m++)
                    {
                    int WeightedLocationValue = diceRoll(0,15);
                    if (WeightedLocationValue >= 7){WeightedLocationValue = 3;}
                    else {WeightedLocationValue = 2;}
                    if (dungeonLayout[m][j] == 99){
                    dungeonLayout[m][j] = WeightedLocationValue;}

                    if (WeightedLocationValue == 2){m = (dungeonLayout.size());}
                    }

                }
                else if (AboveLocationValue == 9)
                {
                    LocationValue = 0;
                }
                else {LocationValue = 9;}
            }

            else
            {
                LocationValue = dungeonLayout[i][j];
            }

            dungeonLayout[i][j] = LocationValue;
            PreviousLocationValue = dungeonLayout[i][j-1];
        }
    }
    */
}
void Dungeon::FillGaps()
{
    for (int i = 0; i < dungeonLayout.size(); i++)
    {
        for (int j = 0; j < dungeonLayout[i].size(); j++)
        {
            if (dungeonLayout[i][j] == 99)
            {
                dungeonLayout[i][j] = 9;
            }
        }
    }
}

int diceRoll(int min, int max){
    random_device rd;  // Obtain a random number from hardware
    mt19937 eng(rd()); // Seed the generator
    uniform_int_distribution<> distr(min, max); // Define the range
    return distr(eng);
}
