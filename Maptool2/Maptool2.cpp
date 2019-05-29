// Maptool2.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Maptool2.h"
#include "MyMain.h"
#include "Singleton.h"
#include "KeyM.h"
#include "MyMain.cpp"
#include "KeyM.cpp"

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
    LoadStringW(hInstance, IDC_MAPTOOL2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MAPTOOL2));

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
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAPTOOL2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MAPTOOL2);
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
int gridSize = 50;
class Block;
class Player;
class ImgM : public Singleton<ImgM> {
public:
	Player *player;
	// 0 ~ 2 블럭
	// 3 Player
	vector<CImage *> imgs;
	void loadImg() {
		imgs.resize(4);
		for (size_t i = 0; i < imgs.size(); i++) {
			imgs [i] = new CImage();
			wstringstream ss;
			if(i <= 2)
				ss << L"../block" << i << L".png";
			else if(i == 3)
				ss << L"k9.jpg";
			imgs [i]->Load(ss.str().c_str());
		}
	}

	vector<Block *> blocks;

	void addBlock(Block *block) {
		blocks.push_back(block);
	}

	void removeBlock(Block *block) {

	}

	void save() {

	}

	void load() {

	}
};

class ImgObj : public Obj {
public:
	int imgIdx = 0;
	ImgObj() {
		size.set(gridSize,gridSize);
	}
	void render(HDC h);

	wstring getSaveData() {
		wstringstream ss;
		ss << p.x << L" " << p.y << L" "  << size.x << L" " << size.y << L" " << imgIdx << L" ";
		return ss.str();
	}

	void getLoadData(wstring s) {
		wstringstream ss(s);
		ss >> imgIdx;
	}
};

class Block : public ImgObj{
public:
	Block() {
		addBlockToM();
	}
	~Block() {
		removeBlockToM();
	}
	void addBlockToM() {
		ImgM::getIns().addBlock(this);
	}
	void removeBlockToM() {
		ImgM::getIns().removeBlock(this);

	}
	virtual Block *copy() {
		return NULL;
	}
};

class Block1 : public Block{
public:
	Block1() {
		imgIdx = 0;
	}
	Block *copy() {
		auto t = new Block1(*this);
		t->addBlockToM();
		t->addToM(0);
		return t;
	}
};

class Block2 : public Block{
public:
	Block2() {
				imgIdx = 1;
	}

	Block *copy() {
		auto t = new Block2(*this);
		t->addBlockToM();
		t->addToM(0);
		return t;
	}

};

class Player : public ImgObj{
public:
	Pos<float> direc;
	float gravity = .24f;
	float accelY = 0;
	float speed = 4;
	Player() {
		size.set(gridSize - 10, gridSize - 10);
		isAble = false;
	}
	void startGame(bool b) {
		p.set(0, 0);
		isAble = b;
	}
	void tick();
};

class MapReapter {
public:
	int mapIdx = 0;
	vector<CImage*> mapImgs;

	int selectImgIdx = -1;

	int x = 0;
	Pos<> winSize;
	int gridCount;

	int speed = 3;

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
		gridCount = winSize.x / gridSize;
	}

	void addImage(wstring s) {
		mapImgs.push_back(new CImage());
		mapImgs.back()->Load(s.c_str());
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
		if (mapIdx > mapImgs.size()) mapIdx = mapImgs.size();

		x = -mapIdx * winSize.x;
	}

	void moveHorizontal() {
		if (!isMove) return;
		x -= speed;
		auto tPlayer = ImgM::getIns().player;
		if (maxWidthSize()-winSize.x > x) {
			x = 0;
			tPlayer->p.x += maxWidthSize()-winSize.x;
		}

		mapIdx = -x / winSize.x;

		
		tPlayer->off.x = x;
	}

	HWND childHWnd;
	bool isGameStart = false;
	Block *hoverBlock = NULL;
			POINT ptMouse;

	void renderGrid(HDC h) {
		if (isGameStart)return;
		// y 
		for (size_t i = 1; i <= gridCount; i++) {
			MoveToEx(h, 0, i *gridSize, NULL);
			LineTo(h, winSize.x, i * gridSize);
			
		}
		// x
		for (size_t i = 1; i <= gridCount; i++) {
			MoveToEx(h, i*gridSize, 0, NULL);
			LineTo(h, i*gridSize, winSize.y);
		}

		
		
		//if (selectImgIdx > -1) {
		//	//ImgM::getIns().imgs [selectImgIdx]->StretchBlt(h, ptMouse.x, ptMouse.y, gridSize, gridSize, SRCCOPY);
		//	if (hoverBlock) {
		//		hoverBlock->p.set(ptMouse.x, ptMouse.y);
		//	}
		//}
	}

	void clickAndDropHoverBlock() {

		if (!hoverBlock) return;
		if (ptMouse.x < 0 || ptMouse.x > winSize.x ||
			ptMouse.y < 0 || ptMouse.y > winSize.y) return;

				DBOUT(ptMouse.x << L" " << ptMouse.y<< L"\n");
				auto t_blocks = ImgM::getIns().blocks;
				int is겹치다 = false;
		for (size_t i = 0; i < t_blocks.size()
; i++) {
			if (t_blocks [i]->p == hoverBlock->p) {
				is겹치다 += 1;
			}
		}
		if (is겹치다 <= 1) {
			auto t = hoverBlock->copy();
			
		}
	}

	void selectedBlock(int idx) {
		selectImgIdx = idx;
		if (hoverBlock) {
			delete hoverBlock;
		}
		switch (idx) {
		case 0:
			hoverBlock = new Block1();
			break;
		case 1:
			hoverBlock = new Block2();
			break;
		}
		hoverBlock->imgIdx = selectImgIdx;
	}

	void update() {

		auto t_blocks = ImgM::getIns().blocks;
		for (size_t i = 0; i < t_blocks.size()
			; i++) {
			t_blocks [i]->off.x = x;
		}

		moveHorizontal();
		SceneM::getIns(0).tick();

		if (selectImgIdx > -1) {
			GetCursorPos(&ptMouse);
			ScreenToClient(childHWnd, &ptMouse);
			ptMouse.x = ptMouse.x / gridSize * gridSize;
			ptMouse.y = ptMouse.y / gridSize * gridSize;
		}

		if (hoverBlock) {
			hoverBlock->p.set(ptMouse.x, ptMouse.y);
			auto t = hoverBlock;
			t->p.x -= x;
			t->off.x = x;
			//DBOUT(t->p.x << L" " << t->off.x << L"\n")
		}
		if (KeyM::GetInst()->StayKeyDown(VK_LBUTTON)) {
			clickAndDropHoverBlock();
		}
	}

	int maxWidthSize() {
		return -winSize.x *(mapImgs.size()-1);
	}

	void draw(HDC h) {
		for (size_t i = 0; i < mapImgs.size(); i++) {
			int _x = mapImgs [i]->GetWidth();
			int _y = mapImgs [i]->GetHeight();
			mapImgs [i]->StretchBlt(h, x + i * winSize.x, 0, winSize.x, winSize.y, SRCCOPY);
		}
		if(mapImgs.size() > 0)
			mapImgs [0]->StretchBlt(h, x-maxWidthSize()+winSize.x, 0, winSize.x, winSize.y, SRCCOPY);
		SceneM::getIns(0).render(h);
		renderGrid(h);
	}
	void startGame(bool b) {
		ImgM::getIns().player->startGame(b);
		isMove = b;
		isGameStart = b;
	}
};

MapReapter map;

LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CREATE:
		SetTimer(hWnd, 1, deltatime, NULL);
		ImgM::getIns().loadImg();
		ImgM::getIns().player = new Player();
		map.childHWnd = hWnd;
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

		// 7, 8
		x = 640;
		hButton.push_back(CreateWindow(L"button", L"<", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 600, x, 20, 20, hWnd, (HMENU)buttonId++, hInst, NULL)); x += 20;
		hButton.push_back(CreateWindow(L"button", L">", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 600, x, 20, 20, hWnd, (HMENU)buttonId++, hInst, NULL)); x += 20;


		map.initListBox(hButton [4]);
	}
		break;
	case WM_TIMER:
		InvalidateRect(hWnd, NULL, false);
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
			{
				bool toggleStartGame = false;
				toggleStartGame = !toggleStartGame;
				map.startGame(toggleStartGame);
			}break;
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
			case 7:
				map.selectedBlock(0);
				
				//ImgM::getIns().addBlock(new Block());
				break;
			case 8:
				map.selectedBlock(1);
				//ImgM::getIns().addBlock(new Block());
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
			wstringstream ss;
			ss << L"map" << (map.mapIdx + 1);
			TextOut(hdc, 0, 0, ss.str().c_str(), ss.str().size());
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

void ImgObj::render(HDC h) {
		if (!isAble)return;
		CImage *img = ImgM::getIns().imgs[imgIdx];
		if (p.x < map.winSize.x) {
			img->StretchBlt(h, p.x + off.x, p.y + off.y, size.x, size.y, SRCCOPY);
			img->StretchBlt(h, -map.maxWidthSize()+map.winSize.x+p.x + off.x, p.y + off.y, size.x, size.y, SRCCOPY);
		} else {
			img->StretchBlt(h, p.x + off.x, p.y + off.y, size.x, size.y, SRCCOPY);

		}
	}

void Player::tick() {
		if (!isAble) return;
		direc.set(0, 0);
		if (KeyM::GetInst()->StayKeyDown(VK_LEFT)) {
			direc += Pos<float>(-1 * speed, 0);
		}if (KeyM::GetInst()->StayKeyDown(VK_RIGHT)) {
			direc += Pos<float>(1 * speed, 0);
		}if (KeyM::GetInst()->StayKeyDown(VK_UP)) {
			accelY = -5;
		}
		//if (KeyM::GetInst()->StayKeyDown(VK_DOWN)) {
		//	direc += Pos<float>(0, 1 * speed);
		//}
		accelY += gravity;
		direc.y += accelY;
		auto blocks = ImgM::getIns().blocks;
		vector<Block *> collBlocks;
		float ro = speed; // rectOff
		for (size_t i = 0; i < blocks.size(); i++) {
			URect rt[4], brt, outputRt[4];
			if (direc.x < 0) {
				rt[0] = URect(-ro, 0, ro, size.y);
			}
			if (direc.x > 0) {
				rt[1] = URect(size.x, 0, size.x+ro, size.y);
			}
			if (direc.y < 0) {
				rt[2] = URect(0, -direc.y, size.x, direc.y+2);
			}
			if (direc.y > 0) {
				rt[3] = URect(0, size.y, size.x, size.y+direc.y+2);
			}
			float mx = p.x, my = p.y;
			if (p.x > -map.maxWidthSize() + map.winSize.x) {
				mx -= (-map.maxWidthSize() + map.winSize.x);
			}
			for (size_t i = 0; i < 4; i++) {
				rt [i].m_Left += mx;
				rt [i].m_Right += mx;
				rt [i].m_Top += my;
				rt [i].m_Bottom += my;
			}
			auto t_b = blocks [i];
			
			brt = URect(t_b->p.x, t_b->p.y, t_b->p.x + t_b->size.x, t_b->p.y + t_b->size.y);

			float biggestRtSize = 1;
			int biggestRtNum = -1;
			for (size_t i = 0; i < 4; i++) {
				if (GetClippingRect(rt [i], brt, outputRt [i])) {
					if (i >= 2) {
						direc.y = 0;
						accelY = 0;
					}
					float ts = outputRt [i].GetSize();
					if (ts > biggestRtSize) {
						biggestRtNum = i;
					}
				}
			}
			if (biggestRtNum == 0) {
				if (direc.x < 0)
					direc.x = 0;
			}
			if (biggestRtNum == 1) {
				if (direc.x > 0)
					direc.x = 0;
			}

			//if (t_b->p.x < map.winSize.x) {
			//	int mx = t_b->p.x - map.maxWidthSize() + map.winSize.x;
			//	brt = URect(mx, t_b->p.y, mx + t_b->size.x, t_b->p.y + t_b->size.y);
			//}
			
			/*if(blocks[i]->collobj(this)){
				collblocks.push_back(blocks [i]);
			}*/
		}
		/*pos<float> averagecollblockpos;
		for (size_t i = 0; i < collblocks.size(); i++) {
			averagecollblockpos +=  collblocks[i]->p;
		}
		averagecollblockpos /= collblocks.size();
		if(averagecollblockpos)*/

		p += direc;
	}