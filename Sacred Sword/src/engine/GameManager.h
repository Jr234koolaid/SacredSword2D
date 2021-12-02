#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "States.h"
#include "mainmenu/MainMenuState.h"
#include "game/ChapterManager.h"

class ss::States;
class ss::MainMenuState;
class ss::ChapterManager;

namespace ss
{
	class GameManager
	{
	private:
		bool isRunning, displayFrames;

		std::queue<ss::States*> states;

		sf::RenderWindow* const& rWindow;
		sf::Font framesFont;
		sf::Text frames;

		void createMainMenuState();
		void createGameState();
		void deleteLastState();

	public:
		GameManager(sf::RenderWindow* const& window);
		~GameManager();

		inline const bool getIsRunning() const { return isRunning; }

		void Update(const float& deltaTime);
		void Render(sf::RenderTarget* const& target);
	};
}

#endif