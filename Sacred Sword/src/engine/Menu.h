#ifndef MENU_H
#define MENU_H

#include "engine/Button.h"

class ss::Button;

namespace ss
{
	class Menu
	{
	protected:
		bool checkScript, exit, quit, alert, inOptions;

		sf::RenderWindow* const& rWindow;
		sf::RectangleShape background;
		const sf::Font& textFont;

		lua_State*& L;

		const bool checkLua(lua_State*& L, const int r);

	public:
		Menu(sf::RenderWindow* const& window, lua_State*& L, const sf::Font& font);
		virtual ~Menu();

		inline const bool getExit() const { return exit; }
		inline const bool getQuit() const { return quit; }

		virtual void Update(const float& deltaTime, const sf::Vector2f& mousePosView) = 0;
		virtual void Render(sf::RenderTarget* const& target) = 0;
	};
}

#endif