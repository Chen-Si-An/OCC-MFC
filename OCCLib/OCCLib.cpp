﻿// OCCLib.cpp: 定義 DLL 的初始化常式。
//

#include "pch.h"
#include "framework.h"
#include "OCCLib.h"
#include "OCCView.h"
#include <vector>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO:  如果這個 DLL 是動態地對 MFC DLL 連結，
//		那麼從這個 DLL 匯出的任何會呼叫
//		MFC 內部的函式，都必須在函式一開頭加上 AFX_MANAGE_STATE
//		巨集。
//
//		例如: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此處為正常函式主體
//		}
//
//		這個巨集一定要出現在每一個
//		函式中，才能夠呼叫 MFC 的內部。這意味著
//		其必須是函式內的第一個陳述式
//		，甚至必須在任何物件變數宣告前面
//		，因為它們的建構函式可能會產生對 MFC
//		DLL 內部的呼叫。
//
//		請參閱 MFC 技術提示 33 和 58 中的
//		詳細資料。
//

// COCCLibApp

BEGIN_MESSAGE_MAP(COCCLibApp, CWinApp)
END_MESSAGE_MAP()


// COCCLibApp 建構

COCCLibApp::COCCLibApp()
{
	// TODO:  在此加入建構程式碼，
	// 將所有重要的初始設定加入 InitInstance 中
}


// 唯一一個 COCCLibApp 物件

COCCLibApp theApp;
vector<COCCView*> m_ayOCCView;


// COCCLibApp 初始化

BOOL COCCLibApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

extern "C" void* PASCAL EXPORT NewView()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	COCCView *pOCCView = new COCCView;
	m_ayOCCView.push_back(pOCCView);

	return (void*)pOCCView;
}

extern "C" void PASCAL EXPORT DeleteView(void *pView)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!pView)
		return;

	COCCView *pOCCView = (COCCView*)pView;
	int iSize = (int)m_ayOCCView.size();
	for (int i = 0; i < iSize; i++)
	{
		if (pOCCView == m_ayOCCView[i])
		{
			m_ayOCCView.erase(m_ayOCCView.begin() + i);
			delete pOCCView;

			break;
		}
	}
}

extern "C" void PASCAL EXPORT CreateContext(void *pView)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	COCCView *pOCCView = (COCCView*)pView;
	if (!pOCCView)
		return;

	return pOCCView->CreateContext();
}

extern "C" void PASCAL EXPORT SetWindow(void *pView, HWND hwnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	COCCView *pOCCView = (COCCView*)pView;
	if (!pOCCView)
		return;

	return pOCCView->SetWindow(hwnd);
}

extern "C" void PASCAL EXPORT Redraw(void* pView)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	COCCView* pOCCView = (COCCView*)pView;
	if (!pOCCView)
		return;

	return pOCCView->Redraw();
}