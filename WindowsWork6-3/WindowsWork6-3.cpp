// WindowsWork6-3.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "WindowsWork6-3.h"
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
    LoadStringW(hInstance, IDC_WINDOWSWORK63, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSWORK63));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSWORK63));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSWORK63);
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

class Line {
public:
	COLORREF color = 0;
	bool selectColor[4] = { 0 };
	int size = 200;
	/*
	0:sin
	1:line
	2:spring
	*/
	int type = 0;

	int offset = 0;

	void init() {
		offset = 0;
		type = 0;
		size = 200;
		color = 0;
	}

	Pos getPos(int t) {
		float bias = 1.f * Radian;
		
		switch (type) {
		case 0:
			return Pos(t, sin((t + offset) * bias) * size/2 + size);
			break;
		case 1:
		{
			int tem = 2 * t + offset;
			if (tem % (size*2) > size) {
				return Pos(t , 2*size -(tem % (size*2) - size));
			} else {
				return Pos(t, size+tem % (size*2));
			}
		}
			break;
		case 2:
			Pos a = Pos(sin((t + offset) * bias) * size + t, cos((t + offset) * bias) * size) + Pos(0,size);
			return a;
			break;
		}
	}

	void render(HDC hdc) {
		int length = 1000;
		
		COLORREF realColor;
		color = 0;
		if (selectColor[0]) color += RGB(255, 0, 0);
		if (selectColor[1]) color += RGB(0, 255, 0);
		if (selectColor[2]) color += RGB(0, 0, 255);
		realColor = color;
		if (selectColor[3]) realColor = RGB(255, 255, 255) -color;
		
		HPEN p = CreatePen(0, 3, realColor);
		HPEN old = (HPEN)SelectObject(hdc, p);

		auto t = getPos(0);
		MoveToEx(hdc, t.x, t.y, NULL);
		for (size_t i = 1; i < length; i++) {
			t = getPos(i);
			LineTo(hdc, t.x, t.y);
		}
		
		SelectObject(hdc, old); DeleteObject(p);
	}
};

Line line;

class Circle {
	int size = 10;
public:
	Pos p;
	int t = 100;

	void init() {
		t = 100;
	}

	void move() {
		if (++t > 1000) t = 0;
		p = line.getPos(t);
	}

	void render(HDC hdc) {
		renderCircle(hdc, p.x- size, p.y- size, size*2);
	}
};


Circle circle;

bool lineMove = false;
bool lineSize = false;
bool bIsRenderCircle = false;


void stop() {
	lineMove = false;
	lineSize = false;
}

void init() {
	stop();
	line.init();
	circle.init();
}



int changingSize = -1;

void update() {
	if(lineMove)
		line.offset += 2;
	if (lineSize) {
		if (line.size < 10) changingSize = 2;
		if(line.size > 300) changingSize = -2;
		line.size += changingSize;
	}
	if (bIsRenderCircle)
	circle.move();
}

void render(HDC hdc) {
	line.render(hdc);
	if(bIsRenderCircle)
	circle.render(hdc);
}

void changeColor(int i) {
	line.selectColor[i] = !line.selectColor[i];
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		win1.init(hWnd);
		SetTimer(hWnd, 0, 20, NULL);
		break;
	case WM_TIMER:
		update();
		win1.clearWindow();
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
			HDC _hdc = win1.prerender(hdc);
			render(_hdc);
			win1.postrender();
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
		CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, IDC_RADIO1);
        return (INT_PTR)TRUE;
    case WM_COMMAND:
		switch (wParam) {
		case IDC_BUTTON6:
			PostQuitMessage(0);
			EndDialog(hDlg, LOWORD(wParam));
			break;
		case IDC_RADIO1:
			line.type = 0;
			break;
		case IDC_RADIO2:
			line.type = 1;
			break;
		case IDC_RADIO3:
			line.type = 2;
			break;
		case IDC_BUTTON1:
			lineMove = true;
			break;
		case IDC_BUTTON2:
			lineSize = true;
			break;
		case IDC_BUTTON3:
			stop();
			break;
		case IDC_BUTTON4:
			init();
			break;
		case IDC_BUTTON5:
			bIsRenderCircle = true;
			break;
		case IDC_CHECK1:
			changeColor(0);
			break;
		case IDC_CHECK2:
			changeColor(1);
			break;
		case IDC_CHECK3:
			changeColor(2);
			break;
		case IDC_CHECK4:
			changeColor(3);
			break;
		}
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
