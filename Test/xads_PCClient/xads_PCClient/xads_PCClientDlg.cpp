
// xads_PCClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "xads_PCClient.h"
#include "xads_PCClientDlg.h"
#include "afxdialogex.h"
#include "CApplication.h"
#include "CFont0.h"
#include "CRange.h"
#include "CWorkbook.h"
#include "CWorksheet.h"
#include "CWorkbooks.h"
#include "CWorksheets.h"
#include "My_Edit.h"
#include "My_Edit1.h"
My_Edit my_edit1;
My_Edit1 my_edit2;
My_Edit my_edit3;
char DataLengthSTR[20];
float HeartTime = 3000;
float TimeOut = 60000;
int SerialNum = 1;
HANDLE SendlistHandl,ManagerHandl;
HANDLE CheckHeartHandl;
HANDLE HeartHandl;
HANDLE ConnectHandle;
int DataLength, Counter, IMEILength, TimeDtrLength, m_Month, m_Day, SendSum, RecSum;
CString GetOnBag, HeartBag, CheckBit, DataLengthStr, IMEI, HeartTimeStr, TimeDtr, m_MonthStr, m_DayStr,IMEIStr;
CString StartBit = _T("7979");
CString EndBit = _T("0d0a");
CString ProtocolNum = _T("70");
CString SerialNumStr = _T("0001");
BOOL HeartTimeChangeFlag = FALSE;
BOOL CheckHeartFlag = TRUE;
BOOL SendHeartFlag = TRUE;
BOOL ConnectFlag = TRUE;
BOOL ManagerFlag = TRUE;
BOOL BeginSendHeartFlag = FALSE;
//BOOL SendListFlag = TRUE;
CString strIp;
DWORD dPort;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Cxads_PCClientDlg �Ի���




Cxads_PCClientDlg::Cxads_PCClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cxads_PCClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_ClientSock = NULL;
	isServerOn = FALSE;
}

void Cxads_PCClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RECDATA, m_RecData);
	DDX_Control(pDX, IDC_LIST1, m_correct);
	DDX_Control(pDX, IDC_EDIT4, m_error);
	DDX_Control(pDX, IDC_SENDTIME, my_edit1);
	DDX_Control(pDX, IDC_EDITPORT, M_Port);
	DDX_Control(pDX, IDC_IMEI, my_edit2);
	DDX_Control(pDX, IDC_TIMEOUT, my_edit3);

	DDX_Control(pDX, IDC_RECBUFF, m_RecBuff);
}

BEGIN_MESSAGE_MAP(Cxads_PCClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTONCONNECT, &Cxads_PCClientDlg::OnBnClickedButtonconnect)
	ON_BN_CLICKED(IDC_BUTTONSTOP, &Cxads_PCClientDlg::OnBnClickedButtonstop)
	ON_BN_CLICKED(IDC_BUTTONQUIT, &Cxads_PCClientDlg::OnBnClickedButtonquit)
	ON_BN_CLICKED(IDC_BUTTONSEND, &Cxads_PCClientDlg::OnBnClickedButtonsend)
	ON_WM_SETFOCUS()
	ON_WM_SETFOCUS()
	ON_EN_CHANGE(IDC_EDITSENDBOX, &Cxads_PCClientDlg::OnEnChangeEditsendbox)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_SENDTIME, &Cxads_PCClientDlg::OnEnChangeSendtime)
	
	ON_BN_CLICKED(IDC_BUTTON4, &Cxads_PCClientDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &Cxads_PCClientDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_SENDERROR, &Cxads_PCClientDlg::OnBnClickedSenderror)
	ON_BN_CLICKED(IDC_BUTTON2, &Cxads_PCClientDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// Cxads_PCClientDlg ��Ϣ�������

BOOL Cxads_PCClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	SetDlgItemText(IDC_IPADDRESS,_T("120.77.221.153"));
	SetDlgItemText(IDC_EDITPORT,_T("8050"));
	::HideCaret(GetDlgItem(IDC_EDITREVBOX)->GetSafeHwnd());
	CRect rect;
	m_correct.GetClientRect(&rect);
	// Ϊ�б���ͼ�ؼ����ȫ��ѡ�к�դ����   
	m_correct.SetExtendedStyle(m_correct.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_correct.InsertColumn(0, _T("����"), LVCFMT_CENTER, rect.Width() / 6, 0);
	m_correct.InsertColumn(1, _T("ά��"), LVCFMT_CENTER, rect.Width() / 6, 0);
	m_correct.InsertColumn(2, _T("ʣ������"), LVCFMT_CENTER, rect.Width() / 6, 0);
	m_correct.InsertColumn(3, _T("�ϳ�����"), LVCFMT_CENTER, rect.Width() / 6, 0);
	m_correct.InsertColumn(4, _T("�³�����"), LVCFMT_CENTER, rect.Width() / 6, 0);
	m_correct.InsertColumn(5, _T("ʱ����"), LVCFMT_CENTER, rect.Width() / 6, 0);
	SetDlgItemText(IDC_SENDTIME,_T("3"));
	SetDlgItemText(IDC_TIMEOUT,_T("60"));
	
	return FALSE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void Cxads_PCClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void Cxads_PCClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR Cxads_PCClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/******************���߳����ӷ�����************************/
void Cxads_PCClientDlg::OnBnClickedButtonconnect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    SendHeartFlag = TRUE;
	ConnectFlag = TRUE;
	ManagerFlag = TRUE;
	ManagerHandl = CreateThread(0, 0, ManagerThread, this, 0, NULL);
	ConnectHandle = CreateThread(0, 0, ConnectNetworkThread, this, 0, NULL);
	if (!CheckHeartHandl)
	{
		CheckHeartHandl = CreateThread(0, 0, CheckHeartThread, this, 0, NULL);
	}
	if (!CheckHeartFlag)
	{
		ResumeThread(CheckHeartHandl);
	}	
	EnableWindow(IDC_BUTTONSEND,TRUE);
	EnableWindow(IDC_SENDERROR,TRUE);
	EnableWindow(IDC_SENDTIME,TRUE);
	EnableWindow(IDC_TIMEOUT,TRUE);
	EnableWindow(IDC_EDITPORT,FALSE);
	EnableWindow(IDC_IPADDRESS, FALSE);
	EnableWindow(IDC_BUTTON4,TRUE);
	EnableWindow(IDC_IMEI,FALSE);

}

/******************�̹߳���********************/
DWORD WINAPI ManagerThread(LPVOID lpParameter)
{
	Cxads_PCClientDlg * pClient = (Cxads_PCClientDlg *)lpParameter;
	while (ManagerFlag)
	{
		if (pClient->isServerOn&&!HeartHandl)
		{
			HeartHandl = CreateThread(0, 0, SendHeartThread, pClient, 0, NULL);
			ManagerFlag = FALSE;
		}
		Sleep(200);
	}
	return 0;
}

/*******************�̺߳���*****************************/
DWORD WINAPI ConnectNetworkThread(  LPVOID lpParameter)
{
	Cxads_PCClientDlg * pClient = (Cxads_PCClientDlg *)lpParameter;
	if(pClient->ConnectSocket(pClient))
	{
	
	}
	return 0;
}

/***************************���ӷ���������************************************/
#define MAX_BUFF 10240
char szMsg[MAX_BUFF] = { 0 };
int iRead;
CString RecstrMsg;
sockaddr_in sa;
BOOL Cxads_PCClientDlg::ConnectSocket(Cxads_PCClientDlg * pClient)
{
	m_ClientSock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);//����socket
	if (NULL == m_ClientSock)
	{
		MessageBox(_T("����socketʧ��"));
		return FALSE;
	}
	//int flag = 0;
	
	sa.sin_family = AF_INET;
	dPort = GetDlgItemInt(IDC_EDITPORT);
	GetDlgItemText(IDC_IPADDRESS,strIp);
	if (strIp == _T("0.0.0.0") )
	{
		MessageBox(_T("��������ȷIP��ַ��˿ں�"));
		EnableWindow(IDC_EDITPORT, TRUE);
		EnableWindow(IDC_IPADDRESS, TRUE);
		return FALSE;
	}
	if ((dPort >= 65535 || dPort < 1024) || dPort == 0)
	{
		MessageBox(_T("������1024-65535��Ķ˿ں�"));
		EnableWindow(IDC_EDITPORT, TRUE);
		EnableWindow(IDC_IPADDRESS, TRUE);
		SetDlgItemText(IDC_EDITPORT,_T(""));
		return FALSE;
	}
	sa.sin_port = htons(dPort);
	char szIpAdd[32];
	USES_CONVERSION; //��������ʹ��T2A
	sprintf_s((LPTSTR)szIpAdd,32,"%s",strIp);
	sa.sin_addr.S_un.S_addr = inet_addr(szIpAdd);//��ַת��Ϊ������
	if (SOCKET_ERROR == connect(m_ClientSock,(sockaddr *)&sa,sizeof(sa)))
	{
		MessageBox(_T("���ӿͻ��˴���,��������д��IP�Ͷ˿��Ƿ����"));
		return FALSE;
	}
	pClient->SetRevBoxText(_T("���ӷ������ɹ�\r\n"));
	pClient->EnableWindow(IDC_BUTTONSTOP,TRUE);
	pClient->EnableWindow(IDC_BUTTONCONNECT,FALSE);
	SendData(GetOnBag);
	Sleep(600);
	isServerOn = TRUE;
	if (SendlistHandl)
	{
		ResumeThread(SendlistHandl);
	}	
	while (ConnectFlag)         //����
	{
		if (socket_Select(m_ClientSock,100,TRUE))
		{		
			char p_StrBcd[MAX_BUFF] = { 0};
			iRead = recv(m_ClientSock, szMsg, MAX_BUFF, 0);   //��ȡ���������ݰ�
			if (iRead > 0)
			{
				Sdk_Str2BcdStr(szMsg,16, p_StrBcd);
				RecstrMsg = p_StrBcd;
				pClient->SetRevBoxText(_T("������ >>") + RecstrMsg);
				RecSum += 1;
				SetDlgItemInt(IDC_RECSUM, RecSum);
				m_RecBuff.SetSel(-1, -1);
				m_RecBuff.ReplaceSel(RecstrMsg+_T("\r\n"));
				SYSTEMTIME time;
				CString strTime;
				GetLocalTime(&time);
				strTime.Format(_T("(%d/%2d/%2d %02d:%02d:%02d)\n"), time.wYear, time.wMonth, time.wDay,
				time.wHour, time.wMinute,time.wSecond);
			} 
			else
			{
				pClient->SetRevBoxText(_T("�Ѷ��ߣ���������"));
				isServerOn = FALSE;
				return TRUE;
			}
		}
	}
	return 0;
}

/************��ʮ��������ת��Ϊ�ַ�����0x0A->"0A"**********************/
void Sdk_Int2Char(int p_nNum, char *p_Ch)
{
	if (p_nNum <= 9)
	{
		*p_Ch = p_nNum + '0';
	}
	else
	{
		/*0 - 9 ��ʮ����*/
		*p_Ch = (p_nNum - 10) + 'A';
	}
}
void Sdk_Str2BcdStr(char *p_Str, int p_nLen, char *p_StrBcd)
{
	int i = 0, j = 0;

	for (i = 0; i < p_nLen; ++i)
	{
		Sdk_Int2Char((p_Str[i] >> 4) & 0x0F, &p_StrBcd[j++]);
		Sdk_Int2Char(p_Str[i] & 0x0F, &p_StrBcd[j++]);
	}
	return;
}


BOOL Cxads_PCClientDlg::EnableWindow(DWORD DlgId, BOOL bUsed)
{
	return GetDlgItem(DlgId)->EnableWindow(bUsed);
}

/******************�ж��Ƿ�����************************/
BOOL socket_Select(SOCKET hSocket,DWORD nTimeOut,BOOL bRead)
{
	FD_SET fdset;
	timeval tv;
	FD_ZERO(&fdset);
	FD_SET(hSocket,&fdset);
	nTimeOut = nTimeOut > 1000 ? 1000 : nTimeOut;
	tv.tv_sec = 0;
	tv.tv_usec = nTimeOut;
	int iRet = 0;
	if (bRead)
	{
		iRet = select(0,&fdset,NULL,NULL,&tv);
	} 
	else
	{
		iRet = select(0,NULL,&fdset,NULL,&tv);
	}
	if (iRet <= 0)
	{
		return FALSE;
	} 
	else if (FD_ISSET(hSocket,&fdset))
	{
		return TRUE;
	}
	return FALSE;
}

/******************�����ݲ��뵽������ʾ�����һ��****************/
void Cxads_PCClientDlg::SetRevBoxText(CString strMsg)
{
	CString timeStr;
	timeStr = GetTime();
	m_RecData.SetSel(-1, -1);
	m_RecData.ReplaceSel(timeStr + _T("\r\n") + strMsg + _T("\r\n"));
	WritetoTxt(timeStr + _T("\r\n") + strMsg + _T("\r\n"));
	m_RecData.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
}

/******��ȡ��ǰʱ��*********/
CString GetTime()
{
	CTime time = CTime::GetCurrentTime();
	CString Tiemstr;
	m_Day = time.GetDay();
	m_Month = time.GetMonth();
	m_DayStr.Format(_T("%d"),m_Day);
	m_MonthStr.Format(_T("%d"), m_Month);
	Tiemstr = time.Format(_T("%Y-%m-%d  %H:%M:%S"));
	return Tiemstr;
}

/***ֹͣ��ť���ر�socket���ӣ����÷��ͺ�ֹͣ��ť***/
void Cxads_PCClientDlg::OnBnClickedButtonstop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	closesocket(m_ClientSock);
	Sleep(100);
	ManagerFlag = FALSE;
	ConnectFlag = FALSE;
	SendHeartFlag = FALSE;
	isServerOn = FALSE;
	SuspendThread(SendlistHandl);
	SuspendThread(CheckHeartHandl);
	Counter = 0;
	CheckHeartFlag = FALSE;
	EnableWindow(IDC_BUTTONCONNECT, TRUE);
	EnableWindow(IDC_BUTTONSEND, FALSE);
	EnableWindow(IDC_BUTTONSTOP, FALSE);
	EnableWindow(IDC_SENDERROR, FALSE);
	EnableWindow(IDC_BUTTON4, FALSE);
	EnableWindow(IDC_SENDTIME, FALSE);
	EnableWindow(IDC_TIMEOUT, FALSE);
	EnableWindow(IDC_EDITPORT, TRUE);
	EnableWindow(IDC_IPADDRESS, TRUE);
	EnableWindow(IDC_IMEI, TRUE);
	SetDlgItemText(IDC_EDITSENDBOX, _T(" "));
}

/**************�˳���ť���رմ��ں�socket����*********/
void Cxads_PCClientDlg::OnBnClickedButtonquit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnBnClickedButtonstop();
	SendMessage(WM_CLOSE);
}

/****************���Ͱ�ť******************/
void Cxads_PCClientDlg::OnBnClickedButtonsend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strGetMsg;	
	int strLength,DestLength;
	int iWrite;
	GetDlgItemText(IDC_EDITSENDBOX, strGetMsg);
	strLength = strGetMsg.GetLength();
	strGetMsg = strRemove(strGetMsg, strLength);
	DestLength = strGetMsg.GetLength();
	char buffer[1024] = {0};
	HexStr2Byte(strGetMsg, buffer, strLength);
	iWrite = send(m_ClientSock, buffer, DestLength / 2, 0);
	if(SOCKET_ERROR == iWrite){
		SetRevBoxText(_T("���ʹ���"));
	}
	SetRevBoxText(_T("�ն�>>") + strGetMsg);
	return; 
}

/***********************����*****************************/
void Cxads_PCClientDlg::SendData(CString GetOnData)
{
	CString strGetMsg = GetOnData;
	int strLength, DestLength;
	int iWrite;
	//GetDlgItemText(IDC_EDITSENDBOX, strGetMsg);
	strLength = strGetMsg.GetLength();
	strGetMsg = strRemove(strGetMsg, strLength);
	DestLength = strGetMsg.GetLength();
	char buffer[10240] = { 0 };
	HexStr2Byte(strGetMsg, buffer, strLength);
    iWrite = send(m_ClientSock, buffer, DestLength / 2, 0);
	if (iWrite)
	{
		SerialNum += 1;
		char temp[20];
		_itoa_s(SerialNum, temp, sizeof(temp), 16);
		SerialNumStr.Format("%04s", temp);	
		SendSum += 1;
		SetDlgItemInt(IDC_SENDSUM, SendSum);
	}
	if (SOCKET_ERROR == iWrite)
		{
			SetRevBoxText(_T("���ʹ���"));
		}
		SetRevBoxText(_T("�ն� >>") + strGetMsg);
	return;
}

/********************�����������̺߳���**********************/
DWORD WINAPI SendHeartThread(LPVOID lpParameter)
{
	Cxads_PCClientDlg * pClient = (Cxads_PCClientDlg *)lpParameter;
	if (pClient->SendHeartData(pClient))
	{

	}
	return 0;
}
bool Cxads_PCClientDlg::SendHeartData(Cxads_PCClientDlg * pClient)
{
	/*if (HeartTimeChangeFlag == TRUE)
	{
		Sleep(HeartTime);
	}*/
	while (isServerOn)
	{
		HeartTimeChangeFlag = FALSE;
		HeartBag = _T("700000000101");
		HeartBag += SerialNumStr;
		DataLength = HeartBag.GetLength();
		DataLength = DataLength / 2 + 2;
		_itoa_s(DataLength, DataLengthSTR, sizeof(DataLengthSTR), 16);
		DataLengthStr.Format(_T("%04s"), DataLengthSTR);
		HeartBag = DataLengthStr + HeartBag;
		int t_iLength = HeartBag.GetLength();
		CheckBit = ReturnCheckBit(t_iLength, HeartBag);
		HeartBag += CheckBit;
		HeartBag += _T("0d0a");
		HeartBag = StartBit + HeartBag;
		SendData(HeartBag);
		BeginSendHeartFlag = TRUE;
		Sleep(HeartTime);
	}
	HeartHandl = NULL;
	return 0;
}

int CountTime;
CString CheckStr;
/**********************��ʱ���***********************/
DWORD WINAPI CheckHeartThread(LPVOID lpParameter)
{
	Cxads_PCClientDlg * pClient = (Cxads_PCClientDlg *)lpParameter;
	if (pClient->CheckHeart(pClient))
	{

	}
	return 0;
}
bool Cxads_PCClientDlg::CheckHeart(Cxads_PCClientDlg * pClient)
{
	while (TRUE)
	{	
		if (BeginSendHeartFlag)
		{
			GetDlgItemText(IDC_RECBUFF, CheckStr);
			int a = CheckStr.Find("7979000A700000000101");
			if (a == -1)
			{
				Counter += 1;
			}
			else
			{
				Counter = 0;
				SetDlgItemText(IDC_RECBUFF, _T(""));
				BeginSendHeartFlag = FALSE;

			}
			CountTime = Counter * 1000;
			if (CountTime >= TimeOut)
			{
				/*SuspendThread(HeartHandl);
				SuspendThread(SendlistHandl);*/
				closesocket(m_ClientSock);
				ManagerFlag = FALSE;
				ConnectFlag = FALSE;
				SendHeartFlag = FALSE;
				isServerOn = FALSE;
				HeartHandl = NULL;
				Sleep(600);
				OnBnClickedButtonconnect();
				Counter = 0;
				BeginSendHeartFlag = FALSE;
			}
		}
	
		Sleep(1000);
	}
	return TRUE;
}


/******************����excel�е����ݰ�********************/
DWORD WINAPI SendListDataThread(LPVOID lpParameter)
{
	Cxads_PCClientDlg * pClient = (Cxads_PCClientDlg *)lpParameter;
	if (pClient->SendListData(pClient))
	{

	}
	return 0;
}
bool Cxads_PCClientDlg::SendListData(Cxads_PCClientDlg * pClient)
{
	int RowCount = m_correct.GetItemCount();
	CString strMsg, DestMsg, DataMsg, HexTimeStampStr, CheckBit, Lat, PasteBag;
	int SendTime, Row, col, DestMsgLength ,DataMsgLength,PersonNum;
	float IntstrMsg;
	char DtempGps[20];
	while (isServerOn)
	{
		for (Row = 0; Row < RowCount; Row++)
		{
			for (col = 0; col < 6; col++)
			{
				if (col >= 0 && col <= 4)
				{
					strMsg = m_correct.GetItemText(Row, col);
					if (col>=0&&col<=1)
					{												
						IntstrMsg = atof(strMsg);
						IntstrMsg *= 1800000;
						_itoa(IntstrMsg, DtempGps, 16);
						strMsg.Format(_T("%08s"), DtempGps);						
					}	
					DestMsg += strMsg;
				}				
				if (col == 5)
				{
					SendTime = _ttoi(m_correct.GetItemText(Row, col));
				}
			}
			DestMsgLength = DestMsg.GetLength();
			if (DestMsgLength / 2 == 8)
			{
				Lat = DestMsg.Right(8);
				DestMsg = Lat + DestMsg.Left(8);
				HexTimeStampStr = GetTimeStamp();
				DataMsg = _T("70000000010200330012") + HexTimeStampStr;
				DataMsg += _T("070028");
				DataMsg += DestMsg;
				DataMsg += _T("001400");
				DataMsg += SerialNumStr;
				DataMsg = _T("0020") + DataMsg;
				DataMsgLength = DataMsg.GetLength();
				CheckBit = ReturnCheckBit(DataMsgLength, DataMsg);
				DataMsg += CheckBit;
				DataMsg = _T("7979") + DataMsg;
				DataMsg += _T("0d0a");
			}
			else
			{
				DataMsg = _T("000f700000000104001f000100");
				DataMsg += SerialNumStr;
				DataMsgLength = DataMsg.GetLength();
				CheckBit = ReturnCheckBit(DataMsgLength, DataMsg);
				DataMsg += CheckBit;
				DataMsg = _T("7979") + DataMsg;
				DataMsg += "0d0a";
			}
			if (SendTime==0)
			{
				PasteBag = PasteBag + DataMsg;
				SendSum += 1;
			}
			else
			{
				if (PasteBag!="")
				{
					//SendSum += 1;
					SendData(PasteBag + DataMsg);
				}
				else
				{
					SendData(DataMsg);
				}				
				DestMsg = _T("");
				DataMsg = _T("");
				PasteBag = _T("");
			}				
			Sleep(SendTime);
		}
	}	
	return TRUE;
}

/*****************���ַ�����Ŀո�ȥ��*************************/
CString Cxads_PCClientDlg::strRemove(CString str, int length)
{
	CString Dest = "";
	for (int i = 0; i<length; i++)
		if (str[i] != ' ')Dest += str[i];
	return Dest;
}

/**********ʮ�����������ַ�����ʽת�����ֽ���  "7979"->buffer{0x79,0x79}**********/
void Cxads_PCClientDlg::HexStr2Byte(const char* source,  char* dest, int sourceLen)
{
	short i;
	unsigned char highByte, lowByte;
	for (i = 0; i < sourceLen; i += 2)
	{
		highByte = toupper(source[i]);
		lowByte = toupper(source[i + 1]);
		if (highByte > 0x39)
			highByte -= 0x37;
		else
			highByte -= 0x30;
		if (lowByte > 0x39)
			lowByte -= 0x37;
		else
			lowByte -= 0x30;
		dest[i/2] = (highByte << 4) | lowByte;
	}
	return;
}

/**************�ж����ݷ������Ƿ�������******************/
void Cxads_PCClientDlg::OnEnChangeEditsendbox()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//CString strMsg;
	//GetDlgItemText(IDC_EDITSENDBOX,strMsg);
	//if (_T("") == strMsg || !isServerOn)
	//{
	//	EnableWindow(IDC_BUTTONSEND,FALSE);
	//}
	//else 
	//{
	//	EnableWindow(IDC_BUTTONSEND,TRUE);
	//}
}

/****************�ж��Ƿ�������ʱ��******************/
int Edittime;
void Cxads_PCClientDlg::OnEnChangeSendtime()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

/******************��ʱ����Ӧ����*******************/

void Cxads_PCClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch (nIDEvent)
	{
	case 1:
		OnBnClickedButtonsend();
	/*case 2:
		REC = recv(m_ClientSock, RecHeart, MAX_BUFF, 0);
		if (REC>0)
		{
			Counter = 0;
		}
		else
		{
			Counter += 1;
		}
		CountTime = Counter*HeartTime;
		if (CountTime>=TimeOut)
		{
			OnBnClickedButtonconnect();
		}
		else break;*/
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

/*****************���ݰ�����*****************/
void Cxads_PCClientDlg::OnBnClickedButton4()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (!SendlistHandl)
	{
		SendlistHandl = CreateThread(0, 0, SendListDataThread, this, 0, NULL);
	}
	else
	{
		ResumeThread(SendlistHandl);
	}
	EnableWindow(IDC_BUTTON4,FALSE);
}

/***************ֹͣ����******************/
void Cxads_PCClientDlg::OnBnClickedButton2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	SuspendThread(SendlistHandl);
	EnableWindow(IDC_BUTTON4, TRUE);
}


/*****************��ȡ���****************/
void Cxads_PCClientDlg::OnBnClickedButton5()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	bool ReadFlag = OpenExcel(2);
	if (ReadFlag)
	{
		MessageBox(_T("��ȡ�ɹ���"));
	}
	else
	{
		MessageBox(_T("��ȡʧ�ܣ�"));
	}
}

/*********************�������***********************/
void Cxads_PCClientDlg::OnBnClickedSenderror()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	char errorBuffer[200] = {0};
	CString errorData;
	int lineNum = rand() % 7 + 0;
	m_error.GetLine(lineNum, (LPTSTR)errorBuffer, sizeof(errorBuffer));
	errorData = errorBuffer;
	SendData(errorData);
}

/********************�������������ʱ��*********************/
BOOL Cxads_PCClientDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  �ڴ����ר�ô����/����û���
	
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if (GetFocus()->GetDlgCtrlID() == IDC_SENDTIME)//���»س��������ǰ���������Լ������Ŀؼ���
		{
			// ���������£�������»س�ʱ����������Ҫ�Ŀؼ���		
			GetDlgItemText(IDC_SENDTIME, HeartTimeStr);
			CString str = HeartTimeStr.Right(1);
			if (str == ".")
			{
				MessageBox(_T("ʱ�䲻����С�����β"));
				return FALSE;
			}
			if (str.GetLength() != 0 && HeartTimeStr != "0")
			{
				MessageBox(_T("���óɹ�"));
			}
			else
			{
				MessageBox(_T("����ʱ�䲻��Ϊ��"));
				return FALSE;
			}
			HeartTime = atof(HeartTimeStr);
			HeartTime = 1000 * HeartTime;		
			if (HeartHandl)
			{
				TerminateThread(HeartHandl,0);
				HeartHandl = NULL;
				HeartTimeChangeFlag = TRUE;
				HeartHandl = CreateThread(0, 0, SendHeartThread, this, 0, NULL);
			}
		}
		if (GetFocus()->GetDlgCtrlID() == IDC_IMEI)//���»س��������ǰ���������Լ������Ŀؼ���
		{
			GetDlgItemText(IDC_IMEI, IMEI);
			IMEILength = IMEI.GetLength();
			if (IMEILength>15 || IMEI==" ")
			{
				MessageBox(_T("�����IMEI������"));
				return FALSE;
			}
			IMEIStr.Format(_T("%016s"), IMEI);
            bool ReadFlag = OpenExcel(1);
			if (ReadFlag)
			{
				GetOnBag += _T("70000000010000010008");
				GetOnBag += IMEIStr;
				GetOnBag += SerialNumStr;
				DataLength = GetOnBag.GetLength();
				DataLength = DataLength / 2 + 2;
				_itoa_s(DataLength,DataLengthSTR,sizeof(DataLengthSTR),16);
				DataLengthStr.Format(_T("%04s"), DataLengthSTR);
				GetOnBag = DataLengthStr + GetOnBag;
				int t_iLength = GetOnBag.GetLength();
				CheckBit = ReturnCheckBit(t_iLength, GetOnBag);
				/*CheckBit.Format(_T("%04X"), t_uiCrcRet);
				CheckBit = CheckBit.Right(4);*/
				GetOnBag += CheckBit;
				GetOnBag += _T("0d0a");
				GetOnBag = StartBit + GetOnBag;
				MessageBox(_T("��ȡ�ɹ�!"));
				EnableWindow(IDC_BUTTONCONNECT, TRUE);
				EnableWindow(IDC_BUTTON4, TRUE);
			}
			else
			{
				MessageBox(_T("��ȡʧ��!"));
			}
		}
		if (GetFocus()->GetDlgCtrlID() == IDC_TIMEOUT)
		{
			GetDlgItemText(IDC_TIMEOUT, TimeDtr);
			CString str = TimeDtr.Right(1);
			if (str == ".")
			{
				MessageBox(_T("ʱ�䲻����С�����β"));
				return FALSE;
			}
			if (str.GetLength() != 0 && TimeDtr != "")
			{
				MessageBox(_T("���óɹ�"));
			}
			else
			{
				MessageBox(_T("����ʱ�䲻��Ϊ��"));
				return FALSE;
			}
			TimeOut = atof(TimeDtr);
			TimeOut *= 1000;
			/*if (!CheckHeartHandl)
			{
				CheckHeartHandl = CreateThread(0, 0, CheckHeartThread, this, 0, NULL);
			}*/
		/*	if (CheckHeartHandl)
			{
				CheckHeartFlag = TRUE;
				ResumeThread(CheckHeartHandl);
			}	*/	
		}	
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

/*******************CRC��У��λ**********************/
unsigned int Cxads_PCClientDlg::CRC16_X25(unsigned char *puchMsg, unsigned int usDataLen)
{
	int i;
	int crc = 0xffff;
	while (usDataLen--)
	{
		crc ^= *puchMsg++;      
		for (i = 0; i < 8; ++i)
		{
			if (crc & 1)
				crc = (crc >> 1) ^ 0x8408;
			else
				crc = (crc >> 1);
		}
	}
	return ~crc;  
}
unsigned char Cxads_PCClientDlg::DoubleStrToUs(CString strText)
{
	int t_iLength = strText.GetLength();
	if (t_iLength != 2)
	{
		//AfxMessageBox(_T("���ú���DoubleStrToUsʱ����������"));
		return 0;
	}
	unsigned char t_ucHigh = SingleStrToUs(strText.Mid(0, 1));
	unsigned char t_ucLow = SingleStrToUs(strText.Mid(1, 1));
	return t_ucHigh * 16 + t_ucLow;
}
unsigned char Cxads_PCClientDlg::SingleStrToUs(CString strText)
{
	int t_iLength = strText.GetLength();
	if (t_iLength != 1)
	{
		//AfxMessageBox(_T("���ú���SingleStrToUsʱ����������"));
		return 0;
	}
	if (strText == _T("0"))
		return 0;
	if (strText == _T("1"))
		return 1;
	if (strText == _T("2"))
		return 2;
	if (strText == _T("3"))
		return 3;
	if (strText == _T("4"))
		return 4;
	if (strText == _T("5"))
		return 5;
	if (strText == _T("6"))
		return 6;
	if (strText == _T("7"))
		return 7;
	if (strText == _T("8"))
		return 8;
	if (strText == _T("9"))
		return 9;
	if (strText == _T("a") || strText == _T("A"))
		return 10;
	if (strText == _T("b") || strText == _T("B"))
		return 11;
	if (strText == _T("c") || strText == _T("C"))
		return 12;
	if (strText == _T("d") || strText == _T("D"))
		return 13;
	if (strText == _T("e") || strText == _T("E"))
		return 14;
	if (strText == _T("f") || strText == _T("F"))
		return 15;
	return 0;
}
CString Cxads_PCClientDlg::ReturnCheckBit(int length,CString destStr)
{
	CString str;
	/*int t_iLength = GetOnBag.GetLength();*/
	int t_iBytes = length / 2;
	unsigned char * t_pucBuf = new unsigned char[t_iBytes];
	memset(t_pucBuf, 0, sizeof(unsigned char)*t_iBytes);
	for (int t_iInx = 0; t_iInx < length; t_iInx += 2)
	{
		CString t_strDouble = destStr.Mid(t_iInx, 2);
		t_pucBuf[t_iInx / 2] = DoubleStrToUs(t_strDouble);
	}
	unsigned int t_uiCrcRet = CRC16_X25(t_pucBuf, t_iBytes);
	str.Format(_T("%04X"), t_uiCrcRet);
	str = str.Right(4);
	delete[] t_pucBuf;
	return str;
}

/******************����excel****************/
bool Cxads_PCClientDlg::OpenExcel(int SheetNum)
{
	HRESULT hr;
	hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		AfxMessageBox(_T("Failed to call Coinitialize()"));
	}
	TCHAR szFilter[] = _T("�ı��ļ� | *.xls; *.xlsx");
	CFileDialog fileDlg(TRUE, _T("txt"), NULL, 0, szFilter, this);
	CString strFilePath;
	if (IDOK == fileDlg.DoModal())
	{
		strFilePath = fileDlg.GetPathName();
		//SetDlgItemText(IDC_EDIT1, strFilePath);
	}
	else
	{
		return FALSE;
	}
	CApplication app;
	CWorkbook book;
	CWorkbooks books;
	CWorksheet sheet;
	CWorksheets sheets;
	CRange range;
	LPDISPATCH lpDisp;
	//�������
	COleVariant covOptional((long)
		DISP_E_PARAMNOTFOUND, VT_ERROR);
	if (!app.CreateDispatch(_T("Excel.Application"), NULL))
	{
		this->MessageBox(_T("�޷�����ExcelӦ��"));
		return FALSE;
	}
	books = app.get_Workbooks();
	//��Excel������pathnameΪExcel���·����
	lpDisp = books.Open(strFilePath, covOptional
		, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional
		, covOptional, covOptional, covOptional
		, covOptional, covOptional, covOptional
		, covOptional);
	book.AttachDispatch(lpDisp);
	sheets = book.get_Worksheets();
	sheet = sheets.get_Item(COleVariant((short)SheetNum));
	/***********************��ȡExcel��Ԫ��*****************************/
	//app.put_Visible(TRUE); //���ɼ�
	//app.put_UserControl(FALSE); 
	////�������Ϊ��A��1���ĵ�Ԫ��
	//range = sheet.get_Range(COleVariant(_T("A1"))
	//,COleVariant(_T("A1")));
	////��õ�Ԫ�������
	//COleVariant rValue;
	//rValue = COleVariant(range.get_Value2());
	////ת���ɿ��ַ�
	//rValue.ChangeType(VT_BSTR);
	////ת����ʽ�������
	//SetDlgItemText(IDC_EDIT1,(CString(rValue.bstrVal)));
	/********************************************************************/
	COleVariant vResult;
	//��ȡ�Ѿ�ʹ���������Ϣ�������Ѿ�ʹ�õ���������������ʼ�С���ʼ��
	range.AttachDispatch(sheet.get_UsedRange());
	range.AttachDispatch(range.get_Rows());
	//ȡ���Ѿ�ʹ�õ�����
	long iRowNum = range.get_Count();
	range.AttachDispatch(range.get_Columns());
	//ȡ���Ѿ�ʹ�õ�����
	long iColNum = range.get_Count();
	//ȡ����ʹ���������ʼ�У���1��ʼ
	long iStartRow = range.get_Row();
	//ȡ����ʹ���������ʼ�У���1��ʼ
	long iStartCol = range.get_Column();
	for (int i = iStartRow + 1; i <= iRowNum; i++)
	{
		if (SheetNum==1)
		{
			CString strRowName = _T("");
			strRowName.Format(_T("%d"), i);
			m_correct.InsertItem(i - 2, strRowName);
		}
		/*CString strRowName = _T("");
		strRowName.Format(_T("%d"), i);
		m_correct.InsertItem(i - 2, strRowName);*/
		for (int j = iStartCol; j <= iColNum; j++)
		{
			//��ȡ��Ԫ���ֵ
			range.AttachDispatch(sheet.get_Cells());
			range.AttachDispatch(range.get_Item(COleVariant((long)i),
				COleVariant((long)j)).pdispVal);
			vResult = range.get_Value2();
			CString str, stry, strm, strd;
			SYSTEMTIME st;
			if (vResult.vt == VT_BSTR)     //�����ַ���
			{
				str = vResult.bstrVal;
			}
			else if (vResult.vt == VT_R8) //8�ֽڵ�����
			{
				str.Format(_T("%f"), vResult.dblVal);
			}
			else if (vResult.vt == VT_DATE) //ʱ���ʽ
			{
				VariantTimeToSystemTime(vResult.date, &st);
				stry.Format(_T("%d"), st.wYear);
				strm.Format(_T("%d"), st.wMonth);
				strd.Format(_T("%d"), st.wDay);
				str = stry + L"-" + strm + L"-" + strd;
			}
			else if (vResult.vt == VT_EMPTY) //��ԪΪ��
			{
				str = L"";
			}
			else if (vResult.vt == VT_I4)
			{
				str.Format(_T("%ld"), (int)vResult.lVal);
			}
			if (SheetNum==1)
			{
				m_correct.SetItemText(i - 2, j - 1, str);
			}
			if (SheetNum==2)
			{
				m_error.SetSel(-1, -1);
				m_error.ReplaceSel(str + _T("\r\n"));
			}
			
		}
	}
	/*�ͷ���Դ*/
	range.ReleaseDispatch();
	sheet.ReleaseDispatch();
	sheets.ReleaseDispatch();
	book.ReleaseDispatch();
	books.ReleaseDispatch();
	app.Quit();
	app.ReleaseDispatch();
	return TRUE;
}

/*******��ȡ��ǰʱ�����ת����16�����ַ���********/
CString Cxads_PCClientDlg::GetTimeStamp()
{
	SYSTEMTIME tmSys;
	GetLocalTime(&tmSys);
	CTime tm3(tmSys);
	__int64 tmDst = __int64(tm3.GetTime()) * 1000 + tmSys.wMilliseconds;
	CString HexTimeStampStr;
	char temp[20];
	char DestTemp[20];
	_i64toa_s(tmDst, temp, sizeof(temp), 10);
	HexTimeStampStr.Format(_T("%s"), temp);
	HexTimeStampStr = HexTimeStampStr.Left(HexTimeStampStr.GetLength() - 3);
	long long i = atoi(HexTimeStampStr);
	_itoa_s(i, DestTemp, 16);
	HexTimeStampStr.Format(_T("%s"), DestTemp);
	return HexTimeStampStr;
}

void Cxads_PCClientDlg::OnOK()
{
	// TODO:  �ڴ����ר�ô����/����û���
	
	//CDialogEx::OnOK();
}

/************�ڹ���Ŀ¼����txt**************/
CString GetExePath()
{
	char sFileName[256] = { 0 };
	CString ProjectPath = _T("");
	GetModuleFileName(AfxGetInstanceHandle(), sFileName, 255);
	ProjectPath.Format("%s", sFileName);
	int pos = ProjectPath.ReverseFind('\\');
	if (pos != -1)
		ProjectPath = ProjectPath.Left(pos);
	else
		ProjectPath = _T("");
	return ProjectPath;
}
BOOL Cxads_PCClientDlg::WritetoTxt(CString sValue)
{
	CString SubPath;
	SubPath = m_MonthStr+"-"+m_DayStr+"-"+"IMEI-" + IMEI;
	CString sFile = GetExePath() + "\\" + SubPath + ".txt";
	CStdioFile file;
	if (file.Open(sFile, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate))
	{
		file.SeekToEnd(); // �ƶ��ļ�ָ�뵽ĩβ
		file.WriteString(sValue);
		file.Close();
	}
	return FALSE;
}
