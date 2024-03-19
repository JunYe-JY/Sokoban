// Copyright 2024 Jun Ye
#include <iostream>
#include <string>
#include <fstream>
#include <SFML/Graphics.hpp>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Main
#include <boost/test/unit_test.hpp>

#include "Sokoban.hpp"

using SB::Sokoban;

BOOST_AUTO_TEST_CASE(testMovePlayer) {
    Sokoban sokoban;
    std::ifstream file("level1.lvl");
    file >> sokoban;
    file.close();
    sokoban.movePlayer(SB::Direction::Right);
    sokoban.movePlayer(SB::Direction::Right);
    sokoban.movePlayer(SB::Direction::Right);
    sokoban.movePlayer(SB::Direction::Right);
    sokoban.movePlayer(SB::Direction::Right);
    sf::Vector2u v = sokoban.playerLoc();
    BOOST_REQUIRE_EQUAL(v.x, 7);
    BOOST_REQUIRE_EQUAL(v.y, 6);
}

BOOST_AUTO_TEST_CASE(testSize) {
    Sokoban sokoban;
    std::ifstream file("level1.lvl");
    file >> sokoban;
    file.close();
    BOOST_REQUIRE_EQUAL(sokoban.pixelWidth(), 640);
    BOOST_REQUIRE_EQUAL(sokoban.pixelHeight(), 640);
}

BOOST_AUTO_TEST_CASE(testMovePlayerVSBoxes) {
    Sokoban sokoban;
    std::ifstream file("level2.lvl");
    file >> sokoban;
    file.close();
    sokoban.movePlayer(SB::Direction::Up);
    sokoban.movePlayer(SB::Direction::Up);
    sf::Vector2u v = sokoban.playerLoc();
    BOOST_REQUIRE_EQUAL(v.x, 8);
    BOOST_REQUIRE_EQUAL(v.y, 5);
}

BOOST_AUTO_TEST_CASE(testlotsBoxes) {
    Sokoban sokoban;
    std::ifstream file("lotsBoxes.lvl");
    file >> sokoban;
    file.close();
    sokoban.movePlayer(SB::Direction::Down);
    sokoban.movePlayer(SB::Direction::Up);
    sokoban.movePlayer(SB::Direction::Left);
    sokoban.movePlayer(SB::Direction::Up);
    sokoban.movePlayer(SB::Direction::Up);
    sokoban.movePlayer(SB::Direction::Right);
    sokoban.movePlayer(SB::Direction::Right);
    sokoban.movePlayer(SB::Direction::Right);
    sokoban.movePlayer(SB::Direction::Down);
    sokoban.movePlayer(SB::Direction::Left);
    BOOST_REQUIRE(sokoban.isWon());
}

BOOST_AUTO_TEST_CASE(testlotsTargets) {
    Sokoban sokoban;
    std::ifstream file("lotsTargets.lvl");
    file >> sokoban;
    file.close();
    sokoban.movePlayer(SB::Direction::Left);
    sokoban.movePlayer(SB::Direction::Right);
    sokoban.movePlayer(SB::Direction::Down);
    sokoban.movePlayer(SB::Direction::Up);
    sokoban.movePlayer(SB::Direction::Right);
    sokoban.movePlayer(SB::Direction::Left);
    sokoban.movePlayer(SB::Direction::Up);
    sokoban.movePlayer(SB::Direction::Right);
    sokoban.movePlayer(SB::Direction::Right);
    sokoban.movePlayer(SB::Direction::Right);
    BOOST_REQUIRE(sokoban.isWon());
}

BOOST_AUTO_TEST_CASE(testMissingSymbol) {
    Sokoban sokoban;
    std::ifstream file("swapoff.lvl");
    file >> sokoban;
    file.close();
    std::stringstream ss;
    ss << sokoban;
    char c;
    bool b;
    while (ss >> c) {
        if (c == '1')
            b = true;
    }
    BOOST_CHECK(b);
}
