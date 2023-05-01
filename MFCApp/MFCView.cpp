
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

#define PRECISION	1.e-7
#define MOUSE_SELECT_MODEL	1
#define MOUSE_ROTATE_MODEL	2
#define MOUSE_MOVE_MODEL	3

extern CExtDll g_ExtDll;

BOOL Cross(double dVec1[3], double dVec2[3], double dVec3[3])
{
	dVec3[0] = dVec1[1] * dVec2[2] - dVec1[2] * dVec2[1];
	dVec3[1] = dVec1[2] * dVec2[0] - dVec1[0] * dVec2[2];
	dVec3[2] = dVec1[0] * dVec2[1] - dVec1[1] * dVec2[0];
	if (fabs(dVec3[0]) < PRECISION && fabs(dVec3[1]) < PRECISION && fabs(dVec3[2]) < PRECISION)
		return FALSE;
	return TRUE;
}

// Calculate angle between two vectors
double Angle(double dVec1[3], double dVec2[3])
{
	double dDot = dVec1[0] * dVec2[0] + dVec1[1] * dVec2[1] + dVec1[2] * dVec2[2];
	double dLen1 = sqrt(dVec1[0] * dVec1[0] + dVec1[1] * dVec1[1] + dVec1[2] * dVec1[2]);
	double dLen2 = sqrt(dVec2[0] * dVec2[0] + dVec2[1] * dVec2[1] + dVec2[2] * dVec2[2]);
	double dCos = dDot / (dLen1 * dLen2);
	if (dCos > 1.0)
		dCos = 1.0;
	else if (dCos < -1.0)
		dCos = -1.0;
	return acos(dCos);
}

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
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_BUTTON_SELECT_MODEL, &CMFCView::OnButtonSelectModel)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_SELECT_MODEL, &CMFCView::OnUpdateButtonSelectModel)
	ON_COMMAND(ID_BUTTON_ROTATE_MODEL, &CMFCView::OnButtonRotateModel)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_ROTATE_MODEL, &CMFCView::OnUpdateButtonRotateModel)
	ON_COMMAND(ID_BUTTON_MOVE_MODEL, &CMFCView::OnButtonMoveModel)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_MOVE_MODEL, &CMFCView::OnUpdateButtonMoveModel)
	ON_COMMAND(ID_BUTTON_VIEW_TOP, &CMFCView::OnButtonViewTop)
	ON_COMMAND(ID_BUTTON_VIEW_BOTTOM, &CMFCView::OnButtonViewBottom)
	ON_COMMAND(ID_BUTTON_VIEW_FRONT, &CMFCView::OnButtonViewFront)
	ON_COMMAND(ID_BUTTON_VIEW_BACK, &CMFCView::OnButtonViewBack)
	ON_COMMAND(ID_BUTTON_VIEW_LEFT, &CMFCView::OnButtonViewLeft)
	ON_COMMAND(ID_BUTTON_VIEW_RIGHT, &CMFCView::OnButtonViewRight)
END_MESSAGE_MAP()

// CMFCView 建構/解構

CMFCView::CMFCView() noexcept
{
	m_iCursorX = 0;
	m_iCursorY = 0;
	m_dCenter[0] = 0.0;
	m_dCenter[1] = 0.0;
	m_dCenter[2] = 0.0;
	m_dAxis[0] = 1.0;
	m_dAxis[1] = 0.0;
	m_dAxis[2] = 0.0;
	m_iMouseState = MOUSE_SELECT_MODEL;
	m_pSelectedShape = NULL;
}

CMFCView::~CMFCView()
{
	m_pSelectedShape = NULL;
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


BOOL CMFCView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此加入特定的程式碼和 (或) 呼叫基底類別
	// Check key delete
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_DELETE)
		{
			CMFCDoc* pDoc = GetDocument();
			if (pDoc && pDoc->m_pOCCView && m_pSelectedShape)
			{
				g_ExtDll.DeleteModel(pDoc->m_pOCCView, m_pSelectedShape);

				vector<void*>::iterator it = find(pDoc->m_vecModel.begin(), pDoc->m_vecModel.end(), m_pSelectedShape);
				if (it != pDoc->m_vecModel.end())
					pDoc->m_vecModel.erase(it);
				m_pSelectedShape = NULL;

				Invalidate();
			}
		}
	}

	return CView::PreTranslateMessage(pMsg);
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

		g_ExtDll.UpdateView(pDoc->m_pOCCView);
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

	vector<void*> vecSelected;
	g_ExtDll.GetSelected(pDoc->m_pOCCView, vecSelected);

	g_ExtDll.EraseView(pDoc->m_pOCCView);

	//Draw the background
	g_ExtDll.DrawCoordSys(pDoc->m_pOCCView);
	g_ExtDll.DrawHorzPlane(pDoc->m_pOCCView);

	//Draw the model
	double dColorSel[3] = { 0.8, 0.8, 0.0 };
	double dColorUnsel[3] = { 0.3, 0.3, 0.3 };
	int iSize = (int)pDoc->m_vecModel.size();
	for (int i = 0; i < iSize; i++)
	{
		g_ExtDll.DrawModel(pDoc->m_pOCCView, pDoc->m_vecModel[i]);

		if (pDoc->m_vecModel[i] == m_pSelectedShape)
			g_ExtDll.SetModelColor(pDoc->m_vecModel[i], dColorSel);
		else
			g_ExtDll.SetModelColor(pDoc->m_vecModel[i], dColorUnsel);
	}
}


void CMFCView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 在此加入您的訊息處理常式程式碼
	CMFCDoc* pDoc = GetDocument();
	if (pDoc && pDoc->m_pOCCView)
		g_ExtDll.Resize(pDoc->m_pOCCView);
}


BOOL CMFCView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	CMFCApp* pApp = (CMFCApp*)AfxGetApp();
	if (pApp)
	{
		switch (m_iMouseState)
		{
		case MOUSE_SELECT_MODEL:
			::SetCursor(pApp->LoadCursor(IDC_CURSOR_SELECT_MODEL));
			return TRUE;
		case MOUSE_ROTATE_MODEL:
			::SetCursor(pApp->LoadCursor(IDC_CURSOR_ROTATE_MODEL));
			return TRUE;
		case MOUSE_MOVE_MODEL:
			::SetCursor(pApp->LoadCursor(IDC_CURSOR_MOVE_MODEL));
			return TRUE;
		}
	}

	return CView::OnSetCursor(pWnd, nHitTest, message);
}


void CMFCView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	m_iCursorX = point.x;
	m_iCursorY = point.y;

	CMFCDoc* pDoc = GetDocument();
	if (pDoc && pDoc->m_pOCCView)
	{
		g_ExtDll.ClearSelected(pDoc->m_pOCCView);
		m_pSelectedShape = NULL;

		g_ExtDll.Select(pDoc->m_pOCCView, m_iCursorX, m_iCursorY);

		vector<void*> vecSelected;
		g_ExtDll.GetSelected(pDoc->m_pOCCView, vecSelected);
		if (vecSelected.size() == 1)
			m_pSelectedShape = vecSelected[0];
		vector<void*>().swap(vecSelected);

		if (m_pSelectedShape)
		{
			g_ExtDll.GetModelCenter(m_pSelectedShape, m_dCenter);

			double dResult[3] = { 0., 0., 0. };
			g_ExtDll.ViewConvert(pDoc->m_pOCCView, point.x, point.y, m_dCenter, dResult);
			m_dAxis[0] = dResult[0] - m_dCenter[0];
			m_dAxis[1] = dResult[1] - m_dCenter[1];
			m_dAxis[2] = dResult[2] - m_dCenter[2];

			double dMatrix[4][4] = {0.};
			g_ExtDll.GetModelMatrix(m_pSelectedShape, dMatrix);
			m_matModel.SetValues(dMatrix);

			DrawScene3D();
		}
	}

	CView::OnLButtonDown(nFlags, point);
}


void CMFCView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	Invalidate();

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
			m_dCenter[0] = 0.;
			m_dCenter[1] = 0.;
			m_dCenter[2] = 0.;

			double dResult[3] = { 0., 0., 0. };
			g_ExtDll.ViewConvert(pDoc->m_pOCCView, point.x, point.y, m_dCenter, dResult);
			m_dAxis[0] = dResult[0] - m_dCenter[0];
			m_dAxis[1] = dResult[1] - m_dCenter[1];
			m_dAxis[2] = dResult[2] - m_dCenter[2];
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
		{
			switch (m_iMouseState)
			{
			case MOUSE_SELECT_MODEL:
				break;
			case MOUSE_ROTATE_MODEL:
				if (m_pSelectedShape)
				{
					double dResult[3] = { 0., 0., 0. };
					g_ExtDll.ViewConvert(pDoc->m_pOCCView, point.x, point.y, m_dCenter, dResult);
					double dVector[3] = { dResult[0] - m_dCenter[0], dResult[1] - m_dCenter[1], dResult[2] - m_dCenter[2] };

					double dAxis[3] = { 0., 0., 0. };
					Cross(m_dAxis, dVector, dAxis);
					double dAngle = Angle(m_dAxis, dVector);

					CMatrix4 matModel;
					matModel.SetIdentity();
					matModel.SetRotation(dAxis[0], dAxis[1], dAxis[2], dAngle, m_dCenter[0], m_dCenter[1], m_dCenter[2]);
					matModel *= m_matModel;

					double dMatrix[4][4] = { 0. };
					matModel.GetValues(dMatrix);
					g_ExtDll.SetModelMatrix(m_pSelectedShape, dMatrix);

					g_ExtDll.UpdateView(pDoc->m_pOCCView);
				}
				break;
			case MOUSE_MOVE_MODEL:
				if (m_pSelectedShape)
				{
					double dResult[3] = { 0., 0., 0. };
					g_ExtDll.ViewConvert(pDoc->m_pOCCView, point.x, point.y, m_dCenter, dResult);
					double dVector[3] = { dResult[0] - m_dCenter[0], dResult[1] - m_dCenter[1], dResult[2] - m_dCenter[2] };
					
					double dTranslate[3] = { dVector[0] - m_dAxis[0], dVector[1] - m_dAxis[1], dVector[2] - m_dAxis[2] };
					CMatrix4 matModel;
					matModel.SetTranslation(dTranslate[0], dTranslate[1], dTranslate[2]);
					matModel *= m_matModel;

					double dMatrix[4][4] = { 0. };
					matModel.GetValues(dMatrix);
					g_ExtDll.SetModelMatrix(m_pSelectedShape, dMatrix);

					g_ExtDll.UpdateView(pDoc->m_pOCCView);
				}
				break;
			}
		}
		else if (nFlags & MK_MBUTTON)
		{
			int iPanningX = point.x - m_iCursorX;
			int iPanningY = -(point.y - m_iCursorY);
			m_iCursorX = point.x;
			m_iCursorY = point.y;
			g_ExtDll.ViewPan(pDoc->m_pOCCView, iPanningX, iPanningY);

			g_ExtDll.UpdateView(pDoc->m_pOCCView);
		}
		else if (nFlags & MK_RBUTTON)
		{
			double dResult[3] = { 0., 0., 0. };
			g_ExtDll.ViewConvert(pDoc->m_pOCCView, point.x, point.y, m_dCenter, dResult);
			double dVector[3] = { dResult[0] - m_dCenter[0], dResult[1] - m_dCenter[1], dResult[2] - m_dCenter[2] };

			double dAxis[3] = { 0., 0., 0. };
			Cross(dVector, m_dAxis, dAxis);
			double dAngle = Angle(dVector, m_dAxis);
			g_ExtDll.ViewRotate(pDoc->m_pOCCView, m_dCenter, dAxis, dAngle);

			g_ExtDll.UpdateView(pDoc->m_pOCCView);
		}
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
		{
			g_ExtDll.ViewZoom(pDoc->m_pOCCView, ptClient.x, ptClient.y, dZoomFactor);
			g_ExtDll.UpdateView(pDoc->m_pOCCView);
		}
		else
			ASSERT(0);
	}

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CMFCView::OnButtonSelectModel()
{
	m_iMouseState = MOUSE_SELECT_MODEL;
}

void CMFCView::OnUpdateButtonSelectModel(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_iMouseState == MOUSE_SELECT_MODEL);
}

void CMFCView::OnButtonRotateModel()
{
	m_iMouseState = MOUSE_ROTATE_MODEL;
}

void CMFCView::OnUpdateButtonRotateModel(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_iMouseState == MOUSE_ROTATE_MODEL);
}

void CMFCView::OnButtonMoveModel()
{
	m_iMouseState = MOUSE_MOVE_MODEL;
}

void CMFCView::OnUpdateButtonMoveModel(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_iMouseState == MOUSE_MOVE_MODEL);
}

void CMFCView::OnButtonViewTop()
{
	CMFCDoc* pDoc = GetDocument();
	if (pDoc && pDoc->m_pOCCView)
	{
		g_ExtDll.ViewReset(pDoc->m_pOCCView);
		g_ExtDll.ViewSetProj(pDoc->m_pOCCView, 0., 0., 1.);
		g_ExtDll.UpdateView(pDoc->m_pOCCView);
	}
}

void CMFCView::OnButtonViewBottom()
{
	CMFCDoc* pDoc = GetDocument();
	if (pDoc && pDoc->m_pOCCView)
	{
		g_ExtDll.ViewReset(pDoc->m_pOCCView);
		g_ExtDll.ViewSetProj(pDoc->m_pOCCView, 0., 0., -1.);
		g_ExtDll.UpdateView(pDoc->m_pOCCView);
	}
}

void CMFCView::OnButtonViewFront()
{
	CMFCDoc* pDoc = GetDocument();
	if (pDoc && pDoc->m_pOCCView)
	{
		g_ExtDll.ViewReset(pDoc->m_pOCCView);
		g_ExtDll.ViewSetProj(pDoc->m_pOCCView, 0., 1., 0.);
		g_ExtDll.UpdateView(pDoc->m_pOCCView);
	}
}

void CMFCView::OnButtonViewBack()
{
	CMFCDoc* pDoc = GetDocument();
	if (pDoc && pDoc->m_pOCCView)
	{
		g_ExtDll.ViewReset(pDoc->m_pOCCView);
		g_ExtDll.ViewSetProj(pDoc->m_pOCCView, 0., -1., 0.);
		g_ExtDll.UpdateView(pDoc->m_pOCCView);
	}
}

void CMFCView::OnButtonViewLeft()
{
	CMFCDoc* pDoc = GetDocument();
	if (pDoc && pDoc->m_pOCCView)
	{
		g_ExtDll.ViewReset(pDoc->m_pOCCView);
		g_ExtDll.ViewSetProj(pDoc->m_pOCCView, -1., 0., 0.);
		g_ExtDll.UpdateView(pDoc->m_pOCCView);
	}
}

void CMFCView::OnButtonViewRight()
{
	CMFCDoc* pDoc = GetDocument();
	if (pDoc && pDoc->m_pOCCView)
	{
		g_ExtDll.ViewReset(pDoc->m_pOCCView);
		g_ExtDll.ViewSetProj(pDoc->m_pOCCView, 1., 0., 0.);
		g_ExtDll.UpdateView(pDoc->m_pOCCView);
	}
}
