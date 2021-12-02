#ifndef PAUSEMENU_H
#define PAUSEMENU_H

#include "engine/Menu.h"
#include "engine/AlertPopup.h"
#include "engine/OptionsMenu.h"

class ss::AlertPopup;
class ss::OptionsMenu;

namespace ss
{
    class PauseMenu :
        public ss::Menu
    {
    private:
        std::map<std::string, ss::Button*> buttons;
        std::vector<ss::OptionsMenu*> optionsMenu;
        std::vector<ss::AlertPopup*> alerts;

        sf::Font menuFont;
        sf::RectangleShape pauseLogo;
        sf::RectangleShape bg; // Temp

        void createAlertPopup(const std::string name, const std::string alertText);
        void createOptionScreen(const std::string name);

        void deleteAlertPopup(ss::AlertPopup*& alertP);
        void deleteOptionScreen(ss::OptionsMenu*& optionsP);

        void setBackgroundPosition();

        void updateAlertPopup(const float& deltaTime, const sf::Vector2f& mousePosView);
        void updateOptionScreen(const float& deltaTime, const sf::Vector2f& mousePosView);
        void updateButtons(const sf::Vector2f& mousePosView);

    public:
        PauseMenu(sf::RenderWindow* const& window, lua_State*& L, const sf::Font& font);
        ~PauseMenu();

        void Update(const float& deltaTime, const sf::Vector2f& mousePosView);
        void Render(sf::RenderTarget* const &target);


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