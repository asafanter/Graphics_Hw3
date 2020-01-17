// PolygonDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CGWork.h"
#include "PolygonDialog.h"
#include "afxdialogex.h"


// PolygonDialog dialog

IMPLEMENT_DYNAMIC(PolygonDialog, CDialog)

PolygonDialog::PolygonDialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG3, pParent),
	_tolerance(20)
{

}

PolygonDialog::~PolygonDialog()
{
}

void PolygonDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, _tolerance);
}


BEGIN_MESSAGE_MAP(PolygonDialog, CDialog)
END_MESSAGE_MAP()


// PolygonDialog message handlers
