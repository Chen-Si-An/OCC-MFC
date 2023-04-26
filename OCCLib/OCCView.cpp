#include "pch.h"
#include "OCCView.h"

#define COORD_SYS_LENGTH 10000.0
#define COORD_SYS_WIDTH	3.0

Standard_Boolean ConvertClickToPoint(Standard_Integer iMouseX, Standard_Integer iMouseY, gp_Pln plnInt, Handle(V3d_View) hView, gp_Pnt& pntResult)
{
	Standard_Real rEyeX, rEyeY, rEyeZ;
	hView->Eye(rEyeX, rEyeY, rEyeZ);
	gp_Pnt pntEye(rEyeX, rEyeY, rEyeZ);

	Standard_Real rProjX, rProjY, rProjZ;
	hView->Proj(rProjX, rProjY, rProjZ);

	Standard_Real rFarX, rFarY, rFarZ;
	hView->Convert(iMouseX, iMouseY, rFarX, rFarY, rFarZ);
	gp_Pnt pntFar(rFarX, rFarY, rFarZ);

	Handle(Geom_Line) pGeomLine;

	switch (hView->Type())
	{
	case V3d_ORTHOGRAPHIC:
		pGeomLine = new Geom_Line(pntFar, gp_Dir(rProjX, rProjY, rProjZ));
		break;
	case V3d_PERSPECTIVE:
		pGeomLine = new Geom_Line(pntEye, gp_Dir(gp_Vec(pntEye, pntFar)));
		break;
	default:
		return Standard_False;
	}

	Handle(Geom_Plane) pGeomPlane = new Geom_Plane(plnInt);
	GeomAPI_IntCS intCS(pGeomLine, pGeomPlane);
	if (intCS.IsDone() && intCS.NbPoints() == 1)
	{
		pntResult = intCS.Point(1);
		return Standard_True;
	}
	else
		return Standard_False;
}


COCCView::COCCView()
{
	//Create the view
	m_hGraphicDriver.Nullify();
	m_hViewer.Nullify();
	m_hView.Nullify();
	m_hContext.Nullify();
	m_hWindow.Nullify();

	//Draw the background
	for (int i = 0; i < 3; i++)
		m_hAISAxis[i].Nullify();
}

COCCView::~COCCView()
{
	if (!m_hView.IsNull())
		m_hView->Remove();
	if (!m_hViewer.IsNull())
		m_hViewer->Remove();
}

void COCCView::CreateContext()
{
	if (!m_hContext.IsNull())
		return;

	try
	{
		Handle(Aspect_DisplayConnection) pDisplayConnection;
		m_hGraphicDriver = new OpenGl_GraphicDriver(pDisplayConnection);
	}
	catch (Standard_Failure)
	{
		AfxMessageBox(_T("Fatal error during graphic initialization"), MB_ICONSTOP);
		ExitProcess(1);
	}

	m_hViewer = new V3d_Viewer(m_hGraphicDriver);
	m_hViewer->SetDefaultLights();
	m_hViewer->SetLightOn();

	m_hView = m_hViewer->CreateView();
	m_hView->SetComputedMode(Standard_False);
	m_hView->SetBackgroundColor(Quantity_Color(1., 1., 1., Quantity_TOC_RGB));
	m_hView->Camera()->SetProjectionType(Graphic3d_Camera::Projection_Perspective);
	m_hView->SetProj(V3d_Zpos);
	m_hView->SetZoom(3.33);
	m_hView->SetViewMappingDefault();

	m_hContext = new AIS_InteractiveContext(m_hViewer);
	m_hContext->SetDisplayMode(AIS_Shaded, Standard_False);
	m_hContext->SetAutomaticHilight(Standard_False);
}

void COCCView::SetWindow(HWND hwnd)
{
	m_hWindow = new WNT_Window(hwnd);

	if (!m_hView.IsNull())
		m_hView->SetWindow(m_hWindow);

	if (!m_hWindow->IsMapped())
		m_hWindow->Map();
}

void COCCView::EraseAllView()
{
	if (!m_hContext.IsNull())
		m_hContext->EraseAll(Standard_False);
}

void COCCView::Resize()
{
	if (!m_hView.IsNull())
		m_hView->MustBeResized();
}

void COCCView::UpdateCurrentViewer()
{
	if (!m_hContext.IsNull())
		m_hContext->UpdateCurrentViewer();
}

void COCCView::DrawCoordSys()
{
	if (m_hAISAxis[0].IsNull())
	{
		m_hAISAxis[0] = new AIS_Shape(TopoDS_Shape());

		BRepBuilderAPI_MakeEdge aMakeEdge(gp_Pnt(0, 0, 0), gp_Pnt(COORD_SYS_LENGTH, 0, 0));
		m_hAISAxis[0]->Set(aMakeEdge.Edge());
		m_hAISAxis[0]->SetDisplayMode(AIS_Shaded);
		m_hAISAxis[0]->SetColor(Quantity_NOC_RED);
		m_hAISAxis[0]->SetWidth(COORD_SYS_WIDTH);
	}
	if (m_hAISAxis[1].IsNull())
	{
		m_hAISAxis[1] = new AIS_Shape(TopoDS_Shape());

		BRepBuilderAPI_MakeEdge aMakeEdge(gp_Pnt(0, 0, 0), gp_Pnt(0, COORD_SYS_LENGTH, 0));
		m_hAISAxis[1]->Set(aMakeEdge.Edge());
		m_hAISAxis[1]->SetDisplayMode(AIS_Shaded);
		m_hAISAxis[1]->SetColor(Quantity_NOC_GREEN);
		m_hAISAxis[1]->SetWidth(COORD_SYS_WIDTH);
	}
	if (m_hAISAxis[2].IsNull())
	{
		m_hAISAxis[2] = new AIS_Shape(TopoDS_Shape());

		BRepBuilderAPI_MakeEdge aMakeEdge(gp_Pnt(0, 0, 0), gp_Pnt(0, 0, COORD_SYS_LENGTH));
		m_hAISAxis[2]->Set(aMakeEdge.Edge());
		m_hAISAxis[2]->SetDisplayMode(AIS_Shaded);
		m_hAISAxis[2]->SetColor(Quantity_NOC_BLUE);
		m_hAISAxis[2]->SetWidth(COORD_SYS_WIDTH);
	}


	for (int i = 0; i < 3; i++)
	{
		m_hContext->Display(m_hAISAxis[i], Standard_False);
		m_hAISAxis[i]->SetInfiniteState(Standard_True);
	}
}

void COCCView::DrawHorzPlane()
{
	if (m_hAISHorzPlane.IsNull())
	{
		m_hAISHorzPlane = new AIS_Shape(TopoDS_Shape());

		BRepBuilderAPI_MakeFace aMakeFace(gp_Pln(gp_Pnt(0, 0, 0), gp_Dir(0, 0, 1)), -COORD_SYS_LENGTH, COORD_SYS_LENGTH, -COORD_SYS_LENGTH, COORD_SYS_LENGTH);
		m_hAISHorzPlane->Set(aMakeFace.Face());
		m_hAISHorzPlane->SetDisplayMode(AIS_Shaded);
		m_hAISHorzPlane->SetColor(Quantity_NOC_GRAY);
		m_hAISHorzPlane->SetMaterial(Graphic3d_NOM_PLASTIC);
		m_hAISHorzPlane->SetTransparency(0.5f);
	}

	m_hContext->Display(m_hAISHorzPlane, Standard_False);
	m_hAISHorzPlane->SetInfiniteState(Standard_True);
}