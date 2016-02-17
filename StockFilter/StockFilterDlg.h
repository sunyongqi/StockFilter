
// StockFilterDlg.h : ヘッダー ファイル
//

#pragma once

#include "StockGraph.h"
#include <string>
#include "afxwin.h"

// CStockFilterDlg ダイアログ
class CStockFilterDlg : public CDialogEx
{
// コンストラクション
public:
	CStockFilterDlg(CWnd* pParent = NULL);	// 標準コンストラクター
	virtual ~CStockFilterDlg();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STOCKFILTER_DIALOG };
#endif
	CStockGraph	m_StockGraph;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedGetprice();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	BOOL InitStockList();
	void UpdateScrollBar(int nPos, int nMax, int nPage);
	bool GetStockPrice(CString strInput);
	BOOL DrawStockGraph(CStockPrice* pStockPrice);
	void UpdateDateText(int iLast, int nNum = NUM_LINE_DISPLAY);
	CString m_strSearch;
	CString m_strDate0;
	CString m_strDate1;
	CString m_strDate2;
	CString m_strDate3;
	CString m_strDate4;
	CString m_strDate5;
	CString m_strDate6;
	CString m_strDate7;
	CString m_strDate8;
	afx_msg void OnBnClickedFindpattern();
	CString m_strStockInput;
	CString m_strStockName;
	CString m_strStockCode;
	afx_msg void OnBnClickedDownloadall();

private:
	CStockPrice* m_pStockPrice;

	int m_iFound;
public:
	int m_nGraphPeriod;
	int m_nIndicator;
	afx_msg void OnBnClickedRadioGraph(UINT nID);
	CListBox m_cStockList;
	afx_msg void OnLbnSelchangeStocklist();
};
