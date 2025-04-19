#pragma once
#include <windows.h>
#include <atomic>
#include <iostream>
#include <chrono>
#include <thread>
#include "Utils.h"

extern std::atomic<bool> stopThreads;
#define DIV 1024

class MonMemory
{
private:
	MEMORYSTATUSEX statex;
	unsigned int size;
	float memoryUsage;
	unsigned int speed;
public:
	MonMemory();
	~MonMemory();
	void UpdateStats();
};

