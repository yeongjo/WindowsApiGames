// WindowsWork5-1.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "WindowsWork5-1.h"

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
    LoadStringW(hInstance, IDC_WINDOWSWORK51, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSWORK51));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSWORK51));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSWORK51);
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

class SlicePart;

class MBitmap {
public:

	HBITMAP hBitmap;
	BITMAP bmp;
	
	Pos pos;
	Pos size;
	Pos sliceStartPos;
	Pos renderSize;

	bool isAble = false;

	MBitmap(){}

	~MBitmap() {
		Destory();
	}

	void init(int imgID) {
		isAble = true;
		hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(imgID));
		GetObject(hBitmap, sizeof(BITMAP), &bmp);
		size.x = bmp.bmWidth;
		size.y = bmp.bmHeight;
		renderSize.set(600, 600);
		size.set(600, 600);
	}

	void Destory() {
		DeleteObject(hBitmap);
	}

	bool checkPointInThis(int x, int y) {
		RECT rt = RECT{ pos.x, pos.y, pos.x+size.x, pos.y+size.y };
		return collPointRect(x, y, &rt);
	}

	SlicePart* createSlicePart(int x, int y, int width, int height);

	void render(HDC hdc) {
		if (!isAble) return;
		int x = pos.x, y = pos.y;
		int sx = size.x, sy = size.y;

		int mx = sliceStartPos.x, my = sliceStartPos.y;
		int rsX = renderSize.x, rsY = renderSize.y;

		HDC hMemDC = CreateCompatibleDC(hdc);

		SelectObject(hMemDC, hBitmap);
		StretchBlt(hdc, x, y, rsX, rsY, hMemDC, mx, my, sx, sy, SRCCOPY);

		DeleteDC(hMemDC);
	}
};

class SlicePart : public MBitmap {
	Pos offsetPos;
public:
	MBitmap* parent;
	

	int speed;

	void init() {
		speed = 50;
		offsetPos.set(0, 0);
		isAble = true;
	}

	void setGoalPos(int x, int y) {
		offsetPos = Pos(x,y);
	}

	void move() {
		if (offsetPos.isZero()) return;
		Pos a = offsetPos / (int)length(offsetPos) * speed;
		if (offsetPos.ads() - a.ads() <= 0) {
			pos += offsetPos;
			offsetPos.set(0,0);
		}
		else {
			offsetPos -= a;
			pos += a;
		}
	}
};

MBitmap bitmaps[2];
vector<SlicePart*> slicePart;


unsigned int selectImageIdx = 0;
unsigned int canvasSize = 600;
unsigned int blockSize = 150;
unsigned int blockCount = 4;

// 빈칸을 찾고 바로 옆이면 이동한다
// 빈칸 우째찾음
void moveBlockToEmpty(SlicePart* sp) {
	unsigned int bb = blockCount * blockCount;
	Pos emptyPos(-1, -1);
	vector<int> blocksArray(bb);
	Pos* lpEmptyPos = 0;
	for (size_t i = 0; i < bb; i++)
	{
		// 좌표로 변환
		if (slicePart[i]->isAble == false) {
			/*blocksArray[t.x + blockCount * t.y] = 0;
		else
			blocksArray[t.x + blockCount * t.y] = 1;*/
			Pos t = slicePart[i]->pos / blockSize;
			lpEmptyPos = &slicePart[i]->pos;
			emptyPos = t;
			break;
		}

	}
	//for (size_t i = 0; i < bb; i++)
	//{
	//	if (blocksArray[i] == 0) {
	//		emptyPos = Pos{ (int)(i % blockCount), (int)(i / blockCount) };
	//	}
	//}
	
	if (emptyPos.x == -1) return;
	Pos t = sp->pos;
	Pos m = sp->pos / blockSize;
	if (m.x == emptyPos.x) {
		if (ads(m.y - emptyPos.y) <= 1) {// 움직일수있음
			sp->setGoalPos(0,(emptyPos.y - m.y) * blockSize);
			if(lpEmptyPos != 0) *lpEmptyPos = t;
		}
	}else if (m.y == emptyPos.y) {
		if (ads(m.x - emptyPos.x) <= 1) {// 움직일수있음
			sp->setGoalPos((emptyPos.x - m.x) * blockSize,0);
			if (lpEmptyPos != 0) *lpEmptyPos = t;
		}
	}
}

bool checkGameEnd() {
	for (size_t y = 0; y < blockCount; y++)
	{
		for (size_t x = 0; x < blockCount; x++)
		{
			//int mx = x * blockSize, my = y * blockSize;
			int idx = y * blockCount + x;
			Pos t = slicePart[idx]->sliceStartPos;
			if (t != slicePart[idx]->pos) return false;
			//if (mx != t.x && my != t.y) return false;
		}
	}
	return true;
}

void checkMouseInSubPicture(int x, int y) {
	for (size_t i = 0; i < blockCount * blockCount; i++)
	{
		if (slicePart[i]->checkPointInThis(x, y)) {
			moveBlockToEmpty(slicePart[i]);
		}
	}
}

SlicePart* MBitmap::createSlicePart(int x, int y, int width, int height) {
	SlicePart* t_slice = new SlicePart(*((SlicePart*)this));
	t_slice->init();
	t_slice->sliceStartPos.x = x; t_slice->sliceStartPos.y = y;
	t_slice->renderSize.x = width; t_slice->renderSize.y = height;
	t_slice->size = Pos(blockSize, blockSize);
	t_slice->parent = this;
	return t_slice;
}


void randomShuffleNums(vector<int>& idx) {
	int bb = blockCount * blockCount;
	for (size_t i = 0; i < bb; i++)
	{
		for (size_t j = 0; j < bb; j++)
		{
			if (rand() % 2) continue;
			int a = idx[i];
			idx[i] = idx[j];
			idx[j] = a;
		}
	}
}

void initNums(vector<int>& idx) {
	for (size_t i = 0; i < idx.size(); i++)
	{
		idx[i] = i;
	}
}

void setRandomPos() {
	for (size_t y = 0; y < blockCount; y++)
	{
		for (size_t x = 0; x < blockCount; x++)
		{
			int idx = y * blockCount + x;
			slicePart[idx]->pos = Pos(x * blockSize, y * blockSize);
		}
	}
}

int isShowFullVersion = false;

// i 는 0아니면 1이다
void init(int a, HWND hWnd) {
	isShowFullVersion = true;
	blockSize = canvasSize / blockCount;

	slicePart.clear();
	slicePart.resize(blockCount * blockCount);

	selectImageIdx = a;
	srand(time(NULL));

	vector<int> randNum(blockCount * blockCount);
	initNums(randNum);
	randomShuffleNums(randNum);

	bitmaps[!a].isAble = false;
	bitmaps[a].isAble = true;
	
	for (size_t y = 0; y < blockCount; y++)
	{
		for (size_t x = 0; x < blockCount; x++)
		{
			int mx = x * blockSize, my = y * blockSize;
			int mx2 = blockSize, my2 = blockSize;
			int gettenRandNum = randNum[y*blockCount + x];
			slicePart[gettenRandNum] = bitmaps[a].createSlicePart(mx, my, mx2, my2);
			if (x == blockCount - 1 && y == blockCount - 1)
				slicePart[gettenRandNum]->isAble = false;
		}
	}
	setRandomPos();

	InvalidateRect(hWnd, NULL, true);
}

HWND hWnd;

bool isGameEnd;

void update() {
	if (isGameEnd) return;

	for (size_t i = 0; i < slicePart.size(); i++)
		slicePart[i]->move();

	InvalidateRect(hWnd, NULL, true);

	if (checkGameEnd()) {
		isGameEnd = true;
		MessageBox(hWnd, L"게임이 끝났습니다", L"질문", MB_YESNO);
		isGameEnd = false;
		init(selectImageIdx, hWnd);
	}
}


void render(HDC hdc) {
	if(isShowFullVersion)
		bitmaps[selectImageIdx].render(hdc);
	else
		for (size_t i = 0; i < blockCount * blockCount; i++)
			slicePart[i]->render(hdc);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
    switch (message)
    {
	case WM_CREATE:
		bitmaps[0].init(IDB_BITMAP1);
		bitmaps[1].init(IDB_BITMAP2);
		init(0, hWnd);
		::hWnd = hWnd;
		SetTimer(hWnd, 1, 33, NULL);
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                
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
		update();
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
		case 'f':case'F':
			isShowFullVersion = !isShowFullVersion;
			break;
		case 's':case'S':
			isShowFullVersion = false;
			break;
		case 'q':case'Q':
			PostQuitMessage(0);
			break;
		case '1':
			init(0, hWnd);
			break;
		case '2':
			init(1, hWnd);
			break;
		case '3':
			blockCount = 3;
			init(selectImageIdx, hWnd);
			break;
		case '4':
			blockCount = 4;
			init(selectImageIdx, hWnd);
			break;
		case '5':
			blockCount = 5;
			init(selectImageIdx, hWnd);
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		checkMouseInSubPicture(LOWORD(lParam), HIWORD(lParam));
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
