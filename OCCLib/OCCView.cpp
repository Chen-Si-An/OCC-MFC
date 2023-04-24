#include "pch.h"
#include "OCCView.h"

COCCView::COCCView()
{
	m_hGraphicDriver.Nullify();
	m_hViewer.Nullify();
	m_hView.Nullify();
	m_hContext.Nullify();
	m_hWindow.Nullify();
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

void COCCView::Redraw()
{
	if (!m_hView.IsNull())
		m_hView->Redraw();
}