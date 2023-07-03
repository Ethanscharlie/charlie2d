#pragma once

#include <SDL.h>
#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <random>

struct Vector2f
{
	Vector2f()
	:x(0.0f), y(0.0f)
	{}

	Vector2f(float p_x, float p_y)
	:x(p_x), y(p_y)
	{}

	void print()
	{
		std::cout << x << ", " << y << std::endl;
	}

    float dist(Vector2f point) 
    {
        return sqrt(pow(point.x - x, 2) + pow(point.y - y, 2));
    }

	float x, y;
};

inline float randFloat(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

template<typename T, std::size_t N>
T getRandomElement(const T (&arr)[N]) {
    // Generate a random index within the array bounds
    std::size_t randomIndex = std::rand() % N;

    // Return the randomly selected element
    return arr[randomIndex];
}
