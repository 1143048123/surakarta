/*!@file Evaluation.cpp
*******************************************************************************
功能: 机器走子算法文件
作者: rookie2(rookie2@163.com)
描述:
最后修改日期: 2012年10月21日
******************************************************************************/

#include "stdafx.h"
#include "Evaluation.h"
#include "CChessPiece.h"

/**
 * 前置声明部分
 */
bool IsInnerOrbitOk();
bool InnerOrbitAddPiece();
bool DoubleOrbitAddPiece();
void EvalutePath(const PATH cpath);
bool PrepareForAttack();

/*
bool PrepareForAttack()
{
    PATH path;
    int nSumOfUpleft  = (g_board[1].nKind + g_board[ 6].nKind + g_board[ 7].nKind) / 21,
        nSumOfUpright = (g_board[4].nKind + g_board[10].nKind + g_board[11].nKind) / 21;

    if ( (g_board[25].nKind == NULL_PIECE && g_board[19].nKind == NULL_PIECE) ||
         ( (g_board[24].nKind + g_board[25].nKind + g_board[31].nKind) <= (g_board[28].nKind + g_board[29].nKind + g_board[34].nKind) )
       ) // 开左上角
    {
        if (nSumOfUpleft == 2 || nSumOfUpleft == 3) // 不是一个棋子
        {

            if (g_board[6].nKind - g_board[12].nKind == MACHINE_PIECE)
            {
                path.push_back(DOWN);
                path.push_back(6);
                path.push_back(12);
                g_mac[g_board[6].nCurPieceIdx].Move(path);
                return true;
            }
            else if (g_board[1].nKind - g_board[2].nKind == MACHINE_PIECE)
            {
                path.push_back(RIGHT);
                p.push_back(1);
                p.push_back(2);
            }
            else if (g_board[7].nKind - g_board[14].nKind == MACHINE_PIECE)
            {
                p.push_back(DOWNRIGHT);
                p.push_back(7);
                p.push_back(14);
            }
        }
        else // 拐角处中人一颗棋子
        {
            if (g_board[1].nKind - g_board[7].nKind == MACHINE_PIECE)
            {
                p.push_back(DOWN);
                p.push_back(1);
                p.push_back(7);
                g_mac[g_board[1].nCurPieceIdx].Move(p);
                return;
            }
            else if (g_board[6].nKind - g_board[7].nKind == MACHINE_PIECE)
            {
                p.push_back(RIGHT);
                p.push_back(6);
                p.push_back(7);
                g_mac[g_board[6].nCurPieceIdx].Move(p);
                return;
            }
        }
    }
    else // 开右上角 
    {

    }

    map<int,int>mapInnerOrbitValue;
    int nResult = 0; // 最终拐角
    mapInnerOrbitValue.insert( pair<int,int>(
        (g_board[24].nKind + g_board[25].nKind + g_board[31].nKind) / 21, DOWNLEFT) );
    mapInnerOrbitValue.insert( pair<int,int>(
        (g_board[28].nKind + g_board[29].nKind + g_board[34].nKind) / 21, DOWNRIGHT) );

    int nSum;
    map<int,int>::iterator iter;
    DIRECTION direct = -1;
    for(iter = mapInnerOrbitValue.begin(); iter!=mapInnerOrbitValue.end(); iter++)
    {
        nSum = iter->first;
        if(nSum != 0)
        {
            direct = iter->second;
            break;
        }
    }
    PATH p;
    switch(direct)
    {
        case UPLEFT:
            
            break;
        case UPRIGHT:
            break;
        case DOWNLEFT:
            break;
        case DOWNRIGHT:
            break;
        default: // 没有
            break;
    } // End of switch

    return false;
}
*/

/*!@function IsInnerOrbitOk
*******************************************************************************
功能: 判断内轨的双方棋子总数之差，机器方多则返回true,反之为false
******************************************************************************/
bool IsInnerOrbitOk()
{
    int nSum = g_board[1].nKind + g_board[4].nKind;
    for(int i=6; i<=11; i++)
        nSum += g_board[i].nKind;
    nSum += g_board[13].nKind + g_board[16].nKind;
    nSum += g_board[19].nKind + g_board[22].nKind;
    for(int i=24; i<=29; i++)
        nSum += g_board[i].nKind;
    nSum += g_board[31].nKind + g_board[34].nKind;

    int nMacNum = nSum / 21;
    int nManNum = nSum % 21;
    if(nMacNum - nManNum >= 0)  return true;
    else                        return false;
}

/*!@function InnerOrbitAddPiece
*******************************************************************************
功能: 向内轨添加棋子
******************************************************************************/
bool InnerOrbitAddPiece()
{
    PATH path;
    //先4个角，再纯内轨，再双轨
    int innerOrbitPos_table[] = { 7, 10, 28, 25, // 4个角
                                  1,  6,  4, 11, 34, 29, 31, 24, // 8个纯内轨
                                  8,  9, 16, 22, 27, 26, 13, 19  // 8个双轨
                                };
    for(int i=0; i<20; i++)
    {
        if (g_board[ innerOrbitPos_table[i] ].nKind == NULL_PIECE)
        {
            switch(innerOrbitPos_table[i])
            {
                case 7:
                    if (g_board[0].nKind - g_board[7].nKind == 21)
                    {
                        path.push_back(7);
                        path.push_back(0);
                        path.push_back(7);
                        g_mac[g_board[0].nCurPieceIdx].Move(path);
                        return true;
                    }
                    else if (g_board[2].nKind - g_board[7].nKind == 21)
                    {
                        path.push_back(9);
                        path.push_back(2);
                        path.push_back(7);
                        g_mac[g_board[2].nCurPieceIdx].Move(path);
                        return true;
                    }
                    else if (g_board[12].nKind - g_board[7].nKind == 21)
                    {
                        path.push_back(5);
                        path.push_back(12);
                        path.push_back(7);
                        g_mac[g_board[12].nCurPieceIdx].Move(path);
                        return true;
                    }
                    else if (g_board[14].nKind - g_board[7].nKind == 21)
                    {
                        path.push_back(11);
                        path.push_back(14);
                        path.push_back(7);
                        g_mac[g_board[14].nCurPieceIdx].Move(path);
                        return true;
                    }
                    break;
                case 10:
                    if (g_board[5].nKind - g_board[10].nKind == 21)
                    {
                        path.push_back(9);
                        path.push_back(5);
                        path.push_back(10);
                        g_mac[g_board[5].nCurPieceIdx].Move(path);
                        return true;
                    }
                    else if (g_board[3].nKind - g_board[10].nKind == 21)
                    {
                        path.push_back(7);
                        path.push_back(3);
                        path.push_back(10);
                        g_mac[g_board[3].nCurPieceIdx].Move(path);
                        return true;
                    }
                    else if (g_board[17].nKind - g_board[10].nKind == 21)
                    {
                        path.push_back(11);
                        path.push_back(11);
                        path.push_back(10);
                        g_mac[g_board[11].nCurPieceIdx].Move(path);
                        return true;
                    }
                    else if (g_board[15].nKind - g_board[10].nKind == 21)
                    {
                        path.push_back(5);
                        path.push_back(15);
                        path.push_back(10);
                        g_mac[g_board[15].nCurPieceIdx].Move(path);
                        return true;
                    }
                    break;
                case 28:
                    if (g_board[33].nKind - g_board[28].nKind == 21)
                    {
                        path.push_back(5);
                        path.push_back(33);
                        path.push_back(28);
                        g_mac[g_board[33].nCurPieceIdx].Move(path);
                        return true;
                    }
                    else if (g_board[23].nKind - g_board[28].nKind == 21)
                    {
                        path.push_back(9);
                        path.push_back(23);
                        path.push_back(28);
                        g_mac[g_board[23].nCurPieceIdx].Move(path);
                        return true;
                    }
                    else if (g_board[21].nKind - g_board[28].nKind == 21)
                    {
                        path.push_back(7);
                        path.push_back(21);
                        path.push_back(28);
                        g_mac[g_board[21].nCurPieceIdx].Move(path);
                        return true;
                    }
                    break;
                case 25:
                    if (g_board[32].nKind - g_board[25].nKind == 21)
                    {
                        path.push_back(11);
                        path.push_back(32);
                        path.push_back(25);
                        g_mac[g_board[32].nCurPieceIdx].Move(path);
                        return true;
                    }
                    else if (g_board[18].nKind - g_board[25].nKind == 21)
                    {
                        path.push_back(7);
                        path.push_back(18);
                        path.push_back(25);
                        g_mac[g_board[18].nCurPieceIdx].Move(path);
                        return true;
                    }
                    else if (g_board[20].nKind - g_board[25].nKind == 21)
                    {
                        path.push_back(9);
                        path.push_back(20);
                        path.push_back(25);
                        g_mac[g_board[20].nCurPieceIdx].Move(path);
                        return true;
                    }
                    break;

                case 1:
                    if (g_board[0].nKind - g_board[1].nKind == 21)
                    {
                        path.push_back(6);
                        path.push_back(0);
                        path.push_back(1);
                        g_mac[g_board[0].nCurPieceIdx].Move(path);
                        return true;
                    }
                    else if (g_board[2].nKind - g_board[1].nKind == 21)
                    {
                        path.push_back(10);
                        path.push_back(2);
                        path.push_back(1);
                        g_mac[g_board[2].nCurPieceIdx].Move(path);
                        return true;
                    }
                    break;
                case 6:
                    if (g_board[0].nKind - g_board[6].nKind == 21)
                    {
                        path.push_back(8);
                        path.push_back(0);
                        path.push_back(6);
                        g_mac[g_board[0].nCurPieceIdx].Move(path);
                        return true;
                    }
                    else if (g_board[12].nKind - g_board[6].nKind == 21)
                    {
                        path.push_back(4);
                        path.push_back(12);
                        path.push_back(6);
                        g_mac[g_board[12].nCurPieceIdx].Move(path);
                        return true;
                    }
                    break;
                case 4:
                    if (g_board[5].nKind - g_board[4].nKind == 21)
                    {
                        path.push_back(10);
                        path.push_back(5);
                        path.push_back(4);
                        g_mac[g_board[5].nCurPieceIdx].Move(path);
                        return true;
                    }
                    else if (g_board[3].nKind - g_board[4].nKind == 21)
                    {
                        path.push_back(6);
                        path.push_back(3);
                        path.push_back(4);
                        g_mac[g_board[3].nCurPieceIdx].Move(path);
                        return true;
                    }
                    break;
                case 11:
                    if (g_board[5].nKind - g_board[11].nKind == 21)
                    {
                        path.push_back(8);
                        path.push_back(5);
                        path.push_back(11);
                        g_mac[g_board[5].nCurPieceIdx].Move(path);
                        return true;
                    }
                    else if (g_board[17].nKind - g_board[11].nKind == 21)
                    {
                        path.push_back(4);
                        path.push_back(17);
                        path.push_back(11);
                        g_mac[g_board[17].nCurPieceIdx].Move(path);
                        return true;
                    }
                    break;
                case 34:
                    if (g_board[35].nKind - g_board[34].nKind == 21)
                    {
                        path.push_back(10);
                        path.push_back(35);
                        path.push_back(34);
                        g_mac[g_board[35].nCurPieceIdx].Move(path);
                        return true;
                    }
                    else if (g_board[33].nKind - g_board[34].nKind == 21)
                    {
                        path.push_back(6);
                        path.push_back(33);
                        path.push_back(34);
                        g_mac[g_board[33].nCurPieceIdx].Move(path);
                        return true;
                    }
                    break;
                case 29:
                    if (g_board[35].nKind - g_board[29].nKind == 21)
                    {
                        path.push_back(4);
                        path.push_back(35);
                        path.push_back(29);
                        g_mac[g_board[35].nCurPieceIdx].Move(path);
                        return true;
                    }
                    else if (g_board[23].nKind - g_board[29].nKind == 21)
                    {
                        path.push_back(8);
                        path.push_back(23);
                        path.push_back(29);
                        g_mac[g_board[23].nCurPieceIdx].Move(path);
                        return true;
                    }
                    break;
                case 31:
                    if (g_board[30].nKind - g_board[31].nKind == 21)
                    {
                        path.push_back(6);
                        path.push_back(30);
                        path.push_back(31);
                        g_mac[g_board[30].nCurPieceIdx].Move(path);
                        return true;
                    }
                    else if (g_board[32].nKind - g_board[31].nKind == 21)
                    {
                        path.push_back(10);
                        path.push_back(32);
                        path.push_back(31);
                        g_mac[g_board[32].nCurPieceIdx].Move(path);
                        return true;
                    }
                    break;
                case 24:
                    if (g_board[30].nKind - g_board[24].nKind == 21)
                    {
                        path.push_back(4);
                        path.push_back(30);
                        path.push_back(24);
                        g_mac[g_board[30].nCurPieceIdx].Move(path);
                        return true;
                    }
                    else if (g_board[18].nKind - g_board[25].nKind == 21)
                    {
                        path.push_back(8);
                        path.push_back(18);
                        path.push_back(24);
                        g_mac[g_board[18].nCurPieceIdx].Move(path);
                        return true;
                    }
                    break;

                case 8:
                    if (g_board[2].nKind - g_board[8].nKind == 21)
                    {
                        path.push_back(8);
                        path.push_back(2);
                        path.push_back(8);
                        g_mac[g_board[2].nCurPieceIdx].Move(path);
                        return true;
                    }
                    else if (g_board[3].nKind - g_board[8].nKind == 21)
                    {
                        path.push_back(9);
                        path.push_back(3);
                        path.push_back(8);
                        g_mac[g_board[3].nCurPieceIdx].Move(path);
                        return true;
                    }
                    else if (g_board[14].nKind - g_board[8].nKind == 21)
                    {
                        path.push_back(4);
                        path.push_back(14);
                        path.push_back(8);
                        g_mac[g_board[14].nCurPieceIdx].Move(path);
                        return true;
                    }
                    else if (g_board[15].nKind - g_board[8].nKind == 21)
                    {
                        path.push_back(11);
                        path.push_back(15);
                        path.push_back(8);
                        g_mac[g_board[15].nCurPieceIdx].Move(path);
                        return true;
                    }
                    break;
                case 9:
                    if (g_board[2].nKind - g_board[9].nKind == 21)
                    {
                        path.push_back(7);
                        path.push_back(2);
                        path.push_back(9);
                        g_mac[g_board[2].nCurPieceIdx].Move(path);
                        return true;
                    }
                    else if (g_board[3].nKind - g_board[9].nKind == 21)
                    {
                        path.push_back(8);
                        path.push_back(3);
                        path.push_back(9);
                        g_mac[g_board[3].nCurPieceIdx].Move(path);
                        return true;
                    }
                    else if (g_board[14].nKind - g_board[9].nKind == 21)
                    {
                        path.push_back(5);
                        path.push_back(14);
                        path.push_back(9);
                        g_mac[g_board[14].nCurPieceIdx].Move(path);
                        return true;
                    }
                    else if (g_board[15].nKind - g_board[9].nKind == 21)
                    {
                        path.push_back(4);
                        path.push_back(15);
                        path.push_back(9);
                        g_mac[g_board[15].nCurPieceIdx].Move(path);
                        return true;
                    }
                    break;
                case 16:
                    if (g_board[17].nKind - g_board[16].nKind == 21)
                    {
                        path.push_back(10);
                        path.push_back(17);
                        path.push_back(16);
                        g_mac[g_board[17].nCurPieceIdx].Move(path);
                        return true;
                    }
                    else if (g_board[23].nKind - g_board[16].nKind == 21)
                    {
                        path.push_back(11);
                        path.push_back(23);
                        path.push_back(16);
                        g_mac[g_board[23].nCurPieceIdx].Move(path);
                        return true;
                    }
                    else if (g_board[15].nKind - g_board[16].nKind == 21)
                    {
                        path.push_back(6);
                        path.push_back(15);
                        path.push_back(16);
                        g_mac[g_board[15].nCurPieceIdx].Move(path);
                        return true;
                    }
                    else if (g_board[21].nKind - g_board[16].nKind == 21)
                    {
                        path.push_back(5);
                        path.push_back(21);
                        path.push_back(16);
                        g_mac[g_board[21].nCurPieceIdx].Move(path);
                        return true;
                    }
                    break;
                case 22:
                    if (g_board[17].nKind - g_board[22].nKind == 21)
                    {
                        path.push_back(9);
                        path.push_back(17);
                        path.push_back(22);
                        g_mac[g_board[17].nCurPieceIdx].Move(path);
                        return true;
                    }
                    else if (g_board[23].nKind - g_board[22].nKind == 21)
                    {
                        path.push_back(10);
                        path.push_back(23);
                        path.push_back(22);
                        g_mac[g_board[23].nCurPieceIdx].Move(path);
                        return true;
                    }
                    else if (g_board[15].nKind - g_board[22].nKind == 21)
                    {
                        path.push_back(7);
                        path.push_back(15);
                        path.push_back(22);
                        g_mac[g_board[15].nCurPieceIdx].Move(path);
                        return true;
                    }
                    else if (g_board[21].nKind - g_board[22].nKind == 21)
                    {
                        path.push_back(6);
                        path.push_back(21);
                        path.push_back(22);
                        g_mac[g_board[21].nCurPieceIdx].Move(path);
                        return true;
                    }
                    break;
                case 27:
                    if (g_board[33].nKind - g_board[27].nKind == 21)
                    {
                        path.push_back(4);
                        path.push_back(33);
                        path.push_back(27);
                        g_mac[g_board[33].nCurPieceIdx].Move(path);
                        return true;
                    }
                    else if (g_board[32].nKind - g_board[27].nKind == 21)
                    {
                        path.push_back(5);
                        path.push_back(32);
                        path.push_back(27);
                        g_mac[g_board[32].nCurPieceIdx].Move(path);
                        return true;
                    }
                    else if (g_board[21].nKind - g_board[27].nKind == 21)
                    {
                        path.push_back(8);
                        path.push_back(21);
                        path.push_back(27);
                        g_mac[g_board[21].nCurPieceIdx].Move(path);
                        return true;
                    }
                    else if (g_board[20].nKind - g_board[27].nKind == 21)
                    {
                        path.push_back(7);
                        path.push_back(20);
                        path.push_back(27);
                        g_mac[g_board[20].nCurPieceIdx].Move(path);
                        return true;
                    }
                    break;
                case 26:
                    if (g_board[32].nKind - g_board[26].nKind == 21)
                    {
                        path.push_back(4);
                        path.push_back(32);
                        path.push_back(26);
                        g_mac[g_board[32].nCurPieceIdx].Move(path);
                        return true;
                    }
                    else if (g_board[33].nKind - g_board[26].nKind == 21)
                    {
                        path.push_back(11);
                        path.push_back(33);
                        path.push_back(26);
                        g_mac[g_board[33].nCurPieceIdx].Move(path);
                        return true;
                    }
                    else if (g_board[20].nKind - g_board[26].nKind == 21)
                    {
                        path.push_back(8);
                        path.push_back(20);
                        path.push_back(26);
                        g_mac[g_board[20].nCurPieceIdx].Move(path);
                        return true;
                    }
                    else if (g_board[21].nKind - g_board[26].nKind == 21)
                    {
                        path.push_back(9);
                        path.push_back(21);
                        path.push_back(26);
                        g_mac[g_board[21].nCurPieceIdx].Move(path);
                        return true;
                    }
                    break;
                case 13:
                    if (g_board[12].nKind - g_board[13].nKind == 21)
                    {
                        path.push_back(6);
                        path.push_back(12);
                        path.push_back(13);
                        g_mac[g_board[12].nCurPieceIdx].Move(path);
                        return true;
                    }
                    else if (g_board[18].nKind - g_board[13].nKind == 21)
                    {
                        path.push_back(5);
                        path.push_back(18);
                        path.push_back(13);
                        g_mac[g_board[18].nCurPieceIdx].Move(path);
                        return true;
                    }
                    else if (g_board[14].nKind - g_board[13].nKind == 21)
                    {
                        path.push_back(10);
                        path.push_back(14);
                        path.push_back(13);
                        g_mac[g_board[14].nCurPieceIdx].Move(path);
                        return true;
                    }
                    else if (g_board[20].nKind - g_board[13].nKind == 21)
                    {
                        path.push_back(11);
                        path.push_back(20);
                        path.push_back(13);
                        g_mac[g_board[20].nCurPieceIdx].Move(path);
                        return true;
                    }
                    break;
                case 19:
                    if (g_board[12].nKind - g_board[19].nKind == 21)
                    {
                        path.push_back(7);
                        path.push_back(12);
                        path.push_back(19);
                        g_mac[g_board[12].nCurPieceIdx].Move(path);
                        return true;
                    }
                    else if (g_board[18].nKind - g_board[19].nKind == 21)
                    {
                        path.push_back(6);
                        path.push_back(18);
                        path.push_back(19);
                        g_mac[g_board[18].nCurPieceIdx].Move(path);
                        return true;
                    }
                    else if (g_board[14].nKind - g_board[19].nKind == 21)
                    {
                        path.push_back(9);
                        path.push_back(14);
                        path.push_back(19);
                        g_mac[g_board[14].nCurPieceIdx].Move(path);
                        return true;
                    }
                    else if (g_board[20].nKind - g_board[19].nKind == 21)
                    {
                        path.push_back(10);
                        path.push_back(20);
                        path.push_back(19);
                        g_mac[g_board[20].nCurPieceIdx].Move(path);
                        return true;
                    }
                    break;
                default:break;
            }
        }
    }

    return false;
}

/*!@function DoubleOrbitAddPiece
*******************************************************************************
功能: 双轨添加棋子
******************************************************************************/
bool DoubleOrbitAddPiece()
{
    PATH path;
    int doubleOrbitPos_table[] = {8, 9, 13, 16, 19, 22, 26, 27};
    // 第一轮：不考虑4个内轨拐点交叉处
    for(int i=0; i<8; i++)
    {
        if (g_board[ doubleOrbitPos_table[i] ].nKind == NULL_PIECE)
        {
            switch(doubleOrbitPos_table[i])
            {
                case 8:
                    if (g_board[1].nKind - g_board[8].nKind == 21)
                    {
                        path.push_back(7);
                        path.push_back(1);
                        path.push_back(8);
                        g_mac[ g_board[1].nCurPieceIdx ].Move(path);
                        return true;
                    }
                    break;
                case 9:
                    if (g_board[4].nKind - g_board[9].nKind == 21)
                    {
                        path.push_back(9);
                        path.push_back(4);
                        path.push_back(9);
                        g_mac[ g_board[4].nCurPieceIdx ].Move(path);
                        return true;
                    }
                    break;
                case 13:
                    if (g_board[6].nKind - g_board[13].nKind == 21)
                    {
                        path.push_back(7);
                        path.push_back(6);
                        path.push_back(13);
                        g_mac[ g_board[6].nCurPieceIdx ].Move(path);
                        return true;
                    }
                    break;
                case 16:
                    if (g_board[11].nKind - g_board[16].nKind == 21)
                    {
                        path.push_back(9);
                        path.push_back(11);
                        path.push_back(16);
                        g_mac[ g_board[11].nCurPieceIdx ].Move(path);
                        return true;
                    }
                    break;
                case 19:
                    if (g_board[24].nKind - g_board[19].nKind == 21)
                    {
                        path.push_back(5);
                        path.push_back(24);
                        path.push_back(19);
                        g_mac[ g_board[24].nCurPieceIdx ].Move(path);
                        return true;
                    }
                    break;
                case 22:
                    if (g_board[29].nKind - g_board[22].nKind == 21)
                    {
                        path.push_back(11);
                        path.push_back(29);
                        path.push_back(22);
                        g_mac[ g_board[29].nCurPieceIdx ].Move(path);
                        return true;
                    }
                    break;
                case 26:
                    if (g_board[31].nKind - g_board[26].nKind == 21)
                    {
                        path.push_back(5);
                        path.push_back(31);
                        path.push_back(26);
                        g_mac[ g_board[31].nCurPieceIdx ].Move(path);
                        return true;
                    }
                    break;
                case 27:
                    if (g_board[34].nKind - g_board[27].nKind == 21)
                    {
                        path.push_back(11);
                        path.push_back(34);
                        path.push_back(27);
                        g_mac[ g_board[34].nCurPieceIdx ].Move(path);
                        return true;
                    }
                    break;
                default:break;
            }
        }
    }

    return false;
}

/*!@function EvalutePath
*******************************************************************************
功能: 给可吃子路径打分, 判断哪条吃子路径最优走子考虑
******************************************************************************/
void EvalutePath(const PATH cpath)
{
    int nValue = 0;
    // 所在位置的价值
    // 内轨2分，外轨1分，双轨3分，无轨0分
    // 内轨交点5分，外轨交点4分
    int value_table[] ={0, 2, 1, 1, 2, 0,
                        2, 5, 3, 3, 5, 2,
                        1, 3, 4, 4, 3, 1,
                        1, 3, 4, 4, 3, 1,
                        2, 5, 3, 3, 5, 2,
                        0, 2, 1, 1, 2, 0
                       };
    nValue = nValue + cpath.back() - cpath.at(1); // 吃子后位置变量的价值

    if (g_board[cpath.at(1)].chOrbit == INNER_ORBIT) // 从纯内轨出发
    {
        if (IsInnerOrbitOk() == true)
        {
            if (g_board[cpath.back()].chOrbit == INNER_ORBIT) // 到达纯内轨
                nValue += 10;
            else if (g_board[cpath.back()].chOrbit == OUTER_ORBIT)// 到达纯外轨
                nValue += 5;
            else // 到达双轨
                nValue += 8;
        }
        else // 如果从纯内轨出发，没有对方内轨棋子多，刚不吃子
        {
            nValue = -1000;
        }
    }
    else if (g_board[cpath.at(1)].chOrbit == DOUBLE_ORBIT) // 从双轨出发
    {
        if (IsInnerOrbitOk() == true)
        {
            if (g_board[cpath.back()].chOrbit == INNER_ORBIT) // 到达纯内轨
                nValue += 20;
            else if (g_board[cpath.back()].chOrbit == OUTER_ORBIT)// 到达纯外轨
                nValue += 5;
            else // 到达双轨
                nValue += 8;
        }
        else
        {
            nValue = -1000;
        }
    }
    else // 从纯外轨出发
    {
        if (g_board[cpath.back()].chOrbit == OUTER_ORBIT) // 到过纯外轨
            nValue += 10;
        else if (g_board[cpath.back()].chOrbit == DOUBLE_ORBIT) // 到达双轨
            nValue += 15;
        else // 到达内轨
            nValue += 20;
    }
    // 记入表中
    Node n;
    n.direct = cpath.at(0);
    n.nIdx = g_board[cpath.at(1)].nCurPieceIdx;
    g_mapValueAndPath.insert(pair<int, Node>(nValue, n));
}