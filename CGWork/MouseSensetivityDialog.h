#pragma once


// MouseSensetivityDialog dialog

class MouseSensetivityDialog : public CDialog
{
	DECLARE_DYNAMIC(MouseSensetivityDialog)

public:
	MouseSensetivityDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~MouseSensetivityDialog();
	double getRotationSensetivity() const { return _rotation; }
	double getTranslationSensetivity() const { return _translation; }
	double getScaleSensetivity() const { return _scale; }

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	double _rotation;
	double _translation;
	double _scale;
};
