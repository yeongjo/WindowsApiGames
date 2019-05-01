// WindowsWork2-3.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "WindowsWork2-3.h"

#define MAX_LOADSTRING 100

#define Width 800
#define Height 600
#define MYTEXTLEN 1000

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
    LoadStringW(hInstance, IDC_WINDOWSWORK23, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSWORK23));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSWORK23));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSWORK23);
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
      CW_USEDEFAULT, Width, CW_USEDEFAULT, Height, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

TCHAR myText[MYTEXTLEN];
int textLen = 0; // 현재 문자의 길이
int textCaretPos; // 텍스트 사이의 커서위치 문자입력시 이 인덱스에 입력됨.
int caretYPos;
int totalCaretYPos;
SIZE caretPos; // 커서의 실제 좌표
RECT winRect = {0,0,Width ,Height};


void pushAll(int i, int offset = 1) {
	//왼쪽으로 지우기
	if(offset < 0 &&  0 <= i + offset)// 최소보다 커서가 작을때
		for (; i < textLen; i++)
			myText[i+ offset] = myText[i];
	//오른쪽으로 밀어내기
	else if(offset > 0 && offset + textLen < MYTEXTLEN) // 최대치를 넘어가지않게
		for (int j = textLen+1; j >= i; j--)
			myText[j + offset] = myText[j];
}

void removeAll() {
	textLen = 0;
	textCaretPos = 0;
	caretYPos = 0;
	totalCaretYPos = 0;
	myText[0] = '\0';
}

//ori < 0 앞으로 \n를 찾는다
//ori > 0 뒤쪽으로 \n를 찾는다
//a는 앞쪽으로 찾은 값의 오프셋 보통 +1 씀
int _findOneLineEndPos<>(int ori = -1, int a = 0) {
	if (ori < 0) { // 앞쪽으로 찾음.
		for (int i = textCaretPos - 1; i > 0; i--) {
			if (myText[i] == '\n')
				return i + a; // \n pos + a
		}
		return 0;
	}
	else {
		for (int i = textCaretPos; i < textLen; i++) {
			if (myText[i] == '\n')
				return i; // \n pos
		}
		return textLen; // \0
	}
}

//ori < 0 앞으로 \n를 찾는다
//ori > 0 뒤쪽으로 \n를 찾는다
int findOneLineEndPos<>(int ori = -1) {
	//if (ori < 0) { // 앞쪽으로 찾음.
	//	for (size_t i = textCaretPos; i > 0; i--) {
	//		if (myText[i] == '\n')
	//			return i + 1; // \n pos + 1
	//	}
	//	return 0;
	//}
	//else if (ori > 0) {
	//	for (size_t i = textCaretPos; i < textLen; i++) {
	//		if (myText[i] == '\n')
	//			return i; // \n pos
	//	}
	//	return textLen; // \0
	//}

	return _findOneLineEndPos<>(ori, 1);
}

void calculateCursorXPos<>(HDC hdc) {
	int a = findOneLineEndPos<>();
	GetTextExtentPoint(hdc, myText + a, textCaretPos - a, &caretPos);
}

int length(int a, int b) {
	return a - b > 0 ? a - b : b - a;
}

// Home key job
void CaretGotoLineFirstWord() {
	textCaretPos = findOneLineEndPos<>();
}

void removeOneline() {
	int first = findOneLineEndPos<>();
	int end = findOneLineEndPos<>(1);
	if (end != textLen)
		end++;
	// 커서가 있던 한줄 덮어씌우기
	memcpy(myText + first, myText + end, sizeof(TCHAR)*(textLen - end+1));
	textCaretPos = first;
	textLen -= (end-first);
}

void _insertOneWord(TCHAR ch) {
	if (ch == _T('\0')) {
		if (textCaretPos > 0) {
			if (myText[textCaretPos-1] == '\n') {
				totalCaretYPos--;
				caretYPos--;
			}
			pushAll(textCaretPos, -1);
			--textCaretPos;
			--textLen;
		}
	}
	else {
		//if (myText[textCaretPos] == '\0')
		//	myText[textCaretPos + 1] = '\0';
		pushAll(textCaretPos, 1);
		myText[textCaretPos++] = ch;
		++textLen;
	}
}

void insertOneWord(TCHAR ch) {
	_insertOneWord(ch);
}

void _insertMultipleWord(const TCHAR* chs, int size) {
	pushAll(textCaretPos, size);
	for (int i = 0; i < size; i++)
		myText[textCaretPos++] = chs[i];
	textLen+= size;
}

void insertMultipleWord(const TCHAR* chs, int size) {
	if(myText[textCaretPos] == '\0')
		myText[textCaretPos+size] = '\0';
	_insertMultipleWord(chs, size);
}

void moveVertical(HDC hdc, bool bIsMoveUp) {
	SIZE t_caretPos;
	int minRange = 1000;
	int nextXPos;
	int first, last;

	/* 위로 움직이기 왼쪽끝으로 커서 움직이게 하고 */
	if (bIsMoveUp && caretYPos > 0) {
		textCaretPos = _findOneLineEndPos<>(-1,-1); // 앞쪽 \n으로 감
		first = findOneLineEndPos<>();
		caretYPos--; // 커서 y만 윗줄로 옮김
	}
	else if (!bIsMoveUp && totalCaretYPos > caretYPos)
	{
		textCaretPos = findOneLineEndPos<>(1); // 이번줄 \n이거나 마지막글씨 가리킴
		first = textCaretPos;
		caretYPos++; // 커서 y만 아래줄로 옮김
		if (textCaretPos < textLen) // 마지막글자면 넘기기
			first = ++textCaretPos; // 이번줄 \n였다면 다음줄 처음으로 옮기기
	}
	else
		return;
	last = findOneLineEndPos<>(1); // 이번줄 \n이거나 마지막글씨 가리킴

	for (int i = 0; i <= last - first; i++)
	{
		GetTextExtentPoint(hdc, myText + first, i, &t_caretPos);
		int t = length(caretPos.cx, t_caretPos.cx);
		if (t <= minRange) {
			minRange = t;
			nextXPos = first + i;
		}
		else { // t 가 커진다면 원하는 커서위치로부터 멀어진다는 뜻이므로 끝내기.
			break;
		}
	}
	textCaretPos = nextXPos;
}

void moveHorizontal(bool bIsRightMove) {
	if (!bIsRightMove) { // 왼쪽으로 가면 위로
		if (textCaretPos >= 1) { // 커서가 1보다 커서 왼쪽으로 갈곳이 있다면
			if (myText[textCaretPos - 1] == '\n')
			{
				textCaretPos = _findOneLineEndPos<>();
				caretYPos--; // 커서 y만 이전줄로 옮김
			}
			else
			{
				textCaretPos--;
			}
		}
	}
	else { // 오른쪽 움직임
		if (textCaretPos < textLen)
			if (myText[textCaretPos] == '\n') {
				++textCaretPos; // 이번줄 \n였다면 다음줄 처음으로 옮기기
				caretYPos++; // 커서 y만 다음줄로 옮김
			}
			else{
				textCaretPos++;
			}
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
    switch (message)
    {
	case WM_CREATE:
		CreateCaret(hWnd, NULL, 5, 10);
		ShowCaret(hWnd);
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
	case WM_CHAR:
		switch(wParam) {
			case VK_RETURN:
				if (totalCaretYPos < 20) {
					insertOneWord(_T('\n'));
					++caretYPos;
					++totalCaretYPos;
				}
				break;
			case VK_BACK:
				insertOneWord(_T('\0'));
				break;
			case VK_ESCAPE:
				removeAll();
				break;
			case VK_TAB:
				insertMultipleWord(_T("    "), 4);
				break;
			default:
				insertOneWord(wParam);
				break;
		}
		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_KEYDOWN:
	{
		HDC hdc = GetDC(hWnd);
		switch (wParam) {
		case VK_HOME:
			CaretGotoLineFirstWord();
			break;
		case VK_DELETE:
			removeOneline();
			break;
		case VK_LEFT:
			moveHorizontal(false);
			break;
		case VK_RIGHT:
			moveHorizontal(true);
			break;
		case VK_UP:
			moveVertical(hdc, true);
			break;
		case VK_DOWN:
			moveVertical(hdc, false);
			break;
		case VK_CAPITAL:
			break;
			ReleaseDC(hWnd, hdc);
		}
		InvalidateRect(hWnd, NULL, true);
		break;
	}
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
			calculateCursorXPos<>(hdc);
			SetCaretPos<>(caretPos.cx, caretYPos*16);
			DrawText(hdc, myText, textLen, &winRect, DT_WORDBREAK);
			EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		DestroyCaret();
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
