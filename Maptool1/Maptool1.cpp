// Maptool1.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Maptool1.h"
#include "MyMain.h"
#include "KeyM.h"
#include "KeyM.cpp"
#include "MyMain.cpp"

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
    LoadStringW(hInstance, IDC_MAPTOOL1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MAPTOOL1));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAPTOOL1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MAPTOOL1);
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



class Item{
public:
	bool isGameStart = false;
	vector<CImage *> *imgs;
	int imgIdx = -1;
	virtual void render(HDC h, Pos<float> &p, Pos<> &size) {
		if (imgIdx == -1) return;
		int offset = 5;
		(*imgs)[imgIdx]->StretchBlt(h, p.x+offset, p.y+offset, size.x-offset*2, size.y-offset*2, SRCCOPY);
	}

	virtual Item *copy() {
		return new Item(*this);
	}

	virtual void update(){}

	virtual bool isObstacle() { return false; }

	virtual wstring getSaveData() {
		wstringstream ss;
		ss << (imgIdx);
		return ss.str();
	}

	virtual void setData(wstring s, wstring b);
};

class Obstacle : public Item {
public:
	Item *subItem;
	~Obstacle() { delete subItem; }
	virtual Item *copy() {
		return new Obstacle(*this);
	}
	virtual bool isObstacle() { return true; }
	virtual wstring getSaveData() {
		wstringstream ss;
		ss << (imgIdx)<<L"/"<<subItem->imgIdx;
		return ss.str();
	}
	virtual void setData(wstring a, wstring b);
	virtual void render(HDC h, Pos<float> &p, Pos<> &size) {
		if (imgIdx == -1) return;
		(*imgs)[imgIdx]->StretchBlt(h, p.x, p.y, size.x, size.y, SRCCOPY);
		if(!isGameStart)
			subItem->render(h, p, size);
	}
};

class Player{
public:
	Pos<float> direc;
	float speed = 2;
	Pos<float> p;
	Pos<> size;
	void init() {
		
	}
	void tick() {
		direc.set(0, 0);
		if (KeyM::GetInst()->StayKeyDown(VK_LEFT)) {
			direc += Pos<float>(-1 * speed, 0);
		}if (KeyM::GetInst()->StayKeyDown(VK_RIGHT)) {
			direc += Pos<float>(1 * speed, 0);
		}if (KeyM::GetInst()->StayKeyDown(VK_UP)) {
			direc += Pos<float>(0, -1 * speed);
		}if (KeyM::GetInst()->StayKeyDown(VK_DOWN)) {
			direc += Pos<float>(0, 1 * speed);
		}
		p += direc;
	}
	void render(HDC h) {
		renderRoundRect(h, p.x, p.y, size.x, size.y, 3, 3, RGB(50,50,50));
	}

	void startGame(bool b);
};
HWND editWindow;

class MapSocket {
public:
	Pos<float> p;
	Pos<> *size;
	Item *item = NULL;

	// pos index
	Pos<> idx;

	MapSocket() {
		item = new Item();
	}

	// TODO 메모리 줄줄
	MapSocket(MapSocket &rhs) {
		p = rhs.p;
		size = new Pos<>(rhs.size->x, rhs.size->y);
		item = rhs.item;
	}
	MapSocket(const MapSocket &rhs) {
		p = rhs.p;
		size = new Pos<>(rhs.size->x, rhs.size->y);
		item = rhs.item;
	}

	bool checkColl(int _x, int _y) {
		RECT rt = {p.x, p.y, p.x+size->x, p.y+size->y};
		if (collPointRect(_x, _y, &rt))
			return true;
		return false;

	}



	void calculatePos() {
		p.set(idx.x * size->x, idx.y * size->y);
	}

	void render(HDC h) {
		item->render(h, p, *size);
	}
};

class PlayModeM {
public:
	vector<vector<MapSocket>> socketArr;
	CImage *mapImg;
	int mapSizeX, mapSizeY;
	bool isAble = false;
	Player *player;
	void copySocketArr(vector<vector<MapSocket>> &arr, CImage *_mapImg, int _mapSizeX, int _mapSizeY, Player *_player) {

		// 이미 전에 한번해서 있다면
		for (size_t i = 0; i < socketArr.size(); i++) {
			for (size_t j = 0; j < socketArr [i].size(); j++) {
				if(socketArr [i][j].item)
					delete socketArr [i][j].item;
				delete socketArr [i][j].size;
			}
		}

		socketArr = arr;

		for (size_t i = 0; i < socketArr.size(); i++) {
			for (size_t j = 0; j < socketArr [i].size(); j++) {
				if(arr [i][j].item)
				socketArr [i][j].item = arr [i][j].item->copy();
				socketArr [i][j].item->isGameStart = true;
			}
		}

		mapImg = _mapImg;
		mapSizeX = _mapSizeX;
		mapSizeY = _mapSizeY;
		player = _player;
	}

	void startGame(bool b) {
		isAble = b;
		player->init();
	}

	void tick() {
		for (size_t i = 0; i < socketArr.size(); i++) {
			for (size_t j = 0; j < socketArr [i].size(); j++) {
				socketArr [i][j].item->update();
			}
		}
		player->tick();
	}

	void render(HDC h) {
		mapImg->StretchBlt(h, 0, 0, mapSizeX, mapSizeY);
		for (size_t i = 0; i < socketArr.size(); i++) {
			for (size_t j = 0; j < socketArr [i].size(); j++) {
				socketArr [i][j].render(h);
			}
		}
		player->render(h);
	}

	static PlayModeM *getIns() {
		if (!ins) {
			ins = new PlayModeM();
		}
		return ins;
	}
	static PlayModeM *ins;
};

PlayModeM *PlayModeM::ins = NULL;

class Map : public Obj {
public:
	vector<vector<MapSocket>> socketArr;

	CImage *mapImg;
	int mapImgIdx = 0;
	vector<CImage*> img;

	int selectImgIdx = -1;

	int mapSizeX = 15*30;
	int mapSizeY = 9*30;

	Pos<> gridSize = Pos<>(30,30);

	int gridCountX = 15;
	int gridCountY = 8;

	Pos<> mouseCursorPos;

	Player *player;

	void startTest(bool b) {
		PlayModeM::getIns()->copySocketArr(socketArr, mapImg, mapSizeX, mapSizeY, player);
		player->startGame(b);
		PlayModeM::getIns()->startGame(b);
	}

	void setEditBox(HWND hDlg) {
		SetDlgItemInt(hDlg, IDC_EDIT1, mapSizeX, TRUE);
		SetDlgItemInt(hDlg, IDC_EDIT2, mapSizeY, TRUE);
		SetDlgItemInt(hDlg, IDC_EDIT3, gridCountX, TRUE);
		SetDlgItemInt(hDlg, IDC_EDIT4, gridCountY, TRUE);
	}

	void mouseMove(LPARAM _pos) {
		int x = LOWORD(_pos) / gridSize.x * gridSize.x;
		int y = HIWORD(_pos) / gridSize.y * gridSize.y;
		mouseCursorPos.set(x, y);
	}

	void click(LPARAM _pos);

	void setSocketArrByGridCount() {
		socketArr.resize(gridCountY);
		for (size_t i = 0; i < socketArr.size(); i++) {
			socketArr[i].resize(gridCountX);
		}
	}

	void loadImage() {
		img.resize(13);
		for (size_t i = 0; i < img.size(); i++) {
			img [i] = new CImage();
			wstringstream s;
			s << L"../k" << (i+1) << L".jpg";
			img [i]->Load(s.str().c_str());
		}
		mapImg = img [0];
	}

	void setGridCount(int x, int y) {
		if (x > 0) {
			gridCountX = x;
		}
		if (y > 0) {
			gridCountY = y;
		}
	}

	void setMapSize(int x, int y) {
		if (x > 0) {
			mapSizeX = x;
		}
		if (y > 0) {
			mapSizeY = y;
		}
	}

	void calculate() {
		gridSize.x = mapSizeX / gridCountX;
		gridSize.y = mapSizeY / gridCountY;

		for (size_t i = 0; i < socketArr.size(); i++) {
			for (size_t j = 0; j < socketArr [i].size(); j++) {
				socketArr [i][j].calculatePos();
			}
		}
	}

	void initMapSize() {
		mapSizeX = gridSize.x * gridCountX;
		mapSizeY = gridSize.y * gridCountY;
	}

	void setMapImage(int i) {
		mapImg = img [i];
		mapImgIdx = i;
	}

	void init() {
		initMapSize();
		
		int x = gridCountX, y = gridCountY;
		socketArr.resize(y);
		for (size_t i = 0; i < socketArr.size(); i++) {
			socketArr [i].resize(x);
			for (size_t j = 0; j < x; j++) {
				socketArr [i][j].idx.set(j, i);
				socketArr [i][j].size = &gridSize;
				socketArr [i][j].calculatePos();
			}
		}
		//calculate();

		loadImage();

		player = new Player();
	}

	void renderGrid(HDC h) {
		// y 
		for (size_t i = 1; i <= gridCountY; i++) {
			MoveToEx(h, 0, i * gridSize.y, NULL);
			LineTo(h, mapSizeX, i * gridSize.y);
			
		}
		// x
		for (size_t i = 1; i <= gridCountX; i++) {
			MoveToEx(h, i*gridSize.x, 0, NULL);
			LineTo(h, i*gridSize.x, mapSizeY);
		}
	}

	void render(HDC h) {
		mapImg->StretchBlt(h, 0, 0, mapSizeX, mapSizeY, SRCCOPY);

		SetROP2(h, R2_XORPEN);
		if (selectImgIdx == -1)
			renderRect(h, mouseCursorPos.x, mouseCursorPos.y, gridSize.x, gridSize.y);
		else
			img[selectImgIdx]->StretchBlt(h, mouseCursorPos.x, mouseCursorPos.y, gridSize.x, gridSize.y);
		SetROP2(h, R2_COPYPEN);
		renderGrid(h);

		for (size_t i = 0; i < socketArr.size(); i++) {
			for (size_t j = 0; j < socketArr[i].size(); j++) {
				socketArr [i][j].render(h);
			}
		}
	}

	HANDLE hFile;
	TCHAR InBuf [1000];
	LPDWORD textSize = new DWORD;
	void save() {
		hFile = CreateFile ( L"test.txt", GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, 0);
		wstringstream ss;
		ss <<mapImgIdx <<L" "<< mapSizeX << L" " << mapSizeY << L" " << gridSize.x << L" " << gridSize.y << L" " << gridCountX << L" " << gridCountY << L" ";
		for (size_t i = 0; i < socketArr.size(); i++) {
			for (size_t j = 0; j < socketArr[i].size(); j++) {
				ss << socketArr [i][j].item->getSaveData() << L" ";
			}
		}
		SetFilePointer (hFile, 0, NULL, FILE_END);
		WriteFile (hFile, ss.str().c_str(), ss.str().size()*2, textSize, NULL); // OutBuff의 내용을 hFile의 끝에 저장
		CloseHandle (hFile);
	}

	void load();

	void setButtonImage(HWND hwnd) {
		HBITMAP hbit;
		HWND hbtn;
		for (size_t i = 0; i < img.size()
			; i++) {
			hbit = (*img [i]);
			hbtn = GetDlgItem(hwnd, IDC_BUTTON1+i);
			SendMessage (hbtn, BM_SETIMAGE, 0, (LPARAM)hbit); 
		}
	}
};

LRESULT CALLBACK testWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

Map map;

class Enemy : public Item {
public:
	Pos<float> pos;
	Pos<float> direc;
	void startGame() {
		isGameStart = true;
	}
	virtual Item *copy() {
		return new Enemy(*this);
	}
	virtual void update() {	}
	virtual void render(HDC h, Pos<float> &p, Pos<> &size) {
		if (imgIdx == -1) return;
		if (!isGameStart) {
			pos = p;
		}
		(*imgs)[imgIdx]->StretchBlt(h, pos.x, pos.y, size.x, size.y, SRCCOPY);

	}
};

class Enemy1 : public Enemy {
public:
	Pos<float> initPos = Pos<float>(-1,-1);
	bool isMoveHori = true;
	virtual Item *copy() {
		return new Enemy1(*this);
	}
	virtual void update() {
		if (!isGameStart)return;
		if (initPos.x == -1) {
			initPos = pos;
		}
		if (isMoveHori) {
			if (pos.x - initPos.x < -map.gridSize.x * 2) {
				direc.set(1, 0);
			}
			if(direc.x != 1)
				direc.set(-1, 0);

			if (direc.x == 1 && ads(pos.x - initPos.x) < 2) {
				isMoveHori = false;
			}
			
		}else{
			if (pos.y - initPos.y < -map.gridSize.y * 2) {
				direc.set(0, 1);
			}
			if(direc.y != 1)
				direc.set(0, -1);
			if (direc.y == 1 && ads(pos.y - initPos.y) < 2) {
				isMoveHori = true;
			}
			
			
		}
		pos += direc;
	}
};

class Enemy2 : public Enemy {
public:
	Pos<float> initPos = Pos<float>(-1,-1);
	bool isMoveHori = true;
	virtual Item *copy() {
		return new Enemy2(*this);
	}
	virtual void update() {
		if (!isGameStart)return;
		direc = (map.player->p - pos).normalize();
		pos += direc;
	}
};

class Enemy3 : public Enemy {
public:
	Pos<float> initPos = Pos<float>(-1,-1);
	bool isMoveHori = true;
	virtual Item *copy() {
		direc.set(-1, 0);
		return new Enemy3(*this);
	}
	virtual void update() {
		if (!isGameStart)return;
		if (initPos.x == -1) {
			initPos = pos;
		}
		if (pos.x - initPos.x < -map.gridSize.x * 2) {
			direc.set(1, 0);
		}
		if(pos.x - initPos.x > map.gridSize.x * 2)
			direc.set(-1, 0);
		pos += direc;
	}
};

class Enemy4 : public Enemy2 {
public:

	class Enemy4Tail : public Enemy {
	public:
		vector<Pos<float>> prevPos;
		void t_update(Pos<float> p) {
			isGameStart = true;
			prevPos.push_back(p);
			if (prevPos.size() > 20) {
				pos = prevPos [0];
				prevPos.erase(prevPos.begin());
			}
		}
	};
	Enemy4Tail tail;

	virtual Item *copy() {
		tail.pos = pos;
		tail.imgIdx = imgIdx;
		tail.imgs = imgs;
		tail.isGameStart = isGameStart;
		return new Enemy4(*this);
	}
	virtual void render(HDC h, Pos<float> &p, Pos<> &size) {
		if(isGameStart)
			tail.render(h, p, size);
		Enemy::render(h, p, size);
	}
	virtual void update() {
		Enemy2::update();
		if(isGameStart)
			tail.t_update(pos);
	}
};

class EnemyGen {
public:

	static Enemy *getEnemy(int i) {
		switch (i) {
		case 0:
			return new Enemy1();
			break;
			case 1:
			return new Enemy2();
			break;
			case 2:
			return new Enemy3();
			break;
			case 3:
			return new Enemy4();
			break;
			default:
				assert("적만드는곳 이상한 인덱스 불림" == 0);
				break;
		}
	}
};

void Map::click(LPARAM _pos) {
	if (selectImgIdx == -1) return;
	int _x = LOWORD(_pos);
	int _y = HIWORD(_pos);
	for (size_t i = 0; i < socketArr.size(); i++) {
		for (size_t j = 0; j < socketArr[i].size(); j++) {
			if (socketArr [i][j].checkColl(_x, _y)) {
				
				// check obstacle
				Item *t = NULL;
				if (selectImgIdx <= 5) { // 장애물
					if (!socketArr [i][j].item->isObstacle()) {
						Item *t_item = socketArr [i][j].item;
						t = socketArr [i][j].item = new Obstacle();
						if (((Obstacle *)t)->subItem) {
							delete ((Obstacle *)t)->subItem;
							((Obstacle *)t)->subItem = NULL;
						}
						((Obstacle *)t)->subItem = t_item;
					} else {
						delete socketArr [i][j].item;
						t = socketArr [i][j].item = new Obstacle();
					}
				} else if(selectImgIdx <= 5+4){ //적
					delete socketArr [i][j].item;
					t = socketArr [i][j].item = EnemyGen::getEnemy(selectImgIdx - 6);
				} else { // 아이템
					if (socketArr [i][j].item->isObstacle()) {
						t = socketArr [i][j].item;
						t = ((Obstacle *)(t))->subItem = new Item();
					} else {
						t = socketArr [i][j].item = new Item();
					}

					
				}
				//selectImgIdx = -1;
				if (t) {
					t->imgIdx = selectImgIdx;
					t->imgs = &img;
				}
			}
		}
	}
}

// TODO 지우는기능추가 sdf

HWND childWin;

void tick() {
	SceneM::getIns(0).tick();
}

void draw(HDC h) {
	SceneM::getIns(0).render(h);
}

void init() {
	map.init();
}

void startTest(HWND hWnd) {
	auto wc = wcex;
	wc.lpszClassName = L"testWindow"; // 차일드 윈도우 클래스 이름
	wc.lpfnWndProc = testWinProc; // 차일드 윈도우 프로시저 지정
	RegisterClassEx(&wc); 

	childWin = CreateWindow ( L"testWindow", NULL, WS_VISIBLE | WS_OVERLAPPEDWINDOW,
	10, 10, 900, 900, hWnd, NULL, hInst, NULL);
	map.startTest(true);
}

void stopTest() {
	map.startTest(false);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		init();
		win.init(hWnd);
		SetTimer(hWnd, 0, 15, NULL);
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
			{
				auto t = CreateDialog(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				ShowWindow(t, SW_SHOW);
			}   break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_TIMER:
		tick();
		win.clearWindow();
		break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			HDC h = win.prerender(hdc);
			draw(h);
			win.postrender();
            EndPaint(hWnd, &ps);
        }
        break;
	case WM_LBUTTONDOWN:
		map.click(lParam);
		break;
	case WM_MOUSEMOVE:
		map.mouseMove(lParam);
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
		editWindow = hDlg;
		map.setButtonImage(hDlg);
		map.setEditBox(hDlg);
        return (INT_PTR)TRUE;

    case WM_COMMAND:
		auto com = LOWORD(wParam);
		for (size_t i = IDC_BUTTON16; i < 4+IDC_BUTTON16; i++) {
			if (com == i) {
				map.selectImgIdx = i-IDC_BUTTON16 + 9;
			}
		}
		switch (com) {
		case IDOK:
			PostQuitMessage(0);
			break;
		case IDC_BUTTON1:
			map.setMapImage(0);
			break;
		case IDC_BUTTON2:
			map.setMapImage(1);
			break;
		case IDC_BUTTON3:
			map.setMapImage(2);
			break;
		case IDC_BUTTON4:
			map.selectImgIdx = 3;
			break;
		case IDC_BUTTON5:
			map.selectImgIdx = 4;
			break;
		case IDC_BUTTON6:
			map.selectImgIdx = 5;
			break;
		case IDC_BUTTON7:
			map.selectImgIdx = 6;
			break;
		case IDC_BUTTON8:
			map.selectImgIdx = 7;
			break;
		case IDC_BUTTON9:
			map.selectImgIdx = 8;
			break;
		
		case IDC_BUTTON10:
			startTest(hDlg);
			break;
		case IDC_BUTTON11:
			map.save();
			break;
		case IDC_BUTTON12:
			map.load();
			break;
		case IDC_EDIT1:
			map.mapSizeX = GetDlgItemInt(hDlg, IDC_EDIT1, NULL, TRUE);
			map.calculate();
			break;
		case IDC_EDIT2:
			map.mapSizeY = GetDlgItemInt(hDlg, IDC_EDIT2, NULL, TRUE);
			map.calculate();
			break;
		case IDC_EDIT3:
			map.gridCountX = GetDlgItemInt(hDlg, IDC_EDIT3, NULL, TRUE);
			map.calculate();
			break;
		case IDC_EDIT4:
			map.gridCountY = GetDlgItemInt(hDlg, IDC_EDIT4, NULL, TRUE);
			map.calculate();
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

LRESULT CALLBACK testWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	static WindowM win2;
	switch (message)
    {
	case WM_CREATE:
		win2.init(hWnd);
		SetTimer(hWnd, 1, 15, NULL);
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
	case WM_TIMER:
		PlayModeM::getIns()->tick();
		win2.clearWindow();
		break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			HDC h = win2.prerender(hdc);
			PlayModeM::getIns()->render(h);
			win2.postrender();
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        DestroyWindow(hWnd);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

inline void Player::startGame(bool b) {
	p.set(0, 0);
	size = map.gridSize;
}

void Item::setData(wstring s, wstring b) {
	wstringstream ss(s);
	ss >> imgIdx;
	if (imgIdx != -1) {
		imgs = &map.img;
	}
}

void Obstacle::setData(wstring a, wstring b) {
	wstringstream ss(a);
	wstringstream ss2(b);
	ss >> imgIdx;
	if (!subItem) subItem = new Item();
	ss2 >> subItem->imgIdx;
	if (imgIdx != -1) {
		imgs = &map.img;
		subItem->imgs = &map.img;
	}
}

void Map::load() {
		hFile = CreateFile ( L"test.txt", GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
		if (hFile == INVALID_HANDLE_VALUE) return;
		

		memset (InBuf, 0, 1000*sizeof(TCHAR));
		ReadFile (hFile, InBuf, 1000, textSize, NULL); // hFile에서 size 만큼 읽어 InBuff에 저장
		InBuf[*textSize] = L'\0';

		wstringstream ss(InBuf);
		ss >>mapImgIdx>> mapSizeX  >> mapSizeY  >> gridSize.x  >> gridSize.y  >> gridCountX  >> gridCountY ;
		setSocketArrByGridCount();
		setEditBox(editWindow);
		for (size_t i = 0; i < socketArr.size(); i++) {
			for (size_t j = 0; j < socketArr [i].size(); j++) {
				wstring s;
				ss >> s;
				int loc = s.find(L'/');
				if (loc == -1) {
					wstringstream ss(s);
					int t_idx;
					ss >> t_idx;
					if(t_idx <= 2+3)
						socketArr [i][j].item->setData(s, L"");
					else if (t_idx <= 2 + 3 + 4) { // 6,7,8,9
						socketArr [i][j].item = EnemyGen::getEnemy(t_idx-6);
						socketArr [i][j].item->setData(s, L"");
					}

				} else {
					wstring a = s.substr(0, loc);
					wstring b = s.substr(loc + 1);
					delete socketArr [i][j].item;
					socketArr [i][j].item = new Obstacle();
					socketArr [i][j].item->setData(a,b);
				}
			}
		}

		calculate();
		CloseHandle (hFile);
	}