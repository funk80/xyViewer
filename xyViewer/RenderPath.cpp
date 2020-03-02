#include "StdAfx.h"
#include "RenderPath.h"

RenderPath::RenderPath()
{
	m_TypeFlag = false;
}

void RenderPath::AddLine(const char* buf)
{
	if(strstr(buf, "#type#")) {
		m_TypeFlag = true;
		return;
	}

	if(!m_TypeFlag) { // Pointó��
        std::pair<bool, PointF> value = CPointUtil::GetPointFromLine(buf);
        if (value.first) {
            PointF point = value.second;
            m_Points.push_back(point);
        }
	} else { // Typeó��
		if(m_Points.size() > m_Types.size()) {
			BYTE type = CPointUtil::GetTypeFromLine(buf);
			if(type != 0xFF) {
				m_Types.push_back(type);
			}
		}
	}
}

void RenderPath::SyncPointType()
{
	while(m_Points.size() > m_Types.size())
	{
		m_Types.push_back(1);
	}
}

void RenderPath::Clear()
{
	m_Points.clear();
	m_Types.clear();

	m_TypeFlag = false;
}

unsigned int RenderPath::GetCount()
{
	return m_Points.size();
}

void RenderPath::GetPoints(std::vector<PointF>& points) const
{
	for(unsigned int i = 0; i < m_Points.size(); ++i)
	{
		points.push_back(m_Points[i]);
	}
}

// �� Point�� type�� �д´�.
// 0 - start, 1 - Line, 3 - Bezier, etc - Gdi+ ���������� ����ϴ� ��. �������������� ���ǹ��ϸ� �˻��Ǿ�� �ȵȴ�.
// 0x83�� ���ϵ� ���� ����
void RenderPath::GetTypes(std::vector<BYTE>& types) const
{
	for(unsigned int i = 0; i < m_Types.size(); ++i)
	{
		types.push_back(m_Types[i]);
	}
}

RectF RenderPath::GetBound()
{
    PointF minPt;
    PointF maxPt;

    if (m_Points.size() > 0) {
        minPt = m_Points.at(0);
        maxPt = m_Points.at(0);
    } else {
        minPt = PointF(0, 0);
        maxPt = PointF(0, 0);
    }

	for(unsigned int i = 0; i < m_Points.size(); ++i)
	{
		PointF pt = m_Points.at(i);
		if(pt.X < minPt.X)
			minPt.X = pt.X;
		if(pt.Y < minPt.Y)
			minPt.Y = pt.Y;

		if(pt.X > maxPt.X)
			maxPt.X = pt.X;
		if(pt.Y > maxPt.Y)
			maxPt.Y = pt.Y;
	}

    RectF rect = RectF(minPt.X, minPt.Y, maxPt.X - minPt.X, maxPt.Y - minPt.Y);

	return rect;
}
