
// xyViewerView.h : CxyViewerView 클래스의 인터페이스
//


#pragma once

#include "PointEditorDlg.h"

class CxyViewerView : public CView
{
protected: // serialization에서만 만들어집니다.
	CxyViewerView();
	DECLARE_DYNCREATE(CxyViewerView)

// 특성입니다.
public:
	CxyViewerDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CxyViewerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	//POINT GetTransformPoint(POINT pt) const;
	PointF GetTransformPoint(PointF pt) const;

public:
	CPointEditorDlg m_dlg;

	POINT m_offset;
	float m_scale;

	POINT m_prevMousePt;

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnViewZoomin();
	afx_msg void OnViewZoomout();
	virtual void OnInitialUpdate();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // xyViewerView.cpp의 디버그 버전
inline CxyViewerDoc* CxyViewerView::GetDocument() const
   { return reinterpret_cast<CxyViewerDoc*>(m_pDocument); }
#endif

