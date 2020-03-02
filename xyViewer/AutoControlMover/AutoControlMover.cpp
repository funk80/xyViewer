#include "stdafx.h"
#include "AutoControlMover.h"

void CAutoControlMover::AutoMove(CWnd* parent, RECT changeWindowRect, ControlMovingStyle* cmsList, int cmsCount)
{
    RECT preWindowRect;
    parent->GetWindowRect(&preWindowRect);

    int preWindowWidth = preWindowRect.right - preWindowRect.left;
    int preWindowHeight = preWindowRect.bottom - preWindowRect.top;

    int changeWidnowWidth = changeWindowRect.right - changeWindowRect.left;
    int changeWidnowHeight = changeWindowRect.bottom - changeWindowRect.top;

    int changeWidth = changeWidnowWidth - preWindowWidth;
    int changeHeight = changeWidnowHeight - preWindowHeight;

    for (int i = 0; i < cmsCount; ++i)
    {
        ControlMovingStyle& cms = cmsList[i];
        CWnd* pWnd = parent->GetDlgItem(cms.controlID);

        RECT controlRect;
        pWnd->GetWindowRect(&controlRect);
        int controlWidth = controlRect.right - controlRect.left;
        int controlHeight = controlRect.bottom - controlRect.top;
        switch(cms.horizeMovingType)
        {
        case MT_None:
            break;
        case MT_Move:
            {
                controlRect.left += changeWidth;
                controlRect.right += changeWidth;
            }
            break;
        case MT_Center:
            {
                controlRect.left += changeWidth / 2;
                controlRect.right = controlRect.left + controlWidth;
            }
            break;
        case MT_Strech:
            {
                controlRect.right += changeWidth;
            }
            break;
        }

        switch(cms.veticalMovingType)
        {
        case MT_None:
            break;
        case MT_Move:
            {
                controlRect.top += changeHeight;
                controlRect.bottom += changeHeight;
            }
            break;
        case MT_Center:
            {
                controlRect.top += changeHeight / 2;
                controlRect.bottom = controlRect.top + controlHeight;
            }
            break;
        case MT_Strech:
            {
                controlRect.bottom += changeHeight;
            }
            break;
        }

        parent->ScreenToClient(&controlRect);

        pWnd->MoveWindow(&controlRect);
    }
}
