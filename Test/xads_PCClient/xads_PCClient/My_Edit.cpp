#include "stdafx.h"
#include "My_Edit.h"



BEGIN_MESSAGE_MAP(My_Edit, CEdit)
	ON_WM_CHAR()
END_MESSAGE_MAP()

My_Edit::My_Edit()
{
}


My_Edit::~My_Edit()
{
}


void My_Edit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == '.')
	{
		CString str;
		GetWindowText(str);
		if (str.GetLength() == 0)
		{
			MessageBox(_T("��һλ��������С����"));
			return;
		}
		if (str.Find('.') == -1)
		{
			CEdit::OnChar(nChar, nRepCnt, nFlags);
		}
		else
		{
			MessageBox(_T("С����ֻ������һ��"));
		}
	}
	else if ((nChar >= '0' && nChar <= '9') || nChar == 0x08)
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
}
