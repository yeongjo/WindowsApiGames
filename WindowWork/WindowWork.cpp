// WindowWork.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "WindowWork.h"
#include <time.h>

#define MAX_LOADSTRING 100

#define WINWIDTH 800
#define WINHEIGHT 600

void RandomSwap(HWND hwnd);

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
HWND                InitInstance(HINSTANCE, int);
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
    LoadStringW(hInstance, IDC_WINDOWWORK, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

	HWND hWnd;
    // 응용 프로그램 초기화를 수행합니다:
	hWnd = InitInstance(hInstance, nCmdShow);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWWORK));

    MSG msg;


	srand(time(NULL));
	RandomSwap(hWnd);

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWWORK));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWWORK);
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
HWND InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	   0, 300, WINWIDTH+40, WINHEIGHT+100, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return hWnd;
}

void DrawX(HDC hdc, RECT& rt) {
	int i;
	// 2y = x
	for (i = 0; i < 25; i++) // foreach 우찌씀
		TextOut(hdc, rt.left + 14 * i, rt.top + 14 * (i * .5f), _T("■"), 1);

	// y = 2x + 175
	for (i = 0; i < 25; i++) // 175 = 25/2*14
		TextOut(hdc, rt.left + 14 * i, rt.top + 175 -14 * (i * .5f), _T("■"), 1);
}

void DrawTri(HDC hdc, RECT& rt) {
	RECT trt = rt;
	trt.top += 30;
	TCHAR temp[25] = _T("■■■■■■■■■■■■■■■■■■■■■■■■");
	for (size_t j = 24; j > 0; j--)
	{
		temp[j] = '\0';
		DrawText(hdc, temp, j, &trt, DT_CENTER);
		trt.top += 7;
	}
}

void DrawDia(HDC hdc, RECT& rt) {
	RECT trt = rt;
	trt.top += (trt.bottom - trt.top)/2;
	TCHAR temp[25] = _T("■■■■■■■■■■■■■■■■■■■■■■■■");
	for (size_t j = 24; j > 0; j--)
	{
		temp[j] = '\0';
		DrawText(hdc, temp, j, &trt, DT_CENTER);
		trt.top -= 4;
	}
	trt = rt;
	trt.top += (trt.bottom - trt.top) / 2;
	memcpy(temp,_T("■■■■■■■■■■■■■■■■■■■■■■■■"), sizeof(TCHAR)*25);
	for (size_t j = 24; j > 0; j--)
	{
		temp[j] = '\0';
		DrawText(hdc, temp, j, &trt, DT_CENTER);
		trt.top += 4;
	}
}

void DrawDoubleTri(HDC hdc, RECT& rt) {
	RECT trt = rt;
	HFONT mFont, oldFont;
	mFont = CreateFont(12, 0, 900,0, FW_NORMAL, FALSE, FALSE, FALSE,
		HANGEUL_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS,
		_T("굴림체"));
	oldFont = (HFONT)SelectObject(hdc, mFont);
	TCHAR temp[25] = _T("■■■■■■■■■■■■■■■■■■■■■■■■");
	for (size_t j = 24; j > 0; j--)
	{
		temp[j] = '\0';
		TextOut(hdc, trt.left, trt.bottom, temp, j);
		trt.left += 8;
		trt.bottom -= 5;
	}
	trt = rt;
	trt.left = rt.right-14;
	memcpy(temp, _T("■■■■■■■■■■■■■■■■■■■■■■■■"), sizeof(TCHAR) * 25);
	for (size_t j = 24; j > 0; j--)
	{
		temp[j] = '\0';
		TextOut(hdc, trt.left, trt.bottom, temp, j);
		trt.left -= 8;
		trt.bottom -= 5;
	}
	SelectObject(hdc, oldFont);
	DeleteObject(mFont);
}

int rectShapeNum[4];

void RandomSwap(HWND hwnd) {
	for (size_t i = 0; i < 4; i++)
	{
		rectShapeNum[i] = rand() % 4;
	}
	InvalidateRect(hwnd, NULL, true);
}

void RandomDraw(HDC hdc, RECT& rt, int i) {

	switch (rectShapeNum[i]) {
	case 0:
		DrawX(hdc, rt);
		break;
	case 1:
		DrawTri(hdc, rt);
		break;
	case 2:
		DrawDia(hdc, rt);
		break;
	case 3:
		DrawDoubleTri(hdc, rt);
		break;
	}
}

void DrawRect(HDC hdc, RECT& rt) {
	Rectangle(hdc, rt.left, rt.top, rt.right, rt.bottom);
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
	RECT rt1 = { 0, 0, WINWIDTH*.5f, WINHEIGHT*.5f };
	RECT rt2 = { WINWIDTH*.5f, 0, WINWIDTH, WINHEIGHT*.5f };
	RECT rt3 = { 0, WINHEIGHT*.5f, WINWIDTH*.5f, WINHEIGHT};
	RECT rt4 = { WINWIDTH*.5f, WINHEIGHT*.5f, WINWIDTH, WINHEIGHT };

	TCHAR temp[200] = _T("■");
	int x = 10, y = 10;

    switch (message)
    {
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
	case WM_CHAR:
		switch (wParam) {
		case 'n':case 'N':
			RandomSwap(hWnd);
			break;
		case 'q':case 'Q':
			PostQuitMessage(0);
			break;
		}
		break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
			DrawRect(hdc, rt1);
			DrawRect(hdc, rt2);
			DrawRect(hdc, rt3);
			DrawRect(hdc, rt4);
			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, RGB(10,120,30));
			RandomDraw(hdc, rt1, 0);
			RandomDraw(hdc, rt2, 1);
			RandomDraw(hdc, rt3, 2);
			RandomDraw(hdc, rt4, 3);
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
