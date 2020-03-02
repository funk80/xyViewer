// PointEditorDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "xyViewer.h"
#include "PointEditorDlg.h"
#include "AutoControlMover/AutoControlMover.h"

// CPointEditorDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CPointEditorDlg, CDialog)

CPointEditorDlg::CPointEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPointEditorDlg::IDD, pParent)
{

}

CPointEditorDlg::~CPointEditorDlg()
{
}

void CPointEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPointEditorDlg, CDialog)
    ON_WM_SIZING()
    ON_EN_CHANGE(IDC_EDIT1, &CPointEditorDlg::OnEnChangeEdit1)
END_MESSAGE_MAP()


// CPointEditorDlg �޽��� ó�����Դϴ�.

void CPointEditorDlg::SetDocument(CxyViewerDoc* pDocument)
{
    m_pDocument = pDocument;
}

CxyViewerDoc* CPointEditorDlg::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	return m_pDocument;
}

void CPointEditorDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
    CDialog::OnSizing(fwSide, pRect);

    // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
    //if (pRect->right - pRect->left < 800) {
    //    pRect->right = pRect->left + 800;
    //}
    //if (pRect->bottom - pRect->top < 400) {
    //    pRect->bottom = pRect->top + 400;
    //}

    ControlMovingStyle cmsList[] = {
        { IDC_EDIT1,		MT_Strech,	MT_Strech },
    };

    CAutoControlMover::AutoMove(this, *pRect, cmsList, sizeof(cmsList) / sizeof(ControlMovingStyle));
}


void CPointEditorDlg::OnEnChangeEdit1()
{
    // TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
    // CDialog::OnInitDialog() �Լ��� ������ 
    //�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
    // �� �˸� �޽����� ������ �ʽ��ϴ�.

    // TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
    CEdit* edit = (CEdit*)GetDlgItem(IDC_EDIT1);
    CString string;
    edit->GetWindowText(string);

    CxyViewerDoc* pDocument = GetDocument();
    if (pDocument) {
        int bufCount = WideCharToMultiByte(CP_ACP, 0, string.GetBuffer(0), -1, NULL, 0, NULL, NULL);
        char* buf = new char[bufCount + 1];
        WideCharToMultiByte(CP_ACP, 0, string.GetBuffer(0), -1, buf, bufCount, NULL, NULL);
        buf[bufCount] = '\0';

        pDocument->LoadData(buf);
        delete[] buf;
    }
}


BOOL CPointEditorDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
    CEdit* edit = (CEdit*)GetDlgItem(IDC_EDIT1);
    edit->SetLimitText(0xFFFFFFFF);

    return TRUE;  // return TRUE unless you set the focus to a control
    // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CPointEditorDlg::OnOK()
{
    // TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

    //CDialog::OnOK();
}
