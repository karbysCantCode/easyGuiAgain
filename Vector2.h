#pragma once
#include <iostream>

class vector2 {
public:
    double x, y;

    vector2(double newX, double newY) : x(newX), y(newY) {} // same as x=newx y=newy

    vector2 operator+(const vector2& additionVec) {
        return vector2(x + additionVec.x, y + additionVec.y);
    }

    vector2 operator+=(const vector2& additionVec) {
        x += additionVec.x;
        y += additionVec.y;
        return *this;
    }

    vector2 operator-(const vector2& subtractionVec) {
        return vector2(x - subtractionVec.x, y - subtractionVec.y);
    }

    vector2 operator-=(const vector2& subtractionVec) {
        x -= subtractionVec.x;
        y -= subtractionVec.y;
        return *this;
    }

    vector2 operator*(const vector2& multVec) {
        return vector2(x * multVec.x, y * multVec.y);
    }

    vector2 operator*=(const vector2& multVec) {
        x *= multVec.x;
        y *= multVec.y;
        return *this;
    }

    vector2 operator/(const vector2& divisionVec) {
        if (divisionVec.x == 0 || divisionVec.y == 0) return vector2(0, 0);
        return vector2(x / divisionVec.x, y / divisionVec.y);
    }

    vector2 operator/=(const vector2& divisionVec) {
        if (divisionVec.x == 0 || divisionVec.y == 0) return vector2(0, 0);
        x /= divisionVec.x;
        y /= divisionVec.y;
        return *this;
    }

    vector2 operator=(const vector2& setVec) {
        x = setVec.x;
        y = setVec.y;
        return *this;
    }

    bool operator==(const vector2& compareVec) {
        return x == compareVec.x && y == compareVec.y;
    }

    bool operator<(const vector2& compareVec) {
        return x < compareVec.x && y < compareVec.y;
    }

    bool operator<=(const vector2& compareVec) {
        return x <= compareVec.x && y <= compareVec.y;
    }

    bool operator>(const vector2& compareVec) {
        return x > compareVec.x && y > compareVec.y;
    }

    bool operator>=(const vector2& compareVec) {
        return x >= compareVec.x && y <= compareVec.y;
    }

    void print() const {
        std::cout << "(" << x << ", " << y << ")" << std::endl;
    }

};

class doubleVec2 {
public:
    vector2 first, second;

    doubleVec2(vector2 firstNew, vector2 secondNew) : first(firstNew), second(secondNew) {} // same as x=newx y=newy

    doubleVec2 operator+(const doubleVec2& additionVec) {
        return doubleVec2(first + additionVec.first, second + additionVec.second)
    }

    doubleVec2 operator+=(const doubleVec2& additionVec) {
        first += additionVec.first;
        second += additionVec.second;
        return this*
    }
};