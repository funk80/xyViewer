#pragma once

class RenderPath
{
public:
	RenderPath();
public:
	void AddLine(const char* buf);
	void SyncPointType();
	void Clear();
	unsigned int GetCount();

	void GetPoints(std::vector<PointF>& points) const;

	// 각 Point의 type을 읽는다.
	// 0 - start, 1 - Line, 3 - Bezier, etc - Gdi+ 내부적으로 사용하는 점. 도형엔진에서는 무의미하며 검색되어서는 안된다.
	// 0x83이 리턴될 수도 있음
	void GetTypes(std::vector<BYTE>& types) const;

	RectF GetBound();
private:
	// Read Data
	std::vector<PointF> m_Points;
	std::vector<BYTE> m_Types;

	bool m_TypeFlag;

};
