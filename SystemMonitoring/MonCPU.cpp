#include "stdafx.h"
#include "MonCPU.h"
#include <iostream>
#include <sstream>
#include "Utils.h"

using namespace Utils;

MonCPU::MonCPU()
{
	CpuInfo cpu;
	if (cpu.Initialize()) {
		this->cpu = cpu;
	}
	else {
		std::cout << "\nFail to initialize CPU Info\n";
	}
}


MonCPU::~MonCPU()
{
}

void MonCPU::UpdateStats()
{
	PDWORD len;
	while (!stopThreads)
	{
		this->cpu.UpdateDynamicInfo();
		std::stringstream ss;
		ss  << "CPU: " << WStringToString(this->cpu.GetName())
			<< "\nCores: " << this->cpu.GetPhysicalCores()
			<< "\nLogical Processors: " << this->cpu.GetLogicalProcessors()
			<< "\nBase Speed: " << this->cpu.GetBaseSpeedMHz() << " MHz"
			<< "\nCurrent Speed: " << this->cpu.GetCurrentSpeedMHz() << " MHz"
			<< "\nThreads: " << this->cpu.GetThreadCount()
			<< "\nUtilization: " << this->cpu.GetUtilization() << "%";
		Console::WriteAtPoint(ss.str(), Point(0, 3));
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}
