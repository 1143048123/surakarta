/*!@file
*******************************************************************************
�ļ���: CChessPiece.cpp
����:   ��ɶ�������Ķ���,�����������������������
����:   Ҧ����
��ע:
******************************************************************************/

#include "stdafx.h"
#include "surakarta.h"
#include "CChessPiece.h"
////////////////////////////////////////////////////////////////////////////////////////
// ��������

void   AddLog(TCHAR *str);
DWORD  CALLBACK ShowMoving(LPVOID param); // ��ʾ�����ƶ����̵ĺ���������
VOID   Delay( ULONG );                    // ��ʱ����

// xֵ�仯��: x = 123 + j * 56;
static int x_table[] = { 123, 179, 235, 291, 347, 403,
                  123, 179, 235, 291, 347, 403,
                  123, 179, 235, 291, 347, 403,
                  123, 179, 235, 291, 347, 403,
                  123, 179, 235, 291, 347, 403,
                  123, 179, 235, 291, 347, 403 };
// yֵ�仯��: y = 120 + i * 56;
static int y_table[] = { 120, 120, 120, 120, 120, 120,
                  176, 176, 176, 176, 176, 176,
                  232, 232, 232, 232, 232, 232,
                  288, 288, 288, 288, 288, 288,
                  344, 344, 344, 344, 344, 344,
                  400, 400, 400, 400, 400, 400 };
///////////////////////////////////////////////////////////////////////////////
// CChessPiece������
bool CChessPiece::s_bIsMoving = false;
CChessPiece::CChessPiece(const int cnIdx) : m_cnIdx(cnIdx)
{}

CChessPiece::~CChessPiece()
{
    DestroyWindow(m_hPieceWnd);
    m_hPieceWnd = NULL;
}

HWND CChessPiece::GetPieceWnd()
{
    return m_hPieceWnd;
}
/*!@function
*******************************************************************************
����: ���ӱ���ʱ����,�����������Ӵ���
������: BeEaten
����: ��
����ֵ: void
******************************************************************************/
void CChessPiece::BeEaten()
{
    SendMessage(g_hBoardWnd, WM_EATEN, NULL, (LPARAM)m_hPieceWnd);
    m_hPieceWnd = NULL;
}

CMacPiece::CMacPiece(const int cnIdx) : CChessPiece(cnIdx)
{}
void CMacPiece::New(const POSITION cpos)
{
    m_pos = cpos;  // ��¼��ǰ����λ��

    m_hPieceWnd = CreateWindow( TEXT("BUTTON"), NULL,
        WS_CHILD | WS_VISIBLE | WS_DISABLED | BS_OWNERDRAW,
        x_table[cpos], y_table[cpos],
        PIECE_WIDTH, PIECE_HEIGHT,
        g_hBoardWnd,
        (HMENU)IDM_MACHINE,
        g_hInst, NULL
        );

    // ��ȡ���޸İ�ť�����Ա�����Բ�ΰ�ť
    DWORD style = GetClassLong( m_hPieceWnd, GCL_STYLE );
    SetClassLong( m_hPieceWnd, GCL_STYLE, style & ~CS_PARENTDC );
    HRGN hRgn = CreateEllipticRgn(0, 0, PIECE_WIDTH, PIECE_HEIGHT);
    SetWindowRgn( m_hPieceWnd, hRgn, TRUE);

    HCURSOR hCur = LoadCursor(NULL, IDC_HAND);
    SetClassLong( m_hPieceWnd, GCL_HCURSOR, (LONG)hCur); // �������ڰ�ť�ϵ���״Ϊ����
    /*
    HDC hdc = GetDC(m_hPieceWnd);
    g_memPieceWndDC = CreateCompatibleDC(hdc); // Ϊ��ťDC������Ӧ��С���ڴ�DC
    ReleaseDC(m_hPieceWnd, hdc);
    */
    
}
void CMacPiece::Move(const DIRECTION cdirect)
{
    PATH path;
    m_path.GetPath(cdirect, path);

    while(CChessPiece::s_bIsMoving == true)
    {
        Sleep(2);
    }
    g_path.SetFinalPath(path, m_pos);
    CreateThread(NULL, 0, ShowMoving, m_hPieceWnd, NULL, NULL);
}
void CMacPiece::Move(PATH path)
{

    while(CChessPiece::s_bIsMoving == true)
    {
        Sleep(2);
    }
    g_path.SetFinalPath(path, m_pos);
    CreateThread(NULL, 0, ShowMoving, m_hPieceWnd, NULL, NULL);
}
void CMacPiece::Fly()
{
    m_path.Fly(m_pos);
}
///////////////////////////////////////////////////////////////////////
HWND CManPiece::s_hFlagWnd = NULL;
int CManPiece::s_nCurBeSelectedPieceIndex = 0;

CManPiece::CManPiece(const int cnIdx):CChessPiece(cnIdx)
{}

void CManPiece::New(POSITION pos)
{
    m_pos = pos; // ��¼��ǰ����λ��
    g_board[pos].nCurPieceIdx = this->m_cnIdx;

    m_hPieceWnd = CreateWindowEx(NULL, TEXT("BUTTON"), NULL,
        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
        x_table[pos], y_table[pos],
        PIECE_WIDTH, PIECE_HEIGHT,
        g_hBoardWnd,
        (HMENU)((int)(m_cnIdx + 100)),
        g_hInst, NULL
        );

        // ��ȡ���޸İ�ť�����Ա�����Բ�ΰ�ť
        DWORD style = GetClassLong( m_hPieceWnd, GCL_STYLE );
        SetClassLong( m_hPieceWnd, GCL_STYLE, style & ~CS_PARENTDC );
        HRGN hRgn = CreateEllipticRgn(0, 0, PIECE_WIDTH, PIECE_HEIGHT);
        SetWindowRgn( m_hPieceWnd, hRgn, TRUE);

        HCURSOR hCur = LoadCursor(NULL, IDC_HAND);
        SetClassLong( m_hPieceWnd, GCL_HCURSOR, (LONG)hCur); // �������ڰ�ť�ϵ���״Ϊ����
}
void CManPiece::Move(const DIRECTION  cdirect)
{
    CChessPiece::s_bIsMoving = true;
    g_path.SetFinalPath(cdirect, m_pos);

    DestroyWindow(s_hFlagWnd);
    s_hFlagWnd=NULL;
    g_path.DestroyPathWnd();

    CreateThread(NULL, 0, ShowMoving, m_hPieceWnd, NULL, NULL);
}
void CManPiece::BeSelected()
{
    s_nCurBeSelectedPieceIndex = m_cnIdx;

    if (s_hFlagWnd != NULL)
    {
        DestroyWindow(s_hFlagWnd);
        s_hFlagWnd = NULL;
        g_path.DestroyPathWnd();
    }
    s_hFlagWnd=CreateWindowEx(NULL, TEXT("BUTTON"), NULL, WS_CHILD | WS_VISIBLE,
        FLAG_WND_X, FLAG_WND_Y, FLAG_WND_WIDTH, FLAG_WND_HEIGHT,
        m_hPieceWnd, NULL, g_hInst, NULL);

    g_path.Walk(m_pos);
    if ( m_pos!=0 && m_pos!=5 && m_pos!=30 && m_pos!=35 )
    {
        g_path.Fly(m_pos);
    }
  
}

/*!@function
*******************************************************************************
����: ��ʱ
������: Delay
����: ulMicroSeconds [in] ��ʱ�ȴ�ʱ��,��λ΢��
******************************************************************************/
/*
VOID Delay(ULONG ulMicroSeconds)
{
     LARGE_INTEGER  timeStop;
     LARGE_INTEGER  timeStart;
     LARGE_INTEGER  Freq;
     ULONG  ulTimeToWait;
     if ( !QueryPerformanceFrequency(&Freq) )
     {
         return;
     }
     ulTimeToWait = (ULONG)(Freq.QuadPart * ulMicroSeconds / 1000 / 1000);
     QueryPerformanceCounter ( &timeStart );
     timeStop = timeStart;
     while( timeStop.QuadPart - timeStart.QuadPart  < ulTimeToWait )
     {
         QueryPerformanceCounter( &timeStop );
     }
}
*/

/*!@function
*******************************************************************************
����: ��ʱ
������: Delay
����: ulMicroSeconds [in] ��ʱ�ȴ�ʱ��,��λ΢��
******************************************************************************/
void Delay(int n)
{
    Sleep(n);
}

/*!@function
*******************************************************************************
����: ���߳�������·��ȷ�Ϻ����,������ʾ��Ӧ���ӵ��ƶ����̶���
������: ShowMoving
����: param [in] ָ��Ҫ���ƶ������ӵĴ��ھ��
******************************************************************************/
DWORD WINAPI ShowMoving(LPVOID param)
{
    HWND hWnd = (HWND)param;

    for(int i=0; i<12; i++)
        EnableWindow(g_man[i].GetPieceWnd(), FALSE);
    // �ƶ���ʽ������������ֵ���壺
    //     1��2��3��4�ֱ��ʾ�ĸ����ڹ죬��ֵΪ��ʱ��
    //     100��200��300��400�ֱ��ʾ�ĸ�����죬��ֵΪ��ʱ��
    //     ����ֵ��Ϊ��ֱ��
    const int move_table[] = {10000,     0,   100,   900,     8, 10000,
                                  1, 10000, 10000, 10000, 10000,     6,
                                200, 10000, 10000, 10000, 10000,   700,
                               1400, 10000, 10000, 10000, 10000,  2600,
                                 13, 10000, 10000, 10000, 10000,    25,
                              10000,    16,  1700,  2200,    21, 10000
                             };
    int nDiffX, nDiffY;
    int x0, y0; // ����Բ������
    int x, y;   // ����ʵʱ�ƶ�����
    int nRadius; // ����Բ�İ뾶��С
    double lfRadian = 0; // ������С:���ȵ�λ
    double lfRadian2Degree = PI / 180.0; // ����ת�Ƕȵ�ת����λ
    int nVPixel, nHPixel;
    PATH path;
    g_path.GetFinalPath(path);
    POSITION posStart = path.at(0);
    POSITION posEnd   = path.at(1);
    ///////////////////////////////////////////////////////////////////////////////////////////
    TCHAR szText[256] = {0};
    wsprintf(szText, TEXT("%S%-2d-->%-2d\t����[%S]"),
             g_board[path.back()].nKind == MAN_PIECE ? "��--->":"��--->", path.at(1), path.back(),
             path.at(0)<=3 ? "��" : "��"
            );
    AddLog(szText);
    ///////////////////////////////////////////////////////////////////////////////////////////
    for(int i=2; ; i++)
    {
        posStart = posEnd;
        try
        {
            posEnd = path.at(i);
        }
        catch (exception e)
        {
            break;
        }

        int nMoveStyle = move_table[posStart] - move_table[posEnd];
        if ( path.front() > 3 )
            nMoveStyle = 0;
        
        if (nMoveStyle==1 || nMoveStyle==2 || nMoveStyle==3 || nMoveStyle==4 ||
            nMoveStyle==100 || nMoveStyle==200 || nMoveStyle==300 || nMoveStyle==400)
        {
            switch( nMoveStyle )
            {
                case 1: // ���Ͻ��ڹ�����
                    lfRadian = 0;
                    nRadius = 56;
                    x0 = 123;
                    y0 = 120;
                    break;
                case 2: // ���Ͻ��ڹ�����
                    lfRadian = -PI*1.5;
                    nRadius = 56;
                    x0 = 403;
                    y0 = 120;
                    break;
                case 3: // ���½��ڹ�����
                    lfRadian = -PI/2.0;
                    nRadius = 56;
                    x0 = 123;
                    y0 = 400;
                    break;
                case 4: // ���½��ڹ�����
                    lfRadian = -PI*1.0;
                    nRadius = 56;
                    x0 = 403;
                    y0 = 400;
                    break;
                case 100: // ���Ͻ��������
                    lfRadian = 0;
                    nRadius = 112;
                    x0 = 123;
                    y0 = 120;
                    break;
                case 200: // ���Ͻ��������
                    lfRadian = -PI*1.5;
                    nRadius = 112;
                    x0 = 403;
                    y0 = 120;
                    break;
                case 300: // ���½��������
                    lfRadian = -PI/2.0;
                    nRadius = 112;
                    x0 = 123;
                    y0 = 400;
                    break;
                case 400: // ���½��������
                    lfRadian = -PI*1.0;
                    nRadius = 112;
                    x0 = 403;
                    y0 = 400;
                    break;
            }
            for(int i=1; i<=270; i++)
            {
                Delay(DELAY_TIME); // ��ʱ�Դﵽ����Ч��
                x = (int)( x0 - nRadius*sin( lfRadian + lfRadian2Degree * i ) ); // (PI/180.0)��һ����λ����
                y = (int)( y0 + nRadius*cos( lfRadian + lfRadian2Degree * i ) );
                MoveWindow(hWnd, x, y, PIECE_WIDTH, PIECE_HEIGHT, TRUE);
                InvalidateRect(GetParent(hWnd), NULL, TRUE); // �ػ�������
                UpdateWindow(GetParent(hWnd));
            }
            MoveWindow(hWnd, x_table[posEnd], y_table[posEnd], PIECE_WIDTH, PIECE_HEIGHT, TRUE);
        }
        else if (nMoveStyle==-1 || nMoveStyle==-2 || nMoveStyle==-3 || nMoveStyle==-4 ||
            nMoveStyle==-100 || nMoveStyle==-200 || nMoveStyle==-300 || nMoveStyle==-400)
        {
            switch(nMoveStyle)
            {
                case -1: // ���Ͻ��ڹ�����
                    lfRadian = 0.0;
                    nRadius = 56;
                    x0 = 123;
                    y0 = 120;
                    break;
                case -2: // ���Ͻ��ڹ�����
                    lfRadian = -PI/2.0;
                    nRadius = 56;
                    x0 = 403;
                    y0 = 120;
                    break;
                case -3: // ���½��ڹ�����
                    lfRadian = PI/2.0;
                    nRadius = 56;
                    x0 = 123;
                    y0 = 400;
                    break;
                case -4: // ���½��ڹ�����
                    lfRadian = -PI*1.0;
                    nRadius = 56;
                    x0 = 403;
                    y0 = 400;
                    break;
                case -100: // ���Ͻ��������
                    lfRadian = 0.0;
                    nRadius = 112;
                    x0 = 123;
                    y0 = 120;
                    break;
                case -200: // ���Ͻ��������
                    lfRadian = -PI/2.0;
                    nRadius = 112;
                    x0 = 403;
                    y0 = 120;
                    break;
                case -300: // ���½��������
                    lfRadian = PI/2.0;
                    nRadius = 112;
                    x0 = 123;
                    y0 = 400;
                    break;
                case -400: // ���½��������
                    lfRadian = -PI*1.0;
                    nRadius = 112;
                    x0 = 403;
                    y0 = 400;
                    break;
            }
            for(int i=0; i<270; i++) // ��ʾ�ƹ����Ӷ���
            {
                Delay(DELAY_TIME);
                x = (int)( x0 + nRadius*cos( lfRadian + lfRadian2Degree * i ) ); // (PI/180.0)��һ����λ����
                y = (int)( y0 - nRadius*sin( lfRadian + lfRadian2Degree * i ) );
                MoveWindow(hWnd, x, y, PIECE_WIDTH, PIECE_HEIGHT, TRUE);
                InvalidateRect(GetParent(hWnd), NULL, TRUE); // �ػ�������
                UpdateWindow(GetParent(hWnd));
            }
            // ���ȶ�������ƫ�����ƫ��λ��
            MoveWindow(hWnd, x_table[posEnd], y_table[posEnd], PIECE_WIDTH, PIECE_HEIGHT, TRUE);
        }
        else 
        {
            x0 = x_table[posStart];
            y0 = y_table[posStart];
            x = x_table[posEnd];
            y = y_table[posEnd];

            nDiffX = x0 - x;
            nDiffY = y0 - y;
                
            if (nDiffX == 0) // X�᷽������ƫ��
            {
                nHPixel = 0;
            } else if (nDiffX > 0) {
                nHPixel = -1;
            } else if (nDiffX < 0) {
                nHPixel = 1;
            }
                
            if (nDiffY == 0) // Y�᷽������ƫ��
            {
                nVPixel = 0;
            } else if (nDiffY > 0) {
                nVPixel = -1;
            } else if (nDiffY < 0) {
                nVPixel = 1;
            }

            while(x0 != x || y0 != y) // ��ʾֱ���ƶ�����
            {
                Delay(DELAY_TIME);
                x0 += nHPixel;
                y0 += nVPixel;
                MoveWindow(hWnd, x0, y0, PIECE_WIDTH, PIECE_HEIGHT, TRUE);
                InvalidateRect(GetParent(hWnd), NULL, TRUE);
                UpdateWindow(GetParent(hWnd));
            }
        }
    }
    // ���ٱ�������
    if (path.front() <=3)
    {
        if(g_board[path.back()].nKind == MAN_PIECE) // �˳Ի�����
            g_mac[ g_board[path.back()].nPrevPieceIdx ].BeEaten();
        else // ��������
            g_man[ g_board[path.back()].nPrevPieceIdx ].BeEaten();
    }
    // �������е�����ͳһ
    g_board[path.back()].nPrevPieceIdx = g_board[path.back()].nCurPieceIdx;
    // ���ų�������
    PlaySound( MAKEINTRESOURCE(IDR_MOVE), GetModuleHandle(NULL), SND_RESOURCE);// | SND_ASYNC );
    
    for(int i=0; i<12; i++)
        EnableWindow(g_man[i].GetPieceWnd(), TRUE);

    CChessPiece::s_bIsMoving = false;

    return 1;
}