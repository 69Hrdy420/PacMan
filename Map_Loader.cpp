#include "Map_Loader.h"
#include <SFML/Graphics.hpp>

const sf::Vector2f start_pos(100.f, 100.f);

//#include <iostream>
//Map_Loader::Map_Loader(const Map_Loader& other)
//    :window(*(new Window("loader")))
//{
//    std::cout << "copy loader" << std::endl;
//}
//Map_Loader& Map_Loader::operator=(const Map_Loader& other)
//{
//    std::cout << "operator loader" << std::endl;
//    Window w("w");
//    std::unordered_set<Line, LineHash> s;
//    std::vector<std::vector<Object>> o;
//    std::vector<int> i;
//    return *(new Map_Loader(w, s, o, i, i));
//}

Map_Loader::Map_Loader(Window& window, const std::unordered_set<Line, LineHash>& paths, const std::vector<std::vector<Object>>& objects, const std::vector<int>& horizontal_bridges, const std::vector<int>& vertical_bridges)
    :window(window)
{
    if (!window.window.isOpen())
        window.create("Map Loader");
    else
        window.window.setTitle("Map Loader");
    if (paths.empty())
        return;
    peers = load_peers(paths, sf::Vector2i(objects.size(), objects[0].size()));
    for (int i = 0; i < objects.size(); i++)
        for (int j = 0; j < objects[0].size(); j++)
            peers[i][j].object = objects[i][j];
    walls = load_optimized_walls(load_bridges(load_walls(paths), horizontal_bridges, vertical_bridges));
}

std::unordered_set<Line, LineHash> Map_Loader::load_bridges(std::unordered_set<Line, LineHash>&& walls, const std::vector<int>& horizontal_bridges, const std::vector<int>& vertical_bridges)
{
    for (int index : horizontal_bridges)
    {
        if (index > peers[0].size())
            continue;
        // LEFT SIDE
        walls.erase(Line(sf::Vector2i(-1, index - 1), sf::Vector2i(-1, index)));
        walls.erase(Line(sf::Vector2i(-1, index), sf::Vector2i(-1, index + 1)));
        walls.insert(Line(sf::Vector2i(-1, index + 1), sf::Vector2i(-2, index + 1)));
        walls.insert(Line(sf::Vector2i(-1, index - 1), sf::Vector2i(-2, index - 1)));
        // RIGHT SIDE
        walls.erase(Line(sf::Vector2i(peers.size(), index - 1), sf::Vector2i(peers.size(), index)));
        walls.erase(Line(sf::Vector2i(peers.size(), index), sf::Vector2i(peers.size(), index + 1)));
        walls.insert(Line(sf::Vector2i(peers.size(), index - 1), sf::Vector2i(peers.size() + 1, index - 1)));
        walls.insert(Line(sf::Vector2i(peers.size(), index + 1), sf::Vector2i(peers.size() + 1, index + 1)));
    }
    for (int index : vertical_bridges)
    {
        if (index > peers.size())
            continue;
        // TOP SIDE
        walls.erase(Line(sf::Vector2i(index - 1, -1), sf::Vector2i(index, -1)));
        walls.erase(Line(sf::Vector2i(index, -1), sf::Vector2i(index + 1, -1)));
        walls.insert(Line(sf::Vector2i(index - 1, -1), sf::Vector2i(index - 1, -2)));
        walls.insert(Line(sf::Vector2i(index + 1, -1), sf::Vector2i(index + 1, -2)));
        // BOTTOM SIDE
        walls.erase(Line(sf::Vector2i(index - 1, peers[0].size()), sf::Vector2i(index, peers[0].size())));
        walls.erase(Line(sf::Vector2i(index, peers[0].size()), sf::Vector2i(index + 1, peers[0].size())));
        walls.insert(Line(sf::Vector2i(index - 1, peers[0].size()), sf::Vector2i(index - 1, peers[0].size() + 1)));
        walls.insert(Line(sf::Vector2i(index + 1, peers[0].size()), sf::Vector2i(index + 1, peers[0].size() + 1)));
    }
    return walls;
}

void Map_Loader::draw()
{
    draw_walls();
    draw_objects();
}

void Map_Loader::draw_objects()
{
    sf::CircleShape peer(Options::object_size / 60, Options::num_circle_points);
    peer.setFillColor(sf::Color::White);
    peer.setOrigin((peer.getRadius()), (peer.getRadius()));

    sf::CircleShape coin(Options::object_size / 15, Options::num_circle_points);
    coin.setFillColor(sf::Color::Yellow);
    coin.setOrigin((coin.getRadius()), (coin.getRadius()));

    for (int x = 0; x < peers.size(); x++)
    {
        for (int y = 0; y < peers[0].size(); y++)
            if (peers[x][y].object == COIN)
            {
                coin.setPosition(static_cast<sf::Vector2f>(start_pos + Options::object_size / 2 * sf::Vector2f(x, y)));
                window.window.draw(coin);
            }
            else if (peers[x][y].object == STARTPOS)
            {
                coin.setFillColor(sf::Color::Green);
                coin.setRadius(Options::object_size / 7.5);
                coin.setOrigin((coin.getRadius()), (coin.getRadius()));
                coin.setPosition(static_cast<sf::Vector2f>(start_pos + Options::object_size / 2 * sf::Vector2f(x, y)));
                window.window.draw(coin);
                coin.setFillColor(sf::Color::Yellow);
                coin.setRadius(Options::object_size / 15);
                coin.setOrigin((coin.getRadius()), (coin.getRadius()));
            }
            else if (peers[x][y].object == GHOST1)
            {
                coin.setFillColor(sf::Color::Red);
                coin.setRadius(Options::object_size / 7.5);
                coin.setOrigin((coin.getRadius()), (coin.getRadius()));
                coin.setPosition(static_cast<sf::Vector2f>(start_pos + Options::object_size / 2 * sf::Vector2f(x, y)));
                window.window.draw(coin);
                coin.setFillColor(sf::Color::Yellow);
                coin.setRadius(Options::object_size / 15);
                coin.setOrigin((coin.getRadius()), (coin.getRadius()));
            }
            else if (peers[x][y].object == POWERUP)
            {
                coin.setRadius(Options::object_size / 7.5);
                coin.setOrigin((coin.getRadius()), (coin.getRadius()));
                coin.setPosition(static_cast<sf::Vector2f>(start_pos + Options::object_size / 2 * sf::Vector2f(x, y)));
                window.window.draw(coin);
                coin.setRadius(Options::object_size / 15);
                coin.setOrigin((coin.getRadius()), (coin.getRadius()));
            }
    }
}

bool Map_Loader::run(const sf::Event& event)
{
    if (!window.window.isOpen())
        return false;
    if (!window.window.hasFocus())
        return true;
    if (event.type == event.KeyPressed)
        return false;
    draw();
    return true;
}

std::unordered_set<Line, LineHash> Map_Loader::load_walls(const std::unordered_set<Line, LineHash>& paths)
{
    std::unordered_set<Line, LineHash> walls_copy;
    walls_copy.reserve(paths.size() * 1.67);
    for (const Line& line : paths)
    {
        if (is_vertical(line))
            load_vertical_wall(line, walls_copy, paths);
        else if (is_horizontal(line))
            load_horizontal_wall(line, walls_copy, paths);
    }
    return walls_copy;
}

void Map_Loader::load_vertical_wall(const Line& line, std::unordered_set<Line, LineHash>& set, const std::unordered_set<Line, LineHash>& paths)
{
    // TOP EDGE
    if (paths.find(Line(sf::Vector2i(line.A.x, line.A.y), sf::Vector2i(line.A.x, line.A.y - 1))) == paths.end()) // path doesn't continue up, so we will focus on top edge
    {
        if ((paths.find(Line(sf::Vector2i(line.A.x, line.A.y), sf::Vector2i(line.A.x - 1, line.A.y))) == paths.end()) &&
            (paths.find(Line(sf::Vector2i(line.A.x, line.A.y), sf::Vector2i(line.A.x + 1, line.A.y))) == paths.end())) // path doesn't go left nor right from top edge
        {
            set.insert(Line(sf::Vector2i(line.A.x - 1, line.A.y), sf::Vector2i(line.A.x - 1, line.A.y - 1))); // insert section from left of the top edge to the top
            set.insert(Line(sf::Vector2i(line.A.x - 1, line.A.y - 1), sf::Vector2i(line.A.x, line.A.y - 1))); // insert horizontal section around the top edge
            set.insert(Line(sf::Vector2i(line.A.x, line.A.y - 1), sf::Vector2i(line.A.x + 1, line.A.y - 1))); // insert horizontal section around the top edge
            set.insert(Line(sf::Vector2i(line.A.x + 1, line.A.y), sf::Vector2i(line.A.x + 1, line.A.y - 1))); // insert section from right of the top edge to the top
        }
        else
        {
            if (paths.find(Line(sf::Vector2i(line.A.x, line.A.y), sf::Vector2i(line.A.x - 1, line.A.y))) == paths.end()) // path doesn't continue left
            {
                set.insert(Line(sf::Vector2i(line.A.x - 1, line.A.y - 1), sf::Vector2i(line.A.x - 1, line.A.y)));           // insert section from left of the top edge to the top
                if (paths.find(Line(sf::Vector2i(line.B.x, line.B.y), sf::Vector2i(line.B.x - 1, line.B.y))) == paths.end())// path doesn't continue left even from bottom edge
                    set.insert(Line(sf::Vector2i(line.B.x - 1, line.B.y), sf::Vector2i(line.A.x - 1, line.A.y)));           // insert section left of path
            }
            if (paths.find(Line(sf::Vector2i(line.A.x, line.A.y), sf::Vector2i(line.A.x + 1, line.A.y))) == paths.end())    // path doesn't continue right
            {
                set.insert(Line(sf::Vector2i(line.A.x + 1, line.A.y - 1), sf::Vector2i(line.A.x + 1, line.A.y)));           // insert section from right of the top edge to the top
                if (paths.find(Line(sf::Vector2i(line.B.x, line.B.y), sf::Vector2i(line.B.x + 1, line.B.y))) == paths.end())// path doesn't continue right even from bottom edge
                    set.insert(Line(sf::Vector2i(line.B.x + 1, line.B.y), sf::Vector2i(line.A.x + 1, line.A.y)));           // insert section right of path
            }
        }
    }

    // BOTTOM EDGE
    if (paths.find(Line(sf::Vector2i(line.B.x, line.B.y), sf::Vector2i(line.B.x, line.B.y + 1))) == paths.end()) // path doesn't continue down, so we will focus on bottom edge
    {
        if ((paths.find(Line(sf::Vector2i(line.B.x, line.B.y), sf::Vector2i(line.B.x - 1, line.B.y))) == paths.end()) &&
            (paths.find(Line(sf::Vector2i(line.B.x, line.B.y), sf::Vector2i(line.B.x + 1, line.B.y))) == paths.end())) // path doesn't go left nor right from bottom edge
        {
            set.insert(Line(sf::Vector2i(line.B.x - 1, line.B.y), sf::Vector2i(line.B.x - 1, line.B.y + 1))); // insert section from left of the bottom edge to the bottom
            set.insert(Line(sf::Vector2i(line.B.x - 1, line.B.y + 1), sf::Vector2i(line.B.x, line.B.y + 1))); // insert horizontal section around the bottom edge
            set.insert(Line(sf::Vector2i(line.B.x, line.B.y + 1), sf::Vector2i(line.B.x + 1, line.B.y + 1))); // insert horizontal section around the bottom edge
            set.insert(Line(sf::Vector2i(line.B.x + 1, line.B.y), sf::Vector2i(line.B.x + 1, line.B.y + 1))); // insert section from right of the bottom edge to the bottom
        }
        else
        {
            if (paths.find(Line(sf::Vector2i(line.B.x, line.B.y), sf::Vector2i(line.B.x - 1, line.B.y))) == paths.end()) // path doesn't continue left
            {
                set.insert(Line(sf::Vector2i(line.B.x - 1, line.B.y + 1), sf::Vector2i(line.B.x - 1, line.B.y)));           // insert section from left of the bottom edge to the bottom
                if (paths.find(Line(sf::Vector2i(line.A.x, line.A.y), sf::Vector2i(line.A.x - 1, line.A.y))) == paths.end())// path doesn't continue left even from top edge
                    set.insert(Line(sf::Vector2i(line.A.x - 1, line.A.y), sf::Vector2i(line.B.x - 1, line.B.y)));           // insert section left of path
            }
            if (paths.find(Line(sf::Vector2i(line.B.x, line.B.y), sf::Vector2i(line.B.x + 1, line.B.y))) == paths.end())    // path doesn't continue right
            {
                set.insert(Line(sf::Vector2i(line.B.x + 1, line.B.y + 1), sf::Vector2i(line.B.x + 1, line.B.y)));           // insert section from right of the bottom edge to the bottom
                if (paths.find(Line(sf::Vector2i(line.A.x, line.A.y), sf::Vector2i(line.A.x + 1, line.A.y))) == paths.end())// path doesn't continue right even from top edge
                    set.insert(Line(sf::Vector2i(line.A.x + 1, line.A.y), sf::Vector2i(line.B.x + 1, line.B.y)));           // insert section right of path
            }
        }
    }

    // BOTH EDGES
    if ((paths.find(Line(sf::Vector2i(line.A.x, line.A.y), sf::Vector2i(line.A.x - 1, line.A.y))) == paths.end()) &&
        (paths.find(Line(sf::Vector2i(line.B.x, line.B.y), sf::Vector2i(line.B.x - 1, line.B.y))) == paths.end())) // there are no paths going left from edges
        set.insert(Line(sf::Vector2i(line.A.x - 1, line.A.y), sf::Vector2i(line.B.x - 1, line.B.y)));            // insert path to the left
    if ((paths.find(Line(sf::Vector2i(line.A.x, line.A.y), sf::Vector2i(line.A.x + 1, line.A.y))) == paths.end()) &&
        (paths.find(Line(sf::Vector2i(line.B.x, line.B.y), sf::Vector2i(line.B.x + 1, line.B.y))) == paths.end())) // there are no paths going right from edges
        set.insert(Line(sf::Vector2i(line.A.x + 1, line.A.y), sf::Vector2i(line.B.x + 1, line.B.y)));            // insert path to the right
}

void Map_Loader::load_horizontal_wall(const Line& line, std::unordered_set<Line, LineHash>& set, const std::unordered_set<Line, LineHash>& paths)
{
    // RIGHT EDGE
    if (paths.find(Line(sf::Vector2i(line.B.x, line.B.y), sf::Vector2i(line.B.x + 1, line.B.y))) == paths.end()) // path doesent continue right, so we will focus on right edge
    {
        if ((paths.find(Line(sf::Vector2i(line.B.x, line.B.y), sf::Vector2i(line.B.x, line.B.y - 1))) == paths.end()) &&
            (paths.find(Line(sf::Vector2i(line.B.x, line.B.y), sf::Vector2i(line.B.x, line.B.y + 1))) == paths.end())) // path doesent go up nor down from right edge
        {
            set.insert(Line(sf::Vector2i(line.B.x, line.B.y + 1), sf::Vector2i(line.B.x + 1, line.B.y + 1))); // insert section from above the rigth edge to the right
            set.insert(Line(sf::Vector2i(line.B.x + 1, line.B.y + 1), sf::Vector2i(line.B.x + 1, line.B.y))); // insert vertical section around the right edge
            set.insert(Line(sf::Vector2i(line.B.x + 1, line.B.y), sf::Vector2i(line.B.x + 1, line.B.y - 1))); // insert vertical section around the right edge
            set.insert(Line(sf::Vector2i(line.B.x, line.B.y - 1), sf::Vector2i(line.B.x + 1, line.B.y - 1))); // insert section from below the rigth edge to the right
        }
        else
        {
            if (paths.find(Line(sf::Vector2i(line.B.x, line.B.y), sf::Vector2i(line.B.x, line.B.y - 1))) == paths.end()) // path doesent continue up
            {
                set.insert(Line(sf::Vector2i(line.B.x + 1, line.B.y - 1), sf::Vector2i(line.B.x, line.B.y - 1)));           // insert section from above the rigth edge to the right
                if (paths.find(Line(sf::Vector2i(line.A.x, line.A.y), sf::Vector2i(line.A.x, line.A.y - 1))) == paths.end())// path doesent continue up even from left edge
                    set.insert(Line(sf::Vector2i(line.A.x, line.A.y - 1), sf::Vector2i(line.B.x, line.B.y - 1)));           // insert section above path
            }
            if (paths.find(Line(sf::Vector2i(line.B.x, line.B.y), sf::Vector2i(line.B.x, line.B.y + 1))) == paths.end())    // path doesent continue down
            {
                set.insert(Line(sf::Vector2i(line.B.x + 1, line.B.y + 1), sf::Vector2i(line.B.x, line.B.y + 1)));           // insert section from below the right edge to the right
                if (paths.find(Line(sf::Vector2i(line.A.x, line.A.y), sf::Vector2i(line.A.x, line.A.y + 1))) == paths.end())// path doesent continue down even from left edge
                    set.insert(Line(sf::Vector2i(line.A.x, line.A.y + 1), sf::Vector2i(line.B.x, line.B.y + 1)));           // insert section below path
            }
        }
    }

    // LEFT EDGE
    if (paths.find(Line(sf::Vector2i(line.A.x, line.A.y), sf::Vector2i(line.A.x - 1, line.A.y))) == paths.end()) // path doesn't continue left, so we will focus on left edge
    {
        if ((paths.find(Line(sf::Vector2i(line.A.x, line.A.y), sf::Vector2i(line.A.x, line.A.y - 1))) == paths.end()) &&
            (paths.find(Line(sf::Vector2i(line.A.x, line.A.y), sf::Vector2i(line.A.x, line.A.y + 1))) == paths.end())) // path doesn't go up nor down from left edge
        {
            set.insert(Line(sf::Vector2i(line.A.x, line.A.y + 1), sf::Vector2i(line.A.x - 1, line.A.y + 1))); // insert section from above the left edge to the left
            set.insert(Line(sf::Vector2i(line.A.x - 1, line.A.y + 1), sf::Vector2i(line.A.x - 1, line.A.y))); // insert vertical section around the left edge
            set.insert(Line(sf::Vector2i(line.A.x - 1, line.A.y), sf::Vector2i(line.A.x - 1, line.A.y - 1))); // insert vertical section around the left edge
            set.insert(Line(sf::Vector2i(line.A.x, line.A.y - 1), sf::Vector2i(line.A.x - 1, line.A.y - 1))); // insert section from below the left edge to the left
        }
        else
        {
            if (paths.find(Line(sf::Vector2i(line.A.x, line.A.y), sf::Vector2i(line.A.x, line.A.y - 1))) == paths.end()) // path doesn't continue up
            {
                set.insert(Line(sf::Vector2i(line.A.x - 1, line.A.y - 1), sf::Vector2i(line.A.x, line.A.y - 1)));           // insert section from above the left edge to the left
                if (paths.find(Line(sf::Vector2i(line.B.x, line.B.y), sf::Vector2i(line.B.x, line.B.y - 1))) == paths.end())// path doesn't continue up even from right edge
                    set.insert(Line(sf::Vector2i(line.B.x, line.B.y - 1), sf::Vector2i(line.A.x, line.A.y - 1)));           // insert section above path
            }
            if (paths.find(Line(sf::Vector2i(line.A.x, line.A.y), sf::Vector2i(line.A.x, line.A.y + 1))) == paths.end())    // path doesn't continue down
            {
                set.insert(Line(sf::Vector2i(line.A.x - 1, line.A.y + 1), sf::Vector2i(line.A.x, line.A.y + 1)));           // insert section from below the left edge to the left
                if (paths.find(Line(sf::Vector2i(line.B.x, line.B.y), sf::Vector2i(line.B.x, line.B.y + 1))) == paths.end())// path doesn't continue down even from right edge
                    set.insert(Line(sf::Vector2i(line.B.x, line.B.y + 1), sf::Vector2i(line.A.x, line.A.y + 1)));           // insert section below path
            }
        }
    }

    // BOTH EDGES
    if ((paths.find(Line(sf::Vector2i(line.A.x, line.A.y), sf::Vector2i(line.A.x, line.A.y - 1))) == paths.end()) &&
        (paths.find(Line(sf::Vector2i(line.B.x, line.B.y), sf::Vector2i(line.B.x, line.B.y - 1))) == paths.end())) // there are no paths going up from edges
        set.insert(Line(sf::Vector2i(line.A.x, line.A.y - 1), sf::Vector2i(line.B.x, line.B.y - 1)));            // insert path above
    if ((paths.find(Line(sf::Vector2i(line.A.x, line.A.y), sf::Vector2i(line.A.x, line.A.y + 1))) == paths.end()) &&
        (paths.find(Line(sf::Vector2i(line.B.x, line.B.y), sf::Vector2i(line.B.x, line.B.y + 1))) == paths.end())) // there are no paths going down from edges
        set.insert(Line(sf::Vector2i(line.A.x, line.A.y + 1), sf::Vector2i(line.B.x, line.B.y + 1)));            // insert path below
}

void Map_Loader::draw_walls()
{
    for (const Line& line : walls)
    {
        sf::Vector2f scaledA = static_cast<sf::Vector2f>(line.A) * Options::object_size / 2.f + start_pos;
        sf::Vector2f scaledB = static_cast<sf::Vector2f>(line.B) * Options::object_size / 2.f + start_pos;
        sf::Vertex lineVertices[] =
        {
            sf::Vertex(scaledA, sf::Color::Blue),
            sf::Vertex(scaledB, sf::Color::Blue)
        };
        window.window.draw(lineVertices, 2, sf::Lines);
    }
}

bool Map_Loader::is_vertical(const Line& line)
{
    return line.A.x == line.B.x && line.A.y != line.B.y;
}

bool Map_Loader::is_horizontal(const Line& line)
{
    return line.A.x != line.B.x && line.A.y == line.B.y;
}

std::vector<std::vector<Peer>> Map_Loader::load_peers(const std::unordered_set<Line, LineHash>& paths, sf::Vector2i size)
{
    std::vector<std::vector<Peer>> result(size.x, std::vector<Peer>(size.y));
    for (const Line path : paths)
    {
        if (paths.find(Line(sf::Vector2i(path.A.x - 1, path.A.y), path.A)) != paths.end())
            result[path.A.x][path.A.y].left = true;
        if (paths.find(Line(sf::Vector2i(path.A.x + 1, path.A.y), path.A)) != paths.end())
            result[path.A.x][path.A.y].right = true;        
        if (paths.find(Line(sf::Vector2i(path.A.x, path.A.y - 1), path.A)) != paths.end())
            result[path.A.x][path.A.y].up = true;        
        if (paths.find(Line(sf::Vector2i(path.A.x, path.A.y + 1), path.A)) != paths.end())
            result[path.A.x][path.A.y].down = true;

        if (paths.find(Line(sf::Vector2i(path.B.x - 1, path.B.y), path.B)) != paths.end())
            result[path.B.x][path.B.y].left = true;
        if (paths.find(Line(sf::Vector2i(path.B.x + 1, path.B.y), path.B)) != paths.end())
            result[path.B.x][path.B.y].right = true;
        if (paths.find(Line(sf::Vector2i(path.B.x, path.B.y - 1), path.B)) != paths.end())
            result[path.B.x][path.B.y].up = true;
        if (paths.find(Line(sf::Vector2i(path.B.x, path.B.y + 1), path.B)) != paths.end())
            result[path.B.x][path.B.y].down = true;
    }
    return result;
}

std::vector<Line> Map_Loader::load_optimized_walls(std::unordered_set<Line, LineHash>&& walls)
{
    std::vector<Line> new_walls;
    new_walls.reserve(walls.size() / 2);
    while (!walls.empty())
    {
        Line line(walls.extract(walls.begin()).value());
        if (is_horizontal(line))
        {
            int start = line.A.x;
            int end = line.B.x;
            while (walls.erase(Line(sf::Vector2i(start, line.A.y), sf::Vector2i(start - 1, line.B.y))))
            {
                start--;
            }
            while (walls.erase(Line(sf::Vector2i(end, line.A.y), sf::Vector2i(end + 1, line.B.y))))
            {
                end++;
            }
            new_walls.emplace_back(Line(sf::Vector2i(start, line.A.y), sf::Vector2i(end, line.B.y)));
        }
        else
        {
            int start = line.A.y;
            int end = line.B.y;
            while (walls.erase(Line(sf::Vector2i(line.A.x, start), sf::Vector2i(line.B.x, start - 1))))
            {
                start--;
            }
            while (walls.erase(Line(sf::Vector2i(line.A.x, end), sf::Vector2i(line.B.y, end + 1))))
            {
                end++;
            }
            new_walls.emplace_back(Line(sf::Vector2i(line.A.x, start), sf::Vector2i(line.B.x, end)));
        }
    }
    new_walls.shrink_to_fit();
    return new_walls;
}

std::vector<Line> Map_Loader::get_walls()
{
    return walls;
}
std::vector<std::vector<Peer>> Map_Loader::get_peers()
{
    return peers;
}