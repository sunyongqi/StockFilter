#pragma once
#include "curl/multi.h"
#include <string>

#define PRICEFILE_PATH "StockData\\"
#define NUM_DAYS_MAX		8000

#define CROSS_THRESHOLD 0.005

//typedef enum
//{
//	OPEN = 0,
//	HIGH,
//	LOW,
//	CLOSE,
//	VOLUME,
//	ADJCLOSE,
//
//} eIndicator;

typedef enum
{
	SHANGHAI = 0,
	SHENZHEN = 1,
} eMarket;

extern double MARKET_INDEX_SS[NUM_DAYS_MAX];
extern double MARKET_INDEX_SZ[NUM_DAYS_MAX];

class CStockFilterDlg;
class CStockGraph;

class CStockPrice
{
public:
	CStockPrice(std::string strCode);
	CStockPrice();
	~CStockPrice();

	bool DownloadSingleStockPrices();
	int LoadStockData();
	bool LoadPriceFile();
	void CalculateMA();
	void CalculateMACD();
	void CalculateKDJ(unsigned int nPeriod);
	void CalculateRSI(unsigned int nPeriod);
	void CalculateSOM();

	int GetDaysTotal() { return m_nDaysTotal; };
	double* GetMarketIndex();

	static int DownloadAllStocksPrices();
	static bool LoadMarketIndices();
	static double MAX(double* pInput, unsigned int iStart, unsigned int iEnd, unsigned int nDaysTotal);
	static double MIN(double* pInput, unsigned int iStart, unsigned int iEnd, unsigned int nDaysTotal);
	static void MA(double* pInput, double* pOutput, unsigned int nPeriod, unsigned int nDaysTotal);
	static void MA(long* pInput, long* pOutput, unsigned int nPeriod, unsigned int nDaysTotal);
	static void EMA(double* pInput, double* pOutput, unsigned int nPeriod, unsigned int nDaysTotal);
	static int GoldenCross(double* pInputFast, double* pInputSlow, unsigned int iStart, unsigned int iEnd, double threshold = CROSS_THRESHOLD);
	static int DeathCross(double* pInputFast, double* pInputSlow, unsigned int iStart, unsigned int iEnd, double threshold = CROSS_THRESHOLD);

protected:
	long DATE[NUM_DAYS_MAX];
	double OPEN[NUM_DAYS_MAX];
	double HIGH[NUM_DAYS_MAX];
	double LOW[NUM_DAYS_MAX];
	double CLOSE[NUM_DAYS_MAX];
	double VOLUME[NUM_DAYS_MAX];
	double ADJCLOSE[NUM_DAYS_MAX];

	double PRICE_MA5[NUM_DAYS_MAX];
	double PRICE_MA10[NUM_DAYS_MAX];
	double PRICE_MA20[NUM_DAYS_MAX];
	double PRICE_MA30[NUM_DAYS_MAX];
	double PRICE_MA60[NUM_DAYS_MAX];
	double VOLUME_MA5[NUM_DAYS_MAX];
	double VOLUME_MA10[NUM_DAYS_MAX];
	double VOLUME_MA20[NUM_DAYS_MAX];
	double VOLUME_MA30[NUM_DAYS_MAX];
	double VOLUME_MA60[NUM_DAYS_MAX];
	double EMA12[NUM_DAYS_MAX];
	double EMA26[NUM_DAYS_MAX];
	double DIF[NUM_DAYS_MAX];
	double DEA[NUM_DAYS_MAX];
	double MACD[NUM_DAYS_MAX];
	double RSV[NUM_DAYS_MAX];
	double K[NUM_DAYS_MAX];
	double D[NUM_DAYS_MAX];
	double J[NUM_DAYS_MAX];
	double RSI[NUM_DAYS_MAX];
	double SOM[NUM_DAYS_MAX];	// StockPrice Over MarketIndex
	double SOM_MA5[NUM_DAYS_MAX];
	double SOM_MA10[NUM_DAYS_MAX];

	eMarket MARKET;

private:
	std::string m_strCode;
	int m_nDaysTotal;

	friend CStockGraph;
	friend CStockFilterDlg;

};

