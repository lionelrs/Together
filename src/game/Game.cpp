/*
** EPITECH PROJECT, 2021
** G-JAM-001-NCE-0-2-jam-marton.szuts
** File description:
** Game
*/

#include "Game.hpp"

Scene global_scene = MENU;
string global_language = ".fr";

Game::Game(const std::string &winTitle, size_t width, size_t height)
{
    _window.create(sf::VideoMode(width, height), winTitle);
    _window.setFramerateLimit(60);
    _player = new Player;
    _controller = new EntityController(_player);

    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPointSize(7);
}

Game::~Game()
{
    delete _player;
    delete _controller;
}

void Game::handleEvents()
{
    while (_window.pollEvent(_event)) {
        _prologue.enterEvent(_event);
        if (_event.type == sf::Event::Closed)
            _window.close();
        if (global_scene == GAME) {
            if (_event.type == sf::Event::KeyPressed)
                _player->setShipMove(_event.key.code);
            if (_event.type == sf::Event::KeyReleased)
                _player->unsetShipMove(_event.key.code);
        }
        if (global_scene == MENU) {
            if (_event.type == sf::Event::MouseButtonPressed) {
                _menu.clickButton(&_window);
            }
            if (_event.type == sf::Event::MouseButtonReleased) {
                _menu.releaseButton(&_window);
            }
        }
        if (global_scene == GAME_OVER) {
            if (_event.type == sf::Event::KeyPressed) {
                if (_event.key.code == sf::Keyboard::Escape)
                    _window.close();
                if (_event.key.code == sf::Keyboard::M) {
                    global_scene = MENU;
                }
            }
        }
    }
}

void Game::clear()
{
    _window.clear(sf::Color::Black);
}

void Game::display()
{
    _window.display();
}

void Game::run()
{
    while (_window.isOpen()) {
        handleEvents();
        clear();
        if (global_scene == GAME) {
            _controller->updateAll();
            _controller->drawAll(&_window);
        } else if (global_scene == MENU) {
            _menu.displayMenu(&_window);
            _menu.menuAnimation();
            _prologue.openPrologue(&_window);
        }
        else if (global_scene == GAME_OVER) {
            _controller->updateAll();
            _controller->drawAll(&_window);
            _gameOver.updateText();
            _gameOver.drawEnd(&_window);
        }
        display();
    }
}