// SpecialGame1.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "SpecialGame1.h"
#include "MyMain.h"

#define MAX_LOADSTRING 100

int deltatime = 12;;

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
    LoadStringW(hInstance, IDC_SPECIALGAME1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPECIALGAME1));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPECIALGAME1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SPECIALGAME1);
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

class Obj;
class WorldM;

class ObjM {
	friend Obj;
	friend WorldM;

	vector<vector<Obj*>> objs;

	ObjM() {
		// Block bullet
		// 바닥에 그리는 격자
		// Guy 터지는효과
		resizeLayer(3);
	}

	void reset() {
		for (size_t i = 0; i < objs.size(); i++) {
			for (size_t j = 0; j < objs[i].size(); j++) {
				delete objs [i][j];
			}
			objs [i].clear();
		}
	}

	void resizeLayer(int layerCount) {
		objs.resize(layerCount);
	}
	
	// +1 -1 로 추가할수있게하기
	// TODO 0넣으면 초기화
	void editlayer(int i) {

	}

	void addObj (Obj* obj, int layer) {
		assert(objs.size() > layer);
		objs[layer].push_back (obj);
	}
	
public:
	static ObjM self;
	void update ();
	void render(HDC hdc);
	void destoryObj(Obj *obj);
};

ObjM ObjM::self;

WindowM win;

int blockSize = 32;

class Obj {
public:
	Pos<float> p;
	Pos<float> off;

	int size = 32;
	int h_size = 16;
	COLORREF color = RGB(20, 20, 20);

	bool isAble = true;

	Obj (int layer = 0) {
		addToM(layer);
	}
	void addToM(int layer) {
		ObjM::self.addObj (this, layer);
	}
	virtual ~Obj() {
		ObjM::self.destoryObj(this);
	}
	virtual void update () {}
	virtual void render(HDC hdc){
		renderRect(hdc, (int)p.x, (int)p.y, size, size, RGB(20, 20, 20));
	}
	virtual int collObj(Obj *o) {
		if (o == nullptr) return 0;
		// 나갔을경우
		if ((p.x > o->p.x + o->size || p.x + size < o->p.x) ||
			(p.y > o->p.y + o->size || p.y + size < o->p.y))
			return 0;
		return 1;
	}
	virtual void takeDamage(int damage) {

	}
};

void ObjM::render(HDC hdc) {
	for (size_t i = 0; i < objs.size(); i++) {
		for (size_t j = 0; j < objs[i].size(); j++) {
			objs[i][j]->render(hdc);
		}
	}
}

class StaticObj :public Obj{
public:
	StaticObj():Obj(){}
	virtual void render (HDC hdc) {
	}
};

class MovableObj :public Obj {
public:
	MovableObj(int layer):Obj(layer){}
	virtual ~MovableObj() {}
	float speed = 3;
	virtual void move () {

	}
	virtual void render (HDC hdc) {
	}
};


class Block : public StaticObj {
public:
	Block(Pos<float> pos) : StaticObj() {
		color = RGB(20, 20, 20);
		p = pos;
	}

	Block(float x, float y) : StaticObj() {
		color = RGB(20, 20, 20);
		p = Pos<float>(x,y);
	}

	// TODO destroy 도 하나식 붙여주기 매니저로 부터 떨어트리기 용
	virtual void destory() {
		isAble = false;
	}

	virtual void render(HDC hdc) {
		if (!isAble) return;
		renderRect(hdc, p.x + off.x, p.y+ off.y, size, size, color);
	}
};

class AssBlock :public Block {
public:
	AssBlock(float x, float y) : Block(x,y) {
		color = RGB(80, 50, 20);
		p = Pos<float>(x, y);
	}

	// 부술수 없음
	virtual void destory() {
	}
};

class Enemy; class Player; class DropedBullet;

class WorldM {
public:
	vector<Block *> blocks;
	vector<Enemy *> enemy; // TODO 삭제 더 편한방법없을까
	vector<DropedBullet *> dropedBullets;
	Player *player = nullptr;

	// 위쪽 ui 때문에
	int blockYOffset = 64;

	int enemyGenSec = 2000;
	int timerEnemyGen;

	int level = 0;

	WorldM() {
		// Timer for enemy spawn
		timerEnemyGen = MTimer::create(enemyGenSec, true, true);
	}

	void initLevel(int level);

	// 맵 읽어와서 만듬
	void createBlocks(int level) {
		// 초기화 ========================
		ObjM::self.reset();
		player = nullptr;
		//for (size_t i = 0; i < blocks.size(); i++) {
		//	//ObjM::self.destoryObj((Obj*)blocks [i]);
		//	delete blocks [i];
		//}
		blocks.clear();

		//for (size_t i = 0; i < enemy.size(); i++) {
		//	//ObjM::self.destoryObj((Obj*)enemy [i]);
		//	delete enemy [i];
		//}
		enemy.clear();
		// ====== ========================

		stringstream ss;
		int yLine = 0;
		ifstream is;

		switch (level) {
		case 0:
			break;
		case 1:
		{
			is.open("map1.txt");
			break;
		}
		case 2:
			is.open("map2.txt");
			break;
		}

		if (is.is_open()) {
			string s;
			while (getline(is, s)) {
				for (size_t i = 0; i < s.size(); i++) {
					switch (s[i]) {
					case '0':
						break;
					case '1':
						blocks.push_back(new Block(i * blockSize, blockSize * yLine + blockYOffset));
						break;
					case '2':
						blocks.push_back(new AssBlock(i * blockSize, blockSize * yLine + blockYOffset));
						break;
					}
				}
				++yLine;
			}
		}
	}


	void destoryAroundBlock(Pos<float>& p, float len = 3);

	void playerDie();

private:
	int playerRegenTimer = -1;

	void shakeEffect() {
		auto a = randomCircle(10);
		for (size_t i = 0; i < ObjM::self.objs.size(); i++) {
			for (size_t j = 0; j < ObjM::self.objs[i].size(); j++) {
				ObjM::self.objs [i][j]->off = a;
			}
		}
	}

	int initRemainShake = 4;
	int remainShake = initRemainShake;

	void shakeUpdate() {
		if (remainShake > 0) {
			--remainShake;
			if(remainShake == 0)
				for (size_t i = 0; i < ObjM::self.objs.size(); i++) {
					for (size_t j = 0; j < ObjM::self.objs[i].size(); j++) {
						ObjM::self.objs [i][j]->off.setZero();
					}
				}
			else
				shakeEffect();
		}
	}
public:
	void startShake() {
		remainShake = initRemainShake;
	}
	
	void addEnemy();

	void addBlock(Pos<float> pos) {
		blocks.push_back(new Block(pos));
	}

	bool isPlayerDead = false;
	void update();

	void setPlayerMatchless();

	void dropItem(int x, int y, int type = 0);
};

WorldM wm;

class Guy;

class Bullet :public MovableObj {
public:
	Pos<float> direc;
	int trailSize = 8;
	float trailCount = 0;
	int damage = 1;

	bool isPlayerBullet = true;

	Guy *owner;

	Bullet() : MovableObj(0){
		init();
		color = RGB(0, 0, 0);
		//isAble = false;
	}

	Bullet(Bullet *bullet): MovableObj(0) {
		init();
		color = bullet->color;
	}

	virtual ~Bullet() {
	}
	// 자동으로 정의된 복사생성자에서 부모 생성자가 불리지않는문제있음


	virtual void init() {
		isAble = false;
		speed = 8;
		size = 10;
	}

	virtual void update();

	virtual void shot(Guy *own, vector<unique_ptr<Bullet>> &bullets, Pos<float> direc, unique_ptr<Bullet> bul);

private:
	Obj *checkIsColl();

public:
	virtual void removeBullet();

	virtual void render(HDC hdc) {
		if (!isAble)return;
		COLORREF t_color = color;
		Pos<float> t_direc = direc * 6;
		for (size_t i = 0; i < trailCount; i++) {

			HGDIOBJ  ori = SelectObject(hdc, GetStockObject(DC_PEN));
			SetDCPenColor(hdc, t_color);

			renderRect(hdc, p.x + off.x - t_direc.x * i, p.y + off.y - t_direc.y * i, size, size, t_color);
			BYTE r = GetRValue(t_color) + 255 / trailSize;
			BYTE g = GetGValue(t_color) + 255 / trailSize;
			BYTE b = GetBValue(t_color) + 255/ trailSize;
			r = r > 255 ? 255 : r;
			g = g > 255 ? 255 : g;
			b = b > 255 ? 255 : b;
			t_color = RGB(r, g, b);

			SelectObject(hdc, ori);
		}
	}
};

class DieAnimation : public StaticObj {
public:
	int _remain = 200;
	int _time = 0;

	int decre = 1;

	DieAnimation(int x, int y) :StaticObj() {
		p.set(x, y);
		size = 50;
		color = RGB(180, 180, 180);
		wm.startShake();

		wm.destoryAroundBlock(p);
	}

	virtual void update();

	virtual void render(HDC hdc) {
		SetROP2(hdc, R2_MASKPEN);
		renderCircle(hdc, p.x - size, p.y - size, size * 2, color);
		SetROP2(hdc, R2_COPYPEN);
	}
};

class Guy : public MovableObj {
public:
	int speed = 3;
	vector<unique_ptr<Bullet>> bullet; // 날아가는 총알
	vector<unique_ptr<Bullet>> hasBullet; // 소지한 총알
	Pos<float> move_vec;
	Pos<float> last_vec = Pos<float>(1,0);
	float bulletDelay = 1000;
	float regenBulletDelay = 1500;
	int bulletDelayIdx = -1;

	int hp = 1;
	int maxHp = 1;
	bool isMatchless = false;

	int remainBullet = 0;

	int regenBulletTimer;

	bool isHasGun = false;

	// 왜 새로만든 Enemy들 이게 안불리지.
	Guy(bool _isHasGun = false) : MovableObj(2) {
		// 기본사이즈 32라서 2만큼 더 줄여서 잘보이게 만듬
		isHasGun = _isHasGun;
		size = size - 10;
		color = RGB(20, 20, 20);
		
	}
	virtual ~Guy() {
		int a = 10;
	}

	virtual void init() {
		if(bulletDelayIdx == -1)
			bulletDelayIdx = MTimer::create(bulletDelay, true, true);
		if(isHasGun)
			regenBulletTimer = MTimer::create(regenBulletDelay, true, true);
	}

	virtual void render(HDC hdc) {
		renderRoundRect(hdc, p.x + off.x, p.y + off.y, size, size, 3, 3, color);
		renderBullets(hdc);
	}

	void shot(int team = 0) {
		if (hasBullet.size() == 0 || !MTimer::isEnd(bulletDelayIdx)) return;

		MTimer::reset(regenBulletDelay);

		hasBullet [0]->shot(this, bullet, last_vec, ::move(hasBullet [0]));
		if (team == 1) {
			bullet.back()->isPlayerBullet = false;
		}
		//bullet.push_back(hasBullet[0]);
		hasBullet.erase(hasBullet.begin()); // 쐈으니까 탄창에서 제거
	}

	void updateBullet() {
		if (!isHasGun) return;
		//for (size_t i = 0; i < bullet.size(); i++) {
		//	bullet[i]->update();
		//}
		updateAutoRegenBullet();
	}

	// 죽음 애니메이션
	virtual void die() {
		new DieAnimation(p.x, p.y);
	}

	virtual void takeDamage(int d) {
		if (isMatchless)
			return;
		if ((hp -= d) <= 0) {
			hp = 0;
			die();
		}
	}

	// 정의해서 사용
	virtual void inputUpdate() {}

	virtual void update() {
		inputUpdate();
		updateBullet();
		move(move_vec.x, move_vec.y);

		++time;
	}

	virtual void move(float x, float y) {
		Pos<float> t(x, y);
		p += t;
	}
	
	void updateAutoRegenBullet() {
		// 최대총알 6발이라 5발까지에서만 장전됨
		if(hasBullet.size() < 6)
			if (MTimer::isEnd(regenBulletTimer)) {
				hasBullet.push_back(make_unique<Bullet>());
				hasBullet.back()->isAble = false;
			}
	}

private:
	int time;

	void renderBullets(HDC h) {
		if (!isHasGun) return;
		// 6발이 최대면 60도에 하나씩있으면 됨
		// t /  360
		auto count = hasBullet.size();
		int hsize = size / 2;
		for (size_t i = 0; i < count; i++) {
			int y = sin((i * (360/count)+time) * (PI / 180)) * hsize + p.y + hsize - hasBullet [i]->size/2;
			int x = cos((i * (360/count)+time) * (PI / 180)) * hsize + p.x + hsize - hasBullet [i]->size/2;
			if(i == 0)
				renderCircle(h, x-2, y-2, hasBullet [i]->size + 4, RGB(255,255,255));
			renderCircle(h, x, y, hasBullet [i]->size, hasBullet [i]->color);
		}
	}
};

class Player :public Guy {
public:


	Player() : Guy(true) {
	}

	virtual ~Player() {
		wm.player = nullptr;
	}

	virtual void init() {
		Guy::init();
		isHasGun = true;
		color = RGB(200, 200, 200);
		isAble = true;
		
		p.set(win.getSize().x/2, win.getSize().y/2);
		wm.player = this;
		hp = maxHp;

		// 플레이어 주변의 블럭 부수기
		wm.destoryAroundBlock(p);
		new DieAnimation(p.x, p.y);
	}

	virtual void inputUpdate () {
		move_vec.set(0, 0);
		if(KeyM::keys[0])
			move_vec += Pos<float>(0, -1);
		if (KeyM::keys[1])
			move_vec += Pos<float> (-1, 0);
		if (KeyM::keys[2])
			move_vec += Pos<float> (0, 1);
		if (KeyM::keys[3])
			move_vec += Pos<float> (1, 0);
		if (KeyM::keys[4]) {
			shot();
		}
		if (move_vec.squareLength() > 0) {
			move_vec = move_vec.normalize();
			last_vec = move_vec;
			move_vec *= speed;
			p += move_vec; // 충돌체크를 위해 미리 가있기
		}

		if (int t = checkIsColl()) {
			switch (t) {
			case 1:
				p -= move_vec; // 원래 위치로
				move_vec.setZero();
				break;
			case 2: // 적이랑 박아서 죽음
				die();
				break;
			}
		} else {
			p -= move_vec; // 원래 위치로
		}
	}

	virtual void die() {
		if(isAble)
			Guy::die();
		wm.playerDie();
	}

	// 0: not coll
	// 1: coll wall
	// 2: coll enemy;
	int checkIsColl() {
		if (!isMatchless)
		for (size_t i = 0; i < wm.enemy.size(); i++) {
			if (collObj((Obj*)wm.enemy[i])) return 2; // 적 박음
		}
		for (size_t i = 0; i < wm.blocks.size(); i++) {
			if (wm.blocks[i]->isAble && collObj(wm.blocks[i])) return 1; // 벽이랑 박음
		}
		return 0;
	}

	virtual void update() {
		if (!isAble) return;
		Guy::update();
		getBullet();
	}

	// 특수총알을 주음
	// 충돌했는지 검사하고 주음.
	void getBullet();

	virtual void render (HDC hdc) {
		if (!isAble) return;
		Guy::render(hdc);
		//renderRoundRect (hdc, p.x + off.x, p.y + off.y, size, size, 3, 3, color);
	}
};


class UI {
public:
	int combo = 1;
	int score = 0;
	int level = 1;

	COLORREF color;

	int comboRemainTime;
	int comboInitTime = 3000;


	UI() {
		color = RGB(50, 50, 50);
	}

	void startCombo() {
		comboRemainTime = comboInitTime;
		combo++;
	}

	void update() {
		comboRemainTime -= deltatime;
		if (comboRemainTime < 0) {
			combo = 1;
		}

		if (MTimer::isEnd(broadcastTimer)) {
			broadcastMsg = L"";
		}
	}

	void addScore(int sco) {
		score += sco * combo;
		startCombo();
	}

	int broadcastTimer = -1;
	wstring broadcastMsg;

	void broadcast(wstring str) {
		if(broadcastTimer == -1)
			broadcastTimer = MTimer::create(1300, true, false);
		MTimer::reset(broadcastTimer);
		broadcastMsg = str;
	}

	virtual void render(HDC hdc) {
		Pos<> size = win.getSize();
		renderRect(hdc, 0, 0, size.x, 64, color);
		wstringstream wss;
		wss << score;
		TextOut(hdc, 20, 3,_T("SCORE"), 5);
		TextOut(hdc, 20, 18, wss.str().c_str(), wss.str().size());

		if(wm.player->isMatchless)
			TextOut(hdc, 120, 3,_T("무적모드 활성화"), wcslen(_T("무적모드 활성화")));

		if(broadcastMsg.size() > 0)
			TextOut(hdc, size.x/2-50, 400,broadcastMsg.c_str(), broadcastMsg.size());

		if (wm.player) {
			wss.str(L"");
			wss.clear();
			wss << L"공격 딜레이 : " << wm.player->bulletDelay;
			TextOut(hdc, 100, 16, wss.str().c_str(), wss.str().size());
			wss.str(L"");
			wss.clear();
			wss << L"총알 재생 딜레이 : " << wm.player->regenBulletDelay;
			TextOut(hdc, 100, 16+15, wss.str().c_str(), wss.str().size());
			wss.str(L"");
			wss.clear();
			wss << L"이동 속도: " << wm.player->speed;
			TextOut(hdc, 100, 16+13+15, wss.str().c_str(), wss.str().size());
		}

		wss.str(L"");
		wss.clear();
		wss << L" x" << combo;
		TextOut(hdc, 100, 3, wss.str().c_str(), wss.str().size());

		wss.str(L"");
		wss.clear();
		level = wm.level + 1;
		wss << L"레벨 " << level;
		TextOut(hdc, size.x/2-50, 3, wss.str().c_str(), wss.str().size());

		wss.str(L"");
		wss.clear();
		wss << L"적 리젠 딜레이 " << wm.enemyGenSec;
		TextOut(hdc, size.x/2-50, 18, wss.str().c_str(), wss.str().size());
	}
};

//Player *player;
UI ui;

void ObjM::update() {
	MTimer::update(deltatime);
	for (size_t i = 0; i < objs.size(); i++) {
		for (size_t j = 0; j < objs[i].size(); j++) {
			objs [i][j]->update();
		}
	}
	wm.update();
	ui.update();
}

class CrossBlock : public MovableObj {
public:
	Enemy *owner;

	CrossBlock(Enemy *owner) : MovableObj(1){
		this->owner = owner;
	}

	virtual ~CrossBlock();

	virtual void render(HDC h);
};

class Enemy : public Guy {
public:

	int score = 3;
	CrossBlock *crossBlock = nullptr;

	Enemy() : Guy(){
		speed = 3;
		color = RGB(210, 10, 20);
		if(crossBlock == nullptr)
			crossBlock = new CrossBlock(this);
	}

	virtual ~Enemy() {
		for (size_t i = 0; i < wm.enemy.size(); i++) {
			if (wm.enemy [i] == this) {
				wm.enemy.erase(wm.enemy.begin() + i);
			}
		}
		if (crossBlock != nullptr)
			delete crossBlock;
	}

	void paintBlocks() {
		int _x = (int)p.x / blockSize * blockSize; 
		int _y = (int)p.y / blockSize * blockSize + wm.blockYOffset- blockSize*2;
		bool isCanPlace = true;
		for (size_t i = 0; i < wm.blocks.size(); i++) {
			if (wm.blocks [i]->p == Pos<float>(_x, _y)) {
				isCanPlace = false;
				wm.blocks [i]->isAble = true;
				return;
			}
		}
		if(isCanPlace)
			wm.blocks.push_back(new Block(_x, _y));
		
	}

	// 그냥 쓰는거임 플레이어 쫒아오게
	virtual void inputUpdate() {
		move_vec = wm.player->p - p;
		move_vec = move_vec.normalize();

		paintBlocks();
	}

	virtual void die() {
		Guy::die();
		ui.addScore(score);
		int ran = rand() % 100;
		if(ran > 20){
			int t = 2;
			if (ran > 40)
				t = 4; // 리젠시간 증가
			if (ran > 65)
				t = 3;
			if (ran > 75)
				t = 0;
			if (ran > 90)
				t = 1;
			wm.dropItem(p.x, p.y, t);
		}
			
		//ObjM::self.destoryObj(this);
		delete this;
	}

	virtual void render(HDC h) {
		Guy::render(h);
	}
};

class GunEnemy: public Enemy {
public:
	GunEnemy() :Enemy() {
		isHasGun = true;
		bulletDelay = 3000;
		init();
	}

	virtual void update() {
		Enemy::update();
		last_vec = move_vec;
		shot(1);
	}

	virtual void die() {
		Enemy::die();
		wm.dropItem(p.x, p.y, 1);
	}
};

class BigEnemy : public Enemy {
public:
	BigEnemy() : Enemy() {
		size = size * 2;
		hp = 2;
	}
	virtual void takeDamage(int d) {
		color -= RGB(50, 0, 0);
		Guy::takeDamage(d);
	}
};

CrossBlock::~CrossBlock() {
	owner->crossBlock = nullptr;
}

void CrossBlock::render(HDC h) {
	//SetROP2(h, R2_NOTCOPYPEN); // 검은 배경에 하얀색을 그리고

	//HBRUSH my = CreateHatchBrush(HS_BDIAGONAL, RGB(0, 0, 0));
	//HBRUSH old = (HBRUSH)SelectObject(h, my);
	//int _x = (int)owner->p.x / blockSize * blockSize - blockSize; 
	//int _y = (int)owner->p.y / blockSize * blockSize + wm.blockYOffset - blockSize*3;
	//bool isCanPlace = true;
	//for (size_t x = 0; x < 3; x++) {
	//	for (size_t y = 0; y < 3; y++) {
	//		if (x == 1 && y == 1) continue;
	//		int _w = _x +x* blockSize + blockSize;
	//		int _h = _y +y* blockSize + blockSize;
	//		Rectangle(h, _x+x* blockSize, _y+y* blockSize, _w, _h);
	//	}
	//}
	//SelectObject(h, old);
	//DeleteObject(my);

	SetROP2(h, R2_MASKPEN); // 그 위에 and 연산으로 마스킹해 그린다.
	HBRUSH my = CreateHatchBrush(HS_BDIAGONAL, RGB(100, 100, 230));
	HBRUSH old = (HBRUSH)SelectObject(h, my);
	int _x = (int)owner->p.x / blockSize * blockSize - blockSize; 
	int _y = (int)owner->p.y / blockSize * blockSize + wm.blockYOffset - blockSize*3;
	bool isCanPlace = true;
	for (size_t x = 0; x < 3; x++) {
		for (size_t y = 0; y < 3; y++) {
			if (x == 1 && y == 1) continue;
			int _w = _x +x* blockSize + blockSize;
			int _h = _y +y* blockSize + blockSize;
			Rectangle(h, _x+x* blockSize, _y+y* blockSize, _w, _h);
		}
	}

	SelectObject(h, old);
	DeleteObject(my);
	SetROP2(h, R2_COPYPEN);
}

class Bullet2 : public Bullet {
public:
	Bullet2() :Bullet() {
		color = RGB(50, 120, 255);
	}

	virtual void shot(Guy* own, vector<unique_ptr<Bullet>> &bullets, Pos<float> direc, unique_ptr<Bullet> bul) {
		bullets.push_back(::move(bul));
		auto t = bullets.back().get();
		t->direc = direc;
		int offgenpos = own->size / 2 - this->size / 2;
		t->p = own->p + Pos<float>(offgenpos, offgenpos);
		t->isAble = true;
		t->owner = own;

		bullets.push_back(unique_ptr<Bullet>(new Bullet(*t)));
		t = bullets.back().get();
		t->addToM(0);
 		t->p += direc * speed * 3 + Pos<float>(10,10);
	}
};

void Bullet::shot(Guy* own, vector<unique_ptr<Bullet>> &bullets, Pos<float> direc, unique_ptr<Bullet> bul) {
	bullets.push_back(::move(bul));
	this->direc = direc;
	int offgenpos = own->size / 2 - this->size / 2;
	this->p = own->p + Pos<float>(offgenpos, offgenpos);
	this->isAble = true;
	this->owner = own;
}

// 떨어진 총알은 앞으로 나가지않는다.
class DropedBullet : public StaticObj{
public:
	unique_ptr<Bullet> bullet;

	DropedBullet() : StaticObj() { 
		
	}

	void init(unique_ptr<Bullet> _bul) {
		assert(_bul.get());
		bullet = ::move(_bul);
		size = 8;
	}

	virtual void usePlayerItem(Player *player) {}

	virtual void print() {

	}
	//virtual void update() {

	//}

	virtual void render(HDC hdc) {
		renderRect(hdc, p.x + off.x, p.y + off.y, size, size, bullet->color);
	}
};
class DropedBullet2 : public DropedBullet{
public:
	DropedBullet2() : DropedBullet() { 
		auto _bul = unique_ptr<Bullet>(new Bullet2());
		init(::move(_bul));
	}

	virtual void print() {
					ui.broadcast(L"두발씩 나가는 총알입니다!");

	}

	virtual void render(HDC hdc) {
		renderRect(hdc, p.x + off.x, p.y + off.y, size, size, bullet->color);
		renderRect(hdc, p.x + 6 + off.x, p.y + 6 + off.y, size, size, bullet->color);
	}
};

class DropedAttackSpeedUp : public DropedBullet {
	virtual void render(HDC h) {
		color = RGB(0, 255, 0);
		size = 10;
		renderRect(h, p.x + off.x, p.y + off.y, size, size, color);
		renderRect(h, p.x + off.x - 5, p.y - 4 + off.y, size+10, 4, color);
	}

	virtual void usePlayerItem(Player *player) {
		if(player->bulletDelay > 100)
			player->bulletDelay -= 100;
	}

	virtual void print() {
		ui.broadcast(L"공격속도가 증가합니다!");
	}
};

class DropedSpeedUp : public DropedBullet {
	virtual void render(HDC h) {
		color = RGB(0, 0, 255);
		size = 10;
		renderRect(h, p.x + off.x, p.y + off.y, size, size, color);
		renderRect(h, p.x + off.x - 5, p.y - 4 + off.y, size+10, 4, color);
	}

	virtual void usePlayerItem(Player *player) {
		player->speed ++;
	}

	virtual void print() {
		ui.broadcast(L"이동속도가 증가합니다!");
	}
};

class DropedBulletRegenSpeedUp : public DropedBullet {
	virtual void render(HDC h) {
		color = RGB(255, 0, 9);
		size = 10;
		renderRect(h, p.x + off.x, p.y + off.y, size, size, color);
		renderRect(h, p.x + off.x - 5, p.y - 4 + off.y, size+10, 4, color);
	}

	virtual void usePlayerItem(Player *player) {
		if(player->regenBulletDelay > 100)
			player->regenBulletDelay-= 150;
		MTimer::changeEndTime(player->regenBulletTimer, player->regenBulletDelay);
	}

	virtual void print() {
		ui.broadcast(L"총알 재생 속도가 증가합니다!");
	}
};


class ScatterBullet : public Bullet {
	int scatterRemainTimer = -1;
	vector<unique_ptr<Bullet>> *bullets;
	vector<ScatterBullet *> childs;
public:
	ScatterBullet *root = nullptr;
	shared_ptr<int> myTime;
	
	ScatterBullet() : Bullet() {
		color = RGB(255, 50, 50);
		myTime = shared_ptr<int>(new int(0));
		init();
	}

	virtual ~ScatterBullet() {
		if (scatterRemainTimer != -1) {
			MTimer::setEnd(scatterRemainTimer);
		}
		//if (!root) { // 이게 최정상친구일때만
		//	for (size_t i = 0; i < childs.size(); i++) {
		//		delete childs [i];
		//	}
		//	childs.clear();
		//}
	}

	virtual void init() {
		scatterRemainTimer = MTimer::create(300, false, false);
	}

	// 총알이 화면밖을 나가면 알아서 호출
	virtual void removeBullet() {
		//if(root)
		//	for (size_t i = 0; i < root->childs.size(); i++) {
		//		if (root->childs [i] == this) {
		//			root->childs.erase(root->childs.begin() + i);
		//			break;
		//		}
		//	}
		Bullet::removeBullet();
	}

	virtual void update() {
		if (!isAble) return;
		if (MTimer::isEnd(scatterRemainTimer)) {
			scatterShot();
			scatterRemainTimer = -1;
		}
		*myTime+=1;
		
		if (*myTime > 10000) {
			delete this;
			return;
		}
		Bullet::update();
	}

	void scatterShot() {
		auto t = new ScatterBullet(*this);
		t->childs.clear();
		t->init();
		t->addToM(2);
		t->direc = direc.rotate(60);
 		t->p += direc * speed * 3 + Pos<float>(10,10);
		t->isAble = true;
		t->root = root == nullptr ? this : root;
		//t->root->childs.push_back(t);
		t->myTime = myTime;
		bullets->push_back(unique_ptr<ScatterBullet>(t));

		t = new ScatterBullet(*t);
		t->init();
		t->addToM(2);
		t->direc = direc.rotate(-60);
 		t->p += direc * speed * 3 + Pos<float>(10,10);
		t->isAble = true;
		t->root = root == nullptr ? this : root;
		//t->root->childs.push_back(t);
		t->myTime = myTime;
		bullets->push_back(unique_ptr<ScatterBullet>(t));
	}

	virtual void shot(Guy* own, vector<unique_ptr<Bullet>> &bullets, Pos<float> direc, unique_ptr<Bullet> bul) {
 		bullets.push_back(::move(bul));
		auto t = bullets.back().get();
		t->direc = direc;
		int offgenpos = own->size / 2 - this->size / 2;
		t->p = own->p + Pos<float>(offgenpos, offgenpos);
		t->isAble = true;
		t->owner = own;
		this->bullets = &bullets;
	}
};

class DropedScatterBullet : public DropedBullet{
public:
	DropedScatterBullet() : DropedBullet() { 
		auto _bul = unique_ptr<Bullet>(new ScatterBullet());
		init(::move(_bul));
	}

	virtual void print() {
		ui.broadcast(L"궁극기를 얻었습니다!");
	}
	virtual void render(HDC hdc) {
		renderRect(hdc, p.x + off.x, p.y + off.y, size, size, bullet->color);
		renderRect(hdc, p.x + 6 + off.x, p.y + 6 + off.y, size, size, bullet->color);
	}
};

void WorldM::addEnemy() {
	auto winSize = win.getSize();
	int ran = rand() % 100;
	if (ran < 0) {
		enemy.push_back(new Enemy());

	} else if (ran < 50) {
		enemy.push_back(new BigEnemy());

	} else
		enemy.push_back(new GunEnemy());

	auto e = enemy.back();
	int x = rand() % 300;
	int y = rand() % 300;
	x = x > 150 ? x + winSize.x - 300 : x;
	y = y > 150 ? y + winSize.y - 300 : y;
	e->p.set(x, y);
}

void WorldM::destoryAroundBlock(Pos<float>& p, float len) {
	// 플레이어 주변의 블럭 부수기
	for (size_t i = 0; i < blocks.size(); i++) {
		if ((blocks [i]->p - p).squareLength() < blockSize*blockSize * len) {
			blocks [i]->destory();
		}
	}
}

Obj *Bullet::checkIsColl() {
	for (size_t i = 0; i < wm.blocks.size(); i++) {
			if (collObj(wm.blocks[i])) { // 왜 자꾸 this->가 null이라 하지
				if(dynamic_cast<AssBlock*>(wm.blocks [i])) return wm.blocks [i];
				wm.blocks [i]->destory();
			}
		}
	if (isPlayerBullet) {
		for (size_t i = 0; i < wm.enemy.size(); i++) {
			if (collObj(wm.enemy[i])) return static_cast<Obj *>(wm.enemy[i]);
		}
	} else { // 적의 총알이라면
		if (wm.player)
			if (collObj(wm.player)) return static_cast<Obj *>(wm.player);
	}
	return nullptr;
}

void WorldM::initLevel(int level) {
	createBlocks(level);
	this->level = level;

	if(player == nullptr)
		player = new Player();
		player->init();
}

void WorldM::update() {
	// TODO Player regen, Enemy Gen
	if (MTimer::isEnd(timerEnemyGen)) {
		if (enemyGenSec > 100) {
			enemyGenSec -= 10;
		}
		MTimer::changeEndTime(timerEnemyGen, enemyGenSec);
		addEnemy();
	}
	if (playerRegenTimer == -1 && isPlayerDead && player) {
		// Timer 플레이어 리젠으로 씀
		playerRegenTimer = MTimer::create(1500, false, false);
	} else if (isPlayerDead && MTimer::isEnd(playerRegenTimer)) {
		player->init();
		isPlayerDead = false;
		playerRegenTimer = -1;
	}

	shakeUpdate();
}

void ObjM::destoryObj(Obj *obj) {
	Obj *t;
	for (size_t i = 0; i < objs.size(); i++) {
		for (size_t j = 0; j < objs[i].size(); j++) {
			t = objs [i][j];
			if (t == obj) {
				//delete t;
				objs[i].erase(objs[i].begin() + j);
				return;
			}
		}
	}
}

void WorldM::dropItem(int x, int y, int type) {
	DropedBullet *b;
	switch (type) {
	case 1:
		dropedBullets.push_back(new DropedScatterBullet());
		break;
	case 0:
		dropedBullets.push_back(new DropedBullet2());
		break;
	case 2:
		dropedBullets.push_back(new DropedAttackSpeedUp());
		break;
	case 3:
		dropedBullets.push_back(new DropedSpeedUp());
		break;
	case 4:
		dropedBullets.push_back(new DropedBulletRegenSpeedUp());
		break;
	}
	b = dropedBullets.back();
	b->p.set(x, y);
}

void WorldM::playerDie() {
	isPlayerDead = true;
	player->isAble = false;
}

void Bullet::removeBullet() {
	for (size_t i = 0; i < owner->bullet.size(); i++) {
		if (owner->bullet [i].get() == this) {
			owner->bullet [i].get()->isAble = false; // TODO 해제했는데 안되는 문제때문에 임시방편으 로 써둠
			owner->bullet.erase(owner->bullet.begin() + i);
			break;
		}
	}
	//ObjM::self.destoryObj(this);
	//delete this;
}

void Bullet::update() {
	if (!isAble) {
		return;
	}
	_ASSERT(owner);
	p += direc * speed;
	if (trailCount < trailSize)
		trailCount += speed / trailSize;
	if (auto t = checkIsColl()) {
		t->takeDamage(damage);
		removeBullet();
	}
	else if (p.x < -20 || p.x > 20 + win.getSize().x ||
		p.y < -20 || p.y > 20 + win.getSize().y){
		removeBullet();
	}
}

void WorldM::setPlayerMatchless() {
	if (player)
		player->isMatchless = !player->isMatchless;
}

void Player::getBullet() {
	for (size_t i = 0; i < wm.dropedBullets.size(); i++) {
		if (collObj(wm.dropedBullets [i])) {
			// 적들이 떨궈줄총알
			//auto b = new Bullet(wm.dropedBullets [i]);
			wm.dropedBullets [i]->print();
			if (wm.dropedBullets [i]->bullet.get() == nullptr) {
				// 아이템을 먹음
				wm.dropedBullets [i]->usePlayerItem(this);
				MTimer::changeEndTime(bulletDelayIdx, bulletDelay);
			} else {
				
				hasBullet.push_back(::move(wm.dropedBullets [i]->bullet));
			}
			//ObjM::self.destoryObj(wm.dropedBullets [i]);
			delete wm.dropedBullets [i];
			wm.dropedBullets.erase(wm.dropedBullets.begin() + i);
		}
	}
}



void DieAnimation::update() {
		_time += deltatime;
		if (_time > _remain) {
			size -= decre; // 계속 감소
			color -= RGB(decre * 10, decre * 10, decre * 10);

			if (size < 0) {
				size = 0;
			}
			if (GetRValue(color) < 0) {
				color = 0;
			}
			if (GetRValue(color) <= 0 || size <= 0) {
				// 애니메이션 재생이 끝나면 사라짐
				//ObjM::self.destoryObj(this);
				delete this;
				return;
			}

			++decre;
		}
		Pos<float> tem = p;
		int s = size;
		p -= Pos<float>(size, size);
		size *= 2;
		for (size_t i = 0; i < wm.enemy.size(); i++) {
			if (collObj((Obj*)wm.enemy [i])){
				/*wm.enemy.erase(wm.enemy.begin() + i);
				ObjM::self.destoryObj((Obj*)wm.enemy [i]);*/
				wm.enemy [i]->die();
			}
		}
		size = s;
		p = tem;
	}

void initLevel(int level) {
	wm.initLevel(level);

	wm.dropItem(wm.player->p.x + 32, wm.player->p.y, 1);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		win.init(hWnd);
		SetTimer(hWnd, 0, deltatime, NULL);
		initLevel(1);
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
	case WM_KEYDOWN:
		KeyM::keyFunc(wParam, true);
		break;
	case WM_KEYUP:
		KeyM::keyFunc(wParam, false);
		break;
	case WM_TIMER:
		ObjM::self.update ();
		InvalidateRect(hWnd, NULL, false);
		break;
	case WM_CHAR:
		//player.input (wParam);
		switch (wParam) {
		case 't':
			wm.setPlayerMatchless();
			break;
		case '1':
			initLevel(0);
			break;
			case '2':
			initLevel(1);
			break;
			case '3':
			initLevel(2);
			break;
		}
		break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			HDC h = win.prerender(hdc);
			ObjM::self.render(h);
			ui.render(h);
			//MTimer::debug(h, 800, 0);
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
