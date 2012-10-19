/*!@file
*******************************************************************************
文件名: CPath.h
功能:   
作者:   姚玉亮
备注:
******************************************************************************/

#pragma once

#include "surakarta.h"

/************* 第1部分(共3部分)：声明机器路径类****************/
class CMachinePath
{
protected:
    PATH m_vectPath[12]; // 4个飞行方向的路径，加8个行走方向
    PATH m_vectFinalPath; // 存放4个路径中价值最大的一条
public:
    void Push(const DIRECTION cdirect, const POSITION cpos);
    void SetFinalPath(const DIRECTION cdirect, POSITION &m_pos);
    void SetFinalPath(PATH path, POSITION &m_pos);
    
    /*!@function
    ***************************************************************************
    功能: 返回最终的走子路径
    函数名: GetFinalPath
    参数: [out] path 路径的引用
    返回值: void
    **************************************************************************/
    void GetFinalPath(PATH &path);

    /*!@function
    *******************************************************************************
    功能: 返回指定方向上的路径
    函数名: GetPath
    参数: [in] cdirect 指定所要获取的路径的方向
          [out] path 指定方向上的路径的引用
    返回值: void
    ******************************************************************************/
    void GetPath(const DIRECTION cdirect, PATH &path);
    bool Condition(POSITION &pos, const DIRECTION &cdirect);
    virtual void FlagEatablePath(const POSITION cposLast,const DIRECTION cdirectOriginal);
    void FlyEngine(const POSITION cposOriginal, POSITION posCur,
	               const DIRECTION cdirectOriginal, DIRECTION directCur, bool bAlreadyFly
                  );
	void Fly(const POSITION cpos);
};
/********************第2部分(共3部分)：声明人路径类部分**************/
class CPath: public CMachinePath
{
private:
    HWND m_hPathWnd[12]; // 标记可起子
public:
    void Walk(const POSITION cPos);
	void FlagEatablePath(const POSITION cposLast, const DIRECTION cnOriginalDirect);
    void DestroyPathWnd();
};