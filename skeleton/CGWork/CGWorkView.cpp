// CGWorkView.cpp : implementation of the CCGWorkView class
//
#include "stdafx.h"
#include "CGWork.h"

#include "CGWorkDoc.h"
#include "CGWorkView.h"

#include <iostream>
using std::cout;
using std::endl;
#include "MaterialDlg.h"
#include "LightDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "PngWrapper.h"
#include "iritSkel.h"


// For Status Bar access
#include "MainFrm.h"

#include <vector>
#include "Scene.h"

// Use this macro to display text messages in the status bar.
#define STATUS_BAR_TEXT(str) (((CMainFrame*)GetParentFrame())->getStatusBar().SetWindowText(str))


/////////////////////////////////////////////////////////////////////////////
// CCGWorkView



Scene scene;


IMPLEMENT_DYNCREATE(CCGWorkView, CView)

BEGIN_MESSAGE_MAP(CCGWorkView, CView)
	//{{AFX_MSG_MAP(CCGWorkView)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_LOAD, OnFileLoad)
	ON_COMMAND(ID_VIEW_ORTHOGRAPHIC, OnViewOrthographic)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ORTHOGRAPHIC, OnUpdateViewOrthographic)
	ON_COMMAND(ID_VIEW_PERSPECTIVE, OnViewPerspective)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PERSPECTIVE, OnUpdateViewPerspective)
	ON_COMMAND(ID_ACTION_ROTATE, OnActionRotate)
	ON_UPDATE_COMMAND_UI(ID_ACTION_ROTATE, OnUpdateActionRotate)
	ON_COMMAND(ID_ACTION_SCALE, OnActionScale)
	ON_UPDATE_COMMAND_UI(ID_ACTION_SCALE, OnUpdateActionScale)
	ON_COMMAND(ID_ACTION_TRANSLATE, OnActionTranslate)
	ON_UPDATE_COMMAND_UI(ID_ACTION_TRANSLATE, OnUpdateActionTranslate)
	ON_COMMAND(ID_AXIS_X, OnAxisX)
	ON_UPDATE_COMMAND_UI(ID_AXIS_X, OnUpdateAxisX)
	ON_COMMAND(ID_AXIS_Y, OnAxisY)
	ON_UPDATE_COMMAND_UI(ID_AXIS_Y, OnUpdateAxisY)
	ON_COMMAND(ID_AXIS_Z, OnAxisZ)
	ON_UPDATE_COMMAND_UI(ID_AXIS_Z, OnUpdateAxisZ)
	ON_COMMAND(ID_FACE_NORMALS, OnFaceNormals)
	ON_UPDATE_COMMAND_UI(ID_FACE_NORMALS, OnUpdateFaceNormals)
	ON_COMMAND(ID_SHOW_VER_NORMALS, OnVerNormals)
	ON_UPDATE_COMMAND_UI(ID_SHOW_VER_NORMALS, OnUpdateVerNormals)

	ON_COMMAND(ID_FACE_NORMALS_CALCULATED, OnFaceNormalsCalc)
	ON_UPDATE_COMMAND_UI(ID_FACE_NORMALS_CALCULATED, OnUpdateFaceNormalsCalc)
	ON_COMMAND(ID_VERTEX_NORMALS_CALCULATED, OnVerNormalsCalc)
	ON_UPDATE_COMMAND_UI(ID_VERTEX_NORMALS_CALCULATED, OnUpdateVerNormalsCalc)

	ON_COMMAND(ID_LIGHT_SHADING_FLAT, OnLightShadingFlat)
	ON_UPDATE_COMMAND_UI(ID_LIGHT_SHADING_FLAT, OnUpdateLightShadingFlat)
	ON_COMMAND(ID_LIGHT_SHADING_GOURAUD, OnLightShadingGouraud)
	ON_UPDATE_COMMAND_UI(ID_LIGHT_SHADING_GOURAUD, OnUpdateLightShadingGouraud)
	ON_COMMAND(ID_LIGHT_CONSTANTS, OnLightConstants)
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_COORDINATESYSTEM_VIEW, &CCGWorkView::OnCoordinatesystemView)
	ON_UPDATE_COMMAND_UI(ID_COORDINATESYSTEM_VIEW, &CCGWorkView::OnUpdateCoordinatesystemView)
	ON_COMMAND(ID_COORDINATESYSTEM_MODEL, &CCGWorkView::OnCoordinatesystemModel)
	ON_UPDATE_COMMAND_UI(ID_COORDINATESYSTEM_MODEL, &CCGWorkView::OnUpdateCoordinatesystemModel)
END_MESSAGE_MAP()


// A patch to fix GLaux disappearance from VS2005 to VS2008
void auxSolidCone(GLdouble radius, GLdouble height) {
        GLUquadric *quad = gluNewQuadric();
        gluQuadricDrawStyle(quad, GLU_FILL);
        gluCylinder(quad, radius, 0.0, height, 20, 20);
        gluDeleteQuadric(quad);
}

/////////////////////////////////////////////////////////////////////////////
// CCGWorkView construction/destruction

CCGWorkView::CCGWorkView()
{
	// Set default values
	m_nAxis = ID_AXIS_X;
	m_nAction = ID_ACTION_ROTATE;
	m_nView = ID_VIEW_ORTHOGRAPHIC;	
	m_bIsPerspective = false;

	m_nLightShading = ID_LIGHT_SHADING_FLAT;

	m_lMaterialAmbient = 0.2;
	m_lMaterialDiffuse = 0.8;
	m_lMaterialSpecular = 1.0;
	m_nMaterialCosineFactor = 32;

	//init the first light to be enabled
	m_lights[LIGHT_ID_1].enabled=true;
	m_pDbBitMap = NULL;
	m_pDbDC = NULL;

	_curr_coordinate_system = CoordinateSystem::VIEW;
}

CCGWorkView::~CCGWorkView()
{
}


/////////////////////////////////////////////////////////////////////////////
// CCGWorkView diagnostics

#ifdef _DEBUG
void CCGWorkView::AssertValid() const
{
	CView::AssertValid();
}

void CCGWorkView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCGWorkDoc* CCGWorkView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCGWorkDoc)));
	return (CCGWorkDoc*)m_pDocument;
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CCGWorkView Window Creation - Linkage of windows to CGWork

BOOL CCGWorkView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	// An CGWork window must be created with the following
	// flags and must NOT include CS_PARENTDC for the
	// class style.

	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	return CView::PreCreateWindow(cs);
}



int CCGWorkView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	InitializeCGWork();

	m_bShowFaceNormals = m_bShowVerNormals = false;
	m_bshowGivenFNormal = m_bshowGivenVNormal = true;//beccause they look cooler
	m_lMouseSensitivity = 1;//TODO: add a dialog that controls this(and controls colors and other stuff to be controlled)
	m_nActiveMesh = -1;//all are active
	return 0;
}


// This method initialized the CGWork system.
BOOL CCGWorkView::InitializeCGWork()
{
	m_pDC = new CClientDC(this);
	
	if ( NULL == m_pDC ) { // failure to get DC
		::AfxMessageBox(CString("Couldn't get a valid DC."));
		return FALSE;
	}

	CRect r;
	GetClientRect(&r);
	m_pDbDC = new CDC();
	m_pDbDC->CreateCompatibleDC(m_pDC);
	SetTimer(1, 1, NULL);
	m_pDbBitMap = CreateCompatibleBitmap(m_pDC->m_hDC, r.right, r.bottom);	
	m_pDbDC->SelectObject(m_pDbBitMap);
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CCGWorkView message handlers


void CCGWorkView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);

	if ( 0 >= cx || 0 >= cy ) {
		return;
	}

	// save the width and height of the current window
	m_WindowWidth = cx;
	m_WindowHeight = cy;

	// compute the aspect ratio
	// this will keep all dimension scales equal
	m_AspectRatio = (GLdouble)m_WindowWidth/(GLdouble)m_WindowHeight;

	CRect r;
	GetClientRect(&r);
	DeleteObject(m_pDbBitMap);
	m_pDbBitMap = CreateCompatibleBitmap(m_pDC->m_hDC, r.right, r.bottom);	
	m_pDbDC->SelectObject(m_pDbBitMap);
}


BOOL CCGWorkView::SetupViewingFrustum(void)
{
    return TRUE;
}


// This viewing projection gives us a constant aspect ration. This is done by
// increasing the corresponding size of the ortho cube.
BOOL CCGWorkView::SetupViewingOrthoConstAspect(void)
{
	return TRUE;
}





BOOL CCGWorkView::OnEraseBkgnd(CDC* pDC) 
{
	// Windows will clear the window with the background color every time your window 
	// is redrawn, and then CGWork will clear the viewport with its own background color.

	
	return true;
}





/////////////////////////////////////////////////////////////////////////////
// CCGWorkView drawing
/////////////////////////////////////////////////////////////////////////////

void CCGWorkView::OnDraw(CDC* pDC)
{
	static float theta = 0.0f;
	CCGWorkDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
	    return;
	CRect r;

	GetClientRect(&r);
	CDC *pDCToUse = /*m_pDC*/m_pDbDC;

	pDCToUse->FillSolidRect(&r, scene.getBackgroundColor());

	auto w = r.Width();
	auto h = r.Height();

	scene.draw(pDCToUse, r.Width(), r.Height(), m_bShowFaceNormals, m_bShowVerNormals, m_bshowGivenFNormal, m_bshowGivenVNormal);
	m_pDC->BitBlt(r.left, r.top, r.Width(), r.Height(), pDCToUse, r.left, r.top, SRCCOPY);
	


	/*pDCToUse->FillSolidRect(&r, RGB(255, 255, 0));
	
	int numLines = 100;
	double radius = r.right / 3.0;
	
	if (r.right > r.bottom) {
		radius = r.bottom / 3.0;
	}
	
	for (int i = 0; i < numLines; ++i)
	{
		double finalTheta = 2 * M_PI / numLines*i + theta*M_PI/180.0f;
		
		pDCToUse->MoveTo(r.right / 2, r.bottom / 2);
		pDCToUse->LineTo((int)(r.right / 2 + radius*cos(finalTheta)), (int)(r.bottom / 2 + radius*sin(finalTheta)));
	}	


	if (pDCToUse != m_pDC) 
	{
		m_pDC->BitBlt(r.left, r.top, r.Width(), r.Height(), pDCToUse, r.left, r.top, SRCCOPY);
	}
	
	for (Model* model : Models) {
		
				
		for (int i = 0; i < model->getVertexes().size() - 1; i++) {
			Vec3d point1 = model->getVertexes()[i];
			Vec3d point2 = model->getVertexes()[i+1];


			//Salih: Testing stuff: x=(x+1)*w , y= (1-y)*h/2
			//MidPointDraw(  (point1(0)+1)*r.Width()/2  , (point1(1)+1)*r.Height()/2, (point2(0) + 1)*r.Width()/2, (point2(1)+1)*r.Height() / 2, *pDCToUse , model->getColor());
		}
		

	}

	//MidPointDraw(355, 489, 355, 163, pDCToUse, RGB(0, 0, 0));
	MidPointDraw(0, 0, 50, 50, pDCToUse, RGB(0, 0, 0));

	theta += 5;	*/
}






/////////////////////////////////////////////////////////////////////////////
// CCGWorkView CGWork Finishing and clearing...

void CCGWorkView::OnDestroy() 
{
	CView::OnDestroy();

	// delete the DC
	if ( m_pDC ) {
		delete m_pDC;
	}

	if (m_pDbDC) {
		delete m_pDbDC;
	}
}



/////////////////////////////////////////////////////////////////////////////
// User Defined Functions

void CCGWorkView::RenderScene() {
	// do nothing. This is supposed to be overriden...

	return;
}


void CCGWorkView::OnFileLoad() 
{
	TCHAR szFilters[] = _T ("IRIT Data Files (*.itd)|*.itd|All Files (*.*)|*.*||");

	CFileDialog dlg(TRUE, _T("itd"), _T("*.itd"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY ,szFilters);

	if (dlg.DoModal () == IDOK) {
		m_strItdFileName = dlg.GetPathName();		// Full path and filename
		PngWrapper p;
		CGSkelProcessIritDataFiles(m_strItdFileName, 1);
		// Open the file and read it.
		// Your code here...

		scene.setInitialized(false);

		Invalidate();	// force a WM_PAINT for drawing.
	} 

}





// VIEW HANDLERS ///////////////////////////////////////////

// Note: that all the following Message Handlers act in a similar way.
// Each control or command has two functions associated with it.

void CCGWorkView::OnViewOrthographic() 
{
	m_nView = ID_VIEW_ORTHOGRAPHIC;
	m_bIsPerspective = false;
	Tmatd projection = TransformationMatrix<double>::ortho(-10.0, 10.0, -5.0, 5.0, -5.0, 5.0);
	scene.setProjection(projection);
	Invalidate();		// redraw using the new view.
}

void CCGWorkView::OnUpdateViewOrthographic(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nView == ID_VIEW_ORTHOGRAPHIC);
}

void CCGWorkView::OnViewPerspective() 
{
	m_nView = ID_VIEW_PERSPECTIVE;
	m_bIsPerspective = true;
	Tmatd projection = TransformationMatrix<double>::perspective(45, 1240.0 / 630.0, 0.1, 100);
	scene.setProjection(projection);
	Invalidate();
}

void CCGWorkView::OnUpdateViewPerspective(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nView == ID_VIEW_PERSPECTIVE);
}




// ACTION HANDLERS ///////////////////////////////////////////

void CCGWorkView::OnActionRotate() 
{
	m_nAction = ID_ACTION_ROTATE;
}

void CCGWorkView::OnUpdateActionRotate(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nAction == ID_ACTION_ROTATE);
}

void CCGWorkView::OnActionTranslate() 
{
	m_nAction = ID_ACTION_TRANSLATE;
}

void CCGWorkView::OnUpdateActionTranslate(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nAction == ID_ACTION_TRANSLATE);
}

void CCGWorkView::OnActionScale() 
{
	m_nAction = ID_ACTION_SCALE;
}

void CCGWorkView::OnUpdateActionScale(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nAction == ID_ACTION_SCALE);
}




// AXIS HANDLERS ///////////////////////////////////////////


// Gets calles when the X button is pressed or when the Axis->X menu is selected.
// The only thing we do here is set the ChildView member variable m_nAxis to the 
// selected axis.
void CCGWorkView::OnAxisX() 
{
	m_nAxis = ID_AXIS_X;
}

// Gets called when windows has to repaint either the X button or the Axis pop up menu.
// The control is responsible for its redrawing.
// It sets itself disabled when the action is a Scale action.
// It sets itself Checked if the current axis is the X axis.
void CCGWorkView::OnUpdateAxisX(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nAxis == ID_AXIS_X);
}


void CCGWorkView::OnAxisY() 
{
	m_nAxis = ID_AXIS_Y;
}

void CCGWorkView::OnUpdateAxisY(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nAxis == ID_AXIS_Y);
}


void CCGWorkView::OnAxisZ() 
{
	m_nAxis = ID_AXIS_Z;
}

void CCGWorkView::OnUpdateAxisZ(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nAxis == ID_AXIS_Z);
}


void CCGWorkView::OnFaceNormals()
{
	m_bShowFaceNormals = !m_bShowFaceNormals;
}

void CCGWorkView::OnUpdateFaceNormals(CCmdUI * pCmdUI)
{

	pCmdUI->SetCheck(m_bShowFaceNormals == true);
}

void CCGWorkView::OnVerNormals()
{
	m_bShowVerNormals = !m_bShowVerNormals;
}

void CCGWorkView::OnUpdateVerNormals(CCmdUI * pCmdUI)
{
	pCmdUI->SetCheck(m_bShowVerNormals == true);
}



//handles choosing what type of normals(faces and vertecis) should we show (doesn't handle if we show or not
void CCGWorkView::OnFaceNormalsCalc()
{
	m_bshowGivenFNormal = !m_bshowGivenFNormal;
}

void CCGWorkView::OnUpdateFaceNormalsCalc(CCmdUI * pCmdUI)
{
	pCmdUI->SetCheck(m_bshowGivenFNormal == true);
}


void CCGWorkView::OnVerNormalsCalc()
{
	m_bshowGivenVNormal = !m_bshowGivenVNormal;
}

void CCGWorkView::OnUpdateVerNormalsCalc(CCmdUI * pCmdUI)
{
	pCmdUI->SetCheck(m_bshowGivenVNormal == true);
}





// OPTIONS HANDLERS ///////////////////////////////////////////




// LIGHT SHADING HANDLERS ///////////////////////////////////////////

void CCGWorkView::OnLightShadingFlat()
{
	m_nLightShading = ID_LIGHT_SHADING_FLAT;
}

void CCGWorkView::OnUpdateLightShadingFlat(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nLightShading == ID_LIGHT_SHADING_FLAT);
}


void CCGWorkView::OnLightShadingGouraud()
{
	m_nLightShading = ID_LIGHT_SHADING_GOURAUD;
}

void CCGWorkView::OnUpdateLightShadingGouraud(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nLightShading == ID_LIGHT_SHADING_GOURAUD);
}

// LIGHT SETUP HANDLER ///////////////////////////////////////////

void CCGWorkView::OnLightConstants()
{
	CLightDialog dlg;

	for (int id = LIGHT_ID_1; id < MAX_LIGHT; id++)
	{
		dlg.SetDialogData((LightID)id, m_lights[id]);
	}
	dlg.SetDialogData(LIGHT_ID_AMBIENT, m_ambientLight);

	if (dlg.DoModal() == IDOK)
	{
		for (int id = LIGHT_ID_1; id < MAX_LIGHT; id++)
		{
			m_lights[id] = dlg.GetDialogData((LightID)id);
		}
		m_ambientLight = dlg.GetDialogData(LIGHT_ID_AMBIENT);
	}
	Invalidate();
}

void CCGWorkView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	CView::OnTimer(nIDEvent);
	if (nIDEvent == 1)
		Invalidate();
}

void CCGWorkView::rotate(const int &angle)
{
	double factor = 0.1f;

	if (_curr_coordinate_system == CoordinateSystem::VIEW)
	{
		rotateView(angle * factor);
	}
	else if (_curr_coordinate_system == CoordinateSystem::MODEL)
	{
		rotateModel(angle * factor);
	}
}

void CCGWorkView::translate(const int &dist)
{
	double factor = 0.05f;

	if (_curr_coordinate_system == CoordinateSystem::VIEW)
	{
		translateView(dist * factor);
	}
	else if(_curr_coordinate_system == CoordinateSystem::MODEL)
	{
		translateModel(dist * factor);
	}
}

void CCGWorkView::scale(const int &scaling)
{
	if (scaling == 0)
	{
		return;
	}

	double factor = 1.1f * m_lMouseSensitivity; 
	if (scaling < 0)
	{
		factor = 1 / factor;
	}

	if (_curr_coordinate_system == CoordinateSystem::VIEW)
	{
		scaleView(factor);
	}
	else if (_curr_coordinate_system == CoordinateSystem::MODEL)
	{
		scaleModel(factor);
	}
}

void CCGWorkView::rotateModel(const double &val)
{
	auto& meshs = scene.getMeshes();
	if (m_nAxis == ID_AXIS_X)
	{
		if (m_nActiveMesh == -1)//do it for all meshs
			for (size_t i = 0; i < meshs.size(); i++)
			{
				meshs[i].rotateX(val);
			}

		else {
			meshs[m_nActiveMesh].rotateX(val);
		}

	}
	else if (m_nAxis == ID_AXIS_Y)
	{
		if (m_nActiveMesh == -1)//do it for all meshs
			for (size_t i = 0; i < meshs.size(); i++)
			{
				meshs[i].rotateY(val);
			}

		else {
			meshs[m_nActiveMesh].rotateY(val);
		}
	}
	else if (m_nAxis == ID_AXIS_Z)
	{
		if (m_nActiveMesh == -1)//do it for all meshs
			for (size_t i = 0; i < meshs.size(); i++)
			{
				meshs[i].rotateZ(val);
			}

		else {
			meshs[m_nActiveMesh].rotateZ(val);
		}

	}
}

	void CCGWorkView::translateModel(const double &val) {


	auto& meshs = scene.getMeshes();

	if (m_nAxis == ID_AXIS_X)
	{
		if (m_nActiveMesh == -1)//do it for all meshs
			for (size_t i = 0; i < meshs.size(); i++)
			{
				meshs[i].translate(Vec3d(val, 0.0, 0.0));
			}

		else {
			meshs[m_nActiveMesh].translate(Vec3d(val, 0.0, 0.0));
		}
	}
	else if (m_nAxis == ID_AXIS_Y)
	{

		if (m_nActiveMesh == -1)//do it for all meshs
			for (size_t i = 0; i < meshs.size(); i++)
			{
				meshs[i].translate(Vec3d(0.0, val, 0.0));
			}

		else {
			meshs[m_nActiveMesh].translate(Vec3d(0.0, val, 0.0));
		}
	}
	else if (m_nAxis == ID_AXIS_Z)
	{
		if (m_nActiveMesh == -1)//do it for all meshs
			for (size_t i = 0; i < meshs.size(); i++)
			{
				meshs[i].translate(Vec3d(0.0, 0.0, val));
			}

		else {
			meshs[m_nActiveMesh].translate(Vec3d(0.0, 0.0, val));
		}
	}

}

void CCGWorkView::scaleModel(const double &val)
{
	if (m_nAxis == ID_AXIS_X)
	{
		scene.getMeshes()[0].scale(Vec3d(val, 1.0, 1.0));
	}
	else if (m_nAxis == ID_AXIS_Y)
	{
		scene.getMeshes()[0].scale(Vec3d(1.0, val, 1.0));
	}
	else if (m_nAxis == ID_AXIS_Z)
	{
		scene.getMeshes()[0].scale(Vec3d(1.0, 1.0, val));
	}
}

void CCGWorkView::rotateView(const double &val)
{
	if (m_nAxis == ID_AXIS_X)
	{
		scene.getView().rotateX(val);
	}
	else if (m_nAxis == ID_AXIS_Y)
	{
		scene.getView().rotateY(val);
	}
	else if (m_nAxis == ID_AXIS_Z)
	{
		scene.getView().rotateZ(val);
	}
}

void CCGWorkView::translateView(const double &val)
{
	if (m_nAxis == ID_AXIS_X)
	{
		scene.getView().translate(Vec3d(val, 0.0, 0.0));
	}
	else if (m_nAxis == ID_AXIS_Y)
	{
		scene.getView().translate(Vec3d(0.0, val, 0.0));
	}
	else if (m_nAxis == ID_AXIS_Z)
	{
		scene.getView().translate(Vec3d(0.0, 0.0, val));
	}
}

void CCGWorkView::scaleView(const double &val)
{
	if (m_nAxis == ID_AXIS_X)
	{
		scene.getView().scale(Vec3d(val, 1.0, 1.0));
	}
	else if (m_nAxis == ID_AXIS_Y)
	{
		scene.getView().scale(Vec3d(1.0, val, 1.0));
	}
	else if (m_nAxis == ID_AXIS_Z)
	{
		scene.getView().scale(Vec3d(1.0, 1.0, val));
	}
}

void CCGWorkView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!scene.isInitialized())
	{
		return;
	}

	static uint last_x = 0;
	uint curr_x = point.x;

	int dx = static_cast<int>(curr_x - last_x);

	if (nFlags == MK_LBUTTON)
	{
		if (m_nAction == ID_ACTION_ROTATE)
		{
			rotate(dx);
		}
		else if (m_nAction == ID_ACTION_TRANSLATE)
		{
			translate(dx);
		}
		else if (m_nAction == ID_ACTION_SCALE)
		{
			scale(dx);
		}
	}

	last_x = curr_x;

	CView::OnMouseMove(nFlags, point);
}


void CCGWorkView::OnCoordinatesystemView()
{
	_curr_coordinate_system = CoordinateSystem::VIEW;
}


void CCGWorkView::OnUpdateCoordinatesystemView(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(_curr_coordinate_system == CoordinateSystem::VIEW);
}


void CCGWorkView::OnCoordinatesystemModel()
{
	_curr_coordinate_system = CoordinateSystem::MODEL;
}


void CCGWorkView::OnUpdateCoordinatesystemModel(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(_curr_coordinate_system == CoordinateSystem::MODEL);
}
