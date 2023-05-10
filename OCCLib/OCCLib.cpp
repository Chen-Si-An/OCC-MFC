// OCCLib.cpp: 定義 DLL 的初始化常式。
//

#include "pch.h"
#include "framework.h"
#include "OCCLib.h"
#include "OCCView.h"
#include <vector>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO:  如果這個 DLL 是動態地對 MFC DLL 連結，
//		那麼從這個 DLL 匯出的任何會呼叫
//		MFC 內部的函式，都必須在函式一開頭加上 AFX_MANAGE_STATE
//		巨集。
//
//		例如: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此處為正常函式主體
//		}
//
//		這個巨集一定要出現在每一個
//		函式中，才能夠呼叫 MFC 的內部。這意味著
//		其必須是函式內的第一個陳述式
//		，甚至必須在任何物件變數宣告前面
//		，因為它們的建構函式可能會產生對 MFC
//		DLL 內部的呼叫。
//
//		請參閱 MFC 技術提示 33 和 58 中的
//		詳細資料。
//

// COCCLibApp

BEGIN_MESSAGE_MAP(COCCLibApp, CWinApp)
END_MESSAGE_MAP()


// COCCLibApp 建構

COCCLibApp::COCCLibApp()
{
	// TODO:  在此加入建構程式碼，
	// 將所有重要的初始設定加入 InitInstance 中
}


// 唯一一個 COCCLibApp 物件

COCCLibApp theApp;
vector<COCCView*> m_ayOCCView;


// COCCLibApp 初始化

BOOL COCCLibApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

extern "C" void* PASCAL EXPORT NewView()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	COCCView *pOCCView = new COCCView;
	m_ayOCCView.push_back(pOCCView);

	return (void*)pOCCView;
}

extern "C" void PASCAL EXPORT DeleteView(void *pView)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!pView)
		return;

	COCCView *pOCCView = (COCCView*)pView;
	int iSize = (int)m_ayOCCView.size();
	for (int i = 0; i < iSize; i++)
	{
		if (pOCCView == m_ayOCCView[i])
		{
			m_ayOCCView.erase(m_ayOCCView.begin() + i);
			delete pOCCView;

			break;
		}
	}
}

// Create the view
extern "C" void PASCAL EXPORT CreateContext(void *pView)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	COCCView *pOCCView = (COCCView*)pView;
	if (!pOCCView)
		return;

	return pOCCView->CreateContext();
}

extern "C" void PASCAL EXPORT SetWindow(void *pView, HWND hwnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	COCCView *pOCCView = (COCCView*)pView;
	if (!pOCCView)
		return;

	return pOCCView->SetWindow(hwnd);
}

extern "C" void PASCAL EXPORT EraseAllView(void* pView)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	COCCView* pOCCView = (COCCView*)pView;
	if (!pOCCView)
		return;

	return pOCCView->EraseAllView();
}

extern "C" void PASCAL EXPORT Resize(void* pView)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	COCCView *pOCCView = (COCCView*)pView;
	if (!pOCCView)
		return;

	return pOCCView->Resize();
}

extern "C" void PASCAL EXPORT UpdateCurrentViewer(void* pView)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	COCCView* pOCCView = (COCCView*)pView;
	if (!pOCCView)
		return;

	return pOCCView->UpdateCurrentViewer();
}

// Draw the background
extern "C" void PASCAL EXPORT DrawCoordSys(void* pView)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	COCCView* pOCCView = (COCCView*)pView;
	if (!pOCCView)
		return;

	return pOCCView->DrawCoordSys();
}

extern "C" void PASCAL EXPORT DrawHorzPlane(void* pView)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	COCCView* pOCCView = (COCCView*)pView;
	if (!pOCCView)
		return;

	return pOCCView->DrawHorzPlane();
}

// Manipulate the camera
extern "C" void PASCAL EXPORT ViewReset(void* pView)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	COCCView* pOCCView = (COCCView*)pView;
	if (!pOCCView)
		return;

	return pOCCView->ViewReset();
}

extern "C" bool PASCAL EXPORT ViewConvert(void* pView, int iMouseX, int iMouseY, double dCenter[3], double dResult[3])
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	COCCView* pOCCView = (COCCView*)pView;
	if (!pOCCView)
		return false;

	gp_Pnt ptCenter(dCenter[0], dCenter[1], dCenter[2]);
	gp_Pnt ptResult;
	pOCCView->ViewConvert(iMouseX, iMouseY, ptCenter, ptResult);

	dResult[0] = ptResult.X();
	dResult[1] = ptResult.Y();
	dResult[2] = ptResult.Z();
	
	return true;
}

extern "C" void PASCAL EXPORT ViewRotate(void* pView, double dCenter[3], double dAxis[3], double dAngle)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	COCCView *pOCCView = (COCCView*)pView;
	if (!pOCCView)
		return;

	gp_Pnt ptCenter(dCenter[0], dCenter[1], dCenter[2]);
	gp_Dir dirAxis(dAxis[0], dAxis[1], dAxis[2]);
	pOCCView->ViewRotate(ptCenter, dirAxis, dAngle);
}

extern "C" void PASCAL EXPORT ViewPan(void* pView, int iPanningX, int iPanningY)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	COCCView* pOCCView = (COCCView*)pView;
	if (!pOCCView)
		return;

	return pOCCView->ViewPan(iPanningX, iPanningY);
}

extern "C" void PASCAL EXPORT ViewZoom(void* pView, int iMouseX, int iMouseY, double dZoomFactor)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	COCCView* pOCCView = (COCCView*)pView;
	if (!pOCCView)
		return;

	return pOCCView->ViewZoom(iMouseX, iMouseY, dZoomFactor);
}

// Set the projection of the view
extern "C" void PASCAL EXPORT ViewSetProj(void* pView, double dProjX, double dProjY, double dProjZ)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	COCCView* pOCCView = (COCCView*)pView;
	if (!pOCCView)
		return;

	gp_Dir dirProj(dProjX, dProjY, dProjZ);
	pOCCView->ViewSetProj(dirProj);
}

// Model
extern "C" void* PASCAL EXPORT ReadIges(void* pView, LPCTSTR pcFileName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	COCCView* pOCCView = (COCCView*)pView;
	if (!pOCCView || !pcFileName)
		return NULL;

	return pOCCView->ReadIges(pcFileName);
}

extern "C" void* PASCAL EXPORT ReadStep(void* pView, LPCTSTR pcFileName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	COCCView* pOCCView = (COCCView*)pView;
	if (!pOCCView || !pcFileName)
		return NULL;

	return pOCCView->ReadStep(pcFileName);
}

extern "C" void* PASCAL EXPORT ReadStl(void* pView, LPCTSTR pcFileName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	COCCView* pOCCView = (COCCView*)pView;
	if (!pOCCView || !pcFileName)
		return NULL;

	return pOCCView->ReadStl(pcFileName);
}

extern "C" void* PASCAL EXPORT ReadObj(void* pView, LPCTSTR pcFileName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	COCCView* pOCCView = (COCCView*)pView;
	if (!pOCCView || !pcFileName)
		return NULL;

	return pOCCView->ReadObj(pcFileName);
}

extern "C" void PASCAL EXPORT DeleteModel(void* pView, void* pModel)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	COCCView* pOCCView = (COCCView*)pView;
	if (!pOCCView || !pModel)
		return;

	Handle(AIS_Shape) hAISShape = dynamic_cast<AIS_Shape*>(static_cast<AIS_InteractiveObject*>(pModel));
	if (hAISShape)
		pOCCView->DeleteModel(hAISShape);
}

extern "C" void PASCAL EXPORT DisplayModel(void* pView, void* pModel)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	COCCView* pOCCView = (COCCView*)pView;
	if (!pOCCView || !pModel)
		return;

	Handle(AIS_Shape) hAISShape = dynamic_cast<AIS_Shape*>(static_cast<AIS_InteractiveObject*>(pModel));
	if (hAISShape)
		pOCCView->DisplayModel(hAISShape);
}

extern "C" void PASCAL EXPORT RemoveModel(void* pView, void* pModel)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	COCCView* pOCCView = (COCCView*)pView;
	if (!pOCCView || !pModel)
		return;

	Handle(AIS_Shape) hAISShape = dynamic_cast<AIS_Shape*>(static_cast<AIS_InteractiveObject*>(pModel));
	if (hAISShape)
		pOCCView->RemoveModel(hAISShape);
}

extern "C" void PASCAL EXPORT SetModelColor(void* pModel, double dColor[3])
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!pModel)
		return;

	Handle(AIS_Shape) hAISShape = dynamic_cast<AIS_Shape*>(static_cast<AIS_InteractiveObject*>(pModel));
	if (!hAISShape)
		return;

	hAISShape->SetColor(Quantity_Color(dColor[0], dColor[1], dColor[2], Quantity_TOC_RGB));

	Handle(Prs3d_Drawer) hPrs3dDrawer = hAISShape->Attributes();
	if (!hPrs3dDrawer)
		return;

	Handle(Prs3d_ShadingAspect) hPrs3dShadingAspect = hPrs3dDrawer->ShadingAspect();
	if (!hPrs3dShadingAspect)
		return;

	hPrs3dShadingAspect->SetColor(Quantity_Color(dColor[0], dColor[1], dColor[2], Quantity_TOC_RGB), Aspect_TOFM_FRONT_SIDE);
	hPrs3dShadingAspect->SetColor(Quantity_Color(0.3, 0.3, 0.3, Quantity_TOC_RGB), Aspect_TOFM_BACK_SIDE);
}

extern "C" bool PASCAL EXPORT GetModelCenter(void* pModel, double dCenter[3])
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!pModel)
		return false;

	Handle(AIS_Shape) hAISShape = dynamic_cast<AIS_Shape*>(static_cast<AIS_InteractiveObject*>(pModel));
	if (!hAISShape)
		return false;

	Bnd_Box bndBox;
	BRepBndLib::Add(hAISShape->Shape(), bndBox);

	gp_Pnt ptCornerMin = bndBox.CornerMin().Transformed(hAISShape->LocalTransformation());
	gp_Pnt ptCornerMax = bndBox.CornerMax().Transformed(hAISShape->LocalTransformation());
	dCenter[0] = (ptCornerMin.X() + ptCornerMax.X()) / 2.0;
	dCenter[1] = (ptCornerMin.Y() + ptCornerMax.Y()) / 2.0;
	dCenter[2] = (ptCornerMin.Z() + ptCornerMax.Z()) / 2.0;

	return true;
}

extern "C" void PASCAL EXPORT GetModelMesh(void* pModel, vector<double>&vecMesh)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	vecMesh.clear();

	if (!pModel)
		return;

	Handle(AIS_Shape) hAISShape = dynamic_cast<AIS_Shape*>(static_cast<AIS_InteractiveObject*>(pModel));
	if (!hAISShape)
		return;

	TopoDS_Shape shape = hAISShape->Shape();
	if (shape.IsNull())
		return;
	shape = shape.Located(hAISShape->LocalTransformation());

	int iNbTriangles = 0;
	for (TopExp_Explorer expFace(shape, TopAbs_FACE); expFace.More(); expFace.Next())
	{
		TopLoc_Location loc;
		Handle(Poly_Triangulation) hPolyTriangulation = BRep_Tool::Triangulation(TopoDS::Face(expFace.Current()), loc);
		if (hPolyTriangulation.IsNull())
			continue;

		iNbTriangles += hPolyTriangulation->NbTriangles();
	}
	vecMesh.resize(iNbTriangles * 3 * 3);

	int iCount = 0;
	for (TopExp_Explorer expFace(shape, TopAbs_FACE); expFace.More(); expFace.Next())
	{
		TopoDS_Face face = TopoDS::Face(expFace.Current());
		TopLoc_Location loc;
		Handle(Poly_Triangulation) hPolyTriangulation = BRep_Tool::Triangulation(face, loc);
		if (hPolyTriangulation.IsNull())
			continue;

		for (int i = 1; i <= hPolyTriangulation->NbTriangles(); i++)
		{
			const Poly_Triangle& triangle = hPolyTriangulation->Triangle(i);

			if (face.Orientation() != TopAbs_REVERSED)
			{
				for (int j = 1; j <= 3; j++)
				{
					gp_Pnt pt = hPolyTriangulation->Node(triangle(j));
					pt.Transform(loc.Transformation());

					vecMesh[iCount++] = pt.X();
					vecMesh[iCount++] = pt.Y();
					vecMesh[iCount++] = pt.Z();
				}
			}
			else
			{
				for (int j = 3; j >=1; j--)
				{
					gp_Pnt pt = hPolyTriangulation->Node(triangle(j));
					pt.Transform(loc.Transformation());

					vecMesh[iCount++] = pt.X();
					vecMesh[iCount++] = pt.Y();
					vecMesh[iCount++] = pt.Z();
				}
			}
		}
	}
}

extern "C" bool PASCAL EXPORT GetLocalTransformation(void* pModel, double dMatrix[4][4])
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!pModel)
		return false;

	Handle(AIS_Shape) hAISShape = dynamic_cast<AIS_Shape*>(static_cast<AIS_InteractiveObject*>(pModel));
	if (!hAISShape)
		return false;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			dMatrix[i][j] = (i == j) ? 1. : 0.;

	gp_Trsf trsf = hAISShape->LocalTransformation();
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 4; j++)
			dMatrix[i][j] = trsf.Value(i + 1, j + 1);

	return true;
}

extern "C" void PASCAL EXPORT SetLocalTransformation(void* pModel, double dMatrix[4][4])
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!pModel)
		return;

	Handle(AIS_Shape) hAISShape = dynamic_cast<AIS_Shape*>(static_cast<AIS_InteractiveObject*>(pModel));
	if (!hAISShape)
		return;

	gp_Trsf trsf;
	trsf.SetValues(dMatrix[0][0], dMatrix[0][1], dMatrix[0][2], dMatrix[0][3], 
		dMatrix[1][0], dMatrix[1][1], dMatrix[1][2], dMatrix[1][3], 
		dMatrix[2][0], dMatrix[2][1], dMatrix[2][2], dMatrix[2][3]);
	
	hAISShape->SetLocalTransformation(trsf);
}

// Mouse event
extern "C" void PASCAL EXPORT InputEvent(void* pView, int iMouseX, int iMouseY)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	COCCView* pOCCView = (COCCView*)pView;
	if (!pOCCView)
		return;

	pOCCView->InputEvent(iMouseX, iMouseY);
}

extern "C" void PASCAL EXPORT ClearSelected(void* pView)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	COCCView* pOCCView = (COCCView*)pView;
	if (!pOCCView)
		return;

	pOCCView->ClearSelected();
}

extern "C" void GetSelected(void* pView, vector<void*> &vecSelectedShape)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	COCCView* pOCCView = (COCCView*)pView;
	if (!pOCCView)
		return;

	pOCCView->GetSelected(vecSelectedShape);
}