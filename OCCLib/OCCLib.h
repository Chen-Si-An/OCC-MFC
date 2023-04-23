// OCCLib.h: OCCLib DLL 的主要標頭檔
//

#pragma once

#ifndef __AFXWIN_H__
	#error "對 PCH 在包含此檔案前先包含 'pch.h'"
#endif

#include "resource.h"		// 主要符號


// COCCLibApp
// 此類別的實作請參閱 OCCLib.cpp
//

class COCCLibApp : public CWinApp
{
public:
	COCCLibApp();

// 覆寫
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
