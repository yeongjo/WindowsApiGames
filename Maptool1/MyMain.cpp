#include "MyMain.h"

RECT rectView;
WindowM win;
int deltatime = 15;
vector<SceneM> SceneM::self;

template<>
Pos<> ads(Pos<>a) {
	int x = a.x >= 0 ? a.x : -a.x;
	int y = a.y >= 0 ? a.y : -a.y;
	return Pos<>(x, y);
}

// call when window size change
inline void WindowM::init(HWND hwnd) {
	RECT rt;
	GetClientRect(hwnd, &rt);
	init(hwnd, rt.right, rt.bottom);
}

inline void WindowM::init(HWND hwnd, int _x, int _y) {
	size.set(_x, _y);
	initRandom(); // 꼽사리
	this->hwnd = hwnd;
}

inline HDC WindowM::prerender(HDC hdc) {
	if (!hbit)
		hbit = CreateCompatibleBitmap(hdc, size.x, size.y);
	mainDc = hdc;
	if (!dc)
		dc = CreateCompatibleDC(hdc);
	oldBit = (HBITMAP)SelectObject(dc, hbit);

	renderRect(dc, 0, 0, size.x, size.y, bkColor);
	return dc;
}

inline void WindowM::postrender() {
	BitBlt(mainDc, 0, 0, size.x, size.y, dc, 0, 0, SRCCOPY);
	SelectObject(dc, oldBit);
	//ReleaseDC(hwnd, dc);
}

inline void WindowM::postrender(int x, int y) {
	BitBlt(mainDc, x, y, size.x, size.y, dc, 0, 0, SRCCOPY);
	SelectObject(dc, oldBit);
	//ReleaseDC(hwnd, dc);
}

// call in tick() plz;

inline void WindowM::clearWindow() {
	InvalidateRect(hwnd, NULL, false);
}

inline const Pos<> &WindowM::getSize() const {
	return size;
}

void getWindowSize(HWND hwnd) {
	GetClientRect(hwnd, &rectView);
}

void renderEllipse(HDC hdc, int _x, int _y, int _sizeX, int _sizeY, COLORREF color) {
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

void renderCircle(HDC hdc, int _x, int _y, int _size, COLORREF color) {
	renderEllipse(hdc, _x, _y, _size, _size, color);
}

void renderRoundRect(HDC hdc, int x, int y, int sizeX, int sizeY, int w, int h, COLORREF color) {
	HBRUSH hBrush = (HBRUSH)CreateSolidBrush(color);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	RoundRect(hdc, x, y, x + sizeX, y + sizeY, w, h);
	SelectObject(hdc, oldBrush);
	DeleteObject(hBrush);
}

void renderRect(HDC hdc, int _x, int _y, int _sizeX, int _sizeY, COLORREF color) {
	int x = _x, y = _y;
	HBRUSH hBrush = (HBRUSH)CreateSolidBrush(color);
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	Rectangle(hdc, x, y, x + _sizeX, y + _sizeY);
	SelectObject(hdc, oldBrush);
	DeleteObject(hBrush);
}

void renderTriangle(HDC hdc, int x, int y, int size, COLORREF color) {
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
	if (l > r * r)
		return false;
	return true;
}

bool collPointRect(int x, int y, RECT *rt) {
	if ((rt->left <= x && x <= rt->right) &&
		(rt->top <= y && y <= rt->bottom))
		return true;
	return false;
}


Pos<> CollCircleRect(int x, int y, int r, RECT *rt) {
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
	} else {
		if (IsPointInCircle(x, y, r, rt->left, rt->top))return Pos<>(1, 1);
		if (IsPointInCircle(x, y, r, rt->left, rt->bottom))return Pos<>(1, -1);
		if (IsPointInCircle(x, y, r, rt->right, rt->top))return Pos<>(-1, 1);
		if (IsPointInCircle(x, y, r, rt->right, rt->bottom))return Pos<>(-1, -1);
	}
	return Pos<>(0, 0);
}

int normalize(int a) {
	return a / ads<int>(a);
}

void setAlign(Pos<> &a, Pos<> &b) {
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
	return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / a));
}

// a 와 b 사이의 값 줌
int random(int a, int b) {
	return rand() % (b - a) + a;
}

int random(int a) {
	return rand() % a;
}

Pos<float> randomCircle(float size) {
	return Pos<float>(random(size), random(size));
}

vector<int> unDuplicateRandom(size_t count, int range) {
	vector<int> t(count * 2);
	for (size_t i = 0; i < count; ) {
		t [i] = rand() % range;
		if (t [i + count] == 0) {
			t [i + count] = 1;
			i++;
		}
	}
	t.resize(count);
	return t;
}

inline Manager::Manager(int layer) {
	addToM(layer);
}

inline Manager::~Manager() {
	SceneM::getIns(0).destoryObj(this);
}

inline void Manager::addToM(int layer) {
	SceneM::getIns(0).addObj(this, layer);
}

inline void Obj::render(HDC hdc) {
	renderRect(hdc, static_cast<int>(p.x), static_cast<int>(p.y), (int)size.x, (int)size.y, RGB(20, 20, 20));
}

inline int Obj::collObj(Obj *o) {
	if (o == nullptr) return 0;
	// 나갔을경우
	if ((p.x > o->p.x + o->size.x || p.x + size.x < o->p.x) ||
		(p.y > o->p.y + o->size.y || p.y + size.y < o->p.y))
		return 0;
	return 1;
}

inline void SceneM::addObj(Manager *obj, size_t layer) {
	assert(objs.size() > layer);
	objs [layer].push_back(obj);
}

inline SceneM &SceneM::getIns(size_t i) {
	if (i == 0) {
		self.resize(1);
	}
	if (i < 0 || i >= self.size()) assert("SceneM 에 있지도 않은 인덱스를 참조함" == 0);
	return self [i];
}

void SceneM::tick() {
	size_t s1 = objs.size();
	for (size_t i = 0; i < s1; i++) {
		size_t s2 = objs [i].size();
		for (size_t j = 0; j < s2; j++) {
			objs [i][j]->tick();
		}
	}
}

inline void SceneM::render(HDC hdc) {
	size_t s1 = objs.size();
	for (size_t i = 0; i < s1; i++) {
		size_t s2 = objs [i].size();
		for (size_t j = 0; j < s2; j++) {
			objs [i][j]->render(hdc);
		}
	}
}

void SceneM::destoryObj(Manager *obj) {
	Manager *t;
	size_t s1 = objs.size();
	for (size_t i = 0; i < s1; i++) {
		size_t s2 = objs [i].size();
		for (size_t j = 0; j < s2; j++) {
			t = objs [i][j];
			if (t == obj) {
				// 스스로 delete 됬을때만 이곳으로 오기때문에 delete 안해도됨
				//delete t;
				objs[i].erase(objs[i].begin() + j);
				return;
			}
		}
	}
}

inline void SceneM::reset() {
	for (size_t i = 0; i < objs.size(); i++) {
		for (size_t j = 0; j < objs [i].size(); j++) {
			delete objs [i][j];
		}
		objs [i].clear();
	}
}

// TODO 줄일때 기존에것들 안지우고 줄임 나중에 추가하기

inline void SceneM::resizeLayer(int layerCount) {
	objs.resize(layerCount);
}
