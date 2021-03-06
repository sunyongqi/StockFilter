#pragma once

#include <map>

#define NUM_LINE_DISPLAY	121
#define MA_LINE_MAX			5

class CStockPrice;

typedef DWORD KPATTERN;

typedef enum
{
	gtKLine = 0,
	gtMarketIndex = 1,
	gtIndicator = 2,
	gtVolume = 3,
} eGraphType;

typedef enum
{
	gpRealtime	= 0,
	gpDay		= 1,
	gpWeek		= 2,
} eGraphPeriod;

typedef enum
{
	siMACD		= 0,
	siKDJ		= 1,
	siRSI		= 2,
	siSOM		= 3,
} eStockIndicator;

typedef enum
{
	gpUpper		= 0,
	gpMidlle	= 1,
	gpLower		= 2,
} eGraphPosition;

typedef struct daySummary
{
	long date;
	double open;
	double high;
	double low;
	double close;
	double volume;
	double adjClose;
	double priceMA[MA_LINE_MAX];
	double volumeMA[MA_LINE_MAX];
	//double priceMA_1;
	//double priceMA_2;
	//double priceMA_3;
	//double priceMA_4;
	//double priceMA_5;
	//double volumeMA_1;
	//double volumeMA_2;
	//double volumeMA_3;
	//double volumeMA_4;
	//double volumeMA_5;
	KPATTERN kPattern1;
	KPATTERN kPattern2;
	KPATTERN kPattern3;
	KPATTERN kPattern4;
} SUMMARY;

enum ePriceChange
{
	pcIncreaseMono = 0,
	pcIncrease = 1,
	pcDecreaseMono = 2,
	pcDecrease = 3,
};

enum eAveragePeriod
{
	ap5Days = 5,
	ap10Days = 10,
	ap20Days = 20,
	ap30Days = 30,
	ap60Days = 60,
};

typedef struct futureTrend
{
	int nIndex;
	bool bRiseNextDay;		// close値で判断
	double fRateNextDay;
	bool bRiseIn5Days;		// priceMA_1値で判断
	double fRateIn5Days;
	bool bRiseIn10Days;		// priceMA_2値で判断
	double fRateIn10Days;
} TREND;

class CStockGraph : public CStatic
{
public:

	CStockGraph();
	virtual ~CStockGraph();

	enum{
		// 色設定
		defRgbRed = RGB(240, 48, 48),
		defRgbGreen = RGB(32, 240, 230),
		defRgbBlue = RGB(0, 0, 255),
		defRgbWhite = RGB(255, 255, 255),
		defRgbYellow = RGB(255, 255, 0),
		defRgbBack	= RGB(0,0,0),
		defRgbYAxis = RGB(160, 160, 160),
		defRgbGraph = RGB(192, 0, 0),
		defRgbLine1 = RGB(216, 44, 235),
		defRgbLine2 = RGB(218, 222, 58),
		defRgbLine3 = RGB(51, 218, 37),
		defRgbLine4 = RGB(111, 129, 251),
		defRgbLine5 = RGB(222, 111, 5),
	};

	void SetColor(COLORREF rgbBack, COLORREF rgbYAxis, COLORREF rgbGraph);
	void SetDateRange(int iLast, int nNum);
	void CalculateDataRange();
	//void AnalyzeData(SUMMARY* pHistory, int nCount = 1);
	void SetStockPrice(CStockPrice* pStockPrice);
	int FindCount(SUMMARY* pDate, int nPeriod);
	int FindNext(SUMMARY* pDate, int nPeriod);
	void SetMark(int iStart, int nCount);
	void SetDrawMode(eGraphPeriod ePeriod, eStockIndicator eIndicator);

	// オーバーライド
	//{{AFX_VIRTUAL(CStockGraph)
	//}}AFX_VIRTUAL

private:

	CPen		m_penYAxis; 		// Y 軸描画用ペン
	CPen		m_penWhite;		// 上段、中段、下段の分割線
	CPen		m_penRed;
	CPen		m_penGreen;
	CPen		m_penBlue;
	CPen		m_penYellow;
	CPen		m_penMA[MA_LINE_MAX];
	CPen		m_penMark;			// 赤い点線

	CBrush		m_brushRed;
	CBrush		m_brushGreen;

	int 		m_nNumDayDisplay; 		// 画面上表示されるデータ数
	int			m_nLastIndex;			// 画面上表示される最新のデータ（一番右の）
	double		m_BottomUpper;			// 上段描画範囲の下限
	double		m_TopUpper;				// 上段描画範囲の上限
	double		m_BottomUpperMarket;			// 上段MarketIndex描画範囲の下限
	double		m_TopUpperMarket;			// 上段MarketIndex描画範囲の上限
	double		m_BottomMiddle;			// 中段描画範囲の下限
	double		m_TopMiddle;			// 中段描画範囲の上限
	double		m_BottomLower;			// 下段描画範囲の上限
	double		m_TopLower;				// 下段描画範囲の上限
	int			m_nWidth;
	int			m_nHeight;
	int			m_nHeightUpper;
	int			m_nHeightMiddle;
	int			m_nHeightLower;
	int			m_nYSpace;
	double		m_fPlotSpace;	// プロットの間隔
	double		m_fMargin;
	int			m_nMarkStart;
	int			m_nMarkCount;
	int			m_eGraphPeriod;
	int			m_eIndicator;

	//SUMMARY*	m_pHistory;
	//int			m_nDaysTotal;
	CStockPrice* m_pStockPrice;
	std::multimap<KPATTERN, TREND> m_mapKPatternTrend;
	int			m_nFindFrom;
	//std::multimap<KPATTERN, TREND>::iterator m_mapIterator;
	int			m_nAveragePeriod[MA_LINE_MAX];

	void Draw(CDC* pDC);
	void DrawBackground(CDC* pDC);
	void DrawKLine(CDC* pDC);
	void DrawMarketIndex(CDC* pDC, int market);
	void DrawMovingAverages(CDC* pDC, int iLine);
	void DrawMACD(CDC* pDC);
	void DrawKDJ(CDC* pDC);
	void DrawRSI(CDC* pDC);
	void DrawSOM(CDC* pDC);
	void DrawLine(CDC* pDC, double* pInput, eGraphType type, CPen* pPen);
	void DrawMark(CDC* pDC);
	void DrawVolume(CDC* pDC);

	int GetYPos(double value, eGraphType type);
	int GetYPosUpper(double value);
	int GetYPosUpperMarket(double value);
	int GetYPosMiddle(double value);
	int GetYPosLower(double value);
	int IndexToXPos(int nIndex);

	void CalculateMovingAverages();
	void CalculateKLinePattern();
	void MakeKPatternMap();
	BYTE EncodeRateToTwoBit(double fChangeRate);
	BYTE EncodeOneDayToBYTE(SUMMARY* pData);
	KPATTERN EncodeKPattern(SUMMARY* pData, int nPeriod);
	ePriceChange GetClosePriceTrend(SUMMARY* pData, int nPeriod);
	ePriceChange GetVolumeTrend(SUMMARY* pData, int nPeriod);
	ePriceChange GetMA5Trend(SUMMARY* pData, int nPeriod);

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CStockGraph)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
