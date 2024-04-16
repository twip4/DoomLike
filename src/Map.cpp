// Created by Paul Baudinot on 14/04/2024.

#include "../include/Map.h"

Map::Map(int x, int y) : width(x), height(y) {
    tiles = new int*[height];
    for (int i = 0; i < height; i++) {
        tiles[i] = new int[width];
        for (int j = 0; j < width; j++) {
            tiles[i][j] = 0;
        }
    }
}

Map::~Map() {
    for (int i = 0; i < height; i++) {
        delete[] tiles[i];
    }
    delete[] tiles;
}

void Map::setTile(int x, int y, int value) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        throw std::out_of_range("Map::setTile: Index out of range");
    }
    tiles[y][x] = value; // Correct index usage: y for row, x for column
}

int Map::getTile(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        throw std::out_of_range("Map::getTile: Index out of range");
    }
    return tiles[y][x]; // Correct index usage: y for row, x for column
}

int Map::getWidth() const {
    return width;
}

int Map::getHeight() const {
    return height;
}

void Map::init(const std::string& mapFile) {
    std::ifstream file(mapFile);

    if (!file) {
        std::cerr << "ERROR: Unable to open the file for reading." << std::endl;
        return;
    }

    std::string line;
    int writerY = 0;
    while (getline(file, line) && writerY < height) {
        std::istringstream stream(line);
        std::string token;
        int writerX = 0;

        while (getline(stream, token, ',') && writerX < width) {
            tiles[writerY][writerX] = std::stoi(token);
            writerX++;
        }
        writerY++;
    }

    file.close();
}

void Map::display() const {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            std::cout << tiles[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
