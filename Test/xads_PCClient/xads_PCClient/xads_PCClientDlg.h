
// xads_PCClientDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// Cxads_PCClientDlg �Ի���
class Cxads_PCClientDlg : public CDialogEx
{
// ����
public:
	Cxads_PCClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_XADS_PCCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonconnect();
	BOOL ConnectSocket(Cxads_PCClientDlg *);
	BOOL EnableWindow(DWORD DlgId, BOOL bUsed);
	void SetRevBoxText(CString strMsg);
	CString strRemove(CString str, int length);
	bool SendHeartData(Cxads_PCClientDlg * pClient, CString HeartData);
	void SendGetOnData(CString GetOnData);

	SOCKET m_ClientSock;
	afx_msg void OnBnClickedButtonstop();
	afx_msg void OnBnClickedButtonquit();
	afx_msg void OnBnClickedButtonsend();
	afx_msg void OnEnChangeEditsendbox();

private:
	BOOL isServerOn;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEnChangeSendtime();
	void HexStr2Byte(const char* source, char*dest, int sourceLen);
	CRichEditCtrl m_RecData;
	CListCtrl m_correct;
	afx_msg void OnBnClickedButton4();
	CEdit m_error;
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton1();
	HANDLE HeartDataHandle;
};

DWORD WINAPI ConnectNetworkThread(  LPVOID lpParameter);
BOOL socket_Select(SOCKET hSocket,DWORD nTimeOut,BOOL bRead);
CString GetTime();
void Sdk_Int2Char(int p_nNum, char *p_Ch);
void Sdk_Str2BcdStr(char *p_Str, int p_nLen, char *p_StrBcd);
void SendHeartThread(LPVOID lpParameter);
