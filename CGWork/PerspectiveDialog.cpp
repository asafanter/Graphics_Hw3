// PerspectiveDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CGWork.h"
#include "PerspectiveDialog.h"
#include "afxdialogex.h"


// PerspectiveDialog dialog

IMPLEMENT_DYNAMIC(PerspectiveDialog, CDialog)

PerspectiveDialog::PerspectiveDialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG2, pParent),
	_near(0.1),
	_far(100),
	_fov_y_deg(45)
{

}

PerspectiveDialog::~PerspectiveDialog()
{
}

void PerspectiveDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, _near);
	DDX_Text(pDX, IDC_EDIT2, _fov_y_deg);
}


BEGIN_MESSAGE_MAP(PerspectiveDialog, CDialog)
END_MESSAGE_MAP()


// PerspectiveDialog message handlers
