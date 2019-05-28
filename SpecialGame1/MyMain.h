#pragma once

#include "resource.h"

#include <sstream>
#include <vector>
#include <time.h>
#include <math.h>
#include <atlimage.h>
#include <algorithm>
#include <fstream>
#include <memory>
#include <cassert>
#include <Commdlg.h>

#define DBOUT( s )            \
{                             \
   std::wstringstream os_;    \
   os_ << s;                   \
   OutputDebugString( os_.str().c_str() );  \
}

using namespace std;

#define PI 3.1415926535f
#define Radian PI/180

extern RECT rectView;
void getWindowSize(HWND hwnd);

template<typename T>
T ads(T a) {
	return a >= 0 ? a : -a;
}

void renderEllipse(HDC hdc, int _x, int _y, int _sizeX, int _sizeY, COLORREF color = RGB(255, 255, 0));
void renderCircle(HDC hdc, int _x, int _y, int _size = 10, COLORREF color = RGB(255, 255, 0));
void renderRoundRect(HDC hdc, int x, int y, int sizeX, int sizeY, int w, int h, COLORREF color);
void renderRect(HDC hdc, int _x, int _y, int _sizeX = 10, int _sizeY = 10, COLORREF color = RGB(255, 255, 0));
void renderTriangle(HDC hdc, int x, int y, int size, COLORREF color = RGB(255, 255, 0));

bool IsPointInCircle(int x, int y, int r, int px, int py);
bool collPointRect(int x, int y, RECT *rt);

template<typename TT=int>
struct Pos{
public:
	TT x = 0, y = 0;

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
		float epsilon = 0.001f;
		if (::ads(x - a.x) <= epsilon && ::ads(y - a.y) <= epsilon)
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

	Pos<TT> rotate(float angle) {
		float co = cos(angle * PI / 180);
		float si = sin(angle * PI / 180);
		float _x = co *x - si * y;
		float _y = si * x + co * y;
		return Pos<TT>(_x, _y);
	}

	void setZero() { set(0, 0); }
};

// 원기준으로 사각형방향 반환
Pos<> CollCircleRect(int x, int y, int r, RECT *rt);

int normalize(int a);
void setAlign(Pos<> &a, Pos<> &b);
void initRandom();
float random();// 0.0 ~ 1.0
float random(float a);// 0.0 ~ a
int random(int a, int b);// a 와 b 사이의 값 줌
int random(int a);// 0 ~ a
Pos<float> randomCircle(float size = 1);


// 윈도사이즈
// 더블버퍼링
class WindowM {
	Pos<>size; // 윈도 사이즈
	HWND hwnd;

	HDC dc;
	HDC mainDc;

	HBITMAP hbit;
	HBITMAP oldBit;

	COLORREF bkColor = RGB(255,255,255);
public:
	// call when window size change
	void init(HWND hwnd);

	void init(HWND hwnd, int _x, int _y);

	HDC prerender(HDC hdc);

	void postrender();

	void postrender(int x, int y);

	// call in tick() plz;
	void clearWindow();

	const Pos<> &getSize() const;
};

#define BYE
#ifdef BYE

extern int deltatime;

class Manager;

// TODO 임시로 SceneM 메인에선 렌더 안했다가 게임시작시에만 
class SceneM {
	friend Manager;

	vector<vector<Manager*>> objs;
	static vector<SceneM> self;

	

	void addObj(Manager *obj, size_t layer);
	
public:
	/*
	교실
	책상, 휴지통, 시계
	시험지, 
	사람들
	*/
	SceneM() {
		resizeLayer(4);
	}
	static SceneM &getIns(size_t i);
	void tick ();
	void render(HDC hdc);
	void destoryObj(Manager *obj);

	void reset();

	// TODO 줄일때 기존에것들 안지우고 줄임 나중에 추가하기
	void resizeLayer(int layerCount);
};

extern WindowM win;

// 씬매니저에서 
class Manager {
public:
	Manager(int layer = 0);
	virtual ~Manager();
	void addToM(int layer);
	virtual void tick () {}
	virtual void render(HDC hdc){}
};

class Obj : Manager{
public:
	// Position
	Pos<float> p;
	// use for camera or shake
	Pos<float> off;

	Pos<float> size;
	COLORREF color = RGB(20, 20, 20);

	bool isAble = true;

	// 숫자가 커질수록 더 나중에 그려진다.
	Obj (int layer = 0) : Manager(layer){
	}
	
	virtual ~Obj() {}
	virtual void tick () {}
	virtual void render(HDC hdc);
	virtual int collObj(Obj *o);
};

// TODO 줄일때 기존에것들 안지우고 줄임 나중에 추가하기

class MovableObj :public Obj {
public:
	MovableObj(int layer):Obj(layer){}
	virtual ~MovableObj() {}
	float speed = 3;
	virtual void move () {

	}
	virtual void render (HDC hdc) {
	}
};

vector<int> unDuplicateRandom(size_t count, int range);

#endif