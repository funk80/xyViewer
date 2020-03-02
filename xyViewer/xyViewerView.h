
// xyViewerView.h : CxyViewerView Ŭ������ �������̽�
//


#pragma once

#include "PointEditorDlg.h"

class CxyViewerView : public CView
{
protected: // serialization������ ��������ϴ�.
	CxyViewerView();
	DECLARE_DYNCREATE(CxyViewerView)

// Ư���Դϴ�.
public:
	CxyViewerDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
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

// ������ �޽��� �� �Լ�
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

#ifndef _DEBUG  // xyViewerView.cpp�� ����� ����
inline CxyViewerDoc* CxyViewerView::GetDocument() const
   { return reinterpret_cast<CxyViewerDoc*>(m_pDocument); }
#endif

