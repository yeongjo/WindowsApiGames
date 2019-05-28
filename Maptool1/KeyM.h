#pragma once

#include <Windows.h>

// �˳��ϰ� 255�� ������� 109Ű Ű����� ���콺 2��ư�� ������ 111�� ��Ƶ� ����
class KeyM
{
private:
static const int	KEYMAX	=	111;
	static	KeyM*		m_pInstance;			// Ŭ���� ������
			bool		m_bKeyDown[KEYMAX];		// Ű�� ���ȴ��� üũ�� �迭
			bool		m_bKeyUp[KEYMAX];		// Ű�� �������� üũ�� �迭

private:
	KeyM(void);

public:
	~KeyM(void);
	static	KeyM*		GetInst(void);			// �ν��Ͻ� ����
			void		FreeInst(void);			// �ν��Ͻ� �ı�
			bool		StayKeyDown( int nKey );// Ű�� ������ �ִ��� üũ
			bool		OnceKeyDown( int nKey );// Ű�� �ѹ� ���ȴ��� üũ
			bool		OnceKeyUp( int nKey );	// Ű�� �ѹ� ���ȴ� �������� üũ
			bool		IsToggleKey( int nKey );// �ѹ� ���������� on off�� �ٲ�
};