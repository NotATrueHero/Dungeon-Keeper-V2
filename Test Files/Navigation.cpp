#include "../Dungeon Creation/DungeonGeneration.cpp"
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
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
    nav.inputUI();
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
//This function was Ai Generated. As per the last time, I've attmpted to avoid this. Since this was more "cosmetic", I decided it was okay.
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
    using namespace ftxui;

    auto North = [this] { moveNorth(); };
    auto East  = [this] { moveEast();  };
    auto South = [this] { moveSouth(); };
    auto West  = [this] { moveWest();  };

    std::string label_up    = " ^ ";
    std::string label_down  = " v ";
    std::string label_left  = " < ";
    std::string label_right = " > ";

    auto btn_up    = Button(&label_up,    North);
    auto btn_down  = Button(&label_down,  South);
    auto btn_left  = Button(&label_left,  West);
    auto btn_right = Button(&label_right, East);

    auto map_view = Renderer([this] {
        const auto& layout = getDungeonLayout();
        int lightSize = 4;
        int mapH = (int)layout.size();
        int mapW = (int)layout[0].size();

        Elements rows;
        for (int dy = -lightSize; dy <= lightSize; dy++) {
            Elements cols;
            for (int dx = -lightSize; dx <= lightSize; dx++) {
                int xDis = x + dx;
                int yDis = y + dy;

                // Out of bounds = empty/blocked
                if (xDis < 0 || xDis >= mapW || yDis < 0 || yDis >= mapH) {
                    cols.push_back(text("  "));
                    continue;
                }

                bool blocked = false;
                if (xDis != x || yDis != y) {
                    int adx = abs(dx), ady = abs(dy);
                    int steps = adx > ady ? adx : ady;
                    for (int s = 1; s < steps; s++) {
                        int cx = x + (dx * s + (dx>0?steps/2:-steps/2)) / steps;
                        int cy = y + (dy * s + (dy>0?steps/2:-steps/2)) / steps;
                        if (cx < 0 || cx >= mapW || cy < 0 || cy >= mapH || layout[cy][cx] == 9)
                            { blocked = true; break; }
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
        return vbox(std::move(rows)) | border | size(WIDTH, EQUAL, 20) | size(HEIGHT, EQUAL, 10);
    });

    auto container = Container::Horizontal({
        map_view,
        Container::Vertical({btn_up, btn_down, btn_left, btn_right}),
    });

    auto screen = App::Fullscreen();
    screen.Loop(container);
}
