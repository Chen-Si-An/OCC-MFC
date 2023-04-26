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


class CExtDll
{
public:
	CExtDll();
	~CExtDll();

	HINSTANCE		m_hDriver;

	NEW_VIEW		m_NewView;
	DELETE_VIEW		m_DeleteView;

	//Create the view
	CREATE_CONTEXT	m_CreateContext;
	SET_WINDOW		m_SetWindow;
	ERASE_ALL_VIEW	m_EraseAllView;
	RESIZE			m_Resize;
	UPDATE_CUR_VIEW	m_UpdateCurView;

	//Draw the background
	DRAW_COORD_SYS	m_DrawCoordSys;
	DRAW_HORZ_PLANE	m_DrawHorzPlane;

	void LoadDriver();
	void* NewView();
	void DeleteView(void *pView);

	//Create the view
	void CreateContext(void *pView);
	void SetWindow(void *pView, HWND hwnd);
	void EraseAllView(void *pView);
	void Resize(void *pView);
	void UpdateCurrentViewer(void *pView);

	//Draw the background
	void DrawCoordSys(void *pView);
	void DrawHorzPlane(void *pView);

	//Manipulate the camera
};

