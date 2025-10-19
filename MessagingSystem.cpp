#include "MessagingSystem.h"

MessagingSystem::MessagingSystem(sf::RenderWindow* window)
{
	_window = window;
	_message.setCharacterSize(24);
	_font.loadFromFile("font/montS.ttf");
	_message.setFont(_font);
	_message.setFillColor(sf::Color::White);
	_message.setPosition(10, 10);
		
}

MessagingSystem::~MessagingSystem()
{
}

void MessagingSystem::update(float dt)
{
	// early exit
	if (_timeToDisplay == 0.f) return;
	
	// increment timer if it should be running, or reset message if expired.
	if (_messageTimer < _timeToDisplay)
	{
		_messageTimer += dt;
	}
	else
	{
		_message.setString("");
		_timeToDisplay = 0.f;
		_messageTimer = 0.f;
	}
}

void MessagingSystem::render()
{
	_window->draw(_message);
}

// update alert with str <message> for float <timeToDisplay> in seconds.
void MessagingSystem::setMessage(std::string message, float timeToDisplay)
{
	_message.setString(message);
	_timeToDisplay = timeToDisplay;
}
