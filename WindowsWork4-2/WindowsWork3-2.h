#pragma once

#include "resource.h"
#include <windowsx.h>
#include <sstream>

using namespace std;

struct Pos {
public:
	int x = 0, y = 0;

	Pos() {}
	Pos(int x, int y) :x(x), y(y) {}

	Pos operator- (const Pos& a) {
		Pos p;
		p.x = x - a.x;
		p.y = y - a.y;
		return p;
	}
	Pos operator- () {
		Pos p;
		p.x = -x;
		p.y = -y;
		return p;
	}
	Pos operator+ (const Pos& a) {
		Pos p;
		p.x = x + a.x;
		p.y = y + a.y;
		return p;
	}
	Pos& operator+= (const Pos& a) {
		x += a.x;
		y += a.y;
		return *this;
	}
	Pos& operator*= (const Pos& a) {
		x *= a.x;
		y *= a.y;
		return *this;
	}
	Pos operator/ (const int a) {
		Pos p;
		p.x = x / a;
		p.y = y / a;
		return p;
	}
	Pos operator* (const int a) {
		Pos p;
		p.x = x * a;
		p.y = y * a;
		return p;
	}
	bool operator==(const Pos& a) {
		if (x == a.x && y == a.y)
			return true;
		return false;
	}
};

void renderCircle(HDC hdc, int _x, int _y, int _width = 10, COLORREF color = RGB(255, 255, 0)) {
	// 블럭사이즈대로 적용하기
	int x = _x;
	int y = _y;
	int width = _width *.5f;

	HBRUSH hBrush = (HBRUSH)CreateSolidBrush(color);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	Ellipse(hdc, x - width, y - width, x + width, y + width);
	SelectObject(hdc, oldBrush);
	DeleteObject(hBrush);
}

int ads(int a) {
	return a >= 0 ? a : -a;
}
int normalize(int a) {
	return a / ads(a);
}