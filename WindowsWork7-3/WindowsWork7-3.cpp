// WindowsWork7-3.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "WindowsWork7-3.h"
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
    LoadStringW(hInstance, IDC_WINDOWSWORK73, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSWORK73));

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


 WNDCLASSEXW wcex;
//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
   

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSWORK73));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSWORK73);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
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


class MapReapter {
public:
	int mapIdx = 0;
	vector<CImage*> hbits;

	int x = 0;
	Pos<> winSize;

	int speed = 10;

	bool isMove = false;

	void init() {
		
		

		/*for (size_t i = 0; i < 8; i++) {
			hbits.push_back(CImage());
			ss << i;
			hbits.back().Load(ss.str().c_str());
			ss.clear();
			ss.str(L"../k");
		}*/
		winSize.set(500, 500);
	}

	void addImage(wstring s) {
		hbits.push_back(new CImage());
		hbits.back()->Load(s.c_str());
	}

	void initListBox(HWND hlist) {
		wstringstream ss(L"../k");
		for (int i = 1; i < 8; i++) {
			ss <<L"../k"<< i << L".jpg";
			SendMessage(hlist, LB_ADDSTRING, 0, (LPARAM)ss.str().c_str());
			ss.clear();
			ss.str(L"../k");
		}
	}

	void move(int i) {
		mapIdx += i;
		if (mapIdx < 0) mapIdx = 0;
		if (mapIdx > hbits.size()) mapIdx = hbits.size();

		x = -mapIdx * winSize.x;
	}

	void moveHorizontal() {
		if (!isMove) return;
		x -= speed;
		if (maxWidthSize() > x) x = 0;

		mapIdx = -x / winSize.x;
	}

	void update() {
		moveHorizontal();
	}

	int maxWidthSize() {
		return -winSize.x *(hbits.size()-1);
	}

	void draw(HDC h) {
		for (size_t i = 0; i < hbits.size(); i++) {
			int _x = hbits [i]->GetWidth();
			int _y = hbits [i]->GetHeight();
			hbits [i]->StretchBlt(h, x + i * winSize.x, 0, winSize.x, winSize.y, 0, 0, _x, _y, SRCCOPY);
		}
	}
};

MapReapter map;

LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CREATE:
		SetTimer(hWnd, 1, deltatime, NULL);
		win.init(hWnd);
		break;
	case WM_TIMER:
		map.update();
		win.clearWindow();
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		HDC h = win.prerender(hdc);
		map.draw(h);
		win.postrender();

		EndPaint(hWnd, &ps);
	}	break;
	default:
		 return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

WindowM wm;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int buttonId;
	static vector<HWND> hButton;
    switch (message)
    {
	case WM_CREATE:
	{
		SetTimer(hWnd, 0, 15, NULL);
		wm.init(hWnd);
		map.init();
		wcex.lpfnWndProc = ChildWndProc; // 차일드윈도우 프로시저
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = L"Child Window Class Name"; // 차일드윈도우 클래스 이름: 클래스 구분자

		GetClientRect(hWnd, &rectView);

		RegisterClassExW(&wcex);

		auto t1 = CreateWindowEx(WS_EX_CLIENTEDGE,
			L"Child Window Class Name", NULL,
			WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
			20, 100, 500, 500,
			hWnd, NULL, hInst, NULL);

		int x = 500;
		hButton.push_back(CreateWindow(L"button", L"select", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 600, x, 150, 20, hWnd, (HMENU)buttonId++, hInst, NULL)); x += 30;
		hButton.push_back(CreateWindow(L"button", L"move", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 600, x, 150, 20, hWnd, (HMENU)buttonId++, hInst, NULL)); x += 30;
		hButton.push_back(CreateWindow(L"button", L"finish", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 600, x, 150, 20, hWnd, (HMENU)buttonId++, hInst, NULL)); x += 30;
		hButton.push_back(CreateWindow(L"button", L"stop", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 600, x, 150, 20, hWnd, (HMENU)buttonId++, hInst, NULL)); x += 30;

		// 4
		hButton.push_back(CreateWindow(L"listbox", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_STANDARD, 600, 100, 200, 400, hWnd, (HMENU)buttonId++, hInst, NULL));

		// 5, 6
		hButton.push_back(CreateWindow(L"button", L"<", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 0, 300, 20, 20, hWnd, (HMENU)buttonId++, hInst, NULL)); x += 30;
		hButton.push_back(CreateWindow(L"button", L">", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 520, 300, 20, 20, hWnd, (HMENU)buttonId++, hInst, NULL)); x += 30;

		map.initListBox(hButton [4]);
	}
		break;
	case WM_TIMER:
		wm.clearWindow();
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
			case 0: // select
			{
				TCHAR t_text [50] = {0};
				int cursor = SendMessage(hButton [4], LB_GETCURSEL, 0, 0);
				SendMessage(hButton[4], LB_GETTEXT, cursor, (LPARAM)t_text);
				map.addImage(t_text);
			}	break;
			case 1:
				map.isMove = true;
				break;
			case 2:
				EnableWindow(hButton [0], false);
				break;
			case 3:
				map.isMove = false;
				break;
			case 5:
				map.move(-1);
				break;
			case 6:
				map.move(1);
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
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			HDC h = wm.prerender(hdc);
			wstringstream ss(L"map");
			ss << (map.mapIdx + 1);
			TextOut(h, 0, 0, ss.str().c_str(), ss.str().size());
			wm.postrender();
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
