#pragma once

//#include "all_includes.h"

#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//What Every Client File Header should have
#include "InputManager.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Windows.h>

namespace trog
{
	class Sprite;
	class Enemy;

	extern float SCR_WIDTH;
	extern float SCR_HEIGHT;
	extern float MAX_LOOK_ANGLE;

	extern bool is3d;
	extern bool clean;
	extern bool loadSprites;
	extern bool loadMainPlayer;

	extern bool hasRunOnce;
	extern bool canTakeGameInput;
	extern bool isInPlayMode;

	extern const char* glsl_version;

	extern GLFWwindow* window;

	extern std::string vertexSource;
	extern unsigned int vertexShader;

	extern std::string fragmentSource;
	extern unsigned int fragmentShader;

	extern std::string textVertexSource;
	extern unsigned int textVertexShader;

	extern std::string textFragmentSource;
	extern unsigned int textFragmentShader;

	extern std::string skyboxVertexSource;
	extern unsigned int skyboxVertexShader;

	extern std::string skyboxFragmentSource;
	extern unsigned int skyboxFragmentShader;

	extern unsigned int shaderProgram;
	extern unsigned int textProgram;
	extern unsigned int skyboxProgram;

	extern unsigned int ebo;
	extern unsigned int vbo;
	extern unsigned int vao;

	extern unsigned int skyboxVbo;
	extern unsigned int skyboxVao;

	extern unsigned int textVao;
	extern unsigned int textVbo;

	extern unsigned int fbo;
	extern unsigned int fbo_text;
	extern unsigned int rbo;

	extern glm::mat4 textProjection;

	extern std::vector<Sprite *> spriteList;
	extern std::vector<Enemy *> enemyList;

	extern int numberOfUnnamedSprites;

	void getError(const char* loc = "");

	void mainGameLoop();
	void mainLoop();

	void setup();

	void draw();
	void drawSkybox();

	void cleanup();
	void load();
	void loadImages();
};
