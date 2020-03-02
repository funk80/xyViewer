#pragma once

enum PointType
{
	None,
};

using namespace Gdiplus;

class CPointUtil
{
public:
	CPointUtil(void);
	~CPointUtil(void);
public:
    // Doc���� ����
	static std::pair<bool, PointF> GetPointFromLine(const char* buf);
	static BYTE GetTypeFromLine(const char* buf);
	static void CPointUtil::InitPathInfo(const std::vector<PointF>& ptSourceList, PointF& offset, PointF& minPt, PointF& maxPt);

    // View���� ����
	static RECT GetBound(const std::vector<POINT>& ptSourceList);
    static float CPointUtil::CalculateScale(RECT rectClient, const PointF minPt, const PointF maxPt);

};