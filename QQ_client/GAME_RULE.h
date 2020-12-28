#if !defined(AFX_GAME_RULE_H__00822504_FF4C_4904_848F_AC6E9E5CEF8D__INCLUDED_)
#define AFX_GAME_RULE_H__00822504_FF4C_4904_848F_AC6E9E5CEF8D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GAME_RULE.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// GAME_RULE dialog

class GAME_RULE : public CDialog
{
// Construction
public:
	GAME_RULE(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(GAME_RULE)
	enum { IDD = IDD_GAME_RULE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GAME_RULE)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(GAME_RULE)
	afx_msg void OnKnow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAME_RULE_H__00822504_FF4C_4904_848F_AC6E9E5CEF8D__INCLUDED_)
