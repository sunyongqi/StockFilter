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

double MARKET_INDEX_SS[NUM_DAYS_MAX];
double MARKET_INDEX_SZ[NUM_DAYS_MAX];

CStockPrice::CStockPrice()
{
	m_nDaysTotal = 0;
}

CStockPrice::CStockPrice(std::string strCode)
{
	if (strCode.front() == L'6')
	{
		strCode += ".ss";
		MARKET = SHANGHAI;
	}
	else
	{
		strCode += ".sz";
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

bool CStockPrice::DownloadSingleStockPrices()
{
	CURL *curl;
	FILE *fp;
	CURLcode res;
	std::string strYahooURL = "http://table.finance.yahoo.com/table.csv?s=";
	std::string strStockURL = strYahooURL + m_strCode;
	std::string strFileName = PRICEFILE_PATH;
	strFileName += m_strCode;
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
	CURL *curl;
	FILE *fp;
	CURLcode res;
	std::string strYahooURL = "http://table.finance.yahoo.com/table.csv?s=";

	std::map<std::wstring, std::wstring>::iterator it = g_mapCodeName.begin();
	int nCount = 0;

	while (it != g_mapCodeName.end())
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;
		std::string strCode = cv.to_bytes(it->first);
		it++;
		std::string strStockURL = strYahooURL + strCode;
		std::string strFileName = PRICEFILE_PATH;
		strFileName += strCode;
		strFileName += ".csv";
		curl = curl_easy_init();
		if (curl)
		{
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
				continue;

			fclose(fp);
			nCount++;
		}
	}
	return nCount;
}

bool CStockPrice::LoadMarketIndices()
{
	CURL *curl;
	FILE *fp;
	CURLcode res;
	std::string strYahooURL = "http://table.finance.yahoo.com/table.csv?s=";


	// SHANGHAI
	std::string strCode = "000001.ss";
	std::string strStockURL = strYahooURL + strCode;
	std::string strFileName = PRICEFILE_PATH;
	strFileName += strCode;
	strFileName += ".csv";
	curl = curl_easy_init();
	if (curl)
	{
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

		if (fopen_s(&fp, strFileName.c_str(), "r"))
			return false;
		char cYear[16], cMonth[16], cDay[16], cOpen[16], cHigh[16], cLow[16], cClose[16], cVolume[16], cAdjClose[16];
		char temp[256];
		fgets(temp, 256, fp);
		for (int i = 0; i < NUM_DAYS_MAX; i++) {
			cYear[0] = cMonth[0] = cDay[0] = cOpen[0] = cHigh[0] = cLow[0] = cClose[0] = cVolume[0] = cAdjClose[0] = '\0';
			if (fscanf_s(fp, "%[^-]-%[^-]-%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]\n", cYear, 16, cMonth, 16, cDay, 16, cOpen, 16, cHigh, 16, cLow, 16, cClose, 16, cVolume, 16, cAdjClose, 16) == EOF)
				break;

			MARKET_INDEX_SS[i] = strtod(cClose, NULL);
		}
		fclose(fp);
	}

	// SHENZHEN
	strCode = "399001.sz";
	strStockURL = strYahooURL + strCode;
	strFileName = PRICEFILE_PATH;
	strFileName += strCode;
	strFileName += ".csv";
	curl = curl_easy_init();
	if (curl)
	{
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

		if (fopen_s(&fp, strFileName.c_str(), "r"))
			return false;

		char cYear[16], cMonth[16], cDay[16], cOpen[16], cHigh[16], cLow[16], cClose[16], cVolume[16], cAdjClose[16];
		char temp[256];
		fgets(temp, 256, fp);
		for (int i = 0; i < NUM_DAYS_MAX; i++) {
			cYear[0] = cMonth[0] = cDay[0] = cOpen[0] = cHigh[0] = cLow[0] = cClose[0] = cVolume[0] = cAdjClose[0] = '\0';
			if (fscanf_s(fp, "%[^-]-%[^-]-%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]\n", cYear, 16, cMonth, 16, cDay, 16, cOpen, 16, cHigh, 16, cLow, 16, cClose, 16, cVolume, 16, cAdjClose, 16) == EOF)
				break;

			MARKET_INDEX_SZ[i] = strtod(cClose, NULL);
		}
		fclose(fp);
	}

	return true;
}

int CStockPrice::LoadStockData()
{
	LoadPriceFile();

	CalculateMA();

	return m_nDaysTotal;
}

bool CStockPrice::LoadPriceFile()
{
	std::string strFileName = PRICEFILE_PATH;
	strFileName += m_strCode;
	strFileName += ".csv";
	FILE* fp = NULL;
	if (fopen_s(&fp, strFileName.c_str(), "r"))
		return false;

	char cYear[16], cMonth[16], cDay[16], cOpen[16], cHigh[16], cLow[16], cClose[16], cVolume[16], cAdjClose[16];

	char temp[256];
	fgets(temp, 256, fp);
	int i;
	for (i = 0; i < NUM_DAYS_MAX; i++) {
		cYear[0] = cMonth[0] = cDay[0] = cOpen[0] = cHigh[0] = cLow[0] = cClose[0] = cVolume[0] = cAdjClose[0] = '\0';
		if (fscanf_s(fp, "%[^-]-%[^-]-%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]\n", cYear, 16, cMonth, 16, cDay, 16, cOpen, 16, cHigh, 16, cLow, 16, cClose, 16, cVolume, 16, cAdjClose, 16) == EOF)
			break;
		DATE[i] = strtol(cYear, NULL, 10) * 10000 + strtol(cMonth, NULL, 10) * 100 + strtol(cDay, NULL, 10);
		OPEN[i] = strtod(cOpen, NULL);
		HIGH[i] = strtod(cHigh, NULL);
		LOW[i] = strtod(cLow, NULL);
		CLOSE[i] = strtod(cClose, NULL);
		VOLUME[i] = strtol(cVolume, NULL, 10);
		ADJCLOSE[i] = strtod(cAdjClose, NULL);
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

void CStockPrice::MA(double* pInput, double* pOutput, int nPeriod, int nDaysTotal)
{
	if (nDaysTotal <= 0 || nPeriod <= 0)
		return;

	for (int iDay = 0; iDay < nDaysTotal; iDay++)
	{
		double fSum = 0;
		for (int iPeriod = 0; iPeriod < nPeriod; iPeriod++)
		{
			if (iDay + iPeriod >= nDaysTotal)
				break;
			fSum += *(pInput + iDay + iPeriod);
		}
		*(pOutput + iDay) = fSum / nPeriod;
	}
}

void CStockPrice::MA(long* pInput, long* pOutput, int nPeriod, int nDaysTotal)
{
	if (nDaysTotal <= 0 || nPeriod <= 0)
		return;

	for (int iDay = 0; iDay < nDaysTotal; iDay++)
	{
		long nSum = 0;
		for (int iPeriod = 0; iPeriod < nPeriod; iPeriod++)
		{
			if (iDay + iPeriod >= nDaysTotal)
				break;
			nSum += *(pInput + iDay + iPeriod);
		}
		*(pOutput + iDay) = nSum / nPeriod;
	}
}