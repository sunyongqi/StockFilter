
// StockFilterDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StockFilter.h"
#include "StockFilterDlg.h"
#include "afxdialogex.h"
#include "StockPrice.h"
#include <map>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <locale>
#include <codecvt>

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif

#define NUM_DAYS_MAX		8000
//struct daySummary history[NUM_DAYS_MAX];
std::map<std::wstring, std::wstring> g_mapCodeName;
std::map<std::wstring, std::wstring> g_mapNameCode;

// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CStockFilterDlg ダイアログ



CStockFilterDlg::CStockFilterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_STOCKFILTER_DIALOG, pParent)
	, m_strStockInput(_T(""))
	, m_strDate0(_T(""))
	, m_strDate1(_T(""))
	, m_strDate2(_T(""))
	, m_strDate3(_T(""))
	, m_strDate4(_T(""))
	, m_strDate5(_T(""))
	, m_strDate6(_T(""))
	, m_strDate7(_T(""))
	, m_strDate8(_T(""))
	, m_strStockName(_T(""))
	, m_strStockCode(_T(""))
	, m_nGraphType(0)
	, m_nIndicator(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CStockFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRAPH, m_StockGraph);
	DDX_Text(pDX, IDC_EDIT1, m_strStockInput);
	DDV_MaxChars(pDX, m_strStockInput, 6);
	DDX_Text(pDX, IDC_STATIC_DATE0, m_strDate0);
	DDX_Text(pDX, IDC_STATIC_DATE1, m_strDate1);
	DDX_Text(pDX, IDC_STATIC_DATE2, m_strDate2);
	DDX_Text(pDX, IDC_STATIC_DATE3, m_strDate3);
	DDX_Text(pDX, IDC_STATIC_DATE4, m_strDate4);
	DDX_Text(pDX, IDC_STATIC_DATE5, m_strDate5);
	DDX_Text(pDX, IDC_STATIC_DATE6, m_strDate6);
	DDX_Text(pDX, IDC_STATIC_DATE7, m_strDate7);
	DDX_Text(pDX, IDC_STATIC_DATE8, m_strDate8);
	DDX_Text(pDX, IDC_STOCKNAME, m_strStockName);
	DDX_Text(pDX, IDC_STOCKCODE, m_strStockCode);
	DDX_Radio(pDX, IDC_RADIO1, m_nGraphType);
	DDX_Radio(pDX, IDC_RADIO4, m_nIndicator);
	DDX_Control(pDX, IDC_STOCKLIST, m_cStockList);
}

BEGIN_MESSAGE_MAP(CStockFilterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_GETPRICE, &CStockFilterDlg::OnBnClickedGetprice)
	ON_EN_CHANGE(IDC_EDIT1, &CStockFilterDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_FINDPATTERN, &CStockFilterDlg::OnBnClickedFindpattern)
	ON_BN_CLICKED(IDC_DOWNLOADALL, &CStockFilterDlg::OnBnClickedDownloadall)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO7, &CStockFilterDlg::OnBnClickedRadioGraph)
	ON_LBN_SELCHANGE(IDC_STOCKLIST, &CStockFilterDlg::OnLbnSelchangeStocklist)
END_MESSAGE_MAP()


// CStockFilterDlg メッセージ ハンドラー

BOOL CStockFilterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "バージョン情報..." メニューをシステム メニューに追加します。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
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

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// T1ODO: 初期化をここに追加します。
	if (!CStockPrice::LoadMarketIndices())
		return TRUE;

	m_pStockPrice = NULL;
	m_iFound = -1;
	m_strStockInput = _T("600881");
	UpdateData(FALSE);

	g_mapCodeName.clear();
	g_mapNameCode.clear();

	std::string strFileName = PRICEFILE_PATH;
	strFileName += "stocklist.csv";
	std::wifstream filestream(strFileName);
	if (!filestream.is_open())
	{
		// ファイルが開けなかった場合は終了する
		return TRUE;
	}

	const std::locale empty_locale = std::locale::empty();
	typedef std::codecvt_utf8<wchar_t> converter_type;
	const converter_type* converter = new converter_type;
	const std::locale utf8_locale = std::locale(empty_locale, converter);
	filestream.imbue(utf8_locale);
	std::wstring strCode, strName;

	std::wstring str;
	while (std::getline(filestream, str)) {
		std::wistringstream stream(str);

		//1行のうち、文字列とコンマを分割する
		while (std::getline(stream, strCode, L','))
		{
			if (std::getline(stream, strName, L','))
			{
				strCode = std::wstring(strCode, 1, strCode.length() - 2);
				strName = std::wstring(strName, 1, strName.length() - 2);
				if (strCode.front() == L'\"')
					strCode.erase(0, 1);	// なぜか1行目だけ2文字削除しないといけない

				//if (strCode.front() == '6')
				//	strCode += L".ss";
				//else
				//	strCode += L".sz";

				g_mapCodeName.insert(std::make_pair(strCode, strName));
				g_mapNameCode.insert(std::make_pair(strName, strCode));
				//int temp = stof(token); //stof(string str) : stringをfloatに変換
			}
			else
				break;
		}
	}

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void CStockFilterDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CStockFilterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CStockFilterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CStockFilterDlg::OnBnClickedGetprice()
{
	UpdateData(TRUE);

	if (!m_pStockPrice)
		delete m_pStockPrice;

	m_pStockPrice = GetStockPrice(m_strStockInput);
	DrawStockGraph(m_pStockPrice);
}

CStockPrice* CStockFilterDlg::GetStockPrice(CString strInput)
{
	m_iFound = -1;
	std::string strCode;
	std::wstring wsInput = strInput;
	std::wstring wsCode = L"";
	CString strFirst = m_strStockInput.Left(1);
	if (strFirst >= L"0" && strFirst <= L"9")
	{
		auto it = g_mapCodeName.find(wsInput);
		if (it != g_mapCodeName.end())
		{
			wsCode = it->first;
			m_strStockCode = wsCode.c_str();
			m_strStockName = it->second.c_str();
		}
		else
		{
			wsCode = wsInput;
		}
	}
	else
	{
		auto it = g_mapNameCode.find(wsInput);
		if (it != g_mapNameCode.end())
		{
			wsCode = it->second;
			m_strStockCode = wsCode.c_str();
			m_strStockName = it->first.c_str();
		}
		else
		{
			return NULL;
		}
	}

	CStringA strA(wsCode.c_str());
	strCode = (LPCSTR)strA;

	UpdateData(FALSE);

	CStockPrice* pStockPrice = new CStockPrice(strCode);
	//pStockPrice->DownloadSingleStockPrices(strCode);
	return pStockPrice;
}

BOOL CStockFilterDlg::DrawStockGraph(CStockPrice* pStockPrice)
{
	//CStockPrice* pStockPrice = new CStockPrice(strCode);
	int nDays = pStockPrice->LoadStockData();
	
	UpdateScrollBar(nDays, nDays, NUM_LINE_DISPLAY);
	UpdateDateText(0, NUM_LINE_DISPLAY);
	

	m_StockGraph.SetDateRange(0, NUM_LINE_DISPLAY);
	m_StockGraph.SetStockPrice(m_pStockPrice);
	m_StockGraph.RedrawWindow();
	return TRUE;
}

void CStockFilterDlg::UpdateDateText(int iLast, int nNum)
{
	m_strDate0.Format(_T("%d"), m_pStockPrice->DATE[iLast]);
	m_strDate1.Format(_T("%d"), m_pStockPrice->DATE[iLast + nNum * 1 / 8]);
	m_strDate2.Format(_T("%d"), m_pStockPrice->DATE[iLast + nNum * 2 / 8]);
	m_strDate3.Format(_T("%d"), m_pStockPrice->DATE[iLast + nNum * 3 / 8]);
	m_strDate4.Format(_T("%d"), m_pStockPrice->DATE[iLast + nNum * 4 / 8]);
	m_strDate5.Format(_T("%d"), m_pStockPrice->DATE[iLast + nNum * 5 / 8]);
	m_strDate6.Format(_T("%d"), m_pStockPrice->DATE[iLast + nNum * 6 / 8]);
	m_strDate7.Format(_T("%d"), m_pStockPrice->DATE[iLast + nNum * 7 / 8]);
	m_strDate8.Format(_T("%d"), m_pStockPrice->DATE[iLast + nNum]);
	UpdateData(FALSE);
}

void CStockFilterDlg::OnEnChangeEdit1()
{
	// TODO: これが RICHEDIT コントロールの場合、このコントロールが
	// この通知を送信するには、CDialogEx::OnInitDialog() 関数をオーバーライドし、
	// CRichEditCtrl().SetEventMask() を
	// OR 状態の ENM_CHANGE フラグをマスクに入れて呼び出す必要があります。

	// TODO: ここにコントロール通知ハンドラー コードを追加してください。
}

void CStockFilterDlg::UpdateScrollBar(int nPos, int nMax, int nPage)
{
	SCROLLINFO	info;
	info.cbSize = sizeof(SCROLLINFO);
	info.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
	info.nMin = 0;
	info.nPos = nPos;

	// 水平バー
	info.nPage = nPage;
	info.nMax = nMax;
	if (!SetScrollInfo(SB_HORZ, &info, TRUE))
		SetScrollRange(SB_HORZ, 0, nPage, TRUE);
}

void CStockFilterDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	SCROLLINFO	info;
	info.cbSize = sizeof(SCROLLINFO);
	info.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
	GetScrollInfo(SB_HORZ, &info);

	int nStep = max(32, info.nPage / 5);
	int nMax = info.nMax;
	int nNew = info.nPos;
	switch (nSBCode) {
	case SB_LINERIGHT:
		nNew += nStep;		break;
	case SB_PAGERIGHT:
		nNew += info.nPage;	break;
	case SB_LINELEFT:
		nNew -= nStep;		break;
	case SB_PAGELEFT:
		nNew -= info.nPage;	break;
	case SB_RIGHT:
		nNew = nMax;		break;
	case SB_LEFT:
		nNew = info.nMin;		break;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		nNew = (int)nPos;	break;
	default:
		return;
	}
	nNew = min(nMax - info.nPage, max(info.nMin, nNew));

	int nPosOrg = info.nPos;
	if (nNew != nPosOrg) {
		SetScrollPos(SB_HORZ, nNew, TRUE);	// ScrollBar Position
		int iLastData = info.nMax - nNew - info.nPage;
		m_StockGraph.SetDateRange(iLastData, NUM_LINE_DISPLAY);	// 一番右の位置を0とする
		m_StockGraph.RedrawWindow();
		UpdateDateText(iLastData, NUM_LINE_DISPLAY);
	}
}

void CStockFilterDlg::OnBnClickedFindpattern()
{
	std::map<std::wstring, std::wstring>::iterator it = g_mapCodeName.begin();
	int i = 0;
	while (it != g_mapCodeName.end())
	{
		std::wstring wstr = it->first;
		m_cStockList.InsertString(-1, wstr.c_str());
		it++;
		if (i++ > 10)
			break;
	}

	//if (!m_pStockPrice)
	//	return;


	//m_iFound = CStockPrice::GoldenCross(m_pStockPrice->DIF, m_pStockPrice->DEA, m_iFound + 1, 1000);
	//if (m_iFound < 0)
	//{
	//	AfxMessageBox(_T("Not Found!"));
	//	return;
	//}

	////m_StockGraph.SetDateRange(m_iFound, NUM_LINE_DISPLAY);	// 一番右の位置を0とする
	//m_StockGraph.SetDateRange(max(m_iFound - (int)(NUM_LINE_DISPLAY / 2.0), 0), NUM_LINE_DISPLAY);	// 見つかったパターンを真ん中に置く
	//m_StockGraph.SetMark(m_iFound, 2);
	//m_StockGraph.RedrawWindow();
	//UpdateDateText(m_iFound, NUM_LINE_DISPLAY);

	//SUMMARY data[4];
	//data[0].open = 90;
	//data[0].high = 96;
	//data[0].low = 90;
	//data[0].close = 96;
	//
	//data[1].open = 89;
	//data[1].high = 91;
	//data[1].low = 86;
	//data[1].close = 89;

	//data[2].open = 100;
	//data[2].high = 100;
	//data[2].low = 92;
	//data[2].close = 92;

	//data[3].close = 100;

	////data[0].open = 9.2;
	////data[0].low = 9.1;
	////data[0].close = data[0].high = 9.7;
	////data[1].open = 8.9;
	////data[1].high = 9.2;
	////data[1].low = 8.5;
	////data[1].close = 9.0;
	////data[2].open = data[2].high = 9.9;
	////data[2].low = data[2].close = 9.3;
	////data[3].close = 10.0;

	//int nCount = m_StockGraph.FindCount(data, 3);
	////int nCount = m_StockGraph.FindCount(history + 15, 3);
	//if (nCount <= 0)
	//{
	//	AfxMessageBox(_T("Cannot find any similar pattern!"));
	//	return;
	//}

	//int nIndex = m_StockGraph.FindNext(data, 3);
	////int nIndex = m_StockGraph.FindNext(history + 15, 3);
	//if (nIndex != -1)
	//{
	//	m_StockGraph.SetDateRange(nIndex, NUM_LINE_DISPLAY);	// 一番右の位置を0とする
	//	//m_StockGraph.SetDateRange(max(nIndex - (int)(NUM_LINE_DISPLAY / 2.0), 0), NUM_LINE_DISPLAY);	// 見つかったパターンを真ん中に置く
	//	m_StockGraph.SetMark(nIndex, 3);
	//	m_StockGraph.RedrawWindow();
	//	UpdateDateText(nIndex, NUM_LINE_DISPLAY);
	//}
	//else
	//	AfxMessageBox(_T("Cannot find more similar pattern!"));
}


void CStockFilterDlg::OnBnClickedDownloadall()
{
	CStockPrice stockPrice;
	stockPrice.DownloadAllStocksPrices();
}


void CStockFilterDlg::OnBnClickedRadioGraph(UINT nID)
{
	UpdateData(TRUE);
	m_StockGraph.SetDrawMode(m_nGraphType, m_nIndicator);
	m_StockGraph.RedrawWindow();
}


void CStockFilterDlg::OnLbnSelchangeStocklist()
{
	if (!m_pStockPrice)
		delete m_pStockPrice;

	CString strInput;
	m_cStockList.GetText(m_cStockList.GetCurSel(), strInput);
	m_pStockPrice = GetStockPrice(strInput);
	DrawStockGraph(m_pStockPrice);
}
