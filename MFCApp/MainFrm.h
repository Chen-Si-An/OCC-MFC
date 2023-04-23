
// MainFrm.h: CMainFrame 類別的介面
//

#pragma once

class CMainFrame : public CFrameWnd
{
	
protected: // 僅從序列化建立
	CMainFrame() noexcept;
	DECLARE_DYNCREATE(CMainFrame)

// 屬性
public:

// 作業
public:

// 覆寫
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 程式碼實作
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控制列內嵌的成員
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;

// 產生的訊息對應函式
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

};


