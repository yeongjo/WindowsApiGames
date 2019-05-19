// WindowsWork8-3.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "WindowsWork8-3.h"
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
    LoadStringW(hInstance, IDC_WINDOWSWORK83, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSWORK83));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSWORK83));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSWORK83);
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

RECT windowSize;
int blockCount = 40;
//int blockSize; // use for zoom
int canvasSize;
void renderLines(HDC hdc) {
	blockSize = windowSize.bottom / blockCount;
	canvasSize = blockSize * blockCount;
	for (int i = 0; i <= canvasSize; i += blockSize)
	{
		MoveToEx(hdc, i, 0, NULL);
		LineTo(hdc, i, canvasSize);
		wstringstream ss;
		ss << (i/ blockSize);
		TextOut(hdc, i, 0, ss.str().c_str(), ss.str().size());
	}
	for (int i = 0; i <= canvasSize; i += blockSize)
	{
		MoveToEx(hdc, 0, i, NULL);
		LineTo(hdc, canvasSize, i);
	}
}


class Shape {
public:
	// 0 none
	// 1 line
	// 2 ellipse
	// 3 triangle
	// 4 rectangle
	int shapeType;
	int x, y;
	int size = 1;
	int thickness = 3;
	int selectIdx = 0;

	bool isSelected = false;

	void load(wstring s) {
		wstringstream ss(s);

		ss >> shapeType;
		ss >> x;
		ss >> y;
		ss >> size;
		ss >> thickness;
		ss >> selectIdx;
		ss >> isSelected;
	}

	wstring toString() {
		wstringstream ss;
		ss << shapeType << L" ";
		ss << x << L" ";
		ss << y << L" ";
		ss << size << L" ";
		ss << thickness << L" ";
		ss << selectIdx << L" ";
		ss << isSelected;
		return ss.str();
	}
	   
	void setRandomPos(){
		x=(rand() % blockCount);
		y=(rand() % blockCount);
	}
	
	// default size is 1
	void setSize(int i) {
		if (size + i <= 0)
			return;
		size += i;
	}

	void select(bool sel = true) {
		isSelected = sel;
	}

	void move(int _x, int _y) {
		x += _x;
		y += _y;
	}

	void render(HDC hdc) {
		if (shapeType == 0)
			return;
		int width = blockSize * size; // 블럭사이즈대로 적용하기
		HPEN hpen = 0;
		HPEN old = 0;
		if (isSelected)
			hpen = (HPEN)CreatePen(PS_SOLID, thickness, RGB(20, 20, 20));
		else
			hpen = (HPEN)CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
		old = (HPEN)SelectObject(hdc, hpen);

		HBRUSH hBrush = (HBRUSH)CreateSolidBrush(RGB(200, 200, 200));
		HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		int x = this->x * blockSize;
		int y = this->y * blockSize;
		switch (shapeType) {
		case 1:
			Ellipse(hdc, x, y, x+width, y + width);
			break;
		case 2:
			POINT point[3];
			point[0].x = x;
			point[0].y = y + width;
			point[1].x = x + width;
			point[1].y = y + width;
			point[2].x = (x + x + width) / 2;
			point[2].y = y;
			Polygon(hdc, point, 3);
			break;
		case 3:
			Rectangle(hdc, x, y, x + width, y + width);
			break;
		}
		SelectObject(hdc, old);
		DeleteObject(hpen);
		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);
	}
};

Shape shapes[5];

// 마지막에 생성된 친구의 SelectIdx
// 1~5 / 0: 미생성
int lastCreatedSelectIndex;

// 모두 선택되었다면 참
bool bIsSelectAll;
void selectShape(int idx) {
	if (idx == 6) {
		for (int i = 0; i < 5; i++)
			shapes[i].select(); // Select all
		bIsSelectAll = true;
	}
	else {
		bIsSelectAll = false;
		int selectedIdx = -1;
		for (int i = 0; i < 5; i++)
			if (shapes[i].selectIdx == idx) {
				shapes[i].select();
				selectedIdx = i;
			}
			else
				shapes[i].select(false); // unselect other

		// Take front Selected obj;
		if (selectedIdx > -1) {
			Shape t_s = shapes[selectedIdx];
			for (int i = selectedIdx - 1; i >= 0; i--)
				shapes[i + 1] = shapes[i];
			shapes[0] = t_s;
		}
	}
}

void addShape(Shape& shape) {
	// 앞에있던 것들을 뒤로 미룬다.
	for (int i = 3; i >= 0; i--)
		shapes[i + 1] = shapes[i];
	// lastCreatedSelectIndexrk 5를 넘지않게 한다.
	lastCreatedSelectIndex = ++lastCreatedSelectIndex > 5 ? 5 : lastCreatedSelectIndex;
	shape.selectIdx = lastCreatedSelectIndex;
	// 만약 앞에 이미 나랑 같은 숫자를 가진 오브젝트들이 있다면 하나씩 줄인다.
	for (int i = 1; i < 5; i++)
		if (shapes[i].shapeType != 0 && shapes[i].selectIdx == lastCreatedSelectIndex) {
			for (int j = 1; j < 5; j++)
				shapes[j].selectIdx--;
			break;
		}
	shapes[0] = shape; // 새로만든걸 맨앞에 넣는다.
	selectShape(shape.selectIdx);
}

void selectShapeMove(int x, int y) {
	for (int i = 0; i < 5; i++)
		if(shapes[i].isSelected)
			shapes[i].move(x, y);
}

void selectShapeZoom(int zoomLevel) {
	for (int i = 0; i < 5; i++)
	{
		if(shapes[i].isSelected)
			shapes[i].setSize(zoomLevel);
	}
}


// 모두 선택이면 모두 지우고 하나선택이면 하나지우기
void removeSelectShape() {
	if (bIsSelectAll) {
		for (int i = 0; i < 5; i++){
			shapes[i].shapeType = 0;
		}
		lastCreatedSelectIndex = 0;
	}
	else {
		// 선택된 오브젝트를 지우면 그 선택친구 selectidx를 가지고
		// 그 idx 보다 숫자가 큰 idx를 가진 오브젝트들을 하나씩 -1 해준다.
		for (int i = 0; i < 5; i++)
		{
			if (shapes[i].isSelected) {
				int t_idx = shapes[i].selectIdx;

				// 오브젝트를 한줄씩 당겨 덮어씌워서 제거해주는 기능
				for (int j = i; j < 4; j++)
					shapes[i] = shapes[i + 1];

				// 그 idx 보다 숫자가 큰 idx를 가진 오브젝트들을 하나씩 -1 해준다.
				for (int i = 0; i < 5; i++)
					if (shapes[i].selectIdx > t_idx)
						--shapes[i].selectIdx;
				--lastCreatedSelectIndex;
			}
		}
	}
}

// 4 -> 0
void render(HDC hdc) {
	for (int i = 4; i >= 0; i--)
	{
		shapes[i].render(hdc);
	}
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HANDLE hFile;
	TCHAR str[100], lpstrFile[100] = L"";
	TCHAR filter[100] = L"소스 File(*.cpp)\0*.cpp\0문서 File \0 *.txt;*.doc \0 ";
	int read_size;
	OPENFILENAME OFN;
	Shape t_shape;
    switch (message)
    {
	case WM_CREATE:
		GetClientRect(hWnd, &windowSize);
		srand(time(NULL));
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
			case IDM_ABOUT:
			{
				//DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				wstringstream ss;
				for (size_t i = 0; i < 5; i++) {
					ss << shapes [i].toString().c_str() << L"\n";
				}
				memset(&OFN, 0, sizeof(OPENFILENAME)); // 초기화
				OFN.lStructSize = sizeof(OPENFILENAME);
				OFN.hwndOwner = hWnd;
				OFN.lpstrFilter = filter;
				OFN.lpstrFile = lpstrFile;
				OFN.nMaxFile = 256;
				OFN.lpstrInitialDir = L".";
				if (GetSaveFileName(&OFN) != 0) {

					wsprintf(str, L"%s 파일에 저장하시겠습니까 ?", OFN.lpstrFile);
					MessageBox(hWnd, str, L"저장하기 선택", MB_OK);
					hFile = CreateFile(OFN.lpstrFile, GENERIC_READ | GENERIC_WRITE,
						FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, 0);
					SetFilePointer(hFile, 0, NULL, FILE_END);
					WriteFile(hFile, ss.str().c_str(), ss.str().size()*2+2, (LPDWORD)& read_size, NULL);
					CloseHandle(hFile);
				}
			}break;
            case IDM_EXIT:
			{
				memset(&OFN, 0, sizeof(OPENFILENAME)); // 초기화
				OFN.lStructSize = sizeof(OPENFILENAME);
				OFN.hwndOwner = hWnd;
				OFN.lpstrFilter = filter;
				OFN.lpstrFile = lpstrFile;
				OFN.nMaxFile = 256;
				OFN.lpstrInitialDir = L"."; // 초기 디렉토리
				TCHAR c [1000] = {};
				if (GetOpenFileName(&OFN) != 0) {
					wsprintf(str, L"%s 파일을 여시겠습니까 ?", OFN.lpstrFile);
					MessageBox(hWnd, str, L"열기 선택", MB_OK);
					
					hFile = CreateFile(OFN.lpstrFile,GENERIC_READ|GENERIC_WRITE,
					FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
					ReadFile(hFile, c, 1000, (LPDWORD)&read_size, NULL);
					c[999] = L'\0';
					CloseHandle(hFile);
					InvalidateRect(hWnd, NULL, true);
					wchar_t *ttt;
					TCHAR *tok = wcstok_s(c, L"\n",&ttt);
					int i = 0;
					while (tok) {
						if (i > 5) break;
						shapes [i].load(tok);
						tok = wcstok_s(NULL, L"\n",&ttt);
						i++;
					}
					InvalidateRect(hWnd, NULL, true);
				}
				
				
			}   break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_CHAR: 
	{
		if ('0' <= wParam && wParam <= '6') {
			int a = wParam-48;
			selectShape(a);
		}
		switch (wParam) {
		case 's':
			blockCount = 30;
			break;
		case 'm':
			blockCount = 40;
			break;
		case 'l':
			blockCount = 50;
			break;
		case 'E':case 'e': // 원 추가하기
			t_shape.shapeType = 1;
			t_shape.setRandomPos();
			addShape(t_shape);
			break;
		case 'T':case 't': // 삼각 추가하기
			t_shape.shapeType = 2;
			t_shape.setRandomPos();
			addShape(t_shape);
			break;
		case 'R':case 'r': // 사각형 추가하기
			t_shape.shapeType = 3;
			t_shape.setRandomPos();
			addShape(t_shape);
			break;
		case '-':
			selectShapeZoom(-1);
			break;
		case '+':
			selectShapeZoom(1);
			break;
		case 'q':case 'Q':
			PostQuitMessage(0);
			break;
		}
		InvalidateRect(hWnd, NULL, true);
		break;
	}
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_LEFT:
			selectShapeMove(-1,0);
			break;
		case VK_UP:
			selectShapeMove(0, -1);
			break;
		case VK_RIGHT:
			selectShapeMove(1, 0);
			break;
		case VK_DOWN:
			selectShapeMove(0, 1);
			break;
		case VK_DELETE:
			removeSelectShape();
			break;
		}
		InvalidateRect(hWnd, NULL, true);
		break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			renderLines(hdc);
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
