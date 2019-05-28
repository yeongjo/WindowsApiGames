#ifndef _C_RECT_H_
#define _C_RECT_H_

#include "PublicDef.h"
#include "CLogWindow.h"

	//------------------------------------------------------------------------------------------
	// class CSmartPtrLockSurface
	//------------------------------------------------------------------------------------------
	// 부모 클래스
	//		CSmartPtr< LPDIRECTDRAWSURFACE7 >
	// 설명 
	//		생성시 전달된 LPDIRECTDRAWSURFACE7 에 Lock 을 걸고
	//		해당 블럭을 빠져 나가 소멸될시 자동으로 Unlock 한다
	//------------------------------------------------------------------------------------------
	using namespace NS_Public;

	class URect
	{
		public:
			URect&	Move( const CPoint& InPos, TYPE_ALIGN_METHOD InAlignMethod = TYPE_ALIGN_METHOD_LEFT_TOP )
			{	
				return Move( InPos.X(), InPos.Y(), InAlignMethod );
			}

			URect&	Move( float InX, float InY, TYPE_ALIGN_METHOD InAlignMethod = TYPE_ALIGN_METHOD_LEFT_TOP )
			{
				//using namespace ;
				float TempWidth		= Width(); 
				float TempHeight	= Height();
				
				float TempHalfWidth		= ( TempWidth ) / 2;
				float TempHalfHeight	= ( TempHeight ) / 2;

				switch( InAlignMethod )
				{
					case TYPE_ALIGN_METHOD_LEFT_TOP:
					{
						SetLeft( InX );
						SetTop( InY );
						break;
					}
					case TYPE_ALIGN_METHOD_LEFT_CENTER:
					{
						SetLeft( InX );
						SetTop( InY - TempHalfWidth );
						break;
					}
					case TYPE_ALIGN_METHOD_LEFT_BOTTOM:
					{
						SetLeft( InX );
						SetTop( InY - TempHeight );
						break;
					}				
					case TYPE_ALIGN_METHOD_CENTER_TOP:
					{
						SetLeft( InX - TempHalfWidth );
						SetTop( InY );				
						break;
					}	
					case TYPE_ALIGN_METHOD_CENTER_CENTER:
					{
						SetLeft( InX - TempHalfWidth );
						SetTop( InY - TempHalfHeight );

						break;
					}
					case TYPE_ALIGN_METHOD_CENTER_BOTTOM:
					{
						SetLeft( InX - TempHalfWidth );
						SetTop( InY - TempHeight );
						break;
					}
					case TYPE_ALIGN_METHOD_RIGHT_TOP:
					{
						SetLeft( InX - TempWidth );
						SetTop( InY );
						break;
					}
					case TYPE_ALIGN_METHOD_RIGHT_CENTER:
					{
						SetLeft( InX - TempWidth );
						SetTop( InY - TempHalfHeight );

						break;
					}
					case TYPE_ALIGN_METHOD_RIGHT_BOTTOM:
					{
						SetLeft( InX - TempWidth );
						SetTop( InY - TempHeight );
						break;
					}
					default:
					{
						break;
					}
				}
				SetRight( Left() + TempWidth );
				SetBottom( Top() + TempHeight );
			
				
				return ( *this );
			}
			
			void	Print() const
			{	
				//CLOG().Log( "Left : %.1f, Top : %.1f Right : %.1f Bottom : %.1f", Left(), Top(), Right(), Bottom() );	
			}

		public:

			float	Left() const				{	return m_Left;			}
			float	Top() const					{	return m_Top;			}
			float	Right() const				{	return m_Right;			}
			float	Bottom() const				{	return m_Bottom;		}
		
			float	Width() const				{	return ( Right() - Left() );	}
			float	Height() const				{	return ( Bottom() - Top() );	}

			void	SetLeft( float InLeft )		{	m_Left		= InLeft;	}
			void	SetTop(	float InTop )		{	m_Top		= InTop;	}
			void	SetRight( float InRight )	{	m_Right		= InRight;	}
			void	SetBottom( float InBottom )	{	m_Bottom	= InBottom;	}
		
			float	GetSize() const				{	return ( Width() * Height()	);	}

		public:
			URect()
			{
				m_Left		= 0;
				m_Top		= 0;
				m_Right		= 0;
				m_Bottom	= 0;
			}
			
			URect( float InLeft, float InTop, float InRight, float InBottom )
			{
				m_Left		= InLeft;
				m_Top		= InTop;
				m_Right		= InRight;
				m_Bottom	= InBottom;
			}
			
			URect( const URect& InRect )
			{
				m_Left = InRect.Left();
				m_Top = InRect.Top();
				m_Right = InRect.Right();
				m_Bottom = InRect.Bottom();
			}
			//--------------------------------------------
			//
			//--------------------------------------------
			URect& operator=( const URect& InRect )
			{
				m_Left = InRect.Left();
				m_Top = InRect.Top();
				m_Right = InRect.Right();
				m_Bottom = InRect.Bottom();
				
				return ( *this );
			}
			virtual ~URect()
			{
			}

	public:

			float	m_Left;
			float	m_Top;
			float	m_Right;
			float	m_Bottom;
	};
#endif //_C_RECT_H_