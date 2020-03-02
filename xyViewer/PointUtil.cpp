#include "StdAfx.h"
#include "PointUtil.h"

#include "MathUtils.h"

CPointUtil::CPointUtil(void)
{
}

CPointUtil::~CPointUtil(void)
{
}

std::pair<bool, PointF> CPointUtil::GetPointFromLine(const char* buf)
{
	bool xFlag = false;
	bool yFlag = false;
	bool minusFlag = false;
	bool numFlag = false;
    bool decimalFlag = false;
    bool validPointX = false;
    bool validPointY = false;

	PointF pt = {0.F, 0.F};
	float number = 0.F;
    float decimal = 0.1F;

	//if(strstr(buf, "#close#")) {
	//	POINT tmpPt = {INT_MIN, INT_MIN};
	//	ptList.push_back(tmpPt);
	//	return;
	//}

	for(unsigned int i = 0; i < strlen(buf) + 1; ++i)
	{
        char ch = i < strlen(buf) ? buf[i] : ' ';

		if(ch == 'x' || ch == 'X') {
			xFlag = true;
			minusFlag = false;
		} else if(ch == 'y' || ch == 'Y') {
			yFlag = true;
			minusFlag = false;
		} else if(ch == '-') {
			minusFlag = true;
        } else if (ch >= '0' && ch <= '9') {
            if (xFlag || yFlag) {
                if (!decimalFlag) {
                    number = number * 10 + ch - '0';
                } else {
                    number = number + (ch - '0') * decimal;
                    decimal = decimal / 10;
                }
                numFlag = true;
            }
        } else if (ch == '.') {
            if (xFlag || yFlag) {
                decimalFlag = true;
            }
        } else {
			if(numFlag == true) {
				if(xFlag == true) {
					pt.X = number * (minusFlag ? -1 : 1);
                    validPointX = true;
				} else if(yFlag == true) {
					pt.Y = number * (minusFlag ? -1 : 1);
                    validPointY = true;
				}
				xFlag = false;
				yFlag = false;
				minusFlag = false;
				numFlag = false;
                decimalFlag = false;
				number = 0;
                decimal = 0.1F;

			}
		}
	}

    bool isValid = validPointX & validPointY;

    return std::make_pair(isValid, pt);
}

BYTE CPointUtil::GetTypeFromLine(const char* buf)
{
	bool bracketStart = false;
	bool bracketEnd = false;
	bool numFlag = false;

	int number = 0;

	for(unsigned int i = 0; i < strlen(buf); ++i)
	{
		if(buf[i] == '[') {
			bracketStart = true;
		} else if(buf[i] == ']') {
			bracketEnd = true;
		} else if(buf[i] >= '0' && buf[i] <= '9') {
			if(bracketStart && bracketEnd) {
				number = number * 10 + buf[i] - '0';
				numFlag = true;
			}
		} else {
			if(numFlag == true) {
				bracketStart = false;
				bracketEnd = false;
				numFlag = false;
				return number;
			}
		}
	}

	return 0xFF;
}

void CPointUtil::InitPathInfo(const std::vector<PointF>& ptSourceList, PointF& offset, PointF& minPt, PointF& maxPt)
{
    if (ptSourceList.size() > 0) {
        minPt = ptSourceList.at(0);
        maxPt = ptSourceList.at(0);
    } else {
        minPt = PointF(0, 0);
        maxPt = PointF(0, 0);
    }

	for(unsigned int i = 0; i < ptSourceList.size(); ++i)
	{
		PointF pt = ptSourceList.at(i);
		if(pt.X < minPt.X)
			minPt.X = pt.X;
		if(pt.Y < minPt.Y)
			minPt.Y = pt.Y;

		if(pt.X > maxPt.X)
			maxPt.X = pt.X;
		if(pt.Y > maxPt.Y)
			maxPt.Y = pt.Y;
	}

	offset.X = -minPt.X;
	offset.Y = -minPt.Y;
}

RECT CPointUtil::GetBound(const std::vector<POINT>& ptSourceList)
{
	POINT offset = {0, 0};
	double scale = 1;

	POINT minPt = {LONG_MAX, LONG_MAX};
	POINT maxPt = {LONG_MIN, LONG_MIN};
	for(unsigned int i = 0; i < ptSourceList.size(); ++i)
	{
		POINT pt = ptSourceList.at(i);
		if(pt.x == INT_MIN && pt.y == INT_MIN) {
			continue;
		}
		if(pt.x < minPt.x)
			minPt.x = pt.x;
		if(pt.y < minPt.y)
			minPt.y = pt.y;

		if(pt.x > maxPt.x)
			maxPt.x = pt.x;
		if(pt.y > maxPt.y)
			maxPt.y = pt.y;
	}

	RECT rect = {0, 0, 0, 0};
	rect.left = minPt.x;
	rect.top = minPt.y;
	rect.right = maxPt.x;
	rect.bottom = maxPt.y;

	return rect;
}

float CPointUtil::CalculateScale(RECT rectClient, const PointF minPt, const PointF maxPt)
{
	float scale = 1;

    SizeF sourceSize = SizeF(maxPt.X - minPt.X, maxPt.Y - minPt.Y);
	SIZE clientSize = {rectClient.right - rectClient.left, rectClient.bottom - rectClient.top};

    float widthRate = (float)clientSize.cx / sourceSize.Width;
    float heightRate = (float)clientSize.cy / sourceSize.Height;

	if(widthRate < heightRate) { // width기준으로 맞춤 
		scale = widthRate;
	} else { // height기준으로 맞춤
		scale = heightRate;
	}

	return scale;
}