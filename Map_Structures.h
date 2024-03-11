#pragma once
#include <unordered_set>
#include "Options.h"
#include <SFML/graphics.hpp>


enum Object
{
    NONE = 0, COIN = 1, POWERUP = 2
};
struct Peer
{
    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;
    Object object = NONE;
    //int left = -1;
    //int right = -1;
    //int up = -1;
    //int down = -1;
};
struct Line
{
    Line() {}
    Line(sf::Vector2i A, sf::Vector2i B)
    {
        if (A.x > B.x)
        {
            this->B = A;
            this->A = B;
        }
        else if (A.y > B.y)
        {
            this->B = A;
            this->A = B;
        }
        else
        {
            this->A = A;
            this->B = B;
        }
    }
    sf::Vector2i A, B;
    bool operator==(const Line& other) const
    {
        return ((A == other.A && B == other.B) ||
            (A == other.B && B == other.A));
    }
    bool has_point(sf::Vector2i point) const
    {
        return (point == A || point == B);
    }
};
struct LineHash
{
    std::size_t operator()(const Line& line) const
    {
        std::size_t hash = 0;

        int x1 = line.A.x;
        int y1 = line.A.y;
        int x2 = line.B.x;
        int y2 = line.B.y;

        if (x1 < x2)
            std::swap(x1, x2);
        if (y1 < y2)
            std::swap(y1, y2);

        hash_combine(hash, x1);
        hash_combine(hash, y1);
        hash_combine(hash, x2);
        hash_combine(hash, y2);

        return hash;
    }
private:
    static void hash_combine(std::size_t& seed, int value)
    {
        std::hash<int> hasher;
        seed ^= hasher(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
};