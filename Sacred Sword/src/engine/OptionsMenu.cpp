#include "pch.h"
#include "OptionsMenu.h"

////////////////////////////// CONSTRUCTOR //////////////////////////////

ss::OptionsMenu::OptionsMenu(sf::RenderWindow* const& window, lua_State*& L, const sf::Font& font)
	: Menu(window, L, font)
{
	checkScript = checkLua(L, luaL_dofile(L, "./scripts/engine/OptionsMenu.lua"));

	if (!checkScript)
	{
		quit = true;
		std::cout << "OPTIONSMENU::ERROR::Could not find LUA script!" << std::endl;
	}

	lua_register(L, "lua_CreateButton", lua_CreateButton);
	lua_register(L, "lua_FadeFromToRect", lua_FadeButtonRect);
	lua_register(L, "lua_FadeFromToText", lua_FadeButtonText);

	{
		bg.setFillColor(sf::Color(20, 20, 20, 245));
		bg.setSize(sf::Vector2f(1000.f, 575.f));

		float posX = rWindow->getView().getCenter().x - (bg.getSize().x / 2.f);
		float posY = rWindow->getView().getCenter().y - (bg.getSize().y / 2.f);

		bg.setPosition(sf::Vector2f(posX, posY));
		bg.setOutlineThickness(2.f);

		const float sizeY = bg.getSize().y - (bg.getSize().y / 4.f);

		divider.setSize(sf::Vector2f(1.f, sizeY));

		posX += bg.getSize().x / 3.f;
		posY += bg.getSize().y / 4.f;

		divider.setPosition(sf::Vector2f(posX, posY));
	}

	lua_getglobal(L, "OnMenuEnter");
	if (lua_isfunction(L, -1))
	{
		lua_pushlightuserdata(L, this);
		lua_pushnumber(L, bg.getPosition().x);
		lua_pushnumber(L, bg.getPosition().y);

		checkLua(L, lua_pcall(L, 3, 0, 0));
	}

	{
		const float posX = bg.getPosition().x + 25.f;
		const float posY = bg.getPosition().y + 25.f;

		optionsL.setFont(textFont);
		optionsL.setCharacterSize(75);
		optionsL.setString("OPTIONS");
		optionsL.setPosition(sf::Vector2f(posX, posY));
	}
}



////////////////////////////// DESTRUCTOR //////////////////////////////

ss::OptionsMenu::~OptionsMenu()
{
	if (!buttons.empty())
	{
		for (auto& i : buttons)
			deletePointers(i.second);
	}
	buttons.clear();
	std::cout << "~OPTIONSMENU::deleted the buttons and cleared the map" << std::endl;
}



////////////////////////////// UPDATE METHOD //////////////////////////////

void ss::OptionsMenu::Update(const float& deltaTime, const sf::Vector2f& mousePosView)
{
	lua_getglobal(L, "AnimateButtons");
	if (lua_isfunction(L, -1))
		checkLua(L, lua_pcall(L, 0, 0, 0));

	if (!buttons.empty())
	{
		for (auto& i : buttons)
		{
			if (i.second != nullptr)
				i.second->Update(mousePosView);
		}

		if (buttons["WINDOW"]->isPressed())
			;
		else if (buttons["TEXT"]->isPressed())
			;
		else if (buttons["AUDIO"]->isPressed())
			;
		else if (buttons["CREDITS"]->isPressed())
			;
		else if (buttons["BACK"]->isPressed())
			quit = true;
	}
}



////////////////////////////// RENDER METHOD //////////////////////////////

void ss::OptionsMenu::Render(sf::RenderTarget* const& target)
{
	target->draw(background);
	target->draw(bg);
	target->draw(divider);

	if (!buttons.empty())
	{
		for (auto& i : buttons)
			i.second->Render(target);
	}

	target->draw(optionsL);
}



////////////////////////////// LUA FUNCTIONS //////////////////////////////

void ss::OptionsMenu::createButton(const float x, const float y, const std::string text, const bool isActive)
{
	buttons[text] = new ss::Button(x, y, text, textFont, isActive);
}

void ss::OptionsMenu::fadeButtonRect(const std::string button, const int to, const int speed)
{
	if (!buttons[button]->getAnimator().getFinishedFading())
		buttons[button]->setButtonRectOpacity(buttons[button]->getAnimator().FadeIn(buttons[button]->getButtonRectOpacity(), to, speed));
}

void ss::OptionsMenu::fadeButtonText(const std::string button, const int to, const int speed)
{
	if (!buttons[button]->getAnimator().getFinishedFading())
		buttons[button]->setButtonTextOpacity(buttons[button]->getAnimator().FadeIn(buttons[button]->getButtonTextOpacity(), to, speed));
}

int ss::OptionsMenu::lua_CreateButton(lua_State* L)
{
	if (lua_gettop(L) != 5)
		return -1;

	ss::OptionsMenu* options = static_cast<ss::OptionsMenu*>(lua_touserdata(L, 1));
	const float posX = static_cast<float>(lua_tonumber(L, 2));
	const float posY = static_cast<float>(lua_tonumber(L, 3));
	const std::string text = static_cast<std::string>(lua_tostring(L, 4));
	const bool active = static_cast<bool>(lua_toboolean(L, 5));

	options->createButton(posX, posY, text, active);

	return 0;
}

int ss::OptionsMenu::lua_FadeButtonRect(lua_State* L)
{
	if (lua_gettop(L) != 4)
		return -1;

	ss::OptionsMenu* options = static_cast<ss::OptionsMenu*>(lua_touserdata(L, 1));
	const std::string button = static_cast<std::string>(lua_tostring(L, 2));
	const int toOpacity = static_cast<int>(lua_tonumber(L, 3));
	const int speed = static_cast<int>(lua_tonumber(L, 4));

	options->fadeButtonRect(button, toOpacity, speed);

	return 0;
}

int ss::OptionsMenu::lua_FadeButtonText(lua_State* L)
{
	if (lua_gettop(L) != 4)
		return -1;

	ss::OptionsMenu* options = static_cast<ss::OptionsMenu*>(lua_touserdata(L, 1));
	const std::string button = static_cast<std::string>(lua_tostring(L, 2));
	const int toOpacity = static_cast<int>(lua_tonumber(L, 3));
	const int speed = static_cast<int>(lua_tonumber(L, 4));

	options->fadeButtonText(button, toOpacity, speed);

	return 0;
}