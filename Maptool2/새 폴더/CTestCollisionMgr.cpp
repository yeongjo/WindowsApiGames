#include "PublicDef.h"
#include "Draw2DSprite.h"
#include "CPlayerObject.h"
#include "CTestCollisionMgr.h"

//-------------------------------------------------
//
//-------------------------------------------------
void CTestCollisionMgr::Create()
{
	//-------------------------------------------------------
	// 외곽선 영역
	//-------------------------------------------------------
	AddCollisionRect( CRect( 0,		0,		10,  800 ) );
	AddCollisionRect( CRect( 0,		590,	800, 600 ) );
	AddCollisionRect( CRect( 790,	0,		800, 600 ) );
	AddCollisionRect( CRect( 0,		0,		800, 10 ) );

	//-------------------------------------------------------
	// 블럭 나열
	//-------------------------------------------------------
	CRect TempRect( 0, 0, 50, 50 );

	AddCollisionRect( TempRect.Move( 100, 550 ) );
	AddCollisionRect( TempRect.Move( 200, 500 ) );
	AddCollisionRect( TempRect.Move( 300, 450 ) );
	AddCollisionRect( TempRect.Move( 400, 400 ) );
	AddCollisionRect( TempRect.Move( 500, 350 ) );
	AddCollisionRect( TempRect.Move( 600, 300 ) );

#if 0
	AddCollisionRect( CRect( 0, 0, 100, 50 ) );
	AddCollisionRect( CRect( 100, 150, 200, 200 ) );
	AddCollisionRect( CRect( 200, 150, 300, 200 ) );
	AddCollisionRect( CRect( 300, 150, 500, 200 ) );
	AddCollisionRect( CRect( 0, 250, 1000, 700 ) );
	AddCollisionRect( CRect( 500, 0, 1000, 700 ) );
#endif// 
}
//-------------------------------------------------
//
//-------------------------------------------------
void CTestCollisionMgr::AddCollisionRect( const CRect& InRect )
{
	m_CollisionRectList.push_back( new CCollisionRect( InRect ) );
}

//-------------------------------------------------
//
//-------------------------------------------------
bool CTestCollisionMgr::Update( LPDIRECT3DDEVICE9 InD3dDevice, CPlayerObject* InObject )
{
	Draw( InD3dDevice );
	CheckCollision( InObject );

	return true;
}
//-------------------------------------------------
//
//-------------------------------------------------
bool CTestCollisionMgr::Draw( LPDIRECT3DDEVICE9 InD3dDevice )
{
	if( m_CollisionRectList.empty() )
	{	return false;	}

	TypeCollisionRectListIter Iter = m_CollisionRectList.begin();
	for( ; Iter != m_CollisionRectList.end() ; ++Iter )
	{
		CCollisionRect* TempRect = ( *Iter );
		RenderRect( InD3dDevice, *TempRect );
	}
	return true;
}
//-------------------------------------------------
//
//-------------------------------------------------
bool CTestCollisionMgr::CheckCollision( CPlayerObject* InObject )
{
	if( m_CollisionRectList.empty() )
	{	return false;	}

	TypeCollisionRectListIter Iter = m_CollisionRectList.begin();
	
	for( ; Iter != m_CollisionRectList.end() ; ++Iter )
	{
		CCollisionRect* TempCollisionRect = ( *Iter );

		CRect ObjectBoundRect = InObject->GetBoundRect();
		CRect TempRect;
		//---------------------------------------
		// 충돌 체크
		//---------------------------------------
		//CLOG().Log( "%.1f", InObject->GetCurrentVelocity() );

		switch( TempCollisionRect->GetIsCheckCollision( ObjectBoundRect ) )
		{
			case ::TYPE_BLOCK_DIRECT_LEFT:
			{
				InObject->GetBoundRect().Move( TempCollisionRect->Left(), ObjectBoundRect.Top(), TYPE_ALIGN_METHOD_RIGHT_TOP );
				break;
			}
			case ::TYPE_BLOCK_DIRECT_TOP:
			{
				if( InObject->GetCurrentVelocity() >= 0.f )
				{
					InObject->Stop();
					InObject->GetBoundRect().Move( ObjectBoundRect.Left(), TempCollisionRect->Top(), TYPE_ALIGN_METHOD_LEFT_BOTTOM );
				}
				break;
			}
			case ::TYPE_BLOCK_DIRECT_RIGHT:
			{
				InObject->GetBoundRect().Move( TempCollisionRect->Right(), ObjectBoundRect.Top() );
				break;
			}
			case ::TYPE_BLOCK_DIRECT_BOTTOM:
			{
				InObject->GetBoundRect().Move( ObjectBoundRect.Left(), TempCollisionRect->Bottom() );
				break;
			}
			default:
			{	
				InObject->SetIsApplyGravity( TRUE );			
				break;
			}
		}
	}
	return true;
}
//-------------------------------------------------
//
//-------------------------------------------------
CTestCollisionMgr::CTestCollisionMgr()
{
	m_CollisionRectList.reserve( 10 );
}
//-------------------------------------------------
//
//-------------------------------------------------
CTestCollisionMgr::~CTestCollisionMgr()
{
	
}