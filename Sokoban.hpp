// Copyright 2024 Jun Ye
#pragma once
#include <iostream>
#include <stack>
#include <SFML/Graphics.hpp>

namespace SB {

enum Direction {
    Up,
    Down,
    Left,
    Right
};

class Sokoban : public sf::Drawable {
 public:
    static const size_t SIZE = 64;
    Sokoban();
    size_t width() const { return _width; }
    size_t height() const { return _height; }
    size_t pixelWidth() const { return _width * SIZE; }
    size_t pixelHeight() const { return _height * SIZE; }
    void setSize(size_t width, size_t height);
    void total(int num) { _total = num; }

    bool isWon() const;
    void restart();

    void undoMove();
    void undoPlayer(sf::Vector2u prevLoc);
    void undoBox(sf::Vector2u prevLoc, Direction dir);
    void undoTeleport(sf::Vector2u &prevLoc);

    void setMap(size_t x, size_t y, char value);
    char returnMap(size_t x, size_t y) const;

    sf::Vector2u playerLoc() const { return _currentLoc; }
    void setPlayerLoc(unsigned int x, unsigned int y);
    void setTeleporterLoc(unsigned int x, unsigned int y);

    void movePlayer(Direction direction);
    void updatePlayer(sf::Vector2u nextLoc);
    void moveBox(sf::Vector2u boxLoc);
    bool isValidBoxMove(sf::Vector2u nextBoxLoc) const;

    sf::Texture getPlayerTexture() const;

    friend std::ostream& operator<<(std::ostream &os, const Sokoban &sokoban);

 protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 private:
    size_t _width;
    size_t _height;
    sf::Texture _ground;      // . -> ground
    sf::Texture _wall;        // # -> wall
    sf::Texture _box;         // A -> box
    sf::Texture _storage;     // a -> target/storage
                        // 1 -> box + storage
                        // 0 -> player + storage
    sf::Texture _border;      // + -> teleporter
    sf::Texture _teleporter;  // + -> teleporter
                        // ~ -> player + teleporter

    std::vector<sf::Texture> _texturesVec;
    int _total;

    std::vector<std::vector<char>> _map;
    std::vector<std::vector<char>> _mapReset;

    std::stack<Direction> _pMoveStack;
    std::stack<bool> _bMoveStack;
    std::stack<bool> _tMoveStack;

    std::vector<sf::Vector2u> _teleporterLoc;
    sf::Vector2u _currentLoc;
    sf::Vector2u _locReset;

    Direction direction;
};

std::istream& operator>>(std::istream& is, Sokoban& sokoban);
std::ostream& operator<<(std::ostream& os, const Sokoban& sokoban);

}  // namespace SB
