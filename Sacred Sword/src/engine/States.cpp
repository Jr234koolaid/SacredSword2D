#include "pch.h"
#include "States.h"

////////////////////////////// LUA CHECK //////////////////////////////

const bool ss::States::checkLua(lua_State*& L, const int r)
{
	if (r != LUA_OK)
	{
		std::string error = lua_tostring(L, -1);
		std::cout << error << std::endl;
		return false;
	}

	return true;
}



////////////////////////////// CONSTRUCTOR //////////////////////////////

ss::States::States(sf::RenderWindow* const& window)
	: rWindow(window),
	fadeState(0),
	checkScript(false), quit(false), paused(false), alert(false), transferMainMenu(false), transferGame(false),
	fadedIn(false), hFadedIn(false), sFadingOut(false), fadedOut(false)
{
	L = luaL_newstate();
	luaL_openlibs(L);
	std::cout << "STATES::Opened a LUA Virtual Machine" << std::endl;

	fadeRect.setSize(rWindow->getView().getSize());
	fadeRect.setPosition(rWindow->getView().getCenter() - (rWindow->getView().getSize() / 2.f));
	fadeRect.setFillColor(sf::Color::Black);
}



////////////////////////////// DESTRUCTOR //////////////////////////////

ss::States::~States()
{
	lua_close(L);
	std::cout << "~STATES::Closed a LUA Virtual Machine" << std::endl;
	std::cout << "~STATES::Deleted a state" << std::endl;
}



////////////////////////////// UPDATE METHOD //////////////////////////////

const unsigned ss::States::getState() const
{
	if (quit)
		return state_Quit;
	else if (transferMainMenu)
		return state_Transfer_MainMenu;
	else if (transferGame)
		return state_Transfer_Game;

	return state_Open;
}

void ss::States::UpdateMousePos(sf::RenderWindow* const& window)
{
	mousePosView = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
}