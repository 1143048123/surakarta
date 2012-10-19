/*!@file
*******************************************************************************
文件名: Sura.h
功能:   1.加载整个项目所能用到的所有系统头文件和系统库,
        2.定义项目中所用到的所有自定义的宏
        3.完成全局变量的声明
作者:   
备注:
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




#define WM_EATEN WM_USER+1 // 处理销毁棋子消息
///////////////////////////////////////////////////////////////////////////
// 宏定义
#define MAX_LOADSTRING  50   // 所加载字符串的最大长度
#define DELAY_TIME      2 // 延时（单位：微秒）：用于显示动画效果
#define SEARCHING_SEND_BUF_TIME 50
#define PI              3.141592653 // 圆周率
#define MAX_BACKWARD_NUM 5
#define MAX_FORWARD_NUM MAX_BACKWARD_NUM
#define DES_PORT 5000
#define SER_PORT 5000
#define MAX_BUFFER_LENGTH 4 // 发送的信息最大字节数
// 用到的长度单位
#define CLIENT_WIDTH    600 // 客户区宽度
#define CLIENT_HEIGHT   600 // 客户区高度
#define PIECE_WIDTH  40  // 棋子宽度
#define PIECE_HEIGHT 40  // 棋子高度
#define PATH_WND_WIDTH   20  // 可走子位置的标记的宽度
#define PATH_WND_HEIGHT  20  // 可走子位置的标记的高度
#define FLAG_WND_WIDTH  PATH_WND_WIDTH  // 标识所选中棋子的标记的宽度
#define FLAG_WND_HEIGHT PATH_WND_HEIGHT // 标识所选中棋子的标记的高度
#define FLAG_WND_X  10 // ( CHESSMAN_WIDTH  - HIGHLIGHTWND_WIDTH  ) / 2
#define FLAG_WND_Y  10 // ( CHESSMAN_HEIGHT - HIGHLIGHTWND_HEIGHT ) / 2
#define LOG_WND_HEIGHT 60

// 所有走子方向,两位数表示
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
// 所有对应方向的路径标记,三位数表示，2开头，比对应方向大200
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
// 玩家所有棋子，三位数表示,首位1,比对应棋子下标大100(机器棋子全用999表示)
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
// 用于存储一条路径中所有经过的点
typedef vector<POSITION> PATH;


/////////////////////////////////////////////////////////////////
// 全局变量声明
extern HINSTANCE g_hInst;         // 实例句柄
extern HWND      g_hBoardWnd;     // 主窗口，棋盘
//extern HDC       g_memPieceWndDC; // 棋子窗口内存Device Context
extern HWND      g_hLogWnd;       // 日志窗口


// 用于记录棋子下标,和其所对应的走子方向
typedef struct node
{
    int nIdx; // 棋子的下标
    DIRECTION direct; // 所对应的走子方向
}Node;
typedef map<int,Node> MAP_VALUE_PATH;
extern MAP_VALUE_PATH g_mapValueAndPath; // 一组权值和对应棋子路径的表


typedef struct
{
    int nKind,         // 在当前位置的棋子类型
        nPrevPieceIdx, // 之前在当前位置的棋子的下标
        nCurPieceIdx;  // 现在在当前位置的棋子的下标
    char chOrbit;
}ChessBoard;
extern ChessBoard g_board[]; // 棋盘

class CPath;
extern CPath g_path;

class CMacPiece;
extern CMacPiece g_mac[];

class CManPiece;
extern CManPiece g_man[];