/*!@file
*******************************************************************************
文件名: CChessPiece.h
功能:   完成对棋子类CChessPiece的的声明, 包括机器棋子类和人棋子类
作者:   姚玉亮
备注:
******************************************************************************/

#pragma once

#include "surakarta.h"
#include "CPath.h"

/*!@class
*******************************************************************************
类名: CChessPiece
作者: 姚玉亮
描述: 封装了一颗棋子所必须的基本属性和方法
******************************************************************************/
class CChessPiece
{
protected:
    const int   m_cnIdx;
    POSITION    m_pos;       // 存放棋子的位置
    HWND        m_hPieceWnd; // 存放棋子所对应的窗口的句柄
public:
    static bool s_bIsMoving;
public:
    CChessPiece(const int cnIdx);
    ~CChessPiece();
    HWND GetPieceWnd(); // m_hPieceWnd
    virtual void New (const POSITION cpos) = 0; // 在pos处新建一枚棋子
    virtual void Move(const DIRECTION cdirect) = 0; // 棋子移动
    // 在ShowMoving()中被调用，用于销毁被吃棋子
    void BeEaten();
};

/*!@class
*******************************************************************************
类名: CMacPiece
作者: 姚玉亮
描述: 继承至CChesePiece,封装了机器棋子所必须的基本属性和方法
******************************************************************************/
class CMacPiece: public CChessPiece
{
private:
    CMachinePath m_path; // 棋子所有方向的走子路径
public:
    CMacPiece(const int cnIdx);
    void New (const POSITION cpos); // 在位置cpos处新建一颗棋子
    void Move(const DIRECTION cdirect); // 棋子根据代入的方向移动
    void Move(PATH path); // 棋子根据所代入的路径移动
    void Fly (); // 搜索可飞行的路径

};

/*!@class
*******************************************************************************
类名: CManPiece
作者: 姚玉亮
描述: 继承至CChessPiece,封装了人的棋子所必须的基本属性和方法
******************************************************************************/
class CManPiece: public CChessPiece
{
private:
    static HWND s_hFlagWnd;
public:
    static int s_nCurBeSelectedPieceIndex;

public:
    CManPiece(const int cnIdx);
    void New (const POSITION cpos);  // 在位置cpos处新建一颗棋子
    void Move(const DIRECTION cdirect); // 棋子根据代入的方向移动
    void BeSelected(); // 用户点击选中当前棋子时调用，用于标记出当前棋子所有的走子路径
};