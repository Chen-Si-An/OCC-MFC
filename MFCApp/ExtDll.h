#pragma once

typedef void*	(__stdcall* NEW_VIEW)();
typedef void	(__stdcall* DELETE_VIEW)(void *pView);
typedef void	(__stdcall* CREATE_CONTEXT)(void *pView);
typedef void	(__stdcall* SET_WINDOW)(void *pView, HWND hwnd);
typedef void	(__stdcall* REDRAW)(void *pView);


class CExtDll
{
public:
	CExtDll();
	~CExtDll();

	HINSTANCE		m_hDriver;

	NEW_VIEW		m_NewView;
	DELETE_VIEW		m_DeleteView;
	CREATE_CONTEXT	m_CreateContext;
	SET_WINDOW		m_SetWindow;
	REDRAW			m_Redraw;

	void LoadDriver();
	void* NewView();
	void DeleteView(void *pView);
	void CreateContext(void *pView);
	void SetWindow(void *pView, HWND hwnd);
	void Redraw(void *pView);
};

