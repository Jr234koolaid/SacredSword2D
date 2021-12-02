#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include "engine/States.h"
#include "engine/Button.h"
#include "engine/AlertPopup.h"
#include "engine/OptionsMenu.h"

class ss::Button;
class ss::AlertPopup;
class ss::OptionsMenu;

namespace ss
{
	class MainMenuState :
		public ss::States
	{
	private:
		sf::Text title;
		sf::Text demo;

		std::map<std::string, ss::Button*> buttons;
		std::vector<ss::OptionsMenu*> optionScreens;
		std::vector<ss::AlertPopup*> alerts;

		sf::RectangleShape buttonContainerLogo;

		void createAlertPopup(const std::string name, const std::string alertText);
		void createOptionScreen(const std::string name);

		void deleteAlertPopup(ss::AlertPopup*& alertP);
		void deleteOptionScreen(ss::OptionsMenu*& optionsP);

		void fadeInOut();

		void updateAlertPopup(const float& deltaTime);
		void updateOptionScreen(const float& deltaTime);
		void updateButtons();

	public:
		MainMenuState(sf::RenderWindow* const& window);
		~MainMenuState();

		void Update(const float& deltaTime);
		void Render(sf::RenderTarget* const& target);


		/////////////// LUA FUNCTIONS ///////////////

		void createButton(const float x, const float y, const std::string text, const bool isActive);
		void fadeButtonRect(const std::string button, const int to, const int speed);
		void fadeButtonText(const std::string button, const int to, const int speed);
		void fadeStateIn(int speed);
		void fadeStateOut(int speed);

		static int lua_CreateButton(lua_State* L);
		static int lua_FadeButtonRect(lua_State* L);
		static int lua_FadeButtonText(lua_State* L);
		static int lua_FadeStateIn(lua_State* L);
		static int lua_FadeStateOut(lua_State* L);
	};
}

#endif