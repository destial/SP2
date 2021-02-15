#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"
#include <set>
#include <string>

struct Mouse {
	bool up, down, left, right, leftclick, rightclick;
	float x, y, scroll;
	Mouse() {
		reset();
	}
	void reset() {
		x = y = up = down = left = right = leftclick = rightclick = scroll = 0;
	}
	~Mouse() {}
};

class Application
{
public:
	Application();
	~Application();
	void Init();
	void Run();
	void Exit();
	enum SCENES
	{
		SCENEHOUSEINSIDE,
		SCENEOUTSIDE,
	};
	static bool IsKeyPressed(unsigned short key);
	static bool IsMousePressed(unsigned short key);
	static bool IsMousePressedOnce(unsigned short key);
	static bool IsKeyPressedOnce(unsigned short key);
	static void GetCursorPos(double* xpos, double* ypos);
	static void log(std::string string);
	static unsigned GetWindowWidth();
	static unsigned GetWindowHeight();
	static unsigned GetUIWidth();
	static unsigned GetUIHeight();
	static unsigned m_width;
	static unsigned m_height;
	static unsigned ui_width;
	static unsigned ui_height;
	static unsigned sceneswitch;
	static std::set<unsigned short> activeKeys;

private:
	//Declare a window object
	StopWatch m_timer;
};

#endif

