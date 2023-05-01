﻿
// MFCDoc.cpp: CMFCDoc 類別的實作
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
#include "Matrix4.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CExtDll g_ExtDll;

void WriteSTL(CStdioFile& file, const vector<double>& vecMesh)
{
	CString str(_T("solid STL generated by Allen\n"));
	file.WriteString(str);

	int iSize = (int)vecMesh.size();
	for (int i = 0; i + 8 < iSize; i += 9)
	{
		double x1 = vecMesh[i + 0];
		double y1 = vecMesh[i + 1];
		double z1 = vecMesh[i + 2];
		double x2 = vecMesh[i + 3];
		double y2 = vecMesh[i + 4];
		double z2 = vecMesh[i + 5];
		double x3 = vecMesh[i + 6];
		double y3 = vecMesh[i + 7];
		double z3 = vecMesh[i + 8];
		double ux = x2 - x1;
		double uy = y2 - y1;
		double uz = z2 - z1;
		double vx = x3 - x1;
		double vy = y3 - y1;
		double vz = z3 - z1;
		double nx = uy * vz - uz * vy;
		double ny = uz * vx - ux * vz;
		double nz = ux * vy - uy * vx;
		double len = sqrt(nx * nx + ny * ny + nz * nz);
		nx /= len;
		ny /= len;
		nz /= len;
		str.Format(_T("  facet normal %lf %lf %lf\n"), nx, ny, nz);
		file.WriteString(str);
		str.Format(_T("    outer loop\n"));
		file.WriteString(str);
		str.Format(_T("      vertex  %lf %lf %lf\n"), x1, y1, z1);
		file.WriteString(str);
		str.Format(_T("      vertex  %lf %lf %lf\n"), x2, y2, z2);
		file.WriteString(str);
		str.Format(_T("      vertex  %lf %lf %lf\n"), x3, y3, z3);
		file.WriteString(str);
		str.Format(_T("    endloop\n"));
		file.WriteString(str);
		str.Format(_T("  endfacet\n"));
		file.WriteString(str);
	}
	str.Format(_T("endsolid\n"));
	file.WriteString(str);
}

// CMFCDoc

IMPLEMENT_DYNCREATE(CMFCDoc, CDocument)

BEGIN_MESSAGE_MAP(CMFCDoc, CDocument)
	ON_COMMAND(ID_BUTTON_IMPORT_IGES, &CMFCDoc::OnButtonImportIges)
	ON_COMMAND(ID_BUTTON_IMPORT_STEP, &CMFCDoc::OnButtonImportStep)
	ON_COMMAND(ID_BUTTON_IMPORT_STL, &CMFCDoc::OnButtonImportStl)
	ON_COMMAND(ID_BUTTON_EXPORT_STL, &CMFCDoc::OnButtonExportStl)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_EXPORT_STL, &CMFCDoc::OnUpdateButtonExportStl)
END_MESSAGE_MAP()


// CMFCDoc 建構/解構

CMFCDoc::CMFCDoc() noexcept
{
	// TODO: 在此加入一次建構程式碼
	m_pOCCView = NULL;
}

CMFCDoc::~CMFCDoc()
{
	if (m_pOCCView)
	{
		int iSize = (int)m_vecModel.size();
		for (int i = 0; i < iSize; i++)
			g_ExtDll.DeleteModel(m_pOCCView, m_vecModel[i]);
		vector<void*>().swap(m_vecModel);

		g_ExtDll.DeleteView(m_pOCCView);
		m_pOCCView = NULL;
	}
}

BOOL CMFCDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此加入重新初始化程式碼
	// (SDI 文件會重用此文件)
	m_pOCCView = g_ExtDll.NewView();

	return TRUE;
}




// CMFCDoc 序列化

void CMFCDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此加入儲存程式碼
	}
	else
	{
		// TODO: 在此加入載入程式碼
	}
}

#ifdef SHARED_HANDLERS

// 縮圖的支援
void CMFCDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改這段程式碼以繪製文件的資料
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜尋處理常式的支援
void CMFCDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 設定來自文件資料的搜尋內容。
	// 內容部分應該以 ";" 隔開

	// 範例:     strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CMFCDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CMFCDoc 診斷

#ifdef _DEBUG
void CMFCDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMFCDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMFCDoc 命令


void CMFCDoc::OnButtonImportIges()
{
	CFileDialog dlg(TRUE, _T("*.iges"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("IGES Files (*.iges)|*.iges||"), NULL);
	if (dlg.DoModal() != IDOK)
		return;

	void* pModel = g_ExtDll.ReadIges(m_pOCCView, dlg.GetPathName());
	if (pModel)
		m_vecModel.push_back(pModel);

	if (m_pOCCView)
		g_ExtDll.DrawModel(m_pOCCView, pModel);

	UpdateAllViews(NULL);
}

void CMFCDoc::OnButtonImportStep()
{
	CFileDialog dlg(TRUE, _T("*.step"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("STEP Files (*.step)|*.step||"), NULL);
	if (dlg.DoModal() != IDOK)
		return;

	void* pModel = g_ExtDll.ReadStep(m_pOCCView, dlg.GetPathName());
	if (pModel)
		m_vecModel.push_back(pModel);

	if (m_pOCCView)
		g_ExtDll.DrawModel(m_pOCCView, pModel);

	UpdateAllViews(NULL);
}

void CMFCDoc::OnButtonImportStl()
{
	CFileDialog dlg(TRUE, _T("*.stl"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("STL Files (*.stl)|*.stl||"), NULL);
	if (dlg.DoModal() != IDOK)
		return;

	void* pModel = g_ExtDll.ReadStl(m_pOCCView, dlg.GetPathName());
	if (pModel)
		m_vecModel.push_back(pModel);

	if (m_pOCCView)
		g_ExtDll.DrawModel(m_pOCCView, pModel);

	UpdateAllViews(NULL);
}

void CMFCDoc::OnButtonExportStl()
{
	CMFCView* pView = (CMFCView*)((CFrameWnd*)AfxGetMainWnd())->GetActiveView();
	if (!pView)
		return;

	vector<double> vecMesh;
	g_ExtDll.GetModelMesh(pView->m_pSelectedShape, vecMesh);
	if (vecMesh.size() == 0)
		return;

	CFileDialog dlg(FALSE, _T("*.stl"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("STL Files (*.stl)|*.stl||"), NULL);
	if (dlg.DoModal() != IDOK)
		return;

	CStdioFile file(dlg.GetPathName(), CFile::modeCreate | CFile::modeWrite | CFile::typeText);
	WriteSTL(file, vecMesh);
	file.Close();
	vector<double>().swap(vecMesh);
}

void CMFCDoc::OnUpdateButtonExportStl(CCmdUI* pCmdUI)
{
	CMFCView* pView = (CMFCView*)((CFrameWnd*)AfxGetMainWnd())->GetActiveView();
	if (!pView)
		return;

	pCmdUI->Enable(pView->m_pSelectedShape != NULL);
}