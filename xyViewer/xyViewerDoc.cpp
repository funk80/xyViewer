
// xyViewerDoc.cpp : CxyViewerDoc 클래스의 구현
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


// CxyViewerDoc 생성/소멸

CxyViewerDoc::CxyViewerDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
}

CxyViewerDoc::~CxyViewerDoc()
{
}

BOOL CxyViewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.
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

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
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
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
		
	}
}

BOOL CxyViewerDoc::LoadData(char* buf)
{
	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
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

        // 처리
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


// CxyViewerDoc 진단

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


// CxyViewerDoc 명령
