

#include "raylib-cpp.hpp"
#include <Mouse.hpp>
#include <Window.hpp>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <raylib.h>
#include <unistd.h>

#define WIDTH 3
#define HEGIHT 3
#define INDEX(x, y) (y * WIDTH + x)
#define LENGTH 200.f

struct BoardData {
    uint8_t color;
    uint8_t filled;
};
BoardData grid[9];

uint8_t currentRound = 1;
bool won = false;
inline void IsWinning() {
    for (int i = 0; i < 3; i++) {
        if (grid[INDEX(0, i)].filled == grid[INDEX(1, i)].filled &&
            grid[INDEX(1, i)].filled == grid[INDEX(2, i)].filled &&
            grid[INDEX(2, i)].filled != 0) {
            won = true;
            break;
        }
        if (grid[INDEX(i, 0)].filled == grid[INDEX(i, 1)].filled &&
            grid[INDEX(i, 1)].filled == grid[INDEX(i, 2)].filled &&
            grid[INDEX(i, 2)].filled != 0) {
            won = true;
            break;
        }
    }
    if (grid[INDEX(0, 0)].filled == grid[INDEX(1, 1)].filled &&
        grid[INDEX(1, 1)].filled == grid[INDEX(2, 2)].filled &&
        grid[INDEX(2, 2)].filled != 0) {
        won = true;
        return;
    }
    if (grid[INDEX(0, 2)].filled == grid[INDEX(1, 1)].filled &&
        grid[INDEX(1, 1)].filled == grid[INDEX(2, 0)].filled &&
        grid[INDEX(2, 0)].filled != 0) {
        won = true;
        return;
    }
}

inline void Update() {
    if (raylib::Mouse::IsButtonPressed(MOUSE_BUTTON_LEFT)) {

        raylib::Vector2 mousePos = GetMousePosition();

        int x = std::floor(mousePos.x / LENGTH);
        int y = std::floor(mousePos.y / LENGTH);
        if (!grid[INDEX(x, y)].filled) {
            grid[INDEX(x, y)].filled = currentRound ? 1 : 2;
            currentRound = !currentRound;
        }
        IsWinning();
    }
}

inline void Draw() {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) {
            DrawRectangle(i * LENGTH, j * LENGTH, LENGTH, LENGTH,
                          grid[INDEX(i, j)].color ? BLACK : WHITE);
            if (grid[INDEX(i, j)].filled == 1) {
                DrawLineEx(Vector2{i * LENGTH, j * LENGTH},
                           Vector2{(i + 1) * LENGTH, (j + 1) * LENGTH}, 10,
                           RED);
                DrawLineEx(Vector2{i * LENGTH, (j + 1) * LENGTH},
                           Vector2{(i + 1) * LENGTH, j * LENGTH}, 10, RED);

            } else if (grid[INDEX(i, j)].filled == 2) {
                DrawCircleV(
                    Vector2(i * LENGTH + LENGTH / 2, j * LENGTH + LENGTH / 2),
                    LENGTH / 2, BLUE);
            }
            if (won) {
                DrawText("YOU WON!", 200, 300, 120, GREEN);
              
            }
        }
}

int main() {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            grid[INDEX(i, j)].color = (i + j) % 2;

    raylib::Window window{800, 600, "X and 0"};
    while (!window.ShouldClose() && !won) {
        window.BeginDrawing();

        Update();
        Draw();

        window.EndDrawing();
        if(won)
        {
              sleep(1);
        }
    }
}