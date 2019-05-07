// SpecialGame1.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "SpecialGame1.h"
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
    LoadStringW(hInstance, IDC_SPECIALGAME1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPECIALGAME1));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPECIALGAME1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SPECIALGAME1);
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

class Obj;

class ObjM {
	friend Obj;
	vector<Obj*> objs;
	
	void addObj (Obj* obj) {
		objs.push_back (obj);
	}
public:
	static ObjM self;
	void update ();
	void render(HDC hdc);
};

ObjM ObjM::self;

class Obj {
public:
	Pos<float> p;
	int size = 32;
	int h_size = 16;
	COLORREF color = RGB(20, 20, 20);

	Obj () {
		ObjM::self.addObj (this);
	}
	virtual void update () {}
	virtual void render(HDC hdc){
		renderRect(hdc, (int)p.x, (int)p.y, size, size, RGB(20, 20, 20));
	}
};

void ObjM::update () {
	MTimer::update(12);
	for (size_t i = 0; i < objs.size (); i++) {
		objs[i]->update ();
	}
}

void ObjM::render(HDC hdc) {
	for (size_t i = 0; i < objs.size(); i++) {
		objs[i]->render(hdc);
	}
}

class StaticObj :public Obj{
public:
	virtual void render (HDC hdc) {
	}
};

class MovableObj :public Obj {
public:
	float speed = 3;
	virtual void move () {

	}
	virtual void render (HDC hdc) {
	}
};


class Block : public StaticObj {
public:
	virtual void render(HDC hdc) {

	}
};

class Bullet :public MovableObj {
public:
	Pos<float> direc;

	Bullet() : MovableObj(){
		speed = 8;
	}

	virtual void update() {
		p += direc*speed;
	}

	virtual void render(HDC hdc) {
		renderCircle(hdc, p.x, p.y);
	}
};

class Enemy : public MovableObj {
public:
	Pos<> move_vec;

};

class Player :public MovableObj {
public:
	int speed = 3;
	vector<Bullet*> bullet;
	Pos<float> move_vec;
	Pos<float> last_vec;
	float bulletDelay = 60;
	int bulletDelayIdx = 0;

	Player() : MovableObj() {
		// 기본사이즈 32라서 2만큼 더 줄여서 잘보이게 만듬
		size = size - 2;
		MTimer::create(bulletDelay, bulletDelayIdx);
	}

	void inputUpdate () {
		move_vec.set(0, 0);
		if(KeyM::keys[0])
			move_vec += Pos<float>(0, -1);
		if (KeyM::keys[1])
			move_vec += Pos<float> (-1, 0);
		if (KeyM::keys[2])
			move_vec += Pos<float> (0, 1);
		if (KeyM::keys[3])
			move_vec += Pos<float> (1, 0);
		if (KeyM::keys[4]) {
			if (MTimer::isEnd(bulletDelayIdx)) {
				MTimer::create(bulletDelay, bulletDelayIdx, false, false);
				shot();
			}
		}
		if (move_vec.squareLength() > 0) {
			move_vec = move_vec.normalize();
			last_vec = move_vec;
			move_vec *= speed;
		}
	}

	void shot() {
		bullet.push_back(new Bullet());
		bullet.back()->direc = last_vec;
	}

	void updateBullet() {
		for (size_t i = 0; i < bullet.size(); i++) {
			bullet[i]->update();
		}
	}

	virtual void update () {
		inputUpdate();
		//updateBullet();
		move(move_vec.x, move_vec.y);
	}

	virtual void move (float x, float y) {
		Pos<float> t (x, y);
		p += t;
	}

	virtual void render (HDC hdc) {
		renderRoundRect (hdc, p.x, p.y, size, size, 3, 3, RGB (20, 20, 20));
	}
};

Player player;
WindowM win;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		win.init(hWnd);
		SetTimer(hWnd, 0, 12, NULL);
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
	case WM_KEYDOWN:
		KeyM::keyFunc(wParam, true);
		break;
	case WM_KEYUP:
		KeyM::keyFunc(wParam, false);
		break;
	case WM_TIMER:
		ObjM::self.update ();
		InvalidateRect(hWnd, NULL, false);
		break;
	case WM_CHAR:
		//player.input (wParam);
		break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			HDC h = win.prerender(hdc);
			ObjM::self.render(h);
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
