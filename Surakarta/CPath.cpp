/*!@file
*******************************************************************************
文件名: CPath.cpp
功能:
作者:   姚玉亮
备注:
******************************************************************************/
#include "stdafx.h"
#include "CPath.h"

// 测试时用
void AddLog(TCHAR *pText);

void EvalutePath(PATH path);
// x值变化表: x = 123 + j * 56;
static int x_table[] = { 133, 189, 245, 301, 357, 413,
                  133, 189, 245, 301, 357, 413,
                  133, 189, 245, 301, 357, 413,
                  133, 189, 245, 301, 357, 413,
                  133, 189, 245, 301, 357, 413,
                  133, 189, 245, 301, 357, 413 };
// y值变化表: y = 120 + i * 56;
static int y_table[] = { 130, 130, 130, 130, 130, 130,
                  186, 186, 186, 186, 186, 186,
                  242, 242, 242, 242, 242, 242,
                  298, 298, 298, 298, 298, 298,
                  354, 354, 354, 354, 354, 354,
                  410, 410, 410, 410, 410, 410 };
/*****************第1部分（共3部分）：机器路径类定义部分**********************/
void CMachinePath::Push(const DIRECTION cdirect, const POSITION cpos)
{
    m_vectPath[cdirect].push_back(cpos);
}
// 所需参数：当前棋子的下标和位置
void CMachinePath::SetFinalPath(const DIRECTION cdirect, POSITION &m_pos)
{
    vector<POSITION>().swap(m_vectFinalPath); // 将路径清空
    m_vectFinalPath.swap( m_vectPath[cdirect] ); // 赋值给最终路径
    // 将走子路径中最后一点（着子点）位置赋值给棋子，告诉棋子位置发生变化
    m_pos = m_vectFinalPath.back();
    // 将第1点棋子类型赋值到最后一点，将最后一点类型置为空
    g_board[ m_vectFinalPath.back() ].nKind = 
    g_board[ m_vectFinalPath.at(1)  ].nKind;
    g_board[ m_vectFinalPath.at(1) ].nKind = NULL_PIECE;
    // 将最后一点中所存放的棋子的下标改为当前所移动的棋子的下标
    g_board[ m_vectFinalPath.back() ].nCurPieceIdx = g_board[m_vectFinalPath.at(1)].nCurPieceIdx;
}
void CMachinePath::SetFinalPath(PATH path, POSITION &m_pos)
{
    m_vectFinalPath.assign(path.begin(), path.end());
    //m_vectFinalPath.swap(path);
    // 将走子路径中最后一点（着子点）位置赋值给棋子，告诉棋子位置发生变化
    m_pos = m_vectFinalPath.back();
    // 将第1点棋子类型赋值到最后一点，将最后一点类型置为空
    g_board[ m_vectFinalPath.back() ].nKind = 
    g_board[ m_vectFinalPath.at(1)  ].nKind;
    g_board[ m_vectFinalPath.at(1) ].nKind = NULL_PIECE;
    // 将最后一点中所存放的棋子的下标改为当前所移动的棋子的下标
    g_board[ m_vectFinalPath.back() ].nCurPieceIdx = g_board[m_vectFinalPath.at(1)].nCurPieceIdx;
}
void CMachinePath::GetFinalPath(PATH &path)
{
    // copy final path info to parameter
    path.assign(m_vectFinalPath.begin(), m_vectFinalPath.end());
}

void CMachinePath::GetPath(const DIRECTION cdirect, PATH &path)
{
    path.assign(m_vectPath[cdirect].begin(), m_vectPath[cdirect].end());
    // path.swap(m_vectPath[cdirect]);
}
bool CMachinePath::Condition(POSITION &pos, const DIRECTION &cdirect)
{
    switch(cdirect)
    {
    case UP_TO_FLY:
        pos -= 6;
        if ( pos < 0) // 到达最顶一行
        {
            pos += 6; // 
            return false;
        }
        else return true;
    case RIGHT_TO_FLY:
        pos += 1;
        if ( pos % 6 == 0 )
        {
            pos -= 1;
            return false;
        }
        else return true;
    case DOWN_TO_FLY:
        pos += 6;
        if ( pos / 6 == 6)
        {
            pos -= 6;
            return false;
        }
        else return true;
    case LEFT_TO_FLY:
        pos -= 1;
        if (pos % 6 == 5 )
        {
            pos += 1;
            return false;
        }
        else return true;
    default:
        MessageBox(NULL, TEXT("在CPath.cpp中出错,飞行方向溢出"), NULL, MB_OK);
        exit(0);
    }
}
void CMachinePath::FlagEatablePath(const POSITION cposLast, const DIRECTION cdirect)
{
    EvalutePath(m_vectPath[cdirect]);
}
void CMachinePath::FlyEngine(const POSITION cposOriginal, POSITION posCur,
	const DIRECTION cdirectOriginal, DIRECTION directCur, bool bAlreadyFly)
{
    while( Condition(posCur, directCur) == true )
    {
		// 如果当前点不为空，有子
        if ( g_board[posCur].nKind != NULL_PIECE )
        {
			// 当前棋子和起点子不一样时，即当前为对方棋子
            if ( g_board[posCur].nKind != g_board[cposOriginal].nKind )
            {
                if ( bAlreadyFly )
                {
                    m_vectPath[cdirectOriginal].push_back(posCur);
                    FlagEatablePath(posCur, cdirectOriginal);
                    return;
                }
                else return;
            }
            else // 自己方棋子
            {
                if ( posCur == cposOriginal ) // 和起点相同
                {
                    if (m_vectPath[cdirectOriginal].size() < 6)
                        continue;
                    else
                        return;
                }
                else // 和起点不同
                    return;
            }
        }
    } // End of for

	if ( posCur == 0 || posCur==5 || posCur==30 || posCur==35)
		return;
	// 直走到头，将最后一点加入路径
	m_vectPath[cdirectOriginal].push_back( posCur );

	static const POSITION pos_table[]={ -1,  6, 12, 17, 11, -1,
		                    1, -1, -1, -1, -1,  4,
						    2, -1, -1, -1, -1,  3,
						   32, -1, -1, -1, -1, 33,
						   31, -1, -1, -1, -1, 34,
						   -1, 24, 18, 23, 29, -1
	                     };

    posCur = pos_table[posCur];          // 换位置至下一轨道第一点

    if (g_board[posCur].nKind != NULL_PIECE ) // 下一方向的第一点不为空，即有子
    {
        if (g_board[posCur].nKind != g_board[cposOriginal].nKind) // 和起始原点子不一样，即为对方子
        {
            m_vectPath[cdirectOriginal].push_back( posCur );

            FlagEatablePath(posCur, cdirectOriginal);
            return;
        }
        else return;
    }
    else // 当前轨道空，进入下一轨道
    {
        m_vectPath[cdirectOriginal].push_back(posCur);
		static const DIRECTION direct_table[] = { -1,  DOWN_TO_FLY, DOWN_TO_FLY, DOWN_TO_FLY, DOWN_TO_FLY, -1,
			                         RIGHT_TO_FLY, -1, -1, -1, -1, LEFT_TO_FLY,
									 RIGHT_TO_FLY, -1, -1, -1, -1, LEFT_TO_FLY,
									 RIGHT_TO_FLY, -1, -1, -1, -1, LEFT_TO_FLY,
									 RIGHT_TO_FLY, -1, -1, -1, -1, LEFT_TO_FLY,
									 -1,    UP_TO_FLY,  UP_TO_FLY,   UP_TO_FLY,   UP_TO_FLY, -1
		                           };

        directCur = direct_table[posCur];
        FlyEngine(cposOriginal, posCur, cdirectOriginal, directCur, true);
        return;
    }
}
// 根据位置参数，向4个方向飞行
void CMachinePath::Fly(const POSITION cpos)
{
    for(DIRECTION direct=UP_TO_FLY; direct<=LEFT_TO_FLY; direct++)
    {
        m_vectPath[direct].assign(1,direct);

        m_vectPath[direct].push_back(cpos);
        FlyEngine(cpos, cpos, direct, direct, false);
    }
}
/***********************第2部分（共3部分）：人走子类定义部分****************************/
void CPath::Walk(const POSITION cpos)
{
    POSITION pos_table[] = { 0, 0, 0, 0, -6, -5, 1, 7, 6, 5, -1, -7};
    POSITION posNext;
    for (DIRECTION direct=UP; direct<=UPLEFT; direct++)
    {
        posNext = cpos + pos_table[direct]; // 对应方向上的点下标

        if (posNext < 0 || posNext > 35) continue;
        if ( (posNext % 6 == 5) && (cpos % 6 == 0) )
            continue;
        if ( (posNext % 6 == 0) && (cpos % 6 == 5) )
            continue;

        if ( g_board[posNext].nKind == NULL_PIECE )
        {
            vector<POSITION>().swap(m_vectPath[direct]);

            m_vectPath[direct].push_back(direct);
            m_vectPath[direct].push_back(cpos);
            m_vectPath[direct].push_back(posNext);

            m_hPathWnd[direct] = CreateWindowEx(NULL, TEXT("BUTTON"), NULL,
                WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                x_table[posNext], y_table[posNext],
                PATH_WND_WIDTH, PATH_WND_HEIGHT, g_hBoardWnd,
                (HMENU)(200+direct), g_hInst, NULL);
        }
    }

}
void CPath::FlagEatablePath(const POSITION cpos,const DIRECTION cdirect)
{
	m_hPathWnd[cdirect] = CreateWindowEx(NULL, TEXT("BUTTON"), NULL,
        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
        x_table[cpos], y_table[cpos],
        PATH_WND_WIDTH, PATH_WND_HEIGHT, g_hBoardWnd,
        (HMENU)(cdirect+200), g_hInst, NULL);
}
void CPath::DestroyPathWnd()
{
    for(int i=0; i<12; i++)
    {
        if(m_hPathWnd[i] != NULL)
        {
            DestroyWindow(m_hPathWnd[i]);
            m_hPathWnd[i] = NULL;
        }
    }
}