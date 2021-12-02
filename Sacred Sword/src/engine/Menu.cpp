#include "pch.h"
#include "Menu.h"

////////////////////////////// CHECK LUA //////////////////////////////

const bool ss::Menu::checkLua(lua_State*& L, const int r)
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

ss::Menu::Menu(sf::RenderWindow* const& window, lua_State*& L, const sf::Font& font)
	: rWindow(window), L(L), checkScript(false), exit(false), quit(false), alert(false), inOptions(false), textFont(font)
{
	background.setSize(sf::Vector2f(rWindow->getView().getSize().x, rWindow->getView().getSize().y));
	background.setFillColor(sf::Color(10, 10, 10, 175));

	const float posX = rWindow->getView().getCenter().x - (rWindow->getView().getSize().x / 2.f);
	const float posY = rWindow->getView().getCenter().y - (rWindow->getView().getSize().y / 2.f);

	background.setPosition(sf::Vector2f(posX, posY));
}



////////////////////////////// DESTRUCTOR //////////////////////////////

ss::Menu::~Menu()
{
	std::cout << "~MENU::Deleted a menu" << std::endl;
}