#pragma once

namespace globals {
    constexpr int SCREEN_WIDTH = 640;
    constexpr int SCREEN_HEIGHT = 480;

    constexpr float SPRITE_SCALE = 2.0f;
}

struct Vector2 {
    int x, y;

    Vector2() : x(0), y(0) {}

    Vector2(int x, int y) : x(x), y(y) {}

    static Vector2 zero() {
        return Vector2(0, 0);
    }
};