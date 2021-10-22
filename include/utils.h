#ifndef _UTILS_H
#define _UTILS_H

#include <iostream>
#include <chrono>
#include <string>

enum Mode{ AGENT, PLAYER };

static const int COLS = 30; // 128;
static const int ROWS = 30; // 64;
static const int ZOOM = 20; // 15;
static const std::string NAME = "SNAKE";

struct Timer{
    std::chrono::_V2::system_clock::time_point start, end;
    std::chrono::duration<float> duration;

    Timer(){
        this->start = std::chrono::high_resolution_clock::now();
    }

    ~Timer(){
        this->end = std::chrono::high_resolution_clock::now();
        this->duration = this->end - this->start;

        float ms = this->duration.count() * 1000.0f;
        std::cout << "Time: " << ms << "ms" << std::endl;

    }
};

struct Vector2i{
    int x, y;

    Vector2i()
        : x(0), y(0) {}

    Vector2i(int x, int y)
        : x(x), y(y) {}

    bool operator <(const Vector2i& o) const{
        return (x < o.x) || ((!(o.x < x)) && (y < o.y));
    }
};

#endif