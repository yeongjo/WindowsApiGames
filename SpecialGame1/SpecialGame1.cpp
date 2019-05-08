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


int blockSize = 32;

class Obj {
public:
	Pos<float> p;
	Pos<float> off;

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
	virtual int collObj(Obj *o) {
		if (o == nullptr) return 0;
		// 나갔을경우
		if ((p.x > o->p.x + o->size || p.x + size < o->p.x) ||
			(p.y > o->p.y + o->size || p.y + size < o->p.y))
			return 0;
		return 1;
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
	Block(Pos<float> pos) : StaticObj() {
		color = RGB(20, 20, 20);
		p = pos;
	}

	Block(float x, float y) : StaticObj() {
		color = RGB(20, 20, 20);
		p = Pos<float>(x,y);
	}

	virtual void render(HDC hdc) {
		renderRect(hdc, p.x + off.x, p.y+ off.y, size, size, color);
	}
};

class Enemy; class Player;

class WorldM {
public:
	vector<Block *> blocks;
	vector<Enemy *> enemy;
	Player *player;

	// 위쪽 ui 때문에
	int blockYOffset = 64;

	WorldM() {
		MTimer::create(2000, 1, true, false);
	}

	// 맵 읽어와서 만듬
	void createBlocks(int level) {
		stringstream ss;
		int yLine = 0;
		ifstream is;

		switch (level) {
		case 0:
			break;
		case 1:
		{
			is.open("map1.txt");
			break;
		}
		case 2:
			is.open("map2.txt");
			break;
		}

		if (is.is_open()) {
			string s;
			while (getline(is, s)) {
				for (size_t i = 0; i < s.size(); i++) {
					switch (s[i]) {
					case '0':
						break;
					case '1':
						blocks.push_back(new Block(i * blockSize, blockSize * yLine + blockYOffset));
						break;
					}
				}
				++yLine;
			}
		}
	}

	void addEnemy();

	void addBlock(Pos<float> pos) {
		blocks.push_back(new Block(pos));
	}

	void update() {
		
	}
};

WorldM wm;

class Bullet :public MovableObj {
public:
	Pos<float> direc;
	int trailSize = 8;
	float trailCount = 0;
	Bullet() : MovableObj(){
		speed = 8;
		size = 10;
	}

	virtual void update() {
		p += direc*speed;
		if(trailCount < trailSize)
		trailCount += speed/trailSize;
	}

	virtual void render(HDC hdc) {
		COLORREF t_color = color;
		Pos<float> t_direc = direc * 6;
		for (size_t i = 0; i < trailCount; i++) {

			HGDIOBJ  ori = SelectObject(hdc, GetStockObject(DC_PEN));
			SetDCPenColor(hdc, t_color);

			renderRect(hdc, p.x + off.x - t_direc.x * i, p.y + off.y - t_direc.y * i, size, size, t_color);
			BYTE r = GetRValue(t_color) + 255 / trailSize;
			BYTE g = GetGValue(t_color) + 255 / trailSize;
			BYTE b = GetBValue(t_color) + 255/ trailSize;
			t_color = RGB(r, g, b);

			SelectObject(hdc, ori);
		}
	}
};


class Guy : public MovableObj {
public:
	int speed = 3;
	vector<Bullet *> bullet;
	Pos<float> move_vec;
	Pos<float> last_vec;
	float bulletDelay = 60;
	int bulletDelayIdx = 0;

	Guy() : MovableObj() {
		// 기본사이즈 32라서 2만큼 더 줄여서 잘보이게 만듬
		size = size - 10;
		color = RGB(20, 20, 20);
	}

	virtual void render(HDC hdc) {
		renderRoundRect(hdc, p.x + off.x, p.y + off.y, size, size, 3, 3, color);
	}

	void shot() {
		bullet.push_back(new Bullet());
		auto bul = bullet.back();
		bul->direc = last_vec;
		int offgenpos = size / 2 - bul->size / 2;
		bul->p = p + Pos<float>(offgenpos, offgenpos);
	}

	void updateBullet() {
		for (size_t i = 0; i < bullet.size(); i++) {
			bullet[i]->update();
		}
	}

	// 죽음 애니메이션
	virtual void die() {

	}

	// 정의해서 사용
	virtual void inputUpdate() {}

	virtual void update() {
		inputUpdate();
		//updateBullet();
		move(move_vec.x, move_vec.y);
	}

	virtual void move(float x, float y) {
		Pos<float> t(x, y);
		p += t;
	}
};


class Player :public Guy {
public:


	Player() : Guy() {
		MTimer::create(bulletDelay, bulletDelayIdx);
		p.set(0, 64);
	}

	virtual void inputUpdate () {
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
			p += move_vec; // 충돌체크를 위해 미리 가있기
		}

		if (int t = checkIsColl()) {
			switch (t) {
			case 1:
				p -= move_vec; // 원래 위치로
				move_vec.setZero();
				break;
			case 2: // 적이랑 박아서 죽음
				die();
				break;
			}
		} else {
			p -= move_vec; // 원래 위치로
		}
	}

	// 0: not coll
	// 1: coll wall
	// 2: coll enemy;
	int checkIsColl() {
		for (size_t i = 0; i < wm.blocks.size(); i++) {
			if (collObj(wm.blocks[i])) return 1; // 벽이랑 박음
		}
		for (size_t i = 0; i < wm.enemy.size(); i++) {
			if (collObj((Obj*)wm.enemy[i])) return 2; // 적 박음
		}
	}

	virtual void render (HDC hdc) {
		renderRoundRect (hdc, p.x + off.x, p.y + off.y, size, size, 3, 3, RGB (20, 20, 20));
	}
};

WindowM win;

class UI {
public:
	int combe;
	int score;
	int level;
	COLORREF color;

	UI() {
		color = RGB(50, 50, 50);
	}

	virtual void render(HDC hdc) {
		Pos<> size = win.getSize();
		renderRect(hdc, 0, 0, size.x, 64, color);
		wstringstream wss;
		wss << score;
		TextOut(hdc, 20, 3,_T("SCORE"), 5);
		TextOut(hdc, 20, 18, wss.str().c_str(), wss.str().size());

		wss.str() = L"";
		wss.clear();
		wss << L"+" << combe;
		TextOut(hdc, 100, 3, wss.str().c_str(), wss.str().size());

		wss.str() = L"";
		wss.clear();
		wss << level;
		TextOut(hdc, size.x-50, 3, wss.str().c_str(), wss.str().size());
	}
};

Player player;

class Enemy : public Guy {
public:
	// 그냥 쓰는거임 플레이어 쫒아오게
	virtual void inputUpdate() {
		move_vec = player.p - p;
		move_vec = move_vec.normalize();
	}
};

void WorldM::addEnemy() {
	auto winSize = win.getSize();
	enemy.push_back(new Enemy());
	auto e = enemy.back();
	int x = rand() % 300;
	int y = rand() % 300;
	x = x > 150 ? x + winSize.x - 150 : x;
	y = y > 150 ? y + winSize.y - 150 : y;
	e->p.set(x, y);
}

UI ui;

void initLevel(int level) {
	wm.createBlocks(level);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		win.init(hWnd);
		SetTimer(hWnd, 0, 12, NULL);
		initLevel(1);
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
			ui.render(h);
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
