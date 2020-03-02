#include "StdAfx.h"
#include "MathUtils.h"

CMathUtils::CMathUtils(void)
{
}

CMathUtils::~CMathUtils(void)
{
}

int CMathUtils::Round(double value)
{
	int intValue = (int)value;

	double underValue = value - (double)intValue;
	if(underValue >= 0.5) {
		return intValue + 1;
	} else {
		return intValue;
	}
}

int CMathUtils::RoundUp(double value)
{
	int intValue = (int)value;

	double underValue = value - (double)intValue;
	if(underValue != 0) {
		return intValue + 1;
	} else {
		return intValue;
	}
}

int CMathUtils::RoundDown(double value)
{
	return (int)value;
}

int CMathUtils::GetPower(int value)
{
	return CMathUtils::RoundDown(std::log10((double)value));
}

int CMathUtils::IndexAlignment(int value)
{
	int power = CMathUtils::GetPower(value);
	int powerValue = (int)std::pow((double)10, power);
	int maxValue = CMathUtils::RoundUp((double)value / powerValue) * powerValue;
	return CMathUtils::RoundUp((double)value / powerValue) * powerValue;
}

SIZE CMathUtils::IndexAlignmentSize(SIZE size)
{
	SIZE newSize = {IndexAlignment(size.cx), IndexAlignment(size.cy)};
	return newSize;
}