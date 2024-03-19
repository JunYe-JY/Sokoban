// Copyright 2024 Jun Ye
#include "Sokoban.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>


namespace SB {
Sokoban::Sokoban() {
    direction = Direction::Down;
    _total = 0;
    _ground.loadFromFile("rcs/ground_01.png");
    _wall.loadFromFile("rcs/block_06.png");
    _box.loadFromFile("rcs/crate_03.png");
    _storage.loadFromFile("rcs/ground_04.png");

    // **********Teleport testing********** //
    _border.loadFromFile("rcs/border.png");
    _teleporter.loadFromFile("rcs/teleporter.png");
    // **********Teleport testing********** //

    _texturesVec.resize(4);
    // Down
    _texturesVec[0].loadFromFile("rcs/player_05.png");
    // Up
    _texturesVec[1].loadFromFile("rcs/player_08.png");
    // Left
    _texturesVec[2].loadFromFile("rcs/player_20.png");
    // Right
    _texturesVec[3].loadFromFile("rcs/player_17.png");
}

void Sokoban::setTeleporterLoc(unsigned int x, unsigned int y) {
    _teleporterLoc.push_back({x, y});
}

void Sokoban::setSize(size_t width, size_t height) {
    _width = width;
    _height = height;
    _map.resize(width, std::vector<char>(height));
    _mapReset.resize(width, std::vector<char>(height));
}

bool Sokoban::isWon() const {
    int total = 0;
    auto isBox = [](char cell) { return cell == '1'; };

    for (const auto& row : _map) {
        for (char cell : row) {
            if (isBox(cell)) {
                total++;
            }
        }
    }
    return total == _total;
}

void Sokoban::restart() {
    _map = _mapReset;
    _currentLoc = _locReset;
}

void Sokoban::setPlayerLoc(unsigned int x, unsigned int y) {
    _currentLoc = _locReset = {x, y};
}

void Sokoban::undoMove() {
    if (_pMoveStack.empty())
        return;
    Direction dir = _pMoveStack.top();
    bool boxMove = _bMoveStack.top();
    bool teleportMove = _tMoveStack.top();
    _pMoveStack.pop();
    _bMoveStack.pop();
    _tMoveStack.pop();

    sf::Vector2u prevLoc = _currentLoc;
    if (teleportMove)
        undoTeleport(prevLoc);

    switch (dir) {
        case Up: {
            prevLoc.y++;
            break;
        }
        case Down: {
            prevLoc.y--;
            break;
        }
        case Left: {
            prevLoc.x++;
            break;
        }
        case Right: {
            prevLoc.x--;
            break;
        }
    }

    undoPlayer(prevLoc);
    if (boxMove)
        undoBox(prevLoc, dir);

    if (dir == Up) {
        direction = Up;
    } else if (dir == Down) {
        direction = Down;
    } else if (dir == Left) {
        direction = Left;
    } else if (dir == Right) {
        direction = Right;
    }
}

void Sokoban::undoPlayer(sf::Vector2u prevLoc) {
    auto prev = _map[prevLoc.x][prevLoc.y];
    auto player = _map[_currentLoc.x ][_currentLoc.y];
    if (prev == '.' && player == '@') {
        _map[prevLoc.x][prevLoc.y] = '@';
        _map[_currentLoc.x ][_currentLoc.y] = '.';
        _currentLoc = prevLoc;

    } else if (prev == '.' && player == '0') {
        _map[prevLoc.x][ prevLoc.y] = '@';
        _map[_currentLoc.x][_currentLoc.y] = 'a';
        _currentLoc = prevLoc;

    } else if (prev == 'a' && player == '0') {
        _map[ prevLoc.x][prevLoc.y] = '0';
        _map[ _currentLoc.x][_currentLoc.y] = 'a';
        _currentLoc = prevLoc;

    } else if (prev == 'a' && player == '@') {
        _map[prevLoc.x][prevLoc.y] = '0';
        _map[_currentLoc.x][_currentLoc.y] = '.';
        _currentLoc = prevLoc;

    } else if (prev == '+' && player == '@') {
        _map[prevLoc.x][prevLoc.y] = '~';
        _map[_currentLoc.x][_currentLoc.y] = '.';
        _currentLoc = prevLoc;

    } else if (prev == '.' && player == '~') {
        _map[prevLoc.x][prevLoc.y] = '@';
        _map[_currentLoc.x][_currentLoc.y] = '+';
        _currentLoc = prevLoc;
    }
}

void Sokoban::undoBox(sf::Vector2u currLoc, Direction dir) {
    sf::Vector2u pPrevLoc = _currentLoc;
    switch (dir) {
        case Up: {
            pPrevLoc.y--;
            currLoc.y -= 2;
            break;
        }
        case Down: {
            pPrevLoc.y++;
            currLoc.y += 2;
            break;
        }
        case Left: {
            pPrevLoc.x--;
            currLoc.x -= 2;
            break;
        }
        case Right: {
            pPrevLoc.x++;
            currLoc.x += 2;
            break;
        }
    }
    auto box = _map[currLoc.x][currLoc.y];
    char next = _map[pPrevLoc.x][pPrevLoc.y];

    if (next == '.' && box == 'A') {
        _map[pPrevLoc.x][pPrevLoc.y] = 'A';
        _map[currLoc.x][currLoc.y] = '.';

    } else if (next == '.' && box == '1') {
        _map[pPrevLoc.x][pPrevLoc.y] = 'A';
        _map[currLoc.x][currLoc.y] = 'a';

    } else if (next == 'a' && box == 'A') {
        _map[pPrevLoc.x][pPrevLoc.y] = '1';
        _map[currLoc.x][currLoc.y] = '.';

    } else if (next == 'a' && box == '1') {
        _map[pPrevLoc.x][pPrevLoc.y] = '1';
        _map[currLoc.x][currLoc.y] = 'a';

    } else if (next == 'a' && box == '1') {
        _map[pPrevLoc.x][pPrevLoc.y] = '1';
        _map[currLoc.x][currLoc.y] = 'a';

    } else if (next == '.' && box == '1') {
        _map[pPrevLoc.x][pPrevLoc.y] = 'A';
        _map[currLoc.x][currLoc.y] = 'a';

    } else if (next == '.' && box == 'A') {
        _map[pPrevLoc.x][pPrevLoc.y] = 'A';
        _map[currLoc.x][currLoc.y] = '.';
    }
}

void Sokoban::movePlayer(Direction dir) {
    direction = dir;
    auto nextLoc = _currentLoc;
    switch (direction) {
        case Up:
            if (nextLoc.y >= 1 && _map[_currentLoc.x][_currentLoc.y - 1] != '#') {
                nextLoc.y--;
                updatePlayer(nextLoc);
            }
            break;
        case Down:
            if (nextLoc.y < _height - 1 && _map[_currentLoc.x][_currentLoc.y + 1] != '#') {
                nextLoc.y++;
                updatePlayer(nextLoc);
            }
            break;
        case Left:
            if (nextLoc.x >= 1 && _map[_currentLoc.x - 1][_currentLoc.y] != '#') {
                nextLoc.x--;
                updatePlayer(nextLoc);
            }
            break;
        case Right:
            if (nextLoc.x < _width - 1 && _map[_currentLoc.x + 1][_currentLoc.y] != '#') {
                nextLoc.x++;
                updatePlayer(nextLoc);
            }
            break;
        }
}

void Sokoban::undoTeleport(sf::Vector2u &prevLoc) {
    if (_map[_currentLoc.x][_currentLoc.y] == '~' &&
        prevLoc == _teleporterLoc[0]) {
        prevLoc = _teleporterLoc[1];
    } else if (_map[_currentLoc.x][_currentLoc.y] == '~' &&
        prevLoc == _teleporterLoc[1]) {
        prevLoc = _teleporterLoc[0];
    }
}

void Sokoban::updatePlayer(sf::Vector2u nextLoc) {
    char next = _map[nextLoc.x][nextLoc.y];
    char player = _map[_currentLoc.x][_currentLoc.y];
    if (next == '.' && player == '0') {
        _map[nextLoc.x][nextLoc.y] = '@';
        _map[_currentLoc.x][_currentLoc.y] = 'a';
        _currentLoc = nextLoc;
        _pMoveStack.push(direction);
        _bMoveStack.push(false);
        _tMoveStack.push(false);

    } else if (next == '.' && player == '@') {
        _map[nextLoc.x][nextLoc.y] = '@';
        _map[_currentLoc.x][_currentLoc.y] = '.';
        _currentLoc = nextLoc;
        _pMoveStack.push(direction);
        _bMoveStack.push(false);
        _tMoveStack.push(false);

    } else if (next== 'a' && player == '0') {
        _map[nextLoc.x][nextLoc.y] = '0';
        _map[_currentLoc.x][_currentLoc.y] = 'a';
        _currentLoc = nextLoc;
        _pMoveStack.push(direction);
        _bMoveStack.push(false);
        _tMoveStack.push(false);

    } else if (next == 'a' && player == '@') {
        _map[nextLoc.x][nextLoc.y] = '0';
        _map[_currentLoc.x][_currentLoc.y] = '.';
        _currentLoc = nextLoc;
        _pMoveStack.push(direction);
        _bMoveStack.push(false);
        _tMoveStack.push(false);

    } else if (next == '+' && player == '@') {
        if (nextLoc == _teleporterLoc[0]) {
            _map[_teleporterLoc[1].x][_teleporterLoc[1].y] = '~';
            _map[_currentLoc.x][_currentLoc.y] = '.';
            _currentLoc = _teleporterLoc[1];
        } else if (nextLoc == _teleporterLoc[1]) {
            _map[_teleporterLoc[0].x][_teleporterLoc[0].y] = '~';
            _map[_currentLoc.x][_currentLoc.y] = '.';
            _currentLoc = _teleporterLoc[0];
        }
        _pMoveStack.push(direction);
        _bMoveStack.push(false);
        _tMoveStack.push(true);

    } else if (next == '.' && player == '~') {
        _map[nextLoc.x][nextLoc.y] = '@';
        _map[_currentLoc.x][_currentLoc.y] = '+';
        _currentLoc = nextLoc;
        _pMoveStack.push(direction);
        _bMoveStack.push(false);
        _tMoveStack.push(false);

    } else if (next == 'A' || next == '1') {
        moveBox(nextLoc);
    }
}

void Sokoban::moveBox(sf::Vector2u boxLoc) {
    auto nextBoxLoc = boxLoc;
    switch (direction) {
        case Up:
            nextBoxLoc.y--;
            break;
        case Down:
            nextBoxLoc.y++;
            break;
        case Left:
            nextBoxLoc.x--;
            break;
        case Right:
            nextBoxLoc.x++;
            break;
    }
    if (!(nextBoxLoc.x >= 0 && nextBoxLoc.x < _width &&
        nextBoxLoc.y >= 0 && nextBoxLoc.y < _height)) {
        return;
    }

    char next = _map[nextBoxLoc.x][nextBoxLoc.y];
    char box = _map[boxLoc.x][boxLoc.y];
    char player = _map[_currentLoc.x][_currentLoc.y];

    if (isValidBoxMove(nextBoxLoc)) {
        if (next == '.' && box == 'A' && player == '@') {
            _map[nextBoxLoc.x][nextBoxLoc.y] = 'A';
            _map[boxLoc.x][boxLoc.y] = '@';
            _map[_currentLoc.x][_currentLoc.y] = '.';

        } else if (next == '.' && box == '1' && player == '@') {
            _map[nextBoxLoc.x][nextBoxLoc.y] = 'A';
            _map[boxLoc.x][boxLoc.y] = '0';
            _map[_currentLoc.x][_currentLoc.y] = '.';

        } else if (next == 'a' && box == 'A' && player == '@') {
            _map[nextBoxLoc.x][nextBoxLoc.y] = '1';
            _map[boxLoc.x][boxLoc.y] = '@';
            _map[_currentLoc.x][_currentLoc.y] = '.';

        } else if (next == 'a' && box == '1' && player == '@') {
            _map[nextBoxLoc.x][nextBoxLoc.y] = '1';
            _map[boxLoc.x][boxLoc.y] = '0';
            _map[_currentLoc.x][_currentLoc.y] = '.';

        } else if (next == 'a' && box == '1' && player == '0') {
            _map[nextBoxLoc.x][nextBoxLoc.y] = '1';
            _map[boxLoc.x][boxLoc.y] = '0';
            _map[_currentLoc.x][_currentLoc.y] = 'a';

        } else if (next == '.' && box == '1' && player == '0') {
            _map[nextBoxLoc.x][nextBoxLoc.y] = 'A';
            _map[boxLoc.x][boxLoc.y] = '0';
            _map[_currentLoc.x][_currentLoc.y] = 'a';

        } else if (next == '.' && box == 'A' && player == '0') {
            _map[nextBoxLoc.x][nextBoxLoc.y] = 'A';
            _map[boxLoc.x][boxLoc.y] = '@';
            _map[_currentLoc.x][_currentLoc.y] = 'a';

        } else if (next == '.' && box == 'A' && player == '~') {
            _map[nextBoxLoc.x][nextBoxLoc.y] = 'A';
            _map[boxLoc.x][boxLoc.y] = '@';
            _map[_currentLoc.x][_currentLoc.y] = '+';
        }

        _pMoveStack.push(direction);
        _bMoveStack.push(true);
        _tMoveStack.push(false);
        _currentLoc = boxLoc;
    }
}

bool Sokoban::isValidBoxMove(sf::Vector2u nextBoxLoc) const {
    return nextBoxLoc.x < _width && nextBoxLoc.y < _height &&
           _map[nextBoxLoc.x][nextBoxLoc.y] != '#' && _map[nextBoxLoc.x][nextBoxLoc.y] != '+' &&
           (_map[nextBoxLoc.x][nextBoxLoc.y] != 'A' && _map[nextBoxLoc.x][nextBoxLoc.y] != '1');
}

sf::Texture Sokoban::getPlayerTexture() const {
    switch (direction) {
        case Down:
            return _texturesVec[0];
            break;
        case Up:
            return _texturesVec[1];
            break;
        case Left:
            return _texturesVec[2];
            break;
        case Right:
            return _texturesVec[3];
            break;
    }
    return _texturesVec[0];
}

void Sokoban::setMap(size_t x, size_t y, char value) {
    _map[x][y] = value;
    _mapReset[x][y] = value;
}

char Sokoban::returnMap(size_t x, size_t y) const {
    return _map[x][y];
}

void Sokoban::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (size_t y = 0; y < _height; y++) {
        for (size_t x = 0; x < _width; x++) {
            sf::Sprite gSprite;
            gSprite.setTexture(_ground);
            gSprite.setPosition(x * SIZE, y * SIZE);
            target.draw(gSprite, states);
            sf::Sprite sprite;
            sf::Texture player = getPlayerTexture();
            switch (_map[x][y]) {
                case '#':
                    sprite.setTexture(_wall);
                    break;
                case 'A':
                    sprite.setTexture(_box);
                    break;
                case 'a':
                    sprite.setTexture(_storage);
                    break;
                case ' ':
                    sprite.setTexture(_wall);
                case '@':
                    sprite.setTexture(player);
                    break;
                case '1': {
                    sf::Sprite gSprite;
                    gSprite.setTexture(_storage);
                    gSprite.setPosition(x * SIZE, y * SIZE);
                    target.draw(gSprite, states);
                    sprite.setTexture(_box);
                    break;
                }
                case '0': {
                    sf::Sprite sSprite;
                    sSprite.setTexture(_storage);
                    sSprite.setPosition(x * SIZE, y * SIZE);
                    target.draw(sSprite, states);
                    sprite.setTexture(player);
                    break;
                }
                case '+': {
                    sf::Sprite tSprite;
                    tSprite.setTexture(_teleporter);
                    tSprite.setPosition(x * SIZE, y * SIZE);
                    target.draw(tSprite, states);
                    sprite.setTexture(_border);
                    break;
                }
                case '~': {
                    sf::Sprite tSprite;
                    tSprite.setTexture(_teleporter);
                    tSprite.setPosition(x * SIZE, y * SIZE);
                    target.draw(tSprite, states);
                    sf::Sprite bSprite;
                    bSprite.setTexture(_border);
                    bSprite.setPosition(x * SIZE, y * SIZE);
                    target.draw(bSprite, states);
                    sprite.setTexture(player);
                    break;
                }
            }
            sprite.setPosition(x * SIZE, y * SIZE);
            target.draw(sprite, states);
        }
    }
}

std::istream& operator>>(std::istream& is, Sokoban& sokoban) {
    unsigned int width, height;
    is >> height >> width;
    sokoban.setSize(width, height);

    auto isStorage = [](char c) { return c == 'a'; };
    auto isBox = [](char c) { return c == 'A'; };
    auto isBoth = [](char c) { return c == '1'; };
    auto isPlayer = [](char c) { return c == '@'; };
    auto isTeleporter = [](char c) { return c == '+'; };
    int box = 0, stor = 0;

    for (unsigned int y = 0; y < sokoban.height(); y++) {
        for (unsigned int x = 0; x < sokoban.width(); x++) {
            char c;
            is >> c;
            sokoban.setMap(x, y, c);
            if (isPlayer(c)) {
                sokoban.setPlayerLoc(x, y);
            } else if (isTeleporter(c)) {
                sokoban.setTeleporterLoc(x, y);
            } else if (isStorage(c)) {
                stor++;
            } else if (isBox(c)) {
                box++;
            } else if (isBoth(c)) {
                stor++;
                box++;
            }
        }
    }
    sokoban.total(std::min(stor, box));
    return is;
}

std::ostream& operator<<(std::ostream& os, const Sokoban& sokoban) {
    for (size_t y = 0; y < sokoban.height(); y++) {
        for (size_t x = 0; x < sokoban.width(); x++) {
            os << sokoban.returnMap(x, y);
        }
        os << std::endl;
    }
    return os;
}

}  // namespace SB
