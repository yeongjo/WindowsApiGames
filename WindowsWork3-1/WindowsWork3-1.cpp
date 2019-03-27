// WindowsWork3-1.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "WindowsWork3-1.h"
#include <sstream>
#include <vector>
#include <time.h>
using namespace std;

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
    LoadStringW(hInstance, IDC_WINDOWSWORK31, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSWORK31));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSWORK31));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSWORK31);
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


RECT windowSize;
int blockCount = 40;
int blockSize; // use for zoom
int canvasSize;

void renderLines(HDC hdc) {
	blockSize = windowSize.bottom / blockCount;
	canvasSize = blockSize * blockCount;
	for (int i = 0; i <= canvasSize; i += blockSize)
	{
		MoveToEx(hdc, i, 0, NULL);
		LineTo(hdc, i, canvasSize);
		//wstringstream ss;
		//ss << (i / blockSize);
		//TextOut(hdc, i, 0, ss.str().c_str(), ss.str().size());
	}
	for (int i = 0; i <= canvasSize; i += blockSize)
	{
		MoveToEx(hdc, 0, i, NULL);
		LineTo(hdc, canvasSize, i);
	}
}


struct Pos {
public:
	int x, y;
};

class CircleThing {
public:
	Pos pos;
	int diameter = 1;
	COLORREF color = RGB(200, 200, 200);
	bool isActive = false;

	void setColor(int r, int g, int b) {
		color = RGB(r, g, b);
	}

	// 못움직이면 false 반환
	bool canMoveTo(int x, int y) {
		
		if (pos.x + x < 0 || blockCount <= pos.x + x ||
			pos.y + y < 0 || blockCount <= pos.y + y)
			return false;
		return true;
	}

	// 못움직이면 안움직이고 false 반환.
	bool move(int x, int y) {
		if (canMoveTo(x, y)) {
			pos.x += x;
			pos.y += y;
			return true;
		}
		return false;
	}

	void render(HDC hdc) {
		// 블럭사이즈대로 적용하기
		int x = pos.x * blockSize + blockSize * .5f;
		int y = pos.y * blockSize + blockSize * .5f;
		int width = blockSize * diameter * .5f;

		HBRUSH hBrush = (HBRUSH)CreateSolidBrush(color);
		HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Ellipse(hdc, x- width, y- width, x + width, y + width);
		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);
	}
};

class Feed : public CircleThing {
public:

	// random move
	// 못움직이면 다른곳으로 움직여본다.
	void randomMove() {
		bool canMove = false;
		while (!canMove) {
			int t = rand() % 4;
			int x = 0, y = 0;
			switch (t) {
			case 0: // up
				y = -1;
				break;
			case 1: // left
				x = -1;
				break;
			case 2: // right
				x = 1;
				break;
			case 3: // down
				y = 1;
				break;
			}

			if (canMoveTo(x, y)) {
				canMove = true;
				move(x, y);
			}
		}
	}
};

// 꼬리를 가지고있다.
// 마우스로 이동방향을 바꿀수있다.
// 꼬리원을 마우스로 드래그해서 위치를 바꿀수있따.
// 키보드로 이동방향을 ㅏㅂ꿀수있ㅇ므
// 
class Player : public CircleThing {
public:

};

vector<CircleThing> circleThings;

void renderFeeds(HDC hdc) {
	for (size_t i = 0; i < circleThings.size(); i++)
	{
		circleThings[i].render(hdc);
	}
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static bool drag;
    switch (message)
    {
	case WM_CREATE:
		srand(time(NULL));
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
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			renderLines(hdc);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;
	case WM_LBUTTONDOWN:
		drag = true;
		break;
	case WM_LBUTTONUP:
		drag = true;
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
