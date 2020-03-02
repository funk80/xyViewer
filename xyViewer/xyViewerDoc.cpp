
// xyViewerDoc.cpp : CxyViewerDoc Ŭ������ ����
//

#include "stdafx.h"
#include "xyViewer.h"

#include "xyViewerDoc.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CxyViewerDoc

IMPLEMENT_DYNCREATE(CxyViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CxyViewerDoc, CDocument)
END_MESSAGE_MAP()


// CxyViewerDoc ����/�Ҹ�

CxyViewerDoc::CxyViewerDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.
}

CxyViewerDoc::~CxyViewerDoc()
{
}

BOOL CxyViewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.
	m_ptOriginList.Clear();
	m_nCurrentIndexList.clear();

    std::vector<PointF> points;
    m_ptOriginList.GetPoints(points);
    CPointUtil::InitPathInfo(points, m_Offset, m_MinPt, m_MaxPt);

	return TRUE;
}

BOOL CxyViewerDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
	m_ptOriginList.Clear();

    char pathName[_MAX_PATH];
    WideCharToMultiByte(CP_ACP, 0, lpszPathName, -1, pathName, _MAX_PATH, NULL, NULL);

	FILE* fp = fopen(pathName, "rt");

	char buf[MAX_PATH * 10];

	while(fgets(buf, MAX_PATH * 10, fp) != NULL)
	{
		m_ptOriginList.AddLine(buf);
	}
	m_ptOriginList.SyncPointType();
	fclose(fp);
//+		[18]	{x=87 y=24}	tagPOINT

    std::vector<PointF> points;
    m_ptOriginList.GetPoints(points);
    CPointUtil::InitPathInfo(points, m_Offset, m_MinPt, m_MaxPt);
	return TRUE;
}




// CxyViewerDoc serialization

void CxyViewerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
		
	}
}

BOOL CxyViewerDoc::LoadData(char* buf)
{
	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
	m_ptOriginList.Clear();

    while(*buf != '\0')
    {
        char* cur = strstr(buf, "\r\n");
        int count = 0;
        if (cur) {
            count = cur - buf;
        } else {
            count = strlen(buf);
        }

        char tmp[MAX_PATH * 10];
        strncpy(tmp, buf, count);
        tmp[count] = '\0';

        // ó��
        m_ptOriginList.AddLine(tmp);

        buf = buf + count + 2;
    }

	m_ptOriginList.SyncPointType();
//+		[18]	{x=87 y=24}	tagPOINT

    std::vector<PointF> points;
    m_ptOriginList.GetPoints(points);
    CPointUtil::InitPathInfo(points, m_Offset, m_MinPt, m_MaxPt);

    CxyViewerView* mainView = GetMainView();
    if (mainView) {
        mainView->OnInitialUpdate();
    }
    CLeftView* leftView = GetLeftView();
    if (leftView) {
        leftView->OnInitialUpdate();
    }
	return TRUE;
}

void CxyViewerDoc::ChangeData(CView* myView)
{
	CLeftView* leftView = GetLeftView();
	CxyViewerView* mainView = GetMainView();

	if(leftView != myView) {
		leftView->InvalidateRect(NULL, TRUE);
	}

	if(mainView != myView) {
		mainView->InvalidateRect(NULL, FALSE);
	}
}

void CxyViewerDoc::SelectPathPoint()
{
    if (m_nCurrentIndexList.size() == 0)
        return;

    CLeftView* leftView = GetLeftView();
	CxyViewerView* mainView = GetMainView();

    if (m_nCurrentIndexList[0] * leftView->m_pointInfoSize.cy < 200) {
        leftView->m_pointInfoOffset.y = 0;
    } else {
        leftView->m_pointInfoOffset.y = -(m_nCurrentIndexList[0] * leftView->m_pointInfoSize.cy - 200);
    }
}


// CxyViewerDoc ����

#ifdef _DEBUG
void CxyViewerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CxyViewerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CxyViewerDoc ���
