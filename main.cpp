// Copyright 2024 Jun Ye
#include <iostream>
#include <fstream>
#include <iomanip>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Sokoban.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: ./Sokoban [level file]" << std::endl;
        return -1;
    }
    std::string levelFilePath = argv[1];
    SB::Sokoban sokoban;
    std::ifstream file(levelFilePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << levelFilePath << std::endl;
        return -1;
    }
    file >> sokoban;
    file.close();

    sf::RenderWindow window(sf::VideoMode(sokoban.pixelWidth(),
        sokoban.pixelHeight()), "Sokoban Game");
    window.setFramerateLimit(90);


    // Load font
    sf::Font font;
    if (!font.loadFromFile("rcs/OpenSans.ttf")) {
        std::cerr << "Failed to load font." << std::endl;
        return -1;
    }
    sf::Text timerText;
    timerText.setFont(font);
    timerText.setCharacterSize(20);
    timerText.setFillColor(sf::Color::White);
    timerText.setPosition(10, 10);
    sf::Clock clock;

    sf::Texture winTexture;
    if (!winTexture.loadFromFile("rcs/youwin.png")) {
        std::cerr << "Failed to load texture." << std::endl;
        return -1;
    }
    sf::Sprite winSprite(winTexture);
    winSprite.scale(0.5, 0.5);
    sf::FloatRect bounds = winSprite.getLocalBounds();
    winSprite.setOrigin(bounds.width / 2, bounds.height / 2);

    sf::SoundBuffer buffer;
    buffer.loadFromFile("rcs/winner.wav");
    sf::Sound sound(buffer);

    // std::cout << sokoban;

    while (window.isOpen()) {
        sf::Time elapsed = clock.getElapsedTime();
        int seconds = elapsed.asSeconds();
        int minutes = seconds / 60;
        seconds %= 60;
        std::ostringstream timerString;
        timerString << std::setfill('0') << std::setw(2) << minutes
                    << ":" << std::setw(2) << seconds;
        timerText.setString(timerString.str());

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed) {
                window.close();
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
            } else if (event.type == sf::Event::KeyPressed) {
                if (!sokoban.isWon()) {
                    switch (event.key.code) {
                        case sf::Keyboard::W:
                        case sf::Keyboard::Up:
                            sokoban.movePlayer(SB::Direction::Up);
                            // std::cout << sokoban;
                            break;
                        case sf::Keyboard::A:
                        case sf::Keyboard::Left:
                            sokoban.movePlayer(SB::Direction::Left);
                            // std::cout << sokoban;
                            break;
                        case sf::Keyboard::S:
                        case sf::Keyboard::Down:
                            sokoban.movePlayer(SB::Direction::Down);
                            // std::cout << sokoban;
                            break;
                        case sf::Keyboard::D:
                        case sf::Keyboard::Right:
                            sokoban.movePlayer(SB::Direction::Right);
                            // std::cout << sokoban;
                            break;
                        case sf::Keyboard::R:
                            sokoban.restart();
                            clock.restart();
                            break;
                        case sf::Keyboard::Z:
                            sokoban.undoMove();
                            // std::cout << sokoban;
                            break;
                        default:
                            break;
                    }
                } else if (event.key.code == sf::Keyboard::R) {
                    sokoban.restart();
                }
            }
        }
        window.clear();
        if (!sokoban.isWon()) {
            window.draw(sokoban);
            window.draw(timerText);
        } else {
            if (sound.getStatus() != sf::Sound::Playing) {
                sound.play();
            }
            winSprite.setPosition(sokoban.pixelWidth() / 2, sokoban.pixelHeight() / 2);
            window.draw(winSprite);
        }
        window.display();
    }

    return 0;
}
