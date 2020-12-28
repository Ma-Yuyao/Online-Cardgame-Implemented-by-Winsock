// QQ_clientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "QQ_client.h"
#include "QQ_clientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

CString cst = "";  //Ҫ��ʾ�ڶԻ����е�����
CString card_info = ""; //�������е��� 
int ju = 1;
int lun = 0;  //�������������
CString chatinfo = ""; //��ʾ��������ҵ�������Ϣ
int times = 0; //���忪ʼ��Ϸ��ʾ����ʾ����

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
// CQQ_clientDlg dialog

CQQ_clientDlg::CQQ_clientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQQ_clientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CQQ_clientDlg)
	m_username = _T("");
	m_sendcard = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	index_image = 0;
}

void CQQ_clientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CQQ_clientDlg)
	DDX_Control(pDX, IDC_RECV, m_recv);
	DDX_Control(pDX, IDC_LISTMEM, m_listmem);
	DDX_Text(pDX, IDC_USERNAME, m_username);
	DDX_Text(pDX, IDC_SendCard, m_sendcard);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CQQ_clientDlg, CDialog)
	//{{AFX_MSG_MAP(CQQ_clientDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONNECT, OnConnect)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTMEM, OnDblclkListmem)
	ON_BN_CLICKED(IDC_GAME_RULEBTN, OnGameRulebtn)
	ON_BN_CLICKED(IDC_GAME_START, OnGameStart)
	ON_EN_CHANGE(IDC_SendCard, OnChangeSendCard)
	ON_BN_CLICKED(IDC_SENDSURE, OnSendsure)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SOCKET,OnSocket)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQQ_clientDlg message handlers

BOOL CQQ_clientDlg::OnInitDialog()
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
	//����ͼ���б�
	m_imagelist.Create(32,32,ILC_COLOR32,4,4);
	//���ͼ��
	for(int i = 0;i < 4;i++)
	{
		m_imagelist.Add( LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(130+i)) );
	}
	//�������б�ؼ���
	m_listmem.SetImageList(&m_imagelist,LVSIL_SMALL);

	GetDlgItem(IDC_SCORE)->EnableWindow(false);	//���û��������

	GetDlgItem(IDC_SendCard)->EnableWindow(false);	//���ó��������
	
	GetDlgItem(IDC_SENDSURE)->EnableWindow(false);	//���á����͡���ť

	//GetDlgItem(IDC_RECV)->EnableWindow(false);	//������Ϣ��ʾ�����

	GetDlgItem(IDC_GAME_START)->EnableWindow(false);	//���ÿ�ʼ��Ϸ��ť

	//m_inforecv.LineScroll(m_inforecv.GetLineCount());    //����Ϣ��ʾ��Ĭ����ʾ������Ϣ
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CQQ_clientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CQQ_clientDlg::OnPaint() 
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
HCURSOR CQQ_clientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CQQ_clientDlg::OnConnect() 
{
	// TODO: Add your control notification handler code here

	//�����׽��ֿ�
	WORD	ver = MAKEWORD(2,0);
	WSADATA	wsadata;
	if( WSAStartup(ver,&wsadata) )
	{
		AfxMessageBox("�����׽��ֿ�ʧ��");
		return;
	}
	
	//�����׽��֣�����TCP
	socket_client = socket(AF_INET,SOCK_STREAM,NULL);
	if( INVALID_SOCKET == socket_client )
	{
		AfxMessageBox("�����׽���ʧ��");
		return;
	}

	//���ӷ�����
	sockaddr_in	addr;
	addr.sin_family = AF_INET;
	addr.sin_port	= htons(3050);
	addr.sin_addr.S_un.S_addr = inet_addr("10.22.118.106");		//Ĭ�Ϸ�������IPֵ��ע�����ķ������ĺ�ע��Ҫ����
	if( SOCKET_ERROR == connect(socket_client,(sockaddr *)&addr,sizeof(sockaddr)) )
	{
		AfxMessageBox("����������ʧ��");
		return;
	}

	//�����첽�׽���
	if( SOCKET_ERROR == WSAAsyncSelect(socket_client,this->m_hWnd,WM_SOCKET,FD_READ) )
	{
		AfxMessageBox("�첽�׽������ó���");
		return;
	}

	//���Ϳͻ���������Ϣ
	UpdateData(true);
	if(m_username == "") //����û�������Ϊ��
	{
		AfxMessageBox("�û���������Ϊ�գ��������û��������ӷ�����");
		return;
	}
	else  //�û���������ȷ
	{
		CString str_name = "From:";
		str_name		+= m_username;
		str_name		+= "\r\n";
		from_name		 = m_username;					//����ͻ�������
		if( SOCKET_ERROR == send(socket_client,str_name,str_name.GetLength(),NULL) )
		{
			AfxMessageBox("������Ϣ���ͳ���");
			return;
		}
	}
	
}

//�ж��û����Ƿ�ʹ�ù�
void CQQ_clientDlg::Namenotused()
{
	GetDlgItem(IDC_USERNAME)->EnableWindow(false);	//�����û��������
	GetDlgItem(IDC_GAME_START)->EnableWindow(true);	//���ÿ�ʼ��Ϸ��ť
	AfxMessageBox("���ӷ������ɹ�");
	GetDlgItem(IDC_CONNECT)->EnableWindow(false);	//���á����ӷ���������ť
}

//�Զ�����Ӧ����
void CQQ_clientDlg::OnSocket(WPARAM wParam,LPARAM lParam)
{
	char buff_recv[512] = "";
	CString str_temp;
	switch(lParam)
	{
	case FD_READ:
		if( SOCKET_ERROR == recv(socket_client,buff_recv,512,NULL) )
		{
			AfxMessageBox("��Ϣ����ʧ��");
			return;
		}

		//����û�����ʹ�ù�
		if(buff_recv[0] == 'U')
		{
//			isNameUsed = true;
			AfxMessageBox("���û����ѱ�����ʹ�ã��뻻һ��");
			return;
		}
		else
		{
			if(times == 0)
				Namenotused();
			times ++;
		}
			
		
		//������������͹����ĳ�Ա��Ϣ
		if(buff_recv[0] == 'L')
		{
			str_temp = getKeyMsg(buff_recv,"List");
			//����ͼ���б�
			UpdateList(str_temp);
			return;
		}
		
		//����������Ա���͹�������Ϣ
		if(buff_recv[0] == 'T')
		{
			//��ȡ��������Ϣ
			str_temp = getKeyMsg(buff_recv,"From");
			to_name	 = str_temp;
			//SetDlgItemText(IDC_FROM,to_name);
			chatinfo += "����" + to_name + "����Ϣ:\r\n";
			
			//��ȡ��Ϣ����
			str_temp = getKeyMsg(buff_recv,"Context");
			chatinfo += str_temp + "\r\n\r\n";
			SetDlgItemText(IDC_RECV,chatinfo);
			
			return;
		}

		//������Card����Ϣ
		if(buff_recv[0] == 'N')
		{
			CString card_temp = "";
			CString str_pack("  "); //������������ո�
			//������ʾ����
			CString str = ""; //str�����������������
			lun++;  //����������
			str.Format(_T("%d"), ju);	
			str_temp += "�� " + str + " ��";
			str.Format(_T("%d"), lun);
			str_temp += "�� " + str + "  ��:";
			str_temp += "\r\n";
			str_temp += "��Ŀǰ�����е�����:";
			card_temp = getKeyMsg(buff_recv,"Name");
			card_info = getKeyMsg(buff_recv,"Name");
			for(int i = 0;i < card_temp.GetLength();i++)
			{
				str_temp += str_pack + card_temp.GetAt(i);
			}
//			str_temp += str_pack + card_temp.GetAt(0);
//			str_temp += str_pack + card_temp.GetAt(1); 
//			str_temp += str_pack + card_temp.GetAt(2);
			str_temp += "\r\n"; 
			str_temp += "����ѡ��Ҫ������:";
			SetMessage(str_temp);
//			SetDlgItemText(IDC_RECV,str_temp);

			return;
		}


		//����ÿһС�ֽ������Ϣ
		if(buff_recv[0] == 'J')
		{
			str_temp = getKeyMsg(buff_recv,"Judge");
			CString othercard = "";
			CString score = "";
			int oc_start = 0,oc_end = 0;
			int oc = 0;
			othercard += "������ҳ�����ϢΪ��";
			oc_end = str_temp.Find("Score:");
			for(oc = oc_start;oc < oc_end;oc++)
			{
				othercard += str_temp.GetAt(oc);
			}
			othercard += "\r\n";
			
			score += "Ŀǰ�÷���ϢΪ��";
			for(oc = oc_end;oc < str_temp.GetLength() ;oc++)
			{
				score += str_temp.GetAt(oc);
			}
			score += "\r\n";
			score += "\r\n";
			
			SetMessage(othercard);
			SetMessage(score);

			GetDlgItem(IDC_SendCard)->EnableWindow(true);	//���á����ơ������
			GetDlgItem(IDC_SENDSURE)->EnableWindow(true);   //���á����͡���ť
			return;
		}

		//����ÿһ���ֽ������Ϣ
		if(buff_recv[0] == 'A')
		{
			lun = 0;     //��������0
			ju ++;   //����������
			str_temp = getKeyMsg(buff_recv,"Allscore");
			CString allscore = "";
			allscore += "�þ���Ϸ����ܵ÷�Ϊ��";
			allscore += str_temp;
			allscore += "\r\n";
			allscore += "\r\n";
			SetScore(allscore);
			allscore += "-------------------------------------------------------------";
			allscore += "\r\n";
			allscore += "\r\n";
			SetMessage(allscore);
			return;
		}
		break;
	}
}
//�����б�ؼ���Ա
void CQQ_clientDlg::UpdateList(CString MemList)
{
	//��ɾ��ԭ�����б�ͼ��
	int nCount = m_listmem.GetItemCount();
	for (int j=0; j < nCount; j++)
	{
		m_listmem.DeleteItem(0);
	}

	//����µĳ�Ա
	index_image = 0;	//��ʼ��ͼƬ����
	CString name;
	for(int i=0;i<MemList.GetLength();i++)
	{
		if(MemList.GetAt(i) == ',' )
		{
			AddMem(name);
			name = "";
			continue;
		}
		name += MemList.GetAt(i);
	}
}

//�Խ��յ������ݽ��н���
CString CQQ_clientDlg::getKeyMsg(CString recv_msg,CString keyword)
{
	int index_start = 0,index_end = 0;
	int index = 0;
	CString str_temp = "";
	
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

	if("List" == keyword)
	{
		index_start = recv_msg.Find("List:");
		index_end	= recv_msg.Find("\r\n",index_start);
		for(index = index_start+5;index < index_end;index++)
		{
			str_temp += recv_msg.GetAt(index);
		}
		return str_temp;
	}

	if("Context" == keyword)
	{
		index_start = recv_msg.Find("Context:");
		index_end	= recv_msg.Find("\r\n.\r\n",index_start);
		for(index = index_start+8;index < index_end;index++)
		{
			str_temp += recv_msg.GetAt(index);
		}
		return str_temp;
	}

	//������Card���¼�
	if("Name" == keyword)
	{
		index_start = recv_msg.Find("Name:");
		index_end	= recv_msg.Find("\r\n",index_start);
		for(index = index_start+5;index < index_end;index++)
		{
			str_temp += recv_msg.GetAt(index);
		}

//		AfxMessageBox(str_temp);
		int card_start = 0,card_end = 0;
		int card = 0;
		CString card_temp = "";

		m_username.ReleaseBuffer();
		card_start = str_temp.Find(m_username);
		card_end = str_temp.Find("end",card_start);

		/*
		CString len;
		len.Format(_T("%d"),card_end);
		AfxMessageBox(len);
		*/
		
		for(card = card_start + m_username.GetLength();card < card_end;card++)
		{
			card_temp += str_temp.GetAt(card);
		}
		return card_temp;
	}

	//����ÿһС�ֽ�����¼�
	if("Judge" == keyword)
	{
		CString str_pack("   "); //������������ո�
		index_start = recv_msg.Find("Judge:");
		index_end	= recv_msg.Find("\r\n",index_start);
		for(index = index_start+6;index < index_end;index++)
		{
			str_temp += recv_msg.GetAt(index);
		}
		return str_temp;
	}

	//����ÿһ���ֵĽ��
	if("Allscore" == keyword)
	{
		index_start = recv_msg.Find("Allscore:");
		index_end	= recv_msg.Find("\r\n",index_start);
		for(index = index_start+9;index < index_end;index++)
		{
			str_temp += recv_msg.GetAt(index);
		}
		return str_temp;
	}
	return "";
}

void CQQ_clientDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	closesocket(socket_client);

	WSACleanup();
	CDialog::OnCancel();
}

//���б�ؼ�����ӳ�Ա
void CQQ_clientDlg::AddMem(CString name)
{
	LVITEM lvitem;
	memset(&lvitem,0,sizeof(LVITEM));
	lvitem.mask = LVIF_IMAGE | LVIF_TEXT;
	lvitem.pszText = name.GetBuffer(0);
	lvitem.iImage = index_image % 4;	//ѭ��ʹ��4��ͼƬ
	
	m_listmem.InsertItem(&lvitem);
	index_image++;
}

void CQQ_clientDlg::OnDblclkListmem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_listmem.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		AfxMessageBox("No items were selected!\n");
		return;
	}
	else
	{
		int nItem = m_listmem.GetNextSelectedItem(pos);
		to_name = m_listmem.GetItemText(nItem,0);
		m_dlg.DoModal();
	}

	*pResult = 0;
}


void CQQ_clientDlg::OnGameRulebtn() 
{
	m_GR_dlg.DoModal();   //��ʾGAME_RULE�Ի���
	// TODO: Add your control notification handler code here
	
}

void CQQ_clientDlg::OnGameStart() 
{
	// TODO: Add your control notification handler code here
	CString send_msg = "StartGame";
	/*
	send_msg += m_username;
	send_msg += "\r\n";
	*/
	if( SOCKET_ERROR == send(socket_client,send_msg,send_msg.GetLength(),NULL) )
	{
		int err_code = WSAGetLastError();
	}
	else
		AfxMessageBox("��ʼ��Ϸ�ɹ�����ȴ�������ҿ�ʼ��Ϸ");
	//���á���ʼ��Ϸ����ť
	GetDlgItem(IDC_GAME_START)->EnableWindow(false);

	GetDlgItem(IDC_SendCard)->EnableWindow(true);	//���ó��������

	GetDlgItem(IDC_SENDSURE)->EnableWindow(true);	//���á����͡���ť
	
	SetDlgItemText(IDC_SCORE,"0");     //��������Ϊ��0��

	SetDlgItemText(IDC_STATIC1,"��Ϸ���棺");   //����������棺��ת��Ϊ����Ϸ���棺��

	SetDlgItemText(IDC_FROM,"");		//������Դ����Ϊ��

	GetDlgItem(IDC_LISTMEM)->EnableWindow(false);	//����LIST�����
}



void CQQ_clientDlg::OnSendsure() 
{
	// TODO: Add your control notification handler code here
	CString send_msg = "Card:";
	send_msg += m_username;
	UpdateData(true);


	if(m_sendcard.GetLength() == 0)
	{
		AfxMessageBox("����ѡ��Ϊ�գ�����ȷ����������ȷ����!");
		SetDlgItemText(IDC_SendCard,"");
	}
	else
	{
		char *p ;
		p = m_sendcard.GetBuffer(m_sendcard.GetLength()); 
		m_sendcard.ReleaseBuffer();
		
		if (card_info.Find(p[0]) != -1)  //��������������ȷ�ģ����е��ƣ�
		{
			
			//��������������༭����
			CString str_pack("  "); //������������ո�
			CString sendcard_msg = "";
			sendcard_msg += str_pack;
			sendcard_msg += m_sendcard;
			sendcard_msg += str_pack;
			sendcard_msg += "��ȴ�������ҳ���";
			sendcard_msg += "\r\n";
			SetMessage(sendcard_msg);
			
			//������������÷��ͺͱ༭��ֱ��������ҷ��ͳɹ���ָ�
			GetDlgItem(IDC_SendCard)->EnableWindow(false);	//���á����ơ������
			GetDlgItem(IDC_SENDSURE)->EnableWindow(false);  //���á����͡���ť
			
			//���Ʒ��͵�������
			send_msg += m_sendcard;
			send_msg += "\r\n";
			if	(SOCKET_ERROR == send(socket_client,
				send_msg,send_msg.GetLength(),NULL) )
			{
				int err_code = WSAGetLastError();
			}
			p = "";
		}
		else	//�����������Ǵ���ģ�û�������ƣ�
		{
			AfxMessageBox("��û�������ƣ�������������ȷ����!");
			SetDlgItemText(IDC_SendCard,"");
		}
	}
	SetDlgItemText(IDC_SendCard,"");  //���������ı�
}

//���ý����ʾ��
void CQQ_clientDlg::SetMessage(CString msg)
{
	cst += msg;
	
	SetDlgItemText(IDC_RECV,cst);

	m_recv.LineScroll( m_recv.GetLineCount() ); //ʹ�����ʾ��Ĭ�ϻ��������·�
	
}

//���û�����ʾ��
void CQQ_clientDlg::SetScore(CString msg)
{
	CString score = "";
	int index_start = 0,index_end = 0;
	int index = 0;
	m_username.ReleaseBuffer();
	index_start = msg.Find(m_username);
	index_end	= msg.Find("  ",index_start + m_username.GetLength() + 2);
	for(index = index_start+m_username.GetLength();index < index_end;index++)
	{
		score += msg.GetAt(index);
	}

	SetDlgItemText(IDC_SCORE,score);
}
