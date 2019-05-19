// WindowsWork7-2.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "WindowsWork7-2.h"
#include "../MyMain.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance, int);
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
    LoadStringW(hInstance, IDC_WINDOWSWORK72, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance, nCmdShow);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSWORK72));

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

LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildWndProc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


WNDCLASSEXW wcex;
//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance, int nCmdShow)
{
    

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSWORK72));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSWORK72);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	auto t = RegisterClassExW(&wcex);

	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

	

    return t;
}

class Shape : public Obj{
public:
	Pos<> direc;

	bool isJump = false;
	bool isSizeChangeMode = false;

	bool isCircle = true;

	RECT winRect;

	int sizeOff = 1;

	Shape() :Obj() {
		color = RGB(100, 200, 100);
	}

	void move(int x, int y) {
		isAble = true;
		isSizeChangeMode = false;
		isJump = false;
		if (direc.x == 0)
			direc.x = x;
		else
			direc.x = 0;
		if (direc.y == 0)
			direc.y = y;
		else	  
			direc.y = 0;
	}

	void changeSize() {
		isAble = true;
		isSizeChangeMode = !isSizeChangeMode;
	}

	Pos<float> jumpPrevP;
	float jumpAmount;

	void jump() {
		if (isJump)return;
		isAble = true;
		isJump = true;
		jumpPrevP = p;
		jumpAmount = -20;
		p += Pos<float>(0, jumpAmount);
	}

	virtual void update() {
		if (!isAble) return;
		int x = p.x;
		int y = p.y;

		// 사이즈 바꾸기만
		if (isSizeChangeMode) {
			size += sizeOff;
			if (size < 1 || size > 100)
				sizeOff = -sizeOff;
			return;
		}

		if (isJump) {
			jumpAmount += 5;
			p += Pos<float>(0, jumpAmount);
			if (p.y > jumpPrevP.y) {
				p.y = jumpPrevP.y;
				isJump = false;
			}
			return;
		}

		if (x < winRect.left || y < winRect.top || x+size > winRect.right || y+size > winRect.bottom) {
			direc = -direc;
		}

		p += direc;
	}

	virtual void render(HDC h) {
		int x = p.x, y = p.y;
		if (isCircle) {
			renderCircle(h, x, y, size, color);
		} else {
			renderTriangle(h, x, y, size, color);
		}
	}
};

class ShapeM : public Obj {
public:
	Shape shapes [2];

	void init(RECT rt) {
		shapes [0].winRect = rt;
		shapes [1].winRect = rt;
	}

	void swap() {
		auto t = shapes [0];
		shapes [0] = shapes [1];
		shapes [1] = t;
	}

	void disable() {
		shapes [0].isAble = false;
		shapes [1].isAble = false;
	}
};

ShapeM sm;

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int buttonId;
	static vector<HWND> hButton;
    switch (message)
    {
	case WM_CREATE:
	{
		wcex.lpfnWndProc = ChildWndProc; // 차일드윈도우 프로시저
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = L"Child Window Class Name"; // 차일드윈도우 클래스 이름: 클래스 구분자

		GetClientRect(hWnd, &rectView);

		RegisterClassExW(&wcex);

		auto t1 = CreateWindowEx(WS_EX_CLIENTEDGE,
			L"Child Window Class Name", NULL,
			WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
			500 + 10, 10, 500, rectView.bottom / 2 - 1,
			hWnd, NULL, hInst, NULL);

		wcex.lpfnWndProc = ChildWndProc2; // 차일드윈도우 프로시저
		wcex.lpszClassName = L"Child Window Class Name2";
		RegisterClassExW(&wcex);


		auto t2 = CreateWindowEx(WS_EX_CLIENTEDGE,
			L"Child Window Class Name2", NULL,
			WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
			0, 10, 500, rectView.bottom / 2 - 1,
			hWnd, NULL, hInst, NULL);

		int x = 10;
		hButton.push_back(CreateWindow(L"button", L"lr", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, x, 500, 20, 20, hWnd, (HMENU)buttonId++, hInst, NULL)); x += 30;
		hButton.push_back(CreateWindow(L"button", L"ud", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, x, 500, 20, 20, hWnd, (HMENU)buttonId++, hInst, NULL)); x += 30;
		hButton.push_back(CreateWindow(L"button", L"j", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, x, 500, 20, 20, hWnd, (HMENU)buttonId++, hInst, NULL)); x += 30;
		hButton.push_back(CreateWindow(L"button", L"s", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, x, 500, 20, 20, hWnd, (HMENU)buttonId++, hInst, NULL)); x += 30;

		//4 ~ 7
		hButton.push_back(CreateWindow(L"button", L"lr", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, x, 500, 20, 20, hWnd, (HMENU)buttonId++, hInst, NULL)); x += 30;
		hButton.push_back(CreateWindow(L"button", L"ud", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, x, 500, 20, 20, hWnd, (HMENU)buttonId++, hInst, NULL)); x += 30;
		hButton.push_back(CreateWindow(L"button", L"j", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, x, 500, 20, 20, hWnd, (HMENU)buttonId++, hInst, NULL)); x += 30;
		hButton.push_back(CreateWindow(L"button", L"s", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, x, 500, 20, 20, hWnd, (HMENU)buttonId++, hInst, NULL)); x += 30;

		// 8,9
		hButton.push_back(CreateWindow(L"button", L"stop", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, x, 500, 80, 20, hWnd, (HMENU)buttonId++, hInst, NULL)); x += 80;
		hButton.push_back(CreateWindow(L"button", L"swap", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, x, 500, 80, 20, hWnd, (HMENU)buttonId++, hInst, NULL)); x += 80;

		SetTimer(hWnd, 0, 15, NULL);

		GetClientRect(t1, &rectView);
		sm.shapes [0].winRect = rectView;
		GetClientRect(t2, &rectView);
		sm.shapes [1].winRect = rectView;
		win.init(hWnd);
	}break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
			case 0:
				sm.shapes [0].move(1, 0);
				break;
			case 1:
				sm.shapes [0].move(0, 1);
				break;
            case 2:
				sm.shapes [0].jump();
				break;
            case 3:
				sm.shapes [0].changeSize();
				break;
            case 4:
				sm.shapes [1].move(1, 0);
				break;
            case 5:
				sm.shapes [1].move(0, 1);
				break;
            case 6:
				sm.shapes [1].jump();
				break;
            case 7:
				sm.shapes [1].changeSize();
				break;
            case 8:
				sm.disable();
				break;
            case 9:
				sm.swap();
				break;
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
		ObjM::self.update();
		win.clearWindow();
		break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			HDC h = win.prerender(hdc);



			win.postrender();
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

LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CREATE:
		SetTimer(hWnd, 1, deltatime, NULL);
		break;
	case WM_TIMER:
		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		sm.shapes [0].render(hdc);

		EndPaint(hWnd, &ps);
	}	break;
	default:
		 return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK ChildWndProc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CREATE:
		SetTimer(hWnd, 2, deltatime, NULL);
		break;
	case WM_TIMER:
		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		sm.shapes [1].render(hdc);

		EndPaint(hWnd, &ps);
	}	break;
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
