#include "pch.h"
#include "ExtDll.h"

CExtDll::CExtDll()
{
	m_hDriver = NULL;

	m_NewView = NULL;
	m_DeleteView = NULL;
	m_Redraw = NULL;
	m_CreateContext = NULL;
	m_SetWindow = NULL;
}

CExtDll::~CExtDll()
{
	if (m_hDriver)
		FreeLibrary(m_hDriver);
}

void CExtDll::LoadDriver()
{
	m_hDriver = LoadLibrary(_T("OCCLib.dll"));
	if (!m_hDriver)
	{
		AfxMessageBox(_T("LoadLibrary Failed"));
		return;
	}

	m_NewView = (NEW_VIEW)GetProcAddress(m_hDriver, "NewView");
	if (!m_NewView)
	{
		AfxMessageBox(_T("GetProcAddress Failed"));
		return;
	}
	

	m_DeleteView = (DELETE_VIEW)GetProcAddress(m_hDriver, "DeleteView");
	if (!m_DeleteView)
	{
		AfxMessageBox(_T("GetProcAddress Failed"));
		return;
	}

	m_CreateContext = (CREATE_CONTEXT)GetProcAddress(m_hDriver, "CreateContext");
	if (!m_CreateContext)
	{
		AfxMessageBox(_T("GetProcAddress Failed"));
		return;
	}

	m_SetWindow = (SET_WINDOW)GetProcAddress(m_hDriver, "SetWindow");
	if (!m_SetWindow)
	{
		AfxMessageBox(_T("GetProcAddress Failed"));
		return;
	}

	m_Redraw = (REDRAW)GetProcAddress(m_hDriver, "Redraw");
	if (!m_Redraw)
	{
		AfxMessageBox(_T("GetProcAddress Failed"));
		return;
	}
}

void* CExtDll::NewView()
{
	if (!m_NewView)
		return NULL;
	return m_NewView();
}

void CExtDll::DeleteView(void* pView)
{
	if (!m_DeleteView)
		return;
	m_DeleteView(pView);
}

void CExtDll::CreateContext(void* pView)
{
	if (!m_CreateContext)
		return;
	if (!pView)
		return;
	m_CreateContext(pView);
}

void CExtDll::SetWindow(void* pView, HWND hwnd)
{
	if (!m_SetWindow)
		return;
	if (!pView)
		return;
	if (!hwnd)
		return;
	m_SetWindow(pView, hwnd);
}

void CExtDll::Redraw(void* pView)
{
	if (!m_Redraw)
		return;
	if (!pView)
		return;
	m_Redraw(pView);
}