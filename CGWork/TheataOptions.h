#pragma once

#include <afxwin.h>
// TheataOptions dialog

class TheataOptions : public CDialog
{
	DECLARE_DYNAMIC(TheataOptions)

public:
	TheataOptions(CWnd* pParent = nullptr);   // standard constructor
	virtual ~TheataOptions();
	double getTheta() const { return _theta; }

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG5 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	double _theta;
};
