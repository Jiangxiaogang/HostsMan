#pragma once

#define MDIF_RESIZE_W	0x01	//�ı���
#define MDIF_RESIZE_H	0x02	//�ı�߶�
#define MDIF_OFFSET_X	0x04	//ˮƽ�ƶ�
#define MDIF_OFFSET_Y	0x08	//��ֱ�ƶ�

void MoveDlgItem(HWND hWnd,int nItem, int dx, int dy, UINT nFlag);
void ShowDlgItem(HWND hWnd, int nItem, BOOL show);
void EnableDlgItem(HWND hWnd, int nItem, BOOL enable);
