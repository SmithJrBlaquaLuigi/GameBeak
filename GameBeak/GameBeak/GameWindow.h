#pragma once

//#ifndef WINDOWH
//#define WINDOWH

#include <SFML\Graphics.hpp>
#include <string>
#include <vector>

#include <Windows.h>

using namespace std;
using namespace sf;

class GameWindow
{
public:
	GameWindow() {}
	GameWindow::GameWindow(RenderWindow* window);
	GameWindow(string title, int width, int height);
	~GameWindow();

	void updateLCD(int clocks, int& lineClocksSinceLastUpdate, int& refreshClocksSinceLastUpdate, int& clocksSinceLastVBlank);
	void setIcon(int width, int height, string path);
	Color getBGPixel(int x, int y);
	void setBGPixel(int x, int y, Color color);
	void setWindowPixel(int x, int y, Color color);
	void setSpritePixel(int x, int y, Color color);
	void setDebugPixel(int x, int y, Color color);
	void drawScreenFromMaps(int scrollX, int ScrollY);
	void drawFullScreenMaps();
	void drawImageToScreen(Image image);
	RenderWindow window;
	Texture texture;
	Color bgPixels[256 * 256];
	Color windowPixels[256 * 256];
	Color spritePixels[256 * 256];
	Color tileDebugPixels[256 * 256];
	int gpuMode = 1; //BGB defaults this to 1, V-Blank. This should be true.
	int getGPUMode();
	Image screen;
	Image debugTileScreen;
	bool poweringOn = true;
};

//#endif