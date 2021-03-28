/*
** EPITECH PROJECT, 2021
** G-JAM-001-NCE-0-2-jam-marton.szuts
** File description:
** EntityController
*/

#include "EntityController.hpp"

EntityController::EntityController(Player *player)
{
    srand(time(NULL));
    _textures.push_back(new sf::Texture);
    _textures[ASTEROID]->loadFromFile("assets/asteroid1.png");
    _player = player;
    _parallax = new Parallax;
    _asteroidClock.restart();
    _randTime = 0.4 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 2.0));
}

EntityController::~EntityController()
{
    for (auto iter : _textures)
        delete iter;
    for (auto iter : _asteroid)
        delete iter;
}

void EntityController::drawAll(sf::RenderWindow *w) const
{
    _parallax->drawLayers(w);
    if (global_scene == GAME) {
        for (auto a : _asteroid) {
            a->drawParticles(w);
            w->draw(a->getSprite());
        }
        _player->drawPlayer(w);
    }
}

void EntityController::updateAll()
{
    _parallax->moveLayers();
    if (global_scene == GAME) {
        updatePlayer();
        updateAsteroids();
    }
}

void EntityController::addAsteroid(sf::Vector2f pos)
{
    _asteroid.push_back(new Asteroid(_textures[ASTEROID], pos, 2 + (rand() % 6)));
}

void EntityController::createRandomAsteroids()
{
    bool isSpawned = false;
    if (_asteroidClock.getElapsedTime().asSeconds() > _randTime) {
        for (int i = 0; !isSpawned; ) {
            float x = rand() % 1920;
            i = 0;
            addAsteroid(sf::Vector2f{x, -300});
            _asteroidClock.restart();
            _randTime = 0.4 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 2.0));
            for (auto itr : _asteroid) {
                if (itr != _asteroid.back() && !itr->isColliding(_asteroid.back())) {
                    i++;
                }
            }
            if (i + 1 == _asteroid.size())
                isSpawned = true;
            else
                _asteroid.pop_back();
        }
    }
}

void EntityController::updatePlayer()
{
    _player->update();
    int i = 0;
    for (auto itr : _asteroid) {
        pair<Ship *, Ship *> p = _player->getShips();
        if (itr->isColliding((Entity *)p.first)) {
            _asteroid.erase(_asteroid.begin() + i);
            p.first->setHpShip(p.first->getHpShip() - 10);
            p.first->getHud()->updateHp(p.first->getHpShip());
            if (p.first->getHpShip() <= 0)
                global_scene = GAME_OVER;
        }
        if (itr->isColliding((Entity *)p.second)) {
            _asteroid.erase(_asteroid.begin() + i);
            p.second->setHpShip(p.second->getHpShip() - 10);
            p.second->getHud()->updateHp(p.second->getHpShip());
            if (p.second->getHpShip() <= 0)
                global_scene = GAME_OVER;
        }
        i++;
    }
}

void EntityController::updateAsteroids()
{
    createRandomAsteroids();
    for (auto itr : _asteroid) {
        itr->moveAsteroid();
        itr->rotateAsteroid();
    }
    destroyAsteroids();
}

void EntityController::destroyAsteroids()
{
    for (size_t i = 0; i < _asteroid.size(); i++) {
        if (_asteroid[i]->getPos().y >= 2000) {
            _asteroid.erase(_asteroid.begin() + i);
        }
    }
}