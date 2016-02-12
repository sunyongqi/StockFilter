
// StockFilter.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル

// CStockFilterApp:
// このクラスの実装については、StockFilter.cpp を参照してください。
//

class CStockFilterApp : public CWinApp
{
public:
	CStockFilterApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CStockFilterApp theApp;