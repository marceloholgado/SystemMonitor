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
	std::wstring cpuName;
	unsigned int cores;
	unsigned int logicalProcessors;
	unsigned int utilization;
	unsigned int process;
	unsigned int threads;
	float baseSpeed;
	float currentSpeed;

public:
	MonCPU();
	~MonCPU();
	void UpdateStats();
};

