#include "../Dungeon Creation/DungeonGeneration.cpp"
#include <iostream>

extern void generateDungeonBase();
extern const std::vector<std::vector<int>>& getDungeonLayout();

class Navigate {
    private:
        int x = 0 ;
        int y = 0 ;
        void findRoutes();
    public:
        Navigate() { findRoutes(); }
        void display();
};

int main() {
    Navigate nav;
    nav.display();
    return 0;
}

void Navigate::findRoutes() {
    generateDungeonBase();
    const auto& layout = getDungeonLayout();
    //Find Entrance;
    for (int i = 0; i < layout.size(); i++)
    {
        for (int j = 0; j < layout[i].size(); j++)
        {
            if (layout[i][j] == 8)
            {
                y = i-1;
                x = j;
            }
        }
    }
}

void Navigate::display(){
    const auto& layout = getDungeonLayout();
    int lightSize = 3;

    // Clamp bounds to grid edges, lightSize cells in each direction from player
    int yStart = y - lightSize; if (yStart < 0) yStart = 0;
    int yEnd   = y + lightSize; if (yEnd >= (int)layout.size()) yEnd = layout.size()-1;
    int xStart = x - lightSize; if (xStart < 0) xStart = 0;
    int xEnd   = x + lightSize; if (xEnd >= (int)layout[0].size()) xEnd = layout[0].size()-1;

    for (int yDis = yStart; yDis <= yEnd; yDis++)
    {
        for (int xDis = xStart; xDis <= xEnd; xDis++)
        {
            if (xDis == x && yDis == y) {std::cout<<"@";}
            else if(layout[yDis][xDis] == 9){std::cout<<"X";}
            else if(layout[yDis][xDis] == 0){std::cout<<"0";}
            else if(layout[yDis][xDis] == 8){std::cout<<"+";}
        }
        std::cout<<std::endl;
    }
}
