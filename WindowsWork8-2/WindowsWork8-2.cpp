// WindowsWork8-2.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "WindowsWork8-2.h"
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
    LoadStringW(hInstance, IDC_WINDOWSWORK65, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSWORK65));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSWORK65));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSWORK65);
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



HWND hCombo, hList;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HANDLE hFile;
	TCHAR str[100], lpstrFile[100] = L"";
	TCHAR filter[100] = L"소스 File(*.cpp)\0*.cpp\0문서 File \0 *.txt;*.doc \0 ";
	int read_size;
	OPENFILENAME OFN;
    switch (message)
    {
	case WM_CREATE:
	{
		auto t = CreateDialog (hInst, MAKEINTRESOURCE (IDD_ABOUTBOX), hWnd, About);
		ShowWindow (t, SW_SHOW);
	}
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
				int count = SendMessage(hList,LB_GETCOUNT,0,0);
				wstringstream ss;
				for (size_t i = 0; i < count; i++) {
					wchar_t tt [100];
					SendMessage(hList,LB_GETTEXT,i,(LPARAM)tt);
					ss << (tt) << L"\n";
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
					while (tok) {
						SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)tok);
						tok = wcstok_s(NULL, L"\n",&ttt);
					}
				}
				
				
			}   break;
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
#define BUFFER 100
#define XBITMAP 48 
#define YBITMAP 48 

void AddItem (HWND hwnd, PTSTR pstr, HBITMAP hbmp) {
	int lbItem = 0;

	int idx = SendMessage (hwnd, LB_GETTEXT, 0, 0);
	SendMessage (hwnd, LB_SETITEMDATA, (WPARAM)lbItem, (LPARAM)hbmp);
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static bool isMan = true;
	
	static int birth;
	static HBITMAP hbmpRed, hbmpBlue, hbmpMarker, hbmpPen, hbmpFork;
	static HBITMAP hbmpPicture, hbmpOld;
	HWND hListBox;
	PMEASUREITEMSTRUCT pmis;
	PDRAWITEMSTRUCT pdis;
	HDC hdcMem;
	HBITMAP hbmp;
	TCHAR achBuffer[BUFFER];
	size_t cch;
	int yPos;
	int lbItem;
	TEXTMETRIC tm;
	RECT rcBitmap;
	HRESULT hr;
    UNREFERENCED_PARAMETER(lParam);
	switch (message) {
	case WM_INITDIALOG:{
		CheckRadioButton (hDlg, IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
		hList = GetDlgItem (hDlg, IDC_LIST1);
		hCombo = GetDlgItem (hDlg, IDC_COMBO1);
		auto tt = LBS_SORT;
		//SetWindowLong (hList, GWL_STYLE, (LBS_NOTIFY | WS_BORDER));
		//SetWindowPos (hList, hDlg, 10, 10, 600, 600, SWP_SHOWWINDOW| SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOMOVE);
		for (size_t i = 0; i < 10; i++) {
			wstringstream ss;
			ss << i + 1990;
			SendMessage (hCombo, CB_ADDSTRING, 0, (LPARAM)ss.str ().c_str ());
		}
		HDC m_hdc = GetDC (hDlg);
		HDC hdc = CreateCompatibleDC (m_hdc);
		auto old = SelectObject (hdc, hbmpRed);
		auto brush = CreateSolidBrush (RGB (255, 0, 0));
		auto oldB = SelectObject (hdc, brush);

		Rectangle (hdc, 0, 0, 1000, 1000);

		DeleteObject (brush);
		DeleteDC (hdc);

		hdc = CreateCompatibleDC (m_hdc);
		old = SelectObject (hdc, hbmpBlue);
		brush = CreateSolidBrush (RGB (0, 0, 200));
		oldB = SelectObject (hdc, brush);

		Rectangle (hdc, 0, 0, 1000, 1000);

		DeleteObject (brush);
		DeleteDC (hdc);
		//hbmpPencil = LoadBitmap (hInst, MAKEINTRESOURCE (IDB_PENCIL));
		//hbmpCrayon = LoadBitmap (hInst, MAKEINTRESOURCE (IDB_CRAYON));
		//hbmpMarker = LoadBitmap (hInst, MAKEINTRESOURCE (IDB_MARKER));
		//hbmpPen = LoadBitmap (hInst, MAKEINTRESOURCE (IDB_PEN));
		//hbmpFork = LoadBitmap (hInst, MAKEINTRESOURCE (IDB_FORK));

		return (INT_PTR)TRUE;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_RADIO1:
			isMan = true;
			break;
		case IDC_RADIO2:
			isMan = false;
			break;
		case IDC_BUTTON1:
			// 정렬
		{
			vector<wstring> sv;
			sv.resize(SendMessage (hList, LB_GETCOUNT, 0,0));
			for (size_t i = 0; i < sv.size(); i++) {
				TCHAR a[100];
				SendMessage (hList, LB_GETTEXT, (WPARAM)i, (LPARAM)a);
				sv.at (i) = a;
			}
			/*for (size_t i = 0; i < sv.size(); i++) {
				for (size_t j = i; j < sv.size(); j++) {

				}
			}*/
			SendMessage (hList, LB_RESETCONTENT, 0, 0);
			sort (sv.begin (), sv.end (), less<wstring>());
			for (size_t i = 0; i < sv.size (); i++) {
				SendMessage (hList, LB_ADDSTRING, 0, (LPARAM)sv[i].c_str ());
			}
		}
			break;
		case IDC_BUTTON2:
			break;
		case IDC_BUTTON3:
			SetDlgItemText (hDlg, IDC_EDIT1,L"");
			SetDlgItemText (hDlg, IDC_EDIT2, L"");
			break;
		case IDC_BUTTON4:
		{
			TCHAR name[50], phone[50];
			GetDlgItemText (hDlg, IDC_EDIT1, name, 50);
			GetDlgItemText (hDlg, IDC_EDIT2, phone, 50);
			wstringstream ss;
			ss << L"이름:" << name << L", 번호:" << phone << L", 성별:" <<
				(isMan ? L"남자" : L"여자") << L", 출생년도:" << birth;
			SendMessage (hList, LB_ADDSTRING, 0, (LPARAM)ss.str ().c_str ());
			break;
		}
		case IDC_BUTTON5:
		{
			int idx = SendMessage (hList, LB_GETCURSEL, 0, 0);
			SendMessage (hList, LB_DELETESTRING, idx, 0);
			break;
		}
		case IDC_COMBO1: // 콤보박스가 눌려짐
			if (HIWORD (wParam) == CBN_SELCHANGE) // 하나가 선택됨(상태 변경)
				birth = SendMessage (hCombo, CB_GETCURSEL, 0, 0) + 1990;
			break;
		}
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
			PostQuitMessage (0);
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
	case WM_DRAWITEM:

		pdis = (PDRAWITEMSTRUCT)lParam;

		// If there are no list box items, skip this message. 
		if (pdis->itemID == -1) {
			break;
		}

		// Draw the bitmap and text for the list box item. Draw a 
		// rectangle around the bitmap if it is selected. 
		switch (pdis->itemAction) {
		case ODA_SELECT:
		case ODA_DRAWENTIRE:

			// Draw the bitmap associated with the item. 
			//
			// Get the item bitmap.
			hbmpPicture = (HBITMAP)SendMessage (pdis->hwndItem,
				LB_GETITEMDATA, pdis->itemID, 0);

			// Create a compatible device context. 
			hdcMem = CreateCompatibleDC (pdis->hDC);

			// Select the item bitmap into the compatible device
			// context and save the old bitmap.
			hbmpOld = (HBITMAP)SelectObject (hdcMem, hbmpPicture);

			// Copy the bitmap into the compatible device context.
			BitBlt (pdis->hDC,
				pdis->rcItem.left, pdis->rcItem.top,
				pdis->rcItem.right - pdis->rcItem.left,
				pdis->rcItem.bottom - pdis->rcItem.top,
				hdcMem, 0, 0, SRCCOPY);

			// Draw the string associated with the item. 
			//
			// Get the item string from the list box.
			SendMessage (pdis->hwndItem, LB_GETTEXT,
				pdis->itemID, (LPARAM)achBuffer);

			// Get the metrics for the current font.
			GetTextMetrics (pdis->hDC, &tm);

			// Calculate the vertical position for the item string 
			// so that the string will be vertically centered in the 
			// item rectangle.
			yPos = (pdis->rcItem.bottom + pdis->rcItem.top -
				tm.tmHeight) / 2;

			// Get the character length of the item string.
			cch = _tcslen(achBuffer);

			// Draw the string in the item rectangle, leaving a six
			// pixel gap between the item bitmap and the string.
			TextOut (pdis->hDC, XBITMAP + 6, yPos, achBuffer, cch);

			// Clean up.
			SelectObject (hdcMem, hbmpOld);
			DeleteDC (hdcMem);

			// Is the item selected? 
			if (pdis->itemState & ODS_SELECTED) {
				// Set RECT coordinates to surround only the 
				// bitmap. 
				rcBitmap.left = pdis->rcItem.left;
				rcBitmap.top = pdis->rcItem.top;
				rcBitmap.right = pdis->rcItem.left + XBITMAP;
				rcBitmap.bottom = pdis->rcItem.top + YBITMAP;

				// Draw a rectangle around bitmap to indicate 
				// the selection. 
				DrawFocusRect (pdis->hDC, &rcBitmap);
			}
			break;

		case ODA_FOCUS:

			// Do not process focus changes. The focus caret 
			// (outline rectangle) indicates the selection. 
			// The IDOK button indicates the final 
			// selection. 
			break;
		}
		return TRUE;
    }

    return (INT_PTR)FALSE;
}
