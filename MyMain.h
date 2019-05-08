#pragma once

#include "resource.h"

#include <windowsx.h>
#include <sstream>
#include <vector>
#include <time.h>
#include <math.h>
#include <atlimage.h>
#include <algorithm>

using namespace std;

#define PI 3.1415926535f
#define Radian PI/180

RECT rectView;

void getWindowSize(HWND hwnd) {
	GetClientRect(hwnd, &rectView);
}

template<typename T>
T ads(T a) {
	return a >= 0 ? a : -a;
}




void renderEllipse(HDC hdc, int _x, int _y, int _sizeX, int _sizeY, COLORREF color = RGB(255, 255, 0)) {
	// ���������� �����ϱ�
	int x = _x;
	int y = _y;
	//int width = _width*.5f;

	HBRUSH hBrush = (HBRUSH)CreateSolidBrush(color);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	Ellipse(hdc, x, y, x + _sizeX, y + _sizeY);
	SelectObject(hdc, oldBrush);
	DeleteObject(hBrush);
}

void renderCircle(HDC hdc, int _x, int _y, int _size = 10, COLORREF color = RGB(255, 255, 0)) {
	renderEllipse(hdc, _x, _y, _size, _size, color);
}

void renderRoundRect(HDC hdc, int x, int y, int sizeX, int sizeY, int w, int h, COLORREF color) {
	HBRUSH hBrush = (HBRUSH)CreateSolidBrush(color);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	RoundRect(hdc, x, y, x + sizeX, y + sizeY, w, h);
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

void renderTriangle(HDC hdc, int x, int y, int size, COLORREF color = RGB(255, 255, 0)) {
	HBRUSH hBrush = (HBRUSH)CreateSolidBrush(color);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	MoveToEx(hdc, x, y, NULL);
	LineTo(hdc, x, y + size);
	LineTo(hdc, x + size / 2, y + size / 2);
	LineTo(hdc, x, y);
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

template<typename TT=int>
struct Pos{
public:
	TT x = 0.f, y = 0.f;

	Pos() {}
	Pos(TT x, TT y) :x(x), y(y) {}

	void set(TT _x, TT _y) { x = _x; y = _y; }

	Pos<TT>operator- (const Pos<TT>& a) {
		Pos<TT>p;
		p.x = x - a.x;
		p.y = y - a.y;
		return p;
	}
	Pos<TT>operator- () {
		Pos<TT>p;
		p.x = -x;
		p.y = -y;
		return p;
	}
	Pos<TT>operator+ (const Pos<TT>& a) {
		Pos<TT>p;
		p.x = x + a.x;
		p.y = y + a.y;
		return p;
	}
	template<typename T>
	Pos<TT>& operator+= (const T& a) {
		x += a;
		y += a;
		return *this;
	}
	template<typename T>
	Pos<TT>& operator+= (const Pos<T>& a) {
		x += a.x;
		y += a.y;
		return *this;
	}
	template<>
	Pos<TT> &operator+= (const Pos<float> &a) {
		x += a.x;
		y += a.y;
		return *this;
	}
	template<typename T>
	Pos<TT> &operator+= (const Pos<TT> &a) {
		x += a.x;
		y += a.y;
		return *this;
	}
	template<typename T>
	Pos<TT> &operator*= (const T &a) {
		x *= a;
		y *= a;
		return *this;
	}
	template<typename T>
	Pos<TT>& operator*= (const Pos<T>& a) {
		x *= a.x;
		y *= a.y;
		return *this;
	}
	template<>
	Pos<TT> &operator*= (const Pos<float> &a) {
		x *= a.x;
		y *= a.y;
		return *this;
	}
	Pos<TT>& operator/= (const Pos<TT>& a) {
		x /= a.x;
		y /= a.y;
		return *this;
	}
	template<typename T>
	Pos<TT>& operator/= (T a) {
		x /= a;
		y /= a;
		return *this;
	}
	template<typename T>
	Pos<TT>operator/ (T a) {
		Pos<TT>p;
		p.x = x / a;
		p.y = y / a;
		return p;
	}
	template<typename T>
	Pos<TT>operator* (T a) {
		Pos<TT>p;
		p.x = x * a;
		p.y = y * a;
		return p;
	}
	Pos<TT>& operator-= (const Pos<TT>& a) {
		x -= a.x;
		y -= a.y;
		return *this;
	}
	bool operator==(const Pos<TT>& a) {
		if (x == a.x && y == a.y)
			return true;
		return false;
	}

	bool operator!=(const Pos<TT>& a) {
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

	Pos<TT> normalize() {
		
		return *this / length();
	}

	Pos<TT>ads() {
		return Pos<TT>(::ads(x), ::ads(y));
	}

	Pos<TT>onlyX() {
		return Pos<TT>(x, 0);
	}

	Pos<TT>onlyY() {
		return Pos<TT>(0, y);
	}

	Pos<TT>flipX() {
		return Pos<TT>(-x, y);
	}
	Pos<TT>flipY() {
		return Pos<TT>(x, -y);
	}

	TT squareLength() {
		return x *x + y * y;
	}

	float length() {
		return sqrtf(squareLength());
	}
};

struct Rect {
	Pos<> a, b;
};


// ���������� �簢������ ��ȯ
Pos<> CollCircleRect(int x, int y, int r, RECT* rt) {
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
			if (x > rt->right) return Pos<> (-1, 0);
			if (rt->top > y) return Pos<> (0, 1);
			if (y > rt->bottom) return Pos<> (0, -1);
		}
	}
	else {
		if (IsPointInCircle(x, y, r, rt->left, rt->top))return Pos<> (1, 1);
		if (IsPointInCircle(x, y, r, rt->left, rt->bottom))return Pos<> (1, -1);
		if (IsPointInCircle(x, y, r, rt->right, rt->top))return Pos<> (-1, 1);
		if (IsPointInCircle(x, y, r, rt->right, rt->bottom))return Pos<> (-1, -1);
	}
	return Pos<>(0, 0);
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
Pos<>ads(Pos<>a) {
	int x = a.x >= 0 ? a.x : -a.x;
	int y = a.y >= 0 ? a.y : -a.y;
	return Pos<>(x, y);
}


class MouseM{
public:
	static Rect lDragRect;
	static bool ldrag, rdrag;
	static int x, y;

	static void lButtonDown() {
		ldrag = true;
		lDragRect.a.set(x,y);
	}
	static void lButtonUp() {
		ldrag = true;
	}
	static void RButtonDown() {
		rdrag = true;
	}
	static void RButtonUp() {
		rdrag = true;
	}

	static void mouseMove(LONG_PTR mousePos) {
		x = LOWORD(mousePos), y = HIWORD(mousePos);
		if (ldrag) lDragRect.b.set(x, y);
	}
};

class DelayC {
	int remainTime;
	int _remainTime = 0;
	bool isLoop;
	bool isBreak = false;
	
public:
	int idx = 0;

	DelayC(int _remain, bool _isLoop, bool beginStart, int id) {
		setRemainTime(_remain, _isLoop);
		idx = id;
		if (beginStart)
			_remainTime = remainTime + 1;
	}

	// isLoop�� false�� ������ ������ �˾Ƽ� �����
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
#ifdef _MBCS
		stringstream ss;
#endif
#ifdef _UNICODE
		wstringstream ss;
#endif
		ss << _remainTime << _T("/") << remainTime;
		TextOut(hdc, x, y, ss.str().c_str(), ss.str().size());
	}

	void reset() {
		_remainTime = 0;
	}

	// ������ ������ ������ �������� �������
	void endNext() {
		_remainTime = remainTime + 1;
	}
};

class MTimer {
	static vector<DelayC> managingObjs;
	static vector<DelayC>::iterator iter;
	static int id;
public:
	// return ID
	// DONT USE THIS!!!!!!!!!
	static int create(int _remainTime, bool _isLoop = false, bool beginStart = true) {
		managingObjs.push_back(DelayC(_remainTime, _isLoop, beginStart, id++));
		return managingObjs.size() - 1;
	}

	// return ID
	static void create(int _remainTime, int _id, bool _isLoop = false, bool beginStart = true) {
		managingObjs.push_back(DelayC(_remainTime, _isLoop, beginStart, _id));
	}

	// must call this on other Update
	static void update(int add = 1) {
		for (iter = managingObjs.begin(); iter != managingObjs.end(); iter++)
		{
			if (iter->update(add))
				managingObjs.erase(iter--);
		}
	}

	static bool isEnd(int idx) {
		for (size_t i = 0; i < managingObjs.size(); i++) {
			if (managingObjs[i].idx == idx) {
				return managingObjs[i].isEnd();
			}
		}
		return false;
	}

	static void reset(int idx) {
		for (size_t i = 0; i < managingObjs.size(); i++) {
			if (managingObjs[i].idx == idx) {
				return managingObjs[i].reset();
			}
		}
	}

	// ������ ������ ������ �������� �������
	static void endNext(int idx) {
		for (size_t i = 0; i < managingObjs.size(); i++) {
			if (managingObjs[i].idx == idx) {
				return managingObjs[i].endNext();
			}
		}
	}

	static bool isHere(int idx) {
		for (size_t i = 0; i < managingObjs.size(); i++) {
			if (managingObjs[i].idx == idx) {
				return true;
			}
		}
		return false;
	}

	static void debug(HDC hdc, int x, int y) {
		int textY = 0;
		for (size_t i = 0; i < managingObjs.size(); i++)
		{
			managingObjs[i].debugRemainTime(hdc, x, y + textY);
			textY += 15;
		}
	}
};

vector<DelayC> MTimer::managingObjs;
vector<DelayC>::iterator MTimer::iter;
int MTimer::id;

// ����������
// ������۸�
class WindowM {
	Pos<>size; // ���� ������
	HWND hwnd;

	HDC dc;
	HDC mainDc;

	HBITMAP hbit;
	HBITMAP oldBit;

	COLORREF bkColor = RGB(255,255,255);
public:
	// call when window size change
	void init(HWND hwnd) {
		RECT rt;
		GetClientRect(hwnd, &rt);
		size.set(rt.right, rt.bottom);

		this->hwnd = hwnd;
	}

	HDC prerender(HDC hdc) {
		if(!hbit)
			hbit = CreateCompatibleBitmap(hdc, size.x, size.y);
		mainDc = hdc;
		dc = CreateCompatibleDC(hdc);
		oldBit = (HBITMAP)SelectObject(dc, hbit);

		renderRect(dc, 0, 0, size.x, size.y, bkColor);
		return dc;
	}

	void postrender() {
		BitBlt(mainDc, 0, 0, size.x, size.y, dc, 0, 0, SRCCOPY);
		SelectObject(dc, oldBit);
		ReleaseDC(hwnd, dc);
	}

	// call in update() plz;
	void clearWindow() {
		InvalidateRect(hwnd, NULL, false);
	}

	Pos<>getSize() {
		return size;
	}
};

class KeyM {

public:
	static bool keys[10];
	/*static bool getKey(WPARAM key) {
		return keys[0];
	}*/
	static void keyFunc(WPARAM key, bool isDown) {
		switch (key) {
		case VK_LEFT:
			keys[1] = isDown;
			break;
		case VK_RIGHT:
			keys[3] = isDown;
			break;
		case VK_UP:
			keys[0] = isDown;
			break;
		case VK_DOWN:
			keys[2] = isDown;
			break;
		case VK_SPACE:
			keys[4] = isDown;
			break;
		}
	}
};

bool KeyM::keys[10];