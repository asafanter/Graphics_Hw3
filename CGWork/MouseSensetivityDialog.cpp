// MouseSensetivityDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CGWork.h"
#include "MouseSensetivityDialog.h"
#include "afxdialogex.h"


// MouseSensetivityDialog dialog

IMPLEMENT_DYNAMIC(MouseSensetivityDialog, CDialog)

MouseSensetivityDialog::MouseSensetivityDialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG1, pParent),
	_rotation(1.0),
	_translation(0.05),
	_scale(1.0)
{

}

MouseSensetivityDialog::~MouseSensetivityDialog()
{
}

void MouseSensetivityDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, _rotation);
	DDX_Text(pDX, IDC_EDIT2, _translation);
	DDX_Text(pDX, IDC_EDIT3, _scale);
}


BEGIN_MESSAGE_MAP(MouseSensetivityDialog, CDialog)
END_MESSAGE_MAP()


// MouseSensetivityDialog message handlers
