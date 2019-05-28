#pragma once

//#include "CRect.h"


enum TYPE_BLOCK_DIRECT
{
	TYPE_BLOCK_DIRECT_LEFT = 0,
	TYPE_BLOCK_DIRECT_TOP,
	TYPE_BLOCK_DIRECT_RIGHT,
	TYPE_BLOCK_DIRECT_BOTTOM,

	MAX_TYPE_BLOCK_DIRECT
};

//--------------------------------------
//
//--------------------------------------
class CDirectRect;
class CCollisionRect : public CRect
{
	public:
		typedef std::list< CDirectRect* >	TypeRectList;
		typedef TypeRectList::iterator		TypeRectListIter;

	public:
		BOOL		Create( const CRect& InRect );

		//CRect		GetRect( TYPE_BLOCK_DIRECT InDirect );

		int			GetIsCheckCollision( const CRect& InRect );

		void		Draw();

	public:
		CCollisionRect( const CRect& InRect );
		virtual ~CCollisionRect();
	public:
		static bool	Compare( const CDirectRect* InRect1st, const CDirectRect* InRect2nd );

	private:
		TypeRectList	m_RectList;

};

//--------------------------------------
//
//--------------------------------------
class CDirectRect : public CRect
{
	public:
		TYPE_BLOCK_DIRECT	GetDirect();
		void				SetDirect( TYPE_BLOCK_DIRECT InDirect );

	public:
		
		CDirectRect( const CRect& InRect, TYPE_BLOCK_DIRECT InDirect );
		virtual ~CDirectRect();
	public:
		TYPE_BLOCK_DIRECT	m_Direct;

};
