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
		this->cpuName = cpu.GetName();
		this->cores = cpu.GetPhysicalCores();
		this->logicalProcessors = cpu.GetLogicalProcessors();
		this->baseSpeed = cpu.GetBaseSpeedMHz();
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
		ss << "CPU: " << WStringToString(this->cpuName)
			<< "\nCores: " << this->cores
			<< "\nLogical Processors: " << this->logicalProcessors
			<< "\nBase Speed: " << this->baseSpeed
			<< "\nCurrent Speed: " << this->currentSpeed
			<< "\nUtilization: " << this->utilization;
		Console::WriteAtPoint(ss.str(), Point(0, 3));
	}
}
