#pragma once

//-------------------------------------------------
//
//-------------------------------------------------
//
//-------------------------------------------------
class CPlayerObject;
class CTestCollisionMgr
{
	public:
		typedef std::vector< CCollisionRect* >		TypeCollisionRectList;
		typedef TypeCollisionRectList::iterator		TypeCollisionRectListIter;
	
	public:
		void	Create();

		void	AddCollisionRect( const CRect& InRect );

		bool	Update( LPDIRECT3DDEVICE9 InD3dDevice, CPlayerObject* InObject );
		bool	CheckCollision( CPlayerObject* InObject );
		bool	Draw( LPDIRECT3DDEVICE9 InD3dDevice );

	public:
		CTestCollisionMgr();
		virtual ~CTestCollisionMgr();

	public:

		TypeCollisionRectList	m_CollisionRectList;
};