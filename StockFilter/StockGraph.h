#pragma once

#include <map>

#define NUM_LINE_DISPLAY	81
#define MA_LINE_MAX			5

class CStockPrice;

typedef DWORD KPATTERN;

typedef enum
{
	gtRealtime	= 0,
	gtDay		= 1,
	gtWeek		= 2,
} eGraphType;

typedef enum
{
	siMACD		= 0,
	siKDJ		= 1,
	siRSI		= 2,
	siSOM		= 3,
} eStockIndicator;

typedef struct daySummary
{
	long date;
	double open;
	double high;
	double low;
	double close;
	long volume;
	double adjClose;
	double priceMA[MA_LINE_MAX];
	long volumeMA[MA_LINE_MAX];
	//double priceMA_1;
	//double priceMA_2;
	//double priceMA_3;
	//double priceMA_4;
	//double priceMA_5;
	//long volumeMA_1;
	//long volumeMA_2;
	//long volumeMA_3;
	//long volumeMA_4;
	//long volumeMA_5;
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
	bool bRiseNextDay;		// close�l�Ŕ��f
	double fRateNextDay;
	bool bRiseIn5Days;		// priceMA_1�l�Ŕ��f
	double fRateIn5Days;
	bool bRiseIn10Days;		// priceMA_2�l�Ŕ��f
	double fRateIn10Days;
} TREND;

class CStockGraph : public CStatic
{
public:

	CStockGraph();
	virtual ~CStockGraph();

	enum{
		// �F�ݒ�
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
	int GetYPosUpper(double value);
	int GetYPosUpperMarket(double value);
	int GetYPosMiddle(double value);
	int GetYPosLower(long value);
	int IndexToXPos(int nIndex);
	int FindCount(SUMMARY* pDate, int nPeriod);
	int FindNext(SUMMARY* pDate, int nPeriod);
	void SetMark(int iStart, int nCount);
	void SetDrawMode(int nType, int nIndicator);

	// �I�[�o�[���C�h
	//{{AFX_VIRTUAL(CStockGraph)
	//}}AFX_VIRTUAL

private:

	CPen		m_penYAxis; 		// Y ���`��p�y��
	CPen		m_penWhite;		// ��i�A���i�A���i�̕�����
	CPen		m_penRed;
	CPen		m_penGreen;
	CPen		m_penBlue;
	CPen		m_penYellow;
	CPen		m_penMA[MA_LINE_MAX];
	CPen		m_penMark;			// �Ԃ��_��

	CBrush		m_brushRed;
	CBrush		m_brushGreen;

	int 		m_nNumDayDisplay; 		// ��ʏ�\�������f�[�^��
	int			m_nLastIndex;			// ��ʏ�\�������ŐV�̃f�[�^�i��ԉE�́j
	double		m_BottomUpper;			// ��i�`��͈͂̉���
	double		m_TopUpper;				// ��i�`��͈͂̏��
	double		m_BottomUpperMarket;			// ��iMarketIndex�`��͈͂̉���
	double		m_TopUpperMarket;			// ��iMarketIndex�`��͈͂̏��
	double		m_BottomMiddle;			// ���i�`��͈͂̉���
	double		m_TopMiddle;			// ���i�`��͈͂̏��
	long		m_BottomLower;			// ���i�`��͈͂̏��
	long		m_TopLower;				// ���i�`��͈͂̏��
	int			m_nWidth;
	int			m_nHeight;
	int			m_nHeightUpper;
	int			m_nHeightMiddle;
	int			m_nHeightLower;
	int			m_nYSpace;
	double		m_fPlotSpace;	// �v���b�g�̊Ԋu
	double		m_fMargin;
	int			m_nMarkStart;
	int			m_nMarkCount;
	int			m_nGraphType;
	int			m_nIndicator;

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
	void DrawSOM(CDC* pDC);
	void DrawMark(CDC* pDC);
	void DrawVolume(CDC* pDC);

	void CalculateMovingAverages();
	void CalculateKLinePattern();
	void MakeKPatternMap();
	BYTE EncodeRateToTwoBit(double fChangeRate);
	BYTE EncodeOneDayToBYTE(SUMMARY* pData);
	KPATTERN EncodeKPattern(SUMMARY* pData, int nPeriod);
	ePriceChange GetClosePriceTrend(SUMMARY* pData, int nPeriod);
	ePriceChange GetVolumeTrend(SUMMARY* pData, int nPeriod);
	ePriceChange GetMA5Trend(SUMMARY* pData, int nPeriod);

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CStockGraph)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
