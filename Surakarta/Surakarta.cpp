/*!@file Sura.cpp
*******************************************************************************
功能:   程序的入口点.完成对用户界面的绘制,对用户的操作进行响应和处理,以及全局的控制
作者:   姚玉亮(rookie2@163.com)
备注:
最近修改日期: 2012年10月21日
修改内容: 注释部分
******************************************************************************/

#include "stdafx.h"
#include "surakarta.h"
#include "CChessPiece.h"

/**
 * 全局变量的定义及初始化
 */

HINSTANCE   g_hInst;
HWND        g_hBoardWnd;
HWND        g_hLogWnd;
ChessBoard  g_board[36];
CPath       g_path;
MAP_VALUE_PATH g_mapValueAndPath;
CMacPiece   g_mac[12] = { CMacPiece(0), CMacPiece(1), CMacPiece(2), CMacPiece(3),
                          CMacPiece(4), CMacPiece(5), CMacPiece(6), CMacPiece(7),
                          CMacPiece(8), CMacPiece(9), CMacPiece(10), CMacPiece(11)
                        };
CManPiece   g_man[12] = { CManPiece(0), CManPiece(1), CManPiece(2), CManPiece(3),
                          CManPiece(4), CManPiece(5), CManPiece(6), CManPiece(7),
                          CManPiece(8), CManPiece(9), CManPiece(10), CManPiece(11)
                        };
/**
 * 声明局部变量
 */

TCHAR     szMainWndClassName[ MAX_LOADSTRING ]; // 注册主窗口的类名
TCHAR     szMainWndTitle    [ MAX_LOADSTRING ]; // 主窗口标题
HDC       memMainWndDC; // 主窗口内存Device Context
HBITMAP   hBoardBmp;    // 棋盘图,即用于棋盘的背景图片资源
HBITMAP   hMacPieceBmp; // 机器方棋子位图
HBITMAP   hManPieceBmp; // 玩家的棋子位图
HBITMAP   hPathWndBmp;  // 可走子位置的标记,用于区分可走位置与不可走位置

/**
 * 函数前置声明在此处添加
 */

ATOM     WndRegisterClass(HINSTANCE);
BOOL     InitInstance(HINSTANCE, int);
LRESULT  CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR  CALLBACK GetServerIp(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
VOID     RepaintChessboard(HDC , RECT);
DWORD    CALLBACK MachineOffice(LPVOID param);
void     AddLog(TCHAR *str);
bool     IsInnerOrbitOk();
bool     InnerOrbitAddPiece();
bool     DoubleOrbitAddPiece();
bool     PrepareForAttack();

/*!@function main
*******************************************************************************
功能: 程序入口,
     该函数完成5部分内容：
     1.加载程序所需资源
     2.初始化棋盘所需的数据
     3.注册窗口类
     4.初始化程序实例，创建主窗口
     5.进入消息循环
******************************************************************************/
int WINAPI WinMain( HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR     szCmdLine,
                    int       nShowCmd )
{
    /**
     * 加载及初始化所需资源
     */

    LoadString( hInstance, IDS_WNDCLASSNAME, szMainWndClassName, MAX_LOADSTRING );
    LoadString( hInstance, IDS_WNDTITLE,     szMainWndTitle,     MAX_LOADSTRING );
    
    hBoardBmp = (HBITMAP)LoadImage( hInstance, (LPCWSTR)IDB_BG, IMAGE_BITMAP,
        CLIENT_WIDTH, CLIENT_HEIGHT, LR_DEFAULTCOLOR );
    hMacPieceBmp = (HBITMAP)LoadImage( hInstance, (LPCWSTR)IDB_ME, IMAGE_BITMAP, 
        PIECE_WIDTH, PIECE_HEIGHT, LR_DEFAULTCOLOR);
    hManPieceBmp = (HBITMAP)LoadImage( hInstance, (LPCWSTR)IDB_PLAYER, IMAGE_BITMAP,
        PIECE_WIDTH, PIECE_HEIGHT, LR_DEFAULTCOLOR);
    hPathWndBmp = (HBITMAP)LoadImage( hInstance, (LPCWSTR)IDB_PATH, IMAGE_BITMAP,
        PATH_WND_WIDTH, PATH_WND_HEIGHT, LR_DEFAULTCOLOR);

    WndRegisterClass( hInstance ); // 注册窗口类

    // 初始化程序实例
    if (!InitInstance(hInstance, nShowCmd))
    {
        MessageBox(NULL, TEXT("Init failed!"), NULL, MB_OK);
        exit(0);
    }
    AddLog(TEXT("==========游戏开始==========="));
    
    // 进入消息循环
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return msg.wParam;
}

/*!@function WndRegisterClass
*******************************************************************************
功能:   注册棋盘的窗口类
参数:   hInstance [in] 程序实例
******************************************************************************/
ATOM WndRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize     = sizeof(WNDCLASSEX);
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hCursor    = LoadCursor(NULL, IDC_ARROW); // 光标
    wcex.hIcon      = LoadIcon(hInstance, (LPCWSTR)IDI_APP); // 程序图标
    wcex.hIconSm    = LoadIcon(hInstance, (LPCWSTR)IDI_APP); // 程序小图标
    wcex.hInstance  = hInstance;
    wcex.hbrBackground = NULL;
    wcex.lpfnWndProc   = MainWndProc; // 指定窗口过程函数
    wcex.lpszClassName = szMainWndClassName; // 窗口类名
    wcex.lpszMenuName  = MAKEINTRESOURCE(IDR_MENU1); // 菜单
    wcex.style = CS_DROPSHADOW;

    return RegisterClassEx(&wcex);
}


/*!@function InitInstance
*******************************************************************************
功能: 完成程序实例的初始化, 以及UI的初始化和显示工作
参数: hInstance [in] 程序实例句柄
      nShowCmd [in] 程序窗口初始显示状态
返回值: 成功返回true, 失败返回false.
******************************************************************************/
BOOL InitInstance(HINSTANCE hInstance, int nShowCmd)
{
    g_hInst = hInstance; // 将实例句柄存储在全局变量中
    // 创建主窗口,即棋盘
    g_hBoardWnd = CreateWindowEx(NULL, szMainWndClassName, szMainWndTitle,
        WS_CLIPCHILDREN | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, 0, CLIENT_WIDTH, CLIENT_HEIGHT + 60,
        NULL, NULL, hInstance, NULL);
    if (!g_hBoardWnd) // 如果主窗口创建失败
    {
        MessageBox(NULL, TEXT("窗口初始化失败"), TEXT("Error"), MB_OK);
        return FALSE;
    }
    // 调整客户区长宽，及位置
    int nWidth  = CLIENT_WIDTH + GetSystemMetrics(SM_CXFRAME);
    int nHeight = CLIENT_HEIGHT + GetSystemMetrics(SM_CYCAPTION)
                + GetSystemMetrics(SM_CYMENU)
                + GetSystemMetrics(SM_CYFRAME) + LOG_WND_HEIGHT;
     // 宽度和高度减2，用于调整画面
    SetWindowPos(g_hBoardWnd, NULL, 0, 0, nWidth-2, nHeight-2, SWP_NOMOVE);

    HDC hdc = GetDC(g_hBoardWnd);
    memMainWndDC = CreateCompatibleDC(hdc); // 根据主窗口DC创建相应大小的内存DC
    ReleaseDC(g_hBoardWnd, hdc);

    // 创建日志窗口,用于实时记录并显示走子情况
    g_hLogWnd = CreateWindowEx(NULL, TEXT("EDIT"), NULL,
        WS_VISIBLE | WS_CHILD | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
        0, 600, CLIENT_WIDTH, LOG_WND_HEIGHT,
        g_hBoardWnd, NULL, g_hInst, NULL);

    // 初始化棋盘数据部分
    for(int i=0; i<=11; i++)
        g_board[i].nKind = MACHINE_PIECE;
    for(int i=12; i<=23; i++)
        g_board[i].nKind = NULL_PIECE;
    for(int i=24; i<=35; i++)
        g_board[i].nKind = MAN_PIECE;
    for(int i=0; i<=11; i++)
    {
        g_board[i].nCurPieceIdx = i; // 机器棋子下标
        g_board[i].nPrevPieceIdx = i;
        g_board[i+24].nCurPieceIdx = i; // 人棋子下标
        g_board[i+24].nPrevPieceIdx = i;
    }
    // 初始化表内容
    int nullOrbitPos_table[]   = {0, 5, 30, 35}; // 不在轨道上,共4个位置
    int innerOrbitPos_table[]  = {1, 4,  6,  7, 10, 11, 24, 25, 28, 29, 31, 34}; // 共12个位置
    int outerOrbitPos_table[]  = {2, 3, 12, 14, 15, 17, 18, 20, 21, 23, 32, 33}; // 共12个位置
    int doubleOrbitPos_table[] = {8, 9, 13, 16, 19, 22, 26, 27}; // 共8个位置
    // 不在轨上
    for (int i=0; i<4; i++)
        g_board[ nullOrbitPos_table[i] ].chOrbit = NULL_ORBIT;
    for (int i=0; i<12; i++)
    // 纯内轨
        g_board[ innerOrbitPos_table[i] ].chOrbit = INNER_ORBIT;
    // 纯外轨
    for (int i=0; i<12; i++)
        g_board[ outerOrbitPos_table[i] ].chOrbit = OUTER_ORBIT;
    // 双轨
    for (int i=0; i<8; i++)
        g_board[ doubleOrbitPos_table[i] ].chOrbit = DOUBLE_ORBIT;  
    // 画出所有的棋子
    for(int i=0; i<12; i++)
    {
        g_mac[i].New((char)i);
        g_man[i].New( (char)(24 + i) );
    }

    ShowWindow( g_hBoardWnd, nShowCmd );
    UpdateWindow( g_hBoardWnd );

    return TRUE;
}

/*!@function AddLog
*******************************************************************************
功能: 向文本框(主窗口上的日志记录窗口)中追加文本(日志)
参数: pText [in] 要追加的文本
******************************************************************************/
void AddLog(TCHAR *pText)
{
    int nLen = GetWindowTextLength(g_hLogWnd);
    SendMessage(g_hLogWnd,  EM_SETSEL,     nLen, -1);
    SendMessage(g_hLogWnd,  EM_REPLACESEL, 0,    (LPARAM)pText);
    SendMessage(g_hLogWnd,  EM_REPLACESEL, 0,    (LPARAM)TEXT("\r\n"));
}

/*!@function
*******************************************************************************
功能: 窗口过程函数
函数名: MainWndProc
参数: hWnd [in] 窗口句柄
      uMsg [in] 消息
      wParam [in] 消息附加参数
      lParam [in] 消息附加参数
******************************************************************************/
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    HDC memPieceWndDC;
    PAINTSTRUCT ps;
    LPDRAWITEMSTRUCT drawItemStruct;
    RECT rect;
    int wmID;
    HWND wmHWnd;
    TCHAR szExitText[MAX_LOADSTRING]; // 退出程序时的提示语

    switch (uMsg)
    {
        case WM_PAINT:
            hdc = BeginPaint( hWnd, &ps );
            GetClientRect(hWnd, &rect);
            RepaintChessboard( hdc, rect ); // 重画棋盘背景
            EndPaint( hWnd, &ps );
            break;
        case WM_DRAWITEM: // 按钮自绘：画棋子
            drawItemStruct = (LPDRAWITEMSTRUCT)lParam;
            
            hdc = GetDC(drawItemStruct->hwndItem);
            memPieceWndDC = CreateCompatibleDC(hdc); // 为按钮DC创建相应大小的内存DC
            ReleaseDC(drawItemStruct->hwndItem, hdc);
            
            // 根据棋子按钮的ID，判断所绘棋子种类，选择相应图片画上
            if ( 999 == drawItemStruct->CtlID )
            {
                SelectObject(memPieceWndDC, hMacPieceBmp);     // 选择机器方棋子图片到内存DC
            }
            else if (drawItemStruct->CtlID >= 100 && drawItemStruct->CtlID <= 111 )
            {
                SelectObject(memPieceWndDC, hManPieceBmp); // 选择玩家棋子图片到内存DC
            }
            else
            {   // 完成对高亮位置的绘图
                HRGN hRgn = CreateEllipticRgn(0, 0, PATH_WND_WIDTH, PATH_WND_HEIGHT);
                SetWindowRgn( drawItemStruct->hwndItem, hRgn, TRUE);
                SelectObject(memPieceWndDC, hPathWndBmp);
            }
            // 在按钮上贴图
            BitBlt(drawItemStruct->hDC, 0, 0, PIECE_WIDTH, PIECE_HEIGHT,
                memPieceWndDC, 0, 0, SRCCOPY);
            DeleteDC(memPieceWndDC);
            break;
        case WM_COMMAND:
            wmID = LOWORD(wParam);   // 获取控件ID
            wmHWnd = (HWND)lParam;   // ID所对应的窗口句柄
            switch( wmID )
            {
                /**
                 * 当玩家点击玩家的棋子时
                 */
                case IDM_MAN0:
                    g_man[0].BeSelected();
                    break;
                case IDM_MAN1:
                    g_man[1].BeSelected();
                    break;
                case IDM_MAN2:
                    g_man[2].BeSelected();
                    break;
                case IDM_MAN3:
                    g_man[3].BeSelected();
                    break;
                case IDM_MAN4:
                    g_man[4].BeSelected();
                    break;
                case IDM_MAN5:
                    g_man[5].BeSelected();
                    break;
                case IDM_MAN6:
                    g_man[6].BeSelected();
                    break;
                case IDM_MAN7:
                    g_man[7].BeSelected();
                    break;
                case IDM_MAN8:
                    g_man[8].BeSelected();
                    break;
                case IDM_MAN9:
                    g_man[9].BeSelected();
                    break;
                case IDM_MAN10:
                    g_man[10].BeSelected();
                    break;
                case IDM_MAN11:
                    g_man[11].BeSelected();
                    break;

                /**
                 * 点玩家点击所列出的路径之一时
                 */
                case IDM_UP_TO_FLY:
                    g_man[CManPiece::s_nCurBeSelectedPieceIndex].Move(UP_TO_FLY);
                    CreateThread(NULL, 0, MachineOffice, NULL, NULL, NULL);
                    break;
                case IDM_RIGHT_TO_FLY:
                    g_man[CManPiece::s_nCurBeSelectedPieceIndex].Move(RIGHT_TO_FLY);
                    CreateThread(NULL, 0, MachineOffice, NULL, NULL, NULL);
                    break;
                case IDM_DOWN_TO_FLY:
                    g_man[CManPiece::s_nCurBeSelectedPieceIndex].Move(DOWN_TO_FLY);
                    CreateThread(NULL, 0, MachineOffice, NULL, NULL, NULL);
                    break;
                case IDM_LEFT_TO_FLY:
                    g_man[CManPiece::s_nCurBeSelectedPieceIndex].Move(LEFT_TO_FLY);
                    CreateThread(NULL, 0, MachineOffice, NULL, NULL, NULL);
                    break;
                case IDM_UP:
                    g_man[CManPiece::s_nCurBeSelectedPieceIndex].Move(UP);
                    CreateThread(NULL, 0, MachineOffice, NULL, NULL, NULL);
                    break;
                case IDM_UPRIGHT:
                    g_man[CManPiece::s_nCurBeSelectedPieceIndex].Move(UPRIGHT);
                    CreateThread(NULL, 0, MachineOffice, NULL, NULL, NULL);
                    break;
                case IDM_RIGHT:
                    g_man[CManPiece::s_nCurBeSelectedPieceIndex].Move(RIGHT);
                    CreateThread(NULL, 0, MachineOffice, NULL, NULL, NULL);
                    break;
                case IDM_DOWNRIGHT:
                    g_man[CManPiece::s_nCurBeSelectedPieceIndex].Move(DOWNRIGHT);
                    CreateThread(NULL, 0, MachineOffice, NULL, NULL, NULL);
                    break;
                case IDM_DOWN:
                    g_man[CManPiece::s_nCurBeSelectedPieceIndex].Move(DOWN);
                    CreateThread(NULL, 0, MachineOffice, NULL, NULL, NULL);
                    break;
                case IDM_DOWNLEFT:
                    g_man[CManPiece::s_nCurBeSelectedPieceIndex].Move(DOWNLEFT);
                    CreateThread(NULL, 0, MachineOffice, NULL, NULL, NULL);
                    break;
                case IDM_LEFT:
                    g_man[CManPiece::s_nCurBeSelectedPieceIndex].Move(LEFT);
                    CreateThread(NULL, 0, MachineOffice, NULL, NULL, NULL);
                    break;
                case IDM_UPLEFT:
                    g_man[CManPiece::s_nCurBeSelectedPieceIndex].Move(UPLEFT);
                    CreateThread(NULL, 0, MachineOffice, NULL, NULL, NULL);
                    break;

                /**
                 * 当用户点击菜单项时
                 */
                case ID_FILE_EXIT:
                    SendMessage(hWnd, WM_CLOSE, NULL, NULL);
                    break;
                default:
                    return DefWindowProc( hWnd, uMsg, wParam, lParam );
            }
            break;
        case WM_EATEN: // 吃子, 对被吃棋子的窗口进行销毁
            wmHWnd = (HWND)lParam;
            DestroyWindow(wmHWnd);
            break;
        case WM_CLOSE:
            LoadString(g_hInst, IDS_EXIT, szExitText, MAX_LOADSTRING);
            if ( IDYES == MessageBox(hWnd, szExitText, TEXT("Exit"), MB_YESNO | MB_ICONQUESTION) )
            {
                DestroyWindow(hWnd);
            }
            break;
        case WM_DESTROY:
            DeleteObject( hBoardBmp );
            DeleteObject( hMacPieceBmp );
            DeleteObject( hManPieceBmp );
            DeleteObject( hPathWndBmp );
            DeleteDC( memMainWndDC );
            PostQuitMessage( 0 );
            break;
        default:
            return DefWindowProc( hWnd, uMsg, wParam, lParam );
    }

    return 0;
}

/*!@function RepaintChessboard
*******************************************************************************
功能: 重绘主窗口
参数: hdc [in] 需要绘图的窗口的HDC
      rect [in] 需要绘图的窗口的RECT
******************************************************************************/
VOID RepaintChessboard( HDC hdc , RECT rect)
{
    SelectObject( memMainWndDC, hBoardBmp ); // Draw backgroud
    BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, memMainWndDC, 0, 0, SRCCOPY );
}

/*!@function MachineOffice
*******************************************************************************
功能: 线程函数.
     机器棋子走子总部, 用于告知机器如何走子, 总结所有机器走子算法有结果, 并择优启用
参数: 无用
******************************************************************************/
DWORD CALLBACK MachineOffice(LPVOID param)
{
    //<PRE> 第一步：搜索所有可吃子路径，确定是否吃子
    map<int,Node>().swap(g_mapValueAndPath); // 清空权值-路径表
    int cntMacNum = 0;
    for(int i=0; i<36; i++)
    {
        if (g_board[i].nKind == MACHINE_PIECE)
        {
            g_mac[ g_board[i].nCurPieceIdx ].Fly();
            cntMacNum++;
            if(cntMacNum > 11)
                break;
        }
    }
    
    if (g_mapValueAndPath.size() != 0)
    {
        int nValue = g_mapValueAndPath.rbegin()->first;
        if(nValue > 0)
        {
            int nIdx = g_mapValueAndPath.rbegin()->second.nIdx;
            DIRECTION direct = g_mapValueAndPath.rbegin()->second.direct;
            g_mac[nIdx].Move(direct);

            return 1;
        }
    }

    /**
     * 第一步：搜索所有可吃子路径，确定是否吃子
     */
    // TODO: 在此添加

    /**
     * 第二步：开局时展开攻击阵式
     */
    // TODO: 在此添加
    /*
    if (PrepareForAttack() == true)
        return 1;
    */

    PATH p0;
    if (g_board[25].nKind - g_board[19].nKind == 21)
    {
        p0.push_back(4);
        p0.push_back(25);
        p0.push_back(19);
        g_mac[g_board[25].nCurPieceIdx].Move(p0);

        return 1;
    }
    if (g_board[19].nKind - g_board[13].nKind == 21)
    {
        p0.push_back(4);
        p0.push_back(19);
        p0.push_back(13);
        g_mac[g_board[19].nCurPieceIdx].Move(p0);

        return 1;
    }
    if (g_board[13].nKind - g_board[7].nKind == 21)
    {
        p0.push_back(4);
        p0.push_back(13);
        p0.push_back(7);
        g_mac[g_board[13].nCurPieceIdx].Move(p0);

        return 1;
    }
    if ( InnerOrbitAddPiece() == true)
        return 1;
    DoubleOrbitAddPiece();
    
    return 1;
}