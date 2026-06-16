#include <vector>
#include <random>

int diceRoll(int min, int max);
//Explaination at the bottom.
class Dungeon {
private:
    int rows;
    int cols;
    std::vector<std::vector<int> > dungeonLayout;

public:
    Dungeon(int r, int c) : rows(r), cols(c), dungeonLayout(r, std::vector<int>(c, 99)) {
        // Enforce minimum size 5x5 and rows must be odd
        if (rows < 5) rows = 5;
        if (cols < 5) cols = 5;
        if (rows % 2 == 0) rows++;
        dungeonLayout.resize(rows,std::vector<int>(cols, 99));
    }

    void generateFirstRow();
    void generateExit();
    void generateDungeon();
    void generateFinalRow();
    void FillGaps();
    void ConvertDungeon();

    const std::vector<std::vector<int> >& getLayout() const { return dungeonLayout; }
};
/*
int main() { //      (Y,X) Yeah don't ask.
    Dungeon dungeon(13, 12);
    dungeon.generateFirstRow();
    dungeon.generateExit();
    dungeon.generateDungeon();
    dungeon.generateFinalRow();
    dungeon.FillGaps();
    dungeon.ConvertDungeon();

    const auto& layout = dungeon.getLayout();
    for (int i = 0; i < layout.size(); i++)
    {
        for (int j = 0; j < layout[i].size(); j++)
        {
           std::cout << layout[i][j] << " ";
        }
        std::cout << std::endl;
    }
    system("pause");
    return 0;
}*/

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
    for (int r = 2; r < rows - 2; r++)
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
                    bool PlacedVertical = false;
                    int LastZeroAt = c-1;
                    for (int i = c-2; (i > 0) && (dungeonLayout[r][i] == 99); i--)
                    {
                        int WeightedLocationValue = diceRoll(0,20);
                        if ((VerticalCounter <= VerticalCounterMax) && (WeightedLocationValue <= 10) && (dungeonLayout[r][i-1] == 0 || dungeonLayout[r][i-1] == 9) && (dungeonLayout[r+1][i] == 99))
                            {
                                VerticalCounter ++;
                                PlacedVertical = true;
                                dungeonLayout[r][i] = 1;
                                dungeonLayout[r+1][i] = 3;
                                for (int j = r+2; (j < rows - 2) && (dungeonLayout[j][i] == 99); j++)
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
                            LastZeroAt = i;
                            }
                        else
                            {
                            dungeonLayout[r][i] = 9;
                            break;
                        }
                    }
                    // If no vertical access was placed, force one at the last 0
                    if (!PlacedVertical && LastZeroAt < c-1)
                    {
                        dungeonLayout[r][LastZeroAt] = 1;
                        dungeonLayout[r+1][LastZeroAt] = 3;
                        for (int j = r+2; (j < rows - 2) && (dungeonLayout[j][LastZeroAt] == 99); j++)
                        {
                            int V = diceRoll(0,20);
                            if (V <= 8) {
                                if (dungeonLayout[j][LastZeroAt] == 99) dungeonLayout[j][LastZeroAt] = 2;
                                if (dungeonLayout[j+1][LastZeroAt] == 99) dungeonLayout[j+1][LastZeroAt] = 9;
                                if (j % 2 == 0) {
                                    dungeonLayout[j][LastZeroAt] = 3;
                                    dungeonLayout[j+1][LastZeroAt] = 2;
                                    if (dungeonLayout[j+2][LastZeroAt] == 99) dungeonLayout[j+2][LastZeroAt] = 9;
                                }
                            } else {
                                if (dungeonLayout[j][LastZeroAt] == 99) dungeonLayout[j][LastZeroAt] = 3;
                            }
                        }
                    }
                }
                //right
                if (c < cols-1 && (diceRoll(0,10) <= ChanceToSpawnSideHallway))
                {
                    dungeonLayout[r][c+1] = 0;
                    int VerticalCounter = 0;
                    int VerticalCounterMax = cols / 7;
                    bool PlacedVertical = false;
                    int LastZeroAt = c+1;
                    for (int i = c+2; (i < cols-1) && (dungeonLayout[r][i] == 99); i++)
                    {
                        int WeightedLocationValue = diceRoll(0,20);
                        if (((VerticalCounter <= VerticalCounterMax) && (WeightedLocationValue <= 10)) && (dungeonLayout[r][i-1] == 0 || dungeonLayout[r][i-1] == 9) && (dungeonLayout[r][i+1] == 99) && (dungeonLayout[r+1][i] == 99) && (dungeonLayout[r][i-1] == 99 || dungeonLayout[r][i-1] == 0 || dungeonLayout[r][i-1] == 9))
                        {
                            VerticalCounter++;
                            PlacedVertical = true;
                            dungeonLayout[r][i] = 1;
                            dungeonLayout[r+1][i] = 3;
                            for (int j = r+2; (j < rows - 2) && (dungeonLayout[j][i] == 99); j++)
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
                            LastZeroAt = i;
                            }
                        else
                            {
                            dungeonLayout[r][i] = 9;
                            break;
                        }
                    }
                    // If no vertical access was placed, force one at the last 0
                    if (!PlacedVertical && LastZeroAt > c+1)
                    {
                        dungeonLayout[r][LastZeroAt] = 1;
                        dungeonLayout[r+1][LastZeroAt] = 3;
                        for (int j = r+2; (j < rows - 2) && (dungeonLayout[j][LastZeroAt] == 99); j++)
                        {
                            int V = diceRoll(0,20);
                            if (V <= 8) {
                                if (dungeonLayout[j][LastZeroAt] == 99) dungeonLayout[j][LastZeroAt] = 2;
                                if (dungeonLayout[j+1][LastZeroAt] == 99) dungeonLayout[j+1][LastZeroAt] = 9;
                                if (j % 2 == 0) {
                                    dungeonLayout[j][LastZeroAt] = 3;
                                    dungeonLayout[j+1][LastZeroAt] = 2;
                                    if (dungeonLayout[j+2][LastZeroAt] == 99) dungeonLayout[j+2][LastZeroAt] = 9;
                                }
                            } else {
                                if (dungeonLayout[j][LastZeroAt] == 99) dungeonLayout[j][LastZeroAt] = 3;
                            }
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
                                                    for (int j = r+2; (j < rows - 2) && (dungeonLayout[j][i] == 99); j++)
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
                                                    for (int j = r+2; (j < rows - 2) && (dungeonLayout[j][i] == 99); j++)
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
                        for (int j = r+2; (j < rows - 2) && (dungeonLayout[j][c+1] == 99); j++)
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
                        for (int j = r+2; (j < rows - 2) && (dungeonLayout[j][c-1] == 99); j++)
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
}

void Dungeon::generateFinalRow()
{
    for (int i = 1; i < cols-1; i++)
    {//Prepare the final row to be generated.
        dungeonLayout[rows-2][i] = 99;
        dungeonLayout[rows-1][i] = 9;
    }

    for (int i = 1; i < cols-1; i++)
    {// Place the 2.
        if (dungeonLayout[rows-3][i] == 3)
        {
            dungeonLayout[rows-2][i] = 2;
        }
    }

    for (int i = 1; i < cols-1; i++)
    {//Find all 2s.
        if (dungeonLayout[rows-2][i] == 2)
        {
            int DistanceFromLeftWall = i-1;
            int DistanceFromRightWall = (cols-1)-i;

            //Generate Hallway to the left and right of the two.
            //Left
            if (DistanceFromLeftWall > 0)
            {
                int SpacesLeft = diceRoll(1, DistanceFromLeftWall);
                for (int k = 1; k <= SpacesLeft && i-k >= 0; k++)
                {
                    dungeonLayout[rows-2][i-k] = 0;
                    if(i-k-1 >= 0 && (dungeonLayout[rows-2][i-k-1] == 2 || dungeonLayout[rows-2][i-k-1] == 9))
                    {break;}
                }
            }
            //Right
            if (DistanceFromRightWall > 0)
            {
                int SpacesRight = diceRoll(1, DistanceFromRightWall);
                for (int k = 1; k <= SpacesRight && i+k < cols-1; k++)
                {
                    dungeonLayout[rows-2][i+k] = 0;
                    if(i+k+1 < cols-1 && (dungeonLayout[rows-2][i+k+1] == 2 || dungeonLayout[rows-2][i+k+1] == 9))
                    {break;}
                }
            }
        }
    }
    //Capture a backup in case of failure.
    int FirstZero = 1;
    for (int i = 1; i < cols-1; i++)
    {
        if (dungeonLayout[rows-2][i] == 0)
        {
            FirstZero = i;
            break;
        }
    }
    //Place the only entrance. Designed to favor the left side.
    bool Success = false;
    for (int i = 1; i < cols-1 && !Success; i++)
    {
        if (dungeonLayout[rows-2][i] == 0)
        {
            int EntranceRoll = diceRoll(0,2);
            if (EntranceRoll == 0)
            {
                Success = true;
                dungeonLayout[rows-2][i] = 1;
                dungeonLayout[rows-1][i] = 8;
            }
        }
    }
    if (!Success)
    {
        dungeonLayout[rows-2][FirstZero] = 1;
        dungeonLayout[rows-1][FirstZero] = 8;
    }
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

void Dungeon::ConvertDungeon()
{ //Convert dungeon to a more readable format.
    for (int i = 0; i < dungeonLayout.size(); i++)
    {
        for (int j = 0; j < dungeonLayout[i].size(); j++)
        {
            if (dungeonLayout[i][j] == 8){j++;}
            if (dungeonLayout[i][j] != 9)
            {
                dungeonLayout[i][j] = 0;
            }
        }
    }
}

int diceRoll(int min, int max){
    std::random_device rd;  // Obtain a random number from hardware
    std::mt19937 eng(rd()); // Seed the generator
    std::uniform_int_distribution<> distr(min, max); // Define the range
    return distr(eng);
}

/*This dungeon generator builds a 2D grid where rows alternate between hallway and wall layers.
 Each cell is a number:
 0 = floor (horizontal movement only),
 1 = downward shaft only,
 2 = upward shaft only,
 3 = bidirectional vertical shaft,
 8 = entrance,
 9 = solid wall.

Generation runs top-down.
The first row gets walls, floors, and vertical shafts via dice rolls, with air pocket checks at the edges.
The entrance is punched through the top wall.
Then `generateDungeon()` drops vertical corridors from each shaft in the first row,
and at every odd row any 3 spawns side hallways left and right — placing floors and further downward shafts with their own vertical extensions.
Every 2 is guaranteed a path down by building hallways and forcing a 1 if the dice fail to place one.
The final row is laid out separately, all remaining empty cells become walls, and everything non-wall collapses to a uniform floor for the final map. */
