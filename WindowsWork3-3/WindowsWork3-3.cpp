﻿// WindowsWork3-3.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "WindowsWork3-3.h"
#include <queue>

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
    LoadStringW(hInstance, IDC_WINDOWSWORK33, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSWORK33));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSWORK33));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSWORK33);
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

int carWidth = 80;
int carLen = 150;

int roadOff = 200;
int roadWidth = 200;
int roadSize = roadOff * 2 + roadWidth;

class TraffiLights {
public:
	enum TraffiColor {
		red, yel, green
	};
	TraffiColor color;

	COLORREF traffiColor[3] = { RGB(220,0,0),RGB(220,220,0),RGB(0,0,220) };

	void render(HDC hdc) {
		Rectangle(hdc, 450, 20, 615, 80);
		
		for (size_t i = 0; i < 3; i++)
		{
			HBRUSH hBrush = (HBRUSH)CreateSolidBrush(traffiColor[i]);
			HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Ellipse(hdc, 455 + i * 50, 25, 455 + i * 50 + 50, 75);
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
		}
	}
};

TraffiLights traffiLight;

class Man {
public:
	void render(HDC hdc) {

	}
};


class Car {
public:
	Pos pos;
	Pos direc;
	/*1 위
	2 왼
	3 아래
	4 오른*/
	int direcIdx = 0;
	int speed = 10;

	/*
	1 = stop
	0 = going
	2 = going but will be stop
	3 = going thougth center
	*/
	int status = 0;

	void changeStatus(int i) {
		status = i;
	}

	void stopAtLine() {
		switch (direcIdx) {
		case 1:
			if (pos.y < 200 - carLen)
				pos.y = 200 - carLen;
			break;
		case 2:
			break;
		}
	}

	bool checkIsCenter() {
		if (pos.x < 200 || pos.x > 400 ||
			pos.y < 200 || pos.y > 400)
			return false;
		return true; 
	}

	void move() {
		pos += direc * speed;

		if (status == 0) {// 가고있는중에
			if (checkIsCenter()) // 중앙에 있을땐
				changeStatus(3); // 중앙을 지난다한다.
		}else
			changeStatus(2);

		if (pos.y+carLen > roadSize)
			pos.y = - carLen; // 문제
		else if(pos.y < -carLen)
			pos.y = roadSize - carLen;
		if (pos.x + carLen > roadSize)
			pos.x = -carLen;
		else if (pos.x < -carLen)
			pos.x = roadSize - carLen;
	}

	void render(HDC hdc) {
		switch (ads(direc.x) > 0) { 
		case true:// 가로로 김
			Rectangle(hdc, pos.x + roadSize, pos.y, roadSize+carLen + pos.x, carWidth + pos.y);
			Rectangle(hdc, pos.x, pos.y, carLen + pos.x, carWidth + pos.y);
			break;
		case false:
			Rectangle(hdc, pos.x, pos.y + roadSize, carWidth + pos.x, carLen + pos.y + roadSize);
			Rectangle(hdc, pos.x, pos.y, carWidth + pos.x, carLen + pos.y);
			break;
		}
	}
};

Car cars[4];
Man man;

// 먼저도착한순서대로 들어가
queue<Car*> carQueue;

// 갈수잇는 차가있으면 idx리턴
// 없으면 -1 리턴
int getFirstGoCar() {
	for (size_t i = 0; i < 4; i++)
	{
		if (cars[i].status == 3)
			return -1;
	}
	for (size_t i = 0; i < 4; i++)
	{
		if (cars[i].status == 1)
			return i;
	}
	return -1;
}

void updateCarStatus() {
	if (int i = getFirstGoCar()) {
		cars[i].changeStatus(0);
	}
}

void init() {
	// 위
	cars[0].pos = Pos(210, 0);
	cars[0].direc = Pos(0,1);
	cars[0].direcIdx = 1;
	//왼
	cars[1].pos = Pos(0, 310);
	cars[1].direc = Pos(1, 0);
	cars[1].direcIdx = 2;
	//아래
	cars[2].pos = Pos(310, 600 - carLen);
	cars[2].direc = Pos(0, -1);
	cars[2].direcIdx = 3;
	//오른
	cars[3].pos = Pos(600- carLen, 210);
	cars[3].direc = Pos(-1, 0);
	cars[3].direcIdx = 4;
}


void update(HWND hWnd) {
	for (size_t i = 0; i < 4; i++)
	{
		cars[i].move();
	}
	/*traffiLight.render(hdc);
	man.render(hdc);*/
	InvalidateRect(hWnd, NULL, true);
}

void render(HDC hdc) {

	Rectangle(hdc, 0, 0, roadSize, roadSize);
	SetROP2(hdc, R2_MASKPEN);

	for (size_t i = 0; i < 2; i++)
	{
		MoveToEx(hdc, i * roadWidth + roadOff, 0, NULL);
		LineTo(hdc, i * roadWidth + roadOff, roadOff*2+ roadWidth);
	}
	for (size_t i = 0; i < 2; i++)
	{
		MoveToEx(hdc, 0, i * roadWidth + roadOff, NULL);
		LineTo(hdc, roadOff * 2 + roadWidth, i * roadWidth + roadOff);
	}

	for (size_t i = 0; i < 4; i++)
	{
		cars[i].render(hdc);
	}
	traffiLight.render(hdc);
	man.render(hdc);

	
	//SetROP2(hdc, R2_NOP);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		init();
		SetTimer(hWnd, 1, 16, NULL);
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
	case WM_TIMER: {
		update(hWnd);
		break;
	}
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
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
