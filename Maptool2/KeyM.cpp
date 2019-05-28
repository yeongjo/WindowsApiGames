#include "KeyM.h"

KeyM		*KeyM::m_pInstance	=	NULL;	// ����ƽ �ɹ������� Ŭ���� ������ �ʱ�ȭ �� �� ���� �ۿ��� �ʱ�ȭ �ϵ��� �Ѵ�.

KeyM::KeyM(void)
{
	ZeroMemory( m_bKeyDown, KEYMAX );
	ZeroMemory( m_bKeyUp, KEYMAX );
	// Ű ���۵��� �ʱ�ȭ ��Ų��.
}

KeyM::~KeyM(void)
{
}

KeyM*	KeyM::GetInst(VOID)
{
	// �̱��� ������ �����
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

//	��� ��������
bool	KeyM::StayKeyDown( INT nKey )
{
	if( GetAsyncKeyState( nKey ) & 0x8000 )
	{
		return TRUE;
	}

	return FALSE;
}

//	�ѹ��� �������� üũ
bool	KeyM::OnceKeyDown( INT nKey )
{
	if( GetAsyncKeyState( nKey ) & 0x8000 )
	{
		//	������ ���� true
		if( m_bKeyDown[nKey] == false )
		{
			m_bKeyDown[nKey]	=	true;
			return TRUE;
		}
	}

	else
	{
		//	���� ���� false
		m_bKeyDown[nKey]	=	false;
	}	

	return FALSE;
}

//	�ѹ� ���ȴٰ� ��������� üũ
bool	KeyM::OnceKeyUp( INT nKey )
{
	if( GetAsyncKeyState( nKey ) & 0x8000 )
	{	
		m_bKeyUp[nKey]	=	true;
	}

	else
	{
		// ������ ���� true�� �ǹǷ� true�϶��� ����
		if( m_bKeyUp[nKey] == true )
		{
			//	Ű���¸� false�� ����� ������ �Ұ�
			m_bKeyUp[nKey]	=	false;

			return TRUE;
		}
	}	

	return FALSE;
}

//	���Ű
bool	KeyM::IsToggleKey( INT nKey )
{
	if( GetKeyState(nKey) & 0x0001 ) // GetKeyState�� 0x0001 �� ������ ���ȴ°� �� üũ�Ѵ�.
	{
		return TRUE;
	}

	return FALSE;
}