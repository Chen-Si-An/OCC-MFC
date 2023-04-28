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
	
	//Manipulate the camera
	m_ViewStartRotation = NULL;
	m_ViewRotation = NULL;
	m_ViewPan = NULL;
	m_ViewZoom = NULL;

	// Model
	m_ReadIges = NULL;
	m_ReadStep = NULL;
	m_ReadStl = NULL;
	m_DeleteModel = NULL;
	m_DrawModel = NULL;
	m_RemoveModel = NULL;
}

CExtDll::~CExtDll()
{
	//FreeDriver();
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

	//Manipulate the camera
	m_ViewStartRotation = (VIEW_START_ROTATION)GetProcAddress(m_hDriver, "ViewStartRotation");
	if (!m_ViewStartRotation)
	{
		AfxMessageBox(_T("GetProcAddress Failed"));
		return;
	}

	m_ViewRotation = (VIEW_ROTATION)GetProcAddress(m_hDriver, "ViewRotation");
	if (!m_ViewRotation)
	{
		AfxMessageBox(_T("GetProcAddress Failed"));
		return;
	}

	m_ViewPan = (VIEW_PAN)GetProcAddress(m_hDriver, "ViewPan");
	if (!m_ViewPan)
	{
		AfxMessageBox(_T("GetProcAddress Failed"));
		return;
	}

	m_ViewZoom = (VIEW_ZOOM)GetProcAddress(m_hDriver, "ViewZoom");
	if (!m_ViewZoom)
	{
		AfxMessageBox(_T("GetProcAddress Failed"));
		return;
	}

	// Model
	m_ReadIges = (READ_IGES)GetProcAddress(m_hDriver, "ReadIges");
	if (!m_ReadIges)
	{
		AfxMessageBox(_T("GetProcAddress Failed"));
		return;
	}

	m_ReadStep = (READ_STEP)GetProcAddress(m_hDriver, "ReadStep");
	if (!m_ReadStep)
	{
		AfxMessageBox(_T("GetProcAddress Failed"));
		return;
	}

	m_ReadStl = (READ_STL)GetProcAddress(m_hDriver, "ReadStl");
	if (!m_ReadStl)
	{
		AfxMessageBox(_T("GetProcAddress Failed"));
		return;
	}

	m_DeleteModel = (DELETE_MODEL)GetProcAddress(m_hDriver, "DeleteModel");
	if (!m_DeleteModel)
	{
		AfxMessageBox(_T("GetProcAddress Failed"));
		return;
	}

	m_DrawModel = (DRAW_MODEL)GetProcAddress(m_hDriver, "DisplayModel");
	if (!m_DrawModel)
	{
		AfxMessageBox(_T("GetProcAddress Failed"));
		return;
	}

	m_RemoveModel = (REMOVE_MODEL)GetProcAddress(m_hDriver, "RemoveModel");
	if (!m_RemoveModel)
	{
		AfxMessageBox(_T("GetProcAddress Failed"));
		return;
	}
}

void CExtDll::FreeDriver()
{
	if (m_hDriver)
		FreeLibrary(m_hDriver);
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

//Manipulate the camera
void CExtDll::ViewStartRotation(void* pView, double dCenter[3], int iMouseX, int iMouseY)
{
	if (!m_ViewStartRotation)
		return;
	if (!pView)
		return;
	m_ViewStartRotation(pView, dCenter, iMouseX, iMouseY);
}

void CExtDll::ViewRotation(void* pView, int iMouseX, int iMouseY)
{
	if (!m_ViewRotation)
		return;
	if (!pView)
		return;
	m_ViewRotation(pView, iMouseX, iMouseY);
}

void CExtDll::ViewPan(void* pView, int iPanningX, int iPanningY)
{
	if (!m_ViewPan)
		return;
	if (!pView)
		return;
	m_ViewPan(pView, iPanningX, iPanningY);
}

void CExtDll::ViewZoom(void* pView, int iMouseX, int iMouseY, double dZoomFactor)
{
	if (!m_ViewZoom)
		return;
	if (!pView)
		return;
	m_ViewZoom(pView, iMouseX, iMouseY, dZoomFactor);
}

void* CExtDll::ReadIges(void *pView, LPCTSTR pFileName)
{
	if (!m_ReadIges)
		return NULL;
	if (!pView)
		return NULL;
	if (!pFileName)
		return NULL;
	return m_ReadIges(pView, pFileName);
}

void* CExtDll::ReadStep(void* pView, LPCTSTR pFileName)
{
	if (!m_ReadStep)
		return NULL;
	if (!pView)
		return NULL;
	if (!pFileName)
		return NULL;
	return m_ReadStep(pView, pFileName);
}

void* CExtDll::ReadStl(void* pView, LPCTSTR pFileName)
{
	if (!m_ReadStl)
		return NULL;
	if (!pView)
		return NULL;
	if (!pFileName)
		return NULL;
	return m_ReadStl(pView, pFileName);
}

void CExtDll::DeleteModel(void* pView, void* pModel)
{
	if (!m_DeleteModel)
		return;
	if (!pView)
		return;
	if (!pModel)
		return;
	m_DeleteModel(pView, pModel);
}

void CExtDll::DrawModel(void* pView, void* pModel)
{
	if (!m_DrawModel)
		return;
	if (!pView)
		return;
	if (!pModel)
		return;
	m_DrawModel(pView, pModel);
}

void CExtDll::RemoveModel(void* pView, void* pModel)
{
	if (!m_RemoveModel)
		return;
	if (!pView)
		return;
	if (!pModel)
		return;
	m_RemoveModel(pView, pModel);
}