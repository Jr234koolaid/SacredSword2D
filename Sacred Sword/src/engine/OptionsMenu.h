#ifndef OPTIONSMENU_H
#define OPTIONSMENU_H

#include "engine/Menu.h"

class ss::Button;

namespace ss
{
	class OptionsMenu :
		public ss::Menu
	{
	private:
		std::map<std::string, ss::Button*> buttons;

		sf::RectangleShape bg; // temp
		sf::RectangleShape divider;
		sf::Text optionsL;

	public:
		OptionsMenu(sf::RenderWindow* const& window, lua_State*& L, const sf::Font& font);
		~OptionsMenu();

		void Update(const float& deltaTime, const sf::Vector2f& mousePosView);
		void Render(sf::RenderTarget* const& target);


		/////////////// LUA FUNCTIONS ///////////////

		void createButton(const float x, const float y, const std::string text, const bool isActive);
		void fadeButtonRect(const std::string button, const int to, const int speed);
		void fadeButtonText(const std::string button, const int to, const int speed);

		static int lua_CreateButton(lua_State* L);
		static int lua_FadeButtonRect(lua_State* L);
		static int lua_FadeButtonText(lua_State* L);
	};
}

#endif