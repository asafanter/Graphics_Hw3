#pragma once

#include <afxwin.h>
// PhongDialog dialog

class PhongDialog : public CDialog
{
	DECLARE_DYNAMIC(PhongDialog)

public:
	PhongDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~PhongDialog();
	int getFactor() const { return _factor; }

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG7 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	int _factor;
};
