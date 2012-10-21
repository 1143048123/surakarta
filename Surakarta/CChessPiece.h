/*!@file CChessPiece.h
*******************************************************************************
功能:   所有棋子类的声明部分, 包括机器棋子类和人棋子类以及棋子类的共性
作者:   rookie2(rookie2@163.com)
备注:
最近修改时间: 2012年10月21日
修改内容: 对注释的修改
******************************************************************************/

#pragma once

#include "surakarta.h"
#include "CPath.h"

/*!@class CChessPiece
*******************************************************************************
作者: rookie2(rookie2@163.com)
描述: 封装了一颗棋子所必须的基本属性和方法
******************************************************************************/
class CChessPiece
{
protected:
    const int   m_cnIdx;     // 常量,用于标识所创建的对象
    POSITION    m_pos;       // 存储所在位置
    HWND        m_hPieceWnd; // 存储棋子的窗口句柄
public:
    static bool s_bIsMoving; // 记录当前是否有该类对象处于占用ShowMoving线程
public:
    CChessPiece(const int cnIdx);
    ~CChessPiece();
    HWND GetPieceWnd();
    virtual void New (const POSITION cpos) = 0;
    virtual void Move(const DIRECTION cdirect) = 0;
    void BeEaten();
};

/*!@class CMacPiece
*******************************************************************************
作者: rookie2(rookie2@163.com)
描述: 继承至CChesePiece,封装了机器棋子所必须的基本属性和方法
******************************************************************************/
class CMacPiece: public CChessPiece
{
private:
    CMachinePath m_path; // 存储路径
public:
    CMacPiece(const int cnIdx);
    void New (const POSITION cpos);
    void Move(const DIRECTION cdirect);
    void Move(PATH path);
    void Fly ();
};

/*!@class CManPiece
*******************************************************************************
作者: rookie2(rookie2@163.com)
描述: 继承至CChessPiece,封装了人的棋子所必须的基本属性和方法
******************************************************************************/
class CManPiece: public CChessPiece
{
private:
    static HWND s_hFlagWnd; // 标记的窗口句柄,用于标记玩家哪颗棋子被选中
public:
    /* 玩家当前被选中棋子的索引值, 即棋子对象初始化时的常量索引值 */
    static int s_nCurBeSelectedPieceIndex;
public:
    CManPiece(const int cnIdx);
    void New (const POSITION cpos);
    void Move(const DIRECTION cdirect);
    void BeSelected();
};