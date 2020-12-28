// QQ_serverDlg.cpp : implementation file
//

#include "stdafx.h"
#include "QQ_server.h"
#include "QQ_serverDlg.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>  

#include <stdio.h> 

#include <fstream.h>

#include <iostream.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

//int zhunbei = 0;
int judge[5] ;
int judgeflag[5] ;
bool isFirst = true;
int startsum = 0;
int sendsum = 0;
int judgesum = 0;
int sendjudgesum = 0;
bool NameisUsed = false; 
CString Log = "";

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQQ_serverDlg dialog

CQQ_serverDlg::CQQ_serverDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQQ_serverDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CQQ_serverDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//初始化客户端成员列表
	for(int i=0;i<5;i++)
	{
		info[i].isUsed = false;
	}
}

void CQQ_serverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CQQ_serverDlg)
	DDX_Control(pDX, IDC_MEMBER, m_memlist);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CQQ_serverDlg, CDialog)
	//{{AFX_MSG_MAP(CQQ_serverDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_STARTRUN, OnStartrun)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SOCKET,OnSocket)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQQ_serverDlg message handlers

BOOL CQQ_serverDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	//初始化图像列表框
	InitListView();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CQQ_serverDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CQQ_serverDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CQQ_serverDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//点击界面的“开启服务器”后逻辑
void CQQ_serverDlg::OnStartrun() 
{
	// TODO: Add your control notification handler code here
	//加载套接字库
	WORD	ver = MAKEWORD(2,0);
	WSADATA	wsadata;

	GetTime();   //日志中记录时间
	Log += "服务器开启...\r\n";

	if( WSAStartup(ver,&wsadata) )
	{
		Log += "加载套接字库失败\r\n";
		AfxMessageBox("加载套接字库失败");
		return;
	}
	
	//创建基于TCP的套接字
	socket_server = socket(AF_INET,SOCK_STREAM,NULL);
	if( INVALID_SOCKET == socket_server )
	{
		Log += "创建套接字失败\r\n";
		AfxMessageBox("创建套接字失败");
		return;
	}
	
	//获取主机的ip地址
	char host_name[20] = "";
	gethostname(host_name,20);
	hostent *tent = gethostbyname(host_name);
	in_addr	ip_addr;

	//下面的代码用于指定IP地址。
	//注：如果用无线的IP地址，用的WIFI不同的话地址也不用，用前注意更改
	tent->h_addr_list[0][0] = 10;  
	tent->h_addr_list[0][1] = 22; 
	tent->h_addr_list[0][2] = 118;  
	tent->h_addr_list[0][3] = 106;

	memmove(&ip_addr,tent->h_addr_list[0],4);
	
	//绑定端口号和ip
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port	= htons(3050);		//默认设置的端口号
	addr.sin_addr	= ip_addr;
	if( bind(socket_server,(sockaddr *)&addr,sizeof(sockaddr)) )
	{
		Log += "绑定端口号和ip出错\r\n";
		AfxMessageBox("绑定端口号和ip出错");
		return;
	}
	
	//监听
	listen(socket_server,5);
	
	//设置异步套接字
	if( SOCKET_ERROR == WSAAsyncSelect(socket_server,this->m_hWnd,WM_SOCKET,
		FD_ACCEPT | FD_CLOSE | FD_READ) )
	{
		Log += "异步套接字设置出错\r\n";
		AfxMessageBox("异步套接字设置出错");
		return;
	}

	//提示信息
	Log += "服务器启动成功\r\n";
	AfxMessageBox("服务器启动成功");
	GetDlgItem(IDC_STARTRUN)->EnableWindow(false);		//禁用“开启服务器”按钮

}

//自定义消息响应函数
void CQQ_serverDlg::OnSocket(WPARAM wParam,LPARAM lParam)
{
	SOCKET		sock_temp = SOCKET_ERROR;
	sockaddr_in addr;
	int			len = sizeof(sockaddr_in);
	char		buf_recv[256] = "";			//临时存储接收到的信息
	CString		str_name = "";
	CString		card = "";
	
	switch(lParam)
	{
	case FD_ACCEPT:
		sock_temp = accept(socket_server,(sockaddr *)&addr,&len);
		if( INVALID_SOCKET == sock_temp)
		{
			Log += "接收客户端的连接请求失败\r\n";
			AfxMessageBox("接收客户端的连接请求失败");
			return;
		}
		break;
		
	case FD_CLOSE:
		sock_temp = (SOCKET)wParam;
		if( NameisUsed == false )
		{
			if(!delMember(sock_temp))
			{
				return;
			}
			SendAllMem();			//封装的函数
			break;
		}
	
	case FD_READ:	
		sock_temp = (SOCKET)wParam;
		if( SOCKET_ERROR == recv(sock_temp,buf_recv,256,NULL) )
		{
			Log += "接收客户端名称出错\r\n";
			AfxMessageBox("接收客户端名称出错");
			return;
		}
		
		//接收客户端发来的用户名信息
		if(buf_recv[0] == 'F')
		{
			str_name = getKeyMsg(buf_recv,"From");
			//判断用户名有没有人使用过
			if( !Nameisused(str_name,sock_temp) )
			{
				NameisUsed = false;
				//添加成员
				if( !addMember(str_name,sock_temp) )
				{
					return;
				}
				//统一更新所有成员的图像列表信息
				SendAllMem();			//封装的函数
				return;
			}
		}
		
		//接受用户发来的开始游戏的消息
		if(buf_recv[0] == 'S')
		{
			/*
			int i = 0;
			for (;i < 5;i++)
			{
				if( info[i].isUsed == false )
					break;
			}
			//zhunbei++;
			if(zhunbei == i)
			*/
			startsum ++;
			if(startsum == GetGameer())
			{
				InitGame();
				for(int i = 0;i < GetGameer();i++)
				{
					SendGameInfo(info[i].user_socket);
					Sleep(100);
				}
					
			}
			//else   可以定义还有XX玩家未准备
			return;
		}

		//接受用户发来的出牌信息
		if(buf_recv[0] == 'C')
		{
			sendsum++;
			card = getKeyMsg(buf_recv,"Card");
			UpdateGame(card);
			if( sendsum == GetGameer() )
			{
				sendsum = 0;
				Judge();
			}
			if( judgesum == 3)
			{
				judgesum = 0;
				GetAllscore();
				InitGame();
				for(int i = 0;i < GetGameer();i++)
				{
					SendGameInfo(info[i].user_socket);
					Sleep(100);
				}
			}
			return;
		}

		//转发用户发来的消息
		if( NameisUsed == false)
		{
			TransMsg(buf_recv);
			return;
		}
		break;
	}
}

//判断用户名是否使用过
BOOL CQQ_serverDlg::Nameisused(CString name,SOCKET sock_temp)
{
	CString str_temp = "";
	bool used = false;
	int i = GetGameer();
	for(int j = 0;j < i;j++)
	{
		if(info[j].name == name)
		{
			used = true;
			break;
		}
	}
	if(used == true)
	{	
		NameisUsed = true;
		str_temp += "U";
		if( SOCKET_ERROR == send(sock_temp,str_temp,str_temp.GetLength(),NULL) )
		{
			AfxMessageBox("成员名是否使用过信息发送出错");
		}
		return true;
	}
	return false;
}


//转发用户发来的消息
void CQQ_serverDlg::TransMsg(CString recv_msg)
{
	//取出To包含的信息
	CString to_name = getKeyMsg(recv_msg,"To");
	//查找对应的socket
	SOCKET	sock_temp = SOCKET_ERROR;
	for(int i=0;i<5;i++)
	{
		if(info[i].isUsed == true && info[i].name == to_name)
		{
			sock_temp = info[i].user_socket;
			break;
		}
	}
	//转发消息
	if( SOCKET_ERROR == send(sock_temp,recv_msg,recv_msg.GetLength(),NULL) )
	{
		AfxMessageBox("消息转发出错");
		return;
	}
}

//向客户端成员列表中添加成员
BOOL CQQ_serverDlg::addMember(CString name,SOCKET sock)
{
	for(int i=0;i<5;i++)
	{
		if(info[i].isUsed == false)
		{
			info[i].isUsed		= true;
			info[i].name		= name;
			info[i].user_socket = sock;
			
			Log += "名称为：" + name + "的用户接入服务器" + "\r\n";
			//添加图像列表成员
			AddListMem(name,i);
			
			return true;
		}
	}
	AfxMessageBox("客户端成员列表已满");
	return false;
}
//删除客户端成员列表中的成员
BOOL CQQ_serverDlg::delMember(SOCKET sock)
{
	for(int i=0;i<5;i++)
	{
		if(info[i].isUsed == true && info[i].user_socket == sock)
		{
			//删除图像列表成员
			Log += "名称为：" + info[i].name + "退出了游戏。\r\n";
			LVFINDINFO	findInfo;
			findInfo.flags = LVFI_PARTIAL | LVFI_STRING;
			findInfo.psz   = info[i].name.GetBuffer(0);
			int index = m_memlist.FindItem(&findInfo);
			m_memlist.DeleteItem(index);

			info[i].isUsed		= false;
			info[i].name		= "";

			return true;
		}
		startsum = 0;
	}
	AfxMessageBox("没有找到该成员");
	return false;
}
//对接收到的数据依据标识字段进行筛选
CString CQQ_serverDlg::getKeyMsg(CString recv_msg,CString keyword)
{
	int index_start = 0,index_end = 0;
	int index = 0;
	CString str_temp = "";

	if("To" == keyword)
	{
		index_start = recv_msg.Find("To:");
		index_end	= recv_msg.Find("\r\n",index_start);
		for(index = index_start+3;index < index_end;index++)
		{
			str_temp += recv_msg.GetAt(index);
		}
		return str_temp;
	}

	if("From" == keyword)
	{
		index_start = recv_msg.Find("From:");
		index_end	= recv_msg.Find("\r\n",index_start);
		for(index = index_start+5;index < index_end;index++)
		{
			str_temp += recv_msg.GetAt(index);
		}
		return str_temp;
	}

	//处理客户端发来的CARD信息
	if("Card" == keyword)
	{
		index_start = recv_msg.Find("Card:");
		index_end	= recv_msg.Find("\r\n",index_start);
		for(index = index_start+5;index < index_end;index++)
		{
			str_temp += recv_msg.GetAt(index);
		}
		return str_temp;
	}
	//处理客户端发来的STARTGAME信息
	if("StartGame" == keyword)
	{
		index_start = recv_msg.Find("StartGame");
		index_end	= recv_msg.Find("\r\n",index_start);
		for(index = index_start+9;index < index_end;index++)
		{
			str_temp += recv_msg.GetAt(index);
		}
		return str_temp;
	}

	return "";
}
//初始化图像列表框
void CQQ_serverDlg::InitListView()
{
	//创建图像列表
	m_imagelist.Create(32,32,ILC_COLOR32,5,5);
	//添加图像
	for(int i = 0;i < 5;i++)
	{
		m_imagelist.Add( LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(129+i)) );
	}
	//关联到列表控件中
	m_memlist.SetImageList(&m_imagelist,LVSIL_SMALL);
}

//向列表框控件中添加成员
void CQQ_serverDlg::AddListMem(CString name,int index)
{
	LVITEM lvitem;
	memset(&lvitem,0,sizeof(LVITEM));
	lvitem.mask = LVIF_IMAGE | LVIF_TEXT;
	lvitem.iItem = index;
	lvitem.pszText = name.GetBuffer(0);
	lvitem.iImage = index;
	
	m_memlist.InsertItem(&lvitem);
}
//发送成员名信息
void CQQ_serverDlg::SendMemName(SOCKET sock,CString name)
{
	//构造信息
	CString str_temp = "";
	str_temp  = "List:";
	str_temp += name;
	str_temp += "\r\n";

	if( SOCKET_ERROR == send(sock,str_temp,str_temp.GetLength(),NULL) )
	{
		AfxMessageBox("成员名信息发送出错");
		return;
	}
}

//发送牌的信息
void CQQ_serverDlg::SendGameInfo(SOCKET sock)
{
	//构造信息
		CString str_temp = "";
		int i = GetGameer();
		//for(int j = 0; j < i ; j++)
		//	InitGame(j);
		for(int k = 0; k < i ; k++)
		{
			CString card = game_info[k].card;
			str_temp += card;
		}
		str_temp += "\r\n";
		if( SOCKET_ERROR == send(sock,str_temp,str_temp.GetLength(),NULL) )
		{
			AfxMessageBox("牌的信息发送出错");
			return;
		}
}

//向所有列表成员发送成员名信息
void CQQ_serverDlg::SendAllMem()
{
	CString name_list = "";
	int count = m_memlist.GetItemCount();
	for(int i=0;i<5;i++)
	{
		if(info[i].isUsed == true)
		{
			//格式化名字列表
			for(int j=0;j<count;j++)
			{
				CString name = m_memlist.GetItemText(j,0);
				if(info[i].name != name)
				{
					name_list += name;
					name_list += ",";
				}
			}
			SendMemName(info[i].user_socket,name_list);
			name_list = "";
		}
	}
}

void CQQ_serverDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	closesocket(socket_server);
	WSACleanup();
	GetTime();
	Log += "服务器关闭...\r\n\r\n";
	WriteLog(Log);
	CDialog::OnCancel();
}

//获得当前共多少玩家
int CQQ_serverDlg::GetGameer()
{
	for (int i = 0;i < 5;i++)
	{
		if( info[i].isUsed == false )
			break;
	}
	return i;
}
//游戏初始化，服务器给每个玩家分配牌,并将牌和玩家姓名绑定
void CQQ_serverDlg::InitGame()
{
	int j = GetGameer();
	srand((unsigned)time(NULL));
	for(int i = 0;i < j;i++)
	{
		if( isFirst == true) //game_info[i].isFirst == true)  //只有在第一次运行时，总分Allscore = 0
		{
			game_info[i].Allscore = 0;
			//	game_info[i].isFirst = false;
		}
		game_info[i].score = 0;        //每一小回合score = 0
		game_info[i].game_times = 3;   //每一小回合重置游戏次数为3
		game_info[i].card = "";
		int c1 = (rand() % (3))+ 1; //生成[1,3]的随机数
		int c2 = (rand() % (3))+ 4; //生成[4,6]的随机数
		int c3 = (rand() % (3))+ 7; //生成[7,9]的随机数
		CString C1,C2,C3;
		C1.Format("%d",c1);
		C2.Format("%d",c2);
		C3.Format("%d",c3);
		game_info[i].card +="Name:" + info[i].name + C1 + C2 + C3 +"end";
		Log += "名称为：" + info[i].name + "的用户开始游戏，其本局牌为：" + C1 + C2 + C3 + "\r\n";
	}
	isFirst = false;
/*
	CString   temp;   
	temp.Format( "%d ",game_info[0].Allscore);   
	AfxMessageBox(temp);
*/
}

//游戏牌更新,并将牌和玩家姓名绑定
void CQQ_serverDlg::UpdateGame(CString card)
{
//	bool isAllsend = true;
	int i = GetInt(card);
//	game_info[i].isSendcard = true;
	game_info[i].game_times--;

	/*
	CString   temp;   
	temp.Format( "%d ",game_info[0].isSendcard);   
	AfxMessageBox(temp);
	temp.Format( "%d ",game_info[1].isSendcard); 
	AfxMessageBox(temp);
	*/

	GetSendCard(i,card);

	DeleteCard(i,card);

/*	int j = GetGameer();

	for(int k = 0; k < j; k++)
	{
		if(game_info[k].isSendcard == false)
		{
			isAllsend = false;
			break;
		}
	}
*/
//	if( isAllsend == true )
//	{
//		Judge();
//		if(game_info[i].game_times == 0)
//		{
//			GetAllscore();
//		}
//	}
}

//此函数用于接受从客户端发来的信息中，玩家出牌的信息，并有标识NAME得到该玩家的姓名所对应的编号
int CQQ_serverDlg::GetInt(CString name)
{
	CString str_temp = "";
	int k = name.GetLength();
	str_temp = name.Left( k - 1);
	int i = 0;
	for (;i < 5;i++)
	{
		if( info[i].isUsed == false )
			break;
	}
	for(int j = 0; j < i; j++)
	{
		if( info[j].name == str_temp)
			break;
	}
	return j;
}

//获得到用户出的牌
void CQQ_serverDlg::GetSendCard(int i,CString sendcard)
{
	CString cardtemp = sendcard;
	int user_namelong = info[i].name.GetLength();
	CString card = cardtemp.GetAt(user_namelong);
	int card1 = _ttoi(card);
	judge[i] = card1;
	judgeflag[i] = i;
	/*
	CString   temp;   
	temp.Format( "%d ",judge[i]);   
	AfxMessageBox(temp);
	*/
}

//冒泡排序，A储存值，B储存索引
void Bubblesort(int* A,int* B,int n)
{
	int temp1,temp2;
	for(int i = 0;i < n;i++)
		for(int j = 0;j < n-i-1;j++)
			if(A[j] <= A[j+1])
			{
				temp1 = A[j];
				A[j] = A[j+1];
				A[j+1] = temp1;
				temp2 = B[j];
				B[j] = B[j+1];
				B[j+1] = temp2;
			}
}

//此函数用于判断用户出的牌大小比较，并加分
void CQQ_serverDlg::Judge()
{
	judgesum++;
	int j = GetGameer();
/*	for(int k = 0; k < j; k++)
		game_info[k].isSendcard = false;
*/
	CString judgeinfo = "";
	Bubblesort(judge,judgeflag,j);
	game_info[judgeflag[0]].score += 10;
	int a = 10;
	for(int z = 0; z < j - 1 ;z++)
	{
		if( judge[z + 1] != judge[z])
			a = a - 2;
		game_info[ judgeflag[ z + 1 ] ].score += a;
	}
	judgeinfo += "Judge:";
	for(int d = 0; d < j ; d++)
	{
		CString str = "";
		CString str_pack("  "); //设置输出几个空格
		judgeinfo += info[judgeflag[d]].name;
		judgeinfo += str_pack;
		str.Format(_T("%d"), judge[d]);
		judgeinfo += str;
		judgeinfo += str_pack;
	}
	judgeinfo += "Score:";
	for(int w = 0; w < j ; w++)
	{
		CString str = "";
		CString str_pack("  "); //设置输出几个空格
		judgeinfo += info[judgeflag[w]].name;
		judgeinfo += str_pack;
		str.Format(_T("%d"), game_info[ judgeflag[w] ].score);
		judgeinfo += str;
		judgeinfo += str_pack;
	}
	judgeinfo += "\r\n";
//	AfxMessageBox(judgeinfo);
	Log += judgeinfo;
	Sendjudge(judgeinfo);
	//return judgeinfo;
}

//发送JUDGE信息
void CQQ_serverDlg::Sendjudge(CString judgeinfo)
{
	sendjudgesum++;
	int i = GetGameer();
	SOCKET sock_temp;
	for(int j = 0; j < i;j++)
	{
		sock_temp = info[j].user_socket;
		if( SOCKET_ERROR == send(sock_temp,judgeinfo,judgeinfo.GetLength(),NULL) )
		{
			Log += "JUDGE信息发送出错\r\n";
			AfxMessageBox("JUDGE信息发送出错");
			return;
		}
		Sleep(100);
	}
	if(sendjudgesum < 3)
	{
		for(int k = 0; k < i; k++)
		{
		sock_temp = info[k].user_socket;
		SendGameInfo(sock_temp);
		Sleep(100);
		}
	}
	else
		sendjudgesum = 0;
}


//此函数用于用户出牌后删除掉该牌
void CQQ_serverDlg::DeleteCard(int i , CString sendcard) 
{
	CString judgetemp = sendcard;
	int user_namelong = info[i].name.GetLength();
	CString delcard = judgetemp.GetAt(user_namelong);
	char *p ;
	p = delcard.GetBuffer(delcard.GetLength()); 
	delcard.ReleaseBuffer();
	int num = game_info[i].card.Remove(p[0]);     //删除该字符串
}

//此函数用于计算用户每局游戏的总得分
void CQQ_serverDlg::GetAllscore()
{
	CString allscore = "";
	int score[5];
	int scoreflag[5];
	int i = GetGameer();
	for(int j = 0; j < i;j++)
	{
		score[j] = game_info[j].score;
		scoreflag[j] = j;
	}
	Bubblesort(score,scoreflag,i);
	
/*	CString   temp;   
	temp.Format( "%d ",scoreflag[0]);   
	AfxMessageBox(temp);
*/	
	game_info[ scoreflag[0] ].Allscore += 10;
	int a = 10;
	for(int z = 0; z < i - 1 ;z++)
	{
		if( score[z + 1] != score[z])
			a = a - 2;
		game_info[ scoreflag[ z + 1 ] ].Allscore += a;
	}

	allscore += "Allscore:";

/*	CString   temp;   
	temp.Format( "%d ",game_info[0].Allscore);   
	AfxMessageBox(temp);
*/
	for(int d = 0; d < i ; d++)
	{
		CString str = "";
		CString str_pack("  "); //设置输出几个空格
		allscore += info[ scoreflag[d] ].name;
		allscore += str_pack;
		str.Format(_T("%d"), game_info[ scoreflag[d] ].Allscore);
		allscore += str;
		allscore += str_pack;
	}

	allscore += "\r\n";

	Log += allscore;

	for(int k = 0; k < i; k++)
	{
		SOCKET sock_temp;
		sock_temp = info[k].user_socket;
		Sendallscore(sock_temp,allscore);
		Sleep(100);
	}
	//return allscore;
}

//发送总得分信息
void CQQ_serverDlg::Sendallscore(SOCKET sock, CString allscore)
{
	if( SOCKET_ERROR == send(sock,allscore,allscore.GetLength(),NULL) )
	{
		Log += "总得分信息发送出错\r\n";
		AfxMessageBox("总得分信息发送出错");
		return;
	}
}

void CQQ_serverDlg::GetTime()   //获得系统时间
{
	CString	Time = "";
	time_t rawtime;
	struct tm *ptminfo;  
	time(&rawtime); 
	ptminfo = localtime(&rawtime);
	int year = ptminfo->tm_year + 1900;   //取得年份
	int month = ptminfo->tm_mon + 1;   //取得月份
	int day = ptminfo->tm_mday;  //取得日期
	int hour = ptminfo->tm_hour;  //取得小时
	int mintue = ptminfo->tm_min;  //取得分钟
	int second = ptminfo->tm_sec;  //取得秒钟
	CString str = "";
	CString str_pack("  "); //设置输出几个空格
	str.Format(_T("%d"), year);
	Time += str + "年";
	str.Format(_T("%d"), month);
	Time += str + "月";
	str.Format(_T("%d"), day);
	Time += str + "日";
	Time += str_pack;
	str.Format(_T("%d"), hour);
	Time += str + ":";
	str.Format(_T("%d"), mintue);
	Time += str + ":";
	str.Format(_T("%d"), second);
	Time += str + "\r\n";
	Log += Time; 
}

void CQQ_serverDlg::WriteLog(CString log)
{
	ofstream write;
	write.open("D:\serverlog.txt", ios::app); 
    //表示你要把内容输出到“serverlog.txt"这个文件里 如果没有这个文件，会自动创建这个文件

	write << Log; 
    //这里是你想要输出的内容，这里是输出了一个string abc
	Log = "";
    write.close(); // 输出完毕后关闭这个文件
}
