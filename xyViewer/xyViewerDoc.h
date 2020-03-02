
// xyViewerDoc.h : CxyViewerDoc 클래스의 인터페이스
//


#pragma once

#include "MathUtils.h"
#include "RenderPath.h"

class CxyViewerDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CxyViewerDoc();
	DECLARE_DYNCREATE(CxyViewerDoc)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void Serialize(CArchive& ar);

public:
    BOOL LoadData(char* buf);
	void ChangeData(CView* myView);
    void SelectPathPoint();
// 구현입니다.
public:
	virtual ~CxyViewerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
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


