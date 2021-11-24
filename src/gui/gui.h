#pragma once

//#include "../Sprite.h"
//#include "../troglodyte.h"
#include "../Sprite.h"
#include "../external/fontRenderer.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

//class Sprite;

namespace trog
{
	//class Sprite;

	class GUI
	{
	public:
		static void initIMGUI(const char* font);
		static void renderInfo();
		static void renderLayout();
		static void renderLower();

		static Sprite* getSpriteName(const char* nameOfSprite);

		static void createNewSprite(bool makeUnnamed = false);

		static void frameBufferWindow();

		static void renderGUI();

		static bool canMakeNewSprite;
	};
}
