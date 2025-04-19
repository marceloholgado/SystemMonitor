#include "stdafx.h"
#include "MonMemory.h"
#include "Utils.h"
#include <sstream>

using namespace Utils;


MonMemory::MonMemory()
{
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);
	size = statex.ullTotalPhys;
	memoryUsage = 0;
}

MonMemory::~MonMemory()
{
}

void MonMemory::UpdateStats()
{
	while (!stopThreads)
	{
		memoryUsage = statex.dwMemoryLoad;
		std::stringstream ss;
		ss << "Memory size: " << size / DIV << " MB | Memory usage: " << memoryUsage << "%";
		Console::WriteAtPoint(ss.str(), Point(0, 0));

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	std::cout << std::endl;
}