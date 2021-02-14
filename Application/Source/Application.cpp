#include "Application.h"
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>
#include "LoadTGA.h"

#include "Scene.h"
#include "AssignmentScene2.h"
#include "SceneUI.h"

const unsigned char FPS = 120; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame
Mouse mouse;
std::set<unsigned short> Application::activeKeys;
unsigned Application::m_width;
unsigned Application::m_height;
unsigned Application::ui_width;
unsigned Application::ui_height;
int Application::state;
GLFWwindow *m_window;

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

static void mouse_callback(GLFWwindow* window, double x, double y) {
	if (x < Application::m_width / 2) {
		mouse.left = true;
		mouse.right = false;
		mouse.x_diff = (Application::m_width / 2) - x;
	}
	else if (x > Application::m_width / 2) {
		mouse.left = false;
		mouse.right = true;
		mouse.x_diff = x - (Application::m_width / 2);
	}
	if (y < Application::m_height / 2) {
		mouse.up = true;
		mouse.down = false;
		mouse.y_diff = (Application::m_height / 2) - y;
	}
	else if (y > Application::m_height / 2) {
		mouse.up = false;
		mouse.down = true;
		mouse.y_diff = y - (Application::m_height / 2);
	}
}

static void scroll_callback(GLFWwindow* window, double nan, double offSet) {
	mouse.scroll = offSet;
}

void resize_callback(GLFWwindow* window, int w, int h) {
	glViewport(0, 0, w, h);
	Application::m_width = w;
	Application::m_height = h;
	Application::ui_width = 80;
	Application::ui_height = 60;
}

bool Application::IsKeyPressed(unsigned short key) {
    return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

bool Application::IsMousePressed(unsigned short key) {
	if (glfwGetMouseButton(m_window, key) != 0) {
		if (key == 0) {
			mouse.rightclick = false;
			mouse.leftclick = true;
		} else if (key == 1) {
			mouse.leftclick = false;
			mouse.rightclick = true;
		}
		return true;
	}
	return false;
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

enum STATES {
	MENU,
	GAME,
	START,
};

void Application::GetCursorPos(double* xpos, double* ypos) {
	glfwGetCursorPos(m_window, xpos, ypos);
}

unsigned Application::GetWindowHeight() {
	return m_height;
}

unsigned Application::GetWindowWidth() {
	return m_width;
}

unsigned Application::GetUIHeight() {
	return ui_height;
}

unsigned Application::GetUIWidth() {
	return ui_width;
}

void Application::log(std::string string) {
	std::cout << string << std::endl;
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

Application::Application() {}

Application::~Application() {}

void Application::Init() {
	//Set the error callback
	glfwSetErrorCallback(error_callback);

	m_width = 800;
	m_height = 600;
	state = START;

	//Initialize GLFW
	if (!glfwInit()) exit(EXIT_FAILURE);

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 


	//Create a window and create its OpenGL context
	m_window = glfwCreateWindow(m_width, m_height, "COMG", NULL, NULL);
	glfwSetWindowSizeCallback(m_window, resize_callback);
	//glfwSetCursorPosCallback(m_window, mouse_callback);
	//glfwSetScrollCallback(m_window, scroll_callback);
	//glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	//If the window couldn't be created
	if (!m_window) {
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	//glfwSetKeyCallback(m_window, key_callback);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK) fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
}
void toggleState() {
	if (Application::state == MENU || Application::state == START) {
		glfwSetCursorPosCallback(m_window, mouse_callback);
		glfwSetScrollCallback(m_window, scroll_callback);
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		Application::state = GAME;
	} else {
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		mouse.reset();
		glfwSetCursorPosCallback(m_window, NULL);
		glfwSetScrollCallback(m_window, NULL);
		Application::state = MENU;
	}
}

void Application::Run() {
	//Main Loop
	Scene* scene;
	Scene *scene1 = new SceneUI();
	Scene* scene2 = new AssignmentScene2();
	scene1->Init();
	scene2->Init();
	scene = scene1;
	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
	while (!glfwWindowShouldClose(m_window)) {// && !IsKeyPressed(VK_ESCAPE)) {
		scene->Update(m_timer.getElapsedTime(), mouse);
		//scene->Render();

		if (Application::IsMousePressedOnce(0)) {
			if (Application::state == MENU || Application::state == START) {
				double x, y;
				Application::GetCursorPos(&x, &y);
				float posX = (x / m_width) * 80;
				float posY = (y / m_height) * 60;
				//Application::log(("(" + std::to_string(posX) + ", " + std::to_string(posY) + ")"));
				if (posX >= 35 && posX <= 45 && posY <= 22 && posY >= 17) {
					if (scene == scene1) {
						scene = scene2;
						toggleState();
					}
				} else if (posX >= 35 && posX <= 45 && posY <= 52 && posY >= 47) {
					if (scene == scene1) {
						break;
					}
				}
			}
		}

		if (Application::IsKeyPressedOnce(VK_ESCAPE)) {
			if (state == GAME) {
				scene = scene1;
				toggleState();
			} else if (state == MENU) {
				scene = scene2;
				toggleState();
			}
		}
		if (Application::state == GAME) {
			mouse.reset();
			glfwSetCursorPos(m_window, m_width / 2, m_height / 2);
		}
		//Swap buffers
		glfwSwapBuffers(m_window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...
		glfwPollEvents();
        m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.   

	} //Check if the ESC key had been pressed or if the window had been closed
	scene1->Exit();
	scene2->Exit();
	delete scene1;
	delete scene2;
}

void Application::Exit() {
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
}
