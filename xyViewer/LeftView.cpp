
// xyViewerView.cpp : CLeftView Ŭ������ ����
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
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CLeftView::OnFilePrintPreview)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CLeftView ����/�Ҹ�

CLeftView::CLeftView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CLeftView::~CLeftView()
{
}

BOOL CLeftView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CLeftView �׸���

void CLeftView::OnDraw(CDC* pDC)
{
	CxyViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.

	std::vector<PointF> points;
	std::vector<BYTE> types;
	pDoc->m_ptOriginList.GetPoints(points);
	pDoc->m_ptOriginList.GetTypes(types);

	CFont font;
	LOGFONT lf;
	::memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = m_pointInfoSize.cy;
	wcscpy(lf.lfFaceName, L"����ü");
	font.CreateFontIndirect(&lf);

	
	CFont* oldFont = (CFont*)pDC->SelectObject(&font);

	for(unsigned int index = 0; index < points.size(); ++index)
	{
		BYTE pathType = types[index] & 0x07; // 0x07�� GDI+ PathPointTypePathTypeMask�� �ǹ���
		PointF point = points[index];

		{
			// �ش� Point�� Text..
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

		bool pathClose = (types[index] & 0x80) ? true : false; // 0x80�� GDI+ PathPointTypeCloseSubpath�� �ǹ���

		// ����
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


// CLeftView �μ�


void CLeftView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CLeftView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CLeftView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CLeftView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
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


// CLeftView ����

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CView::AssertValid();
}

void CLeftView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CxyViewerDoc* CLeftView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CxyViewerDoc)));
	return (CxyViewerDoc*)m_pDocument;
}
#endif //_DEBUG


// CLeftView �޽��� ó����

void CLeftView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CxyViewerDoc* pDoc = GetDocument();

	// ������ Index ����
	pDoc->m_nCurrentIndexList.clear();

	// Data ����
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

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	CxyViewerDoc* pDoc = GetDocument();

	m_pointInfoOffset.x = 0;
	m_pointInfoOffset.y = 0;
	m_pointInfoSize.cx = 300;
	m_pointInfoSize.cy = 15;
}

BOOL CLeftView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	m_pointInfoOffset.y += zDelta;

	if(m_pointInfoOffset.y > 0)
		m_pointInfoOffset.y = 0;

	InvalidateRect(NULL, TRUE);

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CLeftView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

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
