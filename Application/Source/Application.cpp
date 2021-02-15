#include <iostream>
#include "Application.h"

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>

#include "TemplateScene.h"

GLFWwindow* m_window;
const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame
unsigned Application::sceneswitch;
//Define an error callback
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

unsigned Application::m_width;
unsigned Application::m_height;
unsigned Application::ui_width;
unsigned Application::ui_height;
std::set<unsigned short> Application::activeKeys;
Mouse mouse;

bool Application::IsKeyPressed(unsigned short key)
{
	return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

bool Application::IsMousePressed(unsigned short key) //0 - Left, 1 - Right, 2 - Middle
{
	return glfwGetMouseButton(m_window, key) != 0;
}

void Application::GetCursorPos(double* xpos, double* ypos)
{
	glfwGetCursorPos(m_window, xpos, ypos);
}

unsigned Application::GetWindowWidth()
{
	return m_width;
}

unsigned Application::GetWindowHeight()
{
	return m_height;
}

unsigned Application::GetUIHeight() {
	return ui_height;
}

unsigned Application::GetUIWidth() {
	return ui_width;
}

Application::Application()
{
}

Application::~Application()
{
}

void resize_callback(GLFWwindow* window, int w, int h)
{
	Application::m_width = w;
	Application::m_height = h;
	Application::ui_width = w / 10;
	Application::ui_height = h / 10;
	glViewport(0, 0, w, h);
}

void mouse_callback(GLFWwindow* window, double x, double y) {
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

static void scroll_callback(GLFWwindow* window, double nan, double offSet) {
	mouse.scroll = offSet;
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

void Application::Init()
{
	glfwSetWindowSizeCallback(m_window, resize_callback);

	//Set the error callback
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

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
	m_window = glfwCreateWindow(m_width, m_height, "SP2 - Group 2", NULL, NULL);
	//m_window = glfwCreateWindow(800, 600, "Test Window", NULL, NULL);

	//If the window couldn't be created
	if (!m_window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
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
	if (err != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}
}

void toggleState() {
	switch (Application::sceneswitch) {
	case Application::SCENEOUTSIDE:
		glfwSetCursorPosCallback(m_window, mouse_callback);
		glfwSetScrollCallback(m_window, scroll_callback);
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		mouse.reset();
		glfwSetCursorPos(m_window, Application::m_width / 2, Application::m_height / 2);
		break;
	case Application::SCENEHOUSEINSIDE:
		glfwSetCursorPosCallback(m_window, mouse_callback);
		glfwSetScrollCallback(m_window, scroll_callback);
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		mouse.reset();
		glfwSetCursorPos(m_window, Application::m_width / 2, Application::m_height / 2);
		break;
	default:
		break;
	}
}

void Application::Run()
{
	//Main Loop
	Scene* scene = new TemplateScene();
	scene->Init();
	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame

	while (!glfwWindowShouldClose(m_window) && !IsKeyPressed(VK_ESCAPE))
	{
		scene->Update(m_timer.getElapsedTime(), mouse);
		scene->Render();

		//Swap buffers
		glfwSwapBuffers(m_window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...
		glfwPollEvents();
		m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.   
		toggleState();

		/*if (IsKeyPressed(VK_F1))
			scene = scene1;
		else if (IsKeyPressed(VK_F2))
			scene = scene2;*/
	} //Check if the ESC key had been pressed or if the window had been closed
	scene->Exit();
	delete scene;
}

void Application::Exit()
{
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
}
