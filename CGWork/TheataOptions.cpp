// TheataOptions.cpp : implementation file
//

#include "StdAfx.h"
#include "CGWork.h"
#include "TheataOptions.h"
#include "afxdialogex.h"


// TheataOptions dialog

IMPLEMENT_DYNAMIC(TheataOptions, CDialog)

TheataOptions::TheataOptions(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG5, pParent),
	_theta(7.0)
{

}

TheataOptions::~TheataOptions()
{
}

void TheataOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, _theta);
}


BEGIN_MESSAGE_MAP(TheataOptions, CDialog)
END_MESSAGE_MAP()


// TheataOptions message handlers
