#pragma once
#include <atomic>
#include <chrono>
#include <string>
#include <windows.h>
#include "CpuInfo.h"

extern std::atomic<bool> stopThreads;

class MonCPU
{
private:
	CpuInfo cpu;

public:
	MonCPU();
	~MonCPU();
	void UpdateStats();
};

