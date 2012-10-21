/*!@file Sura.h
*******************************************************************************
功能:   1.加载整个项目所能用到的所有系统头文件和系统库,
        2.定义项目中所用到的所有自定义的宏
        3.完成全局变量的声明
作者: 姚玉亮(rookie2@163.com)
备注:
最近修改时间: 2012年10月21日
修改内容: 注释部分
******************************************************************************/

#pragma once

/**
 * 项目所需头文件,命名空间和库
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
 * 宏定义
 */
#define WM_EATEN          WM_USER+1   // 处理销毁棋子消息
#define MAX_LOADSTRING    50          // 所加载字符串的最大长度
#define DELAY_TIME        2           // 延时（单位：毫秒）用于显示动画效果
#define PI                3.141592653 // 圆周率
#define MAX_BUFFER_LENGTH 4   // 发送的信息最大字节数
#define CLIENT_WIDTH      600 // 客户区宽度
#define CLIENT_HEIGHT     600 // 客户区高度
#define PIECE_WIDTH       40  // 棋子宽度
#define PIECE_HEIGHT      40  // 棋子高度
#define PATH_WND_WIDTH    20  // 可走子位置的标记的宽度
#define PATH_WND_HEIGHT   20  // 可走子位置的标记的高度
#define FLAG_WND_WIDTH    PATH_WND_WIDTH  // 标识所选中棋子的标记的宽度
#define FLAG_WND_HEIGHT   PATH_WND_HEIGHT // 标识所选中棋子的标记的高度
#define FLAG_WND_X        10 // 标记窗口的左上角X值
#define FLAG_WND_Y        10 // 标记窗口的左上角Y值
#define LOG_WND_HEIGHT    60 // 日志窗口的高度
#define UP_TO_FLY         0  // 上飞行
#define RIGHT_TO_FLY      1  // 右飞行
#define DOWN_TO_FLY       2  // 下飞行
#define LEFT_TO_FLY       3  // 左飞行
#define UP                4  // 上走子
#define UPRIGHT           5  // 右上走子
#define RIGHT             6  // 右走子
#define DOWNRIGHT         7  // 右下走子
#define DOWN              8  // 下走子
#define DOWNLEFT          9  // 左下走子
#define LEFT              10 // 左走子
#define UPLEFT            11 // 左上走子
#define IDM_UP_TO_FLY    200 // 菜单标识,上飞行
#define IDM_RIGHT_TO_FLY 201 // 菜单标识,右飞行
#define IDM_DOWN_TO_FLY  202 // 菜单标识,下飞行
#define IDM_LEFT_TO_FLY  203 // 菜单标识,左飞行
#define IDM_UP	         204 // 菜单标识,上走子
#define IDM_UPRIGHT      205 // 菜单标识,右上走子
#define IDM_RIGHT        206 // 菜单标识,右走子
#define IDM_DOWNRIGHT    207 // 菜单标识,右下走子
#define IDM_DOWN         208 // 菜单标识,下走子
#define IDM_DOWNLEFT     209 // 菜单标识,左下走子
#define IDM_LEFT         210 // 菜单标识,左走子
#define IDM_UPLEFT       211 // 菜单标识,左上走子
#define IDM_MACHINE      999 // 菜单标识,机器棋子
#define IDM_MAN0         100 // 菜单标识,人棋子0号
#define IDM_MAN1         101 // 菜单标识,人棋子1号
#define IDM_MAN2         102 // 菜单标识,人棋子2号
#define IDM_MAN3         103 // 菜单标识,人棋子3号
#define IDM_MAN4         104 // 菜单标识,人棋子4号
#define IDM_MAN5         105 // 菜单标识,人棋子5号
#define IDM_MAN6         106 // 菜单标识,人棋子6号
#define IDM_MAN7         107 // 菜单标识,人棋子7号
#define IDM_MAN8         108 // 菜单标识,人棋子8号
#define IDM_MAN9         109 // 菜单标识,人棋子9号
#define IDM_MAN10        110 // 菜单标识,人棋子10号
#define IDM_MAN11        111 // 菜单标识,人棋子11号
#define NULL_PIECE       0   // 没有棋子
#define MAN_PIECE        1   // 人棋子
#define MACHINE_PIECE    21  // 机器棋子
#define INNER_ORBIT      1   // 内轨
#define OUTER_ORBIT      2   // 外轨
#define DOUBLE_ORBIT     3   // 双轨
#define NULL_ORBIT       0   // 不在轨道上
#define POSITION         char // 位置变量类型
#define DIRECTION        char // 方向变量类型

typedef vector<POSITION> PATH; // 类型定义

/**
 * 全局变量声明在此处添加
 */
extern HINSTANCE g_hInst;         // 实例句柄
extern HWND      g_hBoardWnd;     // 主窗口，棋盘
extern HWND      g_hLogWnd;       // 日志窗口

// 用于记录棋子下标,和其所对应的走子方向
typedef struct node
{
    int nIdx; // 棋子的下标
    DIRECTION direct; // 所对应的走子方向
}Node;
typedef map<int,Node> MAP_VALUE_PATH;
extern MAP_VALUE_PATH g_mapValueAndPath; // 一组权值和对应棋子路径的表

// 用于存储棋盘上每颗棋子所携带的数据
typedef struct
{
    int nKind,         // 在当前位置的棋子类型
        nPrevPieceIdx, // 之前在当前位置的棋子的下标
        nCurPieceIdx;  // 现在在当前位置的棋子的下标
    char chOrbit;
}ChessBoard;
extern ChessBoard g_board[]; // 棋盘

class CPath;
extern CPath g_path; // 路径类对象

class CMacPiece;
extern CMacPiece g_mac[]; // CMacPiece类对象数组

class CManPiece;
extern CManPiece g_man[]; // CManPiece类对象数组