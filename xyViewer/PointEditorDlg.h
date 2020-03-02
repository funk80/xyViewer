#pragma once

#include "xyViewerDoc.h"
// CPointEditorDlg ��ȭ �����Դϴ�.

class CPointEditorDlg : public CDialog
{
	DECLARE_DYNAMIC(CPointEditorDlg)

public:
	CPointEditorDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPointEditorDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
