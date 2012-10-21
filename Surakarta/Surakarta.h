/*!@file Sura.h
*******************************************************************************
����:   1.����������Ŀ�����õ�������ϵͳͷ�ļ���ϵͳ��,
        2.������Ŀ�����õ��������Զ���ĺ�
        3.���ȫ�ֱ���������
����: Ҧ����(rookie2@163.com)
��ע:
����޸�ʱ��: 2012��10��21��
�޸�����: ע�Ͳ���
******************************************************************************/

#pragma once

/**
 * ��Ŀ����ͷ�ļ�,�����ռ�Ϳ�
 */
#include "stdafx.h"
#include <math.h>
#include <stdio.h>
#include <Windows.h>
#include <MMSystem.h>
#include <cstring>
#include <vector>
#include <map>
#include "resource.h"
#pragma comment(lib,"WS2_32.lib")
#pragma comment(lib, "winmm.lib")
using namespace std;

/**
 * �궨��
 */
#define WM_EATEN          WM_USER+1   // ��������������Ϣ
#define MAX_LOADSTRING    50          // �������ַ�������󳤶�
#define DELAY_TIME        2           // ��ʱ����λ�����룩������ʾ����Ч��
#define PI                3.141592653 // Բ����
#define MAX_BUFFER_LENGTH 4   // ���͵���Ϣ����ֽ���
#define CLIENT_WIDTH      600 // �ͻ������
#define CLIENT_HEIGHT     600 // �ͻ����߶�
#define PIECE_WIDTH       40  // ���ӿ��
#define PIECE_HEIGHT      40  // ���Ӹ߶�
#define PATH_WND_WIDTH    20  // ������λ�õı�ǵĿ��
#define PATH_WND_HEIGHT   20  // ������λ�õı�ǵĸ߶�
#define FLAG_WND_WIDTH    PATH_WND_WIDTH  // ��ʶ��ѡ�����ӵı�ǵĿ��
#define FLAG_WND_HEIGHT   PATH_WND_HEIGHT // ��ʶ��ѡ�����ӵı�ǵĸ߶�
#define FLAG_WND_X        10 // ��Ǵ��ڵ����Ͻ�Xֵ
#define FLAG_WND_Y        10 // ��Ǵ��ڵ����Ͻ�Yֵ
#define LOG_WND_HEIGHT    60 // ��־���ڵĸ߶�
#define UP_TO_FLY         0  // �Ϸ���
#define RIGHT_TO_FLY      1  // �ҷ���
#define DOWN_TO_FLY       2  // �·���
#define LEFT_TO_FLY       3  // �����
#define UP                4  // ������
#define UPRIGHT           5  // ��������
#define RIGHT             6  // ������
#define DOWNRIGHT         7  // ��������
#define DOWN              8  // ������
#define DOWNLEFT          9  // ��������
#define LEFT              10 // ������
#define UPLEFT            11 // ��������
#define IDM_UP_TO_FLY    200 // �˵���ʶ,�Ϸ���
#define IDM_RIGHT_TO_FLY 201 // �˵���ʶ,�ҷ���
#define IDM_DOWN_TO_FLY  202 // �˵���ʶ,�·���
#define IDM_LEFT_TO_FLY  203 // �˵���ʶ,�����
#define IDM_UP	         204 // �˵���ʶ,������
#define IDM_UPRIGHT      205 // �˵���ʶ,��������
#define IDM_RIGHT        206 // �˵���ʶ,������
#define IDM_DOWNRIGHT    207 // �˵���ʶ,��������
#define IDM_DOWN         208 // �˵���ʶ,������
#define IDM_DOWNLEFT     209 // �˵���ʶ,��������
#define IDM_LEFT         210 // �˵���ʶ,������
#define IDM_UPLEFT       211 // �˵���ʶ,��������
#define IDM_MACHINE      999 // �˵���ʶ,��������
#define IDM_MAN0         100 // �˵���ʶ,������0��
#define IDM_MAN1         101 // �˵���ʶ,������1��
#define IDM_MAN2         102 // �˵���ʶ,������2��
#define IDM_MAN3         103 // �˵���ʶ,������3��
#define IDM_MAN4         104 // �˵���ʶ,������4��
#define IDM_MAN5         105 // �˵���ʶ,������5��
#define IDM_MAN6         106 // �˵���ʶ,������6��
#define IDM_MAN7         107 // �˵���ʶ,������7��
#define IDM_MAN8         108 // �˵���ʶ,������8��
#define IDM_MAN9         109 // �˵���ʶ,������9��
#define IDM_MAN10        110 // �˵���ʶ,������10��
#define IDM_MAN11        111 // �˵���ʶ,������11��
#define NULL_PIECE       0   // û������
#define MAN_PIECE        1   // ������
#define MACHINE_PIECE    21  // ��������
#define INNER_ORBIT      1   // �ڹ�
#define OUTER_ORBIT      2   // ���
#define DOUBLE_ORBIT     3   // ˫��
#define NULL_ORBIT       0   // ���ڹ����
#define POSITION         char // λ�ñ�������
#define DIRECTION        char // �����������

typedef vector<POSITION> PATH; // ���Ͷ���

/**
 * ȫ�ֱ��������ڴ˴����
 */
extern HINSTANCE g_hInst;         // ʵ�����
extern HWND      g_hBoardWnd;     // �����ڣ�����
extern HWND      g_hLogWnd;       // ��־����

// ���ڼ�¼�����±�,��������Ӧ�����ӷ���
typedef struct node
{
    int nIdx; // ���ӵ��±�
    DIRECTION direct; // ����Ӧ�����ӷ���
}Node;
typedef map<int,Node> MAP_VALUE_PATH;
extern MAP_VALUE_PATH g_mapValueAndPath; // һ��Ȩֵ�Ͷ�Ӧ����·���ı�

// ���ڴ洢������ÿ��������Я��������
typedef struct
{
    int nKind,         // �ڵ�ǰλ�õ���������
        nPrevPieceIdx, // ֮ǰ�ڵ�ǰλ�õ����ӵ��±�
        nCurPieceIdx;  // �����ڵ�ǰλ�õ����ӵ��±�
    char chOrbit;
}ChessBoard;
extern ChessBoard g_board[]; // ����

class CPath;
extern CPath g_path; // ·�������

class CMacPiece;
extern CMacPiece g_mac[]; // CMacPiece���������

class CManPiece;
extern CManPiece g_man[]; // CManPiece���������