#pragma once
#include <iostream>

class vector2 {
public:
    double x, y;

    vector2(double newX, double newY) : x(newX), y(newY) {} // same as x=newx y=newy

    vector2() : x(0), y(0) {}

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
        return doubleVec2(first + additionVec.first, second + additionVec.second);
    }

    doubleVec2 operator+=(const doubleVec2& additionVec) {
        first += additionVec.first;
        second += additionVec.second;
        return *this;
    }
};

class tripleDouble {
public:
    double x, y, z;

    tripleDouble(double X, double Y, double Z) : x(X), y(Y), z(Z) {}

    tripleDouble() : x(0), y(0), z(0) {}

    tripleDouble operator+(const tripleDouble& toAdd) const  {
        return tripleDouble(toAdd.x + x, toAdd.y + y, toAdd.z + z);
    }
    tripleDouble& operator+=(const tripleDouble& toAdd) {
        x += toAdd.x;
        y += toAdd.y;
        z += toAdd.z;
        return *this;
    }

    tripleDouble operator-(const tripleDouble& toSub) const  {
        return tripleDouble(x - toSub.x, y - toSub.y, z - toSub.z);
    }
    tripleDouble& operator-=(const tripleDouble& toSub) {
        x -= toSub.x;
        y -= toSub.y;
        z -= toSub.z;
        return *this;
    }

    tripleDouble operator*(const tripleDouble& toMult) const  {
        return tripleDouble(x * toMult.x, y * toMult.y, z * toMult.z);
    }
    tripleDouble& operator*=(const tripleDouble& toMult) {
        x *= toMult.x;
        y *= toMult.y;
        z *= toMult.z;
        return *this;
    }

    tripleDouble operator/(const tripleDouble& toDiv) const  {
        return tripleDouble(x / toDiv.x, y / toDiv.y, z / toDiv.z);
    }
    tripleDouble& operator/=(const tripleDouble& toDiv) {
        x /= toDiv.x;
        y /= toDiv.y;
        z /= toDiv.z;
        return *this;
    }

    tripleDouble& operator=(const tripleDouble& toEqual) {
        if (this != &toEqual) {  // Check for self-assignment
            x = toEqual.x;
            y = toEqual.y;
            z = toEqual.z;
        }
        return *this;
    }

    bool operator==(const tripleDouble& toCompare) const  {
        return x == toCompare.x && y == toCompare.y && z == toCompare.z;
    }
    bool operator>=(const tripleDouble& toCompare) const  {
        return x >= toCompare.x && y >= toCompare.y && z >= toCompare.z;
    }
    bool operator<=(const tripleDouble& toCompare) const  {
        return x <= toCompare.x && y <= toCompare.y && z <= toCompare.z;
    }

    bool operator<(const tripleDouble& toCompare) const  {
        return x < toCompare.x && y < toCompare.y && z < toCompare.z;
    }
    bool operator>(const tripleDouble& toCompare) const  {
        return x > toCompare.x && y > toCompare.y && z > toCompare.z;
    }
};

struct vector2Pair {
    vector2 Position;
    vector2 SIze;
};