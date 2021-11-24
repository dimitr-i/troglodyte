#include "gui.h"
#include "../InputManager.h"

#include "../external/Image.h"
#include "../external/shaderController.h"
//#include <fstream>

namespace trog
{
	bool GUI::canMakeNewSprite = true;

	static float guiColor[3] = { 0.25f, 0.25f, 0.25f };

	static float float_positionX = SCR_WIDTH / 2.0f;
	static float float_positionY = SCR_HEIGHT / 2.0f;
	static float float_positionZ = 0.0f;

	static char int_positionX[15] = { 0 };
	static char int_positionY[15] = { 0 };
	static char int_positionZ[15] = { 0 };

	static float float_sizeX = 1.0f;
	static float float_sizeY = 1.0f;
	static float float_sizeZ = 1.0f;

	static char int_scaleX[15] = { 0 };
	static char int_scaleY[15] = { 0 };
	static char int_scaleZ[15] = { 0 };

	static float float_rotationX = 0.0f;
	static float float_rotationY = 0.0f;
	static float float_rotationZ = 0.0f;

	static char int_rotationX[15] = { 0 };
	static char int_rotationY[15] = { 0 };
	static char int_rotationZ[15] = { 0 };

	static bool slideModeBoolPos = true;
	static bool slideModeBoolSize = true;
	static bool slideModeBoolRot = true;

	static bool expandModePos = false;
	static bool expandModeSize = false;
	static bool expandModeRot = false;

	std::string clickedOnName;

	int spriteClickedOnCounter = 0;

	std::vector<Sprite*> listOfClickedOnSprites;

	Sprite* GUI::getSpriteName(const char* nameOfSprite)
	{
		for (auto& sprite : spriteList)
		{
			if (sprite->name == nameOfSprite)
			{
				return sprite;
			}
		}

		return NULL;
	}

	void GUI::initIMGUI(const char* font)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);

		ImFont* fonted = io.Fonts->AddFontFromFileTTF(font, 15);
	}

	void GUI::renderInfo()
	{
		glUseProgram(shaderProgram);
		//shaderController::dontShowText(0);

		ImGui::Begin("Sprite Information");

		if (hasRunOnce == false)
		{
			ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
			ImGui::SetWindowSize(ImVec2(300.0f, SCR_HEIGHT));
		}

		if (spriteClickedOnCounter >= 2)
		{
			for (auto& sprite : listOfClickedOnSprites)
			{
				if (sprite->name != clickedOnName)
				{
					sprite->isClickedOn = false;
				}
			}
		}

		spriteClickedOnCounter = 0;

		listOfClickedOnSprites.clear();

		for (auto& sprite : spriteList)
		{
			if (sprite->isClickedOn)
			{
				listOfClickedOnSprites.emplace_back(sprite);

				guiColor[0] = sprite->color[0];
				guiColor[1] = sprite->color[1];
				guiColor[2] = sprite->color[2];

				ImVec2 TextPos = ImGui::GetCursorPos();
				ImVec2 TextSize = ImGui::CalcTextSize(sprite->name.c_str());

				ImVec2 TextWidth = ImVec2(TextPos.x + TextSize.x, TextPos.y + TextSize.y);

				ImVec2 mouseCoords = ImVec2(inputManager::getMouseCoords().x, inputManager::getMouseCoords().y);

				if (mouseCoords.x > TextPos.x && mouseCoords.x < TextWidth.x)
				{
					if (mouseCoords.y > TextPos.y && mouseCoords.y < TextWidth.y)
					{
						if (ImGui::IsMouseDoubleClicked(0))
						{
							sprite->inputTextShowing = true;
						}
					}
				}

				if (sprite->inputTextShowing == true)
				{
					ImGui::PushItemWidth(100.0f);
					if (ImGui::InputText("Sprite Name", sprite->spriteNameToBe, IM_ARRAYSIZE(sprite->spriteNameToBe), ImGuiInputTextFlags_EnterReturnsTrue))
					{
						sprite->name = sprite->spriteNameToBe;

						sprite->inputTextShowing = false;
					}
					ImGui::PopItemWidth();
				}

				ImGui::PushItemWidth(180.0f);

				static const char* listForCombo[]{"Layer 1", "Layer 2", "Layer 3", "Layer 4", "Layer 5", "Layer 6", "Layer 7"};

				if (sprite->inputTextShowing == false)
				{
					ImGui::Text(sprite->name.c_str()); ImGui::SameLine();

					ImGui::Combo("##ggnooneeverusesthisisissoocool", &sprite->layer, listForCombo, IM_ARRAYSIZE(listForCombo));
				}

				ImGui::PopItemWidth();

				ImGui::NewLine();
			
				if (ImGui::CollapsingHeader("Sprite Information"))
				{
					ImGui::Text("Color Edit");

					ImGui::ColorEdit3("Color", (float*)guiColor); ImGui::NewLine();

					sprite->color[0] = guiColor[0];
					sprite->color[1] = guiColor[1];
					sprite->color[2] = guiColor[2];

					ImGui::Text("Transform"); ImGui::SameLine();

					ImGui::Checkbox("Slider Mode##slidePosCheck", &slideModeBoolPos);

					if (!slideModeBoolPos)
					{
						ImGui::SameLine();
						ImGui::Checkbox("Expand##posExpandCheck", &expandModePos);
					}

					if (slideModeBoolPos)
					{
						if (is3d)
						{
							float_positionX = sprite->position.x;
							float_positionY = sprite->position.y;
							float_positionZ = sprite->position.z;

							ImGui::SliderFloat("PosX", &float_positionX, -25.0f, 25.0f); // HOW BIG TO MAKE IT?
							ImGui::SliderFloat("PosY", &float_positionY, -15.0f, 15.0f);
							ImGui::SliderFloat("PosZ", &float_positionZ, 0.0f, 500.0f); ImGui::NewLine();

							sprite->position.x = float_positionX;
							sprite->position.y = float_positionY;
							sprite->position.z = float_positionZ;
						}
						else
						{
							float_positionX = sprite->position.x;
							float_positionY = sprite->position.y;

							ImGui::SliderFloat("PosX", &float_positionX, 0.0f, SCR_WIDTH); // HOW BIG TO MAKE IT?
							ImGui::SliderFloat("PosY", &float_positionY, 0.0f, SCR_HEIGHT); ImGui::NewLine();

							sprite->position.x = float_positionX;
							sprite->position.y = float_positionY;
							sprite->position.z = 0.0f;
						}
					}

					else
					{
						if (expandModePos)
						{
							ImGui::PushItemWidth(100.0f);
						}
						else { ImGui::PushItemWidth(50.0f); }

						strcpy_s(int_positionX, std::to_string(sprite->position.x).c_str());
						strcpy_s(int_positionY, std::to_string(sprite->position.y).c_str());
						strcpy_s(int_positionZ, std::to_string(sprite->position.z).c_str());

						if (ImGui::InputText("##InputIntXPosGUI", int_positionX, IM_ARRAYSIZE(int_positionX), ImGuiInputTextFlags_EnterReturnsTrue))
						{
							sprite->position.x = std::atof(int_positionX);
						}
						ImGui::SameLine();

						if (expandModePos)
						{
							ImGui::Text("Position X");
						}

						if (ImGui::InputText("##InputIntYPosGUI", int_positionY, IM_ARRAYSIZE(int_positionY), ImGuiInputTextFlags_EnterReturnsTrue))
						{
							sprite->position.y = std::atof(int_positionY);
						}
						ImGui::SameLine();

						if (expandModePos)
						{
							ImGui::Text("Position Y");
						}

						if (ImGui::InputText("##InputIntZPosGUI", int_positionZ, IM_ARRAYSIZE(int_positionZ), ImGuiInputTextFlags_EnterReturnsTrue))
						{
							sprite->position.z = std::atof(int_positionZ);
						}
						ImGui::SameLine();

						if (expandModePos)
						{
							ImGui::Text("Position Z");
						}

						if (!expandModePos) { ImGui::Text("Position XYZ"); }

						ImGui::NewLine();

						ImGui::PopItemWidth();

						sprite->position.x = std::atof(int_positionX);
						sprite->position.y = std::atof(int_positionY);
						sprite->position.z = std::atof(int_positionZ);
					}

					ImGui::Text("Scale"); ImGui::SameLine();

					ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + 32.5f, ImGui::GetCursorPos().y + 1.0f));

					ImGui::Checkbox("Slider Mode##sizeSlideCheck", &slideModeBoolSize);

					if (!slideModeBoolSize)
					{
						ImGui::SameLine();
						ImGui::Checkbox("Expand##sizeExpandCheck", &expandModeSize);
					}

					if (slideModeBoolSize)
					{
						if (is3d)
						{
							float_sizeX = sprite->size.x;
							float_sizeY = sprite->size.y;
							float_sizeZ = sprite->size.z;

							ImGui::SliderFloat("SizeX", &float_sizeX, 1.0f, 100.0f);
							ImGui::SliderFloat("SizeY", &float_sizeY, 1.0f, 100.0f);
							ImGui::SliderFloat("SizeZ", &float_sizeZ, 1.0f, 100.0f); ImGui::NewLine();

							sprite->size.x = float_sizeX;
							sprite->size.y = float_sizeY;
							sprite->size.z = float_sizeZ;
						}
						else
						{
							float_sizeX = sprite->size.x;
							float_sizeY = sprite->size.y;

							ImGui::SliderFloat("SizeX", &float_sizeX, 1.0f, 3000.0f);
							ImGui::SliderFloat("SizeY", &float_sizeY, 1.0f, 3000.0f); ImGui::NewLine();

							sprite->size.x = float_sizeX;
							sprite->size.y = float_sizeY;
						}
					}

					else
					{
						if (expandModeSize)
						{
							ImGui::PushItemWidth(100.0f);
						}
						else { ImGui::PushItemWidth(50.0f); }

						strcpy_s(int_scaleX, std::to_string(sprite->size.x).c_str());
						strcpy_s(int_scaleY, std::to_string(sprite->size.y).c_str());
						strcpy_s(int_scaleZ, std::to_string(sprite->size.z).c_str());

						if (ImGui::InputText("##InputIntXScaleGUI", int_scaleX, IM_ARRAYSIZE(int_scaleX), ImGuiInputTextFlags_EnterReturnsTrue))
						{
							sprite->size.x = std::atof(int_scaleX);
						}
						ImGui::SameLine();

						if (expandModeSize)
						{
							ImGui::Text("Size X");
						}

						if (ImGui::InputText("##InputIntYScaleGUI", int_scaleY, IM_ARRAYSIZE(int_scaleY), ImGuiInputTextFlags_EnterReturnsTrue))
						{
							sprite->size.y = std::atof(int_scaleY);
						}
						ImGui::SameLine();

						if (expandModeSize)
						{
							ImGui::Text("Size Y");
						}

						if (ImGui::InputText("##InputIntZScaleGUI", int_scaleZ, IM_ARRAYSIZE(int_scaleZ), ImGuiInputTextFlags_EnterReturnsTrue))
						{
							sprite->size.z = std::atof(int_scaleZ);
						}
						ImGui::SameLine();

						if (expandModeSize)
						{
							ImGui::Text("Size Z");
						}

						if (!expandModeSize) { ImGui::Text("Scale XYZ"); }

						ImGui::NewLine();

						ImGui::PopItemWidth();

						sprite->size.x = std::atof(int_scaleX);
						sprite->size.y = std::atof(int_scaleY);
						sprite->size.z = std::atof(int_scaleZ);
					}

					ImGui::Text("Rotation"); ImGui::SameLine();

					ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + 11.5f, ImGui::GetCursorPos().y + 1.0f));

					ImGui::Checkbox("Slider Mode##rotSlideCheck", &slideModeBoolRot);

					if (!slideModeBoolRot)
					{
						ImGui::SameLine();
						ImGui::Checkbox("Expand##rotExpandCheck", &expandModeRot);
					}

					if (slideModeBoolRot)
					{
						if (is3d)
						{
							float_rotationX = sprite->rotate.x;
							float_rotationY = sprite->rotate.y;
							float_rotationZ = sprite->rotate.z;

							ImGui::SliderFloat("RotX", &float_rotationX, 0.0f, 360.0f);
							ImGui::SliderFloat("RotY", &float_rotationY, 0.0f, 360.0f);
							ImGui::SliderFloat("RotZ", &float_rotationZ, 0.0f, 360.0f);

							sprite->rotate.x = float_rotationX;
							sprite->rotate.y = float_rotationY;
							sprite->rotate.z = float_rotationZ;
						}
						else
						{
							float_rotationZ = sprite->rotate.z;

							ImGui::SliderFloat("RotZ", &float_rotationZ, 0.0f, 360.0f);

							sprite->rotate.z = float_rotationZ;
						}
					}
					else
					{
						if (expandModeRot)
						{
							ImGui::PushItemWidth(100.0f);
						}
						else { ImGui::PushItemWidth(50.0f); }

						strcpy_s(int_rotationX, std::to_string(sprite->rotate.x).c_str());
						strcpy_s(int_rotationY, std::to_string(sprite->rotate.y).c_str());
						strcpy_s(int_rotationZ, std::to_string(sprite->rotate.z).c_str());

						if (ImGui::InputText("##InputIntXRotGUI", int_rotationX, IM_ARRAYSIZE(int_rotationX), ImGuiInputTextFlags_EnterReturnsTrue))
						{
							sprite->rotate.x = std::atof(int_positionX);
						}
						ImGui::SameLine();

						if (expandModeRot)
						{
							ImGui::Text("Rotation X");
						}

						if (ImGui::InputText("##InputIntYRotGUI", int_rotationY, IM_ARRAYSIZE(int_rotationY), ImGuiInputTextFlags_EnterReturnsTrue))
						{
							sprite->rotate.y = std::atof(int_rotationY);
						}
						ImGui::SameLine();

						if (expandModeRot)
						{
							ImGui::Text("Rotation Y");
						}

						if (ImGui::InputText("##InputIntZRotGUI", int_rotationZ, IM_ARRAYSIZE(int_rotationZ), ImGuiInputTextFlags_EnterReturnsTrue))
						{
							sprite->rotate.z = std::atof(int_rotationZ);
						}
						ImGui::SameLine();

						if (expandModeRot)
						{
							ImGui::Text("Rotation Z");
						}

						if (!expandModeRot) { ImGui::Text("Rotation XYZ"); }

						ImGui::NewLine();

						ImGui::PopItemWidth();

						sprite->rotate.x = std::atof(int_rotationX);
						sprite->rotate.y = std::atof(int_rotationY);
						sprite->rotate.z = std::atof(int_rotationZ);
					}

					ImGui::NewLine();
				}

				if (ImGui::CollapsingHeader("Vertice / Indice Information"))
				{
					ImGui::PushItemWidth(60);

					if (ImGui::CollapsingHeader("Presets"))
					{
						if (ImGui::Checkbox("Is Unordered", &sprite->isUnOrdered))
						{
							sprite->shape = 0;
							sprite->update();
						} ImGui::NewLine();
						if (ImGui::Checkbox("Square", &sprite->isSquare))
						{
							sprite->shape = 2;
							sprite->update();
						}
						if (ImGui::Checkbox("Rectangle", &sprite->isRect))
						{
							sprite->shape = 1;
							sprite->update();
						}
						if (ImGui::Checkbox("Triangle", &sprite->isTri))
						{
							sprite->shape = 3;
							sprite->update();
						}
						if (ImGui::Checkbox("Cube", &sprite->isCube))
						{
							sprite->shape = 4;
							sprite->update();
						}
						if (ImGui::Checkbox("Polygon", &sprite->isPoly))
						{
							sprite->shape = 5;
							sprite->update();
						}
					}
					ImGui::NewLine();

					for (int i = 0; i < sprite->vertices.size(); i++)
					{
						std::string verticeName;

						if (sprite->shape == 1 || sprite->shape == 2)
						{
							switch (i)
							{
							case 2:
								verticeName = "Top Right##";
								verticeName += std::to_string(i);
								break;
							case 5:
								verticeName = "Bottom Right##";
								verticeName += std::to_string(i);
								break;
							case 8:
								verticeName = "Bottom Left##";
								verticeName += std::to_string(i);
								break;
							case 11:
								verticeName = "Top Left##";
								verticeName += std::to_string(i);
								break;
							default:
								verticeName = "##";
								verticeName += std::to_string(i);
							}
						}

						else if (sprite->shape == 3)
						{
							switch (i)
							{
							case 2:
								verticeName = "Top Center##";
								verticeName += std::to_string(i);
								break;
							case 5:
								verticeName = "Bottom Right##";
								verticeName += std::to_string(i);
								break;
							case 8:
								verticeName = "Bottom Left##";
								verticeName += std::to_string(i);
								break;
							default: 
								verticeName = "##";
								verticeName += i;
							}
						}

						else
						{
							verticeName = "vertice #";
							verticeName += std::to_string(i);
						}

						std::string placeHolder = std::to_string(sprite->vertices.at(i));

						//if shape == blah blah blah order vertices and indices certain way

						if (ImGui::InputTextWithHint(verticeName.c_str(), placeHolder.c_str(), sprite->verticeToBe.at(i), IM_ARRAYSIZE(sprite->sizeOfVertice), ImGuiInputTextFlags_EnterReturnsTrue))
						{
							sprite->vertices.at(i) = strtof(sprite->verticeToBe.at(i), NULL);
						}

						if (((i + 1) % 3 != 0) && sprite->shape == 1 || sprite->shape == 2 || sprite->shape == 3)
						{
							ImGui::SameLine();
						}

						if ((i + 1) % 3 == 0 && (sprite->shape == 2 || sprite->shape == 3))
						{
							ImGui::NewLine();
						}
					}
					ImGui::NewLine();

					for (int i = 0; i < sprite->indices.size(); i++)
					{
						std::string indiceName;

						indiceName = "indice #";
						indiceName += std::to_string(i);

						std::string placeHolder = std::to_string(sprite->indices.at(i));

						//if shape == blah blah blah order vertices and indices certain way

						if (ImGui::InputTextWithHint(indiceName.c_str(), placeHolder.c_str(), sprite->indiceToBe.at(i), IM_ARRAYSIZE(sprite->sizeOfIndice), ImGuiInputTextFlags_EnterReturnsTrue))
						{
							sprite->indices.at(i) = strtof(sprite->indiceToBe.at(i), NULL);
						}
					}

					ImGui::PopItemWidth();
				}

				spriteClickedOnCounter += 1;
			}
		}

		if (spriteClickedOnCounter == 0)
		{
			ImGui::Text("No Currently Selected Sprite.");
		}

		ImGui::End();

		glUseProgram(0);
	}

	void GUI::createNewSprite(bool makeUnnamed)
	{
		if (makeUnnamed == true)
		{
			Sprite* spriteCreated = new Sprite(1, "wall");

			std::string size = std::to_string(numberOfUnnamedSprites + 1);

			spriteCreated->name = "Unnamed Sprite (";
			spriteCreated->name.append(size);
			spriteCreated->name.append(")");

			numberOfUnnamedSprites += 1;

			spriteCreated->inputTextShowingLayout = false;
		}

		else
		{
			Sprite* spriteCreated = new Sprite(1, "wall");

			spriteCreated->inputTextShowingLayout = true;
		}
	}

	void GUI::renderLayout()
	{
		ImGui::Begin("Game Layout");
		
		if (hasRunOnce == false)
		{
			ImGui::SetWindowPos(ImVec2(SCR_WIDTH - 300.0f, 0.0f));
			ImGui::SetWindowSize(ImVec2(300.0f, SCR_HEIGHT));
		}

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 25.0f));

		if (ImGui::Button("Create New Sprite##createNewSpriteButton"))
		{
			GUI::createNewSprite();
			ImGui::SetKeyboardFocusHere();
		}

		ImGui::PopStyleVar();

		int counter = 0;
		int counterOfSpriteClickedOnTemp = 0;

		for (auto& sprite : spriteList)
		{
			if (sprite->inputTextShowingLayout)
			{
				GUI::canMakeNewSprite = false;
			}
			else
				GUI::canMakeNewSprite = true;

			if (sprite->isClickedOn)
			{
				counterOfSpriteClickedOnTemp += 1;
			}
		}

		for (auto& sprite : spriteList)
		{
			if (sprite->showInEditor)
			{
				ImVec2 TextPos = ImGui::GetCursorScreenPos();
				ImVec2 TextSize = ImGui::CalcTextSize(sprite->name.c_str());

				ImVec2 TextWidth = ImVec2(TextPos.x + TextSize.x, TextPos.y + TextSize.y);

				ImVec2 mouseCoords = ImVec2(inputManager::getMouseCoords().x, inputManager::getMouseCoords().y);

				//std::cout << mouseCoords.x << " : " << mouseCoords.y << std::endl;
				//std::cout << TextWidth.x << " : " << TextWidth.y << std::endl;

				if (mouseCoords.x > TextPos.x && mouseCoords.x < TextWidth.x)
				{
					if (mouseCoords.y > TextPos.y && mouseCoords.y < TextWidth.y)
					{
						if (ImGui::IsMouseDoubleClicked(0))
						{
							sprite->inputTextShowingLayout = true;
						}

						if (ImGui::IsMouseClicked(0))
						{
							if (!sprite->inputTextShowingLayout)
							{
								sprite->isClickedOn = !sprite->isClickedOn;
								if (sprite->isClickedOn)
								{
									clickedOnName = sprite->name;
								}
							}
						}

						if (ImGui::IsMouseClicked(1))
						{
							sprite->canShowDelete = !sprite->canShowDelete;
						}
					}
				}

				std::string newId = "##spriteNameLayout" + std::to_string(counter);
				std::string inputTextId = "Sprite Name" + newId;

				if (sprite->inputTextShowingLayout == true)
				{
					ImGui::PushItemWidth(100.0f);

					if (ImGui::InputText(inputTextId.c_str(), sprite->spriteNameToBeLayout, IM_ARRAYSIZE(sprite->spriteNameToBeLayout), ImGuiInputTextFlags_EnterReturnsTrue))
					{
						if (sprite->spriteNameToBeLayout[0] == '\0')
						{
							std::string size = std::to_string(numberOfUnnamedSprites + 1);

							sprite->name = "Unnamed Sprite (";
							sprite->name.append(size);
							sprite->name.append(")");

							numberOfUnnamedSprites += 1;

							sprite->inputTextShowingLayout = false;
						}

						else
						{
							sprite->name = sprite->spriteNameToBeLayout;

							sprite->inputTextShowingLayout = false;
						}
					}
					ImGui::PopItemWidth();
				}

				//If you click off the input text turn it off

				if (sprite->inputTextShowingLayout == true)
				{
					ImGui::SetKeyboardFocusHere();
				}

				if (sprite->inputTextShowingLayout == false)
				{
					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(50.0f, 10.0f));

					//render Image of sprite thingy
					ImGui::Text(sprite->name.c_str());

					if (sprite->canShowDelete)
					{
						ImGui::SameLine();

						if (ImGui::Button("Delete Sprite"))
						{
							sprite->canShowDelete = false;

							int indexOfSprite = 0;

							for (auto& spriteInFile : spriteList)
							{
								if (sprite->name == spriteInFile->name)
								{
									spriteList.erase(spriteList.begin() + indexOfSprite);

									if (sprite->name.find("Unnamed Sprite") != std::string::npos)
									{
										numberOfUnnamedSprites -= 1;
									}

									break;
								}

								indexOfSprite++;
							}
						}

					}

					ImGui::PopStyleVar();
				}

				counter += 1;
			}
		}

		ImGui::End();
	} 

	void GUI::frameBufferWindow()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);

		ImGui::Begin("Main Game", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar);
		
		glEnable(GL_DEPTH_TEST);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("View"))
			{
				ImGui::MenuItem("is 3d", NULL, &is3d);
				ImGui::EndMenu();
			}

			if (ImGui::ImageButton((ImTextureID)Image::playButtonTexture, ImVec2(17.50f, 17.5f)))
			{
				isInPlayMode = !isInPlayMode;
			}

			ImGui::EndMenuBar();
			//ImGui::EndMenu();
		}

		ImVec2 pos = ImGui::GetCursorScreenPos();
		
		/*if (!hasRunOnce)
		{
			ImGui::SetWindowSize(ImVec2(655, 418));
			ImGui::SetWindowPos(ImVec2(312.5, 8));
		}*/

		//std::cout << isInPlayMode << std::endl;

		if (!isInPlayMode)
		{
			ImGui::SetWindowSize(ImVec2(SCR_WIDTH - 5.0f, SCR_HEIGHT - 7.5f));
			ImGui::SetWindowPos(ImVec2(2.5, 2.5));

			glViewport(0, 0, (SCR_WIDTH) / 2.0f, (SCR_HEIGHT) / 2.0f);
			ImGui::GetWindowDrawList()->AddImage((void*)fbo_text, ImVec2(pos), ImVec2(pos.x + SCR_WIDTH - 5.0f, pos.y + SCR_HEIGHT - 5.0f), ImVec2(0, 1), ImVec2(1, 0));
		}

		if (isInPlayMode)
		{
			ImGui::SetWindowSize(ImVec2(657, 418));
			ImGui::SetWindowPos(ImVec2(310, 8));

			glViewport(0, 0, SCR_WIDTH / 2.0f, SCR_HEIGHT / 2.0f);
			ImGui::GetWindowDrawList()->AddImage((void*)fbo_text, ImVec2(pos), ImVec2(pos.x + SCR_WIDTH / 2.0f, pos.y + SCR_HEIGHT / 2.0f), ImVec2(0, 1), ImVec2(1, 0));
		}

		//ImGui::SetWindowSize(ImVec2(SCR_WIDTH - 5.0f, SCR_HEIGHT - 7.5f));
		//ImGui::SetWindowPos(ImVec2(2.5, 2.5));

		//glViewport(0, 0, (SCR_WIDTH) / 2.0f, (SCR_HEIGHT) / 2.0f);
		//ImGui::GetWindowDrawList()->AddImage((void*)fbo_text, ImVec2(pos), ImVec2(pos.x + SCR_WIDTH - 5.0f, pos.y + SCR_HEIGHT - 5.0f), ImVec2(0, 1), ImVec2(1, 0));

		if (ImGui::IsWindowFocused())
		{
			canTakeGameInput = true;
		}

		else
		{
			canTakeGameInput = false;
		}

		glDisable(GL_DEPTH_TEST);

		ImGui::End();

		ImGui::PopStyleVar();
	}

	void GUI::renderLower()
	{
		
	}

	void GUI::renderGUI()
	{
		if (!isInPlayMode)
		{
			//GUI::renderInfo();
			//GUI::renderLayout();
			//GUI::renderLower();
		}
	}
}
