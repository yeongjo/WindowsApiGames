#pragma once

#include "resource.h"
#include <windowsx.h>
#include <sstream>
#include <cmath>
#include <vector>
#include <time.h>

using namespace std;

#define PI (3.141592/180)

struct Pos {
public:
	int x = 0, y = 0;
	
	Pos<>() {}
	Pos<>(int x, int y) :x(x), y(y) {}

	Pos operator- (const Pos<>& a) {
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
	Pos operator+ (const Pos<>& a) {
		Pos p;
		p.x = x + a.x;
		p.y = y + a.y;
		return p;
	}
	Pos<>& operator+= (const Pos<>& a) {
		x += a.x;
		y += a.y;
		return *this;
	}
	Pos<>& operator*= (const Pos<>& a) {
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
	bool operator==(const Pos<>& a) {
		if (x == a.x && y == a.y)
			return true;
		return false;
	}
};

void renderCircle(HDC hdc, int _x, int _y, int _width = 10, COLORREF color = RGB(255, 255, 0)) {
	// 블럭사이즈대로 적용하기
	int x = _x;
	int y = _y;
	int width = _width * .5f;

	HBRUSH hBrush = (HBRUSH)CreateSolidBrush(color);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	Ellipse(hdc, x - width, y - width, x + width, y + width);
	SelectObject(hdc, oldBrush);
	DeleteObject(hBrush);
}

// POINT/RECTANGLE
bool pointRect(float px, float py, float rx, float ry, float rw, float rh) {

	// is the point inside the rectangle's bounds?
	if (px >= rx &&        // right of the left edge AND
		px <= rx + rw &&   // left of the right edge AND
		py >= ry &&        // below the top AND
		py <= ry + rh) {   // above the bottom
		return true;
	}
	return false;
}

bool IsPointInCircle(int x, int y, int r, int px, int py) {
	int dx = x - px;
	int dy = y - py;
	int l = dx * dx + dy * dy;
	if (l > r*r)
		return false;
	return true;
}

bool IsCircleInCircle(int x, int y, int r, int x2, int y2, int r2) {
	return IsPointInCircle(x,y,(r+r2)*.5f,x2,y2);
}

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
			if (rt->left > x) return Pos<>(1, 0);
			if (x > rt->right) return Pos<>(-1, 0);
			if (rt->top > y) return Pos<>(0, 1);
			if (y > rt->bottom) return Pos<>(0, -1);
		}
	}
	else {
		if (IsPointInCircle(x, y, r, rt->left, rt->top))return Pos<>(1, 1);
		if (IsPointInCircle(x, y, r, rt->left, rt->bottom))return Pos<>(1, -1);
		if (IsPointInCircle(x, y, r, rt->right, rt->top))return Pos<>(-1, 1);
		if (IsPointInCircle(x, y, r, rt->right, rt->bottom))return Pos<>(-1, -1);
	}
	return Pos<>(0, 0);
}

int ads(int a) {
	return a >= 0 ? a : -a;
}
int normalize(int a) {
	return a / ads(a);
}

int deltaTime = 16;