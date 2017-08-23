#include <stdio.h>
#include "Main.h"
#include "Image.h"

extern "C"
{

	__declspec(dllexport) void setPauseState(bool pauseEnabled)
	{
		paused = pauseEnabled;
	}
	__declspec(dllexport) bool getPauseState()
	{
		return paused;
	}

	__declspec(dllexport) void setRunState(bool runEnabled)
	{
		run = runEnabled;
	}
	__declspec(dllexport) bool getRunState()
	{
		return run;
	}

	__declspec(dllexport) int* getRegisters()
	{
		static int registers[] = { regAF, regBC, regDE, regHL };
		return registers;
	}

	__declspec(dllexport) int* getScreenPixelData()
	{
		static int* pixelData = beakWindow.screen.getIntArray();
		return beakWindow.screen.getIntArray();
	}

	__declspec(dllexport) int* getScreenDimensions()
	{
		static int dimensions[] = { beakWindow.screen.getSize().x, beakWindow.screen.getSize().y };
	
		return dimensions;
	}

	__declspec(dllexport) void setRom(byte* rom, int romSize)
	{
		beakMemory.loadRom(rom, romSize);
	}

	__declspec(dllexport) void initiateEmulator()
	{
		startEmulator();
	}
}
