#include "pch.h"
#include "Window.h"

////////////////////////////// CONSTRUCTOR //////////////////////////////

ss::Window::Window()
	: deltaTime(0.f), framerateLimit(60), title("Sacred Sword"),
	windowSettings(), ev(), dtClock()
{
	windowResolution.width = 1600; //1280 | 1600 | 1920
	windowResolution.height = 900;  //720 | 900  | 1080

	windowSettings.antialiasingLevel = 0;

	//window = new sf::RenderWindow(windowResolution, title, sf::Style::None, windowSettings);
	window = new sf::RenderWindow(windowResolution, title, sf::Style::Titlebar | sf::Style::Close, windowSettings);
	window->setFramerateLimit(framerateLimit);

	manager = new ss::GameManager(window);
}



////////////////////////////// DESTRUCTOR //////////////////////////////

ss::Window::~Window()
{
	if (manager != nullptr)
		deletePointers(manager);

	deletePointers(window);
	std::cout << "WINDOW::Deleted the Window" << std::endl;
}



////////////////////////////// UPDATE METHOD //////////////////////////////

void ss::Window::Update()
{
	deltaTime = dtClock.restart().asSeconds();

	if (deltaTime > 1.f / static_cast<float>(framerateLimit))
		deltaTime = 1.f / static_cast<float>(framerateLimit);

	while (window->pollEvent(ev))
	{
		if (ev.type == sf::Event::Closed)
			window->close();
	}

	if (manager->getIsRunning())
		manager->Update(deltaTime);
	else
	{
		deletePointers(manager);
		window->close();
	}
}



////////////////////////////// RENDER METHOD //////////////////////////////

void ss::Window::Render()
{
	if (!window->isOpen())
		return;

	window->clear(sf::Color::Black);

	manager->Render(window);

	window->display();
}



////////////////////////////// GAMELOOP //////////////////////////////

void ss::Window::Run()
{
	while (window->isOpen())
	{
		Update();
		Render();
	}
}