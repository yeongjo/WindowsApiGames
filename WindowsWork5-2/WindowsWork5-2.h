#pragma once

#include "resource.h"

#include <windowsx.h>
#include <sstream>
#include <vector>
#include <time.h>
#include <math.h>

using namespace std;

template<typename T>
T ads(T a) {
	return a >= 0 ? a : -a;
}

void renderCircle(HDC hdc, int _x, int _y, int _sizeX = 10, int _sizeY = 10, COLORREF color = RGB(255, 255, 0)) {
	// 블럭사이즈대로 적용하기
	int x = _x;
	int y = _y;
	//int width = _width*.5f;

	HBRUSH hBrush = (HBRUSH)CreateSolidBrush(color);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	Ellipse(hdc, x, y, x + _sizeX, y + _sizeY);
	SelectObject(hdc, oldBrush);
	DeleteObject(hBrush);
}

void renderRect(HDC hdc, int _x, int _y, int _sizeX = 10, int _sizeY = 10, COLORREF color = RGB(255, 255, 0)) {
	int x = _x, y = _y;
	HBRUSH hBrush = (HBRUSH)CreateSolidBrush(color);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	Rectangle(hdc, x, y, x + _sizeX, y + _sizeY);
	SelectObject(hdc, oldBrush);
	DeleteObject(hBrush);
}

bool IsPointInCircle(int x, int y, int r, int px, int py) {
	int dx = x - px;
	int dy = y - py;
	int l = dx * dx + dy * dy;
	if (l > r*r)
		return false;
	return true;
}

bool collPointRect(int x, int y, RECT* rt) {
	if ((rt->left <= x && x <= rt->right) &&
		(rt->top <= y && y <= rt->bottom))
		return true;
	return false;
}


struct Pos {
public:
	int x = 0, y = 0;

	Pos() {}
	Pos(int x, int y) :x(x), y(y) {}

	void set(int _x, int _y) { x = _x; y = _y; }

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
	Pos& operator/= (const Pos& a) {
		x /= a.x;
		y /= a.y;
		return *this;
	}
	Pos& operator/= (const int a) {
		x /= a;
		y /= a;
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
	Pos& operator-= (const Pos& a) {
		x -= a.x;
		y -= a.y;
		return *this;
	}
	bool operator==(const Pos& a) {
		if (x == a.x && y == a.y)
			return true;
		return false;
	}

	bool operator!=(const Pos& a) {
		if (x != a.x || y != a.y)
			return true;
		return false;
	}

	bool operator<(const int a) {
		if (x < a && y < a)
			return true;
		return false;
	}
	bool operator<=(const int a) {
		if (x <= a && y <= a)
			return true;
		return false;
	}
	bool operator>(const int a) {
		if (x > a && y > a)
			return true;
		return false;
	}
	bool operator>=(const int a) {
		if (x >= a && y >= a)
			return true;
		return false;
	}

	bool isZero() {
		return x == 0 && y == 0;
	}

	Pos ads() {
		return Pos(::ads(x), ::ads(y));
	}
};

// 원기준으로 사각형방향 반환
Pos CollCircleRect(int x, int y, int r, RECT* rt) {
	if ((rt->left <= x && x <= rt->right) ||
		(rt->top <= y && y <= rt->bottom)) {
		RECT rcEx = {
			rt->left - r,
			rt->top - r,
			rt->right + r,
			rt->bottom + r
		};

		if (rcEx.left < x && x < rcEx.right && rcEx.top < y && y < rcEx.bottom) {
			if (rt->left > x) return Pos(1, 0);
			if (x > rt->right) return Pos(-1, 0);
			if (rt->top > y) return Pos(0, 1);
			if (y > rt->bottom) return Pos(0, -1);
		}
	}
	else {
		if (IsPointInCircle(x, y, r, rt->left, rt->top))return Pos(1, 1);
		if (IsPointInCircle(x, y, r, rt->left, rt->bottom))return Pos(1, -1);
		if (IsPointInCircle(x, y, r, rt->right, rt->top))return Pos(-1, 1);
		if (IsPointInCircle(x, y, r, rt->right, rt->bottom))return Pos(-1, -1);
	}
	return Pos(0, 0);
}

int squareLength(Pos a) {
	return a.x*a.x + a.y*a.y;
}

float length(Pos a) {
	return sqrtf(squareLength(a));
}

//template<typename T>
int normalize(int a) {
	return a / ads<int>(a);
}



template<>
Pos ads(Pos a) {
	int x = a.x >= 0 ? a.x : -a.x;
	int y = a.y >= 0 ? a.y : -a.y;
	return Pos(x, y);
}