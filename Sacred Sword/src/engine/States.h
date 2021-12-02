#ifndef STATES_H
#define STATES_H

namespace ss
{
	class States
	{
	protected:
		enum state_states { state_Open = 0, state_Quit, state_Paused, state_Transfer_MainMenu, state_Transfer_Game };

		uint8_t fadeState;

		bool checkScript, quit, paused, alert, transferMainMenu, transferGame;
		bool fadedIn, hFadedIn, sFadingOut, fadedOut;

		sf::RenderWindow* const& rWindow;
		sf::Vector2f mousePosView;
		sf::Font textFont;
		sf::RectangleShape fadeRect;

		lua_State* L = nullptr;

		const bool checkLua(lua_State*& L, const int r);

		virtual void fadeInOut() = 0;

	public:
		States(sf::RenderWindow* const& window);
		virtual ~States();

		const unsigned getState() const;

		void UpdateMousePos(sf::RenderWindow* const& window);

		virtual void Update(const float& deltaTime) = 0;
		virtual void Render(sf::RenderTarget* const& target) = 0;
	};
}

#endif