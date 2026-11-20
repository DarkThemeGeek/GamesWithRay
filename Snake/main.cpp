#include "Color.hpp"
#include "game.hpp"
#include "raylib-cpp.hpp"
#include "raylib.h"
#include <Keyboard.hpp>
#include <Vector2.hpp>
#include <deque>
#include <iostream>
#include <queue>
#define WIDTH 12
#define HEGIHT 12
#define LENGTH 40.f

// floats store well ints until 2^24

int main() {
    float accumulator = 0;

    float speed = 0.5f;

    raylib::Window window{800, 600, "Snake"};
    SetTargetFPS(60);

    SnakeGame game(WIDTH, HEGIHT, LENGTH);

    game.Init();
    while (!WindowShouldClose()) {
        window.BeginDrawing();

        // Fixed updates independent of fps for speed
        accumulator += GetFrameTime();
        game.Update();
        if (accumulator >= speed) {
            accumulator -= speed;
            game.FixedUpdate();
        }

        game.Draw();

        window.EndDrawing();
    }
}