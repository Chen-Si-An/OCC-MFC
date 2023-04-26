#include "pch.h"
#include "ExtDll.h"

CExtDll::CExtDll()
{
	m_hDriver = NULL;

	m_NewView = NULL;
	m_DeleteView = NULL;

	//Create the view
	m_CreateContext = NULL;
	m_SetWindow = NULL;
	m_EraseAllView = NULL;
	m_Resize = NULL;
	m_UpdateCurView = NULL;

	//Draw the background
	m_DrawCoordSys = NULL;
	m_DrawHorzPlane = NULL;
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

	//Create the view
	m_CreateContext = (CREATE_CONTEXT)GetProcAddress(m_hDriver, "CreateContext");
	if (!m_CreateContext)
	{
		AfxMessageBox(_T("GetProcAddress Failed"));
		return;
	}

	m_EraseAllView = (ERASE_ALL_VIEW)GetProcAddress(m_hDriver, "EraseAllView");
	if (!m_EraseAllView)
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

	m_Resize = (RESIZE)GetProcAddress(m_hDriver, "Resize");
	if (!m_Resize)
	{
		AfxMessageBox(_T("GetProcAddress Failed"));
		return;
	}

	m_UpdateCurView = (UPDATE_CUR_VIEW)GetProcAddress(m_hDriver, "UpdateCurrentViewer");
	if (!m_UpdateCurView)
	{
		AfxMessageBox(_T("GetProcAddress Failed"));
		return;
	}

	//Draw the background
	m_DrawCoordSys = (DRAW_COORD_SYS)GetProcAddress(m_hDriver, "DrawCoordSys");
	if (!m_DrawCoordSys)
	{
		AfxMessageBox(_T("GetProcAddress Failed"));
		return;
	}

	m_DrawHorzPlane = (DRAW_HORZ_PLANE)GetProcAddress(m_hDriver, "DrawHorzPlane");
	if (!m_DrawHorzPlane)
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

//Create the view
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

void CExtDll::EraseAllView(void* pView)
{
	if (!m_EraseAllView)
		return;
	if (!pView)
		return;
	m_EraseAllView(pView);
}

void CExtDll::Resize(void* pView)
{
	if (!m_Resize)
		return;
	if (!pView)
		return;
	m_Resize(pView);
}

void CExtDll::UpdateCurrentViewer(void* pView)
{
	if (!m_UpdateCurView)
		return;
	if (!pView)
		return;
	m_UpdateCurView(pView);
}

//Draw the background
void CExtDll::DrawCoordSys(void* pView)
{
	if (!m_DrawCoordSys)
		return;
	if (!pView)
		return;
	m_DrawCoordSys(pView);
}

void CExtDll::DrawHorzPlane(void* pView)
{
	if (!m_DrawHorzPlane)
		return;
	if (!pView)
		return;
	m_DrawHorzPlane(pView);
}