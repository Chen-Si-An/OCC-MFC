#pragma once

#include <OpenGl_GraphicDriver.hxx>
#include <V3d_Viewer.hxx>
#include <V3d_View.hxx>
#include <AIS_InteractiveContext.hxx>
#include <WNT_Window.hxx>

class COCCView
{
public:
	Handle(Graphic3d_GraphicDriver) m_hGraphicDriver;
	Handle(V3d_Viewer) m_hViewer;
	Handle(V3d_View) m_hView;
	Handle(AIS_InteractiveContext) m_hContext;
	Handle(WNT_Window) m_hWindow;

public:
	COCCView();
	virtual ~COCCView();
	
	//Create the view
	void CreateContext();
	void SetWindow(HWND hwnd);
	void Redraw();
};

