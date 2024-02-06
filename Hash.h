#pragma once
#include <SFML/graphics.hpp>

struct Line 
{
    sf::Vertex vertices[2];
    bool operator==(const Line& other) const
    {
        return ((vertices[0].position == other.vertices[0].position && vertices[1].position == other.vertices[1].position) ||
            (vertices[0].position == other.vertices[1].position && vertices[1].position == other.vertices[0].position));
    }
};

struct LineHash 
{
    std::size_t operator()(const Line& line) const
    {
        std::size_t hash = 0;

        float x1 = line.vertices[0].position.x;
        float y1 = line.vertices[0].position.y;
        float x2 = line.vertices[1].position.x;
        float y2 = line.vertices[1].position.y;

        if (x1 > x2 || (x1 == x2 && y1 > y2)) 
        {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }

        hash_combine(hash, x1);
        hash_combine(hash, y1);
        hash_combine(hash, x2);
        hash_combine(hash, y2);

        return hash;
    }
private:
    static void hash_combine(std::size_t& seed, float value) 
    {
        std::hash<float> hasher;
        seed ^= hasher(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
};