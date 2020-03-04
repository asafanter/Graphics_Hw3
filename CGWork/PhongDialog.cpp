// PhongDialog.cpp : implementation file
//

#include "StdAfx.h"
#include "CGWork.h"
#include "PhongDialog.h"
#include "afxdialogex.h"


// PhongDialog dialog

IMPLEMENT_DYNAMIC(PhongDialog, CDialog)

PhongDialog::PhongDialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG7, pParent),
	_factor(1)
{

}

PhongDialog::~PhongDialog()
{
}

void PhongDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, _factor);
}


BEGIN_MESSAGE_MAP(PhongDialog, CDialog)
END_MESSAGE_MAP()


// PhongDialog message handlers
