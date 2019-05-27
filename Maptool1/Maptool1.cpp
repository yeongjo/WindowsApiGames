// Maptool1.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Maptool1.h"
#include "MyMain.h"

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
	vector<CImage *> *imgs;
	int imgIdx = -1;
	void render(HDC h, Pos<> &p, Pos<> &size) {
		if (imgIdx == -1) return;
		(*imgs)[imgIdx]->StretchBlt(h, p.x, p.y, size.x, size.y, SRCCOPY);
	}

	wstring getSaveData() {
		
	}
};

class MapSocket {
public:
	Pos<> p;
	Pos<> *size;
	Item *item;

	// pos index
	Pos<> idx;

	MapSocket() {
		item = new Item();
	}

	bool checkColl(int _x, int _y) {
		RECT rt = {p.x, p.y, p.x+size->x, p.y+size->y};
		if (collPointRect(_x, _y, &rt))
			return true;
		return false;

	}



	void calculatePos() {
		p.set(idx.x + size->x, idx.y + size->y);
	}

	void render(HDC h) {
		item->render(h, p, *size);
	}
};

// TODO 지우는기능추가 sdf

class Map : public Obj {
public:
	vector<vector<MapSocket>> socketArr;

	CImage *mapImg;
	vector<CImage*> img;

	int selectImgIdx = -1;

	int mapSizeX = 1000;
	int mapSizeY = 1000;

	Pos<> gridSize = Pos<>(30,30);

	int gridCountX = 15;
	int gridCountY = 8;


	void click(int _x, int _y) {
		if (selectImgIdx == -1) return;
		for (size_t i = 0; i < socketArr.size(); i++) {
			for (size_t j = 0; j < socketArr[i].size(); j++) {
				if (socketArr [i][j].checkColl(_x, _y)) {
					socketArr [i][j].item->imgIdx = selectImgIdx;
					socketArr [i][j].item->imgs = &img;
					selectImgIdx = -1;
				}
			}
		}
	}

	void loadImage() {
		img.resize(9);
		for (size_t i = 1; i <= img.size(); i++) {
			img [i] = new CImage();
			wstring s(L"../k");
			s += i;
			img [i]->Load(s.c_str());
		}
		mapImg = img [0];
	}

	void setGridCount(int x, int y) {
		if (x > 0) {
			gridCountX = x;
			gridSize.x = mapSizeX / gridCountX;
		}
		if (y > 0) {
			gridCountY = y;
			gridSize.y = mapSizeY / gridCountY;
		}
	}

	void setMapSize(int x, int y) {
		if (x > 0) {
			mapSizeX = x;
			gridSize.x = mapSizeX / gridCountX;
		}
		if (y > 0) {
			mapSizeY = y;
			gridSize.y = mapSizeY / gridCountY;
		}
	}

	void calculate() {
		gridSize.x = mapSizeX / gridCountX;
		gridSize.y = mapSizeY / gridCountY;
	}

	void setMapImage(int i) {
		mapImg = img [i];
	}

	void init(int x, int y) {
		socketArr.resize(y);
		for (size_t i = 0; i < socketArr.size(); i++) {
			socketArr [i].resize(x);
			for (size_t j = 0; j < x; j++) {
				socketArr [i][j].idx.set(j, i);
				socketArr [i][j].size = &gridSize;
			}
		}
		calculate();

		loadImage();
	}

	void renderGrid(HDC h) {
		// y 
		for (size_t i = 1; i <= gridCountY; i++) {
			MoveToEx(h, 0, i*gridSize.x, NULL);
			LineTo(h, gridSize.y * gridCountY, i * gridSize.x);
			
		}
		// x
		for (size_t i = 1; i <= gridCountX; i++) {
			MoveToEx(h, i*gridSize.y, 0, NULL);
			LineTo(h, i * gridSize.x, gridSize.y * gridCountX);
		}
	}

	void render(HDC h) {
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
		FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
		
		wstringstream ss;
		for (size_t i = 0; i < socketArr.size(); i++) {
			for (size_t j = 0; j < socketArr[i].size(); j++) {
				ss << socketArr [i][j].item->getSaveData() << L" ";
			}
		}

		SetFilePointer (hFile, 0, NULL, FILE_END);
		WriteFile (hFile, InBuf, lstrlen(InBuf)*2, textSize, NULL); // OutBuff의 내용을 hFile의 끝에 저장
		CloseHandle (hFile);
	}

	void load() {
		hFile = CreateFile ( L"test.txt", GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
		wstringstream ss;
		for (size_t i = 0; i < socketArr.size(); i++) {
			for (size_t j = 0; j < socketArr [i].size(); j++) {
				ss >> socketArr [i][j].item->imgIdx;
			}
		}

		memset (InBuf, 0, 1000*sizeof(TCHAR));
		ReadFile (hFile, InBuf, 1000, textSize, NULL); // hFile에서 size 만큼 읽어 InBuff에 저장
		InBuf[*textSize] = L'\0';
		CloseHandle (hFile);
	}
};

LRESULT CALLBACK testWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

Map map;
HWND childWin;

void tick() {
	SceneM::getIns(0).tick();
}

void draw(HDC h) {
	SceneM::getIns(0).render(h);
}

void startTest(HWND hWnd) {
	auto wc = wcex;
	wc.lpszClassName = L"testWindow"; // 차일드 윈도우 클래스 이름
	wc.lpfnWndProc = testWinProc; // 차일드 윈도우 프로시저 지정
	RegisterClassEx(&wc); 

	childWin = CreateWindow ( L"testWindow", NULL, WS_POPUP | WS_VISIBLE | WS_OVERLAPPEDWINDOW,
10, 10, 900, 900, hWnd, NULL, hInst, NULL);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
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
		SetDlgItemInt(hDlg, IDC_EDIT1, map.mapSizeX, TRUE);
		SetDlgItemInt(hDlg, IDC_EDIT2, map.mapSizeY, TRUE);
		SetDlgItemInt(hDlg, IDC_EDIT3, map.gridCountX, TRUE);
		SetDlgItemInt(hDlg, IDC_EDIT4, map.gridCountY, TRUE);
        return (INT_PTR)TRUE;

    case WM_COMMAND:
		switch (LOWORD(wParam)) {
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
				map.mapSizeX = GetDlgItemInt (hDlg, IDC_EDIT1, NULL, TRUE);
				break;
				case IDC_EDIT2:
				map.mapSizeY = GetDlgItemInt (hDlg, IDC_EDIT2, NULL, TRUE);
				break;
				case IDC_EDIT3:
				map.gridCountX = GetDlgItemInt (hDlg, IDC_EDIT3, NULL, TRUE);
				break;
				case IDC_EDIT4:
				map.gridCountY = GetDlgItemInt (hDlg, IDC_EDIT4, NULL, TRUE);
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
		tick();
		win2.clearWindow();
		break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			HDC h = win2.prerender(hdc);
			draw(h);
			win2.postrender();
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