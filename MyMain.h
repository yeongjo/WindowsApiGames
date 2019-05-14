#pragma once

#include "resource.h"

#include <windowsx.h>
#include <sstream>
#include <vector>
#include <time.h>
#include <math.h>
#include <atlimage.h>
#include <algorithm>
#include <fstream>
#include <memory>
#include <cassert>

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

struct Rect {
	Pos<> a, b;
};


// 원기준으로 사각형방향 반환
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

// 0.0 ~ 1.0
float random() {
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

// 0.0 ~ a
float random(float a) {
	return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/a));
}
// a 와 b 사이의 값 줌
int random(int a, int b) {
	return rand() % (b - a) + a;
}

int random(int a) {
	return rand() % a;
}

Pos<float> randomCircle(float size = 1) {
	return Pos<float>(random(size),random(size));
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

	// isLoop가 false면 return isBreak 루프가 끝나면 알아서 사라짐
	bool update(int add = 1) {
		_remainTime += add;
		return isBreak;
	}

	void setEnd() {
		isBreak = true;
	}

	void setRemainTime(int _remain, bool _isLoop) {
		remainTime = _remain;
		isLoop = _isLoop;
	}

	void changeRemainTime(int time) {
		remainTime = time;
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
		ss << idx << _T(":")<< _remainTime << _T("/") << remainTime;
		TextOut(hdc, x, y, ss.str().c_str(), ss.str().size());
	}

	void reset() {
		_remainTime = 0;
	}

	// 다음에 무조건 끝나는 조건으로 만들어줌
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
		managingObjs.push_back(DelayC(_remainTime, _isLoop, beginStart, id));
		return id++;
	}

	// input ID and use
	static int create(int _remainTime, int _id, bool _isLoop = false, bool beginStart = true) {
		if (id < _id) return 0;
		managingObjs.push_back(DelayC(_remainTime, _isLoop, beginStart, _id));
		id = _id + 1;
		return 1;
	}

	// must call this on other Update
	static void update(int add = 1) {
		int t = 0;
		for (iter = managingObjs.begin(); iter != managingObjs.end(); ++t)
		{
			if (iter->update(add)) {
				iter = managingObjs.erase(iter);
			} else
				++iter;
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

	static void setEnd(int idx) {
		for (size_t i = 0; i < managingObjs.size(); i++) {
			if (managingObjs[i].idx == idx) {
				return managingObjs[i].setEnd();
			}
		}
	}

	static void changeEndTime(int idx, int time) {
		for (size_t i = 0; i < managingObjs.size(); i++) {
			if (managingObjs[i].idx == idx) {
				return managingObjs[i].changeRemainTime(time);
			}
		}
	}

	static void reset(int idx) {
		for (size_t i = 0; i < managingObjs.size(); i++) {
			if (managingObjs[i].idx == idx) {
				return managingObjs[i].reset();
			}
		}
	}

	// 다음에 무조건 끝나는 조건으로 만들어줌
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
	void init(HWND hwnd) {
		RECT rt;
		GetClientRect(hwnd, &rt);
		size.set(rt.right, rt.bottom);

		initRandom(); // 꼽사리

		this->hwnd = hwnd;
	}

	HDC prerender(HDC hdc) {
		if(!hbit)
			hbit = CreateCompatibleBitmap(hdc, size.x, size.y);
		mainDc = hdc;
		if(!dc)
			dc = CreateCompatibleDC(hdc);
		oldBit = (HBITMAP)SelectObject(dc, hbit);

		renderRect(dc, 0, 0, size.x, size.y, bkColor);
		return dc;
	}

	void postrender() {
		BitBlt(mainDc, 0, 0, size.x, size.y, dc, 0, 0, SRCCOPY);
		SelectObject(dc, oldBit);
		//ReleaseDC(hwnd, dc);
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
//TODO 키 매니저 어떻게 쉽게 만들까

#ifdef 
class Obj;
class WorldM;

class ObjM {
	friend Obj;
	friend WorldM;

	vector<vector<Obj*>> objs;

	ObjM() {
		// Block bullet
		// 바닥에 그리는 격자
		// Guy 터지는효과
		resizeLayer(3);
	}

	void reset() {
		for (size_t i = 0; i < objs.size(); i++) {
			for (size_t j = 0; j < objs[i].size(); j++) {
				delete objs [i][j];
			}
			objs [i].clear();
		}
	}

	void resizeLayer(int layerCount) {
		objs.resize(layerCount);
	}
	
	// +1 -1 로 추가할수있게하기
	// TODO 0넣으면 초기화
	void editlayer(int i) {

	}

	void addObj (Obj* obj, int layer) {
		assert(objs.size() > layer);
		objs[layer].push_back (obj);
	}
	
public:
	static ObjM self;
	void update ();
	void render(HDC hdc);
	void destoryObj(Obj *obj);
};

ObjM ObjM::self;

WindowM win;

int blockSize = 32;

class Obj {
public:
	Pos<float> p;
	Pos<float> off;

	int size = 32;
	int h_size = 16;
	COLORREF color = RGB(20, 20, 20);

	bool isAble = true;

	Obj (int layer = 0) {
		addToM(layer);
	}
	void addToM(int layer) {
		ObjM::self.addObj (this, layer);
	}
	virtual ~Obj() {
		ObjM::self.destoryObj(this);
	}
	virtual void update () {}
	virtual void render(HDC hdc){
		renderRect(hdc, (int)p.x, (int)p.y, size, size, RGB(20, 20, 20));
	}
	virtual int collObj(Obj *o) {
		if (o == nullptr) return 0;
		// 나갔을경우
		if ((p.x > o->p.x + o->size || p.x + size < o->p.x) ||
			(p.y > o->p.y + o->size || p.y + size < o->p.y))
			return 0;
		return 1;
	}
	virtual void takeDamage(int damage) {

	}
};

void ObjM::render(HDC hdc) {
	for (size_t i = 0; i < objs.size(); i++) {
		for (size_t j = 0; j < objs[i].size(); j++) {
			objs[i][j]->render(hdc);
		}
	}
}

class StaticObj :public Obj{
public:
	StaticObj():Obj(){}
	virtual void render (HDC hdc) {
	}
};

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


class Block : public StaticObj {
public:
	Block(Pos<float> pos) : StaticObj() {
		color = RGB(20, 20, 20);
		p = pos;
	}

	Block(float x, float y) : StaticObj() {
		color = RGB(20, 20, 20);
		p = Pos<float>(x,y);
	}

	// TODO destroy 도 하나식 붙여주기 매니저로 부터 떨어트리기 용
	virtual void destory() {
		isAble = false;
	}

	virtual void render(HDC hdc) {
		if (!isAble) return;
		renderRect(hdc, p.x + off.x, p.y+ off.y, size, size, color);
	}
};

class AssBlock :public Block {
public:
	AssBlock(float x, float y) : Block(x,y) {
		color = RGB(80, 50, 20);
		p = Pos<float>(x, y);
	}

	// 부술수 없음
	virtual void destory() {
	}
};

class Enemy; class Player; class DropedBullet;

class WorldM {
public:
	vector<Block *> blocks;
	vector<Enemy *> enemy; // TODO 삭제 더 편한방법없을까
	vector<DropedBullet *> dropedBullets;
	Player *player = nullptr;

	// 위쪽 ui 때문에
	int blockYOffset = 64;

	int enemyGenSec = 2000;
	int timerEnemyGen;

	int level = 0;

	WorldM() {
		// Timer for enemy spawn
		timerEnemyGen = MTimer::create(enemyGenSec, true, true);
	}

	void initLevel(int level);

	// 맵 읽어와서 만듬
	void createBlocks(int level) {
		// 초기화 ========================
		ObjM::self.reset();
		player = nullptr;
		//for (size_t i = 0; i < blocks.size(); i++) {
		//	//ObjM::self.destoryObj((Obj*)blocks [i]);
		//	delete blocks [i];
		//}
		blocks.clear();

		//for (size_t i = 0; i < enemy.size(); i++) {
		//	//ObjM::self.destoryObj((Obj*)enemy [i]);
		//	delete enemy [i];
		//}
		enemy.clear();
		// ====== ========================

		stringstream ss;
		int yLine = 0;
		ifstream is;

		switch (level) {
		case 0:
			break;
		case 1:
		{
			is.open("map1.txt");
			break;
		}
		case 2:
			is.open("map2.txt");
			break;
		}

		if (is.is_open()) {
			string s;
			while (getline(is, s)) {
				for (size_t i = 0; i < s.size(); i++) {
					switch (s[i]) {
					case '0':
						break;
					case '1':
						blocks.push_back(new Block(i * blockSize, blockSize * yLine + blockYOffset));
						break;
					case '2':
						blocks.push_back(new AssBlock(i * blockSize, blockSize * yLine + blockYOffset));
						break;
					}
				}
				++yLine;
			}
		}
	}


	void destoryAroundBlock(Pos<float>& p, float len = 3);

	void playerDie();

private:
	int playerRegenTimer = -1;

	void shakeEffect() {
		auto a = randomCircle(10);
		for (size_t i = 0; i < ObjM::self.objs.size(); i++) {
			for (size_t j = 0; j < ObjM::self.objs[i].size(); j++) {
				ObjM::self.objs [i][j]->off = a;
			}
		}
	}

	int initRemainShake = 4;
	int remainShake = initRemainShake;

	void shakeUpdate() {
		if (remainShake > 0) {
			--remainShake;
			if(remainShake == 0)
				for (size_t i = 0; i < ObjM::self.objs.size(); i++) {
					for (size_t j = 0; j < ObjM::self.objs[i].size(); j++) {
						ObjM::self.objs [i][j]->off.setZero();
					}
				}
			else
				shakeEffect();
		}
	}
public:
	void startShake() {
		remainShake = initRemainShake;
	}
	
	void addEnemy();

	void addBlock(Pos<float> pos) {
		blocks.push_back(new Block(pos));
	}

	bool isPlayerDead = false;
	void update();

	void setPlayerMatchless();

	void dropItem(int x, int y, int type = 0);
};

WorldM wm;

void ObjM::update() {
	MTimer::update(deltatime);
	for (size_t i = 0; i < objs.size(); i++) {
		for (size_t j = 0; j < objs[i].size(); j++) {
			objs [i][j]->update();
		}
	}
	wm.update();
	ui.update();
}

void WorldM::addEnemy() {
	auto winSize = win.getSize();
	int ran = rand() % 100;
	if (ran < 0) {
		enemy.push_back(new Enemy());

	} else if (ran < 50) {
		enemy.push_back(new BigEnemy());

	} else
		enemy.push_back(new GunEnemy());

	auto e = enemy.back();
	int x = rand() % 300;
	int y = rand() % 300;
	x = x > 150 ? x + winSize.x - 300 : x;
	y = y > 150 ? y + winSize.y - 300 : y;
	e->p.set(x, y);
}

void WorldM::destoryAroundBlock(Pos<float>& p, float len) {
	// 플레이어 주변의 블럭 부수기
	for (size_t i = 0; i < blocks.size(); i++) {
		if ((blocks [i]->p - p).squareLength() < blockSize*blockSize * len) {
			blocks [i]->destory();
		}
	}
}

void WorldM::initLevel(int level) {
	createBlocks(level);
	this->level = level;

	if(player == nullptr)
		player = new Player();
		player->init();
}

void WorldM::update() {
	// TODO Player regen, Enemy Gen
	if (MTimer::isEnd(timerEnemyGen)) {
		if (enemyGenSec > 100) {
			enemyGenSec -= 10;
		}
		MTimer::changeEndTime(timerEnemyGen, enemyGenSec);
		addEnemy();
	}
	if (playerRegenTimer == -1 && isPlayerDead && player) {
		// Timer 플레이어 리젠으로 씀
		playerRegenTimer = MTimer::create(1500, false, false);
	} else if (isPlayerDead && MTimer::isEnd(playerRegenTimer)) {
		player->init();
		isPlayerDead = false;
		playerRegenTimer = -1;
	}

	shakeUpdate();
}

void WorldM::dropItem(int x, int y, int type) {
	DropedBullet *b;
	switch (type) {
	case 1:
		dropedBullets.push_back(new DropedScatterBullet());
		break;
	case 0:
		dropedBullets.push_back(new DropedBullet2());
		break;
	case 2:
		dropedBullets.push_back(new DropedAttackSpeedUp());
		break;
	case 3:
		dropedBullets.push_back(new DropedSpeedUp());
		break;
	case 4:
		dropedBullets.push_back(new DropedBulletRegenSpeedUp());
		break;
	}
	b = dropedBullets.back();
	b->p.set(x, y);
}

void WorldM::playerDie() {
	isPlayerDead = true;
	player->isAble = false;
}

void WorldM::setPlayerMatchless() {
	if (player)
		player->isMatchless = !player->isMatchless;
}