#include "pch.h"
#include "Animator.h"

ss::Animator::Animator()
	: fadeDirection(0), finishedMoving(false), finishedFading(false)
{
}

ss::Animator::~Animator()
{
}

const int ss::Animator::FadeInOut(int opacity, const int lowerOpacity, const int higherOpacity)
{
	if (opacity == higherOpacity)
		fadeDirection = -1;
	else if (opacity == lowerOpacity)
		fadeDirection = 2;

	opacity += fadeDirection;
	opacity = std::clamp(opacity, lowerOpacity, higherOpacity);

	return opacity;
}

const sf::Uint8 ss::Animator::FadeIn(int from, const int to, const int speed)
{
	if (from != to)
		finishedFading = false;

	if (!finishedFading)
	{
		from += speed;

		if (from > to)
			from = to;
	}

	if (from == to)
		finishedFading = true;

	return from;
}

const sf::Uint8 ss::Animator::FadeOut(int from, const int to, const int speed)
{
	return static_cast<sf::Uint8>(0);
}

void ss::Animator::MoveInOut()
{
}

const float ss::Animator::MoveIn(float from, const float to, const float speed)
{
	from += speed;

	if (from > to)
		from = to;

	return from;
}

const float ss::Animator::MoveOut(float from, const float to, const float speed)
{
	from -= speed;

	if (from < to)
		from = to;

	return from;
}