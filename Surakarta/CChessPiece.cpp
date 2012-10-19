/*!@file
*******************************************************************************
文件名: CChessPiece.cpp
功能:   完成对棋子类的定义,包括机器棋子类和人棋子类
作者:   姚玉亮
备注:
******************************************************************************/

#include "stdafx.h"
#include "surakarta.h"
#include "CChessPiece.h"
////////////////////////////////////////////////////////////////////////////////////////
// 函数声明

void   AddLog(TCHAR *str);
DWORD  CALLBACK ShowMoving(LPVOID param); // 显示棋子移动过程的函数的声明
VOID   Delay( ULONG );                    // 延时函数

// x值变化表: x = 123 + j * 56;
static int x_table[] = { 123, 179, 235, 291, 347, 403,
                  123, 179, 235, 291, 347, 403,
                  123, 179, 235, 291, 347, 403,
                  123, 179, 235, 291, 347, 403,
                  123, 179, 235, 291, 347, 403,
                  123, 179, 235, 291, 347, 403 };
// y值变化表: y = 120 + i * 56;
static int y_table[] = { 120, 120, 120, 120, 120, 120,
                  176, 176, 176, 176, 176, 176,
                  232, 232, 232, 232, 232, 232,
                  288, 288, 288, 288, 288, 288,
                  344, 344, 344, 344, 344, 344,
                  400, 400, 400, 400, 400, 400 };
///////////////////////////////////////////////////////////////////////////////
// CChessPiece类声明
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
功能: 棋子被吃时调用,用于销毁棋子窗口
函数名: BeEaten
参数: 无
返回值: void
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
    m_pos = cpos;  // 记录当前棋子位置

    m_hPieceWnd = CreateWindow( TEXT("BUTTON"), NULL,
        WS_CHILD | WS_VISIBLE | WS_DISABLED | BS_OWNERDRAW,
        x_table[cpos], y_table[cpos],
        PIECE_WIDTH, PIECE_HEIGHT,
        g_hBoardWnd,
        (HMENU)IDM_MACHINE,
        g_hInst, NULL
        );

    // 获取并修改按钮类风格，以便做出圆形按钮
    DWORD style = GetClassLong( m_hPieceWnd, GCL_STYLE );
    SetClassLong( m_hPieceWnd, GCL_STYLE, style & ~CS_PARENTDC );
    HRGN hRgn = CreateEllipticRgn(0, 0, PIECE_WIDTH, PIECE_HEIGHT);
    SetWindowRgn( m_hPieceWnd, hRgn, TRUE);

    HCURSOR hCur = LoadCursor(NULL, IDC_HAND);
    SetClassLong( m_hPieceWnd, GCL_HCURSOR, (LONG)hCur); // 重设光标在按钮上的形状为手形
    /*
    HDC hdc = GetDC(m_hPieceWnd);
    g_memPieceWndDC = CreateCompatibleDC(hdc); // 为按钮DC创建相应大小的内存DC
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
    m_pos = pos; // 记录当前棋子位置
    g_board[pos].nCurPieceIdx = this->m_cnIdx;

    m_hPieceWnd = CreateWindowEx(NULL, TEXT("BUTTON"), NULL,
        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
        x_table[pos], y_table[pos],
        PIECE_WIDTH, PIECE_HEIGHT,
        g_hBoardWnd,
        (HMENU)((int)(m_cnIdx + 100)),
        g_hInst, NULL
        );

        // 获取并修改按钮类风格，以便做出圆形按钮
        DWORD style = GetClassLong( m_hPieceWnd, GCL_STYLE );
        SetClassLong( m_hPieceWnd, GCL_STYLE, style & ~CS_PARENTDC );
        HRGN hRgn = CreateEllipticRgn(0, 0, PIECE_WIDTH, PIECE_HEIGHT);
        SetWindowRgn( m_hPieceWnd, hRgn, TRUE);

        HCURSOR hCur = LoadCursor(NULL, IDC_HAND);
        SetClassLong( m_hPieceWnd, GCL_HCURSOR, (LONG)hCur); // 重设光标在按钮上的形状为手形
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
功能: 延时
函数名: Delay
参数: ulMicroSeconds [in] 延时等待时间,单位微秒
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
功能: 延时
函数名: Delay
参数: ulMicroSeconds [in] 延时等待时间,单位微秒
******************************************************************************/
void Delay(int n)
{
    Sleep(n);
}

/*!@function
*******************************************************************************
功能: 该线程在走子路径确认后调用,用于显示相应棋子的移动过程动画
函数名: ShowMoving
参数: param [in] 指向要被移动的棋子的窗口句柄
******************************************************************************/
DWORD WINAPI ShowMoving(LPVOID param)
{
    HWND hWnd = (HWND)param;

    for(int i=0; i<12; i++)
        EnableWindow(g_man[i].GetPieceWnd(), FALSE);
    // 移动方式表，两点间做差，差值意义：
    //     1，2，3，4分别表示四个角内轨，负值为逆时针
    //     100，200，300，400分别表示四个角外轨，负值为逆时针
    //     其它值视为走直线
    const int move_table[] = {10000,     0,   100,   900,     8, 10000,
                                  1, 10000, 10000, 10000, 10000,     6,
                                200, 10000, 10000, 10000, 10000,   700,
                               1400, 10000, 10000, 10000, 10000,  2600,
                                 13, 10000, 10000, 10000, 10000,    25,
                              10000,    16,  1700,  2200,    21, 10000
                             };
    int nDiffX, nDiffY;
    int x0, y0; // 所在圆心坐标
    int x, y;   // 棋子实时移动坐标
    int nRadius; // 所在圆的半径大小
    double lfRadian = 0; // 度数大小:弧度单位
    double lfRadian2Degree = PI / 180.0; // 弧度转角度的转换单位
    int nVPixel, nHPixel;
    PATH path;
    g_path.GetFinalPath(path);
    POSITION posStart = path.at(0);
    POSITION posEnd   = path.at(1);
    ///////////////////////////////////////////////////////////////////////////////////////////
    TCHAR szText[256] = {0};
    wsprintf(szText, TEXT("%S%-2d-->%-2d\t吃子[%S]"),
             g_board[path.back()].nKind == MAN_PIECE ? "人--->":"机--->", path.at(1), path.back(),
             path.at(0)<=3 ? "是" : "否"
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
                case 1: // 左上角内轨正向
                    lfRadian = 0;
                    nRadius = 56;
                    x0 = 123;
                    y0 = 120;
                    break;
                case 2: // 右上角内轨正向
                    lfRadian = -PI*1.5;
                    nRadius = 56;
                    x0 = 403;
                    y0 = 120;
                    break;
                case 3: // 左下角内轨正向
                    lfRadian = -PI/2.0;
                    nRadius = 56;
                    x0 = 123;
                    y0 = 400;
                    break;
                case 4: // 右下角内轨正向
                    lfRadian = -PI*1.0;
                    nRadius = 56;
                    x0 = 403;
                    y0 = 400;
                    break;
                case 100: // 左上角外轨正向
                    lfRadian = 0;
                    nRadius = 112;
                    x0 = 123;
                    y0 = 120;
                    break;
                case 200: // 右上角外轨正向
                    lfRadian = -PI*1.5;
                    nRadius = 112;
                    x0 = 403;
                    y0 = 120;
                    break;
                case 300: // 左下角外轨正向
                    lfRadian = -PI/2.0;
                    nRadius = 112;
                    x0 = 123;
                    y0 = 400;
                    break;
                case 400: // 右下角外轨正向
                    lfRadian = -PI*1.0;
                    nRadius = 112;
                    x0 = 403;
                    y0 = 400;
                    break;
            }
            for(int i=1; i<=270; i++)
            {
                Delay(DELAY_TIME); // 延时以达到动画效果
                x = (int)( x0 - nRadius*sin( lfRadian + lfRadian2Degree * i ) ); // (PI/180.0)是一个单位弧度
                y = (int)( y0 + nRadius*cos( lfRadian + lfRadian2Degree * i ) );
                MoveWindow(hWnd, x, y, PIECE_WIDTH, PIECE_HEIGHT, TRUE);
                InvalidateRect(GetParent(hWnd), NULL, TRUE); // 重画主窗口
                UpdateWindow(GetParent(hWnd));
            }
            MoveWindow(hWnd, x_table[posEnd], y_table[posEnd], PIECE_WIDTH, PIECE_HEIGHT, TRUE);
        }
        else if (nMoveStyle==-1 || nMoveStyle==-2 || nMoveStyle==-3 || nMoveStyle==-4 ||
            nMoveStyle==-100 || nMoveStyle==-200 || nMoveStyle==-300 || nMoveStyle==-400)
        {
            switch(nMoveStyle)
            {
                case -1: // 左上角内轨逆向
                    lfRadian = 0.0;
                    nRadius = 56;
                    x0 = 123;
                    y0 = 120;
                    break;
                case -2: // 右上角内轨逆向
                    lfRadian = -PI/2.0;
                    nRadius = 56;
                    x0 = 403;
                    y0 = 120;
                    break;
                case -3: // 左下角内轨逆向
                    lfRadian = PI/2.0;
                    nRadius = 56;
                    x0 = 123;
                    y0 = 400;
                    break;
                case -4: // 右下角内轨逆向
                    lfRadian = -PI*1.0;
                    nRadius = 56;
                    x0 = 403;
                    y0 = 400;
                    break;
                case -100: // 左上角外轨逆向
                    lfRadian = 0.0;
                    nRadius = 112;
                    x0 = 123;
                    y0 = 120;
                    break;
                case -200: // 右上角外轨逆向
                    lfRadian = -PI/2.0;
                    nRadius = 112;
                    x0 = 403;
                    y0 = 120;
                    break;
                case -300: // 左下角外轨逆向
                    lfRadian = PI/2.0;
                    nRadius = 112;
                    x0 = 123;
                    y0 = 400;
                    break;
                case -400: // 右下角外轨逆向
                    lfRadian = -PI*1.0;
                    nRadius = 112;
                    x0 = 403;
                    y0 = 400;
                    break;
            }
            for(int i=0; i<270; i++) // 显示绕轨走子动画
            {
                Delay(DELAY_TIME);
                x = (int)( x0 + nRadius*cos( lfRadian + lfRadian2Degree * i ) ); // (PI/180.0)是一个单位弧度
                y = (int)( y0 - nRadius*sin( lfRadian + lfRadian2Degree * i ) );
                MoveWindow(hWnd, x, y, PIECE_WIDTH, PIECE_HEIGHT, TRUE);
                InvalidateRect(GetParent(hWnd), NULL, TRUE); // 重画主窗口
                UpdateWindow(GetParent(hWnd));
            }
            // 弧度度运算有偏差，纠正偏差位置
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
                
            if (nDiffX == 0) // X轴方向像素偏量
            {
                nHPixel = 0;
            } else if (nDiffX > 0) {
                nHPixel = -1;
            } else if (nDiffX < 0) {
                nHPixel = 1;
            }
                
            if (nDiffY == 0) // Y轴方向像素偏量
            {
                nVPixel = 0;
            } else if (nDiffY > 0) {
                nVPixel = -1;
            } else if (nDiffY < 0) {
                nVPixel = 1;
            }

            while(x0 != x || y0 != y) // 显示直线移动动画
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
    // 销毁被吃棋子
    if (path.front() <=3)
    {
        if(g_board[path.back()].nKind == MAN_PIECE) // 人吃机器子
            g_mac[ g_board[path.back()].nPrevPieceIdx ].BeEaten();
        else // 机器吃人
            g_man[ g_board[path.back()].nPrevPieceIdx ].BeEaten();
    }
    // 将棋盘中的数据统一
    g_board[path.back()].nPrevPieceIdx = g_board[path.back()].nCurPieceIdx;
    // 播放吃子声音
    PlaySound( MAKEINTRESOURCE(IDR_MOVE), GetModuleHandle(NULL), SND_RESOURCE);// | SND_ASYNC );
    
    for(int i=0; i<12; i++)
        EnableWindow(g_man[i].GetPieceWnd(), TRUE);

    CChessPiece::s_bIsMoving = false;

    return 1;
}