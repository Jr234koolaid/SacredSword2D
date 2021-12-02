#ifndef WINDOW_H
#define WINDOW_H

#include "engine/GameManager.h"

class ss::GameManager;

namespace ss
{
	class Window
	{
	private:
		float deltaTime;
		const uint32_t framerateLimit;

		const std::string title;

		ss::GameManager* manager = nullptr;

		sf::RenderWindow* window = nullptr;

		sf::VideoMode windowResolution;
		sf::ContextSettings windowSettings;
		sf::Event ev;
		sf::Clock dtClock;

		void Update();
		void Render();

	public:
		Window();
		~Window();

		void Run();
	};
}

#endif