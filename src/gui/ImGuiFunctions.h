#pragma once
#include "../troglodyte.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace trog
{
	class ImGuiFunctions
	{
	public:
		static void newFrame();
		static void renderFrame();
	};
}