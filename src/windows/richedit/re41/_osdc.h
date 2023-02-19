/*	@doc INTERNAL
 *
 *	@module _OSDC.H  Off Screen DC class |
 *	
 *	Define class for off screen DC
 *
 *	Original Author: <nl>
 *		Ricksa
 *
 *	History: <nl>
 *		1/11/96	ricksa	Created
 *
 *	Copyright (c) 1996-1998, Microsoft Corporation. All rights reserved.
 */
#ifndef __OSDC_H__
#define __OSDC_H__

/*
 *	COffscreenDC
 *	
 * 	@class	The COffscreenDC is a helper that creates, fills and destroys
 *			an off screen DC and its bitmaps.
 *
 */
class COffscreenDC
{
//@access Public Methods
public:
									//@cmember Constructor - create null object.
				COffscreenDC()		{_hdc = NULL;}

									//@cmember Destructor - clean up allocated 
									// resources if any.
				~COffscreenDC()		{FreeData();}
	
									//@cmember Initialize data based on input DC
	HDC			Init(HDC hdc, LONG xWidth, LONG yHeight, COLORREF crBackground);

									//@cmember Get DC for offscreen rendering
	HDC			GetDC()		{return _hdc;}
	void		GetDimensions(LONG *pdxp, LONG *pdyp);//@cmember the dimensions of the bitmap
									//@cmember Fill bitmap associated with off
									// screen rendering with background color.
	void		FillBitmap(LONG dxp, LONG dyp);

									//@cmember Render off screen bitmap to hdc
	void		RenderBitMap(HDC hdc, LONG xLeft, LONG yTop, LONG xWidth, LONG yHeight);

									//@cmember Get a copy of a bitmap from hdc
	BOOL		Get(HDC hdc, LONG xLeft, LONG yTop, LONG xWidth, LONG yHeight);

									//@cmember Reallocate bitmap for render
	BOOL		Realloc(LONG xWidth, LONG yHeight);

									//@cmember Select in a palette
	void		SelectPalette(HPALETTE hpa);

//@access Private Methods
private:
									//@cmember Free all data associated with object
	void		FreeData();

//@access Private Data 
private:
	HDC			_hdc;		//@cmember HDC for off screen DC
	HBITMAP		_hbmpOld;	//@cmember bitmap when DC created
	HBITMAP		_hbmp;		//@cmember compatible bitmap for render
	HPALETTE	_hpalOld;	//@cmember palette used by DC
};

/*
 *	COffscreenDC::FillBitmap (xWidth, yHeight)
 *
 *	@mfunc	
 *		Fill bitmap
 */
inline void COffscreenDC::FillBitmap(
	LONG dxp,		//@parm Width to fill with background color
	LONG dyp)		//@parm height to fill with background color
{
	// Erase background
	RECT rcClient;
	rcClient.top = rcClient.left = 0;
	rcClient.right = dxp;
	rcClient.bottom = dyp;
	ExtTextOut(_hdc, 0, 0, ETO_OPAQUE, &rcClient, NULL, 0, NULL);
}

/*
 *	COffscreenDC::RenderBitMap(hdc, xLeft, yTop, xWidth, yHeight)
 *
 *	@mfunc	
 *		Render bitmap to input DC
 */
inline void COffscreenDC::RenderBitMap(
	HDC  hdc,			//@parm HDC to render to
	LONG xLeft,			//@parm left position to start render
	LONG yTop,			//@parm top top position to start render
	LONG xWidth,		//@parm width to render
	LONG yHeight)		//@parm height to render
{
	BitBlt(hdc, xLeft, yTop, xWidth, yHeight, _hdc, 0, 0, SRCCOPY);
}
	
/*
 *	COffscreenDC::Get(hdc, xLeft, yTop, xWidth, yHeight)
 *
 *	@mfunc	
 *		Get a copy of a bitmap from another DC
 *
 *	@rdesc
 *		TRUE - succeeded
 *		FALSE - Failed
 */
inline BOOL COffscreenDC::Get(
	HDC  hdc,			//@parm HDC to copy from
	LONG xLeft,			//@parm left position of source bitmap
	LONG yTop,			//@parm top top position of source bitmap
	LONG xWidth,		//@parm width of bitmap
	LONG yHeight)		//@parm height to bitmap
{
	return BitBlt(_hdc, 0, 0, xWidth, yHeight, hdc, xLeft, yTop, SRCCOPY);
}

#endif __OSDC_H__