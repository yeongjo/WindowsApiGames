// WindowsWork5-5.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "WindowsWork5-5.h"

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
    LoadStringW(hInstance, IDC_WINDOWSWORK55, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSWORK55));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSWORK55));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSWORK55);
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

float globalZoomLevel = 1;

class DropBlock;
int* one = new int(1);


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
		img->Load(path);
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
		currentGroupIdx = i % animGroup.size();
	}

	UINT nextSpriteDelay;
	void goNextSpriteIdx() {
		if (nextSpriteDelay++ < animFrameDelay) return;
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


// 가로 5
// 세로 6
constexpr auto BlockSizeY = 6;
constexpr auto BlockSizeX = 5;
DropBlock* blocks[BlockSizeY][BlockSizeX];
int defaultSize = 79;

void initBlocks() {
	for (size_t y = 0; y < BlockSizeY; y++)
		for (size_t x = 0; x < BlockSizeX; x++)
			blocks[y][x] = nullptr;
}

class DropBlock : public MSprite {
public:

	Pos cusPos;
	int id = -1;

	virtual void init(const WCHAR* path, Pos _splitUV = Pos(0, 0), int _spriteCount = 1) {
		MSprite::init(path, _splitUV, _spriteCount);
	}

	virtual ~DropBlock() {

	}

	void init() {
		isAble = true;
	}

	DropBlock* createMySelf() {
		return new DropBlock(*this);
	}

	// ========================== RENDER ========================

	virtual void render(HDC hdc) {
		pos = cusPos * defaultSize;
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
	void move(int _x, int _y) {
		if (!isCanMove()) return;
		//_x *= speed;
		//_y *= speed;

		if (cusPos.x + _x < 0 ||
			cusPos.x + _x >= BlockSizeX ||
			blocks[cusPos.y][cusPos.x + _x]||
			bIsDroped) return;

		//blocks[cusPos.y][cusPos.x] = nullptr;
		
		cusPos.x += _x;
		if (!dropCheck())
			cusPos.y += _y;
		//blocks[cusPos.y][cusPos.x] = this;
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

	// ========================== DROP ==========================

	bool bIsDroped = false;

	// 바닥에 닿으면 참 반환
	// 이미 바닥에 닿아있는 친구들은 거짓반환
	bool dropCheck() {
		if (cusPos.y >= BlockSizeY - 1 || blocks[cusPos.y + 1][cusPos.x] != nullptr) {
			// 바닥에 닿았다.
			return bIsDroped = true;
		}
		return false;
	}

	bool dropUpdate() {
		if (bIsDroped) return true;
		// 닿았다.
		if (dropCheck()) return true;
		// 바닥에 닿지않았다면 계속 내려가기
		//blocks[cusPos.y][cusPos.x] = nullptr;
		//blocks[++cusPos.y][cusPos.x] = this;
		cusPos.y += 1;
		return false;
	}
};

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

DropBlock blockPresets[6];

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
}

DropBlock* CreatePresetBlock(int i) {
	auto t = blockPresets[i].createMySelf();
	t->cusPos.set(2, 0);
	return t;
}


DropBlock *dropingBlock = NULL;
MTimer timer;
int timerForDrop;

void dropUpate() {
	if (!dropingBlock) {
		dropingBlock = CreatePresetBlock(random(6));
	}
	else {
		if (dropingBlock->dropUpdate()) {
			// 바닥에 닿았다면
			//dropingBlock = nullptr;
			int y = dropingBlock->cusPos.y;
			int x = dropingBlock->cusPos.x;
			blocks[y][x] = dropingBlock;
			dropingBlock = CreatePresetBlock(random(6));
		}
	}
}

void rotate() {
	int id = dropingBlock->id;
	if (id < 4) {
		id = (id+1) % 4;
		dropingBlock->spriteIdx = dropingBlock->id = id;
	}
	else {
		id = !(id - 4)+4;
		dropingBlock->spriteIdx = dropingBlock->id = id;
	}
}

void move(int x, int y) {
	if (!dropingBlock) return;
	dropingBlock->move(x, y);
}

void init() {
	dbdc.Create(rectView.right, rectView.bottom, 24, 0);

	timerForDrop = timer.create(800, true);

	initBlockBitmap();
}


void render(HDC hdc) {
	HDC bDC = dbdc.GetDC();
	FillRect(bDC, &rectView, (HBRUSH)(GetStockObject(WHITE_BRUSH)));

	for (size_t y = 0; y < BlockSizeY; y++)
		for (size_t x = 0; x < BlockSizeX; x++)
			if (blocks[y][x])
				blocks[y][x]->render(bDC);

	//for (size_t i = 0; i < 6; i++)
	//{
	//	blockPresets[i].render(bDC);
	//}
	if(dropingBlock)
		dropingBlock->render(bDC);

	timer.debug(bDC, 0, 0);

	dbdc.Draw(hdc, 0, 0);
	dbdc.ReleaseDC();
}

void clearTwoBlock(int _x, int _y) {
	for (size_t y = _y; y < _y+2; y++)
	{
		for (size_t x = _x; x < _x+2; x++)
		{
			delete blocks[y][x];
			blocks[y][x] = nullptr;
		}
	}
	for (int y = _y; y >= 0; y--)
	{
		for (size_t x = _x; x < _x + 2; x++)
		{
			blocks[y][x] = blocks[y-2][x];
		}
	}
}

void clearLineAndDrop(int _y) {
	for (size_t x = 0; x < BlockSizeX; x++)
	{
		delete blocks[_y][x];
		blocks[_y][x] = nullptr;
	}
	for (size_t y = _y; y > 0; y--)
	{
		for (size_t x = 0; x < BlockSizeX; x++)
		{
			blocks[y][x] = blocks[y-1][x];
			if(blocks[y][x])
			blocks[y][x]->cusPos.y++;
		}
	}
}

void checkLineAndClearLine() {
	
	for (int y = BlockSizeY - 1; y >= 0; y--)
	{
		int checkNum = 0;
		for (size_t x = 0; x < BlockSizeX; x++)
		{
			if (blocks[y][x] && blocks[y][x]->id == 5)
				checkNum++;
		}
		if (checkNum == BlockSizeX) {
			clearLineAndDrop(y++);
		}
	}

	for (int y = BlockSizeY - 1; y > 0; y--)
	{
		for (size_t x = 0; x < BlockSizeX - 1; x++)
		{
			if ((blocks[y][x]&&
				blocks[y][x + 1]&&
				blocks[y + 1][x]&&
				blocks[y + 1][x + 1])&&(
				blocks[y][x]->id == 1&&
				blocks[y][x+1]->id == 0 &&
				blocks[y+1][x]->id == 2 && 
				blocks[y+1][x+1]->id == 3)) {
				clearTwoBlock(x, y);
			}
		}
	}
}



void update(HWND hwnd) {
	GetClientRect(hwnd, &rectView);
	timer.update(16);

	if(timer.isEnd(timerForDrop))
		dropUpate();

	checkLineAndClearLine();
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
			timer.endNext(timerForDrop);
			//move(0, 1);
			break;
		case VK_SPACE:
			rotate();
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
