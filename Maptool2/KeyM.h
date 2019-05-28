#pragma once

#include <Windows.h>

// 넉넉하게 255를 잡았지만 109키 키보드와 마우스 2버튼을 포함한 111로 잡아도 무관
class KeyM
{
private:
static const int	KEYMAX	=	111;
	static	KeyM*		m_pInstance;			// 클래스 포인터
			bool		m_bKeyDown[KEYMAX];		// 키가 눌렸는지 체크할 배열
			bool		m_bKeyUp[KEYMAX];		// 키가 떼졌는지 체크할 배열

private:
	KeyM(void);

public:
	~KeyM(void);
	static	KeyM*		GetInst(void);			// 인스턴스 생성
			void		FreeInst(void);			// 인스턴스 파괴
			bool		StayKeyDown( int nKey );// 키가 눌리고 있는지 체크
			bool		OnceKeyDown( int nKey );// 키가 한번 눌렸는지 체크
			bool		OnceKeyUp( int nKey );	// 키가 한번 눌렸다 떼졌는지 체크
			bool		IsToggleKey( int nKey );// 한번 눌릴때마다 on off로 바뀜
};