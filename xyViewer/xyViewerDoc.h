
// xyViewerDoc.h : CxyViewerDoc Ŭ������ �������̽�
//


#pragma once

#include "MathUtils.h"
#include "RenderPath.h"

class CxyViewerDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CxyViewerDoc();
	DECLARE_DYNCREATE(CxyViewerDoc)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void Serialize(CArchive& ar);

public:
    BOOL LoadData(char* buf);
	void ChangeData(CView* myView);
    void SelectPathPoint();
// �����Դϴ�.
public:
	virtual ~CxyViewerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()

public:


public:
	// Read Data
	//std::vector<POINT> m_ptOriginList;
    PointF m_Offset;
	PointF m_MinPt;
	PointF m_MaxPt;
	RenderPath m_ptOriginList;

	// Current Data
	std::vector<int> m_nCurrentIndexList;
};


