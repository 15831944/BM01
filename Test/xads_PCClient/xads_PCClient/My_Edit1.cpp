#include "stdafx.h"
#include "My_Edit1.h"



BEGIN_MESSAGE_MAP(My_Edit1, CEdit)
	ON_WM_CHAR()
END_MESSAGE_MAP()

My_Edit1::My_Edit1()
{
}


My_Edit1::~My_Edit1()
{
}

void My_Edit1::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if ((nChar >= '0' && nChar <= '9') || nChar == 0x08 )
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
}
