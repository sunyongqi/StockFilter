#include "stdafx.h"
#include "StockPrice.h"

//extern "C"
//{

//}
#include <errno.h>
#include <stdlib.h>
#ifndef WIN32
#  include <unistd.h>
#endif

#include <map>
#include <string>
//#include <locale> 
#include <codecvt>

extern std::map<std::wstring, std::wstring> g_mapCodeName;

CStockPrice g_MarketIndexShanghai;
CStockPrice g_MarketIndexShenzhen;

//double MARKET_INDEX_SS[NUM_DAYS_MAX];
//double MARKET_INDEX_SZ[NUM_DAYS_MAX];

CStockPrice::CStockPrice()
{
	m_nDaysTotal = 0;
}

CStockPrice::CStockPrice(std::string strCode)
{
	if (strCode.front() == L'6')
	{
		MARKET = SHANGHAI;
	}
	else
	{
		MARKET = SHENZHEN;
	}
	m_strCode = strCode;
	m_nDaysTotal = 0;
}

CStockPrice::~CStockPrice()
{
}

#define MAX_BUF 1024
static char wr_buf[MAX_BUF];
static int  wr_index = 0;

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
	size_t written = fwrite(ptr, size, nmemb, stream);
	return written;
}

bool CStockPrice::DownloadStockPrices()
{
	if (m_strCode.front() == L'6')
	{
		MARKET = SHANGHAI;
	}
	else
	{
		MARKET = SHENZHEN;
	}
	return DownloadData163(m_strCode);
	//return DownloadDataYahoo(m_strCode);
}

bool CStockPrice::DownloadStockPrices(std::string strCode)
{
	m_strCode = strCode;
	return DownloadData163(m_strCode);
	//return DownloadDataYahoo(m_strCode);
}

bool CStockPrice::DownloadDataYahoo(std::string strCode)
{
	if (strCode != "000001.ss" && strCode != "399001.sz")
	{
		if (strCode.front() == '6')
		{
			strCode += ".ss";
		}
		else
		{
			strCode += ".sz";
		}
	}

	CURL *curl;
	FILE *fp;
	CURLcode res;
	std::string strYahooURL = "http://table.finance.yahoo.com/table.csv?s=";
	std::string strStockURL = strYahooURL + strCode;
	std::string strFileName = PRICEFILE_PATH_YAHOO;
	strFileName += "Yahoo_";
	strFileName += strCode;
	strFileName += ".csv";
	curl = curl_easy_init();
	if (curl) {
		if (!fopen_s(&fp, strFileName.c_str(), "wb"))
		{
			curl_easy_setopt(curl, CURLOPT_URL, strStockURL.c_str());
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
			res = curl_easy_perform(curl);
			/* always cleanup */
			curl_easy_cleanup(curl);
		}
		else
			return false;

		fclose(fp);
	}
	else
		return false;

	return true;
}

bool CStockPrice::DownloadData163(std::string strCode)
{
	if (strCode != "0000001" && strCode != "1399001")
	{
		if (strCode.front() == '6')
		{
			strCode = "0" + strCode;
		}
		else
		{
			strCode = "1" + strCode;
		}
	}

	CURL *curl;
	FILE *fp;
	CURLcode res;
	std::string strYahooURL = "http://quotes.money.163.com/service/chddata.html?code=";
	std::string strStartDate = "&start=20150101";
	std::string strStockURL = strYahooURL + strCode + strStartDate;
	std::string strFileName = PRICEFILE_PATH_163;
	strFileName += "163_";
	strFileName += strCode;
	strFileName += ".csv";
	curl = curl_easy_init();
	if (curl) {
		if (!fopen_s(&fp, strFileName.c_str(), "wb"))
		{
			curl_easy_setopt(curl, CURLOPT_URL, strStockURL.c_str());
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
			res = curl_easy_perform(curl);
			/* always cleanup */
			curl_easy_cleanup(curl);
		}
		else
			return false;

		fclose(fp);
	}
	else
		return false;

	return true;
}

int CStockPrice::DownloadAllStocksPrices()
{
	std::map<std::wstring, std::wstring>::iterator it = g_mapCodeName.begin();
	int nCount = 0;

	while (it != g_mapCodeName.end())
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;
		std::string strCode = cv.to_bytes(it->first);
		if (DownloadData163(strCode))
		{
			nCount++;
		}
		it++;
	}
	return nCount;
}

bool CStockPrice::LoadMarketIndices()
{
	return (g_MarketIndexShanghai.DownloadStockPrices("0000001") &&
		g_MarketIndexShanghai.LoadStockData() &&
		g_MarketIndexShenzhen.DownloadStockPrices("1399001") &&
		g_MarketIndexShenzhen.LoadStockData());
}


//bool CStockPrice::LoadMarketIndices()
//{
//	CURL *curl;
//	FILE *fp;
//	CURLcode res;
//	std::string strYahooURL = "http://table.finance.yahoo.com/table.csv?s=";
//
//
//	// SHANGHAI
//	std::string strCode = "000001.ss";
//	std::string strStockURL = strYahooURL + strCode;
//	std::string strFileName = PRICEFILE_PATH_YAHOO;
//	strFileName += strCode;
//	strFileName += ".csv";
//	curl = curl_easy_init();
//	if (curl)
//	{
//		if (!fopen_s(&fp, strFileName.c_str(), "wb"))
//		{
//			curl_easy_setopt(curl, CURLOPT_URL, strStockURL.c_str());
//			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
//			curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
//			res = curl_easy_perform(curl);
//			/* always cleanup */
//			curl_easy_cleanup(curl);
//		}
//		else
//			return false;
//		fclose(fp);
//
//		if (fopen_s(&fp, strFileName.c_str(), "r"))
//			return false;
//		char cYear[16], cMonth[16], cDay[16], cOpen[16], cHigh[16], cLow[16], cClose[16], cVolume[16], cAdjClose[16];
//		char temp[256];
//		fgets(temp, 256, fp);
//		for (int i = 0; i < NUM_DAYS_MAX; i++) {
//			cYear[0] = cMonth[0] = cDay[0] = cOpen[0] = cHigh[0] = cLow[0] = cClose[0] = cVolume[0] = cAdjClose[0] = '\0';
//			if (fscanf_s(fp, "%[^-]-%[^-]-%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]\n", cYear, 16, cMonth, 16, cDay, 16, cOpen, 16, cHigh, 16, cLow, 16, cClose, 16, cVolume, 16, cAdjClose, 16) == EOF)
//				break;
//
//			MARKET_INDEX_SS[i] = strtod(cClose, NULL);
//		}
//		fclose(fp);
//	}
//
//	// SHENZHEN
//	strCode = "399001.sz";
//	strStockURL = strYahooURL + strCode;
//	strFileName = PRICEFILE_PATH_YAHOO;
//	strFileName += strCode;
//	strFileName += ".csv";
//	curl = curl_easy_init();
//	if (curl)
//	{
//		if (!fopen_s(&fp, strFileName.c_str(), "wb"))
//		{
//			curl_easy_setopt(curl, CURLOPT_URL, strStockURL.c_str());
//			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
//			curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
//			res = curl_easy_perform(curl);
//			/* always cleanup */
//			curl_easy_cleanup(curl);
//		}
//		else
//			return false;
//		fclose(fp);
//
//		if (fopen_s(&fp, strFileName.c_str(), "r"))
//			return false;
//
//		char cYear[16], cMonth[16], cDay[16], cOpen[16], cHigh[16], cLow[16], cClose[16], cVolume[16], cAdjClose[16];
//		char temp[256];
//		fgets(temp, 256, fp);
//		for (int i = 0; i < NUM_DAYS_MAX; i++) {
//			cYear[0] = cMonth[0] = cDay[0] = cOpen[0] = cHigh[0] = cLow[0] = cClose[0] = cVolume[0] = cAdjClose[0] = '\0';
//			if (fscanf_s(fp, "%[^-]-%[^-]-%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]\n", cYear, 16, cMonth, 16, cDay, 16, cOpen, 16, cHigh, 16, cLow, 16, cClose, 16, cVolume, 16, cAdjClose, 16) == EOF)
//				break;
//
//			MARKET_INDEX_SZ[i] = strtod(cClose, NULL);
//		}
//		fclose(fp);
//	}
//
//	return true;
//}

int CStockPrice::LoadStockData()
{
	LoadPriceFile();

	CalculateMA();
	CalculateMACD();
	CalculateKDJ(9);
	CalculateRSI(6);
	CalculateSOM();

	return m_nDaysTotal;
}

bool CStockPrice::LoadPriceFile()
{
	return LoadFile163();
	//return LoadFileYahoo();
}

bool CStockPrice::LoadFileYahoo()
{
	std::string strCode = m_strCode;
	if (strCode != "000001.ss" && strCode != "399001.sz")
	{
		if (strCode.front() == '6')
		{
			strCode = strCode + ".ss";
		}
		else
		{
			strCode = strCode + ".sz";
		}
	}
	std::string strFileName = PRICEFILE_PATH_YAHOO;
	strFileName += "Yahoo_";
	strFileName += strCode;
	strFileName += ".csv";
	FILE* fp = NULL;
	if (fopen_s(&fp, strFileName.c_str(), "r"))
		return false;

	char cYear[16], cMonth[16], cDay[16], cOpen[16], cHigh[16], cLow[16], cClose[16], cVolume[16], cAdjClose[16];

	char temp[256];
	fgets(temp, 256, fp);
	int i;
	for (i = 0; i < NUM_DAYS_MAX; i++) {
		//cYear[0] = cMonth[0] = cDay[0] = cOpen[0] = cHigh[0] = cLow[0] = cClose[0] = cVolume[0] = cAdjClose[0] = '\0';
		if (fscanf_s(fp, "%[^-]-%[^-]-%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]\n", cYear, 16, cMonth, 16, cDay, 16, cOpen, 16, cHigh, 16, cLow, 16, cClose, 16, cVolume, 16, cAdjClose, 16) == EOF)
			break;
		DATE[i] = strtol(cYear, NULL, 10) * 10000 + strtol(cMonth, NULL, 10) * 100 + strtol(cDay, NULL, 10);
		OPEN[i] = strtod(cOpen, NULL);
		HIGH[i] = strtod(cHigh, NULL);
		LOW[i] = strtod(cLow, NULL);
		CLOSE[i] = strtod(cClose, NULL);
		VOLUME[i] = strtod(cVolume, NULL);
		//ADJCLOSE[i] = strtod(cAdjClose, NULL);
	}
	fclose(fp);
	m_nDaysTotal = i;

	return true;
}

bool CStockPrice::LoadFile163()
{
	std::string strCode = m_strCode;
	bool bIsMarketIndex = false;
	if (strCode != "0000001" && strCode != "1399001")
	{
		if (strCode.front() == '6')
		{
			strCode = "0" + strCode;
		}
		else
		{
			strCode = "1" + strCode;
		}
	}
	else
		bIsMarketIndex = true;
	std::string strFileName = PRICEFILE_PATH_163;
	strFileName += "163_";
	strFileName += strCode;
	strFileName += ".csv";
	FILE* fp = NULL;
	if (fopen_s(&fp, strFileName.c_str(), "r"))
		return false;

	// 日期,股票代码,名称,收盘价,最高价,最低价,开盘价,前收盘,涨跌额,涨跌幅,换手率,成交量,成交金额,总市值,流通市值,成交笔数
	// 2016 - 02 - 17, '000780,平庄能源,4.72,4.85,4.58,4.62,4.71,0.01,0.2123,1.6935,17177567,80265069.68,4787525849.28,4787525849.28,4832
	char cYear[16], cMonth[16], cDay[16], cCode[16], cName[16], cClose[16], cHigh[16], cLow[16], cOpen[16], cLastClose[16], cChange[16], cPercent[16], cTurnover[16], cVolume[16], cAmount[32], cTotalMarketCap[16], cMarketCap[16], cQuantity[16];

	char temp[256];
	fgets(temp, 256, fp);
	int i;
	for (i = 0; i < NUM_DAYS_MAX; i++) {
		if (!bIsMarketIndex)
		{
			if (fscanf_s(fp, "%[^-]-%[^-]-%[^,], '%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]\n", cYear, 16, cMonth, 16, cDay, 16, cCode, 16, cName, 16, cClose, 16, cHigh, 16, cLow, 16, cOpen, 16, cLastClose, 16, cChange, 16, cPercent, 16, cTurnover, 16, cVolume, 16, cAmount, 32, cTotalMarketCap, 16, cMarketCap, 16, cQuantity, 16) == EOF)
				break;
		}
		else
		{
			if (fscanf_s(fp, "%[^-]-%[^-]-%[^,], '%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],,%[^,],%[^,],,,%[^\n]\n", cYear, 16, cMonth, 16, cDay, 16, cCode, 16, cName, 16, cClose, 16, cHigh, 16, cLow, 16, cOpen, 16, cLastClose, 16, cChange, 16, cPercent, 16, cVolume, 16, cAmount, 32, cQuantity, 16) == EOF)
				break;
		}
			
		m_strName = cName;
		DATE[i] = strtol(cYear, NULL, 10) * 10000 + strtol(cMonth, NULL, 10) * 100 + strtol(cDay, NULL, 10);
		OPEN[i] = strtod(cOpen, NULL);
		HIGH[i] = strtod(cHigh, NULL);
		LOW[i] = strtod(cLow, NULL);
		CLOSE[i] = strtod(cClose, NULL);
		VOLUME[i] = strtod(cVolume, NULL);
		if (!bIsMarketIndex)
			TURNOVER[i] = strtod(cTurnover, NULL);
	}
	fclose(fp);
	m_nDaysTotal = i;

	return true;
}

void CStockPrice::CalculateMA()
{
	MA(CLOSE, PRICE_MA5, 5, m_nDaysTotal);
	MA(CLOSE, PRICE_MA10, 10, m_nDaysTotal);
	MA(CLOSE, PRICE_MA20, 20, m_nDaysTotal);
	MA(CLOSE, PRICE_MA30, 30, m_nDaysTotal);
	MA(CLOSE, PRICE_MA60, 60, m_nDaysTotal);

	MA(VOLUME, VOLUME_MA5, 5, m_nDaysTotal);
	MA(VOLUME, VOLUME_MA10, 10, m_nDaysTotal);
	MA(VOLUME, VOLUME_MA20, 20, m_nDaysTotal);
	MA(VOLUME, VOLUME_MA30, 30, m_nDaysTotal);
	MA(VOLUME, VOLUME_MA60, 60, m_nDaysTotal);
}

void CStockPrice::CalculateMACD()
{
	EMA(CLOSE, EMA12, 12, m_nDaysTotal);
	EMA(CLOSE, EMA26, 26, m_nDaysTotal);
	for (int i = 0; i < m_nDaysTotal; i++)
		DIF[i] = EMA12[i] - EMA26[i];
	EMA(DIF, DEA, 9, m_nDaysTotal);
	for (int i = 0; i < m_nDaysTotal; i++)
		MACD[i] = (DIF[i] - DEA[i]) * 2;
}

void CStockPrice::CalculateKDJ(unsigned int nPeriod)
{
	K[m_nDaysTotal - nPeriod] = D[m_nDaysTotal - nPeriod] = 50;
	for (int i = m_nDaysTotal - nPeriod - 1; i >= 0; i--)
	{
		double Ln = MIN(LOW, i, i + nPeriod - 1, m_nDaysTotal);
		double Hn = MAX(HIGH, i, i + nPeriod - 1, m_nDaysTotal);
		if (Ln != Hn)
		{
			double RSVi = (CLOSE[i] - Ln) / (Hn - Ln) * 100.0;
			K[i] = K[i + 1] * 2.0 / 3.0 + RSVi / 3.0;
		}
		else
		{
			K[i] = K[i + 1];
		}
		D[i] = D[i + 1] * 2.0 / 3.0 + K[i] / 3.0;
		J[i] = K[i] * 3 - D[i] * 2;
	}
}

void CStockPrice::CalculateRSI(unsigned int nPeriod)
{
	for (int i = 0; i < m_nDaysTotal; i++)
	{
		double RSi = 0;
		double sumRise = 0;
		double sumRiseFall = 0;
		for (unsigned int j = 0; j < nPeriod - 1; j++)
		{
			double diff = CLOSE[i + j] - CLOSE[i + j + 1];
			if (diff > 0)
				sumRise += diff;
			sumRiseFall += abs(diff);
		}
		if (sumRiseFall > 0)
			RSI[i] = 100 * sumRise / sumRiseFall;
		else
			RSI[i] = 50;
		//RSI[i] = 100.0 * RSi / (1 + RSi);
	}
}

void CStockPrice::CalculateSOM()
{
	double* pMarketIndex = GetMarketIndex();
	for (int i = 0; i < m_nDaysTotal; i++)
		SOM[i] = CLOSE[i] / pMarketIndex[i] * 100;
	MA(SOM, SOM_MA5, 5, m_nDaysTotal);
	MA(SOM, SOM_MA10, 10, m_nDaysTotal);
}

double* CStockPrice::GetMarketIndex()
{
	double* pMarketIndex = NULL;
	if (MARKET == SHANGHAI)
		pMarketIndex = g_MarketIndexShanghai.CLOSE;
	else
		pMarketIndex = g_MarketIndexShenzhen.CLOSE;
	return pMarketIndex;
}

double CStockPrice::MAX(double* pInput, unsigned int iStart, unsigned int iEnd, unsigned int nDaysTotal)
{
	ASSERT(iEnd < nDaysTotal);
	ASSERT(iEnd >= iStart);
	double max = pInput[iStart];
	for (unsigned int i = iStart + 1; i <= iEnd; i++)
	{
		if (max < pInput[i])
			max = pInput[i];
	}
	return max;
}

double CStockPrice::MIN(double* pInput, unsigned int iStart, unsigned int iEnd, unsigned int nDaysTotal)
{
	ASSERT(iEnd < nDaysTotal);
	ASSERT(iEnd >= iStart);
	double min = pInput[iStart];
	for (unsigned int i = iStart + 1; i <= iEnd; i++)
	{
		if (min > pInput[i])
			min = pInput[i];
	}
	return min;
}

void CStockPrice::MA(double* pInput, double* pOutput, unsigned int nPeriod, unsigned int nDaysTotal)
{
	if (nDaysTotal <= 0 || nPeriod <= 0)
		return;

	for (unsigned int iDay = 0; iDay < nDaysTotal; iDay++)
	{
		double fSum = 0;
		for (unsigned int iPeriod = 0; iPeriod < nPeriod; iPeriod++)
		{
			if (iDay + iPeriod >= nDaysTotal)
				break;
			fSum += pInput[iDay + iPeriod];
		}
		pOutput[iDay] = fSum / nPeriod;
	}
}

void CStockPrice::MA(long* pInput, long* pOutput, unsigned int nPeriod, unsigned int nDaysTotal)
{
	if (nDaysTotal <= 0 || nPeriod <= 0)
		return;

	for (unsigned int iDay = 0; iDay < nDaysTotal; iDay++)
	{
		long nSum = 0;
		for (unsigned int iPeriod = 0; iPeriod < nPeriod; iPeriod++)
		{
			if (iDay + iPeriod >= nDaysTotal)
				break;
			nSum += pInput[iDay + iPeriod];
		}
		pOutput[iDay] = nSum / nPeriod;
	}
}

void CStockPrice::EMA(double* pInput, double* pOutput, unsigned int nPeriod, unsigned int nDaysTotal)
{
	if (nDaysTotal <= 0 || nPeriod <= 0)
		return;

	int nCount = (1 + nPeriod) * nPeriod / 2;
	double fSum = 0;
	for (unsigned int iPeriod = 1; iPeriod <= nPeriod; iPeriod++)
	{
		pOutput[nDaysTotal - iPeriod] = 0;
		fSum += pInput[nDaysTotal - iPeriod] * (double)iPeriod;
	}
	pOutput[nDaysTotal - nPeriod] = fSum / (double)((1 + nPeriod) * nPeriod / 2);
	for (int iDay = nDaysTotal - nPeriod - 1; iDay >= 0; iDay--)
	{
		pOutput[iDay] = (pOutput[iDay + 1] * ((double)nPeriod - 1.0) + pInput[iDay] * 2.0) / (double)(nPeriod + 1);
	}
}

int CStockPrice::GoldenCross(double* pInputFast, double* pInputSlow, unsigned int iStart, unsigned int iEnd, double threshold)
{
	ASSERT(iEnd > iStart);

	for (unsigned int i = iStart; i < iEnd; i++)
	{
		double d0 = pInputSlow[i] - pInputFast[i];
		double d1 = pInputSlow[i + 1] - pInputFast[i + 1];
		double d2 = pInputSlow[i + 2] - pInputFast[i + 2];
		if (d0 <= 0 && d1 >= threshold && d2 > d1)
			return i;
	}
	return -1;
}

int CStockPrice::DeathCross(double* pInputFast, double* pInputSlow, unsigned int iStart, unsigned int iEnd, double threshold)
{
	if (iEnd <= iStart)
		return -1;

	for (unsigned int i = iStart; i < iEnd; i++)
	{
		//if (pInputFast[i] <= pInputSlow[i] && pInputFast[i + 1] > pInputSlow[i + 1])
		double d0 = pInputSlow[i] - pInputFast[i];
		double d1 = pInputSlow[i + 1] - pInputFast[i + 1];
		double d2 = pInputSlow[i + 2] - pInputFast[i + 2];
		if (d0 >= 0 && d1 <= -threshold && d2 < d1)
			return i;
	}
	return -1;
}