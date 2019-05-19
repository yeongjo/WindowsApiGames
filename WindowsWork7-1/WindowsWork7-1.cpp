// WindowsWork7-1.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "../MyMain.h"
#include "framework.h"
#include "WindowsWork7-1.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle [MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass [MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.

	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_WINDOWSWORK71, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow)) {
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSWORK71));

	MSG msg;

	// 기본 메시지 루프입니다:
	while (GetMessage(&msg, nullptr, 0, 0)) {
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance) {
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSWORK71));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINDOWSWORK71);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd) {
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

class Shape {
public:
	Pos<> p;
	// 원 사각형 삼가형 
	int shapeType = 0;
	// 궤도 크기
	float orbitSize = 1;
	int moveSpeed = 10;
	int size = 20;
	COLORREF color;

	Shape() {
		changeColor(1);
	}

	bool checkbox [3] = {0};

	void toggleColorCheckBox(int i) {
		checkbox [i] = !checkbox [i];
		int r = checkbox [0];
		int g = checkbox [1];
		int b = checkbox [2];
		changeColor(r | g << 1 | b << 2);
	}

	void changeColor(int i) {
		int r, g, b;
		r = g = b = 0;
		if (i & 1) {
			g += 255;
			b += 255;
		}
		if (i & 2) {
			r += 255;
			b += 255;
		}if (i & 4) {
			r += 255;
			g += 255;
		}
		r =  r > 255 ? 255 : r;
		g =  g > 255 ? 255 : g;
		b =  b > 255 ? 255 : b;
		color = RGB(r, g, b);
	}

private:
	int moveAmount = 0;
public:
	void update() {
		moveAmount+= moveSpeed;
		if (moveAmount > 360) moveAmount = 0;
		getLine(moveAmount, p.x, p.y);
	}

	void getLine(float t, int &x, int &y) {
		float hs = orbitSize / 2;
		int offX = 300, offY=300;
		switch (shapeType) {
		case 0:
			//t = t * 0.001f;
			x = cos(t * PI/180) * hs * 100;
			y = sin(t * PI/180) * hs* 100;
			break;
		case 1:
		{
			int tt = 360/4; //90
			int htt = tt / 2; //45
			if (t < tt) { // -htt, -htt
				y = -htt;
				x = t - htt;
			}else if (t < tt*2) { // htt, -htt
				x = htt;
				y = (t-tt) - htt;
			} else if (t < tt * 3) { // htt, htt
				y = htt;
				x = -(t - tt * 2) + htt;
			} else { // -htt, htt
				x = -htt;
				y = -(t-tt*3) + htt;
			}
			x *= orbitSize;
		y *= orbitSize;
		}break;
		case 2:
			int tt = 360/3; //120
			int htt = tt / 2; //60
			if (t < tt) {
				x = t -htt;
				y = -htt;
			}else if (t < tt*2) {
				x = -(t-tt) + htt;
				y = (t-tt) - htt;
			}else {
				y = -(t-2*tt) +htt;
				x = -htt;
			}
			x *= orbitSize;
		y *= orbitSize;
			break;
		}
		
		x += offX;y += offY;
		
	}

	void renderLine(HDC h) {
		int x, y;
		int begin = 0;
		for (size_t i = 0; i < 360; i++) {
			getLine(i, x, y);
			if (!begin) {
				begin = 1;
				MoveToEx(h, x, y, NULL);
			}else
			LineTo(h, x, y);
		}
	}

	void render(HDC h) {
		renderLine(h);
		int x = p.x, y = p.y;
		int hs = size / 2;
		//switch (shapeType) {
		//case 0:
		renderCircle(h, x-hs, y-hs, size, color);
		//	break;
		//case 1:
		//	renderRect(h, x, y, size,size, color);
		//	break;
		//case 2:
		//	renderTriangle(h, x, y, size, color);
		//	break;
		//}
	}
};

Shape shape;

void update() {
	win.clearWindow();
	shape.update();
}

void render(HDC h) {
	shape.render(h);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	static int buttonId;
	static int up, down;
	static vector<HWND> hButton;
	switch (message) {
	case WM_CREATE:
	{
		win.init(hWnd);
		SetTimer(hWnd, 0, 15, NULL);
		int y = 10;
		hButton.push_back(CreateWindow(L"button", L"cicle", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, 10, y, 200, 20, hWnd, (HMENU)buttonId++, hInst, NULL)); y += 20;
		hButton.push_back(CreateWindow(L"button", L"rectangle", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 10, y, 200, 20, hWnd, (HMENU)buttonId++, hInst, NULL)); y += 20;
		hButton.push_back(CreateWindow(L"button", L"triangle", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 10, y, 200, 20, hWnd, (HMENU)buttonId++, hInst, NULL)); y += 20;
		hButton.push_back(CreateWindow(L"button", L"big", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, 10, y, 200, 20, hWnd, (HMENU)buttonId++, hInst, NULL)); y += 20;
		hButton.push_back(CreateWindow(L"button", L"midium", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 10, y, 200, 20, hWnd, (HMENU)buttonId++, hInst, NULL)); y += 20;
		hButton.push_back(CreateWindow(L"button", L"small", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 10, y, 200, 20, hWnd, (HMENU)buttonId++, hInst, NULL)); y += 20;

		// 6, 7
		hButton.push_back(CreateWindow(L"scrollbar", L"midium", WS_CHILD | WS_VISIBLE | SB_HORZ, 10, y, 200, 20, hWnd, (HMENU)buttonId++, hInst, NULL)); y += 20;
		SetScrollRange(hButton.back(), SB_CTL, 1, 100, TRUE); SetScrollPos(hButton.back(), SB_CTL, 0, TRUE);
		hButton.push_back(CreateWindow(L"scrollbar", L"small", WS_CHILD | WS_VISIBLE | SBS_HORZ, 10, y, 200, 20, hWnd, (HMENU)buttonId++, hInst, NULL)); y += 20;
		SetScrollRange(hButton.back(), SB_CTL, 4, 100, TRUE); SetScrollPos(hButton.back(), SB_CTL, 0, TRUE);
		// 달성률 100퍼 와근데 팀으로 망하면 재시작이네 

		hButton.push_back(CreateWindow(L"button", L"Cyan", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 10, y, 200, 20, hWnd, (HMENU)buttonId++, hInst, NULL)); y += 20;
		hButton.push_back(CreateWindow(L"button", L"Magenta", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 10, y, 200, 20, hWnd, (HMENU)buttonId++, hInst, NULL)); y += 20;
		hButton.push_back(CreateWindow(L"button", L"Yellow", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 10, y, 200, 20, hWnd, (HMENU)buttonId++, hInst, NULL)); y += 20;

		hButton.push_back(CreateWindow(L"button", L"invert", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, y, 200, 20, hWnd, (HMENU)buttonId++, hInst, NULL)); y += 20;

		// 아니 이거 컨트롤 추가하는데만 반년걸려

		break;
	}
	case WM_TIMER:
		update();
		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 메뉴 선택을 구문 분석합니다:
		switch (wmId) {
		//case BN_CLICKED:
			//switch (HIWORD(wParam)) {
		case 0:
			shape.shapeType = 0;
			break;
		case 1:
			shape.shapeType = 1;
			break;
		case 2:
			shape.shapeType = 2;
			break;
		case 3:
			shape.orbitSize = .4f;
			break;
		case 4:
			shape.orbitSize = 1.f;
			break;
		case 5:
			shape.orbitSize = 2.f;
			break;
		case 8:
			shape.toggleColorCheckBox(0);
			break;
		case 9:
			shape.toggleColorCheckBox(1);
			break;
		case 10:
			shape.toggleColorCheckBox(2);
			break;
		case 11:
			for (size_t i = 0; i < 2; i++) {
				shape.toggleColorCheckBox(i);
			}
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
	case WM_HSCROLL:
		{
			int tem;
			if ((HWND)lParam == hButton [6]) tem = up;
			if ((HWND)lParam == hButton [7]) tem = down;
			switch (LOWORD(wParam)) {
			case SB_LINELEFT: tem = max(1, tem - 1); break;
			case SB_LINERIGHT: tem = min(100, tem + 1); break;
			case SB_PAGELEFT: tem = max(1, tem - 10); break;
			case SB_PAGERIGHT: tem = min(100, tem + 10); break;
			case SB_THUMBTRACK: tem = HIWORD(wParam); break;
			}
			if ((HWND)lParam == hButton [6]) up = tem;
			if ((HWND)lParam == hButton [7])down = tem;
			SetScrollPos((HWND)lParam, SB_CTL, tem, TRUE);
			shape.moveSpeed = up;
			shape.size = down;
			//InvalidateRect(hWnd, NULL, true);
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
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);
	switch (message) {
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
