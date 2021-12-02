#ifndef DIALOGUECOMPONENT_H
#define DIALOGUECOMPONENT_H

#include "engine/Menu.h"

namespace ss
{
	class DialogueComponent
		: public ss::Menu
	{
	private:
		bool displayImage, displayDialogue, displayBackground, mousePressed;
		std::string setStr;
		std::string dialogueString;

		sf::RectangleShape textBg; // temp
		sf::Text dialogueCharacter;
		sf::Text dialogueText;

	public:
		DialogueComponent(sf::RenderWindow* const& window, lua_State*& L, const sf::Font& font, const uint32_t scene);
		~DialogueComponent();

		void Update(const float& deltaTime, const sf::Vector2f& mousePosView);
		void Render(sf::RenderTarget* const& target);


		/////////////// LUA FUNCTIONS ///////////////

		void setDialogueText(const std::string text, const uint32_t op);
		void setDialogueCharacter(const std::string text);
		void setString(const std::string text);

		static int lua_SetDialogueText(lua_State* L);
		static int lua_SetDialogueCharacter(lua_State* L);
		static int lua_SetString(lua_State* L);
	};
}

#endif