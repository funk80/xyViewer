// PointEditorDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "xyViewer.h"
#include "PointEditorDlg.h"
#include "AutoControlMover/AutoControlMover.h"

// CPointEditorDlg 대화 상자입니다.

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


// CPointEditorDlg 메시지 처리기입니다.

void CPointEditorDlg::SetDocument(CxyViewerDoc* pDocument)
{
    m_pDocument = pDocument;
}

CxyViewerDoc* CPointEditorDlg::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	return m_pDocument;
}

void CPointEditorDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
    CDialog::OnSizing(fwSide, pRect);

    // TODO: 여기에 메시지 처리기 코드를 추가합니다.
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
    // TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
    // CDialog::OnInitDialog() 함수를 재지정 
    //하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
    // 이 알림 메시지를 보내지 않습니다.

    // TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
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

    // TODO:  여기에 추가 초기화 작업을 추가합니다.
    CEdit* edit = (CEdit*)GetDlgItem(IDC_EDIT1);
    edit->SetLimitText(0xFFFFFFFF);

    return TRUE;  // return TRUE unless you set the focus to a control
    // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CPointEditorDlg::OnOK()
{
    // TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

    //CDialog::OnOK();
}
