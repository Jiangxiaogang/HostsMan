#pragma once

#define MDIF_RESIZE_W	0x01	//改变宽度
#define MDIF_RESIZE_H	0x02	//改变高度
#define MDIF_OFFSET_X	0x04	//水平移动
#define MDIF_OFFSET_Y	0x08	//垂直移动

void MoveDlgItem(HWND hWnd,int nItem, int dx, int dy, UINT nFlag);
void ShowDlgItem(HWND hWnd, int nItem, BOOL show);
void EnableDlgItem(HWND hWnd, int nItem, BOOL enable);
