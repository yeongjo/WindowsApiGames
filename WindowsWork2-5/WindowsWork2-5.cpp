// WindowsWork2-5.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "WindowsWork2-5.h"
#include <string>
#include <sstream>
#include <time.h>
using namespace std;

#define MAX_LOADSTRING 100

RECT windowSize;

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
    LoadStringW(hInstance, IDC_WINDOWSWORK25, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSWORK25));


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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSWORK25));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSWORK25);
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

int pressEnter = 0;
static TCHAR* savedCode = 0;;
int thicknessOffset;
static int shapeData[6];
void drawShapes(HDC hdc, const TCHAR* code) {
	wstringstream ss_check(code);
	TCHAR tch;
	ss_check >> tch;
	switch (tch) {
	case '-':
		thicknessOffset--;
		break;
	case '+':
		thicknessOffset++;
		break;
	default:
		if ('0' <= tch && tch <= '9' && pressEnter) {
			thicknessOffset = 0;
			wstringstream ss(code);
			for (size_t i = 0; i < 6; i++)
				ss >> shapeData[i];
			pressEnter = 0;
		}
		break;
	}

	int thickness = (shapeData[5] + thicknessOffset > 0) ? shapeData[5] + thicknessOffset : 1;
	HPEN hpen = CreatePen(PS_SOLID, thickness, RGB(rand() % 255, rand() % 255, rand() % 255));
	HPEN old = (HPEN)SelectObject(hdc, hpen);
	switch (shapeData[0]) {
	case 1:
		MoveToEx(hdc, shapeData[1], shapeData[2], NULL);
		LineTo(hdc, shapeData[3], shapeData[4]);
		break;
	case 2:
		Ellipse(hdc, shapeData[1], shapeData[2], shapeData[3], shapeData[4]);
		break;
	case 3:
		POINT point[3];
		point[0].x = shapeData[1];
		point[0].y = shapeData[4];
		point[1].x = shapeData[3];
		point[1].y = shapeData[4];
		point[2].x = (shapeData[1] + shapeData[3]) / 2;
		point[2].y = shapeData[2];
		Polygon(hdc, point, 3);
		break;
	case 4:
		Rectangle(hdc, shapeData[1], shapeData[2], shapeData[3], shapeData[4]);
		break;
	}
	SelectObject(hdc, old);
	DeleteObject(hpen);
}

class TextBox {
public:
	int caretX, caretY;
	wstring str;

	void init(HWND hWnd) {
		CreateCaret(hWnd, NULL, 3, 12);
		ShowCaret(hWnd);
	}
	TCHAR* input(HWND hWnd, TCHAR ch) {
		InvalidateRect(hWnd, NULL, true);
		if (ch == _T('\n')|| ch == _T('\r')) {
			TCHAR* code =new TCHAR[50];
			memcpy(code, str.c_str(), sizeof(TCHAR)*(_tcsclen(str.c_str())+1));
			str.clear();
			pressEnter++;
			return code;
		}
		else if (ch == 8) {
			if(str.size() > 0)
				str.erase(str.size() - 1, 1);
		}
		else
			str += ch;
		return 0;
	}
	void render(HDC hdc) {
		SIZE textSize;
		GetTextExtentPoint(hdc, str.c_str(), str.size(), &textSize);
		caretX = textSize.cx;
		caretY = windowSize.bottom - 15;
		SetCaretPos(caretX, caretY);
		TextOut(hdc, 0, windowSize.bottom - 15, str.c_str(), str.size());
	}
};


void printErrorMessage(HDC hdc) {
	TCHAR ch[] = _T("도형이 없습니다.");
	TextOut(hdc, 0, 0, ch, _tcsclen(ch));
}

TextBox textBox;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		GetClientRect(hWnd, &windowSize);
		textBox.init(hWnd);
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
	case WM_CHAR: {
		TCHAR* code = textBox.input(hWnd, wParam);
		 if (code)
			savedCode = code;
		break;
	}
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_LEFT:
			shapeData[1]--;
			shapeData[3]--;
			break;
		case VK_UP:
			shapeData[2]--;
			shapeData[4]--;
				break;
		case VK_RIGHT:
			shapeData[1]++;
			shapeData[3]++;
				break;
		case VK_DOWN:
			shapeData[2]++;
			shapeData[4]++;
				break;
		}
		InvalidateRect(hWnd, NULL, true);
		break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
			textBox.render(hdc);
			if (savedCode) {
				drawShapes(hdc, savedCode);
			}
			else
				printErrorMessage(hdc);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
		delete[] savedCode;
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
