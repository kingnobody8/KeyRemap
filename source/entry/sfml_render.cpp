#include "entry/sfml_render.h"
#include "entry/imgui_funcs.h"
#include "sfml/Graphics.hpp"

sf::RenderWindow* pRenWin = nullptr;

sf::RenderWindow* GetRenWin() { return pRenWin; }

void DoRender()
{
	sf::Color bgclr("338888FF");
	pRenWin->clear(bgclr);

	DrawImgui(pRenWin);

	pRenWin->display();
}

void InitRenderer()
{
	int style = sf::Style::Default;
	sf::Vector2i res(1280, 720);
	sf::ContextSettings settings;
	pRenWin = new sf::RenderWindow(sf::VideoMode(res.x, res.y), "Key Remap", style, settings);
	pRenWin->setActive();
	pRenWin->setVerticalSyncEnabled(true);
}

bool RunFrame()
{
	sf::Event tEvent;
	while (pRenWin->pollEvent(tEvent))
	{
		if (tEvent.type == sf::Event::EventType::Closed)
		{
			return false;
		}
		InputImgui();
		DoIOImgui();
		DoRender();
	}

	return true;
}

void ExitRenderer()
{
	delete pRenWin;
	pRenWin = nullptr;
}


