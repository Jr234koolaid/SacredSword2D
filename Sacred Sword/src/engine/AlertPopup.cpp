#include "pch.h"
#include "AlertPopup.h"

////////////////////////////// CONSTRUCTOR //////////////////////////////

ss::AlertPopup::AlertPopup(sf::RenderWindow* const& window, lua_State*& L, const sf::Font& font, const std::string text)
	: Menu(window, L, font), alert(text)
{
	checkScript = checkLua(L, luaL_dofile(L, "./scripts/engine/AlertPopup.lua"));

	if (!checkScript)
	{
		quit = true;
		std::cout << "MAINMENU::ERROR::Could not find LUA script!" << std::endl;
	}

	lua_register(L, "lua_CreateButton", lua_CreateButton);
	lua_register(L, "lua_FadeFromToRect", lua_FadeButtonRect);
	lua_register(L, "lua_FadeFromToText", lua_FadeButtonText);

	{
		bg.setFillColor(sf::Color(20, 20, 20, 245));
		bg.setSize(sf::Vector2f(650.f, 325.f));

		const float posX = rWindow->getView().getCenter().x - (bg.getSize().x / 2.f);
		const float posY = rWindow->getView().getCenter().y - (bg.getSize().y / 2.f);

		bg.setPosition(sf::Vector2f(posX, posY));
		bg.setOutlineColor(sf::Color::Yellow);
		bg.setOutlineThickness(2.f);
	}

	lua_getglobal(L, "OnMenuEnter");
	if (lua_isfunction(L, -1))
	{
		lua_pushlightuserdata(L, this);
		lua_pushnumber(L, bg.getPosition().x);
		lua_pushnumber(L, bg.getPosition().y);
		lua_pushnumber(L, bg.getSize().x / 2.f);
		lua_pushnumber(L, bg.getSize().y / 2.f);

		checkLua(L, lua_pcall(L, 5, 0, 0));
	}

	{
		alertText.setFont(textFont);
		alertText.setCharacterSize(25);
		alertText.setString(alert);

		const float posX = bg.getPosition().x + ((bg.getSize().x / 2.f) - (alertText.getGlobalBounds().width / 2.f));
		const float posY = bg.getPosition().y + (alertText.getGlobalBounds().height * 1.25f); // 1.75

		alertText.setPosition(sf::Vector2f(posX, posY));
	}
}



////////////////////////////// DESTRUCTOR //////////////////////////////

ss::AlertPopup::~AlertPopup()
{
	if (!buttons.empty())
	{
		for (auto& i : buttons)
			deletePointers(i.second);
	}
	buttons.clear();
	std::cout << "~ALERTPOPUP::deleted the buttons and cleared the map" << std::endl;
}



////////////////////////////// UPDATE METHOD //////////////////////////////

void ss::AlertPopup::Update(const float& deltaTime, const sf::Vector2f& mousePosView)
{
	lua_getglobal(L, "AnimateButtons");
	if (lua_isfunction(L, -1))
		checkLua(L, lua_pcall(L, 0, 0, 0));

	if (!buttons.empty())
	{
		for (auto& i : buttons)
			i.second->Update(mousePosView);

		if (buttons["EXIT"]->isPressed())
			exit = true;
		else if (buttons["BACK"]->isPressed())
			quit = true;
	}
}



////////////////////////////// RENDER METHOD //////////////////////////////

void ss::AlertPopup::Render(sf::RenderTarget* const& target)
{
	target->draw(background);
	target->draw(bg);

	if (!buttons.empty())
		for (auto& i : buttons)
			i.second->Render(target);

	target->draw(alertText);
}



////////////////////////////// LUA FUNCTIONS //////////////////////////////

void ss::AlertPopup::createButton(const float x, const float y, const std::string text, const bool isActive)
{
	buttons[text] = new ss::Button(x, y, text, textFont, isActive);
}

void ss::AlertPopup::fadeButtonRect(const std::string button, const int to, const int speed)
{
	if (!buttons[button]->getAnimator().getFinishedFading())
		buttons[button]->setButtonRectOpacity(buttons[button]->getAnimator().FadeIn(buttons[button]->getButtonRectOpacity(), to, speed));
}

void ss::AlertPopup::fadeButtonText(const std::string button, const int to, const int speed)
{
	if (!buttons[button]->getAnimator().getFinishedFading())
		buttons[button]->setButtonTextOpacity(buttons[button]->getAnimator().FadeIn(buttons[button]->getButtonTextOpacity(), to, speed));
}

int ss::AlertPopup::lua_CreateButton(lua_State* L)
{
	if (lua_gettop(L) != 5)
		return -1;

	ss::AlertPopup* popup = static_cast<ss::AlertPopup*>(lua_touserdata(L, 1));
	const float posX = static_cast<float>(lua_tonumber(L, 2));
	const float posY = static_cast<float>(lua_tonumber(L, 3));
	const std::string text = static_cast<std::string>(lua_tostring(L, 4));
	const bool active = static_cast<bool>(lua_toboolean(L, 5));

	popup->createButton(posX, posY, text, active);

	return 0;
}

int ss::AlertPopup::lua_FadeButtonRect(lua_State* L)
{
	if (lua_gettop(L) != 4)
		return -1;

	ss::AlertPopup* alert = static_cast<ss::AlertPopup*>(lua_touserdata(L, 1));
	const std::string button = static_cast<std::string>(lua_tostring(L, 2));
	const int toOpacity = static_cast<int>(lua_tonumber(L, 3));
	const int speed = static_cast<int>(lua_tonumber(L, 4));

	alert->fadeButtonRect(button, toOpacity, speed);

	return 0;
}

int ss::AlertPopup::lua_FadeButtonText(lua_State* L)
{
	if (lua_gettop(L) != 4)
		return -1;

	ss::AlertPopup* alert = static_cast<ss::AlertPopup*>(lua_touserdata(L, 1));
	const std::string button = static_cast<std::string>(lua_tostring(L, 2));
	const int toOpacity = static_cast<int>(lua_tonumber(L, 3));
	const int speed = static_cast<int>(lua_tonumber(L, 4));

	alert->fadeButtonText(button, toOpacity, speed);

	return 0;
}