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

	// �� Point�� type�� �д´�.
	// 0 - start, 1 - Line, 3 - Bezier, etc - Gdi+ ���������� ����ϴ� ��. �������������� ���ǹ��ϸ� �˻��Ǿ�� �ȵȴ�.
	// 0x83�� ���ϵ� ���� ����
	void GetTypes(std::vector<BYTE>& types) const;

	RectF GetBound();
private:
	// Read Data
	std::vector<PointF> m_Points;
	std::vector<BYTE> m_Types;

	bool m_TypeFlag;

};
