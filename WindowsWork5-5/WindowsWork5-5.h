#pragma once

#include "resource.h"

#include <windowsx.h>
#include <sstream>
#include <vector>
#include <time.h>
#include <math.h>
#include <atlimage.h>

using namespace std;

RECT rectView;

void getWindowSize(HWND hwnd) {
	GetClientRect(hwnd, &rectView);
}

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

	Pos<>() {}
	Pos<>(int x, int y) :x(x), y(y) {}

	void set(int _x, int _y) { x = _x; y = _y; }

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
	template<typename T>
	Pos<>& operator+= (const T& a) {
		x += a;
		y += a;
		return *this;
	}
	template<>
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
	Pos<>& operator/= (const Pos<>& a) {
		x /= a.x;
		y /= a.y;
		return *this;
	}
	template<typename T>
	Pos<>& operator/= (T a) {
		x /= a;
		y /= a;
		return *this;
	}
	template<typename T>
	Pos operator/ (T a) {
		Pos p;
		p.x = x / a;
		p.y = y / a;
		return p;
	}
	template<typename T>
	Pos operator* (T a) {
		Pos p;
		p.x = x * a;
		p.y = y * a;
		return p;
	}
	Pos<>& operator-= (const Pos<>& a) {
		x -= a.x;
		y -= a.y;
		return *this;
	}
	bool operator==(const Pos<>& a) {
		if (x == a.x && y == a.y)
			return true;
		return false;
	}

	bool operator!=(const Pos<>& a) {
		if (x != a.x || y != a.y)
			return true;
		return false;
	}
	template<typename T>
	bool operator<(T a) {
		if (x < a && y < a)
			return true;
		return false;
	}
	template<typename T>
	bool operator<=(T a) {
		if (x <= a && y <= a)
			return true;
		return false;
	}
	template<typename T>
	bool operator>(T a) {
		if (x > a && y > a)
			return true;
		return false;
	}
	template<typename T>
	bool operator>=(T a) {
		if (x >= a && y >= a)
			return true;
		return false;
	}

	bool isZero() {
		return x == 0 && y == 0;
	}

	Pos ads() {
		return Pos<>(::ads(x), ::ads(y));
	}

	Pos onlyX() {
		return Pos<>(x, 0);
	}

	Pos onlyY() {
		return Pos<>(0, y);
	}

	Pos flipX() {
		return Pos<>(-x, y);
	}
	Pos flipY() {
		return Pos<>(x, -y);
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

void setAlign(Pos<>& a, Pos<>& b) {
	int x = a.x, y = a.y;
	int x2 = b.x, y2 = b.y;
	b.x = max(x, x2);
	a.x = min(x, x2);
	b.y = max(y, y2);
	a.y = min(y, y2);
}

void initRandom() {
	srand(time(NULL));
}




template <typename T>
T random(T a, T b) {
	return rand() % (b - a) + a;
}
template <typename T>
T random(T a) {
	return rand() % a;
}

template<>
Pos ads(Pos a) {
	int x = a.x >= 0 ? a.x : -a.x;
	int y = a.y >= 0 ? a.y : -a.y;
	return Pos<>(x, y);
}


class DelayC {
	int remainTime;
	int _remainTime = 0;
	bool isLoop;
	bool isBreak = false;
public:
	DelayC(int _remain, bool _isLoop, bool beginStart) {
		setRemainTime(_remain, _isLoop);
		if (beginStart)
			_remainTime = remainTime + 1;
	}

	// return isBreak
	bool update(int add = 1) {
		_remainTime += add;
		return isBreak;
	}

	void setRemainTime(int _remain, bool _isLoop) {
		remainTime = _remain;
		isLoop = _isLoop;
	}

	bool isEnd() {
		if (remainTime < _remainTime) {
			if (isLoop)
				_remainTime = 0;
			else
				isBreak = true;
			return true;
		}
		return false;
	}

	void debugRemainTime(HDC hdc, int x, int y) {
		wstringstream ss;
		ss << _remainTime << _T("/") << remainTime;
		TextOut(hdc, x, y, ss.str().c_str(), ss.str().size());
	}

	void reset() {
		_remainTime = 0;
	}

	void endNext() {
		_remainTime = remainTime + 1;
	}
};

class MTimer {
	vector<DelayC> managingObjs;
	vector<DelayC>::iterator iter;
public:
	// return ID
	int create(int _remainTime, bool _isLoop = false, bool beginStart = true) {
		managingObjs.push_back(DelayC(_remainTime, _isLoop, beginStart));
		return managingObjs.size() - 1;
	}

	// must call this on other Update
	void update(int add = 1) {
		for (iter = managingObjs.begin(); iter != managingObjs.end(); iter++)
		{
			if (iter->update(add))
				managingObjs.erase(iter);
		}
	}

	bool isEnd(int idx) {
		return managingObjs[idx].isEnd();
	}

	void reset(int idx) {
		managingObjs[idx].reset();
	}

	void endNext(int idx) {
		managingObjs[idx].endNext();
	}

	void debug(HDC hdc, int x, int y) {
		int textY = 0;
		for (size_t i = 0; i < managingObjs.size(); i++)
		{
			managingObjs[i].debugRemainTime(hdc, x, y+textY);
			textY += 15;
		}
	}
};