#pragma once

class CMathUtils
{
public:
	CMathUtils(void);
	~CMathUtils(void);
public:
	static int Round(double value);
	static int RoundUp(double value);
	static int RoundDown(double value);
	static int GetPower(int value);

	static int IndexAlignment(int value);
	static SIZE IndexAlignmentSize(SIZE size);
};
