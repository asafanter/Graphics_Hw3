#include "stdafx.h"
#include "CGWork.h"
#include "ActiveMeshDialog.h"
#include "afxdialogex.h"


IMPLEMENT_DYNAMIC(ActiveMeshDialog, CDialog)



ActiveMeshDialog::ActiveMeshDialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG4, pParent),
	_active(-1)
{

}

ActiveMeshDialog::~ActiveMeshDialog()
{
}

void ActiveMeshDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, _active);
}


BEGIN_MESSAGE_MAP(ActiveMeshDialog, CDialog)
END_MESSAGE_MAP()


// PerspectiveDialog message handlers
