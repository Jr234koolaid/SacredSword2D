#include "pch.h"
#include "GameManager.h"

////////////////////////////// CONSTRUCTOR //////////////////////////////

ss::GameManager::GameManager(sf::RenderWindow* const& window)
	: rWindow(window), isRunning(true), displayFrames(true)
{
	if (!framesFont.loadFromFile("./res/fonts/SourceSansPro-Regular.ttf"))
		std::cout << "ERROR::GAMEMANAGER::Uh oh" << std::endl;

	{
		frames.setFont(framesFont);
		frames.setCharacterSize(25);
		frames.setString("null");

		const float xPos = (rWindow->getView().getCenter().x - (rWindow->getView().getSize().x / 2.f)) + 10.f;
		const float yPos = (rWindow->getView().getCenter().y - (rWindow->getView().getSize().y / 2.f)) + 10.f;

		frames.setPosition(sf::Vector2f(xPos, yPos));
	}

	states.emplace(new ss::MainMenuState(window));
	std::cout << "GAMEMANAGER::Created a MainMenu state" << std::endl;
}



////////////////////////////// DESTRUCTOR //////////////////////////////

ss::GameManager::~GameManager()
{
	while (!states.empty())
	{
		if (states.front() != nullptr)
			deletePointers(states.front());
		
		states.pop();
	}

	std::cout << "~GAMEMANAGER::Deleted the GameManager" << std::endl;
}



////////////////////////////// POINTER FUNCTIONS //////////////////////////////

void ss::GameManager::createMainMenuState()
{
	deleteLastState();

	states.emplace(new ss::MainMenuState(rWindow));
	std::cout << "GAMEMANAGER::Created a MainMenu state" << std::endl;
}

void ss::GameManager::createGameState()
{
	deleteLastState();

	states.emplace(new ss::ChapterManager(rWindow));
	std::cout << "GAMEMANAGER::Created a Game state" << std::endl;
}

void ss::GameManager::deleteLastState()
{
	deletePointers(states.front());

	states.pop();

	std::cout << "GAMEMANAGER::Deleted a state" << std::endl;
}



////////////////////////////// UPDATE METHOD //////////////////////////////

void ss::GameManager::Update(const float& deltaTime)
{
	if (isRunning)
	{
		if (!states.empty())
		{
			states.back()->Update(deltaTime);

			if (states.back()->getState() != 0)
			{

				if (states.back()->getState() == 1)
					deleteLastState();
				else if (states.back()->getState() == 3)
					createMainMenuState();
				else if (states.back()->getState() == 4)
					createGameState();
			}
		}
		else
			isRunning = false;

		{
			const float xPos = (rWindow->getView().getCenter().x - (rWindow->getView().getSize().x / 2.f)) + 10.f;
			const float yPos = (rWindow->getView().getCenter().y - (rWindow->getView().getSize().y / 2.f)) + 10.f;

			frames.setString(std::to_string(static_cast<int>(1.f / deltaTime)));
			frames.setPosition(sf::Vector2f(xPos, yPos));
		}
	}
}



////////////////////////////// RENDER METHOD //////////////////////////////

void ss::GameManager::Render(sf::RenderTarget* const& target)
{
	if (isRunning)
	{
		if (!states.empty())
			if (states.back()->getState() == 0)
				states.back()->Render(target);

		target->draw(frames);
	}
}