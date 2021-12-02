#ifndef ALERTPOPUP_H
#define ALERTPOPUP_H

#include "engine/Menu.h"

class ss::Button;

namespace ss
{
	class AlertPopup :
		public ss::Menu
	{
	private:
		const std::string alert;
		std::map<std::string, ss::Button*> buttons;

		sf::RectangleShape bg; // temp
		sf::Text alertText;

	public:
		AlertPopup(sf::RenderWindow* const& window, lua_State*& L, const sf::Font& font, const std::string text);
		~AlertPopup();

		inline const bool getExit() const { return exit; }

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