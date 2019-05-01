// WindowsWork3-4.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "WindowsWork3-4.h"

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
    LoadStringW(hInstance, IDC_WINDOWSWORK34, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSWORK34));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSWORK34));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSWORK34);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 1090, 670, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

class Circle {
public:
	Pos pos;
	int width = 30;
	COLORREF _color = RGB(40, 200, 0);
	COLORREF color = _color;

	virtual void render(HDC hdc) {
		int x = pos.x, y = pos.y;
		renderCircle(hdc, x, y, width, color);
	}
};

void gameEnd();

int collCount = 0;

int mouseX, mouseY;
bool ldrag = false;

class Player : public Circle {
public:
	bool dragStart = false;

	Player() {
		init();

	}

	void init() {
		color = RGB(255, 255, 255);
		pos.y = pos.x = 0;
	}

	void update() {
		if (ldrag) {
			if (IsPointInCircle(pos.x, pos.y, width*.5f, mouseX, mouseY))
				dragStart = true;
			if (dragStart) {
				pos.x = mouseX;
				pos.y = mouseY;

				
				if (IsPointInCircle(pos.x, pos.y, width, windowSize.right, windowSize.bottom)) {
					gameEnd();
				}
			}
		}
		else
			dragStart = false;
	}
};

Player player;

class Planet : public Circle{
public:
	Pos* parentPos = nullptr;
	int lenFromCenter = 0;
	float angle = 0;
	int spinSpeed = 0;

	COLORREF collcolor = RGB(220, 0, 0);

	bool isColl = false;

	void init() {
		if (int a = rand() % 10 < 5) {
			spinSpeed = a;
		}
		pos.x = rand() % 1000;
		pos.y = rand() % 700;
		width = rand() % 200 + 70;
		color = _color;
	}

	void coll() {
		if (!isColl) {
			collCount++;
			isColl = true;
			spinSpeed = spinSpeed > 0 ? spinSpeed : 3;
			spinSpeed = -spinSpeed;
			color = collcolor;
		}
	}

	void stop() {
		spinSpeed = 0;
	}

	void setParent(Pos *p, int len) {
		spinSpeed = spinSpeed > 0 ? spinSpeed : -4;
		parentPos = p;
		width = 18;
		lenFromCenter = (len + width)/2;
	}

	void updatePos<>() {
		angle += spinSpeed;
	}

	bool collWithPlayer() {
		return IsCircleInCircle(pos.x, pos.y, width, player.pos.x, player.pos.y, player.width);
	}

	virtual void render(HDC hdc) {
		if (parentPos != nullptr) {
			pos.y = parentPos->y + lenFromCenter * sin((angle)*PI);
			pos.x = parentPos->x + lenFromCenter * cos((angle)*PI);
		}

		if (collWithPlayer()) {
			SetROP2(hdc, R2_NOT);
			coll();
		}
		Circle::render(hdc);
		SetROP2(hdc, R2_COPYPEN);

		int a = 60;
		for (size_t i = 0; i < 3; i++)
		{
			int y = sin((angle + i * a)*PI) * width*.5f + pos.y;// +width * .5f;
			int x = cos((angle + i * a)*PI) * width*.5f + pos.x;// +width * .5f;
			int y2 = sin((angle + 180 + i * a)*PI) * width*.5f + pos.y;// +width * .5f;
			int x2 = cos((angle + 180 + i * a)*PI) * width*.5f + pos.x;// +width * .5f;
			MoveToEx(hdc, x, y, NULL);
			LineTo(hdc, x2, y2);
		}
	}
};



vector<Planet> planets(20);


void init() {
	collCount = 0;
	for (size_t i = 0; i < planets.size(); i++)
	{
		planets[i].init();
	}
	for (size_t i = 0; i < 6; i+=2)
	{
		planets[i].setParent(&planets[i+1].pos, planets[i + 1].width);
		//planets[i] 은 자식 오브젝트 0, 2, 4
	}
	player.init();
}

void gameEnd() {
	for (size_t i = 0; i < planets.size(); i++)
	{
		planets[i].stop();
	}
}

void update(HWND hWnd) {
	for (size_t i = 0; i < planets.size(); i++)
	{
		planets[i].updatePos<>();
	}
	player.update();

	InvalidateRect(hWnd, NULL, true);
}

void render(HDC hdc) {
	player.render(hdc);
	
	for (size_t i = 0; i < planets.size(); i++)
	{
		planets[i].render(hdc);
	}
	wstringstream ss;
	ss << L"충돌한 원 갯수 : " << collCount << " : " << mouseX << ","<< mouseY;
	TextOut(hdc, 10, 10, ss.str().c_str(), ss.str().size());
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE: {
		init();
		GetClientRect(hWnd, &windowSize);
		SetTimer(hWnd, 1, deltaTime, NULL);
		srand(time(NULL));
	}
		break;
	case WM_TIMER:
		update(hWnd);
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
			render(hdc);
			EndPaint(hWnd, &ps);
        }
        break;
	case WM_CHAR:
		switch (wParam) {
		case 'r':case'R':
			init();
			break;
		case 'q':case'Q':
			PostQuitMessage(0);
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		ldrag = true;
		break;
	case WM_LBUTTONUP:
		ldrag = false;
		break;
	case WM_MOUSEMOVE:
		mouseX = LOWORD(lParam);
		mouseY = HIWORD(lParam);
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
