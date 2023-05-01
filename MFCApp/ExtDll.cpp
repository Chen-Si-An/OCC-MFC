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
	m_EraseView = NULL;
	m_Resize = NULL;
	m_UpdateView = NULL;

	//Draw the background
	m_DrawCoordSys = NULL;
	m_DrawHorzPlane = NULL;
	
	//Manipulate the camera
	m_ViewReset = NULL;
	m_ViewConvert = NULL;
	m_ViewRotate = NULL;
	m_ViewPan = NULL;
	m_ViewZoom = NULL;
	m_ViewSetProj = NULL;

	// Model
	m_ReadIges = NULL;
	m_ReadStep = NULL;
	m_ReadStl = NULL;
	m_DeleteModel = NULL;
	m_DrawModel = NULL;
	m_RemoveModel = NULL;
	m_SetModelColor = NULL;
	m_GetModelCenter = NULL;
	m_GetModelMesh = NULL;
	m_GetModelMatrix = NULL;
	m_SetModelMatrix = NULL;

	// Selection
	m_Select = NULL;
	m_ClearSelected = NULL;
	m_GetSelected = NULL;
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

	m_EraseView = (ERASE_VIEW)GetProcAddress(m_hDriver, "EraseAllView");
	if (!m_EraseView)
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

	m_UpdateView = (UPDATE_VIEW)GetProcAddress(m_hDriver, "UpdateCurrentViewer");
	if (!m_UpdateView)
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
	m_ViewReset = (VIEW_RESET)GetProcAddress(m_hDriver, "ViewReset");
	if (!m_ViewReset)
	{
		AfxMessageBox(_T("GetProcAddress Failed"));
		return;
	}

	m_ViewConvert = (VIEW_CONVERT)GetProcAddress(m_hDriver, "ViewConvert");
	if (!m_ViewConvert)
	{
		AfxMessageBox(_T("GetProcAddress Failed"));
		return;
	}

	m_ViewRotate = (VIEW_ROTATE)GetProcAddress(m_hDriver, "ViewRotate");
	if (!m_ViewRotate)
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

	m_ViewSetProj = (VIEW_SET_PROJ)GetProcAddress(m_hDriver, "ViewSetProj");
	if (!m_ViewSetProj)
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

	m_SetModelColor = (SET_MODEL_COLOR)GetProcAddress(m_hDriver, "SetModelColor");
	if (!m_SetModelColor)
	{
		AfxMessageBox(_T("GetProcAddress Failed"));
		return;
	}

	m_GetModelCenter = (GET_MODEL_CENTER)GetProcAddress(m_hDriver, "GetModelCenter");
	if (!m_GetModelCenter)
	{
		AfxMessageBox(_T("GetProcAddress Failed"));
		return;
	}

	m_GetModelMesh = (GET_MODEL_MESH)GetProcAddress(m_hDriver, "GetModelMesh");
	if (!m_GetModelMesh)
	{
		AfxMessageBox(_T("GetProcAddress Failed"));
		return;
	}

	m_GetModelMatrix = (GET_MODEL_MATRIX)GetProcAddress(m_hDriver, "GetLocalTransformation");
	if (!m_GetModelMatrix)
	{
		AfxMessageBox(_T("GetProcAddress Failed"));
		return;
	}

	m_SetModelMatrix = (SET_MODEL_MATRIX)GetProcAddress(m_hDriver, "SetLocalTransformation");
	if (!m_SetModelMatrix)
	{
		AfxMessageBox(_T("GetProcAddress Failed"));
		return;
	}

	// Selection
	m_Select = (SELECT)GetProcAddress(m_hDriver, "InputEvent");
	if (!m_Select)
	{
		AfxMessageBox(_T("GetProcAddress Failed"));
		return;
	}

	m_ClearSelected = (CLEAR_SELECTED)GetProcAddress(m_hDriver, "ClearSelected");
	if (!m_ClearSelected)
	{
		AfxMessageBox(_T("GetProcAddress Failed"));
		return;
	}

	m_GetSelected = (GET_SELECTED)GetProcAddress(m_hDriver, "GetSelected");
	if (!m_GetSelected)
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

void CExtDll::EraseView(void* pView)
{
	if (!m_EraseView)
		return;
	if (!pView)
		return;
	m_EraseView(pView);
}

void CExtDll::Resize(void* pView)
{
	if (!m_Resize)
		return;
	if (!pView)
		return;
	m_Resize(pView);
}

void CExtDll::UpdateView(void* pView)
{
	if (!m_UpdateView)
		return;
	if (!pView)
		return;
	m_UpdateView(pView);
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
void CExtDll::ViewReset(void* pView)
{
	if (!m_ViewReset)
		return;
	if (!pView)
		return;
	m_ViewReset(pView);
}

bool CExtDll::ViewConvert(void* pView, int iMouseX, int iMouseY, double dCenter[3], double dResult[3])
{
	if (!m_ViewConvert)
		return FALSE;
	if (!pView)
		return FALSE;
	return m_ViewConvert(pView, iMouseX, iMouseY, dCenter, dResult);
}

void CExtDll::ViewRotate(void* pView, double dCenter[3], double dAxis[3], double dAngle)
{
	if (!m_ViewRotate)
		return;
	if (!pView)
		return;
	m_ViewRotate(pView, dCenter, dAxis, dAngle);
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

void CExtDll::ViewSetProj(void* pView, double dProjX, double dProjY, double dProjZ)
{
	if (!m_ViewSetProj)
		return;
	if (!pView)
		return;
	m_ViewSetProj(pView, dProjX, dProjY, dProjZ);
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

void CExtDll::SetModelColor(void* pModel, double dColor[3])
{
	if (!m_SetModelColor)
		return;
	if (!pModel)
		return;
	m_SetModelColor(pModel, dColor);
}

bool CExtDll::GetModelCenter(void* pModel, double dCenter[3])
{
	if (!m_GetModelCenter)
		return false;
	if (!pModel)
		return false;
	m_GetModelCenter(pModel, dCenter);
	return true;
}

void CExtDll::GetModelMesh(void* pModel, vector<double>& vecMesh)
{
	if (!m_GetModelMesh)
		return;
	if (!pModel)
		return;
	m_GetModelMesh(pModel, vecMesh);
}

bool CExtDll::GetModelMatrix(void* pModel, double dMatrix[4][4])
{
	if (!m_GetModelMatrix)
		return false;
	if (!pModel)
		return false;
	m_GetModelMatrix(pModel, dMatrix);
	return true;
}

void CExtDll::SetModelMatrix(void* pModel, double dMatrix[4][4])
{
	if (!m_SetModelMatrix)
		return;
	if (!pModel)
		return;
	m_SetModelMatrix(pModel, dMatrix);
}

// Selection
void CExtDll::Select(void* pView, int iMouseX, int iMouseY)
{
	if (!m_Select)
		return;
	if (!pView)
		return;
	m_Select(pView, iMouseX, iMouseY);
}

void CExtDll::ClearSelected(void* pView)
{
	if (!m_ClearSelected)
		return;
	if (!pView)
		return;
	m_ClearSelected(pView);
}

void CExtDll::GetSelected(void* pView, vector<void*>& vecSelected)
{
	if (!m_GetSelected)
		return;
	if (!pView)
		return;
	m_GetSelected(pView, vecSelected);
}