/*!@file
*******************************************************************************
�ļ���: CPath.h
����:   
����:   Ҧ����
��ע:
******************************************************************************/

#pragma once

#include "surakarta.h"

/************* ��1����(��3����)����������·����****************/
class CMachinePath
{
protected:
    PATH m_vectPath[12]; // 4�����з����·������8�����߷���
    PATH m_vectFinalPath; // ���4��·���м�ֵ����һ��
public:
    void Push(const DIRECTION cdirect, const POSITION cpos);
    void SetFinalPath(const DIRECTION cdirect, POSITION &m_pos);
    void SetFinalPath(PATH path, POSITION &m_pos);
    
    /*!@function
    ***************************************************************************
    ����: �������յ�����·��
    ������: GetFinalPath
    ����: [out] path ·��������
    ����ֵ: void
    **************************************************************************/
    void GetFinalPath(PATH &path);

    /*!@function
    *******************************************************************************
    ����: ����ָ�������ϵ�·��
    ������: GetPath
    ����: [in] cdirect ָ����Ҫ��ȡ��·���ķ���
          [out] path ָ�������ϵ�·��������
    ����ֵ: void
    ******************************************************************************/
    void GetPath(const DIRECTION cdirect, PATH &path);
    bool Condition(POSITION &pos, const DIRECTION &cdirect);
    virtual void FlagEatablePath(const POSITION cposLast,const DIRECTION cdirectOriginal);
    void FlyEngine(const POSITION cposOriginal, POSITION posCur,
	               const DIRECTION cdirectOriginal, DIRECTION directCur, bool bAlreadyFly
                  );
	void Fly(const POSITION cpos);
};
/********************��2����(��3����)��������·���ಿ��**************/
class CPath: public CMachinePath
{
private:
    HWND m_hPathWnd[12]; // ��ǿ�����
public:
    void Walk(const POSITION cPos);
	void FlagEatablePath(const POSITION cposLast, const DIRECTION cnOriginalDirect);
    void DestroyPathWnd();
};