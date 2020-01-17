#pragma once


// PerspectiveDialog dialog

class PerspectiveDialog : public CDialog
{
	DECLARE_DYNAMIC(PerspectiveDialog)

public:
	PerspectiveDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~PerspectiveDialog();
	double getNear() const { return _near; }
	double getFovYDeg() const { return _fov_y_deg; }
	double getFar() const { return _far; }

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	double _near;
	double _far;
	double _fov_y_deg;
};
