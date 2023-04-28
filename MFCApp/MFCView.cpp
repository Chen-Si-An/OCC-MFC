
// MFCView.cpp: CMFCView 類別的實作
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以定義在實作預覽、縮圖和搜尋篩選條件處理常式的
// ATL 專案中，並允許與該專案共用文件程式碼。
#ifndef SHARED_HANDLERS
#include "MFCApp.h"
#endif

#include "MFCDoc.h"
#include "MFCView.h"
#include "ExtDll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CExtDll g_ExtDll;


// CMFCView

IMPLEMENT_DYNCREATE(CMFCView, CView)

BEGIN_MESSAGE_MAP(CMFCView, CView)
	// 標準列印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

// CMFCView 建構/解構

CMFCView::CMFCView() noexcept
{
	m_iCursorX = 0;
	m_iCursorY = 0;
}

CMFCView::~CMFCView()
{
}

BOOL CMFCView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此經由修改 CREATESTRUCT cs 
	// 達到修改視窗類別或樣式的目的

	return CView::PreCreateWindow(cs);
}

void CMFCView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 在此加入特定的程式碼和 (或) 呼叫基底類別
	CMFCDoc* pDoc = GetDocument();
	if (pDoc && pDoc->m_pOCCView)
	{
		g_ExtDll.CreateContext(pDoc->m_pOCCView);
		g_ExtDll.SetWindow(pDoc->m_pOCCView, m_hWnd);
	}
}

// CMFCView 繪圖

void CMFCView::OnDraw(CDC* /*pDC*/)
{
	CMFCDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此加入原生資料的描繪程式碼
	if (pDoc->m_pOCCView)
	{
		DrawScene3D();

		g_ExtDll.UpdateCurrentViewer(pDoc->m_pOCCView);
	}
}


// CMFCView 列印

BOOL CMFCView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 預設的準備列印程式碼
	return DoPreparePrinting(pInfo);
}

void CMFCView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 加入列印前額外的初始設定
}

void CMFCView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 加入列印後的清除程式碼
}


// CMFCView 診斷

#ifdef _DEBUG
void CMFCView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCDoc* CMFCView::GetDocument() const // 內嵌非偵錯版本
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCDoc)));
	return (CMFCDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCView 訊息處理常式


void CMFCView::DrawScene3D()
{
	CMFCDoc* pDoc = GetDocument();
	if (!pDoc || !pDoc->m_pOCCView)
		return;

	g_ExtDll.EraseAllView(pDoc->m_pOCCView);

	//Draw the background
	g_ExtDll.DrawCoordSys(pDoc->m_pOCCView);
	g_ExtDll.DrawHorzPlane(pDoc->m_pOCCView);

	//Draw the model
	int iSize = (int)pDoc->m_vecModel.size();
	for (int i = 0; i < iSize; i++)
		g_ExtDll.DrawModel(pDoc->m_pOCCView, pDoc->m_vecModel[i]);
}


void CMFCView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 在此加入您的訊息處理常式程式碼
	CMFCDoc* pDoc = GetDocument();
	if (pDoc && pDoc->m_pOCCView)
		g_ExtDll.Resize(pDoc->m_pOCCView);
}


void CMFCView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	m_iCursorX = point.x;
	m_iCursorY = point.y;

	CView::OnLButtonDown(nFlags, point);
}


void CMFCView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值

	CView::OnLButtonUp(nFlags, point);
}


void CMFCView::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	m_iCursorX = point.x;
	m_iCursorY = point.y;

	CView::OnMButtonDown(nFlags, point);
}


void CMFCView::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值

	CView::OnMButtonUp(nFlags, point);
}


void CMFCView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	m_iCursorX = point.x;
	m_iCursorY = point.y;

	CMFCDoc* pDoc = GetDocument();
	if (pDoc && pDoc->m_pOCCView)
	{
		if (nFlags & MK_LBUTTON)
			;
		else
		{
			double dCenter[3] = { 0., 0., 0. };
			g_ExtDll.ViewStartRotation(pDoc->m_pOCCView, dCenter, point.x, point.y);
		}
	}

	CView::OnRButtonDown(nFlags, point);
}


void CMFCView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值

	CView::OnRButtonUp(nFlags, point);
}


void CMFCView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	CMFCDoc* pDoc = GetDocument();
	if (pDoc && pDoc->m_pOCCView)
	{
		if (nFlags & MK_LBUTTON)
			;
		else if (nFlags & MK_MBUTTON)
		{
			int iPanningX = point.x - m_iCursorX;
			int iPanningY = -(point.y - m_iCursorY);
			m_iCursorX = point.x;
			m_iCursorY = point.y;
			g_ExtDll.ViewPan(pDoc->m_pOCCView, iPanningX, iPanningY);
		}
		else if (nFlags & MK_RBUTTON)
			g_ExtDll.ViewRotation(pDoc->m_pOCCView, point.x, point.y);
	}

	CView::OnMouseMove(nFlags, point);
}


BOOL CMFCView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	CMFCDoc* pDoc = GetDocument();
	if (pDoc && pDoc->m_pOCCView)
	{
		CPoint ptClient;
		ScreenToClient(&ptClient);
		ptClient += pt;

		double dStep = zDelta / 120. / 20.;
		double dZoomFactor = 0.;
		if (zDelta > 0.)
			dZoomFactor = 1. + dStep;
		else
			dZoomFactor = 1. / (1. - dStep);

		if (dZoomFactor > 0.)
			g_ExtDll.ViewZoom(pDoc->m_pOCCView, ptClient.x, ptClient.y, dZoomFactor);
		else
			ASSERT(0);
	}

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
