// WindowsWork3-1.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "WindowsWork3-1.h"
#include <windowsx.h>
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


struct Pos;

int checkCollWithFeed(Pos pos, void* self);

int deltaTime = 33;

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

class CircleThing;
void addCircleThing(CircleThing* ct);

struct Pos {
public:
	int x = 0, y = 0;

	Pos() {}
	Pos(int x, int y) :x(x), y(y) {}

	Pos operator- (const Pos& a) {
		Pos p;
		p.x = x - a.x;
		p.y = y - a.y;
		return p;
	}
	Pos operator+ (const Pos& a) {
		Pos p;
		p.x = x + a.x;
		p.y = y + a.y;
		return p;
	}
	Pos operator/ (const int a) {
		Pos p;
		p.x = x / a;
		p.y = y / a;
		return p;
	}
	bool operator==(const Pos& a) {
		if (x == a.x && y == a.y)
			return true;
		return false;
	}
};

class CircleThing {
public:
	Pos pos;
	Pos prevPos;
	Pos jumpOff;
	int diameter = 1;
	int sizeOff = 0;
	COLORREF color = RGB(200, 200, 200);

	CircleThing() {
		addCircleThing(this);
	}

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

	int sizeOffCount = 0;
	int sizeOffZeroCount = 2; // 이정도 횟수만큼 검사해야 원래대로 돌아온다.
	void changeSizeOffZero() {
		if (sizeOff != 0) {
			if (sizeOffCount > sizeOffZeroCount) {
				sizeOff = sizeOffCount = 0;
			}
			sizeOffCount++;
		}
	}

	// 못움직이면 안움직이고 false 반환.
	virtual bool move(int x, int y) {
		changeSizeOffZero();
		if (canMoveTo(x, y)) {
			prevPos = pos + jumpOff / blockSize;
			pos.x += x;
			pos.y += y;
			return true;
		}
		return false;
	}

	virtual void render(HDC hdc) {
		// 블럭사이즈대로 적용하기
		int x = pos.x * blockSize + blockSize * .5f;
		int y = pos.y * blockSize + blockSize * .5f;
		int width = blockSize * diameter * .5f + sizeOff;

		HBRUSH hBrush = (HBRUSH)CreateSolidBrush(color);
		HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Ellipse(hdc, x- width+ jumpOff.x, y- width+ jumpOff.y, x + width+jumpOff.x, y + width+ jumpOff.y);
		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);
	}
};

class Feed : public CircleThing {
public:
	bool is2timesDisable =false;
	Feed(int x, int y):CircleThing(){
		pos.x = x;
		pos.y = y;
		setColor(10, 200, 10);
	}


	int disableCount = 0;
	// random move
	// 못움직이면 다른곳으로 움직여본다.
	void randomMove() {
		if (is2timesDisable) { // 비활성화되있다면 넘김.
			disableCount++;
			if (disableCount > 3) {
				disableCount = 0;
				is2timesDisable = false;
			}
			return;
		}
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


		if (checkCollWithFeed(pos, this) > -1) {
			is2timesDisable = true;
			sizeOff += 4;
		}
	}
};

class Tail : public CircleThing {
public:
	CircleThing* followObj;

	Tail() : CircleThing(){
		setColor(100, 10, 10);
	}
	Tail(const Tail& t) : CircleThing(t){
		setColor(100, 10, 10);
	}
	Tail(Tail&& t) : CircleThing(t) {
		setColor(100, 10, 10);
	}

	void setFollowObj(CircleThing* ct) {
		followObj = ct;
		pos = ct->prevPos;
	}

	void followMove() {
		Pos v = followObj->prevPos;// -followObj->pos;
		prevPos = pos;
		//move(v.x, v.y);
		pos = v;
	}
};




// 무작위로 돌아다닐 음식들
vector<Feed*> feeds;
vector<CircleThing*> circleThings;


void removeCircleThings(CircleThing* ct) {
	vector<CircleThing*>::iterator it = circleThings.begin();
	for (; it < circleThings.end(); it++)
	{
		if (ct == (*it)) {
			circleThings.erase(it);
			break;
		}
	}
}

// i에 있는 음식을 없앤다.
void removeFeed(int i){
	removeCircleThings(feeds[i]);
	feeds.erase(feeds.begin() + i);
}

// 음식이랑 충돌시 index 반환
// 부딫힌놈도 비활성화
// 부딫힌놈 지워버림
int checkCollWithFeed(Pos pos, void* self) {
	for (size_t i = 0; i < feeds.size(); i++)
	{
		if (feeds[i]->is2timesDisable || self == feeds[i])
			continue;
		if (feeds[i]->pos == pos || feeds[i]->prevPos == pos) {
			removeFeed(i);
			return i;
		}
	}
	return -1;
}

// 꼬리를 가지고있다.
// 마우스로 이동방향을 바꿀수있다.
// 꼬리원을 마우스로 드래그해서 위치를 바꿀수있따.
// 키보드로 이동방향을 ㅏㅂ꿀수있ㅇ므
// 
class Player : public CircleThing {
public:
	Pos direc;
	vector<Tail*> tails;

	int jumpAcc;

	Player():CircleThing(){}

	void addTail() {
		Tail* t = new Tail();
		Tail* t_last = nullptr;
		if(tails.size() > 0)
			t_last = tails[tails.size() - 1];
		tails.push_back(t);
		if (tails.size() > 0 && t_last != nullptr)
			tails[tails.size() - 1]->setFollowObj(t_last);
		else
			tails[tails.size() - 1]->setFollowObj(this);
	}

	void ChangeDirec(int x, int y) {
		direc.x = x;
		direc.y = y;
	}

	void jump() {
		jumpAcc = 3*blockSize;
	}

	void jumpUpdate() {
		if (jumpAcc > blockSize)
			jumpAcc -= blockSize;
		else
			jumpAcc = 0;

		jumpOff.x = direc.y * jumpAcc;
		jumpOff.y = direc.x * jumpAcc;
	}

	void move() {
		// 못움직이면 다른방향으로 가게하기
		if (!CircleThing::move(direc.x, direc.y)) {
			ChangeDirec(-direc.x, -direc.y);
		}
		if (int i = checkCollWithFeed(pos, this) > -1) {
			addTail();
			//removeFeed(i-1);
		}
		for (size_t i = 0; i < tails.size(); i++)
		{
			tails[i]->followMove();
		}
	}

	void render(HDC hdc) {
		CircleThing::render(hdc);
	}
};


// 안에 꼬리를 가짐
Player player;

// Must call after add
void addCircleThing(CircleThing* ct) {
	circleThings.push_back(ct);
}

void renderCircleThings(HDC hdc) {
	for (size_t i = 0; i < circleThings.size(); i++)
	{
		circleThings[i]->render(hdc);
	}
}


void genFeeds() {
	static int t_delay = 2;
	if (t_delay-- < 1) {
		feeds.push_back(new Feed(rand() % blockCount, rand() % blockCount));
		t_delay = 2;
	}
}

void moveFeeds() {
	for (size_t i = 0; i < feeds.size(); i++)
	{
		feeds[i]->randomMove();
	}
}

int ads(int a) {
	return a >= 0 ? a : -a;
}
int normalize(int a) {
	return a / ads(a);
}

void renderCircle(HDC hdc, int _x, int _y) {
	// 블럭사이즈대로 적용하기
	int x = _x * blockSize + blockSize * .5f;
	int y = _y * blockSize + blockSize * .5f;
	int width = blockSize/3;

	HBRUSH hBrush = (HBRUSH)CreateSolidBrush(RGB(255,255,0));
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	Ellipse(hdc, x - width, y - width, x + width, y + width);
	SelectObject(hdc, oldBrush);
	DeleteObject(hBrush);
}

bool drag;
int mouseX, mouseY;
int DmouseX, DmouseY;
int feedDelayTime = 3;
int _feedDelayTime = feedDelayTime;

int playerDelayTime = 3;
int _playerDelayTime = playerDelayTime;

Feed *dragingFeed = nullptr;

void update(HWND hWnd, HDC hdc) {

	// 음식먹는거
	if (_feedDelayTime > feedDelayTime) {
		_feedDelayTime = 0;
		moveFeeds();
		genFeeds();
	}
	_feedDelayTime++;

	if (_playerDelayTime > playerDelayTime) {
		_playerDelayTime = 0;
		// 플레이어 움직이기
		// 플레이어랑 충돌시 먹는거도 해야함
		player.move();
		player.jumpUpdate();
	}
	_playerDelayTime++;

	if (drag) {
		
		// 플레이어를 선택했을때
		int x = player.pos.x * blockSize + blockSize * .5f - mouseX;
		int y = player.pos.y * blockSize + blockSize * .5f - mouseY;
		int _mouseX = (mouseX) / blockSize;
		int _mouseY = (mouseY) / blockSize;
		DmouseX = _mouseX;
		DmouseY = _mouseY;
		
		// 드래그중인 음식이 있을때
		if (dragingFeed != nullptr) {
			dragingFeed->pos = Pos(_mouseX, _mouseY);
		}
		else {

			int collFeedIdx = -1;
			for (size_t i = 0; i < feeds.size(); i++)
			{
				if (feeds[i]->pos == Pos(_mouseX, _mouseY)) {
					collFeedIdx = i;
					break;
				}
			}

			if (x*x + y * y <= blockSize * blockSize * .25f) {
				player.sizeOff = 4;
				drag = false;
			}
			else if (collFeedIdx > -1) { //다른 말랑이들 선택했을때
				if (dragingFeed == nullptr)
					dragingFeed = feeds[collFeedIdx];
			}
			else {
				int direcX = _mouseX - player.pos.x;
				int direcY = _mouseY - player.pos.y;
				if (ads(direcX) > ads(direcY)) {
					if (direcX != 0) {
						player.direc.x = normalize(direcX);
						player.direc.y = 0;
					}
				}
				else
					if (direcY != 0) {
						player.direc.x = 0;
						player.direc.y = normalize(direcY);
					}
			}
		}
	}
	else
		dragingFeed = nullptr;

	InvalidateRect(hWnd, NULL, true);
}

void init() {
	player.setColor(200, 20, 20);
	player.pos.x = 1;
	player.pos.y = 1;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		srand(time(NULL));
		GetClientRect(hWnd, &windowSize);
		init();
		SetTimer(hWnd, 1, deltaTime, NULL);
		break;
	case WM_TIMER: {
		HDC hdc = GetDC(hWnd);
		update(hWnd, hdc);
		ReleaseDC(hWnd, hdc);
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
			playerDelayTime++;
			break;
		case '+':
			playerDelayTime-- < 1 ? playerDelayTime = 1 : playerDelayTime;
			break;
		case 'q':case 'Q':
			PostQuitMessage(0);
			break;
		case 'e':case 'E':
			player.sizeOff = 4;
			break;
		}
		break;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_UP:
			player.ChangeDirec(0,-1);
			break;
		case VK_LEFT:
			player.ChangeDirec(-1, 0);
			break;
		case VK_RIGHT:
			player.ChangeDirec(1, 0);
			break;
		case VK_DOWN:
			player.ChangeDirec(0, 1);
			break;
		case VK_SPACE:
			player.jump();
			break;
		}
		
		break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			renderLines(hdc);
			renderCircleThings(hdc);
			renderCircle(hdc, DmouseX, DmouseY);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;
	case WM_LBUTTONDOWN:
		drag = true;
		break;
	case WM_LBUTTONUP:
		drag = false;
		break;
	case WM_MOUSEMOVE:
		mouseX = wParam;
		mouseX = GET_X_LPARAM(lParam);
		mouseY = GET_Y_LPARAM(lParam);
		break;
    case WM_DESTROY:
		KillTimer(hWnd, 1);
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
