#include "pch.h"
#include "Chapter.h"

////////////////////////////// CHECK LUA //////////////////////////////

const bool ss::Chapter::checkLua(lua_State*& L, const int r)
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

ss::Chapter::Chapter(sf::RenderWindow* const& window, lua_State*& L, const sf::Font& font)
	: rWindow(window), L(L), textFont(font),
	waitMax(10), wait(0),
	checkScript(false), exit(false), paused(false), alert(false), inCutscene(false), inGameplay(false)
{
}



////////////////////////////// DESTRUCTOR //////////////////////////////

ss::Chapter::~Chapter()
{
}



////////////////////////////// UPDATE METHODS //////////////////////////////

const bool ss::Chapter::updateWait()
{
	if (wait > 0)
		wait--;

	if (wait == 0)
		return true;

	return false;
}

const unsigned ss::Chapter::getState() const
{
	if (exit)
		return state_Exit;

	return state_Open;
}