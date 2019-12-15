#pragma once

//forward declare
namespace sf { class RenderWindow; }

sf::RenderWindow* GetRenWin();

void InitRenderer();

bool RunFrame();

void ExitRenderer();

