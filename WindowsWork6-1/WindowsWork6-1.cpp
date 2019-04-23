// WindowsWork6-1.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include <atlimage.h>
#include "framework.h"
//#include "WindowsWork6-1.h"
#include "../MyMain.h"

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
    LoadStringW(hInstance, IDC_WINDOWSWORK61, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSWORK61));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSWORK61));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSWORK61);
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


WindowM win1;
WindowM win2;

class Sprite {
	int size = 10;
public:
	Pos p;
	COLORREF color = RGB(200,200,200);
	/*
	0 : circle
	1 : rect
	2 : tri
	*/
	int shape = 0;

	Pos direc = { 3,3 };

	WindowM* parWin;

	void init(WindowM* win) {
		parWin = win;
	}

	void move() {
		p += direc;

		Pos winSize = parWin->getSize();
		if (p.x < 0 || p.x > winSize.x - size) direc.x = -direc.x;
		if (p.y < 0 || p.y > winSize.y - size) direc.y = -direc.y;
	}

	void changeColor(int idx) {
		switch (idx) {
		case 0:
			color = RGB(200, 0, 200);
			break;
		case 1:
			color = RGB(0, 200, 200);
			break;
		case 2:
			color = RGB(200, 200, 0);
			break;
		}
	}

	void render(HDC hdc) {
		switch (shape) {
		case 0:
			renderCircle(hdc, p.x, p.y, size, color);
			break;
		case 1:
			renderRect(hdc, p.x, p.y, size, size, color);
			break;
		case 2:
			renderTriangle(hdc, p.x, p.y, size, color);
			break;
		}
	}
};

Sprite pSprite;
Sprite dSprite;

void render(HDC _hdc) {
	HDC hdc = win1.prerender(_hdc);
	pSprite.render(hdc);
	win1.postrender();
}

void d_render(HDC _hdc) {
	HDC hdc = win2.prerender(_hdc);
	dSprite.render(hdc);
	win2.postrender();
}

bool pMove = true, dMove = true;

void update() {
	if(pMove)
		pSprite.move();
	win1.clearWindow();
}

void d_update() {
	if(dMove)
		dSprite.move();
	win2.clearWindow();
}


void changeColor(int i) {
	pSprite.changeColor(i);
	dSprite.changeColor(i);
}

void changeShape(int i) {
	pSprite.shape = i;
	dSprite.shape = i;
}

bool isOpenedDialog = false;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		win1.init(hWnd);
		pSprite.init(&win1);
		SetTimer(hWnd, 1, 20, NULL);
		break;
	case WM_TIMER:
		update();
		if (!isOpenedDialog) {
			isOpenedDialog = true;
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, About);
		}
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                //DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
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
	static int colorCheck[3], shapeCheck[3];
	static int colorRadio, shapeRadio;
	static RECT winSize;

    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
		CheckRadioButton(hDlg, IDCIRCLE, IDTRI, IDRECT);
		CheckRadioButton(hDlg, IDNAGENTA, IDYELLOW, IDCYAN);
		win2.init(hDlg);
		dSprite.init(&win2);
		SetTimer(hDlg, 1, 20, NULL);
		changeShape(1);
		changeColor(1);
        return (INT_PTR)TRUE;
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hDlg, &ps);
		d_render(hdc);

		EndPaint(hDlg, &ps);
		break;
	}
	case WM_TIMER:
		d_update();
		break;
    case WM_COMMAND:
		switch (wParam) {
		case IDDSTART:
			dMove = true;
			break;
		case IDDSTOP:
			dMove = false;
			break;
		case IDDEXIT:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
			break;
		case IDPSTART:
			pMove = true;
			break;
		case IDPSTOP:
			pMove = false;
			break;
		case IDPEXIT:
			PostQuitMessage(0);
			break;
		case IDCIRCLE:
			shapeRadio = 0;
			changeShape(shapeRadio);
			break;
		case IDRECT:
			shapeRadio = 1;
			changeShape(shapeRadio);
			break;
		case IDTRI:
			shapeRadio = 2;
			changeShape(shapeRadio);
			break;
		case IDNAGENTA:
			colorRadio = 0;
			changeColor(0);
			break;
		case IDCYAN:
			colorRadio = 1;
			changeColor(1);
			break;
		case IDYELLOW:
			colorRadio = 2;
			changeColor(2);
			break;
		}
    }
    return (INT_PTR)FALSE;
}
