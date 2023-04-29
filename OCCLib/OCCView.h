#pragma once

// Open Cascade includes
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

#include <BRepBndLib.hxx>
#include <BRep_Builder.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>

#include <IGESControl_Reader.hxx>
#include <STEPControl_Reader.hxx>
#include <RWStl.hxx>

// Standard includes
#include <vector>
#include <algorithm>
using namespace std;

class COCCView
{
public:
	// Create the view
	Handle(Graphic3d_GraphicDriver) m_hGraphicDriver;
	Handle(V3d_Viewer) m_hViewer;
	Handle(V3d_View) m_hView;
	Handle(AIS_InteractiveContext) m_hContext;
	Handle(WNT_Window) m_hWindow;

	// Draw the background
	Handle(AIS_Shape) m_hAISAxis[3];
	Handle(AIS_Shape) m_hAISHorzPlane;

	// Model
	vector<Handle(AIS_Shape)> m_vecAISModel;

public:
	COCCView();
	virtual ~COCCView();
	
	// Create the view
	void CreateContext();
	void SetWindow(HWND hwnd);
	void EraseAllView();
	void Resize();
	void UpdateCurrentViewer();

	// Draw the background
	void DrawCoordSys();
	void DrawHorzPlane();

	// Manipulate the camera
	Standard_Boolean ViewConvert(Standard_Integer iMouseX, Standard_Integer iMouseY, gp_Pnt ptRef, gp_Pnt& ptResult);
	void ViewRotate(gp_Pnt ptCenter, gp_Dir dirAxis, Standard_Real dAngle);
	void ViewPan(Standard_Integer iPanningX, Standard_Integer iPanningY);
	void ViewZoom(Standard_Integer iMouseX, Standard_Integer iMouseY, Standard_Real dZoomFactor);

	// Model
	void* ReadIges(LPCTSTR pcFileName);
	void* ReadStep(LPCTSTR pcFileName);
	void* ReadStl(LPCTSTR pcFileName);
	void DeleteModel(Handle(AIS_Shape) hAISShape);
	void DisplayModel(Handle(AIS_Shape) hAISShape);
	void RemoveModel(Handle(AIS_Shape) hAISShape);

	// Mouse event
	void InputEvent(Standard_Integer iMouseX, Standard_Integer iMouseY);
	void ClearSelected();
	void GetSelected(vector<Standard_Address>& vecSelected);
};

