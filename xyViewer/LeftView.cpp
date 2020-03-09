
// xyViewerView.cpp : CLeftView 클래스의 구현
//

#include "stdafx.h"
#include "xyViewer.h"

#include "xyViewerDoc.h"
#include "LeftView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLeftView

IMPLEMENT_DYNCREATE(CLeftView, CView)

BEGIN_MESSAGE_MAP(CLeftView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CLeftView::OnFilePrintPreview)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CLeftView 생성/소멸

CLeftView::CLeftView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CLeftView::~CLeftView()
{
}

BOOL CLeftView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CLeftView 그리기

void CLeftView::OnDraw(CDC* pDC)
{
	CxyViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.

	std::vector<PointF> points;
	std::vector<BYTE> types;
	pDoc->m_ptOriginList.GetPoints(points);
	pDoc->m_ptOriginList.GetTypes(types);

	CFont font;
	LOGFONT lf;
	::memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = m_pointInfoSize.cy;
	wcscpy(lf.lfFaceName, L"돋음체");
	font.CreateFontIndirect(&lf);

	
	CFont* oldFont = (CFont*)pDC->SelectObject(&font);

	for(unsigned int index = 0; index < points.size(); ++index)
	{
		BYTE pathType = types[index] & 0x07; // 0x07은 GDI+ PathPointTypePathTypeMask를 의미함
		PointF point = points[index];

		{
			// 해당 Point의 Text..
			WCHAR str[_MAX_PATH];
			swprintf(str, L"[%d/%d][type:%X]x = %.2f, y = %.2f", index, points.size() - 1, pathType, point.X, point.Y);

			for(unsigned int j = 0; j < pDoc->m_nCurrentIndexList.size(); ++j)
			{
				int currentIndex = pDoc->m_nCurrentIndexList[j];
				if(index == currentIndex) {
					WCHAR tmpStr[_MAX_PATH];
					swprintf(tmpStr, L"#[%d]#", j);
					wcscat(str, tmpStr);
					break;
				}

			}
			pDC->TextOut(m_pointInfoOffset.x, m_pointInfoOffset.y + index*m_pointInfoSize.cy, str, wcslen(str));
		}

		bool pathClose = (types[index] & 0x80) ? true : false; // 0x80은 GDI+ PathPointTypeCloseSubpath를 의미함

		// 끝점
		if (pathClose) {
			//start = PointF(0, 0);

			//char str[_MAX_PATH];
			//strcpy(str, "----------------------------------------");
			//pDC->TextOut(m_pointInfoOffset.x, m_pointInfoOffset.y + i*m_pointInfoSize.cy, str, strlen(str));
			pDC->MoveTo(m_pointInfoOffset.x,		m_pointInfoOffset.y + (index + 1)*m_pointInfoSize.cy - 2);
			pDC->LineTo(m_pointInfoOffset.x + 300,	m_pointInfoOffset.y + (index + 1)*m_pointInfoSize.cy - 2);
		}
	}

	pDC->SelectObject(oldFont);
}


// CLeftView 인쇄


void CLeftView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CLeftView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CLeftView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CLeftView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CLeftView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CLeftView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CLeftView 진단

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CView::AssertValid();
}

void CLeftView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CxyViewerDoc* CLeftView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CxyViewerDoc)));
	return (CxyViewerDoc*)m_pDocument;
}
#endif //_DEBUG


// CLeftView 메시지 처리기

void CLeftView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CxyViewerDoc* pDoc = GetDocument();

	// 인접한 Index 생성
	pDoc->m_nCurrentIndexList.clear();

	// Data 선택
	if(point.x >= m_pointInfoOffset.x && point.x <= m_pointInfoOffset.x + m_pointInfoSize.cx) {
		int index = (int)((double)(point.y - m_pointInfoOffset.y) / m_pointInfoSize.cy);
		if(index >= 0 && index < (int)pDoc->m_ptOriginList.GetCount()) {
			pDoc->m_nCurrentIndexList.push_back(index);
		}
	}

	InvalidateRect(NULL, TRUE);

	pDoc->ChangeData(this);

	CView::OnMouseMove(nFlags, point);
}

void CLeftView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CxyViewerDoc* pDoc = GetDocument();

	m_pointInfoOffset.x = 0;
	m_pointInfoOffset.y = 0;
	m_pointInfoSize.cx = 300;
	m_pointInfoSize.cy = 15;
}

BOOL CLeftView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	m_pointInfoOffset.y += zDelta;

	if(m_pointInfoOffset.y > 0)
		m_pointInfoOffset.y = 0;

	InvalidateRect(NULL, TRUE);

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CLeftView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

    CxyViewerDoc* pDoc = GetDocument();

    switch (nChar)
    {
    case VK_UP:
        m_pointInfoOffset.y += m_pointInfoSize.cy;
        break;
    case VK_DOWN:
        m_pointInfoOffset.y -= m_pointInfoSize.cy;
        break;
    case VK_PRIOR:
        m_pointInfoOffset.y += (m_pointInfoSize.cy * 40);
        break;
    case VK_NEXT:
        m_pointInfoOffset.y -= (m_pointInfoSize.cy * 40);
        break;
    case VK_HOME:
        m_pointInfoOffset.y = 0;
        break;
    case VK_END:
        m_pointInfoOffset.y = -((pDoc->m_ptOriginList.GetCount() - 1 - 40) * m_pointInfoSize.cy);
        break;
    }

	if(m_pointInfoOffset.y > 0)
		m_pointInfoOffset.y = 0;

	InvalidateRect(NULL, TRUE);

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
