// SystemMonitoring.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <thread>
#include "MonCPU.h"
#include "MonMemory.h"
#include <atomic>
#include <chrono>
#include <condition_variable>

std::atomic<bool> stopThreads(false);
std::mutex mtx;
std::condition_variable cv;

using namespace Utils;

void wait_for_enter() 
{
	std::string input;	
	Console::WriteAtPoint("Hit ENTER to finish all threads...", Point(0, 10));
	std::getline(std::cin, input);
	stopThreads = true;
	cv.notify_all();
}

void HideCursor()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(hConsole, &cursorInfo);
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(hConsole, &cursorInfo);
}


int _tmain(int argc, _TCHAR* argv[])
{
	HideCursor();
	MonCPU monCPU;
	MonMemory monMemory;
	std::thread cpuMonThread(&MonCPU::UpdateStats, monCPU);
	std::thread memoryMonThread(&MonMemory::UpdateStats, monMemory);
	std::thread waitThread(wait_for_enter);
	waitThread.join();

	cpuMonThread.join();
	memoryMonThread.join();
	return 0;
}

