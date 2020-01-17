// CGWorkView.h : interface of the CCGWorkView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGWORKVIEW_H__5857316D_EA60_11D5_9FD5_00D0B718E2CD__INCLUDED_)
#define AFX_CGWORKVIEW_H__5857316D_EA60_11D5_9FD5_00D0B718E2CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "gl\gl.h"    // Include the standard CGWork  headers
#include "gl\glu.h"   // Add the utility library


#include "Light.h"
#include "MouseSensetivityDialog.h"
#include "PerspectiveDialog.h"
#include "PolygonDialog.h"
#include "ActiveMeshDialog.h"

class CCGWorkView : public CView
{
protected: // create from serialization only
	CCGWorkView();
	DECLARE_DYNCREATE(CCGWorkView)

// Attributes
public:
	CCGWorkDoc* GetDocument();

// Operations
public:

private:
	int m_nAxis;				// Axis of Action, X Y or Z
	int m_nAction;				// Rotate, Translate, Scale
	int m_nView;				// Orthographic, perspective
	bool m_bIsPerspective;			// is the view perspective
	bool m_bShowBoundingBox;			//TODO: Add gui button
	bool m_bShowFaceNormals;		//should it draw the face normals,
	bool m_bShowVerNormals;		//should it draw the vertexes normals, 
	bool m_bshowGivenFNormal;//default is false, if false && m_bShowFaceNormals is true then show calculated face normals
	bool m_bshowGivenVNormal;//default is false if false && m_bShowVerNormals is true then show calculated vertex normals
	int m_nActiveMesh; //the id of the mesh to apply the transformation to ,initiated to -1(means all Meshs are active) TODO: implement changing this value
	double m_RotationSensitivity;		
	double m_TranslationSensitivity;
	double m_ScaleSensitivity;


	CString m_strItdFileName;		// file name of IRIT data

	int m_nLightShading;			// shading: Flat, Gouraud.

	double m_lMaterialAmbient;		// The Ambient in the scene
	double m_lMaterialDiffuse;		// The Diffuse in the scene
	double m_lMaterialSpecular;		// The Specular in the scene
	int m_nMaterialCosineFactor;		// The cosine factor for the specular

	LightParams m_lights[MAX_LIGHT];	//configurable lights array
	LightParams m_ambientLight;		//ambient light (only RGB is used)

	enum class CoordinateSystem
	{
		MODEL,
		VIEW
	};

	CoordinateSystem _curr_coordinate_system;
	MouseSensetivityDialog _mouse_sensetivity_dialog;
	PerspectiveDialog _perspective_dialog;
	PolygonDialog _polygon_dialog;
	ActiveMeshDialog  _active_mesh_dialog;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCGWorkView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCGWorkView();
	void rotate(const int &angle);
	void translate(const int &dist);
	void scale(const int &scaling);

	void rotateView(const double &val);
	void translateView(const double &val);
	void scaleView(const double &val);

	void rotateModel(const double &val);
	void translateModel(const double &val);
	void scaleModel(const double &val);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL InitializeCGWork();
	BOOL SetupViewingFrustum(void);
	BOOL SetupViewingOrthoConstAspect(void);

	virtual void RenderScene();


	HGLRC    m_hRC;			// holds the Rendering Context
	CDC*     m_pDC;			// holds the Device Context
	int m_WindowWidth;		// hold the windows width
	int m_WindowHeight;		// hold the windows height
	double m_AspectRatio;		// hold the fixed Aspect Ration

	HBITMAP m_pDbBitMap;
	CDC* m_pDbDC;

// Generated message map functions
protected:
	//{{AFX_MSG(CCGWorkView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFileLoad();
	afx_msg void OnViewOrthographic();
	afx_msg void OnUpdateViewOrthographic(CCmdUI* pCmdUI);
	afx_msg void OnViewPerspective();
	afx_msg void OnUpdateViewPerspective(CCmdUI* pCmdUI);
	afx_msg void OnActionRotate();
	afx_msg void OnUpdateActionRotate(CCmdUI* pCmdUI);
	afx_msg void OnActionScale();
	afx_msg void OnUpdateActionScale(CCmdUI* pCmdUI);
	afx_msg void OnActionTranslate();
	afx_msg void OnUpdateActionTranslate(CCmdUI* pCmdUI);
	afx_msg void OnAxisX();
	afx_msg void OnUpdateAxisX(CCmdUI* pCmdUI);
	afx_msg void OnAxisY();
	afx_msg void OnUpdateAxisY(CCmdUI* pCmdUI);
	afx_msg void OnAxisZ();
	afx_msg void OnUpdateAxisZ(CCmdUI* pCmdUI);
	afx_msg void OnFaceNormals();
	afx_msg void OnUpdateFaceNormals(CCmdUI* pCmdUI);
	afx_msg void OnVerNormals();
	afx_msg void OnUpdateVerNormals(CCmdUI* pCmdUI);
	afx_msg void OnFaceNormalsCalc();
	afx_msg void OnUpdateFaceNormalsCalc(CCmdUI * pCmdUI);
	afx_msg void OnVerNormalsCalc();
	afx_msg void OnUpdateVerNormalsCalc(CCmdUI * pCmdUI);
	afx_msg void OnShowBoundingBox();
	afx_msg void OnUpdateShowBoundingBox(CCmdUI * pCmdUI);
	afx_msg void OnLightShadingFlat();
	afx_msg void OnUpdateLightShadingFlat(CCmdUI* pCmdUI);
	afx_msg void OnLightShadingGouraud();
	afx_msg void OnUpdateLightShadingGouraud(CCmdUI* pCmdUI);
	afx_msg void OnLightConstants();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCoordinatesystemView();
	afx_msg void OnUpdateCoordinatesystemView(CCmdUI *pCmdUI);
	afx_msg void OnCoordinatesystemModel();
	afx_msg void OnUpdateCoordinatesystemModel(CCmdUI *pCmdUI);

	//ID_COLORS_WIREFRAMES

	afx_msg void OnColorsWireFrame();
	afx_msg void OnColorsBackground();
	afx_msg void OnColorsFaceNormals();
	afx_msg void OnColorsVerticesNormals();
	afx_msg void OnColorsBoundingBox();
	


	afx_msg void OnOptionsMousesensitivity();
	afx_msg void OnOptionsPerspectivecontrol();
	afx_msg void OnOptionsPolygonfinenesstolerance();
	afx_msg void OnOptionsActiveMesh();
};

#ifndef _DEBUG  // debug version in CGWorkView.cpp
inline CCGWorkDoc* CCGWorkView::GetDocument()
   { return (CCGWorkDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CGWORKVIEW_H__5857316D_EA60_11D5_9FD5_00D0B718E2CD__INCLUDED_)
