// WindowsWork5-2.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "WindowsWork5-2.h"

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
    LoadStringW(hInstance, IDC_WINDOWSWORK52, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSWORK52));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSWORK52));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSWORK52);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

class SlicePart;
int* one = new int(1);

class MBitmap {
public:

	HBITMAP hBitmap;
	BITMAP bmp;

	Pos pos;
	Pos size;
	Pos sliceStartPos;
	Pos renderSize;

	bool isAble = false;

	MBitmap() {}

	~MBitmap() {
		Destory();
	}

	void init(int imgID) {
		isAble = true;
		hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(imgID));
		GetObject(hBitmap, sizeof(BITMAP), &bmp);
		size.x = bmp.bmWidth;
		size.y = bmp.bmHeight;
		renderSize.set(600, 600);
		size.set(600, 600);
	}

	void Destory() {
		DeleteObject(hBitmap);
	}

	bool checkPointInThis(int x, int y) {
		RECT rt = RECT{ pos.x, pos.y, pos.x + renderSize.x, pos.y + renderSize.y };
		return collPointRect(x, y, &rt);
	}

	SlicePart* createSlicePart(int x, int y, int width, int height, int cropX, int cropY);

	virtual void render(HDC hdc) {
		if (!isAble) return;
		int x = pos.x, y = pos.y;
		int sx = size.x, sy = size.y;

		int mx = sliceStartPos.x, my = sliceStartPos.y;
		int rsX = renderSize.x, rsY = renderSize.y;

		HDC hMemDC = CreateCompatibleDC(hdc);

		SelectObject(hMemDC, hBitmap);
		StretchBlt(hdc, x, y, rsX, rsY, hMemDC, mx, my, sx, sy, SRCCOPY);

		DeleteDC(hMemDC);
	}
};

class SlicePart : public MBitmap {
public:
	MBitmap* parent;
	Pos offsetPos;
	int* posScale;

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

		HDC hMemDC = CreateCompatibleDC(hdc);
		SelectObject(hMemDC, hBitmap);
		StretchBlt(hdc, (x**posScale) + ox, (y**posScale) + oy, rsX, rsY, hMemDC, mx, my, sx, sy, SRCCOPY);

		DeleteDC(hMemDC);
	}
};

MBitmap bitmaps[2];
vector<SlicePart*> slicePart;
vector<SlicePart*> dragSlicePart;
SlicePart *clickedObj;

Pos rightStartPos{ 600,0 };


unsigned int canvasSize = 600;
unsigned int maxBlockCount = 5;
unsigned int smbc = maxBlockCount* maxBlockCount;
unsigned int slicePartCount = 3;
unsigned int slicePartSize = canvasSize/slicePartCount;
unsigned int blockCount = 4;
unsigned int blockSize;

bool ldrag, rdrag;

bool bMoveMode, bSizeMode;

void checkMouseInSubPicture(int x, int y) {
	if (ldrag) {// 왼쪽꺼 이동 좌클릭
		for (size_t i = 0; i < slicePart.size(); i++)
		{
			if (slicePart[i]->checkPointInThis(x, y)) {
				// TODO
				auto t = new SlicePart(*slicePart[i]);
				t->init();
				clickedObj = t;
				dragSlicePart.push_back(t);
				bMoveMode = true;
				clickedObj->renderSize.set(blockSize, blockSize);
				return;
			}
		}
	}
	auto trasToRightSidePos = (Pos(x, y) - rightStartPos) / blockSize;
	 // 우측 이미지 선택 이동 및 크기변환
	for (size_t i = 0; i < dragSlicePart.size(); i++)
	{
		auto t_size = dragSlicePart[i]->renderSize;
		dragSlicePart[i]->renderSize = dragSlicePart[i]->renderSize / (int)blockSize - Pos(1,1);
		if (dragSlicePart[i]->checkPointInThis(trasToRightSidePos.x, trasToRightSidePos.y)) {
			clickedObj = dragSlicePart[i];
			if (rdrag) // 우클릭
				bMoveMode = true;
			else if (ldrag) // 좌클릭
				bSizeMode = true;
		}
		dragSlicePart[i]->renderSize = t_size;
	}
}

void dragingObj(int x, int y) {
	if (!clickedObj) return;

	if (bMoveMode) {
		clickedObj->pos = Pos(x - slicePartSize * .5f, y - slicePartSize * .5f);
		clickedObj->posScale = one;
		clickedObj->offsetPos.set(0, 0);
	}
	else if (bSizeMode) {
		clickedObj->renderSize = Pos(x, y) - clickedObj->pos * blockSize - rightStartPos;
	}
}

void loseDragObj(int x, int y) {
	if (!clickedObj) return;
	auto t = (Pos(x, y) - rightStartPos) / blockSize;
	if (bMoveMode) {
		if (x < rightStartPos.x) {
			vector<SlicePart*>::iterator iter;
			for (iter = dragSlicePart.begin(); iter < dragSlicePart.end(); iter++)
			{
				if (*iter == clickedObj) {
					dragSlicePart.erase(iter);
					clickedObj = NULL;
					return;
				}
			}
		}
		clickedObj->offsetPos = rightStartPos;
		clickedObj->pos = t;
		clickedObj->posScale = (int*)&blockSize;
		bMoveMode = false;
	}
	else if (bSizeMode) {
		auto size = (t - clickedObj->pos)*blockSize + Pos(blockSize, blockSize); // 다음칸의 왼쪽위 위치가 사이트가 되게
		if (size.x < 1 || size.y < 1) size.set(blockSize, blockSize);
		clickedObj->renderSize = size;
		bSizeMode = false;
	}
	clickedObj = NULL;
}

SlicePart* MBitmap::createSlicePart(int x, int y, int width, int height, int cropX, int cropY) {
	SlicePart* t_slice = new SlicePart(*((SlicePart*)this));
	t_slice->init();
	t_slice->sliceStartPos.x = x; t_slice->sliceStartPos.y = y;
	t_slice->renderSize.x = width; t_slice->renderSize.y = height;
	t_slice->size = Pos(cropX, cropY);
	t_slice->parent = this;
	return t_slice;
}

void init() {
	blockSize = canvasSize / blockCount;

	bitmaps[0].init(IDB_BITMAP1);

	slicePart.clear();
	slicePart.resize(slicePartCount*slicePartCount);

	for (size_t y = 0; y < slicePartCount*slicePartCount; y++)
	{
		// meanless
		int my = y % slicePartCount * slicePartSize, mx = y / slicePartCount * slicePartSize;
		int mx2 = slicePartSize, my2 = slicePartSize;
		slicePart[y] = bitmaps[0].createSlicePart(mx, my, mx2, my2, slicePartSize, slicePartSize);
		slicePart[y]->pos.set(mx, my);
	}
}

void update(HWND hWnd) {
	InvalidateRect(hWnd, NULL, true);
}


void renderLines(HDC hdc) {
	//slicePartSize = canvasSize / slicePartCount;
	blockSize = canvasSize / blockCount;
	for (size_t i = 0; i < slicePartCount; i++)
	{
		MoveToEx(hdc, 0, i*slicePartSize, NULL);
		LineTo(hdc, canvasSize, i*slicePartSize);
	}
	for (size_t i = 0; i < slicePartCount; i++)
	{
		MoveToEx(hdc, i*slicePartSize, 0, NULL);
		LineTo(hdc, i*slicePartSize, canvasSize);
	}

	for (size_t i = 0; i < blockCount; i++)
	{
		MoveToEx(hdc, canvasSize+0, i*blockSize, NULL);
		LineTo(hdc, canvasSize+canvasSize, i*blockSize);
	}
	for (size_t i = 0; i < blockCount; i++)
	{
		MoveToEx(hdc, canvasSize+i*blockSize, 0, NULL);
		LineTo(hdc, canvasSize+i*blockSize, canvasSize);
	}
}

void render(HDC hdc) {
	//bitmaps[0].render(hdc);
	for (size_t i = 0; i < slicePart.size(); i++)
		slicePart[i]->render(hdc);
	for (size_t i = 0; i < dragSlicePart.size(); i++)
		dragSlicePart[i]->render(hdc);
	renderLines(hdc);
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

void changeBlockSize(int i) {
	blockCount = i;
	blockSize = canvasSize / blockCount;
	for (size_t i = 0; i < dragSlicePart.size(); i++)
	{
		dragSlicePart[i]->renderSize = Pos(blockSize, blockSize);
	}
}
//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		init();
		SetTimer(hWnd, 1, 33, NULL);
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
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			render(hdc);
            EndPaint(hWnd, &ps);
        }
        break;
	case WM_CHAR:
		switch (wParam) {
		case  '1':
			changeBlockSize(1);
			break;
		case  '2':
			changeBlockSize(2);
			break;
		case  '3':
			changeBlockSize(3);
			break;
		case  '4':
			changeBlockSize(4);
			break;
		case  '5':
			changeBlockSize(5);
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		ldrag = true;
		checkMouseInSubPicture(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_LBUTTONUP:
		ldrag = false;
		loseDragObj(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_RBUTTONDOWN:
		rdrag = true;
		checkMouseInSubPicture(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_RBUTTONUP:
		rdrag = false;
		loseDragObj(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_MOUSEMOVE:
		dragingObj(LOWORD(lParam), HIWORD(lParam));
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
