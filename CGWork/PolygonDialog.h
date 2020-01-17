#pragma once


// PolygonDialog dialog

class PolygonDialog : public CDialog
{
	DECLARE_DYNAMIC(PolygonDialog)

public:
	PolygonDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~PolygonDialog();
	double getTolerance() const { return _tolerance; }

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	double _tolerance;
};
