#include <iostream>
#include <vector>
#include <raylib.h>
#include <cmath>

#include "Math.hpp"

struct Bird {
    Math::Rect<float> rect;
    Math::Vec2<float> velocity;
};

struct PipePair {
    Math::Rect<float> safeArea;
    bool isScored;
};

Math::Vec2<int> windowSize;

int main() {
    InitWindow(0, 0, "Flappy Dick!");
    ToggleFullscreen();
    windowSize = {GetScreenWidth(), GetScreenHeight()};
    srand(time(0));

    while (true) {
    std::vector<PipePair> pipes;
    int score = 0;
        Bird bird = { {
                float(windowSize.x / 6),
                float(windowSize.y / 2 - windowSize.x / 50),
                float(windowSize.x / 25),
                float(windowSize.x / 25)
            }, {300, 0}
        };

        float timeSincePipe = 2.f;
        while (true) {
            if (WindowShouldClose()) goto exit;
            float deltaTime = GetFrameTime();
            bird.velocity.y -= 1200*deltaTime;
            if (IsKeyPressed(KEY_SPACE)) bird.velocity.y = 650;
            bird.rect.y -= bird.velocity.y*deltaTime;

            if (bird.rect.y <= 0) { bird.velocity.y = 0; bird.rect.y = 0; }
            if (bird.rect.y + bird.rect.h >= windowSize.y) goto died;

            timeSincePipe += deltaTime;

            if (pipes.size() && pipes.at(0).safeArea.x+pipes.at(0).safeArea.w < 0) 
                pipes.erase(pipes.begin());

            if (timeSincePipe >= 2.f) {
                PipePair newPipe = { {
                        (float)windowSize.x,
                        (float)(rand()%(windowSize.y-450))+100.f,
                        150,
                        300
                    }, false
                };
                pipes.push_back(newPipe);
                timeSincePipe -= 2.f;
            }

            BeginDrawing(); {
                ClearBackground(BLACK);
                for (auto& pipe : pipes) {
                    if (bird.rect.IsCollidingWith({ pipe.safeArea.x, 0, pipe.safeArea.w, pipe.safeArea.y }) ||
                        bird.rect.IsCollidingWith({ pipe.safeArea.x, pipe.safeArea.y+pipe.safeArea.h, pipe.safeArea.w, windowSize.y-pipe.safeArea.y+pipe.safeArea.h }))
                        goto died;
                    
                    if (Math::Rect<float>(bird.rect.x-bird.rect.w, bird.rect.y, bird.rect.w, bird.rect.h).IsCollidingWith(pipe.safeArea) && !pipe.isScored) {
                        score++;
                        pipe.isScored = true;
                    }

                    pipe.safeArea.x -= bird.velocity.x*deltaTime;
                    DrawRectangle(pipe.safeArea.x, 0, pipe.safeArea.w, pipe.safeArea.y, GREEN);
                    DrawRectangle(pipe.safeArea.x, pipe.safeArea.y+pipe.safeArea.h, pipe.safeArea.w, windowSize.y-pipe.safeArea.y+pipe.safeArea.h, GREEN);
                }
                DrawRectangle(bird.rect.x, bird.rect.y, bird.rect.w, bird.rect.h, YELLOW);
                DrawText(std::to_string(score).c_str(), 10, 10, 20, WHITE);
                DrawFPS(10, 40);
            } EndDrawing();
        }
died:
        std::cout << "youre bad!!\n";
    }

exit:
    CloseWindow();
    return 0;
}