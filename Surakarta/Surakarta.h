/*!@file
*******************************************************************************
�ļ���: Sura.h
����:   1.����������Ŀ�����õ�������ϵͳͷ�ļ���ϵͳ��,
        2.������Ŀ�����õ��������Զ���ĺ�
        3.���ȫ�ֱ���������
����:   
��ע:
******************************************************************************/

#pragma once
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




#define WM_EATEN WM_USER+1 // ��������������Ϣ
///////////////////////////////////////////////////////////////////////////
// �궨��
#define MAX_LOADSTRING  50   // �������ַ�������󳤶�
#define DELAY_TIME      2 // ��ʱ����λ��΢�룩��������ʾ����Ч��
#define SEARCHING_SEND_BUF_TIME 50
#define PI              3.141592653 // Բ����
#define MAX_BACKWARD_NUM 5
#define MAX_FORWARD_NUM MAX_BACKWARD_NUM
#define DES_PORT 5000
#define SER_PORT 5000
#define MAX_BUFFER_LENGTH 4 // ���͵���Ϣ����ֽ���
// �õ��ĳ��ȵ�λ
#define CLIENT_WIDTH    600 // �ͻ������
#define CLIENT_HEIGHT   600 // �ͻ����߶�
#define PIECE_WIDTH  40  // ���ӿ��
#define PIECE_HEIGHT 40  // ���Ӹ߶�
#define PATH_WND_WIDTH   20  // ������λ�õı�ǵĿ��
#define PATH_WND_HEIGHT  20  // ������λ�õı�ǵĸ߶�
#define FLAG_WND_WIDTH  PATH_WND_WIDTH  // ��ʶ��ѡ�����ӵı�ǵĿ��
#define FLAG_WND_HEIGHT PATH_WND_HEIGHT // ��ʶ��ѡ�����ӵı�ǵĸ߶�
#define FLAG_WND_X  10 // ( CHESSMAN_WIDTH  - HIGHLIGHTWND_WIDTH  ) / 2
#define FLAG_WND_Y  10 // ( CHESSMAN_HEIGHT - HIGHLIGHTWND_HEIGHT ) / 2
#define LOG_WND_HEIGHT 60

// �������ӷ���,��λ����ʾ
#define UP_TO_FLY    0
#define RIGHT_TO_FLY 1
#define DOWN_TO_FLY  2
#define LEFT_TO_FLY  3

#define UP        4
#define UPRIGHT   5
#define RIGHT     6
#define DOWNRIGHT 7
#define DOWN      8
#define DOWNLEFT  9
#define LEFT      10
#define UPLEFT    11
// ���ж�Ӧ�����·�����,��λ����ʾ��2��ͷ���ȶ�Ӧ�����200
#define IDM_UP_TO_FLY    200
#define IDM_RIGHT_TO_FLY 201
#define IDM_DOWN_TO_FLY  202
#define IDM_LEFT_TO_FLY  203

#define IDM_UP	         204
#define IDM_UPRIGHT      205
#define IDM_RIGHT        206
#define IDM_DOWNRIGHT    207
#define IDM_DOWN         208
#define IDM_DOWNLEFT     209
#define IDM_LEFT         210
#define IDM_UPLEFT       211
////////////////////////////////////////////////////////////
// ����������ӣ���λ����ʾ,��λ1,�ȶ�Ӧ�����±��100(��������ȫ��999��ʾ)
#define IDM_MACHINE 999

#define IDM_MAN0  100
#define IDM_MAN1  101
#define IDM_MAN2  102
#define IDM_MAN3  103
#define IDM_MAN4  104
#define IDM_MAN5  105
#define IDM_MAN6  106
#define IDM_MAN7  107
#define IDM_MAN8  108
#define IDM_MAN9  109
#define IDM_MAN10 110
#define IDM_MAN11 111

#define NULL_PIECE    0
#define MAN_PIECE     1
#define MACHINE_PIECE 21

#define INNER_ORBIT 1
#define OUTER_ORBIT 2
#define DOUBLE_ORBIT 3
#define NULL_ORBIT   0

#define POSITION  char
#define DIRECTION char
// ���ڴ洢һ��·�������о����ĵ�
typedef vector<POSITION> PATH;


/////////////////////////////////////////////////////////////////
// ȫ�ֱ�������
extern HINSTANCE g_hInst;         // ʵ�����
extern HWND      g_hBoardWnd;     // �����ڣ�����
//extern HDC       g_memPieceWndDC; // ���Ӵ����ڴ�Device Context
extern HWND      g_hLogWnd;       // ��־����


// ���ڼ�¼�����±�,��������Ӧ�����ӷ���
typedef struct node
{
    int nIdx; // ���ӵ��±�
    DIRECTION direct; // ����Ӧ�����ӷ���
}Node;
typedef map<int,Node> MAP_VALUE_PATH;
extern MAP_VALUE_PATH g_mapValueAndPath; // һ��Ȩֵ�Ͷ�Ӧ����·���ı�


typedef struct
{
    int nKind,         // �ڵ�ǰλ�õ���������
        nPrevPieceIdx, // ֮ǰ�ڵ�ǰλ�õ����ӵ��±�
        nCurPieceIdx;  // �����ڵ�ǰλ�õ����ӵ��±�
    char chOrbit;
}ChessBoard;
extern ChessBoard g_board[]; // ����

class CPath;
extern CPath g_path;

class CMacPiece;
extern CMacPiece g_mac[];

class CManPiece;
extern CManPiece g_man[];