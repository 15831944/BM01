
// xads_PCClientDlg.cpp : 实现文件
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

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// Cxads_PCClientDlg 对话框




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


// Cxads_PCClientDlg 消息处理程序

BOOL Cxads_PCClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SetDlgItemText(IDC_IPADDRESS,_T("120.77.221.153"));
	SetDlgItemText(IDC_EDITPORT,_T("8050"));
	::HideCaret(GetDlgItem(IDC_EDITREVBOX)->GetSafeHwnd());
	CRect rect;
	m_correct.GetClientRect(&rect);
	// 为列表视图控件添加全行选中和栅格风格   
	m_correct.SetExtendedStyle(m_correct.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_correct.InsertColumn(0, _T("经度"), LVCFMT_CENTER, rect.Width() / 6, 0);
	m_correct.InsertColumn(1, _T("维度"), LVCFMT_CENTER, rect.Width() / 6, 0);
	m_correct.InsertColumn(2, _T("剩余人数"), LVCFMT_CENTER, rect.Width() / 6, 0);
	m_correct.InsertColumn(3, _T("上车人数"), LVCFMT_CENTER, rect.Width() / 6, 0);
	m_correct.InsertColumn(4, _T("下车人数"), LVCFMT_CENTER, rect.Width() / 6, 0);
	m_correct.InsertColumn(5, _T("时间间隔"), LVCFMT_CENTER, rect.Width() / 6, 0);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Cxads_PCClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Cxads_PCClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/******************开线程连接服务器************************/
void Cxads_PCClientDlg::OnBnClickedButtonconnect()
{
	// TODO: 在此添加控件通知处理程序代码
	CreateThread(0,0,ConnectNetworkThread,this,0,NULL);
	
	EnableWindow(IDC_BUTTONSEND,TRUE);
}
/*******************线程函数****************************************/
DWORD WINAPI ConnectNetworkThread(  LPVOID lpParameter)
{
	Cxads_PCClientDlg * pClient = (Cxads_PCClientDlg *)lpParameter;
	if(pClient->ConnectSocket(pClient))
	{
		
	}
	return 0;
}

/***************************连接服务器函数************************************/
#define MAX_BUFF 256
int iRead;
BOOL Cxads_PCClientDlg::ConnectSocket(Cxads_PCClientDlg * pClient)
{
	m_ClientSock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);//创建socket
	if (NULL == m_ClientSock)
	{
		MessageBox(_T("创建socket失败"));
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
		MessageBox(_T("请输入正确IP地址或端口"));
		return FALSE;
	}
	sa.sin_port = htons(dPort);
	char szIpAdd[32];
	USES_CONVERSION; //定义后才能使用T2A
	sprintf_s((LPTSTR)szIpAdd,32,"%s",strIp);
	sa.sin_addr.S_un.S_addr = inet_addr(szIpAdd);//地址转换为二进制
	if (SOCKET_ERROR == connect(m_ClientSock,(sockaddr *)&sa,sizeof(sa)))
	{
		MessageBox(_T("连接客户端错误,请检查你填写的IP和端口是否错误"));
		return FALSE;
	}
	pClient->SetRevBoxText(_T("连接服务器成功\r\n"));
	pClient->EnableWindow(IDC_BUTTONSTOP,TRUE);
	pClient->EnableWindow(IDC_BUTTONCONNECT,FALSE);
	SendGetOnData(_T("79790022700000000100000100080351609080302733001c00027010001d0002320000133a790d0a"));
	isServerOn = TRUE;
	OnEnChangeEditsendbox();
	//CreateThread(0, 0, (LPTHREAD_START_ROUTINE)SendHeartThread, this, 0, NULL);

	CString strMsg;
	while (TRUE)         //监听
	{
		if (socket_Select(m_ClientSock,100,TRUE))
		{
			char szMsg[MAX_BUFF] = {0 };
			char p_StrBcd[MAX_BUFF] = { 0};
			iRead = recv(m_ClientSock, szMsg, MAX_BUFF, 0);   //读取缓冲区数据包
			if (iRead > 0)
			{
				Sdk_Str2BcdStr(szMsg,16, p_StrBcd);
				strMsg = p_StrBcd;
				pClient->SetRevBoxText(_T("服务器>>") + strMsg);
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
				pClient->SetRevBoxText(_T("已断线，请重新连接"));
				isServerOn = FALSE;
				return TRUE;
			}
		}
	}
	return TRUE;
}

/************把十六进制数转换为字符串，0x0A->"0A"**********************/
void Sdk_Int2Char(int p_nNum, char *p_Ch)
{
	if (p_nNum <= 9)
	{
		*p_Ch = p_nNum + '0';
	}
	else
	{
		/*0 - 9 是十个数*/
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

/******************判断是否连接超时************************/
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

/******************把数据插入到数据显示区最后一行****************/
void Cxads_PCClientDlg::SetRevBoxText(CString strMsg)
{
	m_RecData.SetSel(-1, -1);
	m_RecData.ReplaceSel(GetTime() + _T("\r\n  ") + strMsg + _T("\r\n"));
	m_RecData.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
}

/******获取当前时间*********/
CString GetTime()
{
	CTime time = CTime::GetCurrentTime();
	CString Tiemstr;
	Tiemstr = time.Format(_T("%Y-%m-%d  %H:%M:%S"));
	return Tiemstr;
}

/***停止按钮，关闭socket连接，禁用发送和停止按钮***/
void Cxads_PCClientDlg::OnBnClickedButtonstop()
{
	// TODO: 在此添加控件通知处理程序代码
	EnableWindow(IDC_BUTTONCONNECT,TRUE);
	EnableWindow(IDC_BUTTONSEND,FALSE);
	EnableWindow(IDC_BUTTONSTOP,FALSE);
	SetDlgItemText(IDC_EDITSENDBOX,_T(" "));
	closesocket(m_ClientSock);
	isServerOn = FALSE;
}

/**************退出按钮，关闭窗口和socket连接***/
void Cxads_PCClientDlg::OnBnClickedButtonquit()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButtonstop();
	SendMessage(WM_CLOSE);
}

/****************发送按钮******************/
void Cxads_PCClientDlg::OnBnClickedButtonsend()
{
	// TODO: 在此添加控件通知处理程序代码
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
		SetRevBoxText(_T("发送错误"));
	}
	SetRevBoxText(_T("终端 >>") + strGetMsg);
	return; 
}

/****************************登录包************************/
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
			SetRevBoxText(_T("发送错误"));
		}
		SetRevBoxText(_T("终端 >>") + strGetMsg);
	return;
}

/********************心跳包发送线程函数**********************/
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
		SetRevBoxText(_T("终端 >>") + strGetMsg);
		Sleep(1000);
	}
	if (SOCKET_ERROR == iWrite)
	{
		SetRevBoxText(_T("发送错误"));
	}
	SetRevBoxText(_T("终端 >>") + strGetMsg);
	return TRUE;
}

/*****************把字符串间的空格去掉*************************/
CString Cxads_PCClientDlg::strRemove(CString str, int length)
{
	CString Dest = "";
	for (int i = 0; i<length; i++)
		if (str[i] != ' ')Dest += str[i];
	return Dest;
}

/**********十六进制数的字符串形式转换成字节流  "7979"->buffer{0x79,0x79}**********/
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

/**************判断数据发送区是否有数据******************/
void Cxads_PCClientDlg::OnEnChangeEditsendbox()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
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

/****************判断是否输入间隔时间******************/
int Edittime;
void Cxads_PCClientDlg::OnEnChangeSendtime()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	// TODO:  在此添加控件通知处理程序代码
}

/******************定时器响应函数*******************/
void Cxads_PCClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
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
	// TODO:  在此添加控件通知处理程序代码
	HRESULT hr;
	hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		AfxMessageBox(_T("Failed to call Coinitialize()"));
	}
	TCHAR szFilter[] = _T("文本文件 | *.xls; *.xlsx");
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
	//定义变量
	COleVariant covOptional((long)
		DISP_E_PARAMNOTFOUND, VT_ERROR);
	if (!app.CreateDispatch(_T("Excel.Application"), NULL))
	{
		this->MessageBox(_T("无法创建Excel应用"));
		return;
	}
	books = app.get_Workbooks();
	//打开Excel，其中pathname为Excel表的路径名
	lpDisp = books.Open(strFilePath, covOptional
		, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional
		, covOptional, covOptional, covOptional
		, covOptional, covOptional, covOptional
		, covOptional);
	book.AttachDispatch(lpDisp);
	sheets = book.get_Worksheets();
	sheet = sheets.get_Item(COleVariant((short)1));
	/***********************读取Excel单元格*****************************/
	//app.put_Visible(TRUE); //表格可见
	//app.put_UserControl(FALSE); 
	////获得坐标为（A，1）的单元格
	//range = sheet.get_Range(COleVariant(_T("A1"))
	//,COleVariant(_T("A1")));
	////获得单元格的内容
	//COleVariant rValue;
	//rValue = COleVariant(range.get_Value2());
	////转换成宽字符
	//rValue.ChangeType(VT_BSTR);
	////转换格式，并输出
	//SetDlgItemText(IDC_EDIT1,(CString(rValue.bstrVal)));
	/********************************************************************/
	COleVariant vResult;
	//读取已经使用区域的信息，包括已经使用的行数、列数、起始行、起始列
	range.AttachDispatch(sheet.get_UsedRange());
	range.AttachDispatch(range.get_Rows());
	//取得已经使用的行数
	long iRowNum = range.get_Count();
	range.AttachDispatch(range.get_Columns());
	//取得已经使用的列数
	long iColNum = range.get_Count();
	//取得已使用区域的起始行，从1开始
	long iStartRow = range.get_Row();
	//取得已使用区域的起始列，从1开始
	long iStartCol = range.get_Column();


	for (int i = iStartRow; i <= iRowNum; i++)
	{
		CString strRowName = _T("");
		strRowName.Format(_T("%d"), i);
		m_correct.InsertItem(i - 2, strRowName);
		for (int j = iStartCol; j <= iColNum; j++)
		{
			//读取单元格的值
			range.AttachDispatch(sheet.get_Cells());
			range.AttachDispatch(range.get_Item(COleVariant((long)i),
				COleVariant((long)j)).pdispVal);
			vResult = range.get_Value2();
			CString str, stry, strm, strd;
			SYSTEMTIME st;
			if (vResult.vt == VT_BSTR)     //若是字符串
			{
				str = vResult.bstrVal;
			}
			else if (vResult.vt == VT_R8) //8字节的数字
			{
				str.Format(_T("%f"), vResult.dblVal);
			}
			else if (vResult.vt == VT_DATE) //时间格式
			{
				VariantTimeToSystemTime(vResult.date, &st);
				stry.Format(_T("%d"), st.wYear);
				strm.Format(_T("%d"), st.wMonth);
				strd.Format(_T("%d"), st.wDay);
				str = stry + L"-" + strm + L"-" + strd;
			}
			else if (vResult.vt == VT_EMPTY) //单元为空
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
	/*释放资源*/
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
	// TODO:  在此添加控件通知处理程序代码
	HRESULT hr;
	hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		AfxMessageBox(_T("Failed to call Coinitialize()"));
	}
	TCHAR szFilter[] = _T("文本文件 | *.xls; *.xlsx");
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
	//定义变量
	COleVariant covOptional((long)
		DISP_E_PARAMNOTFOUND, VT_ERROR);
	if (!app.CreateDispatch(_T("Excel.Application"), NULL))
	{
		this->MessageBox(_T("无法创建Excel应用"));
		return;
	}
	books = app.get_Workbooks();
	//打开Excel，其中pathname为Excel表的路径名
	lpDisp = books.Open(strFilePath, covOptional
		, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional
		, covOptional, covOptional, covOptional
		, covOptional, covOptional, covOptional
		, covOptional);
	book.AttachDispatch(lpDisp);
	sheets = book.get_Worksheets();
	sheet = sheets.get_Item(COleVariant((short)2));
	/***********************读取Excel单元格*****************************/
	//app.put_Visible(TRUE); //表格可见
	//app.put_UserControl(FALSE); 
	////获得坐标为（A，1）的单元格
	//range = sheet.get_Range(COleVariant(_T("A1"))
	//,COleVariant(_T("A1")));
	////获得单元格的内容
	//COleVariant rValue;
	//rValue = COleVariant(range.get_Value2());
	////转换成宽字符
	//rValue.ChangeType(VT_BSTR);
	////转换格式，并输出
	//SetDlgItemText(IDC_EDIT1,(CString(rValue.bstrVal)));
	/********************************************************************/
	COleVariant vResult;
	//读取已经使用区域的信息，包括已经使用的行数、列数、起始行、起始列
	range.AttachDispatch(sheet.get_UsedRange());
	range.AttachDispatch(range.get_Rows());
	//取得已经使用的行数
	long iRowNum = range.get_Count();
	range.AttachDispatch(range.get_Columns());
	//取得已经使用的列数
	long iColNum = range.get_Count();
	//取得已使用区域的起始行，从1开始
	long iStartRow = range.get_Row();
	//取得已使用区域的起始列，从1开始
	long iStartCol = range.get_Column();


	for (int i = iStartRow+1; i <= iRowNum; i++)
	{
		/*CString strRowName = _T("");
		strRowName.Format(_T("%d"), i);
		m_correct.InsertItem(i - 2, strRowName);*/
		for (int j = iStartCol; j <= iColNum; j++)
		{
			//读取单元格的值
			range.AttachDispatch(sheet.get_Cells());
			range.AttachDispatch(range.get_Item(COleVariant((long)i),
				COleVariant((long)j)).pdispVal);
			vResult = range.get_Value2();
			CString str, stry, strm, strd;
			SYSTEMTIME st;
			if (vResult.vt == VT_BSTR)     //若是字符串
			{
				str = vResult.bstrVal;
			}
			else if (vResult.vt == VT_R8) //8字节的数字
			{
				str.Format(_T("%f"), vResult.dblVal);
			}
			else if (vResult.vt == VT_DATE) //时间格式
			{
				VariantTimeToSystemTime(vResult.date, &st);
				stry.Format(_T("%d"), st.wYear);
				strm.Format(_T("%d"), st.wMonth);
				strd.Format(_T("%d"), st.wDay);
				str = stry + L"-" + strm + L"-" + strd;
			}
			else if (vResult.vt == VT_EMPTY) //单元为空
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
	/*释放资源*/
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
	// TODO:  在此添加控件通知处理程序代码
}
