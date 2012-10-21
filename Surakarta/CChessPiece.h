/*!@file CChessPiece.h
*******************************************************************************
����:   �������������������, ������������������������Լ�������Ĺ���
����:   rookie2(rookie2@163.com)
��ע:
����޸�ʱ��: 2012��10��21��
�޸�����: ��ע�͵��޸�
******************************************************************************/

#pragma once

#include "surakarta.h"
#include "CPath.h"

/*!@class CChessPiece
*******************************************************************************
����: rookie2(rookie2@163.com)
����: ��װ��һ������������Ļ������Ժͷ���
******************************************************************************/
class CChessPiece
{
protected:
    const int   m_cnIdx;     // ����,���ڱ�ʶ�������Ķ���
    POSITION    m_pos;       // �洢����λ��
    HWND        m_hPieceWnd; // �洢���ӵĴ��ھ��
public:
    static bool s_bIsMoving; // ��¼��ǰ�Ƿ��и��������ռ��ShowMoving�߳�
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
����: rookie2(rookie2@163.com)
����: �̳���CChesePiece,��װ�˻�������������Ļ������Ժͷ���
******************************************************************************/
class CMacPiece: public CChessPiece
{
private:
    CMachinePath m_path; // �洢·��
public:
    CMacPiece(const int cnIdx);
    void New (const POSITION cpos);
    void Move(const DIRECTION cdirect);
    void Move(PATH path);
    void Fly ();
};

/*!@class CManPiece
*******************************************************************************
����: rookie2(rookie2@163.com)
����: �̳���CChessPiece,��װ���˵�����������Ļ������Ժͷ���
******************************************************************************/
class CManPiece: public CChessPiece
{
private:
    static HWND s_hFlagWnd; // ��ǵĴ��ھ��,���ڱ������Ŀ����ӱ�ѡ��
public:
    /* ��ҵ�ǰ��ѡ�����ӵ�����ֵ, �����Ӷ����ʼ��ʱ�ĳ�������ֵ */
    static int s_nCurBeSelectedPieceIndex;
public:
    CManPiece(const int cnIdx);
    void New (const POSITION cpos);
    void Move(const DIRECTION cdirect);
    void BeSelected();
};