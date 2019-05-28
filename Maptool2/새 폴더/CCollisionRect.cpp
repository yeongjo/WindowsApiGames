#include "PublicDef.h"
#include "CCollisionRect.h"

//------------------------------------------------
//
//------------------------------------------------
BOOL CCollisionRect::Create( const CRect& InRect )
{
	//----------------------------
	// 4 방향
	//----------------------------
	const float CheckWidth	= InRect.Width() / 2;
	const float CheckHeight	= InRect.Height() / 2;
	
	CRect TempSourceRect = InRect;

	//-----------------------
	//
	//-----------------------
	CDirectRect* TempLeftRect	= NULL;
	CDirectRect* TempTopRect	= NULL;
	CDirectRect* TempRightRect	= NULL;
	CDirectRect* TempBottomRect	= NULL;
	// 왼쪽
	TempLeftRect	= new CDirectRect( CRect( InRect.Left() - CheckWidth, 
											InRect.Top(), 
											InRect.Left(), 
											InRect.Bottom() ),
											TYPE_BLOCK_DIRECT_LEFT );

	// 위쪽
	TempTopRect		= new CDirectRect( CRect( InRect.Left(), 
											InRect.Top() - CheckHeight, 
											InRect.Right(), 
											InRect.Top() ),
											TYPE_BLOCK_DIRECT_TOP );

	// 오른쪽
	TempRightRect	= new CDirectRect( CRect( InRect.Right(), 
											InRect.Top(), 
											InRect.Right() + CheckWidth, 
											InRect.Bottom() ),
											TYPE_BLOCK_DIRECT_RIGHT );

	// 아래쪽
	TempBottomRect	= new CDirectRect( CRect( InRect.Left(),
											InRect.Bottom(), 
											InRect.Right(), 
											InRect.Bottom() + CheckHeight ),
											TYPE_BLOCK_DIRECT_BOTTOM );

	m_RectList.push_back( TempLeftRect );
	m_RectList.push_back( TempTopRect );
	m_RectList.push_back( TempRightRect );
	m_RectList.push_back( TempBottomRect );

	return TRUE;
}

//------------------------------------------------
//
//------------------------------------------------
int CCollisionRect::GetIsCheckCollision( const CRect& InRect )
{
	//----------------------------------
	//
	//----------------------------------
	CRect DummyRect;
	bool IsCollision = GetClippingRect( (*this), InRect, DummyRect );

	if( !IsCollision )
	{	return -1;	}

	CRect TempCheckRect = InRect;
	
	TypeRectList TempRectList;

	CRect TempOutRect;
	TypeRectListIter Iter = m_RectList.begin();
	for( Iter ; Iter != m_RectList.end()  ; ++Iter )
	{
		CDirectRect* TempRect	= (*Iter);//m_RectList.at( i );
		bool IsCollision		= GetClippingRect( TempCheckRect, *TempRect, TempOutRect );
		if( IsCollision )
		{
			TempOutRect;
			TempRectList.push_back( new CDirectRect( TempOutRect, TempRect->GetDirect() ) );
		}
	}
	int Count = TempRectList.size();
	if( Count <= 0 )
	{	return -1;	}

	//--------------------------------------
	// 가장 충돌 영역이 넓은 것을 찾는다
	//--------------------------------------
	TempRectList.sort( Compare );

	CDirectRect* TempRect = (*TempRectList.begin() );
	
	int TempDirect = static_cast< int >( TempRect->GetDirect() );

	return TempDirect;
}
//------------------------------------------------
//
//------------------------------------------------
bool CCollisionRect::Compare( const CDirectRect* InRect1st, const CDirectRect* InRect2nd )
{
	return ( InRect1st->GetSize() > InRect2nd->GetSize() );
}
//------------------------------------------------
//
//------------------------------------------------
CCollisionRect::CCollisionRect( const CRect& InRect )
:CRect( InRect )
{
	Create( InRect );
}
//------------------------------------------------
//
//------------------------------------------------
CCollisionRect::~CCollisionRect()
{

}

//--------------------------------------
//
//--------------------------------------
TYPE_BLOCK_DIRECT CDirectRect::GetDirect()
{
	return m_Direct;
}
//--------------------------------------
//
//--------------------------------------
void CDirectRect::SetDirect( TYPE_BLOCK_DIRECT InDirect )
{
	m_Direct = InDirect;
}

//--------------------------------------
//
//--------------------------------------
CDirectRect::CDirectRect( const CRect& InRect, TYPE_BLOCK_DIRECT InDirect )
:CRect( InRect )
{	
	SetDirect( InDirect );
}
//--------------------------------------
//
//--------------------------------------
CDirectRect::~CDirectRect()
{

}



