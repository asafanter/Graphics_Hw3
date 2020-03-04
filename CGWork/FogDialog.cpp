// FogDialog.cpp : implementation file
//

#include "StdAfx.h"
#include "CGWork.h"
#include "FogDialog.h"
#include "afxdialogex.h"


// FogDialog dialog

IMPLEMENT_DYNAMIC(FogDialog, CDialog)

FogDialog::FogDialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG6, pParent),
	_min(1.0),
	_max(-1.0)
{

}

FogDialog::~FogDialog()
{
}

void FogDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, _min);
	DDX_Text(pDX, IDC_EDIT2, _max);
}


BEGIN_MESSAGE_MAP(FogDialog, CDialog)
END_MESSAGE_MAP()


// FogDialog message handlers
