#include "pch.h"
#include "OCCView.h"

#define COORD_SYS_LENGTH 10000.0
#define COORD_SYS_WIDTH	3.0

#define DIST_RATIO 0.32


COCCView::COCCView()
{
	// Create the view
	m_hGraphicDriver.Nullify();
	m_hViewer.Nullify();
	m_hView.Nullify();
	m_hContext.Nullify();
	m_hWindow.Nullify();

	// Draw the background
	for (int i = 0; i < 3; i++)
		m_hAISAxis[i].Nullify();
}

COCCView::~COCCView()
{
	// Remove the view
	if (!m_hView.IsNull())
		m_hView->Remove();
	if (!m_hViewer.IsNull())
		m_hViewer->Remove();

	// Remove the array of model
	for (vector<Handle(AIS_Shape)>::iterator it = m_vecAISModel.begin(); it != m_vecAISModel.end(); it++)
		if (!(*it).IsNull())
			m_hContext->Remove(*it, Standard_False);
	vector<Handle(AIS_Shape)>().swap(m_vecAISModel);
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
	m_hView->SetImmediateUpdate(Standard_False);

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
		m_hContext->Display(m_hAISAxis[i], AIS_Shaded, -1, Standard_False);
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

	m_hContext->Display(m_hAISHorzPlane, AIS_Shaded, -1, Standard_False);
	m_hAISHorzPlane->SetInfiniteState(Standard_True);
}

gp_Pln GetNearPlane(Handle(V3d_View) hView, gp_Pnt ptCenter)
{
	Standard_Real rEyeX, rEyeY, rEyeZ;
	hView->Eye(rEyeX, rEyeY, rEyeZ);
	gp_Pnt ptEye(rEyeX, rEyeY, rEyeZ);

	Standard_Real rProjX, rProjY, rProjZ;
	hView->Proj(rProjX, rProjY, rProjZ);
	gp_Dir dirProj(rProjX, rProjY, rProjZ);

	gp_Vec vecView(ptCenter, ptEye);
	Standard_Real dDist = vecView.Magnitude() * DIST_RATIO;

	gp_Pnt ptNear;
	ptNear.SetX(ptCenter.X() + dirProj.X() * dDist);
	ptNear.SetY(ptCenter.Y() + dirProj.Y() * dDist);
	ptNear.SetZ(ptCenter.Z() + dirProj.Z() * dDist);

	return gp_Pln(ptNear, dirProj);
}

Standard_Boolean ConvertClickToPoint(Standard_Integer iMouseX, Standard_Integer iMouseY, gp_Pln plnInt, Handle(V3d_View) hView, gp_Pnt& ptResult)
{
	Standard_Real rEyeX, rEyeY, rEyeZ;
	hView->Eye(rEyeX, rEyeY, rEyeZ);
	gp_Pnt ptEye(rEyeX, rEyeY, rEyeZ);

	Standard_Real rProjX, rProjY, rProjZ;
	hView->Proj(rProjX, rProjY, rProjZ);

	Standard_Real rFarX, rFarY, rFarZ;
	hView->Convert(iMouseX, iMouseY, rFarX, rFarY, rFarZ);
	gp_Pnt ptFar(rFarX, rFarY, rFarZ);

	Handle(Geom_Line) pGeomLine;
	switch (hView->Type())
	{
	case V3d_ORTHOGRAPHIC:
		pGeomLine = new Geom_Line(ptFar, gp_Dir(rProjX, rProjY, rProjZ));
		break;
	case V3d_PERSPECTIVE:
		pGeomLine = new Geom_Line(ptEye, gp_Dir(gp_Vec(ptEye, ptFar)));
		break;
	default:
		return Standard_False;
	}

	Handle(Geom_Plane) pGeomPlane = new Geom_Plane(plnInt);
	GeomAPI_IntCS intCS(pGeomLine, pGeomPlane);
	if (intCS.IsDone() && intCS.NbPoints() == 1)
	{
		ptResult = intCS.Point(1);
		return Standard_True;
	}
	else
		return Standard_False;
}

void COCCView::ViewReset()
{
	m_hView->Reset(Standard_False);
}

Standard_Boolean COCCView::ViewConvert(Standard_Integer iMouseX, Standard_Integer iMouseY, gp_Pnt ptRef, gp_Pnt& ptResult)
{
	gp_Pln plnNear = GetNearPlane(m_hView, ptRef);
	return ConvertClickToPoint(iMouseX, iMouseY, plnNear, m_hView, ptResult);
}

void COCCView::ViewRotate(gp_Pnt ptCenter, gp_Dir dirAxis, Standard_Real dAngle)
{
	m_hView->SetAxis(ptCenter.X(), ptCenter.Y(), ptCenter.Z(), 
		dirAxis.X(), dirAxis.Y(), dirAxis.Z());
	m_hView->Rotate(dAngle);
}

void COCCView::ViewPan(Standard_Integer iPanningX, Standard_Integer iPanningY)
{
	m_hView->Pan(iPanningX, iPanningY);
}

void COCCView::ViewZoom(Standard_Integer iMouseX, Standard_Integer iMouseY, Standard_Real dZoomFactor)
{
	// Calculate zoom coefficient
	Standard_Real rZoomCoef = dZoomFactor;
	if (m_hView->Type() == V3d_PERSPECTIVE)
		if (rZoomCoef < 1.)
		{
			Standard_Real rDelta = 1 / rZoomCoef - 1;
			rZoomCoef = (1 + rDelta) / (1 + 2 * rDelta);
		}


	// Calculate zoom point
	Standard_Real rAtX, rAtY, rAtZ;
	m_hView->At(rAtX, rAtY, rAtZ);
	gp_Pnt ptAt(rAtX, rAtY, rAtZ);

	Standard_Real rProjX, rProjY, rProjZ;
	m_hView->Proj(rProjX, rProjY, rProjZ);
	gp_Dir dirProj(rProjX, rProjY, rProjZ);

	gp_Pln plnView(ptAt, dirProj);
	gp_Pnt ptMouse;
	ConvertClickToPoint(iMouseX, iMouseY, plnView, m_hView, ptMouse);
	gp_Vec vecMouse(ptAt, ptMouse);


	// Calculate panning distance
	gp_Pnt ptOrg;
	ConvertClickToPoint(0, 0, plnView, m_hView, ptOrg);
	gp_Pnt ptX;
	ConvertClickToPoint(1, 0, plnView, m_hView, ptX);
	gp_Pnt ptY;
	ConvertClickToPoint(0, 1, plnView, m_hView, ptY);
	gp_Dir dirX(gp_Vec(ptOrg, ptX));
	gp_Dir dirY(gp_Vec(ptY, ptOrg));

	Standard_Real rDX = vecMouse.Dot(dirX);
	Standard_Real rDY = vecMouse.Dot(dirY);

	Standard_Real rPixel2MM = m_hView->Convert(1);
	Standard_Integer iDX = (Standard_Integer)Round(rDX / rPixel2MM);
	Standard_Integer iDY = (Standard_Integer)Round(rDY / rPixel2MM);

	Standard_Real rPanningX = iDX * rPixel2MM * (1. / rZoomCoef - 1.);
	Standard_Real rPanningY = iDY * rPixel2MM * (1. / rZoomCoef - 1.);


	// Zoom
	Standard_Real rOldDepth = m_hView->Depth();
	m_hView->Panning(rPanningX, rPanningY);
	m_hView->SetZoom(rZoomCoef);

	Standard_Real rOffset = m_hView->Depth() - rOldDepth;
	m_hView->At(rAtX, rAtY, rAtZ);
	rAtX += rOffset * rProjX;
	rAtY += rOffset * rProjY;
	rAtZ += rOffset * rProjZ;
	m_hView->SetAt(rAtX, rAtY, rAtZ);
}

void COCCView::ViewSetProj(gp_Dir dirProj)
{
	m_hView->SetProj(dirProj.X(), dirProj.Y(), dirProj.Z());
}

void* COCCView::ReadIges(LPCTSTR pcFileName)
{
	IGESControl_Reader igesReader;
	TCollection_AsciiString ascFileName(pcFileName);
	IFSelect_ReturnStatus status = igesReader.ReadFile(ascFileName.ToCString());
	if (status != IFSelect_RetDone)
		return NULL;

	// Root transfers
	igesReader.TransferRoots();

	// Collect result entities
	TopoDS_Shape shpLoad = igesReader.OneShape();
	if (shpLoad.IsNull())
		return NULL;

	Handle (AIS_Shape) hAISShape = new AIS_Shape(shpLoad);
	m_vecAISModel.push_back(hAISShape);

	
	return (Standard_Address)hAISShape.operator->();
}

void* COCCView::ReadStep(LPCTSTR pcFileName)
{
	STEPControl_Reader stepReader;
	TCollection_AsciiString ascFileName(pcFileName);
	IFSelect_ReturnStatus status = stepReader.ReadFile(ascFileName.ToCString());
	if (status != IFSelect_RetDone)
		return NULL;

	// Root transfers
	stepReader.TransferRoots();

	// Collect result entities
	TopoDS_Shape shpLoad = stepReader.OneShape();
	if (shpLoad.IsNull())
		return NULL;

	Handle (AIS_Shape) hAISShape = new AIS_Shape(shpLoad);
	m_vecAISModel.push_back(hAISShape);

	return (Standard_Address)hAISShape.operator->();
}

Handle(Poly_Triangulation) RegenNode(Handle(Poly_Triangulation) pMesh)
{
	if (pMesh.IsNull())
		return NULL;

	Standard_Integer iNbTriangles = pMesh->NbTriangles();
	Handle(Poly_Triangulation) pNewMesh = new Poly_Triangulation(3 * iNbTriangles, iNbTriangles, Standard_False);

#pragma omp parallel for
	for (Standard_Integer i = 1; i <= iNbTriangles; i++)
	{
		Poly_Triangle triangle = pMesh->Triangle(i);

		pNewMesh->SetNode(3 * i - 2, pMesh->Node(triangle.Value(1)));
		pNewMesh->SetNode(3 * i - 1, pMesh->Node(triangle.Value(2)));
		pNewMesh->SetNode(3 * i, pMesh->Node(triangle.Value(3)));

		pNewMesh->SetTriangle(i, Poly_Triangle(3 * i - 2, 3 * i - 1, 3 * i));
	}

	return pNewMesh;
}

void* COCCView::ReadStl(LPCTSTR pcFileName)
{
	TCollection_AsciiString ascFileName(pcFileName);
	Handle(Poly_Triangulation) pMesh = RWStl::ReadAscii(ascFileName);
	if (pMesh.IsNull())
		pMesh = RWStl::ReadBinary(ascFileName);
	if (pMesh.IsNull())
		return NULL;

	Handle(Poly_Triangulation) pNewMesh = RegenNode(pMesh);
	if (pNewMesh.IsNull())
		return NULL;

	TopoDS_Face faceSTL;
	BRep_Builder builderTool;
	builderTool.MakeFace(faceSTL, pNewMesh);
	if (faceSTL.IsNull())
		return NULL;

	Handle (AIS_Shape) hAISShape = new AIS_Shape(faceSTL);
	m_vecAISModel.push_back(hAISShape);

	return (Standard_Address)hAISShape.operator->();
}

void* COCCView::ReadObj(LPCTSTR pcFileName)
{
	/*
	Standard_Integer iLength = lstrlen(pcFileName);
	Standard_CString csFileName = new Standard_Character[iLength + 1];
	WideCharToMultiByte(CP_ACP, 0, pcFileName, -1, (char*)csFileName, iLength + 1, NULL, NULL);
	Handle(Poly_Triangulation) pMesh = RWObj::ReadFile(csFileName);
	delete[] csFileName;
	*/
	RWObj_TriangulationReader reader;
	reader.SetCreateShapes(Standard_False);
	reader.Read(TCollection_AsciiString(pcFileName), Message_ProgressRange());
	Handle(Poly_Triangulation) pMesh = reader.GetTriangulation();
	if (pMesh.IsNull())
		return NULL;
	
	TopoDS_Face faceOBJ;
	BRep_Builder builderTool;
	builderTool.MakeFace(faceOBJ, pMesh);
	if (faceOBJ.IsNull())
		return NULL;

	Handle (AIS_Shape) hAISShape = new AIS_Shape(faceOBJ);
	m_vecAISModel.push_back(hAISShape);

	return (Standard_Address)hAISShape.operator->();
}

void COCCView::DeleteModel(Handle(AIS_Shape) hAISShape)
{
	if (hAISShape.IsNull())
		return;

	m_hContext->Remove(hAISShape, Standard_False);
	m_vecAISModel.erase(remove(m_vecAISModel.begin(), m_vecAISModel.end(), hAISShape), m_vecAISModel.end());
}

void COCCView::DisplayModel(Handle(AIS_Shape) hAISShape)
{
	if (hAISShape.IsNull())
		return;

	m_hContext->Display(hAISShape, Standard_False);
}

void COCCView::RemoveModel(Handle(AIS_Shape) hAISShape)
{
	if (hAISShape.IsNull())
		return;

	m_hContext->Remove(hAISShape, Standard_False);
}

// Mouse Event
void COCCView::InputEvent(Standard_Integer iMouseX, Standard_Integer iMouseY)
{
	if (m_hContext.IsNull())
		return;

	m_hContext->MoveTo(iMouseX, iMouseY, m_hView, Standard_False);
	m_hContext->SelectDetected();
}

void COCCView::ClearSelected()
{
	if (m_hContext.IsNull())
		return;

	m_hContext->ClearSelected(Standard_False);
}

void COCCView::GetSelected(vector<Standard_Address>& vecSelected)
{
	vecSelected.clear();
	if (m_hContext.IsNull())
		return;

	Handle(AIS_InteractiveObject) hSelectedObject;
	for (m_hContext->InitSelected(); m_hContext->MoreSelected(); m_hContext->NextSelected())
	{
		hSelectedObject = m_hContext->SelectedInteractive();
		if (hSelectedObject.IsNull() || !hSelectedObject->IsKind(STANDARD_TYPE(AIS_Shape)))
			continue;

		vecSelected.push_back((Standard_Address)hSelectedObject.operator->());
	}
}