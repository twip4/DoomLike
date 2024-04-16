//
// Created by Paul Baudinot on 14/04/2024.
//

#ifndef DOOMLIKE_MAP_H
#define DOOMLIKE_MAP_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>

using namespace std;
class Map {
private:
    int width, height;
    int** tiles;

public:
    Map(int x, int y);
    ~Map();
    void init(const std::string& map);
    void setTile(int x, int y, int value);
    int getTile(int x, int y) const;
    int getWidth() const;
    int getHeight() const;
    void display() const;
};

#endif //DOOMLIKE_MAP_H
