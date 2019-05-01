// WindowsWorks6-4.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "../MyMain.h"
#include "WindowsWorks6-4.h"
#include <stack>

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
    LoadStringW(hInstance, IDC_WINDOWSWORKS64, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSWORKS64));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSWORKS64));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSWORKS64);
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

class Num {
public:
	// num or 부호
	bool isNum;
	float n = -1;
	/*
	0:+
	1:-
	2:*
	3:/
	*/
	int sign = -1;

	Num(float t) : n(t), isNum(true){}
	Num(int t) : sign(t), isNum(false) {}
};

string infix;


float calcate(float a, float b, char c) {
	switch (c) {
	case '+':
		return a + b;
		break;
	case '-':
		return a - b;
		break;
	case '*':
		return a * b;
		break;
	case '/':
		return a / b;
		break;
	}
}

// Simply determine if character is one of the four standard operators.
bool isOperator(char  character) {
	if (character == '+' || character == '-' || character == '*' || character == '/') {
		return true;
	}
	return false;
}


// If the character is not an operator or a parenthesis, then it is assumed to be an operand.
bool isOperand(char character) {
	if (!isOperator(character) && character != '(' && character != ')' && character != '.') {
		return true;
	}
	return false;
}


// Compare operator precedence of main operators.
// Return 0 if equal, -1 if op2 is less than op1, and 1 if op2 is greater than op1.
int compareOperators(char op1, char op2) {
	if ((op1 == '*' || op1 == '/') && (op2 == '+' || op2 == '-')) { return -1; }
	else if ((op1 == '+' || op1 == '-') && (op2 == '*' || op2 == '/')) { return 1; }
	return 0;
}

void addChar(HWND hdlg, char c) {
	char ch[256];
	GetDlgItemText(hdlg, IDC_EDIT1, ch, 100);
	infix = ch;
	if(c == '\0')
		infix.erase(infix.size() - 1);
	infix += c;
	SetDlgItemText(hdlg, IDC_EDIT1, infix.c_str());
}

string convertPostfix(string infix) {
	stack<char> opStack;
	string postFixString = "";

	// Get a pointer to our character array.
	char* cPtr = const_cast<char*>(infix.c_str());

	// Loop through the array (one character at a time) until we reach the end of the string.
	while (*cPtr != '\0') {
		// If operand, simply add it to our postfix string.
		// If it is an operator, pop operators off our stack until it is empty, an open parenthesis or an operator with less than or equal precedence.
		if (isOperand(*cPtr)) { postFixString += *cPtr; }
		else if (isOperator(*cPtr)) {
			postFixString += ' ';
			while (!opStack.empty() && opStack.top() != '(' && compareOperators(opStack.top(), *cPtr) <= 0) {
				postFixString = postFixString + opStack.top() + " ";
				opStack.pop();
			}
			opStack.push(*cPtr);
		}
		// Simply push all open parenthesis onto our stack
		// When we reach a closing one, start popping off operators until we run into the opening parenthesis.
		else if (*cPtr == '(') { opStack.push(*cPtr); }
		else if (*cPtr == ')') {
			while (!opStack.empty()) {
				if (opStack.top() == '(') { opStack.pop(); break; }
				postFixString = postFixString + opStack.top() + " ";
				opStack.pop();
			}
		}

		// Advance our pointer to next character in string.
		cPtr++;
	}

	// After the input expression has been ran through, if there is any remaining operators left on the stack
	// pop them off and put them onto the postfix string.
	while (!opStack.empty()) {
		postFixString = postFixString + opStack.top() + " ";
		opStack.pop();
	}


	// Show the postfix string at the end.
	return postFixString;
}

void calculate(HWND hdlg) {
	int i = 0;
	int ten = 10;
	int myNum = 0;
	stack<Num> mStack;
	auto postfix = convertPostfix(infix);
	do {
		if (postfix.size() <= i + 1)
			break;
		if (isOperator(postfix[i])) {
			if (myNum != 0) {
				mStack.push(Num((float)myNum));
				myNum = 0;
			}
			//calculate
			auto b = mStack.top(); mStack.pop();
			auto a = mStack.top(); mStack.pop();
			auto c = calcate(a.n, b.n, postfix[i]);
			mStack.push(Num((float)c));
		}
		else if (postfix[i] == ' ') {
			if (myNum != 0) {
				mStack.push(Num((float)myNum));
				myNum = 0;
			}
		}
		else {
			myNum *= ten;
			myNum += postfix[i] - 48;
		}
		i++;
	} while (!mStack.empty() || myNum);
	stringstream ss;
	ss << mStack.top().n; mStack.pop();
	infix = ss.str();
	SetDlgItemText(hdlg, IDC_EDIT1, infix.c_str());
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
	{
		auto hdlg = CreateDialog (hInst, MAKEINTRESOURCE (IDD_ABOUTBOX), hWnd, About);
		ShowWindow (hdlg, SW_SHOW);
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
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
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
		switch (LOWORD(wParam)) {
		case IDC_EDIT1:

			break;
		case IDC_BUTTON19:
			// c
			infix.clear();
			SetDlgItemText(hDlg, IDC_EDIT1, infix.c_str());
			break;
		case IDC_BUTTON20: {
			// hex
			stringstream ss(infix.c_str());
			stack<int> sta;
			int a;
			ss >> a;
			while (a) {
				sta.push(a % 8);
				a /= 8;
			}
			stringstream t_ss;
			while (!sta.empty()) {
				t_ss << sta.top(); sta.pop();
			}
			infix = t_ss.str();
			SetDlgItemText(hDlg, IDC_EDIT1, infix.c_str());
			break;
		}
		case IDC_BUTTON21: {
			// *10
			stringstream ss(infix.c_str());
			int a;
			ss >> a;
			a *= 10;
			ss.str("");	ss.clear();
			ss << a;
			infix = ss.str();
			SetDlgItemText(hDlg, IDC_EDIT1, infix.c_str());
			break;
		}
		case IDC_BUTTON22:{
			// <-
			stringstream ss(infix.c_str());
			int a;
			ss >> a;
			a /= 10;
			ss.str(""); ss.clear();
			ss << a;
			infix = ss.str();
			SetDlgItemText(hDlg, IDC_EDIT1, infix.c_str());
			break;
		}
		case IDC_BUTTON23: {
			// 지수승
			stringstream ss(infix.c_str());
			int a, i = 0;
			ss >> a;
			while (a) {
				a /= 10; ++i;
			}
			string t = ss.str();
			stringstream t_ss;
			t_ss << ss.str()[0] << '.' << ss.str().c_str() + 1 << "e+" << i-1;
			infix = t_ss.str();
			SetDlgItemText(hDlg, IDC_EDIT1, infix.c_str());
			break;
		}
		case IDC_BUTTON17:
			// reverse
			reverse(infix.begin(), infix.end());
			SetDlgItemText(hDlg, IDC_EDIT1, infix.c_str());
			break;
		case IDC_BUTTON18:
			// ce
			addChar(hDlg, '\0');
			break;
		case IDC_BUTTON16:
			addChar(hDlg, '/');
			break;
		case IDC_BUTTON15:
			addChar(hDlg, '*');
			break;
		case IDC_BUTTON14:
			addChar(hDlg, '-');
			break;
		case IDC_BUTTON13:
			addChar(hDlg, '+');
			break;
		case IDC_BUTTON10:
			addChar(hDlg, '7');
			break;
		case IDC_BUTTON11:
			addChar(hDlg, '8');
			break;
		case IDC_BUTTON12:
			addChar(hDlg, '9');
			break;
		case IDC_BUTTON9:
			addChar(hDlg, '6');
			break;
		case IDC_BUTTON8:
			addChar(hDlg, '5');
			break;
		case IDC_BUTTON7:
			addChar(hDlg, '4');
			break;
		case IDC_BUTTON6:
			addChar(hDlg, '3');
			break;
		case IDC_BUTTON5:
			addChar(hDlg, '2');
			break;
		case IDC_BUTTON4:
			addChar(hDlg, '1');
			break;
		case IDC_BUTTON3:
			calculate(hDlg);
			//addChar(hDlg, '=');
			break;
		case IDC_BUTTON2:
			addChar(hDlg, '.');
			break;
		case IDC_BUTTON1:
			addChar(hDlg, '0');
			break;
		}
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
			PostQuitMessage(0);
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
