#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"
#include <set>
#include <string>

struct Mouse {
	Mouse() {
		reset();
	}
	void reset() {
		x = 0;
		y = 0;
		scroll = 0;
		left = 0;
		right = 0;
		up = 0;
		down = 0;
		leftclick = 0;
		rightclick = 0;
	}
	bool left, right, up, down, leftclick, rightclick;
	int x, y;
	float scroll;
};
class Application
{
public:
	Application();
	~Application();
	void Init();
	void Run();
	void Exit();
	enum SCENES {
		START_SCENE,
		MENU_SCENE,
		WIN_SCENE,
		OVERWORLD_SCENE,
		DISTRICT_SCENE,
		MART_SCENE,
		MAZE_SCENE,
		BEACH_SCENE,
		SHARK_SCENE,
		CARNIVAL_SCENE,
		TOTALSCENES,
	};

	enum OBJECTIVES {
		TUTORIAL,
		GET_BOOK,
		SURVIVE_SHARK,
		FINISH_MAZE,
		TOTAL_OBJECTIVES,

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
	static unsigned GetFPS();
	static unsigned m_width;
	static unsigned m_height;
	static unsigned ui_width;
	static unsigned ui_height;
	static unsigned sceneswitch;
	static unsigned previousscene;
	static unsigned FPS;
	static bool quit;
	static bool restart;
	static std::set<unsigned short> activeKeys;

private:
	//Declare a window object
	StopWatch m_timer;
};

class Player {
private:
	static unsigned ammo;
	static int money;
	static int health;
	static int sword;
	static int armourplate;
	static int helmet;
	static bool jetpackequipped;
	static bool SharkSurvived;
	static bool MazeComplete;
	static bool BookPurchased;
	static bool ShootingComplete;
public:
	Player() {};
	~Player() {};
	static unsigned getAmmo();
	static int getMoney();
	static int getHealth();
	static int getSword();
	static int getArmourplate();
	static int getHelmet();
	static bool getJetpack();
	static bool getSharkSurvived();
	static bool getMazeComplete();
	static bool getBookPurchased();
	static bool getShootingComplete();
	static void setAmmo(unsigned a);
	static void setMoney(int m);
	static void setHealth(int h);
	static void setSword(int s);
	static void setArmourplate(int ap);
	static void setHelmet(int ht);
	static void setJetpack(bool jp);
	static void setSharkSurvived(bool Ss);
	static void setMazeComplete(bool Mc);
	static void setBookPurchased(bool Bp);
	static void setShootingComplete(bool Sc);
};

#endif

