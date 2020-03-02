
// xyViewer.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "xyViewer.h"
#include "MainFrm.h"

#include "xyViewerDoc.h"
#include "xyViewerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// testcode
class CSDIDocTemplate : public CSingleDocTemplate
{
	DECLARE_DYNAMIC(CSDIDocTemplate)
// Constructors
public:
  CSDIDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass,
    CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass,
      int nSplitterID = -1, int nRow = 0, int nCol = 0);

// Attributes
public:
  int m_nSplitterID;	// -1, no splitter else splitter ID (0, 1,...)
  int m_nRow;			// if splitter, row number (0, 1,...)
  int m_nCol;			// if splitter, col number (0, 1,...)

  CView* m_pView;		// view pointer

  // these overrides to retreive the view class and resource ID
  CRuntimeClass* GetViewClass()
    { return m_pViewClass; }
  UINT GetResourceID()
	{ return m_nIDResource; }
};

IMPLEMENT_DYNAMIC(CSDIDocTemplate, CSingleDocTemplate)

CSDIDocTemplate::CSDIDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass,
    CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass,
		int nSplitterID, int nRow, int nCol) :
  CSingleDocTemplate(nIDResource, pDocClass, pFrameClass, pViewClass)
{
	m_nSplitterID = nSplitterID;
	m_nRow = nRow;
	m_nCol = nCol;
	m_pView = NULL;
}
// testcode

// CxyViewerApp

BEGIN_MESSAGE_MAP(CxyViewerApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CxyViewerApp::OnAppAbout)
	// ǥ�� ������ ���ʷ� �ϴ� ���� ����Դϴ�.
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// ǥ�� �μ� ���� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CxyViewerApp ����

CxyViewerApp::CxyViewerApp()
{

	m_bHiColorIcons = TRUE;

	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}

// ������ CxyViewerApp ��ü�Դϴ�.

CxyViewerApp theApp;


// CxyViewerApp �ʱ�ȭ

BOOL CxyViewerApp::InitInstance()
{
	// ���� ���α׷� �Ŵ��佺Ʈ�� ComCtl32.dll ���� 6 �̻��� ����Ͽ� ���־� ��Ÿ����
	// ����ϵ��� �����ϴ� ���, Windows XP �󿡼� �ݵ�� InitCommonControlsEx()�� �ʿ��մϴ�. 
	// InitCommonControlsEx()�� ������� ������ â�� ���� �� �����ϴ�.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ���� ���α׷����� ����� ��� ���� ��Ʈ�� Ŭ������ �����ϵ���
	// �� �׸��� �����Ͻʽÿ�.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	ULONG_PTR gdiplusToken;
	GdiplusStartupInput gsi;
	if (GdiplusStartup(&gdiplusToken, &gsi, NULL) != Ok)
	{
		return 0;
	}

	// OLE ���̺귯���� �ʱ�ȭ�մϴ�.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ
	// ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	SetRegistryKey(_T("���� ���� ���α׷� �����翡�� ������ ���� ���α׷�"));
	LoadStdProfileSettings(4);  // MRU�� �����Ͽ� ǥ�� INI ���� �ɼ��� �ε��մϴ�.

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// ���� ���α׷��� ���� ���ø��� ����մϴ�. ���� ���ø���
	//  ����, ������ â �� �� ������ ���� ������ �մϴ�.
	CSingleDocTemplate* pDocTemplate;

	//pDocTemplate = new CSDIDocTemplate(
	//	IDR_MAINFRAME,
	//	RUNTIME_CLASS(CxyViewerDoc),
	//	RUNTIME_CLASS(CMainFrame),       // �� SDI ������ â�Դϴ�.
	//	RUNTIME_CLASS(CxyViewerView), 1, 0, 0);
	//if (!pDocTemplate)
	//	return FALSE;
	//AddDocTemplate(pDocTemplate);

	//pDocTemplate = new CSDIDocTemplate(
	//	IDR_MAINFRAME,
	//	RUNTIME_CLASS(CxyViewerDoc),
	//	RUNTIME_CLASS(CMainFrame),       // �� SDI ������ â�Դϴ�.
	//	RUNTIME_CLASS(CxyViewerView), 1, 1, 0);
	//if (!pDocTemplate)
	//	return FALSE;
	//AddDocTemplate(pDocTemplate);

	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CxyViewerDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ������ â�Դϴ�.
		RUNTIME_CLASS(CxyViewerView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);



	// ǥ�� �� ���, DDE, ���� ���⿡ ���� ������� ���� �м��մϴ�.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// ����ٿ� ������ ����� ����ġ�մϴ�.
	// ���� ���α׷��� /RegServer, /Register, /Unregserver �Ǵ� /Unregister�� ���۵� ��� FALSE�� ��ȯ�մϴ�.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// â �ϳ��� �ʱ�ȭ�Ǿ����Ƿ� �̸� ǥ���ϰ� ������Ʈ�մϴ�.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// ���̻簡 ���� ��쿡�� DragAcceptFiles�� ȣ���մϴ�.
	//  SDI ���� ���α׷������� ProcessShellCommand �Ŀ� �̷��� ȣ���� �߻��ؾ� �մϴ�.
	return TRUE;
}



// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// ��ȭ ���ڸ� �����ϱ� ���� ���� ���α׷� ����Դϴ�.
void CxyViewerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CxyViewerApp ����� ���� �ε�/���� �޼���

void CxyViewerApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CxyViewerApp::LoadCustomState()
{
}

void CxyViewerApp::SaveCustomState()
{
}

// CxyViewerApp �޽��� ó����



