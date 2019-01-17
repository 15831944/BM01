#pragma once
#include "afxwin.h"
class My_Edit1 : 
	public CEdit
{
public:
	My_Edit1();
	~My_Edit1();
protected:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);//ÉùÃ÷ÏûÏ¢
	DECLARE_MESSAGE_MAP()
};

