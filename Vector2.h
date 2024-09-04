#pragma once
#include <iostream>

template <typename T>
class vector2 {
public:
    T x, y;

    vector2(T newX, T newY) : x(newX), y(newY) {} // same as x=newx y=newy

    vector2() : x(0), y(0) {}

    template <typename U>
    vector2(const vector2<U>& other) {
        x = static_cast<T>(other.x);
        y = static_cast<T>(other.y);
    }

    vector2 operator+(vector2& additionVec) const {
        return vector2(x + additionVec.x, y + additionVec.y);
    }

    vector2 operator+=(vector2& additionVec) {
        x += additionVec.x;
        y += additionVec.y;
        return *this;
    }

    vector2 operator-(vector2& subtractionVec) const {
        return vector2(x - subtractionVec.x, y - subtractionVec.y);
    }

    vector2 operator-=(vector2& subtractionVec) {
        x -= subtractionVec.x;
        y -= subtractionVec.y;
        return *this;
    }

    vector2 operator*(vector2& multVec) {
        return vector2(x * multVec.x, y * multVec.y);
    }

    vector2 operator*=(vector2& multVec) {
        x *= multVec.x;
        y *= multVec.y;
        return *this;
    }

    vector2 operator/(vector2& divisionVec) {
        if (divisionVec.x == 0 || divisionVec.y == 0) return vector2(0, 0);
        return vector2(x / divisionVec.x, y / divisionVec.y);
    }

    vector2 operator/=(vector2& divisionVec) {
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
    vector2 operator=(vector2& setVec) {
        x = setVec.x;
        y = setVec.y;
        return *this;
    }

    bool operator==(vector2& compareVec) {
        return x == compareVec.x && y == compareVec.y;
    }

    bool operator!=(vector2& compareVec) {
        return x != compareVec.x && y != compareVec.y;
    }

    bool operator<(const vector2& compareVec) {
        return x < compareVec.x && y < compareVec.y;
    }
    bool operator<(vector2& compareVec) {
        return x < compareVec.x && y < compareVec.y;
    }

    bool operator<=(vector2& compareVec) {
        return x <= compareVec.x && y <= compareVec.y;
    }

    bool operator>(const vector2& compareVec) {
        return x > compareVec.x && y > compareVec.y;
    }
    bool operator>(vector2& compareVec) {
        return x > compareVec.x && y > compareVec.y;
    }

    bool operator>=(vector2& compareVec) {
        return x >= compareVec.x && y <= compareVec.y;
    }

    void print() const {
        std::cout << "(" << x << ", " << y << ")" << std::endl;
    }

};
template <typename T>
class doubleVec2 {
public:
    vector2<T> first, second;

    doubleVec2(vector2<T> firstNew, vector2<T> secondNew) : first(firstNew), second(secondNew) {} // same as x=newx y=newy

    doubleVec2() {}

    doubleVec2 operator+(doubleVec2& additionVec) {
        return doubleVec2(first + additionVec.first, second + additionVec.second);
    }

    doubleVec2 operator+=(doubleVec2& additionVec) {
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

template <typename T>
struct vector2Pair {
    vector2<T> Position;
    vector2<T> SIze;
};