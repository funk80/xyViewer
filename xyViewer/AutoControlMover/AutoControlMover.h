#pragma once

enum MovingType
{
    MT_None,
    MT_Move,
    MT_Center,
    MT_Strech
};

struct ControlMovingStyle
{
    int controlID;
    MovingType horizeMovingType;
    MovingType veticalMovingType;
};

class CAutoControlMover
{
public:
    CAutoControlMover() {}
    
public:
    static void AutoMove(CWnd* parent, RECT changeWindowRect, ControlMovingStyle* cmsList, int cmsCount);

};