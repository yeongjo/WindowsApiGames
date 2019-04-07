// WindowsWork4-3.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "WindowsWork4-3.h"

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
    LoadStringW(hInstance, IDC_WINDOWSWORK43, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSWORK43));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSWORK43));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSWORK43);
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

int blockSize = 7;

COLORREF colors[] = { RGB(255, 0, 0),
RGB(0, 255, 0), 
RGB(0, 0, 255), 
RGB(200, 200, 0), 
RGB(200, 0, 200), 
RGB(0, 200, 200)};

int selectColorIdx = 0;

class RenderObj {
public:
	Pos pos;
	Pos size{blockSize, blockSize };
	COLORREF color = RGB(200, 200, 200);

	bool isCircle;

	void init(int _x, int _y, bool _isCircle = false) {
		pos.x = _x;
		pos.y = _y;
		setColor(selectColorIdx);
		isCircle = _isCircle;
	}

	void setColor(int i) {
		color = colors[i];
	}
	void setColor(int r, int g, int b) {
		color = RGB(r, g, b);
	}


	virtual void render(HDC hdc) {
		// 블럭사이즈대로 적용하기
		int x = pos.x;
		int y = pos.y;
		int x2 = size.x + x;
		int y2 = size.y + y;

		if(isCircle)
			renderCircle(hdc, x, y, size.x, size.y, color);
		else {
			HBRUSH hBrush = (HBRUSH)CreateSolidBrush(color);
			HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Rectangle(hdc, x, y, x2, y2);
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
		}
	}
};

bool isRenderLine = true;
void renderLines(HDC hdc) {
	if (!isRenderLine)
		return;
	int canvasSize = blockSize * 100;
	for (int i = 0; i <= 100; i++)
	{
		MoveToEx(hdc, i*blockSize, 0, NULL);
		LineTo(hdc, i*blockSize, canvasSize);
		//wstringstream ss;
		//ss << (i / blockSize);
		//TextOut(hdc, i, 0, ss.str().c_str(), ss.str().size());
	}
	for (int i = 0; i <= 100; i++)
	{
		MoveToEx(hdc, 0, i*blockSize, NULL);
		LineTo(hdc, canvasSize, i*blockSize);
	}
}

Pos convertMousePosToWorld();

vector<RenderObj> renderObjs;

/*
0 = circle
1 = rect
2 = freerect
*/
int createShapeType = 0;
int controlSizeMode = 0;

void makeAndControlMode(int x, int y, bool isCircle) {
	if (!controlSizeMode) {
		RenderObj obj;
		obj.init(x, y, isCircle);
		renderObjs.push_back(obj);
		controlSizeMode = 1;
	}
	else {
		Pos& pos = renderObjs[renderObjs.size() - 1].pos;
		Pos& size = renderObjs[renderObjs.size() - 1].size;
		size = convertMousePosToWorld() - pos;
	}
}

bool ldrag;

void update(HWND hWnd) {
	if (ldrag) {
		Pos t = convertMousePosToWorld();
		switch (createShapeType) {
		case 0:
			makeAndControlMode(t.x, t.y, true);
			break;
		case 1:
			makeAndControlMode(t.x, t.y, false);
			break;
		case 2:
			RenderObj obj;
			obj.init(t.x, t.y, false);
			renderObjs.push_back(obj);
			break;
		}
	}
	else {
		controlSizeMode = 0;
	}

	InvalidateRect(hWnd, NULL, true);
}

void render(HDC hdc) {
	renderLines(hdc);
	for (size_t i = 0; i < renderObjs.size(); i++)
	{
		renderObjs[i].render(hdc);
	}
}


Pos mousePos;

Pos convertMousePosToWorld() {
	return mousePos / blockSize * blockSize;
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
    switch (message)
    {
	case WM_CREATE:
		SetTimer(hWnd, 1, 33, NULL);
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
				isRenderLine = true;
                break;
			case ID_GRID_OFF:
				isRenderLine = false;
				break;
			case ID_COLOR_1:
				selectColorIdx = 0;
				break;
			case ID_COLOR_2:
				selectColorIdx = 1;
				break;
			case ID_COLOR_3:
				selectColorIdx = 2;
				break;
			case ID_COLOR_4:
				selectColorIdx = 3;
				break;
			case ID_COLOR_5:
				selectColorIdx = 4;
				break;
			case ID_COLOR_6:
				selectColorIdx = 5;
				break;
			case ID_SHAPE_ELLIPSE:
				createShapeType = 0;
				break;
			case ID_SHAPE_RECTANGLE:
				createShapeType = 1;
				break;
			case ID_SHAPE_FREEDRAW:
				createShapeType = 2;
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
		update(hWnd);
		break;
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
		mousePos.x = LOWORD(lParam);
		mousePos.y = HIWORD(lParam);
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
