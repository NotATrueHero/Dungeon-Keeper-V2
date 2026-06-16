#include "../Dungeon Creation/DungeonGeneration.cpp"
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/color.hpp>
#include <string>

extern void generateDungeonBase();
extern const std::vector<std::vector<int>>& getDungeonLayout();

class Navigate {
    private:
        int x = 0;
        int y = 0;
        void findRoutes();
        bool inputCheck(int xIn, int yIn);
    public:
        Navigate() { findRoutes(); }
        void display();

        void moveNorth();
        void moveEast();
        void moveSouth();
        void moveWest();

        void inputUI();
};

int main() {
    Navigate nav;
    nav.display();
    return 0;
}

void Navigate::findRoutes() {
    generateDungeonBase();
    const auto& layout = getDungeonLayout();
    for (int i = 0; i < (int)layout.size(); i++)
        for (int j = 0; j < (int)layout[i].size(); j++)
            if (layout[i][j] == 8) { y = i-1; x = j; }
}

bool Navigate::inputCheck(int xIn, int yIn) {
    const auto& layout = getDungeonLayout();
    if (layout[y + yIn][x + xIn] != 9) return true;
    return false;
}
//This function was Ai Generated
void Navigate::display() {
    using namespace ftxui;
    const auto& layout = getDungeonLayout();
    int lightSize = 4;

    int yStart = y - lightSize; if (yStart < 0) yStart = 0;
    int yEnd   = y + lightSize; if (yEnd >= (int)layout.size()) yEnd = layout.size()-1;
    int xStart = x - lightSize; if (xStart < 0) xStart = 0;
    int xEnd   = x + lightSize; if (xEnd >= (int)layout[0].size()) xEnd = layout[0].size()-1;

    Elements rows;
    for (int yDis = yStart; yDis <= yEnd; yDis++) {
        Elements cols;
        for (int xDis = xStart; xDis <= xEnd; xDis++) {
            bool blocked = false;
            if (xDis != x || yDis != y) {
                int dx = xDis - x, dy = yDis - y;
                int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
                for (int s = 1; s < steps; s++) {
                    int cx = x + (dx * s + (dx>0?steps/2:-steps/2)) / steps;
                    int cy = y + (dy * s + (dy>0?steps/2:-steps/2)) / steps;
                    if (layout[cy][cx] == 9) { blocked = true; break; }
                }
            }

            std::string cell;
            Color fg = Color::Default;
            if (blocked) {
                cell = "  ";
            } else if (xDis == x && yDis == y) {
                cell = "@@"; fg = Color::Yellow;
            } else if (layout[yDis][xDis] == 9) {
                cell = "##"; fg = Color::GrayDark;
            } else if (layout[yDis][xDis] == 8) {
                cell = "++"; fg = Color::Green;
            } else {
                cell = "..";
            }
            cols.push_back(text(cell) | color(fg));
        }
        rows.push_back(hbox(std::move(cols)));
    }

    auto document = vbox(std::move(rows)) | border;
    auto screen = Screen::Create(Dimension::Fit(document));
    Render(screen, document);
    screen.Print();
}

void Navigate::moveNorth()   { if (inputCheck(0, -1)) y--; }
void Navigate::moveEast()    { if (inputCheck(1, 0))  x++; }
void Navigate::moveSouth()   { if (inputCheck(0, 1))  y++; }
void Navigate::moveWest()    { if (inputCheck(-1, 0)) x--; }

void Navigate::inputUI() {
}
