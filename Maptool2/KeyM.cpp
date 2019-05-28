#include "KeyM.h"

KeyM		*KeyM::m_pInstance	=	NULL;	// 스태틱 맴버변수는 클래스 내에서 초기화 할 수 없어 밖에서 초기화 하도록 한다.

KeyM::KeyM(void)
{
	ZeroMemory( m_bKeyDown, KEYMAX );
	ZeroMemory( m_bKeyUp, KEYMAX );
	// 키 버퍼들을 초기화 시킨다.
}

KeyM::~KeyM(void)
{
}

KeyM*	KeyM::GetInst(VOID)
{
	// 싱글톤 패턴을 사용함
	if( m_pInstance == NULL )
	{
		m_pInstance	=	new KeyM;
	}

	return m_pInstance;
}
VOID	KeyM::FreeInst(VOID)
{
	if( m_pInstance != NULL )
	{
		delete m_pInstance;
		m_pInstance	=	NULL;
	}
}

//	계속 눌리는지
bool	KeyM::StayKeyDown( INT nKey )
{
	if( GetAsyncKeyState( nKey ) & 0x8000 )
	{
		return TRUE;
	}

	return FALSE;
}

//	한번만 눌리는지 체크
bool	KeyM::OnceKeyDown( INT nKey )
{
	if( GetAsyncKeyState( nKey ) & 0x8000 )
	{
		//	눌리는 순간 true
		if( m_bKeyDown[nKey] == false )
		{
			m_bKeyDown[nKey]	=	true;
			return TRUE;
		}
	}

	else
	{
		//	떼는 순간 false
		m_bKeyDown[nKey]	=	false;
	}	

	return FALSE;
}

//	한번 눌렸다가 띄어지는지 체크
bool	KeyM::OnceKeyUp( INT nKey )
{
	if( GetAsyncKeyState( nKey ) & 0x8000 )
	{	
		m_bKeyUp[nKey]	=	true;
	}

	else
	{
		// 눌려진 순간 true가 되므로 true일때만 진입
		if( m_bKeyUp[nKey] == true )
		{
			//	키상태를 false로 만들어 재진입 불가
			m_bKeyUp[nKey]	=	false;

			return TRUE;
		}
	}	

	return FALSE;
}

//	토글키
bool	KeyM::IsToggleKey( INT nKey )
{
	if( GetKeyState(nKey) & 0x0001 ) // GetKeyState의 0x0001 은 이전에 눌렸는가 를 체크한다.
	{
		return TRUE;
	}

	return FALSE;
}