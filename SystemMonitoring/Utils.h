#pragma once
#include <windows.h>
#include <string>
#include <mutex>
#include <iostream>


/**
* Place all utilities classes here.
*/
namespace Utils
{
	std::string WStringToString(const std::wstring& wstr);
	/**
	* Class represents a point 2D.
	*/
	class Point{
	public:
		int x; /**< X coordinate of the point. */
		int y; /**< X coordinate of the point. */
		/**
		* Default constructor creates a Point (0, 0).
		*/
		Point();
		/**
		* Constructor creates a point in a specific coordinate.
		*
		* @param x A coordinate X of the point.
		* @param y A coordinate Y of the point.
		*/
		Point(int x, int y);
	};
	class Console{
		public:
			static void WriteAtPoint(const std::string& str, Point p);
		private:
			static void GoToXY(const int& x, const int& y);
			static std::mutex mutex_;
	};
};

