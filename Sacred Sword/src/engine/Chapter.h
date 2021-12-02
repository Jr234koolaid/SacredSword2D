#ifndef CHAPTER_H
#define CHAPTER_H

#include "game/Player.h"
#include "game/PauseMenu.h"
#include "game/Enemy.h"
#include "game/WorldObject.h"
#include "game/DialogueComponent.h"

namespace ss
{
	class Chapter
	{
	protected:
		enum state_states { state_Open = 0, state_Cutscene, state_Gameplay, state_Exit };

		const int waitMax;
		int wait;
		bool checkScript, exit, paused, alert, inCutscene, inGameplay;

		sf::RenderWindow* const& rWindow;
		sf::View playerCamera;
		const sf::Font& textFont;

		lua_State*& L;

		virtual void createPauseMenu() = 0;

		const bool updateWait();
		const bool checkLua(lua_State*& L, const int r);

	public:
		Chapter(sf::RenderWindow* const& window, lua_State*& L, const sf::Font& font);
		virtual ~Chapter();

		const unsigned getState() const;

		virtual void Update(const float& deltaTime, const sf::Vector2f& mousePosView) = 0;
		virtual void Render(sf::RenderTarget* const& target) = 0;


		/////////////// LUA FUNCTIONS ///////////////

		virtual void createPlayer(const float x, const float y, const std::string name) = 0;
	};
}

#endif