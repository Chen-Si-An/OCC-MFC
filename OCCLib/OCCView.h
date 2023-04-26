#pragma once

#include <OpenGl_GraphicDriver.hxx>
#include <V3d_Viewer.hxx>
#include <V3d_View.hxx>
#include <AIS_InteractiveContext.hxx>
#include <WNT_Window.hxx>

#include <Geom_Line.hxx>
#include <Geom_Plane.hxx>

#include <GeomAPI_IntCS.hxx>

#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>

#include <AIS_Shape.hxx>

#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>

class COCCView
{
public:
	//Create the view
	Handle(Graphic3d_GraphicDriver) m_hGraphicDriver;
	Handle(V3d_Viewer) m_hViewer;
	Handle(V3d_View) m_hView;
	Handle(AIS_InteractiveContext) m_hContext;
	Handle(WNT_Window) m_hWindow;

	//Draw the background
	Handle(AIS_Shape) m_hAISAxis[3];
	Handle(AIS_Shape) m_hAISHorzPlane;

public:
	COCCView();
	virtual ~COCCView();
	
	//Create the view
	void CreateContext();
	void SetWindow(HWND hwnd);
	void EraseAllView();
	void Resize();
	void UpdateCurrentViewer();

	//Draw the background
	void DrawCoordSys();
	void DrawHorzPlane();
};

