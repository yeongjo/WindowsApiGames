// WindowsWork5-6.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "WindowsWork5-6.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSWORK56, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSWORK56));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSWORK56));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSWORK56);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


void renderRectR2Not(HDC hdc, int x, int y, int renderSizeX, int renderSizeY) {
	SetROP2(hdc, R2_NOT);
	renderRect(hdc, x - 1, y - 1, renderSizeX + 1, renderSizeY + 1);
	SetROP2(hdc, R2_COPYPEN);
}

class MSprite {
public:

	// ========== IMAGE ===========
	// For bitmap
	HBITMAP hBitmap = 0;
	BITMAP bmp;

	// New one
	CImage* img = NULL;

	// ========== POS ===========

	Pos pos;
	Pos size;
	Pos sliceStartPos;
	Pos renderSize;

	// ========== SPLIT ===========

	Pos splitUV;
	UINT spriteCount;
	UINT spriteIdx = 0;

	bool isAble = true;

	vector<vector<int>> animGroup;
	UINT currentGroupIdx = 0;
	UINT currentGroupSecondIdx = 0;

	UINT animFrameDelay = 3;


	MSprite() {}

	virtual ~MSprite() {
		//Destory();
	}

	virtual void init(const WCHAR* path, Pos _splitUV = Pos(0, 0), int _spriteCount = 1) {
		if (!img)
			img = new CImage();
		ATLASSERT(!img->Load(path));

		size.x = img->GetWidth();
		size.y = img->GetHeight();
		renderSize = size;
		splitUV = _splitUV;
		spriteCount = _spriteCount;
	}

	void init(int imgID) {
		//isAble = true;
		hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(imgID));
		GetObject(hBitmap, sizeof(BITMAP), &bmp);
		size.x = bmp.bmWidth;
		size.y = bmp.bmHeight;
		renderSize.set(600, 600);
		size.set(600, 600);
	}

	void setAnimGroup(const vector<vector<int>>& _animGroup) {
		animGroup = _animGroup;
	}

	void Destory() {
		if (hBitmap)
			DeleteObject(hBitmap);
		if (img)
			img->Destroy();
	}

	bool collPointIn(int x, int y) {
		RECT rt = RECT{ pos.x, pos.y, pos.x + renderSize.x, pos.y + renderSize.y };
		return collPointRect(x, y, &rt);
	}

	void changeAnimGroup(UINT i) {
		if (i == currentGroupIdx) return;
		currentGroupIdx = i % animGroup.size();
		currentGroupSecondIdx = 0;
	}

	UINT nextSpriteDelay;
	void goNextSpriteIdx() {
		if (nextSpriteDelay++ < animFrameDelay || animGroup.size() == 0) return;
		nextSpriteDelay = 0;
		currentGroupSecondIdx = (currentGroupSecondIdx + 1) % animGroup[currentGroupIdx].size();
		spriteIdx = animGroup[currentGroupIdx][currentGroupSecondIdx];
	}

	virtual void update() {
		goNextSpriteIdx();
	}

	MSprite createMySelf() {
		return *this;
	}

	virtual void render(HDC hdc) {
		if (!isAble) return;

		int x = pos.x, y = pos.y;
		int sx = size.x, sy = size.y;

		int mx = sliceStartPos.x, my = sliceStartPos.y;
		int rsX = renderSize.x, rsY = renderSize.y;

		if (spriteCount > 1) {
			UINT nSpriteWidth = size.x / splitUV.x;
			UINT nSpriteHeight = size.y / splitUV.y;
			UINT xCoord = spriteIdx % splitUV.x;
			UINT yCoord = spriteIdx / splitUV.x;
			renderSize.set(nSpriteWidth, nSpriteHeight);
			//CImage* t_img = &img;
			//t_img->Draw(hdc, x + 100, y, rsX, rsX,
			//	xCoord * nSpriteWidth, yCoord * nSpriteHeight, nSpriteWidth,
			//	nSpriteHeight);

			img->Draw(hdc, x, y, nSpriteWidth, nSpriteHeight,
				xCoord * nSpriteWidth, yCoord * nSpriteHeight, nSpriteWidth,
				nSpriteHeight);

		}
		else {
			img->Draw(hdc, x, y, rsX, rsX);
		}

		/*HDC hMemDC = CreateCompatibleDC(hdc);

		SelectObject(hMemDC, hBitmap);
		StretchBlt(hdc, x, y, rsX, rsY, hMemDC, mx, my, sx, sy, SRCCOPY);

		DeleteDC(hMemDC);*/
	}
};

int defaultSize = 79;


class Object : public MSprite {
public:
	//MSprite sprite;
	Pos cusPos;
	int id = -1;
	int hp = 3;

	bool isDead =false;

	virtual void init(const WCHAR* path, Pos _splitUV = Pos(0, 0), int _spriteCount = 1) {
		MSprite::init(path, _splitUV, _spriteCount);
	}

	virtual ~Object() {

	}

	void init() {
		isAble = true;
	}

	Object* createMySelf() {
		return new Object(*this);
	}

	// ========================== RENDER ========================

	virtual void render(HDC hdc) {
		MSprite::render(hdc);
	}

	virtual void update() {
		MSprite::update();
		jumpUpdate();
	}

	virtual bool isCanMove() {
		if (isJump) return false;
		return true;
	}


	// ========================== MOVE ==========================
	int speed = 1;
	virtual void move(int _x, int _y) {
		if (!isCanMove()) return;
		_x *= speed;
		_y *= speed;

		pos.x += _x;
		pos.y += _y;
	}

	// ========================== Jump ==========================

	bool isJump = false;
	int height = 0;
	int acc = -40;

	void jumpUpdate() {
		if (!isJump) return;

		int gravity = 8;

		height += acc += gravity;

		if (height >= 0) {
			isJump = false;
			height = 0;
			acc = -40;
		}
	}

	void jump() {
		if (!isCanMove()) return;
		isJump = true;
	}

	// ========================== GAME PLAY ==========================

	void takeDamage(int damage) {
		hp -= damage;
		hp = hp < 0 ? 0 : hp;
		if (hp == 0)
			isDead = true;
	}
};

class Block;
vector<Block*> blockGroup[3];

class Block : public Object {
public:
	virtual void update() {
		Object::update();
		
	}
};

class Enemy;


MTimer timer;

vector<Enemy*> enemys;

class Enemy : public Object {
public:

	virtual void init() {
		Object::init(_T("../mon1_sprite.png"), Pos(5, 5), 15);
		vector<vector<int>> _animGroup;
		_animGroup.resize(3);
		_animGroup[0].resize(5);
		_animGroup[1].resize(3);
		_animGroup[2].resize(5);
		int t = 0;
		for (size_t i = 0; i < _animGroup.size(); i++) {
			for (size_t j = 0; j < _animGroup[i].size(); j++) {
				_animGroup[i][j] = t++;
				if (j == 2 && i == 1)
					t += 2;
			}

		}
		setAnimGroup(_animGroup);
	}
	int timerIdx = -1;
	virtual void update() {
		Object::update();
		if (isDead) {
			if(timerIdx == -1)
				timerIdx = timer.create(250, false, false);
			if (timer.isEnd(timerIdx)) {
				vector<Enemy*>::iterator iter;
				for (iter = enemys.begin(); iter != enemys.end(); iter++) {
					if (*iter == this) {
						enemys.erase(iter);
						break;
					}
				}
			} else {
				changeAnimGroup(2);
			}
		} else {
			speed = 1;
			int x = random(3) - speed, y = random(3) - speed;
			move(x, y);
		}
	}
};

class Bullet : public Object {
public:
	Pos direc;
	int damage = 1;

	Bullet(Pos direc) : Object() {
		speed = 10;
		this->direc = direc;
	}

	virtual void update() {
		Object::update();
		move(direc.x, direc.y);
		//collPointIn()
		//다른오브젝트가 이 어두운 부랄이 불리게만든다.
	}

	bool checkCollisionWithOther() {
		bool coll = false;
		Object *obj = nullptr;
		for (size_t i = 0; i < enemys.size(); i++) {
			if (coll = enemys[i]->collPointIn(pos.x, pos.y)) {
				obj = enemys[i];
				goto outfor;
			}
		}
		for (size_t j = 0; j < 3; j++) {
			for (size_t i = 0; i < blockGroup[j].size(); i++) {
				if (coll = blockGroup[j][i]->collPointIn(pos.x, pos.y)) {
					obj = blockGroup[j][i];
					goto outfor;
				}
			}
		}
		outfor:
		if (coll && obj) {
			obj->takeDamage(damage);
			return true;
		}
		return false;
	}

	virtual void render(HDC hdc) {
		renderCircle(hdc, pos.x, pos.y, 10, 10, RGB(200,0,0));
	}
};



class Player : public Object {
	vector<Bullet*> bullets;
	Pos lastDirec;
public:

	virtual void init() {
		Object::init(_T("../player.png"), Pos(9, 4), 36);
		speed = 5;

		vector<vector<int>> _animGroup(4);
		for (size_t i = 0; i < 4; i++) {
			_animGroup[i].resize(9);
			for (size_t j = 0; j < _animGroup[i].size(); j++) {
				_animGroup[i][j] = i * _animGroup[i].size() + j;
			}
		}
		
		animGroup = _animGroup;
	}

	virtual void update() {
		Object::update();
		for (size_t i = 0; i < bullets.size(); i++) {
			bullets[i]->update();
			if (bullets[i]->checkCollisionWithOther()) {
				bullets.erase(bullets.begin() + i);
			}
		}
	}

	void shot() {
		auto t = new Bullet(lastDirec);
		t->pos = pos;
		bullets.push_back(t);
	}

	virtual void move(int x, int y){
		Object::move(x, y);

		if (x == 0 && y == -1) {
			changeAnimGroup(0);
		} else if (x == 0 && y == 1) {
			changeAnimGroup(2);
		} else if (x == -1 && y == 0) {
			changeAnimGroup(1);
		} else if (x == 1 && y == 0) {
			changeAnimGroup(3);
		}

		lastDirec.x = x;
		lastDirec.y = y;
	}

	virtual void render(HDC hdc) {
		Object::render(hdc);
		for (size_t i = 0; i < bullets.size(); i++) {
			bullets[i]->render(hdc);
		}
	}
};

Player player;

void generateEnemy() {
	auto t = new Enemy();
	t->init();
	t->pos.set(random(rectView.right), random(rectView.bottom));
	enemys.push_back(t);
}

// Double Buffer
CImage dbdc;


// MOUSE
RECT dragRect;
bool g_ldrag, g_rdrag;
int g_mouseX, g_mouseY;


void setMousePos(int param) {
	int x = LOWORD(param);
	int y = HIWORD(param);
	g_mouseX = x;
	g_mouseY = y;
}

Object blockPresets[6];

void initBlockPos() {
	for (size_t i = 0; i < 3; i++) {
		blockGroup[i].resize(2 * (i+1));
		int x = random(200, (int)rectView.right - 200), y = random(200, (int)rectView.bottom - 200);
		for (size_t j = 0; j < blockGroup[i].size(); j++) {
			blockGroup[i][j] = (Block*)blockPresets[0].createMySelf();
			blockGroup[i][j]->pos.set(x + defaultSize * (j/2),y + defaultSize * (j%2));
		}
	}
}

void initBlockBitmap() {
	blockPresets[0].init(_T("../474.jpg"), Pos(6, 1), 6);

	vector<vector<int>> _animGroup(6);
	vector<vector<int>> a(1);
	a[0].resize(1);
	for (size_t i = 0; i < 6; i++)
	{
		if (i != 0)
			blockPresets[i] = blockPresets[0];
		a[0][0] = i;
		//blockPresets[i].setAnimGroup(a);
		blockPresets[i].spriteIdx = i;
		blockPresets[i].id = i;
		blockPresets[i].cusPos.x = i;
	}

	initBlockPos();
}

void checkDestoryBlock() {
	vector<Block*>::iterator iter;
	for (size_t i = 0; i < 3; i++) {
		for (iter = blockGroup[i].begin(); iter != blockGroup[i].end(); iter++) {
			if ((*iter)->isDead) {
				blockGroup[i].erase(iter);
				break;
			}
		}
	}
}

Object* CreatePresetBlock(int i) {
	auto t = blockPresets[i].createMySelf();
	t->cusPos.set(2, 0);
	return t;
}

int timerForDrop;


void shot() {
	player.shot();
}

void move(int x, int y) {
	player.move(x, y);
}

void init() {
	dbdc.Create(rectView.right, rectView.bottom, 24, 0);

	timerForDrop = timer.create(1000, true);

	initBlockBitmap();
	player.init();
}


void render(HDC hdc) {
	HDC bDC = dbdc.GetDC();
	FillRect(bDC, &rectView, (HBRUSH)(GetStockObject(WHITE_BRUSH)));

	for (size_t y = 0; y < 3; y++)
		for (size_t x = 0; x < blockGroup[y].size(); x++)
			blockGroup[y][x]->render(bDC);

	for (size_t i = 0; i < enemys.size(); i++) {
		enemys[i]->render(bDC);
	}

	player.render(bDC);

	timer.debug(bDC, 0, 0);

	dbdc.Draw(hdc, 0, 0);
	dbdc.ReleaseDC();
}

void update(HWND hwnd) {
	GetClientRect(hwnd, &rectView);
	timer.update(16);

	if (timer.isEnd(timerForDrop)) {
		generateEnemy();
	}

	checkDestoryBlock();

	for (size_t i = 0; i < enemys.size(); i++) {
		enemys[i]->update();
	}

	player.update();
	
	//for (size_t y = 0; y < BlockSizeY; y++)
	//	for (size_t x = 0; x < BlockSizeX; x++)
	//		if (blocks[y][x])
	//			blocks[y][x]->update();
	InvalidateRect(hwnd, NULL, false);
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		getWindowSize(hWnd);
		SetTimer(hWnd, 1, 16, NULL);
		srand(time(NULL));
		init();
		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 메뉴 선택을 구문 분석합니다:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_TIMER:
		update(hWnd);
		break;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_LEFT:
			move(-1, 0);
			break;
		case VK_RIGHT:
			move(1, 0);
			break;
		case VK_DOWN:
			move(0, 1);
			//move(0, 1);
			break;
		case VK_UP:
			move(0, -1);
			break;
		case VK_SPACE:
			shot();
			break;
		}
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rectView);
		render(hdc);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
