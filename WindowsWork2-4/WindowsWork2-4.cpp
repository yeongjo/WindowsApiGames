#include <Windows.h>
#include <tchar.h>

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming Lab";

RECT windowSize;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage,WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_ERROR);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground =
		(HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_ERROR);
	RegisterClassEx(&WndClass);
	hWnd = CreateWindow
	(lpszClass, lpszWindowName,
		WS_OVERLAPPEDWINDOW,
		0, 0, 800, 600,
		NULL, (HMENU)NULL,
		hInstance, NULL);

	GetClientRect(hWnd, &windowSize);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);


	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

// 0 : empty
// 1 : player1
// 2 : player2
// 3 : red zone
//char blocks[20 * 20];
int blockSize = 20;
RECT redzoneRect;

bool isPlayer2Turn = false;
static int moveErrorIdx = 0;

class Player {
public:
	int x, y;
	int id;
	COLORREF color;

	void setPos<>(int x, int y) {
		this->x = x;
		this->y = y;
	}

	// 레드존 안에 있다면 true 반환
	// 그러면 죽이고 새로 만들어야 함
	bool isInRedZone() {
		// 레드존 바깥에 있는지 검사한다.
		if (redzoneRect.top > y || redzoneRect.bottom <= y ||
			redzoneRect.left > x || redzoneRect.right <= x) {
			return false;
		}
		return true;
	}

	// 움직일수 없다면 false 반환 (레드존에 들어감)
	// 그러면 새로운 블럭만들어야함.
	bool move(int x, int y) {
		if (!(id == 1 && !isPlayer2Turn ||
			id == 2 && isPlayer2Turn)) {
			moveErrorIdx = id;
			return true;
		}
		moveErrorIdx = 0;
		isPlayer2Turn = !(bool)(id-1);

		this->x += x*blockSize;
		this->y += y * blockSize;

		if (isInRedZone())
			return false;
		return true;
	}

	void render(HDC hdc) {
		HBRUSH brush = (HBRUSH)CreateSolidBrush(color);
		HBRUSH old = (HBRUSH)SelectObject(hdc, brush);
		Ellipse(hdc, x + 1, y + 1, x + 18, y + 18);
		SelectObject(hdc, old);
		DeleteObject(brush);
		
	}
};

Player players[2];

void renderRedZone(HDC hdc) {
	HBRUSH brush = (HBRUSH)CreateSolidBrush(RGB(200, 50, 60));
	//HBRUSH old = (HBRUSH)SelectObject(hdc, brush);
	FillRect(hdc, &redzoneRect, brush);
	//SelectObject(hdc, old);
	DeleteObject(brush);
}
void renderPlayers(HDC hdc) {
	players[0].render(hdc);
	players[1].render(hdc);
}

// 0 : empty
// 1 : player1
// 2 : player2
// 3 : red zone
void renderLines(HDC hdc) {

	for (int i = 0; i < 21; i++)
	{
		MoveToEx(hdc, i*blockSize, 0, NULL);
		LineTo(hdc, i*blockSize, 20*blockSize);
	}
	for (int i = 0; i < 21; i++)
	{
		MoveToEx(hdc, 0, i*blockSize, NULL);
		LineTo(hdc, 20 * blockSize, i*blockSize);
	}
}


SIZE deadPlayerPos[20];
int deadPlayerCount = 0;
void addDeadPlayer(int x, int y) {
	deadPlayerPos[deadPlayerCount++] = SIZE{ x,y };
}

void renderDeadPlayers(HDC hdc) {
	HBRUSH brush = (HBRUSH)CreateSolidBrush(RGB(110,100,100));
	HBRUSH old = (HBRUSH)SelectObject(hdc, brush);
	
	for (int i = 0; i < deadPlayerCount; i++)
	{
		int x = deadPlayerPos[i].cx;
		int y = deadPlayerPos[i].cy;
		Ellipse(hdc, x + 1, y + 1, x + 18, y + 18);
	}
	SelectObject(hdc, old);
	DeleteObject(brush);
}

void initRedZone() { 
	redzoneRect = { blockSize*(10 - 2),blockSize*(10 - 2) ,blockSize*(12) ,blockSize*(12) };
}

void printMoveError(HDC hdc, int idx) {
	if (idx == 2 && !isPlayer2Turn) {
		const TCHAR* temp = _T("Player2가 움직일 차례가 아닙니다.");
		TextOut(hdc, 0, 20 * blockSize + 5, temp, _tcsclen(temp));
	}
	else if(idx == 1 && isPlayer2Turn){
		const TCHAR* temp = _T("Player1이 움직일 차례가 아닙니다.");
		TextOut(hdc, 0, 20 * blockSize + 5, temp, _tcsclen(temp));
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM
	wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	bool player1Moveable = true;
	bool player2Moveable = true;
	int x = 0, y = 0;
	switch (iMessage) {
		// 워하는건 안하는건데
	case WM_CREATE:
		initRedZone();
		//initPlayer
		players[0].color = RGB(50, 200, 60);
		players[0].id = 1;
		players[1].color = RGB(50, 60, 200);
		players[1].id = 2;
		players[1].x = 20 * (blockSize-1);
		/*players[1].x = 20 * blockSize;*/
		break;
	case WM_CHAR:
		switch (wParam) {
			// 플레이어1
			
		case 'w':
			player1Moveable = players[0].move(0, -1);
			break;
		case 'a':
			player1Moveable = players[0].move(-1, 0);
			break;
		case 's':
			player1Moveable = players[0].move(0, 1);
			break;
		case 'd':
			player1Moveable = players[0].move(1, 0);
			break;
			// --------------------
		case 'r': case 'R':
			// 모두 지우기
			players[0].setPos<>(0,0);
			players[1].setPos<>(blockSize*19, 0);
			deadPlayerCount = 0;
			// Clear dead body
			break;
		case 'q': case 'Q':
			// 프로그램 종료
			PostQuitMessage(0);
			break;
		}
		if (!player1Moveable) {
			addDeadPlayer(players[0].x, players[0].y);
			players[0].setPos<>(0,0);
		}
		break;
	case WM_KEYDOWN:
			switch (wParam) {
				// 플레이어2
			case VK_LEFT:
					player2Moveable = players[1].move(-1, 0);
				break;
			case VK_RIGHT:
					player2Moveable = players[1].move(1, 0);
				break;
			case VK_UP:
					player2Moveable = players[1].move(0, -1);
				break;
			case VK_DOWN:
					player2Moveable = players[1].move(0, 1);
				break;

			}
			// --------------------
			if (!player2Moveable) {
				addDeadPlayer(players[1].x, players[1].y);
				players[1].setPos<>(blockSize * 19, 0);
			}
		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		renderRedZone(hdc);
		renderPlayers(hdc);
		renderDeadPlayers(hdc);
		renderLines(hdc);
		printMoveError(hdc, moveErrorIdx);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}