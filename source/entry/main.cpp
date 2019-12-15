/*
	Welcome to the application.
*/

#include <vld.h>
#include "entry/sfml_render.h"
#include "entry/imgui_funcs.h"

int main(int argc, char* argv[])
{
	InitRenderer();
	InitImgui();

	bool bRun = true;
	while (bRun)
	{
		bRun = RunFrame();
	}

	ExitImgui();
	ExitRenderer();

	return 0;
}
