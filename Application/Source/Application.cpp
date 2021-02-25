#include <iostream>
#include "Application.h"

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>
#include "StartMenuScene.h"
#include "OverworldScene.h"
#include "SceneShaqeel.h"
#include "SceneShaqler.h"
#include "SceneW.h"
#include "SceneOfTheBeach.h"
#include "SceneRyan.h"
#include "SceneXL.h"
#include "PauseMenuScene.h"
#include "WinnerScene.h"


GLFWwindow* m_window;
unsigned Application::FPS = 120; // FPS of this game
unsigned frameTime = 1000 / Application::FPS; // time for each frame
unsigned Application::m_width;
unsigned Application::m_height;
unsigned Application::ui_width;
unsigned Application::ui_height;
unsigned Application::sceneswitch;
unsigned Application::previousscene;
bool Application::quit;

unsigned Player::getAmmo() {
	return ammo;
}

int Player::getHealth() {
	return health;
}

int Player::getMoney() {
	return money;
}

void Player::setAmmo(unsigned a) {
	ammo = a;
}

void Player::setHealth(int h) {
	health = h;
}

void Player::setMoney(int m) {
	money = m;
}

std::set<unsigned short> Application::activeKeys;
Scene* scene[Application::TOTALSCENES];
Mouse mouse;

Application::Application() {}

Application::~Application() {}

//Define an error callback
static void error_callback(int error, const char* description) {
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

static void scroll_callback(GLFWwindow* window, double nan, double offSet) {
	mouse.scroll = offSet;
}

static void mouse_callback(GLFWwindow* window, double x, double y) {
	if (x < Application::m_width / 2) {
		mouse.left = true;
		mouse.right = false;
		mouse.x = (Application::m_width / 2) - x;
	} else if (x > Application::m_width / 2) {
		mouse.left = false;
		mouse.right = true;
		mouse.x = x - (Application::m_width / 2);
	}
	if (y < Application::m_height / 2) {
		mouse.up = true;
		mouse.down = false;
		mouse.y = (Application::m_height / 2) - y;
	} else if (y > Application::m_height / 2) {
		mouse.up = false;
		mouse.down = true;
		mouse.y = y - (Application::m_height / 2);
	}
}

static void resize_callback(GLFWwindow* window, int w, int h) {
	Application::m_width = w;
	Application::m_height = h;
	Application::ui_width = w / 10;
	Application::ui_height = h / 10;
	glViewport(0, 0, w, h);
}

bool Application::IsKeyPressed(unsigned short key) {
	return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

bool Application::IsMousePressed(unsigned short key) {
	return glfwGetMouseButton(m_window, key) != 0;
}

void Application::GetCursorPos(double* xpos, double* ypos) {
	glfwGetCursorPos(m_window, xpos, ypos);
}

unsigned Application::GetWindowWidth() {
	return m_width;
}

unsigned Application::GetWindowHeight() {
	return m_height;
}

unsigned Application::GetUIHeight() {
	return ui_height;
}

unsigned Application::GetFPS() {
	return FPS;
}

unsigned Application::GetUIWidth() {
	return ui_width;
}

bool Application::IsMousePressedOnce(unsigned short key) {
	std::pair<std::set<unsigned short>::iterator, bool> ret;
	if (glfwGetMouseButton(m_window, key) != 0) {
		ret = activeKeys.insert(key);
		if (!ret.second) {
			return false;
		} else {
			if (key == 0) {
				mouse.rightclick = false;
				mouse.leftclick = true;
			} else if (key == 1) {
				mouse.leftclick = false;
				mouse.rightclick = true;
			}
			return true;
		}
	} else {
		activeKeys.erase(key);
		return false;
	}
}

bool Application::IsKeyPressedOnce(unsigned short key) {
	std::pair<std::set<unsigned short>::iterator, bool> ret;
	if ((GetAsyncKeyState(key) & 0x8001) != 0) {
		ret = activeKeys.insert(key);
		if (!ret.second)
			return false;
		else
			return true;
	} else {
		activeKeys.erase(key);
		return false;
	}
}

void Application::log(std::string string) {
	std::cout << string << std::endl;
}

void Application::Init() {

	//Set the error callback
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
		exit(EXIT_FAILURE);

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version

	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

	//Create a window and create its OpenGL context
	m_width = 800;
	m_height = 600;
	ui_height = 60;
	ui_width = 80;
	sceneswitch = STARTSCENE;
	Player::setMoney(1000);
	Player::setAmmo(256);
	Player::setHealth(100);
	m_window = glfwCreateWindow(m_width, m_height, "SP2 - Group 2", NULL, NULL);
	quit = false;
	mouse.reset();
	glfwSetWindowSizeCallback(m_window, resize_callback);

	//If the window couldn't be created
	if (!m_window) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	glfwSetKeyCallback(m_window, key_callback);

	glewExperimental = true; // Needed for core profile

	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
}

void toggleState() {
	switch (Application::sceneswitch) {
	case Application::MENUSCENE:
	case Application::WINSCENE:
	case Application::STARTSCENE:

		// Use mouse positioning to click on UI menu
		glfwSetCursorPosCallback(m_window, NULL);
		glfwSetScrollCallback(m_window, NULL);
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		break;
	case Application::SCENESHAQ:
	case Application::SCENEWALTON:
	case Application::SCENERYAN:
	case Application::SCENEXL:
	case Application::SCENESHAQLER:
	default:

		// Use mouse movement for playing and looking
		glfwSetCursorPosCallback(m_window, mouse_callback);
		glfwSetScrollCallback(m_window, scroll_callback);
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		mouse.reset();
		glfwSetCursorPos(m_window, Application::GetWindowWidth() / 2, Application::GetWindowHeight() / 2);
		break;
	}
}

void Application::Run() {
	int i = 0;
	// Initialize and create scenes
	scene[SCENESHAQ] = new SceneShaqeel();
	scene[SCENEWALTON] = new SceneW();
	scene[SCENEBEACH] = new SceneOfTheBeach();
	scene[SCENERYAN] = new SceneRyan();
	scene[SCENEXL] = new SceneXL();
	scene[OVERWORLD] = new OverworldScene();
	scene[SCENESHAQLER] = new SceneShaqler();
	scene[STARTSCENE] = new StartMenuScene();
	scene[MENUSCENE] = new PauseMenuScene();
	scene[WINSCENE] = new WinnerScene();

	/*scene[SHAQLER]*/
	for (unsigned i = 0; i < Application::TOTALSCENES; i++) {
		if (scene[i])
			scene[i]->Init();
	}

	// Start timer to calculate how long it takes to render this frame
	m_timer.startTimer();

	// Main Loop
	while (!glfwWindowShouldClose(m_window) && !Application::quit) {
		for (std::set<unsigned short>::iterator i = activeKeys.begin(); i != activeKeys.end(); i++) {
			if (((GetAsyncKeyState(*i) & 0x8001) != 0)) continue;
			else {
				activeKeys.erase(i);
				break;
			}
		}
		// Update and render selected scene
		if (scene[Application::sceneswitch]) {
			int previousScene = Application::sceneswitch;
			scene[previousScene]->Update(m_timer.getElapsedTime(), mouse);
			scene[previousScene]->Render();
			if (previousScene != Application::sceneswitch) {
				scene[Application::sceneswitch]->InitGL();
				continue;
			}

		} else {
			Application::sceneswitch = STARTSCENE;
			continue;
		}

		// Toggle mouse states depending on scene
		toggleState();

		// Switch scenes
		Application::previousscene = Application::sceneswitch;
		switch (Application::sceneswitch) {
		case Application::SCENESHAQ:
			if (Application::IsKeyPressedOnce(VK_F1)) {
				Application::sceneswitch = Application::SCENEWALTON;
			}
			break;
		case Application::SCENEWALTON:
			if (Application::IsKeyPressedOnce(VK_F1)) {
				Application::sceneswitch = Application::SCENEBEACH;
			}
			break;
		case Application::SCENEBEACH:
			if (Application::IsKeyPressedOnce(VK_F1)) {
				Application::sceneswitch = Application::SCENERYAN;
			}
			break;
		case Application::SCENERYAN:
			if (Application::IsKeyPressedOnce(VK_F1)) {
				Application::sceneswitch = Application::SCENEXL;
			}
			break;
		case Application::SCENEXL:
			if (Application::IsKeyPressedOnce(VK_F1)) {
				Application::sceneswitch = Application::MENUSCENE;
			}
			break;
		case Application::STARTSCENE:
			if (Application::IsKeyPressedOnce(VK_F1)) {
				Application::sceneswitch = Application::OVERWORLD;
			}
		case Application::WINSCENE:
			if (Application::IsKeyPressedOnce(VK_F1)) {
				Application::sceneswitch = Application::MENUSCENE;
			}
			break;
		case Application::MENUSCENE:
			if (Application::IsKeyPressedOnce(VK_F1)) {
				Application::sceneswitch = Application::WINSCENE;
			}
			break;
		case Application::OVERWORLD:
			if (Application::IsKeyPressedOnce(VK_F1)) {
				Application::sceneswitch = Application::SCENESHAQ;
			}
			break;
		default:
			if (Application::IsKeyPressedOnce(VK_F1)) {
				Application::sceneswitch = Application::OVERWORLD;
			}
			break;
		}

		//Swap buffers
		glfwSwapBuffers(m_window);

		//Get and organize events, like keyboard and mouse input, window resizing, etc...
		glfwPollEvents();

		// Frame rate limiter. Limits each frame to a specified time in ms.   
		m_timer.waitUntil(frameTime);  

	} 

	//If the window had been closed or quit from the main menu
	for (unsigned i = 0; i < Application::TOTALSCENES; i++) {
		if (scene[i]) {

			//Clean up scenes
			scene[i]->Exit();
			delete scene[i];
		}
	}
}

void Application::Exit() {

	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);

	//Finalize and clean up GLFW
	glfwTerminate();
}
