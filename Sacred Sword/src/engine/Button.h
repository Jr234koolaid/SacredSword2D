#ifndef BUTTON_H
#define BUTTON_H

#include "engine/Animator.h"

class ss::Animator;

namespace ss
{
	class Button
	{
	private:
		enum button_states { B_IDLE = 0, B_HOVERED, B_PRESSED };

		short unsigned buttonState;
		const float x, y, buttonPadding;
		const int lowerOpacity, higherOpacity;
		int opacity;
		const bool isActive;

		ss::Animator ani;

		sf::Text buttonText;
		const sf::Font& buttonFont;
		sf::RectangleShape buttonRect;
		sf::Color bIdle, bTIdle, bHovered, bTPressed, bDeactivated, bTDeactivated;

		void setTextPosition();
		void setButtonOpacity(const int set);
		void fade();
		void fixOpacities();

	public:
		Button(const float x, const float y, const std::string text, const sf::Font& font, const bool isActive = true);
		~Button();

		const bool isPressed() const;
		inline const float getX() const { return buttonRect.getPosition().x; }
		inline const float getY() const { return buttonRect.getPosition().y; }
		inline const float getWidth() const { return buttonRect.getSize().x; }
		inline const float getHalfWidth() const { return buttonRect.getSize().x / 2.f; }
		inline const float getHeight() const { return buttonRect.getSize().y; }
		inline const float getHalfHeight() const { return buttonRect.getSize().y / 2.f; }
		inline const int getButtonRectOpacity() const { return buttonRect.getFillColor().a; }
		inline const int getButtonTextOpacity() const { return buttonText.getFillColor().a; }
		inline ss::Animator& getAnimator() { return ani; }

		void setX(const float x);
		void setY(const float y);
		inline void setButtonRectOpacity(const sf::Uint8 opacity) { buttonRect.setFillColor(sf::Color(buttonRect.getFillColor().r, buttonRect.getFillColor().g, buttonRect.getFillColor().b, opacity)); }
		inline void setButtonTextOpacity(const sf::Uint8 opacity) { buttonText.setFillColor(sf::Color(buttonText.getFillColor().r, buttonText.getFillColor().r, buttonText.getFillColor().r, opacity)); }

		void Update(const sf::Vector2f& mousePosView);
		void Render(sf::RenderTarget* const& target);
	};
}

#endif