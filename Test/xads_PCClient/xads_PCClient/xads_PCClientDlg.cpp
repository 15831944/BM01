
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
	ON_BN_CLICKED(IDC_BUTTON1, &Cxads_PCClientDlg::OnBnClickedButton1)
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
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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
	CreateThread(0,0,ConnectNetworkThread,this,0,NULL);
	
	EnableWindow(IDC_BUTTONSEND,TRUE);
}
/*******************�̺߳���****************************************/
DWORD WINAPI ConnectNetworkThread(  LPVOID lpParameter)
{
	Cxads_PCClientDlg * pClient = (Cxads_PCClientDlg *)lpParameter;
	if(pClient->ConnectSocket(pClient))
	{
		
	}
	return 0;
}

/***************************���ӷ���������************************************/
#define MAX_BUFF 256
int iRead;
BOOL Cxads_PCClientDlg::ConnectSocket(Cxads_PCClientDlg * pClient)
{
	m_ClientSock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);//����socket
	if (NULL == m_ClientSock)
	{
		MessageBox(_T("����socketʧ��"));
		return FALSE;
	}
	//int flag = 0;
	sockaddr_in sa;
	sa.sin_family = AF_INET;
	CString strIp;
	DWORD dPort = GetDlgItemInt(IDC_EDITPORT);
	GetDlgItemText(IDC_IPADDRESS,strIp);
	if (strIp == _T("0.0.0.0") || (dPort >= 65535 && dPort < 1024) || dPort == 0)
	{
		MessageBox(_T("��������ȷIP��ַ��˿�"));
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
	SendGetOnData(_T("79790022700000000100000100080351609080302733001c00027010001d0002320000133a790d0a"));
	isServerOn = TRUE;
	OnEnChangeEditsendbox();
	//CreateThread(0, 0, (LPTHREAD_START_ROUTINE)SendHeartThread, this, 0, NULL);

	CString strMsg;
	while (TRUE)         //����
	{
		if (socket_Select(m_ClientSock,100,TRUE))
		{
			char szMsg[MAX_BUFF] = {0 };
			char p_StrBcd[MAX_BUFF] = { 0};
			iRead = recv(m_ClientSock, szMsg, MAX_BUFF, 0);   //��ȡ���������ݰ�
			if (iRead > 0)
			{
				Sdk_Str2BcdStr(szMsg,16, p_StrBcd);
				strMsg = p_StrBcd;
				pClient->SetRevBoxText(_T("������>>") + strMsg);
				Sleep(2000);
				SendGetOnData(_T("797900157000000001010016000121001900020046002c54320d0a"));				
				SYSTEMTIME time;
				CString strTime;
				GetLocalTime(&time);
				strTime.Format(_T("(%d/%2d/%2d %02d:%02d:%02d)\n"), time.wYear, time.wMonth, time.wDay,
				time.wHour, time.wMinute,time.wSecond);
			} 
			else
			{
				pClient->SetRevBoxText(_T("�Ѷ��ߣ�����������"));
				isServerOn = FALSE;
				return TRUE;
			}
		}
	}
	return TRUE;
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

/******************�ж��Ƿ����ӳ�ʱ************************/
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
	m_RecData.SetSel(-1, -1);
	m_RecData.ReplaceSel(GetTime() + _T("\r\n  ") + strMsg + _T("\r\n"));
	m_RecData.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
}

/******��ȡ��ǰʱ��*********/
CString GetTime()
{
	CTime time = CTime::GetCurrentTime();
	CString Tiemstr;
	Tiemstr = time.Format(_T("%Y-%m-%d  %H:%M:%S"));
	return Tiemstr;
}

/***ֹͣ��ť���ر�socket���ӣ����÷��ͺ�ֹͣ��ť***/
void Cxads_PCClientDlg::OnBnClickedButtonstop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	EnableWindow(IDC_BUTTONCONNECT,TRUE);
	EnableWindow(IDC_BUTTONSEND,FALSE);
	EnableWindow(IDC_BUTTONSTOP,FALSE);
	SetDlgItemText(IDC_EDITSENDBOX,_T(" "));
	closesocket(m_ClientSock);
	isServerOn = FALSE;
}

/**************�˳���ť���رմ��ں�socket����***/
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
	SetRevBoxText(_T("�ն� >>") + strGetMsg);
	return; 
}

/****************************��¼��************************/
void Cxads_PCClientDlg::SendGetOnData(CString GetOnData)
{
	CString strGetMsg = GetOnData;
	int strLength, DestLength;
	int iWrite;
	//GetDlgItemText(IDC_EDITSENDBOX, strGetMsg);
	strLength = strGetMsg.GetLength();
	strGetMsg = strRemove(strGetMsg, strLength);
	DestLength = strGetMsg.GetLength();
	char buffer[1024] = { 0 };
	HexStr2Byte(strGetMsg, buffer, strLength);
    iWrite = send(m_ClientSock, buffer, DestLength / 2, 0);
	if (SOCKET_ERROR == iWrite)
		{
			SetRevBoxText(_T("���ʹ���"));
		}
		SetRevBoxText(_T("�ն� >>") + strGetMsg);
	return;
}

/********************�����������̺߳���**********************/
void SendHeartThread(LPVOID lpParameter)
{
	Cxads_PCClientDlg * pClient = (Cxads_PCClientDlg *)lpParameter;
	CString destMessage = _T("797900157000000001010016000121001900020046002c54320d0a");
	if (pClient->SendHeartData(pClient, destMessage))
	{

	}
	return;
}
bool Cxads_PCClientDlg::SendHeartData(Cxads_PCClientDlg * pClient, CString HeartData)
{
	CString strGetMsg = HeartData;
	int strLength, DestLength;
	int iWrite;
	//GetDlgItemText(IDC_EDITSENDBOX, strGetMsg);
	strLength = strGetMsg.GetLength();
	strGetMsg = strRemove(strGetMsg, strLength);
	DestLength = strGetMsg.GetLength();
	char buffer[1024] = { 0 };
	HexStr2Byte(strGetMsg, buffer, strLength);
	while (TRUE)
	{
		iWrite = send(m_ClientSock, buffer, DestLength / 2, 0);
		SetRevBoxText(_T("�ն� >>") + strGetMsg);
		Sleep(1000);
	}
	if (SOCKET_ERROR == iWrite)
	{
		SetRevBoxText(_T("���ʹ���"));
	}
	SetRevBoxText(_T("�ն� >>") + strGetMsg);
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
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void Cxads_PCClientDlg::OnBnClickedButton4()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	HRESULT hr;
	hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		AfxMessageBox(_T("Failed to call Coinitialize()"));
	}
	TCHAR szFilter[] = _T("�ı��ļ� | *.xls; *.xlsx");
	CFileDialog fileDlg(TRUE, NULL, NULL, 0, szFilter, this);
	CString strFilePath;
	if (IDOK == fileDlg.DoModal())
	{
		strFilePath = fileDlg.GetPathName();
		//SetDlgItemText(IDC_EDIT1, strFilePath);
	}
	else
	{
		return;
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
		return;
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
	sheet = sheets.get_Item(COleVariant((short)1));
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


	for (int i = iStartRow; i <= iRowNum; i++)
	{
		CString strRowName = _T("");
		strRowName.Format(_T("%d"), i);
		m_correct.InsertItem(i - 2, strRowName);
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
			m_correct.SetItemText(i - 2, j - 1, str);
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
}


void Cxads_PCClientDlg::OnBnClickedButton5()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	HRESULT hr;
	hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		AfxMessageBox(_T("Failed to call Coinitialize()"));
	}
	TCHAR szFilter[] = _T("�ı��ļ� | *.xls; *.xlsx");
	CFileDialog fileDlg(TRUE, NULL, NULL, 0, szFilter, this);
	CString strFilePath;
	if (IDOK == fileDlg.DoModal())
	{
		strFilePath = fileDlg.GetPathName();
		//SetDlgItemText(IDC_EDIT1, strFilePath);
	}
	else
	{
		return;
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
		return;
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
	sheet = sheets.get_Item(COleVariant((short)2));
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


	for (int i = iStartRow+1; i <= iRowNum; i++)
	{
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
			m_error.SetSel(-1, -1);
			m_error.ReplaceSel(str + _T("\r\n"));
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
}


void Cxads_PCClientDlg::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
