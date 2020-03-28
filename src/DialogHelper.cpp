#include "StdAfx.h"
#include "DialogHelper.h"

void EnableDlgItem(HWND hWnd, int nItem, BOOL enable)
{
	HWND hItem;
	hItem = GetDlgItem(hWnd,nItem);
	EnableWindow(hItem, enable);
}

void ShowDlgItem(HWND hWnd, int nItem, BOOL show)
{
	HWND hItem;
	hItem = GetDlgItem(hWnd,nItem);
	ShowWindow(hItem, show ? SW_SHOW : SW_HIDE);
}

void MoveDlgItem(HWND hWnd,int nItem, int dx, int dy, UINT nFlag)
{
	RECT rc;
	HWND hItem;
	POINT pt;

	hItem = GetDlgItem(hWnd,nItem);
	GetWindowRect(hItem,&rc);
	if(nFlag & MDIF_OFFSET_X)
	{
		rc.left  += dx;
		rc.right += dx;
	}
	if(nFlag & MDIF_OFFSET_Y)
	{
		rc.top    += dy;
		rc.bottom += dy;
	}
	if(nFlag & MDIF_RESIZE_W)
	{
		rc.right += dx;
	}
	if(nFlag & MDIF_RESIZE_H)
	{
		rc.bottom += dy;
	}
	pt.x = rc.left;
	pt.y = rc.top;
	ScreenToClient(hWnd,&pt);
	MoveWindow(hItem, pt.x, pt.y, rc.right - rc.left, rc.bottom - rc.top, TRUE);
}
