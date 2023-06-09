﻿
// MFCDoc.h: CMFCDoc 類別的介面
//


#pragma once

#include <vector>
using namespace std;


class CMFCDoc : public CDocument
{
protected: // 僅從序列化建立
	CMFCDoc() noexcept;
	DECLARE_DYNCREATE(CMFCDoc)

// 屬性
public:

// 作業
public:

// 覆寫
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 程式碼實作
public:
	virtual ~CMFCDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 產生的訊息對應函式
protected:
	afx_msg void OnButtonImportIges();
	afx_msg void OnButtonImportStep();
	afx_msg void OnButtonImportStl();
	afx_msg void OnButtonImportObj();
	afx_msg void OnButtonExportStl();
	afx_msg void OnUpdateButtonExportStl(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 為搜尋處理常式設定搜尋內容的 Helper 函式
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	void* m_pOCCView;
	vector<void*> m_vecModel;
};
