#pragma once
#include "afxwin.h"
class My_Edit :
	public CEdit
{
public:
	My_Edit();
	~My_Edit();
protected:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);//ÉùÃ÷ÏûÏ¢
	DECLARE_MESSAGE_MAP()
};

