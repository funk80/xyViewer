
// xyViewerView.cpp : CxyViewerView Ŭ������ ����
//

#include "stdafx.h"
#include "xyViewer.h"

#include "xyViewerDoc.h"
#include "xyViewerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CxyViewerView

IMPLEMENT_DYNCREATE(CxyViewerView, CView)

BEGIN_MESSAGE_MAP(CxyViewerView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CxyViewerView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_VIEW_ZOOMIN, &CxyViewerView::OnViewZoomin)
	ON_COMMAND(ID_VIEW_ZOOMOUT, &CxyViewerView::OnViewZoomout)
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CxyViewerView ����/�Ҹ�

CxyViewerView::CxyViewerView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CxyViewerView::~CxyViewerView()
{
}

BOOL CxyViewerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CxyViewerView �׸���

void CxyViewerView::OnDraw(CDC* pDC)
{
	CxyViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.

    RECT desktopRect;
    ::GetClientRect(::GetDesktopWindow(), &desktopRect);

    SIZE dcSize = { (desktopRect.right - desktopRect.left) * 2, (desktopRect.bottom - desktopRect.top) * 2 };

	CDC memGdiplusDC;
	CBitmap hCairoBitmap;
	memGdiplusDC.CreateCompatibleDC(pDC);
	hCairoBitmap.CreateCompatibleBitmap(pDC, dcSize.cx, dcSize.cy);
	CBitmap* hCairoOldBitmap = memGdiplusDC.SelectObject(&hCairoBitmap);
	memGdiplusDC.Rectangle(-1, -1, dcSize.cx, dcSize.cy);

    Graphics gdi(memGdiplusDC.GetSafeHdc());

	{ // Data ���
		WCHAR str[_MAX_PATH];
		swprintf(str, L"offset(%3d, %3d), scale = %f", m_offset.x, m_offset.y, m_scale);
        Gdiplus::Font font(L"����ü", 15, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
        SolidBrush brush(Color(0, 0, 0));
        gdi.DrawString(str, wcslen(str), &font, PointF(10, 10), &brush);
	}

    // xy ��ǥ �׸���
	{
        float gridSize = pDoc->m_MaxPt.X;
        if (pDoc->m_MaxPt.X > pDoc->m_MaxPt.Y){
            gridSize = pDoc->m_MaxPt.X / 4;
        } else {
            gridSize = pDoc->m_MaxPt.Y / 4;
        }
        PointF ptXmin = GetTransformPoint(PointF(-gridSize, 0.F));
        PointF ptXmax = GetTransformPoint(PointF(pDoc->m_MaxPt.X * 2, 0.F));
        PointF ptYmin = GetTransformPoint(PointF(0.F, -gridSize));
        PointF ptYmax = GetTransformPoint(PointF(0.F, pDoc->m_MaxPt.Y*2));

        Pen pen(Color(0, 255, 0), 1);
        gdi.DrawLine(&pen, ptXmin, ptXmax);
        gdi.DrawLine(&pen, ptYmin, ptYmax);
	}

	std::vector<PointF> points;
	std::vector<BYTE> types;
	pDoc->m_ptOriginList.GetPoints(points);
	pDoc->m_ptOriginList.GetTypes(types);

	PointF start = {0, 0};
	PointF closePoint = {0, 0};

	GraphicsPath gdiPath;
	for(unsigned int index = 0; index < points.size(); ++index)
	{
		BYTE pathType = types[index] & 0x07; // 0x07�� GDI+ PathPointTypePathTypeMask�� �ǹ���

		if (pathType == 0) { // ������
			start = GetTransformPoint(points[index]);
			closePoint = start;
            gdiPath.StartFigure();
		} else if (pathType == 1) { // ����
			PointF end = GetTransformPoint(points[index]);

            gdiPath.AddLine(start, end);
			start = end;
		} else if (pathType == 3) { // �������� � - ���� 2���� ��� ���̴�.
			PointF cp1 = GetTransformPoint(points[index]);
			PointF cp2 = GetTransformPoint(points[index + 1]);
			PointF end = GetTransformPoint(points[index + 2]);

            gdiPath.AddBezier(start, cp1, cp2, end);

            index += 2;

			start = end;
		} else {
			_ASSERTE(!"�˼����� Type. ������������ ����ϴ� ���� �ƴ϶� ���õ�");
		}

		bool pathClose = (types[index] & 0x80) ? true : false; // 0x80�� GDI+ PathPointTypeCloseSubpath�� �ǹ���

		// ����
		if (pathClose) {
            gdiPath.CloseFigure();

            start = PointF(0, 0);
			closePoint = start;
		}
	}
    gdiPath.CloseAllFigures();

    GraphicsPath guidePath;
	for(unsigned int index = 0; index < points.size(); ++index)
	{
		BYTE pathType = types[index] & 0x07; // 0x07�� GDI+ PathPointTypePathTypeMask�� �ǹ���

		{ // �����
			PointF point = GetTransformPoint(points[index]);

			float size = 2.F;
            RectF rect(point.X - size, point.Y - size, size * 2, size * 2);

            guidePath.AddEllipse(rect);
		}
	}

    { // Point �׸���
        Pen pen(Color(0, 0, 0), 1);
        gdi.DrawPath(&pen, &gdiPath);
    }

    { // Guide �� �׸���
        SolidBrush brush(Color(0, 0, 255));
        gdi.FillPath(&brush, &guidePath);
    }

    // ���� �� �׸���
	for(unsigned int i = 0; i < pDoc->m_nCurrentIndexList.size(); ++i)
	{
		int index = pDoc->m_nCurrentIndexList[i];

        PointF pt = GetTransformPoint(points[index]);

        SolidBrush brush(Color(255, 0, 0));
        float size = 5;
        RectF rect(pt.X - size, pt.Y - size, size * 2, size * 2);
        gdi.FillEllipse(&brush, rect);
	}

    pDC->BitBlt(0, 0, dcSize.cx, dcSize.cy, &memGdiplusDC, 0, 0, SRCCOPY);

	memGdiplusDC.SelectObject(hCairoOldBitmap);
}


// CxyViewerView �μ�


void CxyViewerView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CxyViewerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CxyViewerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CxyViewerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}

void CxyViewerView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CxyViewerView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CxyViewerView ����

#ifdef _DEBUG
void CxyViewerView::AssertValid() const
{
	CView::AssertValid();
}

void CxyViewerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CxyViewerDoc* CxyViewerView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CxyViewerDoc)));
	return (CxyViewerDoc*)m_pDocument;
}
#endif //_DEBUG


// CxyViewerView �޽��� ó����

void CxyViewerView::OnLButtonDown(UINT nFlags, CPoint point)
{
    SetCapture();
    ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
   	CView::OnLButtonDown(nFlags, point);
}

void CxyViewerView::OnLButtonUp(UINT nFlags, CPoint point)
{
    ReleaseCapture();
    CView::OnLButtonUp(nFlags, point);
}

void CxyViewerView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CxyViewerDoc* pDoc = GetDocument();

	// ������ Index ����
	pDoc->m_nCurrentIndexList.clear();

	std::vector<PointF> points;
	pDoc->m_ptOriginList.GetPoints(points);

	for(unsigned int i = 0; i < points.size(); ++i)
	{
		PointF pt = GetTransformPoint(points[i]);

		float distance = std::sqrt(std::pow(std::abs((float)(point.x - pt.X)), 2) + std::pow(std::abs((float)(point.y - pt.Y)), 2));
		if(distance < 5) {
			pDoc->m_nCurrentIndexList.push_back(i);
		}
	}

	// �Ÿ������� sort
	for(unsigned int i = 0; i < pDoc->m_nCurrentIndexList.size(); ++i)
	{
		for(unsigned int j = i; j < pDoc->m_nCurrentIndexList.size() - 1; ++j)
		{
			int index1 = pDoc->m_nCurrentIndexList[j];
			int index2 = pDoc->m_nCurrentIndexList[j + 1];

			PointF pt1 = points[index1];
			PointF pt2 = points[index2];

			float distance1 = std::sqrt(std::pow(std::abs((float)(point.x - pt1.X)), 2) + std::pow(std::abs((float)(point.y - pt1.Y)), 2));
			float distance2 = std::sqrt(std::pow(std::abs((float)(point.x - pt2.X)), 2) + std::pow(std::abs((float)(point.y - pt2.Y)), 2));

			if(distance1 > distance2) {
				pDoc->m_nCurrentIndexList[j] = index2;
				pDoc->m_nCurrentIndexList[j + 1] = index1;
			}
		}
	}

    pDoc->SelectPathPoint();

	if(nFlags & MK_LBUTTON && m_prevMousePt.x != INT_MIN && m_prevMousePt.y != INT_MIN) {
		//m_offset.x += (LONG)((double)(point.x - m_prevMousePt.x) / m_scale);
		//m_offset.y += (LONG)((double)(point.y - m_prevMousePt.y) / m_scale);
        m_offset.x += point.x - m_prevMousePt.x;
        m_offset.y += point.y - m_prevMousePt.y;

        //static HCURSOR cursor = ::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND));
        //::SetCursor(cursor);
	}

	m_prevMousePt.x = point.x;
	m_prevMousePt.y = point.y;

	InvalidateRect(NULL, FALSE);

	pDoc->ChangeData(this);

	CView::OnMouseMove(nFlags, point);
}

void CxyViewerView::OnViewZoomin()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CxyViewerDoc* pDoc = GetDocument();

    /*if (m_scale >= 4) {
        m_scale = 4;
    } else*/
    {
		RectF rectBound = pDoc->m_ptOriginList.GetBound();
		//m_offset.x += (int)( rectBound.Width * (m_scale - m_scale * 1.1) / 2 );
		//m_offset.y += (int)( rectBound.Height * (m_scale - m_scale * 1.1) / 2 );

		m_scale *= 1.1F;
		m_scale = (float)(int)(m_scale * 100) / 100;
		//m_offset.x *= m_scale;
		//m_offset.y *= m_scale;
	}

	//InvalidateRect(NULL, FALSE);
}

void CxyViewerView::OnViewZoomout()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CxyViewerDoc* pDoc = GetDocument();

	if(m_scale <= 0.1) {
		m_scale = 0.1F;
	} else {
		RectF rectBound = pDoc->m_ptOriginList.GetBound();
		//m_offset.x += (int)( rectBound.Width * (m_scale - m_scale / 1.1) / 2 );
		//m_offset.y += (int)( rectBound.Height * (m_scale - m_scale / 1.1) / 2 );

		m_scale /= 1.1F;
		m_scale = (float)(int)(m_scale * 100) / 100;
	}

	//InvalidateRect(NULL, FALSE);
}
void CxyViewerView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	CxyViewerDoc* pDoc = GetDocument();

	RECT rectClient;
	GetClientRect(&rectClient);
	m_offset.x = 20;
	m_offset.y = 20;
    ::OffsetRect(&rectClient, m_offset.x, m_offset.y);
    if (rectClient.right - rectClient.left > m_offset.x) {
        rectClient.right -= m_offset.x;
        if (rectClient.right - rectClient.left > 20) { // ������ ����
            rectClient.right -= 20;
        }
    }
    if (rectClient.bottom - rectClient.top > m_offset.y) {
        rectClient.bottom -= m_offset.y;
        if (rectClient.bottom - rectClient.top > 20) { // �Ʒ��� ����
            rectClient.bottom -= 20;
        }
    }

	m_prevMousePt.x = INT_MIN;
	m_prevMousePt.y = INT_MIN;

	m_scale = CPointUtil::CalculateScale(rectClient, pDoc->m_MinPt, pDoc->m_MaxPt);
}

BOOL CxyViewerView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	if(nFlags == MK_CONTROL) {
		if(zDelta < 0) {
			OnViewZoomout();
		} else {
			OnViewZoomin();
		}
	}

	//InvalidateRect(NULL, FALSE);

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CxyViewerView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	//if(nChar == 49) {
	//	OnViewZoomin();
	//} else if(nChar == 50) {
	//	OnViewZoomout();
	//}

	//InvalidateRect(NULL, FALSE);

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

//POINT CxyViewerView::GetTransformPoint(POINT pt) const
//{
//    CxyViewerDoc* pDoc = GetDocument();
//
//    POINT point = { pt.x, pt.y };
//	if(pt.x != INT_MIN || pt.y != INT_MIN) {
//		// ������ + (���� + ���ο�����) * ������
//		point.x = (int)((float)m_offset.x + (float)(pt.x + pDoc->m_Offset.x) * m_scale);
//		point.y = (int)((float)m_offset.y + (float)(pt.y + pDoc->m_Offset.y) * m_scale);
//	}
//	return point;
//}

PointF CxyViewerView::GetTransformPoint(PointF pt) const
{
    CxyViewerDoc* pDoc = GetDocument();

    PointF point;
	// ������ + (���� + ���ο�����) * ������
	point.X = (float)m_offset.x + (pt.X + pDoc->m_Offset.X) * m_scale;
	point.Y = (float)m_offset.y + (pt.Y + pDoc->m_Offset.Y) * m_scale;
	return point;
}