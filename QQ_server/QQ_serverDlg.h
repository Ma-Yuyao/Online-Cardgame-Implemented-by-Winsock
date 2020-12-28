// QQ_serverDlg.h : header file
//

#if !defined(AFX_QQ_SERVERDLG_H__5D99A64D_4A30_41C3_B660_4D084A1F78C4__INCLUDED_)
#define AFX_QQ_SERVERDLG_H__5D99A64D_4A30_41C3_B660_4D084A1F78C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//自定义消息
#define WM_SOCKET	WM_USER + 100

//定义用于保存客户端连接结构
struct client_info
{
	BOOL	isUsed;
	CString name;
	SOCKET	user_socket;
};

//定义随机分配牌的结构
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

	//维护成员列表的函数
	BOOL	addMember(CString name,SOCKET sock);  //增加成员
	BOOL	delMember(SOCKET sock);				  //删除成员

	//解析接收到的信息
	CString getKeyMsg(CString recv_msg,CString keyword);

	//向客户端发送消息的函数
	void	SendMemName(SOCKET sock,CString name);     //发送成员名信息
	void	SendAllMem();                              //向所有列表成员发送成员名信息
	void	TransMsg(CString recv_msg);                //转发消息
	BOOL	Nameisused(CString name,SOCKET sock_temp); //判断用户名是否使用过
	
	//游戏处理函数
	void	InitGame();								    //初始化游戏数据
	void	SendGameInfo(SOCKET sock);                  //发送游戏数据信息
	void    UpdateGame(CString card);					//更新游戏数据信息
	int		GetInt(CString name);						//将玩家的姓名和其game_info结构体的编号绑定
	void	Judge();									//判定玩家每轮出牌的结果
	void    DeleteCard(int i , CString card);			//删除玩家每回合出的牌
	void	GetAllscore();								//获得每一局的总分数
	void	Sendjudge(CString judgeinfo);				//向服务器发送每轮的判定结果
	void	GetSendCard(int i,CString sendcard);		//获得玩家出的牌
	int		GetGameer();								//得到总玩家数
	void    Sendallscore(SOCKET sock,CString allscore); //发送每局总得分

	//列表框控件的操作
	void	InitListView();				            //初始化LISTVIEW，用于显示用户头像+用户名
	void	AddListMem(CString name,int index);     //增加list成员

	//输出日志类
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
	SOCKET			socket_server;  //定义SOCKET类型的socket_server
	client_info		info[5];        //为结构体client_info建5个实例
	game_info       game_info[5];   //为结构体game_info建5个实例
	CImageList		m_imagelist;    //新建CImageList类实例，用于对用户头像进行控制

	// Generated message map functions
	//{{AFX_MSG(CQQ_serverDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnStartrun();
	virtual void OnCancel();
	//}}AFX_MSG
	afx_msg void OnSocket(WPARAM wParam,LPARAM lParam);   //自定义消息处理函数
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QQ_SERVERDLG_H__5D99A64D_4A30_41C3_B660_4D084A1F78C4__INCLUDED_)
