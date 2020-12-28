; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CQQ_clientDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "QQ_client.h"

ClassCount=5
Class1=CQQ_clientApp
Class2=CQQ_clientDlg
Class3=CAboutDlg

ResourceCount=5
Resource1=IDD_QQ_CLIENT_DIALOG
Resource2=IDR_MAINFRAME
Resource3=IDD_ABOUTBOX
Class4=CTalkDlg
Resource4=IDD_TALK
Class5=GAME_RULE
Resource5=IDD_GAME_RULE

[CLS:CQQ_clientApp]
Type=0
HeaderFile=QQ_client.h
ImplementationFile=QQ_client.cpp
Filter=N

[CLS:CQQ_clientDlg]
Type=0
HeaderFile=QQ_clientDlg.h
ImplementationFile=QQ_clientDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_RECV

[CLS:CAboutDlg]
Type=0
HeaderFile=QQ_clientDlg.h
ImplementationFile=QQ_clientDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_QQ_CLIENT_DIALOG]
Type=1
Class=CQQ_clientDlg
ControlCount=15
Control1=IDCANCEL,button,1342242816
Control2=IDC_CONNECT,button,1342242816
Control3=IDC_LISTMEM,SysListView32,1350631426
Control4=IDC_STATIC,static,1342308352
Control5=IDC_USERNAME,edit,1350631552
Control6=IDC_RECV,edit,1352728580
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC1,static,1342308352
Control9=IDC_GAME_START,button,1342242816
Control10=IDC_GAME_RULEBTN,button,1342242816
Control11=IDC_STATIC,static,1342308352
Control12=IDC_SCORE,edit,1350631552
Control13=IDC_SendCard,edit,1350631552
Control14=IDC_SENDSURE,button,1342242816
Control15=IDC_STATIC,static,1342308352

[DLG:IDD_TALK]
Type=1
Class=CTalkDlg
ControlCount=3
Control1=IDC_SENDBTN,button,1342242816
Control2=IDC_SEND,edit,1352728580
Control3=IDC_STATIC,static,1342308352

[CLS:CTalkDlg]
Type=0
HeaderFile=TalkDlg.h
ImplementationFile=TalkDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CTalkDlg

[DLG:IDD_GAME_RULE]
Type=1
Class=GAME_RULE
ControlCount=6
Control1=IDC_STATIC,static,1342308364
Control2=IDC_KNOW,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352

[CLS:GAME_RULE]
Type=0
HeaderFile=GAME_RULE.h
ImplementationFile=GAME_RULE.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=GAME_RULE

