// QQ_serverDlg.h : header file
//

#if !defined(AFX_QQ_SERVERDLG_H__5D99A64D_4A30_41C3_B660_4D084A1F78C4__INCLUDED_)
#define AFX_QQ_SERVERDLG_H__5D99A64D_4A30_41C3_B660_4D084A1F78C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//�Զ�����Ϣ
#define WM_SOCKET	WM_USER + 100

//�������ڱ���ͻ������ӽṹ
struct client_info
{
	BOOL	isUsed;
	CString name;
	SOCKET	user_socket;
};

//������������ƵĽṹ
struct game_info
{
	CString card;
	int score;
	int Allscore;
	int game_times;
//	bool isFirst;
	bool isSendcard;
};
/////////////////////////////////////////////////////////////////////////////
// CQQ_serverDlg dialog

class CQQ_serverDlg : public CDialog	
{
// Construction
public:
	CQQ_serverDlg(CWnd* pParent = NULL);	// standard constructor

	//ά����Ա�б�ĺ���
	BOOL	addMember(CString name,SOCKET sock);  //���ӳ�Ա
	BOOL	delMember(SOCKET sock);				  //ɾ����Ա

	//�������յ�����Ϣ
	CString getKeyMsg(CString recv_msg,CString keyword);

	//��ͻ��˷�����Ϣ�ĺ���
	void	SendMemName(SOCKET sock,CString name);     //���ͳ�Ա����Ϣ
	void	SendAllMem();                              //�������б��Ա���ͳ�Ա����Ϣ
	void	TransMsg(CString recv_msg);                //ת����Ϣ
	BOOL	Nameisused(CString name,SOCKET sock_temp); //�ж��û����Ƿ�ʹ�ù�
	
	//��Ϸ������
	void	InitGame();								    //��ʼ����Ϸ����
	void	SendGameInfo(SOCKET sock);                  //������Ϸ������Ϣ
	void    UpdateGame(CString card);					//������Ϸ������Ϣ
	int		GetInt(CString name);						//����ҵ���������game_info�ṹ��ı�Ű�
	void	Judge();									//�ж����ÿ�ֳ��ƵĽ��
	void    DeleteCard(int i , CString card);			//ɾ�����ÿ�غϳ�����
	void	GetAllscore();								//���ÿһ�ֵ��ܷ���
	void	Sendjudge(CString judgeinfo);				//�����������ÿ�ֵ��ж����
	void	GetSendCard(int i,CString sendcard);		//�����ҳ�����
	int		GetGameer();								//�õ��������
	void    Sendallscore(SOCKET sock,CString allscore); //����ÿ���ܵ÷�

	//�б��ؼ��Ĳ���
	void	InitListView();				            //��ʼ��LISTVIEW��������ʾ�û�ͷ��+�û���
	void	AddListMem(CString name,int index);     //����list��Ա

	//�����־��
	void	GetTime();
	void	WriteLog(CString log);

// Dialog Data
	//{{AFX_DATA(CQQ_serverDlg)
	enum { IDD = IDD_QQ_SERVER_DIALOG };
	CListCtrl	m_memlist;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQQ_serverDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON			m_hIcon;
	SOCKET			socket_server;  //����SOCKET���͵�socket_server
	client_info		info[5];        //Ϊ�ṹ��client_info��5��ʵ��
	game_info       game_info[5];   //Ϊ�ṹ��game_info��5��ʵ��
	CImageList		m_imagelist;    //�½�CImageList��ʵ�������ڶ��û�ͷ����п���

	// Generated message map functions
	//{{AFX_MSG(CQQ_serverDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnStartrun();
	virtual void OnCancel();
	//}}AFX_MSG
	afx_msg void OnSocket(WPARAM wParam,LPARAM lParam);   //�Զ�����Ϣ������
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QQ_SERVERDLG_H__5D99A64D_4A30_41C3_B660_4D084A1F78C4__INCLUDED_)
