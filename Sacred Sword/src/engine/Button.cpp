#include "pch.h"
#include "Button.h"

////////////////////////////// CONSTRUCTOR //////////////////////////////

ss::Button::Button(const float x, const float y, const std::string text, const sf::Font& font, const bool isActive)
	: buttonState(B_IDLE),
	x(x), y(y), buttonPadding(8.f),
	lowerOpacity(190), higherOpacity(255),
	opacity(lowerOpacity), isActive(isActive),
	buttonFont(font)
{
	bIdle = sf::Color(35, 35, 35, 0);
	bHovered = sf::Color(65, 65, 65, 0);
	bDeactivated = sf::Color(25, 25, 25, 0);

	bTIdle = sf::Color(240, 240, 240, 0);
	bTPressed = sf::Color(95, 95, 95, 0);
	bTDeactivated = sf::Color(45, 45, 45, 0);

	const sf::Vector2f area(230.f, 45.f);
	const sf::Vector2f pos(x, y);

	buttonRect.setSize(area);
	buttonRect.setPosition(pos);

	if (isActive)
		buttonRect.setFillColor(bIdle);
	else
		buttonRect.setFillColor(bDeactivated);

	buttonText.setFont(buttonFont);
	buttonText.setCharacterSize(30);
	buttonText.setString(text);

	if (isActive)
		buttonText.setFillColor(bTIdle);
	else
		buttonText.setFillColor(bTDeactivated);

	setTextPosition();
}



////////////////////////////// DESTRUCTOR //////////////////////////////

ss::Button::~Button()
{
	std::cout << "~BUTTON::Deleted a button" << std::endl;
}



////////////////////////////// METHODS //////////////////////////////

const bool ss::Button::isPressed() const
{
	if (buttonState == B_PRESSED)
		return true;

	return false;
}

void ss::Button::setX(const float x)
{
	buttonRect.setPosition(sf::Vector2f(x, buttonRect.getPosition().y));
	setTextPosition();
}

void ss::Button::setY(const float y)
{
	buttonRect.setPosition(sf::Vector2f(buttonRect.getPosition().x, y));
	setTextPosition();
}

void ss::Button::setTextPosition()
{
	const float lengthX = buttonText.getGlobalBounds().width / 2.f;
	const float posX = buttonRect.getPosition().x + ((buttonRect.getSize().x / 2.f) - lengthX);

	const float heightY = (buttonText.getGlobalBounds().height / 2.f) + buttonPadding;
	const float posY = buttonRect.getPosition().y + ((buttonRect.getSize().y / 2.f) - heightY);

	buttonText.setPosition(sf::Vector2f(posX, posY));
}

void ss::Button::setButtonOpacity(const int set)
{
	bIdle.a = set;
	bHovered.a = set;
}

void ss::Button::fade()
{
	setButtonOpacity(opacity);

	opacity = ani.FadeInOut(opacity, lowerOpacity, higherOpacity);

	buttonRect.setFillColor(sf::Color(buttonRect.getFillColor().r, buttonRect.getFillColor().g, buttonRect.getFillColor().b, opacity));
}

void ss::Button::fixOpacities()
{
	if (bIdle.a != opacity)
		bIdle.a = opacity;
	if (bHovered.a != opacity)
		bHovered.a = opacity;
	if (bDeactivated.a != opacity)
		bDeactivated.a = opacity;

	if (bTIdle.a != 255)
		bTIdle.a = 255;
	if (bTPressed.a != 255)
		bTPressed.a = 255;
	if (bTDeactivated.a != 255)
		bTDeactivated.a = 255;
}



////////////////////////////// UPDATE METHOD //////////////////////////////

void ss::Button::Update(const sf::Vector2f& mousePosView)
{
	if (ani.getFinishedFading())
	{
		fixOpacities();

		if (isActive)
		{
			if (buttonRect.getGlobalBounds().contains(mousePosView))
			{
				buttonState = B_HOVERED;

				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
					buttonState = B_PRESSED;
			}
			else
				buttonState = B_IDLE;

			switch (buttonState)
			{
			case B_IDLE:
				setButtonOpacity(lowerOpacity);
				buttonRect.setFillColor(bIdle);
				buttonText.setFillColor(bTIdle);
				break;
			case B_HOVERED:
				fade();
				buttonRect.setFillColor(bHovered);
				buttonText.setFillColor(bTIdle);
				break;
			case B_PRESSED:
				fade();
				buttonRect.setFillColor(bHovered);
				buttonText.setFillColor(bTPressed);
				break;
			}
		}
	}
}



////////////////////////////// RENDER METHOD //////////////////////////////

void ss::Button::Render(sf::RenderTarget* const& target)
{
	target->draw(buttonRect);
	target->draw(buttonText);
}