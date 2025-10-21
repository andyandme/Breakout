#include "GameManager.h"
#include "Ball.h"
#include "PowerupManager.h"
#include <iostream>

GameManager::GameManager(sf::RenderWindow* window)
    : _window(window), _paddle(nullptr), _ball(nullptr), _brickManager(nullptr), _powerupManager(nullptr),
    _messagingSystem(nullptr), _ui(nullptr), _pause(false), _time(0.f), _lives(3), _pauseHold(0.f), _levelComplete(false),
    _powerupInEffect({ none,0.f }), _timeLastPowerupSpawned(0.f)
{
    _font.loadFromFile("font/montS.ttf");
    _masterText.setFont(_font);
    _masterText.setPosition(50, 400);
    _masterText.setCharacterSize(48);
    _masterText.setFillColor(sf::Color::Yellow);
}

void GameManager::initialize()
{
    _paddle = new Paddle(_window);
    _brickManager = new BrickManager(_window, this);
    _messagingSystem = new MessagingSystem(_window);
    _ball = new Ball(_window, 400.0f, this); 
    _powerupManager = new PowerupManager(_window, _paddle, _ball);
    _ui = new UI(_window, _lives, this);

    // Create bricks
    _brickManager->createBricks(5, 10, 80.0f, 30.0f, 5.0f);
    
    _baseView = _window->getDefaultView();
    _currentView = _baseView;

}

void GameManager::update(float dt)
{
    _powerupInEffect = _powerupManager->getPowerupInEffect();
    _ui->updatePowerupText(_powerupInEffect);
    _powerupInEffect.second -= dt;
    
    if (_isShaking) {
        _shakeTimer += dt;
        if (_shakeTimer < _shakeDuration) {
            auto randUnit = []() {
                return (static_cast<float>(std::rand() % 2001) / 1000.f) - 1.f;


            };
            float offsetX = randUnit() * _shakeMagnitude;
            float offsetY = randUnit() * _shakeMagnitude;

            _currentView = _baseView;
            _currentView.move(offsetX, offsetY);
            _window->setView(_currentView);


        }
        else
        {
            _isShaking = false;
            _shakeTimer = 0.f;
            _shakeDuration = 0.f;
            _shakeMagnitude = 0.f;
            _window->setView(_baseView);

        }
    }


    //if (_lives <= 0)
    //{
    //    _masterText.setString("Game over.");
    //    return;
    //}
    //if (_levelComplete)
    //{
    //    _masterText.setString("Level completed.");
    //    return;
    //}
    
    if (_lives <= 0) {
        _masterText.setString("Game Over - Press Enter to Restart");
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        {
            resetGame();
        }
        return;
    }
    if (_levelComplete) {
        _masterText.setString("Level Complete - Press Enter to Restart");
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        {
            resetGame();
        }
        return;


    }

    // pause and pause handling
    if (_pauseHold > 0.f) _pauseHold -= dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
    {
        if (!_pause && _pauseHold <= 0.f)
        {
            _pause = true;
            _masterText.setString("paused.");
            _pauseHold = PAUSE_TIME_BUFFER;
        }
        if (_pause && _pauseHold <= 0.f)
        {
            _pause = false;
            _masterText.setString("");
            _pauseHold = PAUSE_TIME_BUFFER;
        }
    }
    if (_pause)
    {
        return;
    }

    // timer.
    _time += dt;


    //if (_time > _timeLastPowerupSpawned + POWERUP_FREQUENCY && (rand()% POWERUP_SPAWN_ROLL_MAX == 0))      // TODO parameterise <== Complete
    //{
    //    _powerupManager->spawnPowerup();
    //    _timeLastPowerupSpawned = _time;
    //}

    // move paddle
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) _paddle->moveRight(dt);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) _paddle->moveLeft(dt);

    // update everything 
    _paddle->update(dt);
    _ball->update(dt);
    _powerupManager->update(dt);
}

void GameManager::loseLife()
{
    _lives--;
    _ui->lifeLost(_lives);
    startScreenShake(0.2f, 15.f);

    // TODO screen shake. <==== Completed
}

void GameManager::render()
{
    _paddle->render();
    _ball->render();
    _brickManager->render();
    _powerupManager->render();
    _window->draw(_masterText);
    _ui->render();
}

void GameManager::levelComplete()
{
    _levelComplete = true;
}

void GameManager::startScreenShake(float duration, float magnitude)
{
    _isShaking = true;
    _shakeTimer = 0.f;
    _shakeDuration = duration;
    _shakeMagnitude = magnitude;


}

void GameManager::resetGame()
{
    //Clear Level
    _masterText.setString("");
    _pause = false;
    _pauseHold = 0.f;
    _time = 0.f;
    _timeLastPowerupSpawned = 0.f;
    _levelComplete = false;
    _lives = INITIAL_LIVES;

    //Remake level
    _brickManager->clearBricks();
    _brickManager->createBricks(5, 10, 80.0f, 30.0f, 5.0f);

    delete _paddle;
    _paddle = new Paddle(_window);
    delete _ball; 
    _ball = new Ball(_window, BALL_SPEED, this);
    delete _powerupManager;
    _powerupManager = new PowerupManager(_window, _paddle, _ball);



    delete _ui;
    _ui = new UI(_window, _lives, this);
}



sf::RenderWindow* GameManager::getWindow() const { return _window; }
UI* GameManager::getUI() const { return _ui; }
Paddle* GameManager::getPaddle() const { return _paddle; }
BrickManager* GameManager::getBrickManager() const { return _brickManager; }
PowerupManager* GameManager::getPowerupManager() const { return _powerupManager; }
