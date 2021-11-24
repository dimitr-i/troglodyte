#pragma once

// GLEW and GLFW
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Dear IMGUI
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// GUI Files
#include "gui/gui.h"

#include <vector>

// My Header Files
#include "Make/makeTexture.h"
#include "Make/makeShader.h"
#include "Make/makeShader.h"
#include "Make/makeFrameBuffer.h"
#include "Make/makeBuffer.h"
#include "Make/makeVertexArray.h"
#include "Make/makeProgram.h"

#include "external/fontRenderer.h"
#include "Sprite.h"
#include "InputManager.h"

// Renderer Files
#include "Renderer/renderer.h"

// Developers Image Library
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

#include "gui/ImGuiFunctions.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp> 

// MATH
#include <math.h>

// IOstream (duh)
#include <iostream>