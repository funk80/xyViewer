#pragma once

#include "xyViewerDoc.h"
// CPointEditorDlg 대화 상자입니다.

class CPointEditorDlg : public CDialog
{
	DECLARE_DYNAMIC(CPointEditorDlg)

public:
	CPointEditorDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPointEditorDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    CxyViewerDoc* GetDocument() const;
    void SetDocument(CxyViewerDoc* pDocument);

public:
    afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
    afx_msg void OnEnChangeEdit1();

private:
    CxyViewerDoc* m_pDocument;
public:
    virtual BOOL OnInitDialog();
    virtual void OnOK();
};
