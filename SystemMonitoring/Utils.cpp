#include "stdafx.h"
#include "Utils.h"


namespace Utils{


	std::mutex Console::mutex_;

	std::string WStringToString(const std::wstring& wstr) {
		if (wstr.empty()) return {};

		int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, &wstr[0],
			(int)wstr.size(), NULL, 0, NULL, NULL);

		std::string result(sizeNeeded, 0);
		WideCharToMultiByte(CP_UTF8, 0, &wstr[0],
			(int)wstr.size(), &result[0], sizeNeeded, NULL, NULL);

		return result;
	}

	Point::Point(){
		x = 0;
		y = 0;
	}
	Point::Point(int x, int y){
		this->x = x;
		this->y = y;
	}

	void Console::WriteAtPoint(const std::string& str, Point p)
	{
		std::lock_guard<std::mutex> lock(mutex_);
		std::cout << std::string(80, ' ');
		GoToXY(p.x, p.y);
		std::cout << str;
	}

	void Console::GoToXY(const int& x, const int& y){
		COORD coord;
		coord.X = x;
		coord.Y = y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	}
};