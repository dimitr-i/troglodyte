#pragma once
#include <string>

#include "external/fontRenderer.h"
#include "Renderer/Renderer.h"

#include <iostream>

namespace trog
{
	class Sprite
	{
	public:
		void renderSprite();

		virtual void Update();

		Sprite(int LAYER = 1, std::string type = "wall", unsigned int image = 0U);

		bool isClickedOn = false;
		bool hasTextureActive = false;
		unsigned int texture;

		float color[3] = { 0.25f, 0.25f, 0.25f };

		int layer = 1;

		int shape = 2; //0 - No Particular Ordering, 1 - Rectangle, 2 - Square, 3 - Triangle, 4 - Cube, 5 - Polygon, 

		bool inputTextShowing = false;
		bool inputTextShowingLayout = false;

		bool writeToFile = true;
		bool showInEditor = true;

		bool canRender = true;
		bool canShowDelete = false;

		char spriteNameToBe[20] = { 0 };
		char spriteNameToBeLayout[20] = { 0 };

		std::string name = "Unnamed Sprite";

		glm::vec3 rotate = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.5f);
		glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f);

		std::vector<unsigned int> indices;

		std::vector<float> vertices;
		std::vector<float> textures;

		// HOW MANY VERTICES
		std::vector<char*> verticeToBe;
		std::vector<char*> indiceToBe;

		float left;
		float right;
		float top;
		float bottom;

		float width;
		float height;

		char sizeOfVertice[5] = { 0 };
		char sizeOfIndice[5] = { 0 };

		bool isSquare = false;
		bool isRect = false;
		bool isTri = false;
		bool isCube = false;
		bool isPoly = false;
		bool isUnOrdered = false;

		void update();
		static int isColliding(Sprite* &sprite1, Sprite* &sprite2);
	};
}
