#pragma once

#include <vector>
using namespace std;

typedef void*	(__stdcall* NEW_VIEW)();
typedef void	(__stdcall* DELETE_VIEW)(void *pView);
typedef void	(__stdcall* CREATE_CONTEXT)(void *pView);
typedef void	(__stdcall* SET_WINDOW)(void *pView, HWND hwnd);
typedef void	(__stdcall* ERASE_VIEW)(void *pView);
typedef void	(__stdcall* RESIZE)(void *pView);
typedef void	(__stdcall* UPDATE_VIEW)(void *pView);
typedef void	(__stdcall* DRAW_COORD_SYS)(void *pView);
typedef void	(__stdcall* DRAW_HORZ_PLANE)(void *pView);
typedef bool	(__stdcall* VIEW_RESET)(void *pView);
typedef bool	(__stdcall* VIEW_CONVERT)(void *pView, int iMouseX, int iMouseY, double dCenter[3], double dResult[3]);
typedef void    (__stdcall* VIEW_ROTATE)(void *pView, double dCenter[3], double dAxis[3], double dAngle);
typedef void	(__stdcall* VIEW_PAN)(void* pView, int iPanningX, int iPanningY);
typedef void	(__stdcall* VIEW_ZOOM)(void* pView, int iMouseX, int iMouseY, double dZoomFactor);
typedef void	(__stdcall* VIEW_SET_PROJ)(void* pView, double dProjX, double dProjY, double dProjZ);
typedef void*	(__stdcall* READ_IGES)(void* pView, LPCTSTR pcFileName);
typedef void*	(__stdcall* READ_STEP)(void* pView, LPCTSTR pcFileName);
typedef void*   (__stdcall* READ_STL)(void* pView, LPCTSTR pcFileName);
typedef void	(__stdcall* DELETE_MODEL)(void* pView, void* pModel);
typedef void	(__stdcall* DRAW_MODEL)(void* pView, void* pModel);
typedef void	(__stdcall* REMOVE_MODEL)(void* pView, void* pModel);
typedef void	(__stdcall* SET_MODEL_COLOR)(void* pModel, double dColor[3]);
typedef bool    (__stdcall* GET_MODEL_CENTER)(void* pModel, double dCenter[3]);
typedef bool    (__stdcall* GET_MODEL_MATRIX)(void* pModel, double dMatrix[4][4]);
typedef void    (__stdcall* SET_MODEL_MATRIX)(void* pModel, double dMatrix[4][4]);
typedef void	(__stdcall* SELECT)(void* pView, int iMouseX, int iMouseY);
typedef void	(__stdcall* CLEAR_SELECTED)(void* pView);
typedef void	(__stdcall* GET_SELECTED)(void* pView, vector<void*>& vecShape);


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
	ERASE_VIEW		m_EraseView;
	RESIZE			m_Resize;
	UPDATE_VIEW		m_UpdateView;

	// Draw the background
	DRAW_COORD_SYS	m_DrawCoordSys;
	DRAW_HORZ_PLANE	m_DrawHorzPlane;

	// Manipulate the camera
	VIEW_RESET		m_ViewReset;
	VIEW_CONVERT	m_ViewConvert;
	VIEW_ROTATE		m_ViewRotate;
	VIEW_PAN		m_ViewPan;
	VIEW_ZOOM		m_ViewZoom;
	VIEW_SET_PROJ	m_ViewSetProj;

	// Model
	READ_IGES		m_ReadIges;
	READ_STEP		m_ReadStep;
	READ_STL		m_ReadStl;
	DELETE_MODEL	m_DeleteModel;
	DRAW_MODEL		m_DrawModel;
	REMOVE_MODEL	m_RemoveModel;
	SET_MODEL_COLOR	m_SetModelColor;
	GET_MODEL_CENTER m_GetModelCenter;
	GET_MODEL_MATRIX m_GetModelMatrix;
	SET_MODEL_MATRIX m_SetModelMatrix;

	// Selection
	SELECT			m_Select;
	CLEAR_SELECTED	m_ClearSelected;
	GET_SELECTED	m_GetSelected;

	void LoadDriver();
	void FreeDriver();
	void* NewView();
	void DeleteView(void *pView);

	// Create the view
	void CreateContext(void *pView);
	void SetWindow(void *pView, HWND hwnd);
	void EraseView(void *pView);
	void Resize(void *pView);
	void UpdateView(void *pView);

	// Draw the background
	void DrawCoordSys(void *pView);
	void DrawHorzPlane(void *pView);

	// Manipulate the camera
	void ViewReset(void *pView);
	bool ViewConvert(void *pView, int iMouseX, int iMouseY, double dCenter[3], double dResult[3]);
	void ViewRotate(void *pView, double dCenter[3], double dAxis[3], double dAngle);
	void ViewPan(void* pView, int iPanningX, int iPanningY);
	void ViewZoom(void* pView, int iMouseX, int iMouseY, double dZoomFactor);
	void ViewSetProj(void* pView, double dProjX, double dProjY, double dProjZ);

	// Model
	void* ReadIges(void* pView, LPCTSTR pcFileName);
	void* ReadStep(void* pView, LPCTSTR pcFileName);
	void* ReadStl(void* pView, LPCTSTR pcFileName);
	void DeleteModel(void* pView, void* pModel);
	void DrawModel(void* pView, void* pModel);
	void RemoveModel(void* pView, void* pModel);
	void SetModelColor(void* pModel, double dColor[3]);
	bool GetModelCenter(void* pModel, double dCenter[3]);
	bool GetModelMatrix(void* pModel, double dMatrix[4][4]);
	void SetModelMatrix(void* pModel, double dMatrix[4][4]);

	// Selection
	void Select(void* pView, int iMouseX, int iMouseY);
	void ClearSelected(void* pView);
	void GetSelected(void* pView, vector<void*>& vecSelected);
};

