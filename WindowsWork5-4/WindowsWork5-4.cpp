﻿// WindowsWork5-4.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "WindowsWork5-4.h"
#include <atlimage.h>

#define MAX_LOADSTRING 100

RECT rectView;

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
    LoadStringW(hInstance, IDC_WINDOWSWORK54, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSWORK54));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSWORK54));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSWORK54);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


float globalZoomLevel = 1;

class SlicePart;
int* one = new int(1);


void renderRectR2Not(HDC hdc, int x, int y, int renderSizeX, int renderSizeY) {
	SetROP2(hdc, R2_NOT);
	renderRect(hdc, x - 1, y - 1, renderSizeX + 1, renderSizeY + 1);
	SetROP2(hdc, R2_COPYPEN);
}

class MBitmap {
public:

	HBITMAP hBitmap;
	BITMAP bmp;

	CImage img;

	Pos pos;
	Pos size;
	Pos sliceStartPos;
	Pos renderSize;

	Pos splitUV;
	UINT spriteCount;
	UINT spriteIdx = 0;

	bool isAble = true;

	vector<vector<int>> animGroup;
	UINT currentGroupIdx = 0;
	UINT currentGroupSecondIdx = 0;

	UINT animFrameDelay = 3;

	int speed = 7;

	int floatingState = 0;

	MBitmap() {}

	~MBitmap() {
		Destory();
	}

	void init(const WCHAR* path, Pos _splitUV = Pos(0,0), int _spriteCount = 1) {
		img.Load(path);
		size.x = img.GetWidth();
		size.y = img.GetHeight();
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

	void setAnimGroup(vector<vector<int>>& _animGroup) {
		animGroup = _animGroup;
	}

	void Destory() {
		DeleteObject(hBitmap);
	}

	bool checkPointInThis(int x, int y) {
		RECT rt = RECT{ pos.x, pos.y, pos.x + renderSize.x, pos.y + renderSize.y };
		return collPointRect(x, y, &rt);
	}

	void changeAnimGroup(UINT i) {
		currentGroupIdx = i % animGroup.size();
	}

	void goNextSpriteIdx() {
		static UINT t;
		if (t++ < animFrameDelay) return;
		t = 0;
		currentGroupSecondIdx = (currentGroupSecondIdx + 1) % animGroup[currentGroupIdx].size();
		spriteIdx = animGroup[currentGroupIdx][currentGroupSecondIdx];
	}

	Pos floatingStartPos;
	void setFloatingState() {
		floatingState = 1;
		floatingStartPos = pos;
	}

	void updateFloatingPos() {
		if (!floatingState) return;
		static bool bIsGoOut = false;
		if (bIsGoOut && length(pos - floatingStartPos) < speed) {
			pos = floatingStartPos;
			bIsGoOut = false;
			floatingState = 0;
		}
		else {
			pos += Pos(speed, -speed);
			if (pos.x > rectView.right + renderSize.x|| pos.y < -renderSize.y) {
				bIsGoOut = true;
				int _x = -renderSize.x;
				int _y = -(_x - floatingStartPos.x) + floatingStartPos.y;
				// x 가 원하는 위치면 ㅇㅋ
				// 아니면 y를 원하는 위치로
				if (_y > rectView.bottom + renderSize.y) {
					_y = rectView.bottom + renderSize.y;
					_x = (_y - floatingStartPos.y) + floatingStartPos.x;
				}
				pos.x = _x;
				pos.y = _y;
			}
		}
	}

	void update() {
		goNextSpriteIdx();
		changeBitmapAndMove();
		updateFloatingPos();
	}

	bool canMove() {
		if (floatingState) return false;
		return true;
	}



	Pos goalPos;

	void setGoalPos() {
		goalPos = pos + Pos(random(25, 100), random(20,100));
	}

	// Lbutton
	void changeBitmapAndMove() {
		if (!canMove()) return;
		Pos t = goalPos - pos;
		int limitLen = 5;
		if (length(t) < limitLen) {
			pos = goalPos;
		}
		else {
			Pos tt = t * 0.3f;
			pos += t * 0.3f;
		}
	}

	MBitmap *createMySelf() {
		return new MBitmap(*this);
	}

	void move(int _x, int _y) {
		if (!canMove()) return;
		_x *= speed;
		_y *= speed;
		pos.x += _x;
		pos.y += _y;
	}

	SlicePart* createSlicePart(int x, int y, int width, int height, int cropX, int cropY);

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
			UINT yCoord = spriteIdx / splitUV.y;
			img.Draw(hdc, x, y, rsX, rsX,
				xCoord * nSpriteWidth, yCoord * nSpriteHeight, nSpriteWidth,
				nSpriteHeight);
		}
		else {
			img.Draw(hdc, x, y, rsX, rsX);
		}

		/*HDC hMemDC = CreateCompatibleDC(hdc);

		SelectObject(hMemDC, hBitmap);
		StretchBlt(hdc, x, y, rsX, rsY, hMemDC, mx, my, sx, sy, SRCCOPY);

		DeleteDC(hMemDC);*/
	}
};

class SlicePart : public MBitmap {
public:
	MBitmap* parent;
	Pos offsetPos;
	int* posScale;

	bool bIsFlipH, bIsFlipV;

	void init() {
		offsetPos.set(0, 0);
		posScale = one;
		isAble = true;
	}


	virtual void render(HDC hdc) {
		if (!isAble) return;
		int x = pos.x, y = pos.y;
		int sx = size.x, sy = size.y;
		int ox = offsetPos.x, oy = offsetPos.y;

		int mx = sliceStartPos.x, my = sliceStartPos.y;
		int rsX = renderSize.x, rsY = renderSize.y;

		if (ox > ads(rsX))
			ox = offsetPos.x = 0;
		else if (ox < 0)
			ox = offsetPos.x = ads(rsX);
		if (oy > ads(rsY))
			oy = offsetPos.y = 0;
		else if (oy < 0)
			oy = offsetPos.y = ads(rsY);

		HDC hMemDC = CreateCompatibleDC(hdc);
		renderRectR2Not(hdc, x - 1, y - 1, renderSize.x + 1, renderSize.y + 1);
		SelectObject(hMemDC, hBitmap);

		//renderRect(hdc, x, y, rsX, rsY, RGB(0, 0, 0));

		//StretchBlt(hdc, (x)+ox, (y)+oy, rsX, rsY, hMemDC, mx, my, sx / globalZoomLevel, sy / globalZoomLevel, SRCPAINT);
		/*int posOffX = ox < 0 ? 0 : ox;
		int sliceOffX = ox < 0 ? -ox : 0;
		int posOffY = oy < 0 ? 0 : oy;
		int sliceOffY = oy < 0 ? -oy : 0;
		ox = ads(ox);
		oy = ads(oy);
		StretchBlt(hdc, (x) +posOffX, (y) +posOffY, rsX - ox, rsY - oy, hMemDC, mx + sliceOffX, my + sliceOffY, sx / globalZoomLevel - ox, sy / globalZoomLevel - oy, SRCCOPY);*/

		renderLoopOffsetImage(hdc, x, y, rsX, rsY, hMemDC, mx, my, sx, sy, ox, oy);
		renderLoopOffsetImage(hdc, x, y, rsX, rsY, hMemDC, mx, my, sx, sy, ox - ads(rsX), oy);
		renderLoopOffsetImage(hdc, x, y, rsX, rsY, hMemDC, mx, my, sx, sy, ox, oy - ads(rsY));
		renderLoopOffsetImage(hdc, x, y, rsX, rsY, hMemDC, mx, my, sx, sy, ox - ads(rsX), oy - ads(rsY));
		//renderLoopOffsetImage(hdc, x, y, rsX, rsY, hMemDC, mx, my, sx, sy, ox + rsX, oy);

		DeleteDC(hMemDC);
	}

	void renderLoopOffsetImage(HDC hdc, int x, int y, int rsX, int rsY, HDC hMemDC, int mx, int  my, int sx, int sy, int ox, int oy) {
		int posOffX = ox < 0 ? 0 : ox;
		int posOffY = oy < 0 ? 0 : oy;
		int sliceOffX = ox < 0 ? -ox : 0;
		int sliceOffY = oy < 0 ? -oy : 0;
		ox = ads(ox);
		oy = ads(oy);

		x += posOffX;
		y += posOffY;
		rsX -= ox;
		rsY -= oy;

		int srcX = mx + sliceOffX;
		int srcY = my + sliceOffY;
		int srcW = sx / globalZoomLevel - ox;
		int srcH = sy / globalZoomLevel - oy;


		if (bIsFlipH) { // 1 좌우
			srcW = -srcW;
			srcX = (x - srcX) + pos.x + renderSize.x;
			//srcX = renderSize.x / 2 - (srcX - renderSize.x / 2); // flip
		}
		if (bIsFlipV) { // 0 세로
			srcH = -srcH;
			srcY = (y - srcY) + pos.y + renderSize.y;
		}

		StretchBlt(hdc, x, y, rsX, rsY, hMemDC, srcX, srcY, srcW, srcH, SRCCOPY);
		//renderRect(hdc, srcX, srcY, srcW, srcH, RGB(200, 0, 0));
		//StretchBlt(hdc, (x)+posOffX, (y)+posOffY, rsX - ox, rsY - oy, hMemDC, mx + sliceOffX, my + sliceOffY, sx / globalZoomLevel - ox, sy / globalZoomLevel - oy, SRCCOPY);
	}
};

SlicePart* MBitmap::createSlicePart(int x, int y, int width, int height, int cropX, int cropY) {
	SlicePart* t_slice = new SlicePart(*((SlicePart*)this));
	t_slice->init();
	t_slice->sliceStartPos.x = x; t_slice->sliceStartPos.y = y;
	t_slice->renderSize.x = width; t_slice->renderSize.y = height;
	t_slice->size = Pos(cropX, cropY);
	t_slice->parent = this;
	return t_slice;
}

vector<MBitmap> bitmaps(1);
CImage dbdc;

RECT dragRect;

bool ldrag, rdrag;
bool pasteMode;
int mouseX, mouseY;

int sizeChangeAmount = 10;
int moveChangeAmount = 20;

void draging();


void moveSprites(int x, int y) {
	for (size_t i = 0; i < bitmaps.size(); i++)
	{
		bitmaps[i].move(x, y);
	}
}

void moveMouse(int param) {
	int x = LOWORD(param);
	int y = HIWORD(param);
	mouseX = x;
	mouseY = y;
	draging();
}

void checkRightClick() {
	for (size_t i = 0; i < bitmaps.size(); i++)
	{
		if (bitmaps[i].checkPointInThis(mouseX, mouseY)) {
			bitmaps[i].setFloatingState();
		}
	}
}

int dragStartX, dragStartY;

void dragStart() {
	dragStartX = mouseX;
	dragStartY = mouseY;

	if (bitmaps[0].checkPointInThis(mouseX, mouseY)) {
		bitmaps[0].setGoalPos();
	}
}

void draging() {
	int width = mouseX - dragStartX;
	int height = mouseY - dragStartY;
	dragRect = RECT{ dragStartX , dragStartY, width, height };
}

void renderDragRect(HDC hdc) {
	if (!ldrag) return;
	//renderRectR2Not(hdc, dragRect.left, dragRect.top, dragRect.right, dragRect.bottom);
}

void dragEnd() {
	Pos start = Pos(dragStartX, dragStartY);
	Pos end = Pos(mouseX, mouseY);
	setAlign(start, end);
	Pos size = end - start;
}

void initBitmapMonster() {
	vector<vector<int>> _animGroup;
	_animGroup.resize(3);
	_animGroup[0].resize(5);
	_animGroup[1].resize(3);
	_animGroup[2].resize(5);
	int t = 0;
	for (size_t i = 0; i < _animGroup.size(); i++)
	{
		/*switch (i) {
		case 0:
			for (size_t j = 0; j < _animGroup[i].size(); j++)
			{
				_animGroup[i].push_back(j);
			}
			break;
		case 1:
			break;
		case 2:
			break;
		}*/
		for (size_t j = 0; j < _animGroup[i].size(); j++)
		{
			_animGroup[i][j] = t++;
			if (j == 2 && i == 1)
				t += 2;
		}
		
	}
	bitmaps[0].setAnimGroup(_animGroup);
}

void init() {
	dbdc.Create(rectView.right, rectView.bottom, 24, 0);
	bitmaps[0].init(_T("../mon1_sprite.png"), Pos(5,5), 15);
	bitmaps[0].animFrameDelay = 5;
	bitmaps[0].renderSize.set(100, 100);
	initBitmapMonster();
}

void zoomControl() {
	int zoomRemainTime = 60;
	static int _zoomRemainTime;
	if (zoomRemainTime < ++_zoomRemainTime) {
		_zoomRemainTime = 0;
		for (size_t i = 0; i < bitmaps.size(); i++)
		{

		}
	}
}

void render(HDC hdc) {
	HDC bDC = dbdc.GetDC();
	FillRect(bDC, &rectView, (HBRUSH)(GetStockObject(WHITE_BRUSH)));

	bitmaps[0].render(bDC);

	dbdc.Draw(hdc, 0, 0);
	dbdc.ReleaseDC();
}

void update(HWND hwnd) {
	GetClientRect(hwnd, &rectView);
	bitmaps[0].update();

	draging();
	InvalidateRect(hwnd, NULL, false);
}
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



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		GetClientRect(hWnd, &rectView);
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
	case WM_CHAR:
		switch (wParam) {
		case 'j': // jump
			globalZoomLevel = 1.2f;
			break;
		case 'e': // 확대했다 원래대로
			globalZoomLevel = 1.4f;
			break;
		case 's': // 줄어드었다 원래대로
			globalZoomLevel = 1.f;
			break;
		case 't': // 하나복제해서 만들기
			break;
		}
		break;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_UP:
			moveSprites(0, -1);
			break;
		case VK_LEFT:
			moveSprites(-1, 0);
			break;
		case VK_RIGHT:
			moveSprites(1, 0);
			break;
		case VK_DOWN:
			moveSprites(0, 1);
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		ldrag = true;
		moveMouse(lParam);
		dragStart();
		break;
	case WM_LBUTTONUP:
		ldrag = false;
		dragEnd();
		break;
	case WM_MOUSEMOVE:
		moveMouse(lParam);
		break;
	case WM_RBUTTONDOWN:
		//if (!pasteMode) break;
		moveMouse(lParam);
		checkRightClick();
		//pasteMode = false;
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
