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

	//��ʼ���ͻ��˳�Ա�б�
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
	//��ʼ��ͼ���б��
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

//�������ġ����������������߼�
void CQQ_serverDlg::OnStartrun() 
{
	// TODO: Add your control notification handler code here
	//�����׽��ֿ�
	WORD	ver = MAKEWORD(2,0);
	WSADATA	wsadata;

	GetTime();   //��־�м�¼ʱ��
	Log += "����������...\r\n";

	if( WSAStartup(ver,&wsadata) )
	{
		Log += "�����׽��ֿ�ʧ��\r\n";
		AfxMessageBox("�����׽��ֿ�ʧ��");
		return;
	}
	
	//��������TCP���׽���
	socket_server = socket(AF_INET,SOCK_STREAM,NULL);
	if( INVALID_SOCKET == socket_server )
	{
		Log += "�����׽���ʧ��\r\n";
		AfxMessageBox("�����׽���ʧ��");
		return;
	}
	
	//��ȡ������ip��ַ
	char host_name[20] = "";
	gethostname(host_name,20);
	hostent *tent = gethostbyname(host_name);
	in_addr	ip_addr;

	//����Ĵ�������ָ��IP��ַ��
	//ע����������ߵ�IP��ַ���õ�WIFI��ͬ�Ļ���ַҲ���ã���ǰע�����
	tent->h_addr_list[0][0] = 10;  
	tent->h_addr_list[0][1] = 22; 
	tent->h_addr_list[0][2] = 118;  
	tent->h_addr_list[0][3] = 106;

	memmove(&ip_addr,tent->h_addr_list[0],4);
	
	//�󶨶˿ںź�ip
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port	= htons(3050);		//Ĭ�����õĶ˿ں�
	addr.sin_addr	= ip_addr;
	if( bind(socket_server,(sockaddr *)&addr,sizeof(sockaddr)) )
	{
		Log += "�󶨶˿ںź�ip����\r\n";
		AfxMessageBox("�󶨶˿ںź�ip����");
		return;
	}
	
	//����
	listen(socket_server,5);
	
	//�����첽�׽���
	if( SOCKET_ERROR == WSAAsyncSelect(socket_server,this->m_hWnd,WM_SOCKET,
		FD_ACCEPT | FD_CLOSE | FD_READ) )
	{
		Log += "�첽�׽������ó���\r\n";
		AfxMessageBox("�첽�׽������ó���");
		return;
	}

	//��ʾ��Ϣ
	Log += "�����������ɹ�\r\n";
	AfxMessageBox("�����������ɹ�");
	GetDlgItem(IDC_STARTRUN)->EnableWindow(false);		//���á���������������ť

}

//�Զ�����Ϣ��Ӧ����
void CQQ_serverDlg::OnSocket(WPARAM wParam,LPARAM lParam)
{
	SOCKET		sock_temp = SOCKET_ERROR;
	sockaddr_in addr;
	int			len = sizeof(sockaddr_in);
	char		buf_recv[256] = "";			//��ʱ�洢���յ�����Ϣ
	CString		str_name = "";
	CString		card = "";
	
	switch(lParam)
	{
	case FD_ACCEPT:
		sock_temp = accept(socket_server,(sockaddr *)&addr,&len);
		if( INVALID_SOCKET == sock_temp)
		{
			Log += "���տͻ��˵���������ʧ��\r\n";
			AfxMessageBox("���տͻ��˵���������ʧ��");
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
			SendAllMem();			//��װ�ĺ���
			break;
		}
	
	case FD_READ:	
		sock_temp = (SOCKET)wParam;
		if( SOCKET_ERROR == recv(sock_temp,buf_recv,256,NULL) )
		{
			Log += "���տͻ������Ƴ���\r\n";
			AfxMessageBox("���տͻ������Ƴ���");
			return;
		}
		
		//���տͻ��˷������û�����Ϣ
		if(buf_recv[0] == 'F')
		{
			str_name = getKeyMsg(buf_recv,"From");
			//�ж��û�����û����ʹ�ù�
			if( !Nameisused(str_name,sock_temp) )
			{
				NameisUsed = false;
				//��ӳ�Ա
				if( !addMember(str_name,sock_temp) )
				{
					return;
				}
				//ͳһ�������г�Ա��ͼ���б���Ϣ
				SendAllMem();			//��װ�ĺ���
				return;
			}
		}
		
		//�����û������Ŀ�ʼ��Ϸ����Ϣ
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
			//else   ���Զ��廹��XX���δ׼��
			return;
		}

		//�����û������ĳ�����Ϣ
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

		//ת���û���������Ϣ
		if( NameisUsed == false)
		{
			TransMsg(buf_recv);
			return;
		}
		break;
	}
}

//�ж��û����Ƿ�ʹ�ù�
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
			AfxMessageBox("��Ա���Ƿ�ʹ�ù���Ϣ���ͳ���");
		}
		return true;
	}
	return false;
}


//ת���û���������Ϣ
void CQQ_serverDlg::TransMsg(CString recv_msg)
{
	//ȡ��To��������Ϣ
	CString to_name = getKeyMsg(recv_msg,"To");
	//���Ҷ�Ӧ��socket
	SOCKET	sock_temp = SOCKET_ERROR;
	for(int i=0;i<5;i++)
	{
		if(info[i].isUsed == true && info[i].name == to_name)
		{
			sock_temp = info[i].user_socket;
			break;
		}
	}
	//ת����Ϣ
	if( SOCKET_ERROR == send(sock_temp,recv_msg,recv_msg.GetLength(),NULL) )
	{
		AfxMessageBox("��Ϣת������");
		return;
	}
}

//��ͻ��˳�Ա�б�����ӳ�Ա
BOOL CQQ_serverDlg::addMember(CString name,SOCKET sock)
{
	for(int i=0;i<5;i++)
	{
		if(info[i].isUsed == false)
		{
			info[i].isUsed		= true;
			info[i].name		= name;
			info[i].user_socket = sock;
			
			Log += "����Ϊ��" + name + "���û����������" + "\r\n";
			//���ͼ���б��Ա
			AddListMem(name,i);
			
			return true;
		}
	}
	AfxMessageBox("�ͻ��˳�Ա�б�����");
	return false;
}
//ɾ���ͻ��˳�Ա�б��еĳ�Ա
BOOL CQQ_serverDlg::delMember(SOCKET sock)
{
	for(int i=0;i<5;i++)
	{
		if(info[i].isUsed == true && info[i].user_socket == sock)
		{
			//ɾ��ͼ���б��Ա
			Log += "����Ϊ��" + info[i].name + "�˳�����Ϸ��\r\n";
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
	AfxMessageBox("û���ҵ��ó�Ա");
	return false;
}
//�Խ��յ����������ݱ�ʶ�ֶν���ɸѡ
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

	//����ͻ��˷�����CARD��Ϣ
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
	//����ͻ��˷�����STARTGAME��Ϣ
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
//��ʼ��ͼ���б��
void CQQ_serverDlg::InitListView()
{
	//����ͼ���б�
	m_imagelist.Create(32,32,ILC_COLOR32,5,5);
	//���ͼ��
	for(int i = 0;i < 5;i++)
	{
		m_imagelist.Add( LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(129+i)) );
	}
	//�������б�ؼ���
	m_memlist.SetImageList(&m_imagelist,LVSIL_SMALL);
}

//���б��ؼ�����ӳ�Ա
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
//���ͳ�Ա����Ϣ
void CQQ_serverDlg::SendMemName(SOCKET sock,CString name)
{
	//������Ϣ
	CString str_temp = "";
	str_temp  = "List:";
	str_temp += name;
	str_temp += "\r\n";

	if( SOCKET_ERROR == send(sock,str_temp,str_temp.GetLength(),NULL) )
	{
		AfxMessageBox("��Ա����Ϣ���ͳ���");
		return;
	}
}

//�����Ƶ���Ϣ
void CQQ_serverDlg::SendGameInfo(SOCKET sock)
{
	//������Ϣ
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
			AfxMessageBox("�Ƶ���Ϣ���ͳ���");
			return;
		}
}

//�������б��Ա���ͳ�Ա����Ϣ
void CQQ_serverDlg::SendAllMem()
{
	CString name_list = "";
	int count = m_memlist.GetItemCount();
	for(int i=0;i<5;i++)
	{
		if(info[i].isUsed == true)
		{
			//��ʽ�������б�
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
	Log += "�������ر�...\r\n\r\n";
	WriteLog(Log);
	CDialog::OnCancel();
}

//��õ�ǰ���������
int CQQ_serverDlg::GetGameer()
{
	for (int i = 0;i < 5;i++)
	{
		if( info[i].isUsed == false )
			break;
	}
	return i;
}
//��Ϸ��ʼ������������ÿ����ҷ�����,�����ƺ����������
void CQQ_serverDlg::InitGame()
{
	int j = GetGameer();
	srand((unsigned)time(NULL));
	for(int i = 0;i < j;i++)
	{
		if( isFirst == true) //game_info[i].isFirst == true)  //ֻ���ڵ�һ������ʱ���ܷ�Allscore = 0
		{
			game_info[i].Allscore = 0;
			//	game_info[i].isFirst = false;
		}
		game_info[i].score = 0;        //ÿһС�غ�score = 0
		game_info[i].game_times = 3;   //ÿһС�غ�������Ϸ����Ϊ3
		game_info[i].card = "";
		int c1 = (rand() % (3))+ 1; //����[1,3]�������
		int c2 = (rand() % (3))+ 4; //����[4,6]�������
		int c3 = (rand() % (3))+ 7; //����[7,9]�������
		CString C1,C2,C3;
		C1.Format("%d",c1);
		C2.Format("%d",c2);
		C3.Format("%d",c3);
		game_info[i].card +="Name:" + info[i].name + C1 + C2 + C3 +"end";
		Log += "����Ϊ��" + info[i].name + "���û���ʼ��Ϸ���䱾����Ϊ��" + C1 + C2 + C3 + "\r\n";
	}
	isFirst = false;
/*
	CString   temp;   
	temp.Format( "%d ",game_info[0].Allscore);   
	AfxMessageBox(temp);
*/
}

//��Ϸ�Ƹ���,�����ƺ����������
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

//�˺������ڽ��ܴӿͻ��˷�������Ϣ�У���ҳ��Ƶ���Ϣ�����б�ʶNAME�õ�����ҵ���������Ӧ�ı��
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

//��õ��û�������
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

//ð������A����ֵ��B��������
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

//�˺��������ж��û������ƴ�С�Ƚϣ����ӷ�
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
		CString str_pack("  "); //������������ո�
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
		CString str_pack("  "); //������������ո�
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

//����JUDGE��Ϣ
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
			Log += "JUDGE��Ϣ���ͳ���\r\n";
			AfxMessageBox("JUDGE��Ϣ���ͳ���");
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


//�˺��������û����ƺ�ɾ��������
void CQQ_serverDlg::DeleteCard(int i , CString sendcard) 
{
	CString judgetemp = sendcard;
	int user_namelong = info[i].name.GetLength();
	CString delcard = judgetemp.GetAt(user_namelong);
	char *p ;
	p = delcard.GetBuffer(delcard.GetLength()); 
	delcard.ReleaseBuffer();
	int num = game_info[i].card.Remove(p[0]);     //ɾ�����ַ���
}

//�˺������ڼ����û�ÿ����Ϸ���ܵ÷�
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
		CString str_pack("  "); //������������ո�
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

//�����ܵ÷���Ϣ
void CQQ_serverDlg::Sendallscore(SOCKET sock, CString allscore)
{
	if( SOCKET_ERROR == send(sock,allscore,allscore.GetLength(),NULL) )
	{
		Log += "�ܵ÷���Ϣ���ͳ���\r\n";
		AfxMessageBox("�ܵ÷���Ϣ���ͳ���");
		return;
	}
}

void CQQ_serverDlg::GetTime()   //���ϵͳʱ��
{
	CString	Time = "";
	time_t rawtime;
	struct tm *ptminfo;  
	time(&rawtime); 
	ptminfo = localtime(&rawtime);
	int year = ptminfo->tm_year + 1900;   //ȡ�����
	int month = ptminfo->tm_mon + 1;   //ȡ���·�
	int day = ptminfo->tm_mday;  //ȡ������
	int hour = ptminfo->tm_hour;  //ȡ��Сʱ
	int mintue = ptminfo->tm_min;  //ȡ�÷���
	int second = ptminfo->tm_sec;  //ȡ������
	CString str = "";
	CString str_pack("  "); //������������ո�
	str.Format(_T("%d"), year);
	Time += str + "��";
	str.Format(_T("%d"), month);
	Time += str + "��";
	str.Format(_T("%d"), day);
	Time += str + "��";
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
    //��ʾ��Ҫ�������������serverlog.txt"����ļ��� ���û������ļ������Զ���������ļ�

	write << Log; 
    //����������Ҫ��������ݣ������������һ��string abc
	Log = "";
    write.close(); // �����Ϻ�ر�����ļ�
}
