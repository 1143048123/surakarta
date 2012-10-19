/*!@file
*******************************************************************************
�ļ���: Sura.cpp
����:   ��ɶ��û�����Ļ���,���û��Ĳ���������Ӧ�ʹ���
����:   Ҧ����
��ע:
******************************************************************************/

#include "stdafx.h"
#include "surakarta.h"
#include "CChessPiece.h"
////////////////////////////////////////////////////////////////////////
// ȫ�ֱ����Ķ���
HINSTANCE   g_hInst;
HWND        g_hBoardWnd;
//HDC         g_memPieceWndDC;
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

TCHAR     szMainWndClassName[ MAX_LOADSTRING ]; // ע�������ڵ�����
TCHAR     szMainWndTitle    [ MAX_LOADSTRING ]; // �����ڱ���
HDC       memMainWndDC; // �������ڴ�Device Context

HBITMAP   hBoardBmp;    // ����ͼ
HBITMAP   hMacPieceBmp; // �ҷ�����λͼ
HBITMAP   hManPieceBmp; // �������λͼ
HBITMAP   hPathWndBmp;  // ������λ�õı��
//<<<<<<<<<<<<<<<<<<<<<<<<<< Forward declarations <<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// TODO: ǰ�������ڴ˴����
ATOM     WndRegisterClass(HINSTANCE);  // ע�ᴰ����
BOOL     InitInstance(HINSTANCE, int); // ��ʼ������ʵ��
LRESULT  CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM); // �����ڴ��ڹ��̺���
INT_PTR  CALLBACK GetServerIp(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
VOID     RepaintChessboard(HDC , RECT);  // �ػ�����
DWORD    CALLBACK MachineOffice(LPVOID param);
void     AddLog(TCHAR *str);
bool     IsInnerOrbitOk();
bool     InnerOrbitAddPiece();
bool     DoubleOrbitAddPiece();
bool     PrepareForAttack();

/*!@main function
*******************************************************************************
����: �������,
     �ú������5�������ݣ�
     1.���س���������Դ
     2.��ʼ���������������
     3.ע�ᴰ����
     4.��ʼ������ʵ��������������
     5.������Ϣѭ��
******************************************************************************/
int WINAPI WinMain( HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR     szCmdLine,
                    int       nShowCmd )
{
    // ����������Դ
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

    WndRegisterClass( hInstance ); // ע�ᴰ����

    // ��ʼ������ʵ��
    if (!InitInstance(hInstance, nShowCmd))
    {
        MessageBox(NULL, TEXT("Init failed!"), NULL, MB_OK);
        exit(0);
    }
    AddLog(TEXT("==========��Ϸ��ʼ==========="));
    
    // ��Ϣѭ��
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return msg.wParam;
}

/*!@function
*******************************************************************************
����:   ע�����̴�����
������: WndRegisterClass
����:   hInstance [in] ����ʵ��
����ֵ: ע��ɹ��Ĵ�����
******************************************************************************/
ATOM WndRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize     = sizeof(WNDCLASSEX);
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hCursor    = LoadCursor(NULL, IDC_ARROW);
    wcex.hIcon      = LoadIcon(hInstance, (LPCWSTR)IDI_APP);
    wcex.hIconSm    = LoadIcon(hInstance, (LPCWSTR)IDI_APP);
    wcex.hInstance  = hInstance;
    wcex.hbrBackground = NULL;
    wcex.lpfnWndProc   = MainWndProc;
    wcex.lpszClassName = szMainWndClassName;
    wcex.lpszMenuName  = MAKEINTRESOURCE(IDR_MENU1);
    wcex.style = CS_DROPSHADOW;

    return RegisterClassEx(&wcex);
}


/*!@function
*******************************************************************************
����: ��ɳ���ʵ���ĳ�ʼ������
        ������
        1.��ʵ������洢��ȫ�ֱ�����
        2.�������а�͸�����Ե�������
        3.ʹ�����ھ���
        4.������������
        5.��ʾ������
������: InitInstance
����: hInstance [in] ʵ�����
      nShowCmd [in] ���򴰿ڳ�ʼ��ʾ״̬
******************************************************************************/
BOOL InitInstance(HINSTANCE hInstance, int nShowCmd)
{
    g_hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����
    // ����������,������
    g_hBoardWnd = CreateWindowEx(NULL, szMainWndClassName, szMainWndTitle,
                                WS_CLIPCHILDREN | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                                CW_USEDEFAULT, 0, CLIENT_WIDTH, CLIENT_HEIGHT + 60,
                                NULL, NULL, hInstance, NULL);
    if (!g_hBoardWnd)
    {
        MessageBox(NULL, TEXT("���ڳ�ʼ��ʧ��"), TEXT("Error"), MB_OK);
        return FALSE;
    }
    // �����ͻ���������λ��
    int nWidth  = CLIENT_WIDTH + GetSystemMetrics(SM_CXFRAME);
    int nHeight = CLIENT_HEIGHT + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYMENU)
                 + GetSystemMetrics(SM_CYFRAME) + LOG_WND_HEIGHT;
     // ��Ⱥ͸߶ȼ�2�����ڵ�������
    SetWindowPos(g_hBoardWnd, NULL, 0, 0, nWidth-2, nHeight-2, SWP_NOMOVE);

    HDC hdc = GetDC(g_hBoardWnd);
    memMainWndDC = CreateCompatibleDC(hdc); // ����������DC������Ӧ��С���ڴ�DC
    ReleaseDC(g_hBoardWnd, hdc);

    // ������־����,����ʵʱ��¼����ʾ�������
    g_hLogWnd = CreateWindowEx(NULL, TEXT("EDIT"), NULL,
        WS_VISIBLE | WS_CHILD | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
        0, 600, CLIENT_WIDTH, LOG_WND_HEIGHT,
        g_hBoardWnd, NULL, g_hInst, NULL);

    // ��ʼ����������
    for(int i=0; i<=11; i++)
        g_board[i].nKind = MACHINE_PIECE;
    for(int i=12; i<=23; i++)
        g_board[i].nKind = NULL_PIECE;
    for(int i=24; i<=35; i++)
        g_board[i].nKind = MAN_PIECE;

    for(int i=0; i<=11; i++)
    {
        g_board[i].nCurPieceIdx = i; // ���������±�
        g_board[i].nPrevPieceIdx = i;
        g_board[i+24].nCurPieceIdx = i; // �������±�
        g_board[i+24].nPrevPieceIdx = i;
    }
    //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    int nullOrbitPos_table[]   = {0, 5, 30, 35}; // ��4��λ��
    int innerOrbitPos_table[]  = {1, 4,  6,  7, 10, 11, 24, 25, 28, 29, 31, 34}; // ��12��λ��
    int outerOrbitPos_table[]  = {2, 3, 12, 14, 15, 17, 18, 20, 21, 23, 32, 33}; // ��12��λ��
    int doubleOrbitPos_table[] = {8, 9, 13, 16, 19, 22, 26, 27}; // ��8��λ��
    // ���ڹ���
    for (int i=0; i<4; i++)
        g_board[ nullOrbitPos_table[i] ].chOrbit = NULL_ORBIT;
    for (int i=0; i<12; i++)
    // ���ڹ�
        g_board[ innerOrbitPos_table[i] ].chOrbit = INNER_ORBIT;
    // �����
    for (int i=0; i<12; i++)
        g_board[ outerOrbitPos_table[i] ].chOrbit = OUTER_ORBIT;
    // ˫��
    for (int i=0; i<8; i++)
        g_board[ doubleOrbitPos_table[i] ].chOrbit = DOUBLE_ORBIT;
  
    //<<<<<<<<<<<<<<<<<<<< �������е�����<<<<<<<<<<<<<<<<<<<<<<<<
    for(int i=0; i<12; i++)
    {
        g_mac[i].New((char)i);
        g_man[i].New( (char)(24 + i) );
    }
    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    ShowWindow( g_hBoardWnd, nShowCmd );
    UpdateWindow( g_hBoardWnd );

    return TRUE;
}

/*!@function
*******************************************************************************
����: ���ı���(�������ϵ�״̬��Ϣ��)��׷���ı�
������: AddLog
����: pText [in] Ҫ׷�ӵ��ı�
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
����: ���ڹ��̺���
������: MainWndProc
����: hWnd [in] ���ھ��
      uMsg [in] ��Ϣ
      wParam [in] ��Ϣ���Ӳ���
      lParam [in] ��Ϣ���Ӳ���
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
    TCHAR szExitText[MAX_LOADSTRING]; // �˳�����ʱ����ʾ��

    switch (uMsg)
    {
        case WM_PAINT:
            hdc = BeginPaint( hWnd, &ps );
            GetClientRect(hWnd, &rect);
            RepaintChessboard( hdc, rect ); // �ػ����̱���
            EndPaint( hWnd, &ps );
            break;
        case WM_DRAWITEM: // ��ť�Ի棺������
            drawItemStruct = (LPDRAWITEMSTRUCT)lParam;
            
            hdc = GetDC(drawItemStruct->hwndItem);
            memPieceWndDC = CreateCompatibleDC(hdc); // Ϊ��ťDC������Ӧ��С���ڴ�DC
            ReleaseDC(drawItemStruct->hwndItem, hdc);
            
            // �������Ӱ�ť��ID���ж������������࣬ѡ����ӦͼƬ����
            if ( 999 == drawItemStruct->CtlID )
            {
                SelectObject(memPieceWndDC, hMacPieceBmp);     // ѡ�����������ͼƬ���ڴ�DC
            }
            else if (drawItemStruct->CtlID >= 100 && drawItemStruct->CtlID <= 111 )
            {
                SelectObject(memPieceWndDC, hManPieceBmp); // ѡ���������ͼƬ���ڴ�DC
            }
            else
            {   // ��ɶԸ���λ�õĻ�ͼ
                HRGN hRgn = CreateEllipticRgn(0, 0, PATH_WND_WIDTH, PATH_WND_HEIGHT);
                SetWindowRgn( drawItemStruct->hwndItem, hRgn, TRUE);
                SelectObject(memPieceWndDC, hPathWndBmp);
            }
            // �ڰ�ť����ͼ
            BitBlt(drawItemStruct->hDC, 0, 0, PIECE_WIDTH, PIECE_HEIGHT,
                memPieceWndDC, 0, 0, SRCCOPY);
            DeleteDC(memPieceWndDC);
            break;
        case WM_COMMAND:
            wmID = LOWORD(wParam);   // ��ȡ�ؼ�ID
            wmHWnd = (HWND)lParam;   // ID����Ӧ�Ĵ��ھ��
            switch( wmID )
            {
                //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
                // ����ҵ����ҵ�����ʱ
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
                //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
                // ����ҵ�����г���·��֮һʱ
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

                case ID_FILE_EXIT:
                    SendMessage(hWnd, WM_CLOSE, NULL, NULL);
                    break;
                // �˵�ѡ����Ӧ����
                    /*
                case ID_FILE_NEWGAME:
                    //ResetDate( 1 ); // �������ݲ���
                    // �������е�����
                    EnumChildWindows( hWnd, (WNDENUMPROC)DestroyAllChessman, TRUE );
                    // ���¼���˫������λͼ��·����ʾλͼ
                    hMacPieceBmp = (HBITMAP)LoadImage( g_hInst, (LPCWSTR)IDB_ME, IMAGE_BITMAP,
                        PIECE_WIDTH, PIECE_HEIGHT, LR_DEFAULTCOLOR);
                    hManPieceBmp = (HBITMAP)LoadImage( hInst, (LPCWSTR)IDB_PLAYER,
                        IMAGE_BITMAP, CHESSMAN_WIDTH, CHESSMAN_HEIGHT, LR_DEFAULTCOLOR);
                    hPathWndBmp = (HBITMAP)LoadImage( hInst, (LPCWSTR)IDB_PATH, IMAGE_BITMAP,
                        PATHWND_WIDTH, PATHWND_HEIGHT, LR_DEFAULTCOLOR);
                    DestroyPathWindow(); // �������п�����λ�õı��
                    // ���������������Ӱ�ť
                    CreateChessman( hWnd ); // �����������ݴ�����Ӧ����
                    break;
                case ID_FILE_OPEN_GAME:
                    break;
                case ID_FILE_SAVE_GAME:
                    break;
                case ID_FILE_EXIT:
                    SendMessage(hWnd, WM_CLOSE, NULL, NULL);
                    break;

                case ID_EDIT_BACKWARD:
                    break;
                case ID_EDIT_FORWARD:
                    break;
                case ID_VS_CREATE_GAME:
                    nTurn = ME_CHESSMAN;
                    EnumChildWindows(hWnd, DisablePlayerChessman, TRUE); // ��ֹ��ҵ������
                    CreateThread(NULL, 0, Server, hWnd, NULL, NULL);
                    break;
                case ID_VS_JOIN_GAME:
                    DialogBox(hInst, (LPCTSTR)IDD_GET_SERVER_IP, hWnd, (DLGPROC)GetServerIp);
                    break;
                */
                default:
                    return DefWindowProc( hWnd, uMsg, wParam, lParam );
            }
            break;
        case WM_EATEN:
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
            //DeleteDC( g_memPieceWndDC );
            PostQuitMessage( 0 );
            break;
        default:
            return DefWindowProc( hWnd, uMsg, wParam, lParam );
    }
    return 0;
}
/*
// �����ڡ������Ϣ�������
INT_PTR CALLBACK GetServerIp(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
    
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK )
		{
            // ��ȡIP��ַ�����ݵ�client::szServerIp
            GetWindowText(GetDlgItem(hDlg, IDC_IPADDRESS1), client::szServerIp, 16);
            client::W2A(); // ��IP��ַ��TCHAR��ת����char*��, ����client::pszServerIp��
            CreateThread(NULL, 0, Client, GetParent(hDlg), NULL, NULL); // �����ͻ����߳�
            EndDialog(hDlg, LOWORD(wParam));
		    return (INT_PTR)TRUE;
		} else if ( LOWORD(wParam) == IDCANCEL )
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
	}
	return (INT_PTR)FALSE;
}
*/

/*!@function
*******************************************************************************
����: �ػ�������
������: RepaintChessboard
����: hdc [in] ��Ҫ��ͼ�Ĵ��ڵ�HDC
      rect [in] ��Ҫ��ͼ�Ĵ��ڵ�RECT
******************************************************************************/
VOID RepaintChessboard( HDC hdc , RECT rect)
{
    SelectObject( memMainWndDC, hBoardBmp ); // Draw backgroud
    BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, memMainWndDC, 0, 0, SRCCOPY );
}

// �������������ܲ�
DWORD CALLBACK MachineOffice(LPVOID param)
{
    //<PRE> ��һ�����������пɳ���·����ȷ���Ƿ����
    map<int,Node>().swap(g_mapValueAndPath); // ���Ȩֵ-·����
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

    //</PRE> ��һ�����������пɳ���·����ȷ���Ƿ����

    //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    //</PRE> �ڶ���������ʱչ��������ʽ
    /* if (PrepareForAttack() == true)
        return 1;*/
    //</PRE> �ڶ���������ʱչ��������ʽ

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