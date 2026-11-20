#include "raylib-cpp.hpp"
#include "raylib.h"
#include <Vector2.hpp>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <deque>
#include <format>
#include <iostream>
#include <vector>
class SnakeGame {

    std::deque<raylib::Vector2> snake;
    raylib::Vector2 movement = {0, 1};
    std::vector<raylib::Vector2> fruits;

  public:
    int width, height;
    float length;
    bool lost = false;

    SnakeGame(int width, int height, float length)
        : width(width), height(height), length(length) {}
    void Update() {
        if (IsKeyPressed(KEY_W)) {
            movement = raylib::Vector2{0, -1};
        }
        if (raylib::Keyboard::IsKeyPressed(KEY_A)) {
            movement = raylib::Vector2{-1, 0};
        }
        if (raylib::Keyboard::IsKeyPressed(KEY_S)) {
            movement = raylib::Vector2{0, 1};
        }
        if (raylib::Keyboard::IsKeyPressed(KEY_D)) {
            movement = raylib::Vector2{1, 0};
        }
    }
    void CheckCollsions(raylib::Vector2 &tail,raylib::Vector2& head) {
        if (tail == snake.front()) {
            lost = true;
        }
        if(head.x<0 || head.x>=width || head.y<0 || head.y>=height){
            lost=true;
        }
        //! starting from one because 1 is head
        for (int i = 1; i < snake.size(); i++) {
            if (snake.front() == snake[i]) {

                lost = true;
            }
        }
        
    }
    void FixedUpdate() {
        auto tail = snake.back();

        snake.pop_back();

        CheckCollsions(tail,snake.front());
        snake.push_front(Vector2(movement + snake.front()));
        // case with 2 parts

        bool dirty = false;

        std::erase_if(fruits, [&](auto &fruitPos) {
            if (fruitPos == snake.front()) {
                snake.push_back(tail);
                dirty = true;

                return true;
            } else
                return false;
        });
        if (dirty)
            SpawnFruitRandom();
    }
    void Draw() {
        for (int i = 0; i < width; i++)
            for (int j = 0; j < height; j++) {
                DrawRectangle(i * length, j * length, length, length,
                              (i + j) % 2 ? LIME : raylib::Color::LightGray());
            }
        for (auto &seg : snake) {
            DrawRectangle(seg.x * length, seg.y * length, length, length,
                          DARKGREEN);
        }
        for (auto &fruit : fruits) {
            DrawCircle(fruit.x * length + length / 2,
                       fruit.y * length + length / 2, length / 2, RED);
        }
        if (lost) {
            DrawText("you lost", 300, 300, 120, BLUE);
        }
    }
    void Init() {
        srand(time(0));
        snake.push_front(raylib::Vector2(width / 2, height / 5));
        snake.push_back(raylib::Vector2(width / 2, height / 5 - 1));
        SpawnFruitRandom();
        SpawnFruitRandom();
        SpawnFruitRandom();
    }

    void SpawnFruitRandom() {

        int x = rand() % width;
        int y = rand() % height;

        // std::printf("{%d,%d}\n\n",x,y);
        fruits.push_back(raylib::Vector2{(float)x, (float)y});
        std::printf("update fruits {%d,%d} %zu\n", x, y, fruits.size());
    }
};