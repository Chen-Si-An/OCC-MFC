
// MFCView.h: CMFCView 類別的介面
//

#pragma once

#include "Matrix4.h"


class CMFCView : public CView
{
protected: // 僅從序列化建立
	CMFCView() noexcept;
	DECLARE_DYNCREATE(CMFCView)

// 屬性
public:
	CMFCDoc* GetDocument() const;

// 作業
public:

// 覆寫
public:
	virtual void OnDraw(CDC* pDC);  // 覆寫以描繪此檢視
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 程式碼實作
public:
	virtual ~CMFCView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 產生的訊息對應函式
protected:
	DECLARE_MESSAGE_MAP()
public:
	int m_iCursorX;
	int m_iCursorY;
	double m_dCenter[3];
	double m_dAxis[3];
	CMatrix4 m_matModel;
	int m_iMouseState;
	void* m_pSelectedShape;

	void DrawScene3D();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnButtonSelectModel();
	afx_msg void OnUpdateButtonSelectModel(CCmdUI* pCmdUI);
	afx_msg void OnButtonRotateModel();
	afx_msg void OnUpdateButtonRotateModel(CCmdUI* pCmdUI);
	afx_msg void OnButtonMoveModel();
	afx_msg void OnUpdateButtonMoveModel(CCmdUI* pCmdUI);
	afx_msg void OnButtonViewTop();
	afx_msg void OnButtonViewBottom();
	afx_msg void OnButtonViewFront();
	afx_msg void OnButtonViewBack();
	afx_msg void OnButtonViewLeft();
	afx_msg void OnButtonViewRight();
};

#ifndef _DEBUG  // 對 MFCView.cpp 中的版本進行偵錯
inline CMFCDoc* CMFCView::GetDocument() const
   { return reinterpret_cast<CMFCDoc*>(m_pDocument); }
#endif

