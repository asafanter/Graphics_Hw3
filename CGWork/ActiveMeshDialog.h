#pragma once


// ActiveMeshDialog dialog

class ActiveMeshDialog : public CDialog
{
	DECLARE_DYNAMIC(ActiveMeshDialog)

public:
	ActiveMeshDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ActiveMeshDialog();
	int getActive() const { return _active; }
	void setActive(int val)  {  _active = val; }
	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	int _active;
};
