// GAME_RULE.cpp : implementation file
//

#include "stdafx.h"
#include "QQ_client.h"
#include "GAME_RULE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GAME_RULE dialog


GAME_RULE::GAME_RULE(CWnd* pParent /*=NULL*/)
	: CDialog(GAME_RULE::IDD, pParent)
{
	//{{AFX_DATA_INIT(GAME_RULE)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void GAME_RULE::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(GAME_RULE)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(GAME_RULE, CDialog)
	//{{AFX_MSG_MAP(GAME_RULE)
	ON_BN_CLICKED(IDC_KNOW, OnKnow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GAME_RULE message handlers

void GAME_RULE::OnKnow() 
{
	CDialog::OnCancel();	//点击“知道了”后关闭对话框
	// TODO: Add your control notification handler code here
	
}
