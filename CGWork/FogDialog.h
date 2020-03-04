#pragma once

#include <afxwin.h>
// FogDialog dialog

class FogDialog : public CDialog
{
	DECLARE_DYNAMIC(FogDialog)

public:
	FogDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~FogDialog();
	double getMin() const { return _min; }
	double getMax() const { return _max; }

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG6 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	double _min;
	double _max;
};
