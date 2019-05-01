// WindowsWork3-2.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//
#include "stdafx.h"
#include "WindowsWork3-2.h"

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
    LoadStringW(hInstance, IDC_WINDOWSWORK32, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSWORK32));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSWORK32));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSWORK32);
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

int remainBlockCount = 20;
int firstHitBlockCount = 0;
int blockWidth = 100, blockHeight = 40;
class Block {
public:
	Pos pos;
	COLORREF initColor = RGB(200, 200, 8);
	COLORREF color = initColor;
	COLORREF hitColor = RGB(0, 200, 200);
	Pos size{ 1,1 };
	bool isHit = false;

	int offX = 0;

	bool isDestory = false;

	void init() {
		isDestory = false;
		color = initColor;
	}

	Pos getBlockSize() {
		return Pos{blockWidth+size.x, blockHeight+size.y};
	}

	void move(int x) {
		offX = x;
	}

	virtual void hit() {
		if (!isHit) {
			pos.y += blockHeight;
			color = hitColor;
			isHit = true;
			firstHitBlockCount++;
		}
		else {
			isDestory = true;
			remainBlockCount--;
		}
	}

	void render(HDC hdc) {
		if (isDestory)return;
		int x = pos.x + offX,  y = pos.y;
		HBRUSH hBrush = (HBRUSH)CreateSolidBrush(color);
		HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Rectangle(hdc, x, y, x + blockWidth+ size.x, y + blockHeight+ size.y);
		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);
	}
};

bool IsPointInCircle(int x, int y, int r, int px, int py) {
	int dx = x - px;
	int dy = y - py;
	int l = dx * dx + dy * dy;
	if (l > r*r)
		return false;
	return true;
}

// 원기준으로 사각형방향 반환
Pos CollCircleRect(int x, int y, int r, RECT* rt) {
	if ((rt->left <= x && x <= rt->right) ||
		(rt->top <= y && y <= rt->bottom)) {
		RECT rcEx = {
			rt->left - r,
			rt->top - r,
			rt->right + r,
			rt->bottom + r
		};

		if (rcEx.left < x && x < rcEx.right && rcEx.top < y && y < rcEx.bottom) {
			if (rt->left > x) return Pos<>(1, 0);
			if (x > rt->right) return Pos<>(-1, 0);
			if (rt->top > y) return Pos<>(0, 1);
			if (y > rt->bottom) return Pos<>(0, -1);
		}
	}
	else {
		if (IsPointInCircle(x, y, r, rt->left, rt->top))return Pos<>(1, 1);
		if (IsPointInCircle(x, y, r, rt->left, rt->bottom))return Pos<>(1, -1);
		if (IsPointInCircle(x, y, r, rt->right, rt->top))return Pos<>(-1, 1);
		if (IsPointInCircle(x, y, r, rt->right, rt->bottom))return Pos<>(-1, -1);
	}
	return Pos<>(0,0);
}

Block blocks[2 * 10];

class LowBar : public Block {
public:
	bool dragStart = false;

	void init() {
		pos = Pos<>(400, 600);
		size = Pos<>(120, -10);
		color = RGB(250, 0, 0);
	}

	// 마우스가 올라가있다면 true 반환
	bool IsMouseOver(int mouseX, int mouseY) {
		if (mouseX < pos.x || pos.x + blockWidth + size.x < mouseX ||
			mouseY < pos.y || pos.y + blockHeight + size.y < mouseY)
			return false;
		return true;
	}

	virtual void hit(){
		
	}

	void drag(int mouseX, int mouseY) {
		if (!dragStart) {
			if (IsMouseOver(mouseX, mouseY))
				dragStart = true;
		}
		else {
			pos.x = mouseX - (blockWidth + size.x)*.5f;
			pos.x = pos.x < 0 ? 0 : pos.x;
			pos.x = pos.x > 800 ? 800 : pos.x;
		}
	}
};

LowBar lowBar;

class Ball {
public:
	Pos pos{400,400};
	int width = 30;
	COLORREF color = RGB(200, 8, 200);

	int moveSpeed = 8;
	Pos direc{ 1,1 };

	void init() {
		pos = Pos<>(400, 400);
	}

	void update() {
		move();
	}

	// hit direc
	bool checkCollBlock(Pos<>& a, Block& b) {
		if (b.isDestory)
			return false;
		RECT rt{ b.pos.x, b.pos.y, b.pos.x+b.getBlockSize().x, b.pos.y+b.getBlockSize().y };
 		a = CollCircleRect(pos.x, pos.y, width, &rt);
		if (a.x != 0 || a.y != 0) {
			b.hit();
			return true;
		}
		return false;
	}

	bool checkCollAndHit() {
		bool isHit = false;
		Pos a;
		if (pos.x < 0 || pos.x > blockWidth*10) {
			isHit = true;
			a.x = 1;
		}
		if (pos.y < 0 || pos.y > 600) {
			isHit = true;
			a.y = 1;
		}
		if(!isHit)
			for (size_t i = 0; i < 20; i++) {
				isHit = checkCollBlock(a, blocks[i]);
				if(isHit)
					break;
		}
		if(!isHit)
			isHit = checkCollBlock(a, lowBar);
		if (isHit) {
			// 방향전환하는부분
			if (a.x != 0)
				direc.x = -direc.x;
			 if (a.y != 0)
				direc.y = -direc.y;
			return true;
		}
		return false;
	}

	void move() {
		checkCollAndHit();
		pos += direc * moveSpeed;
	}

	void render(HDC hdc) {
		int x = pos.x, y = pos.y;
		renderCircle(hdc, x, y, width, color);
	}
};


Ball ball;

void init() {
	remainBlockCount = 20;
	for (size_t i = 0; i < 2; i++)
	{
		for (size_t j = 0; j < 10; j++)
		{
			blocks[j + i * j].init();
			blocks[j + i* 10].pos.y = blockHeight * i + 30;
			blocks[j + i * 10].pos.x = blockWidth * j;
		}
	}
	ball.init();
	lowBar.init();
}

bool ldrag;
int mouseX, mouseY;

int playerDelayTime = 30;
int _playerDelayTime = playerDelayTime;

int moveOff = 1;
void update(HWND hWnd) {
	
	ball.move();
	

	if (_playerDelayTime > playerDelayTime) {
		moveOff = -moveOff;
		_playerDelayTime = 0;
		for (size_t i = 0; i < 20; i++)
		{
			blocks[i].move(moveOff *30);
		}
	}
	_playerDelayTime++;

	if (ldrag)
		lowBar.drag(mouseX, mouseY);
	else
		lowBar.dragStart = false;

	InvalidateRect(hWnd, NULL, true);
}

int exitCount = 0;
void render(HDC hdc) {
	for (size_t i = 0; i < 20; i++)
	{
		blocks[i].render(hdc);
	}
	ball.render(hdc);
	lowBar.render(hdc);
	
	wstringstream ss;
	ss << L"남은블럭: " << remainBlockCount << L"  한대맞은블럭: " << firstHitBlockCount;
	TextOut(hdc, 0, 0, ss.str().c_str(), ss.str().size());

	if (remainBlockCount <= 0)
	{
		exitCount++;
		if(exitCount > 60)
			PostQuitMessage(0);
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		GetClientRect(hWnd, &windowSize);
		init();
		SetTimer(hWnd, 1, 16, NULL);
		break;
	case WM_TIMER: {
		update(hWnd);
		break;
	}
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
		case '-':
			ball.moveSpeed--;
			ball.moveSpeed-- < 1 ? ball.moveSpeed = 1 : ball.moveSpeed;
			break;
		case '+':
			ball.moveSpeed++;
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
	case WM_LBUTTONDOWN:
		ldrag = true;
		break;
	case WM_LBUTTONUP:
		ldrag = false;
		break;
	case WM_MOUSEMOVE:
		mouseX = GET_X_LPARAM(lParam);
		mouseY = GET_Y_LPARAM(lParam);
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
