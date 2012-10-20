/*!@file
*******************************************************************************
�ļ���: CChessPiece.h
����:   ��ɶ�������CChessPiece�ĵ�����, �����������������������
����:   Ҧ����
��ע:
******************************************************************************/

#pragma once

#include "surakarta.h"
#include "CPath.h"

/*!@class
*******************************************************************************
����: CChessPiece
����: Ҧ����
����: ��װ��һ������������Ļ������Ժͷ���
******************************************************************************/
class CChessPiece
{
protected:
    const int   m_cnIdx;
    POSITION    m_pos;       // ������ӵ�λ��
    HWND        m_hPieceWnd; // �����������Ӧ�Ĵ��ڵľ��
public:
    static bool s_bIsMoving;
public:
    CChessPiece(const int cnIdx);
    ~CChessPiece();
    HWND GetPieceWnd(); // m_hPieceWnd
    virtual void New (const POSITION cpos) = 0; // ��pos���½�һö����
    virtual void Move(const DIRECTION cdirect) = 0; // �����ƶ�
    // ��ShowMoving()�б����ã��������ٱ�������
    void BeEaten();
};

/*!@class
*******************************************************************************
����: CMacPiece
����: Ҧ����
����: �̳���CChesePiece,��װ�˻�������������Ļ������Ժͷ���
******************************************************************************/
class CMacPiece: public CChessPiece
{
private:
    CMachinePath m_path; // �������з��������·��
public:
    CMacPiece(const int cnIdx);
    void New (const POSITION cpos); // ��λ��cpos���½�һ������
    void Move(const DIRECTION cdirect); // ���Ӹ��ݴ���ķ����ƶ�
    void Move(PATH path); // ���Ӹ����������·���ƶ�
    void Fly (); // �����ɷ��е�·��

};

/*!@class
*******************************************************************************
����: CManPiece
����: Ҧ����
����: �̳���CChessPiece,��װ���˵�����������Ļ������Ժͷ���
******************************************************************************/
class CManPiece: public CChessPiece
{
private:
    static HWND s_hFlagWnd;
public:
    static int s_nCurBeSelectedPieceIndex;

public:
    CManPiece(const int cnIdx);
    void New (const POSITION cpos);  // ��λ��cpos���½�һ������
    void Move(const DIRECTION cdirect); // ���Ӹ��ݴ���ķ����ƶ�
    void BeSelected(); // �û����ѡ�е�ǰ����ʱ���ã����ڱ�ǳ���ǰ�������е�����·��
};