#pragma once

typedef void*	(__stdcall* NEW_VIEW)();
typedef void	(__stdcall* DELETE_VIEW)(void *pView);
typedef void	(__stdcall* CREATE_CONTEXT)(void *pView);
typedef void	(__stdcall* SET_WINDOW)(void *pView, HWND hwnd);
typedef void	(__stdcall* ERASE_ALL_VIEW)(void *pView);
typedef void	(__stdcall* RESIZE)(void *pView);
typedef void	(__stdcall* UPDATE_CUR_VIEW)(void *pView);
typedef void	(__stdcall* DRAW_COORD_SYS)(void *pView);
typedef void	(__stdcall* DRAW_HORZ_PLANE)(void *pView);
typedef void	(__stdcall* VIEW_START_ROTATION)(void *pView, double dCenter[3], int iMouseX, int iMouseY);
typedef void	(__stdcall* VIEW_ROTATION)(void *pView, int iMouseX, int iMouseY);
typedef void	(__stdcall* VIEW_PAN)(void* pView, int iPanningX, int iPanningY);
typedef void	(__stdcall* VIEW_ZOOM)(void* pView, int iMouseX, int iMouseY, double dZoomFactor);
typedef void*	(__stdcall* READ_IGES)(void* pView, const char* pcFileName);
typedef void*	(__stdcall* READ_STEP)(void* pView, const char* pcFileName);
typedef void*   (__stdcall* READ_STL)(void* pView, const char* pcFileName);
typedef void    (__stdcall* DELETE_MODEL)(void* pView, void* pModel);


class CExtDll
{
public:
	CExtDll();
	~CExtDll();

	HINSTANCE		m_hDriver;

	NEW_VIEW		m_NewView;
	DELETE_VIEW		m_DeleteView;

	// Create the view
	CREATE_CONTEXT	m_CreateContext;
	SET_WINDOW		m_SetWindow;
	ERASE_ALL_VIEW	m_EraseAllView;
	RESIZE			m_Resize;
	UPDATE_CUR_VIEW	m_UpdateCurView;

	// Draw the background
	DRAW_COORD_SYS	m_DrawCoordSys;
	DRAW_HORZ_PLANE	m_DrawHorzPlane;

	// Manipulate the camera
	VIEW_START_ROTATION	m_ViewStartRotation;
	VIEW_ROTATION	m_ViewRotation;
	VIEW_PAN		m_ViewPan;
	VIEW_ZOOM		m_ViewZoom;

	// Model
	READ_IGES		m_ReadIges;
	READ_STEP		m_ReadStep;
	READ_STL		m_ReadStl;
	DELETE_MODEL	m_DeleteModel;

	void LoadDriver();
	void* NewView();
	void DeleteView(void *pView);

	// Create the view
	void CreateContext(void *pView);
	void SetWindow(void *pView, HWND hwnd);
	void EraseAllView(void *pView);
	void Resize(void *pView);
	void UpdateCurrentViewer(void *pView);

	// Draw the background
	void DrawCoordSys(void *pView);
	void DrawHorzPlane(void *pView);

	// Manipulate the camera
	void ViewStartRotation(void *pView, double dCenter[3], int iMouseX, int iMouseY);
	void ViewRotation(void *pView, int iMouseX, int iMouseY);
	void ViewPan(void* pView, int iPanningX, int iPanningY);
	void ViewZoom(void* pView, int iMouseX, int iMouseY, double dZoomFactor);

	// Model
	void* ReadIges(void* pView, const char* pcFileName);
	void* ReadStep(void* pView, const char* pcFileName);
	void* ReadStl(void* pView, const char* pcFileName);
	void DeleteModel(void* pView, void* pModel);
};

