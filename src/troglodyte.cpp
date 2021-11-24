#include "troglodyte.h"
#include "all_includes.h"
#include "external/shaderController.h"

#include "Renderer/progressBar.h"

#include "external/Image.h"
#include "Renderer/camera.h"

#include <algorithm>

#include <fstream>

#include "external/time.h"

#include "stb_Image.h"
#include "Enemy.h"

//ADD STB IMAGE

namespace trog
{
	struct less_than_key
	{
		inline bool operator() (Sprite*& sprite1, Sprite*& sprite2)
		{
			return (sprite1->layer < sprite2->layer);
		}
	};

	float trog::SCR_WIDTH = 1280.0f;
	float trog::SCR_HEIGHT = 720.0f;
	float trog::MAX_LOOK_ANGLE = 89.0f;

	bool trog::is3d = true;
	bool trog::canTakeGameInput = true;
	bool trog::isInPlayMode = false;

	bool trog::clean = false;

	const char* trog::glsl_version = "#version 330";

	GLFWwindow* trog::window = renderer::windowSetup();

	std::string trog::vertexSource = makeShader::readShader("../troglodyte/res/shaders/vertex.vert");
	unsigned int trog::vertexShader = makeShader::shader(vertexSource.c_str(), 0);

	std::string trog::fragmentSource = makeShader::readShader("../troglodyte/res/shaders/fragment.frag");
	unsigned int trog::fragmentShader = makeShader::shader(fragmentSource.c_str(), 1);

	std::string trog::textVertexSource = makeShader::readShader("../troglodyte/res/shaders/textVertex.vert");
	unsigned int trog::textVertexShader = makeShader::shader(textVertexSource.c_str(), 0);

	std::string trog::textFragmentSource = makeShader::readShader("../troglodyte/res/shaders/textFragment.frag");
	unsigned int trog::textFragmentShader = makeShader::shader(textFragmentSource.c_str(), 1);

	std::string trog::skyboxVertexSource = makeShader::readShader("../troglodyte/res/shaders/skyboxVertex.vert");
	unsigned int trog::skyboxVertexShader = makeShader::shader(skyboxVertexSource.c_str(), 0);

	std::string trog::skyboxFragmentSource = makeShader::readShader("../troglodyte/res/shaders/skyboxFragment.frag");
	unsigned int trog::skyboxFragmentShader = makeShader::shader(skyboxFragmentSource.c_str(), 1);

	unsigned int trog::shaderProgram = makeProgram::program(vertexShader, fragmentShader);
	unsigned int trog::textProgram = makeProgram::program(textVertexShader, textFragmentShader);
	unsigned int trog::skyboxProgram = makeProgram::program(skyboxVertexShader, skyboxFragmentShader);

	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};
	
	unsigned int trog::vbo = trog::makeBuffer::vbo();
	unsigned int trog::ebo = trog::makeBuffer::ebo();

	unsigned int skyboxVbo = makeBuffer::vbo();
	unsigned int skyboxVao = makeVertexArray::vao();

	std::vector<std::string> faces
	{
		"../troglodyte/res/Images/skybox/skyboxside.png",
		"../troglodyte/res/Images/skybox/skyboxside.png",
		"../troglodyte/res/Images/skybox/skyboxtop.png",
		"../troglodyte/res/Images/skybox/skyboxbottom.png",
		"../troglodyte/res/Images/skybox/skyboxside.png",
		"../troglodyte/res/Images/skybox/skyboxside.png"
	};
	unsigned int cubemapTexture = makeTexture::s_Texture(faces);

	unsigned int trog::fbo = trog::makeFrameBuffer::frameBuffer();

	unsigned int trog::fbo_text = trog::makeFrameBuffer::texture();

	unsigned int trog::rbo = trog::makeFrameBuffer::rbo();

	unsigned int trog::textVbo = makeBuffer::vbo();

	unsigned int trog::textVao = makeVertexArray::vao();
	unsigned int trog::vao = makeVertexArray::vao();

	std::vector<Sprite*> trog::spriteList;
	std::vector<Enemy*> trog::enemyList;

	int trog::numberOfUnnamedSprites = 0;

	//Sprite spriteMain;

	bool trog::hasRunOnce = false;

	glm::mat4 trog::textProjection = glm::ortho(0.0f, SCR_WIDTH, 0.0f, SCR_HEIGHT);

	void trog::setup()
	{
		trog::getError("setup start");
		//glfwSwapInterval(1);

		std::cout << "trog Startup" << std::endl;

		trog::getError();

		glUseProgram(textProgram);
		glBindVertexArray(textVao);
		glBindBuffer(GL_ARRAY_BUFFER, textVbo);

		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		glEnableVertexAttribArray(0);
		
		glUseProgram(0);
		glUseProgram(skyboxProgram);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(skyboxVao);
		glBindBuffer(GL_ARRAY_BUFFER, skyboxVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		glUseProgram(trog::shaderProgram);

		trog::getError("set callbacks");

		glfwSetKeyCallback(window, inputManager::key_callback);
		glfwSetMouseButtonCallback(window, inputManager::mouse_click_callback);
		glfwSetScrollCallback(window, inputManager::scroll_callback);

		trog::fontRenderer::initText("../troglodyte/res/fonts/Montserrat-SemiBold.ttf");
		trog::GUI::initIMGUI("../troglodyte/res/fonts/Montserrat-SemiBold.ttf");

		trog::loadImages();
		trog::load();

		GLFWimage images[1];
		images[0].pixels = stbi_load("../troglodyte/res/images/icon.png", &images[0].width, &images[0].height, 0, 4); //rgba channels 
		glfwSetWindowIcon(window, 1, images);
		stbi_image_free(images[0].pixels);

		//shaderController::uniform(shaderController::getUniform("textureFrag", shaderProgram), 0);

		getError("setup finished");
		//7trog::Sprite mainPlayer(1);f5
	}

	void trog::drawSkybox()
	{
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		glUseProgram(skyboxProgram);
		glm::mat4 view = glm::mat4(1.0f); // remove translation from the view matrix
		glm::mat4 projection = camera::getProjection();
		view = camera::getView();

		glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "view"), 1, GL_FALSE, glm::value_ptr(glm::mat4(glm::mat3(view))));
		glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		// skybox cube
		getError("set matrices in skybox");

		glBindVertexArray(skyboxVao);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);

		getError();
	}

	void trog::draw()
	{
		//render layer after layer.
		camera::update();

		//drawSkybox(); //This is unoptimized see why this is not rendering after the cube look at learnopengl.com for more info
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		std::sort(spriteList.begin(), spriteList.end(), less_than_key());

		for (auto& sprite : spriteList)
		{
			if (sprite->canRender == false)
				continue;

			sprite->Update();

			sprite->renderSprite();
		}

		for (Enemy*& sprite : enemyList)
		{
			//sprite->Update();

			sprite->renderSprite();
		} // make a list of all the sprites and then render and sort that

		GUI::renderGUI();

		getError();
	}

	void trog::mainGameLoop()
	{
		setup();

		trog::getError();
		//have a function that runs once every timethe layers or the game scene is updated, add them to correct vectors.

		while (!glfwWindowShouldClose(trog::window))
		{
			if (!clean)
			{
				time_::calculateDeltaTime();
				//std::cout << time_::deltaTime << std::endl;

				renderer::GetFPS();
				//glfwSetWindowTitle(window, renderer::GetFPS().c_str());

				//ImGuiFunctions::newFrame();

				glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				//makeFrameBuffer::bind();

				//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				//glEnable(GL_DEPTH_TEST);

				draw();

				//std::string FPS = renderer::GetFPS();
	
				mainLoop();

				//makeFrameBuffer::unbind();

				//glDisable(GL_DEPTH_TEST);
				//GUI::frameBufferWindow();

				//ImGuiFunctions::renderFrame();

				//makeFrameBuffer::status();

				//glfwSetWindowSize(window, 1280, 920);

				glfwSwapBuffers(window);

				glfwWaitEventsTimeout(0.007); 
				//glfwPollEvents();

				if (hasRunOnce == false)
					hasRunOnce = true;
			}
		}

		cleanup();
	}

	void trog::getError(const char* loc)
	{
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR) {
			std::cerr << "OpenGL error: " << err << ", at: " << loc << std::endl;
		}
	}

	void trog::cleanup()
	{
		glDisable(GL_CULL_FACE);
		glDisable(GL_BLEND);

		std::cout << "trog Cleanup Start" << std::endl;
		clean = true;

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		std::ofstream save_file("../troglodyte/res/files_for_saving/sprites.dat");
		if (save_file.is_open())
		{
			save_file << "" << std::endl;

			//The way the file is structured:
			/*
			* name
			* layer
			* what the type of the sprite is, so when you instantiate a new sprite on startup, you get the type. If it is equal to for example 'testSprite' than create a testSprite
			* and when a user creates a new sprite, create a new file with the type of the sprite. When the sprite is loaded in check if its name matches any of the class names,
			* make a class with that type so the user can control it and create a script for it.
			* is clicked on
			* color.x
			* color.y
			* color.z
			* rotate.x
			* rotate.y
			* rotate.z
			* position.x
			* position.y
			* position.z
			* size.x
			* size.y
			* size.z
			* shape
			* */

			for (auto& sprite : spriteList)
			{
				if (!sprite->writeToFile)
				{
					continue;
				}

				save_file << sprite->name << std::endl;
				save_file << sprite->layer << std::endl;
				save_file << typeid(sprite).name() << std::endl;
				save_file << sprite->color[0] << std::endl;
				save_file << sprite->color[1] << std::endl;
				save_file << sprite->color[2] << std::endl;
				save_file << sprite->rotate.x << std::endl;
				save_file << sprite->rotate.y << std::endl;
				save_file << sprite->rotate.z << std::endl;
				save_file << sprite->position.x << std::endl;
				save_file << sprite->position.y << std::endl;
				save_file << sprite->position.z << std::endl;
				save_file << sprite->size.x << std::endl;
				save_file << sprite->size.y << std::endl;
				save_file << sprite->size.z << std::endl;
				save_file << sprite->shape << std::endl;

				delete sprite;
			}

			save_file.close();
		}
		else
		{
			std::cout << "error opening sprites file, does not exist" << std::endl;
		}

		std::ofstream camera_file("../troglodyte/res/files_for_saving/camera.dat");
		if (camera_file.is_open())
		{
			camera_file << "" << std::endl;

			camera_file << camera::cameraPos.x << std::endl;
			camera_file << camera::cameraPos.y << std::endl;
			camera_file << camera::cameraPos.z << std::endl;

			camera_file << camera::cameraFront.x << std::endl;
			camera_file << camera::cameraFront.y << std::endl;
			camera_file << camera::cameraFront.z << std::endl;

			camera_file << camera::cameraUp.x << std::endl;
			camera_file << camera::cameraUp.y << std::endl;
			camera_file << camera::cameraUp.z << std::endl;

			camera_file << camera::fov << std::endl;
			camera_file << camera::yaw << std::endl;
			camera_file << camera::pitch << std::endl;

			camera_file.close();
		}
		else
		{
			std::cout << "error opening camera file, does not exist" << std::endl;
		}
		/*
			glm::vec3 camera::cameraPos.x;
			glm::vec3 camera::cameraPos.y;
			glm::vec3 camera::cameraPos.z;
			glm::vec3 camera::cameraFront.x;
			glm::vec3 camera::cameraFront.y;
			glm::vec3 camera::cameraFront.z;
			glm::vec3 camera::cameraUp.x;
			glm::vec3 camera::cameraUp.y;
			glm::vec3 camera::cameraUp.z;
			glm::vec3 camera::cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));

			float camera::fov = 90.0f;

			float camera::yaw = -90.0f;
			float camera::pitch = 0.0f;
		*/

		std::cout << "trog Cleanup End" << std::endl;

		glfwTerminate();
	}

	void trog::load()
	{
		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		int num_of_sprite_attribs = 16;

		std::ifstream save_file("../troglodyte/res/files_for_saving/sprites.dat", std::ios::in);
		if (save_file.is_open())
		{
			std::string temp;

			std::string line;

			std::vector <std::string> vec;

			if (std::getline(save_file, temp))
			{
				vec.push_back(temp);
				while (std::getline(save_file, line))
				{
					if (line.empty())
						continue;

					//std::cout << line << std::endl;

					vec.push_back(line);
				}
			}

			//The way the file is structured:
			/*
			* name
			* layer
			* what the type of the sprite is, so when you instantiate a new sprite on startup, you get the type. If it is equal to for example 'testSprite' than create a testSprite
			* and when a user creates a new sprite, create a new file with the type of the sprite. When the sprite is loaded in check if its name matches any of the class names,
			* make a class with that type so the user can control it and create a script for it.
			* color.x
			* color.y
			* color.z
			* rotate
			* position.x
			* position.y
			* position.z
			* size.x
			* size.y
			* size.z
			* shape
			* */

			for (int i = 1; i < vec.size(); i = i + num_of_sprite_attribs)
			{
				Sprite* tempSprite = new Sprite(1, "player");

				tempSprite->name = vec.at(i);
				tempSprite->layer = std::atoi(vec.at(i + 1).c_str());
				//sprite script thing
				tempSprite->color[0] = std::atof(vec.at(i + 3).c_str());
				tempSprite->color[1] = std::atof(vec.at(i + 4).c_str());
				tempSprite->color[2] = std::atof(vec.at(i + 5).c_str());

				tempSprite->rotate.x = std::atof(vec.at(i + 6).c_str());
				tempSprite->rotate.y = std::atof(vec.at(i + 7).c_str());
				tempSprite->rotate.z = std::atof(vec.at(i + 8).c_str());

				tempSprite->position.x = std::atof(vec.at(i + 9).c_str());
				tempSprite->position.y = std::atof(vec.at(i + 10).c_str());
				tempSprite->position.z = std::atof(vec.at(i + 11).c_str());

				tempSprite->size.x = std::atof(vec.at(i + 12).c_str());
				tempSprite->size.y = std::atof(vec.at(i + 13).c_str());
				tempSprite->size.z = std::atof(vec.at(i + 14).c_str());

				tempSprite->shape = std::atoi(vec.at(i + 15).c_str());
				tempSprite->update();
				//std::cout << tempSprite->shape << std::endl;
			}

			save_file.close();
		}
		else
		{
			std::cout << "error opening sprites file, does not exist" << std::endl;
		}

		std::ifstream camera_file("../troglodyte/res/files_for_saving/camera.dat", std::ios::in);
		if (camera_file.is_open())
		{
			std::string temp3;

			std::string line3;

			std::vector<std::string> vec3;

			if (std::getline(camera_file, temp3))
			{
				vec3.push_back(temp3);
				while (std::getline(camera_file, line3))
				{
					vec3.push_back(line3);
				}
			}

			camera::cameraPos.x = atof(vec3.at(1).c_str());
			camera::cameraPos.y = atof(vec3.at(2).c_str());
			camera::cameraPos.z = atof(vec3.at(3).c_str());

			camera::cameraFront.x = atof(vec3.at(4).c_str());
			camera::cameraFront.y = atof(vec3.at(5).c_str());
			camera::cameraFront.z = atof(vec3.at(6).c_str());

			camera::cameraUp.x = atof(vec3.at(7).c_str());
			camera::cameraUp.y = atof(vec3.at(8).c_str());
			camera::cameraUp.z = atof(vec3.at(9).c_str());

			camera::cameraRight = glm::normalize(glm::cross(camera::cameraFront, camera::cameraUp));

			camera::fov = atof(vec3.at(10).c_str());
			camera::yaw = atof(vec3.at(11).c_str());
			camera::pitch = atof(vec3.at(12).c_str());

			camera_file.close();
		}
		else
		{
			std::cout << "error opening camera file, does not exist" << std::endl;
		}
		getError("load finished");
	}

	void trog::loadImages()
	{
		Image::setImageValues();
		getError("set Images");
	}
}

int main()
{
	trog::mainGameLoop();
}