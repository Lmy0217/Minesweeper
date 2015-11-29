/**
 *	文件：io.h
 *	读取与写入配置文件函数集
 *	
 *	Made by Luo Mingyuan
 *	Email: lmy0217@126.com
 *	谨此纪念娄芳老师
 *	2015-11-28
 */


#pragma once
#include <time.h>

/*
读配置文件
	节 BASIC
	值 ROW COL COUNT WNDX WNDY WNDSCALE OPTIONFLAG
*/
bool readProfileBasic() {
	TCHAR value[5];

	//读取ROW COL COUNT
	row = GetPrivateProfileInt(TEXT("BASIC"), TEXT("ROW"), DEFAULT_ERROR_INT, PROFILE);
	col = GetPrivateProfileInt(TEXT("BASIC"), TEXT("COL"), DEFAULT_ERROR_INT, PROFILE);
	count = GetPrivateProfileInt(TEXT("BASIC"), TEXT("COUNT"), DEFAULT_ERROR_INT, PROFILE);
	//判断ROW COL COUNT正确性
	if (row < 9 || row > 24 || col < 9 || col > 30 || count < 10 || count > 668)
	{
		row = DEFAULT_ROW;
		col = DEFAULT_COL;
		count = DEFAULT_COUNT;
		wsprintf(value, TEXT("%d"), DEFAULT_ROW);
		WritePrivateProfileString(TEXT("BASIC"), TEXT("ROW"), value, PROFILE);
		wsprintf(value, TEXT("%d"), DEFAULT_COL);
		WritePrivateProfileString(TEXT("BASIC"), TEXT("COL"), value, PROFILE);
		wsprintf(value, TEXT("%d"), DEFAULT_COUNT);
		WritePrivateProfileString(TEXT("BASIC"), TEXT("COUNT"), value, PROFILE);
	}
	brickNumber = row * col;//计算雷区块数

	//读取WNDX WNDY WNDSCALE
	wndX = GetPrivateProfileInt(TEXT("BASIC"), TEXT("WNDX"), DEFAULT_ERROR_INT, PROFILE);
	wndY = GetPrivateProfileInt(TEXT("BASIC"), TEXT("WNDY"), DEFAULT_ERROR_INT, PROFILE);
	wndScale = GetPrivateProfileInt(TEXT("BASIC"), TEXT("WNDSCALE"), DEFAULT_ERROR_INT, PROFILE);
	//判断WNDX WNDY WNDSCALE正确性
	if (wndX < -1 || wndX > GetSystemMetrics(SM_CXSCREEN) || wndY < -1 || wndY > GetSystemMetrics(SM_CYSCREEN) || wndScale < DEFAULT_WNDSCALE)
	{
		wndX = DEFAULT_WNDX;
		wndY = DEFAULT_WNDY;
		wndScale = DEFAULT_WNDSCALE;
		wsprintf(value, TEXT("%d"), DEFAULT_WNDX);
		WritePrivateProfileString(TEXT("BASIC"), TEXT("WNDX"), value, PROFILE);
		wsprintf(value, TEXT("%d"), DEFAULT_WNDY);
		WritePrivateProfileString(TEXT("BASIC"), TEXT("WNDY"), value, PROFILE);
		wsprintf(value, TEXT("%d"), DEFAULT_WNDSCALE);
		WritePrivateProfileString(TEXT("BASIC"), TEXT("WNDSCALE"), value, PROFILE);
	}

	//计算gap结构体
	gap.bottom = (int)((float)wndScale / INITIAL_BMWDITH * DEFAULT_GAP_BOTTOM);
	gap.left = (int)((float)wndScale / INITIAL_BMWDITH * DEFAULT_GAP_LEFT);
	gap.right = (int)((float)wndScale / INITIAL_BMWDITH * DEFAULT_GAP_RIGHT);
	gap.top = (int)((float)wndScale / INITIAL_BMWDITH * DEFAULT_GAP_TOP);
	//计算主窗口宽高
	wndWidth = wndScale * col + gap.left + gap.right + 2 * DEFAULT_XFRAME;
	wndHeight = wndScale * row + gap.top + gap.bottom + 2 * DEFAULT_YFRAME + DEFAULT_YTOP;
	//位置两个-1默认桌面居中
	if (wndX == -1 || wndY == -1)
	{
		wndX = (GetSystemMetrics(SM_CXSCREEN) - wndWidth) / 2;
		wndY = (GetSystemMetrics(SM_CYSCREEN) - wndHeight) / 2;
	}

	//读取OPTIONFLAG
	optionFlag = GetPrivateProfileInt(TEXT("BASIC"), TEXT("OPTIONFLAG"), DEFAULT_ERROR_INT, PROFILE);
	//判断OPTIONFLAG正确性
	if (optionFlag < 0 || optionFlag > 0xFF)
	{
		optionFlag = DEFAULT_OPTIONFLAG;
		wsprintf(value, TEXT("%d"), DEFAULT_OPTIONFLAG);
		WritePrivateProfileString(TEXT("BASIC"), TEXT("OPTIONFLAG"), value, PROFILE);
	}
	if (row == 9 && col == 9 && count == 10)//初级
	{
		if ((optionFlag & 0x03) != OF_JUNIOR)
		{
			optionFlag = optionFlag / 4 * 4 + OF_JUNIOR;
			wsprintf(value, TEXT("%d"), optionFlag);
			WritePrivateProfileString(TEXT("BASIC"), TEXT("OPTIONFLAG"), value, PROFILE);
		}
	}
	else if (row == 16 && col == 16 && count == 40)//中级
	{
		if ((optionFlag & 0x03) != OF_MEDIATE)
		{
			optionFlag = optionFlag / 4 * 4 + OF_MEDIATE;
			wsprintf(value, TEXT("%d"), optionFlag);
			WritePrivateProfileString(TEXT("BASIC"), TEXT("OPTIONFLAG"), value, PROFILE);
		}
	}
	else if (row == 16 && col == 30 && count == 99)//高级
	{
		if ((optionFlag & 0x03) != OF_SENIOR)
		{
			optionFlag = optionFlag / 4 * 4 + OF_SENIOR;
			wsprintf(value, TEXT("%d"), optionFlag);
			WritePrivateProfileString(TEXT("BASIC"), TEXT("OPTIONFLAG"), value, PROFILE);
		}
	}
	else if ((optionFlag & 0x03) != OF_USERDEFINED)//自定义
	{
		optionFlag = optionFlag / 4 * 4 + OF_USERDEFINED;
		wsprintf(value, TEXT("%d"), optionFlag);
		WritePrivateProfileString(TEXT("BASIC"), TEXT("OPTIONFLAG"), value, PROFILE);
	}
	return true;
}

/*
写配置文件
	节 BASIC
	值 ROW COL COUNT OPTIONFLAG
*/
bool writeProfileBasic()
{
	TCHAR value[5];
	wsprintf(value, TEXT("%d"), row);
	WritePrivateProfileString(TEXT("BASIC"), TEXT("ROW"), value, PROFILE);
	wsprintf(value, TEXT("%d"), col);
	WritePrivateProfileString(TEXT("BASIC"), TEXT("COL"), value, PROFILE);
	wsprintf(value, TEXT("%d"), count);
	WritePrivateProfileString(TEXT("BASIC"), TEXT("COUNT"), value, PROFILE);
	wsprintf(value, TEXT("%d"), optionFlag);
	WritePrivateProfileString(TEXT("BASIC"), TEXT("OPTIONFLAG"), value, PROFILE);
	return true;
}

/*
读配置文件
	节 SAVE
	值 SEED FIRSTLBUTTONNO TIME FLAGS
*/
bool readProfileSave() {
	seed = GetPrivateProfileInt(TEXT("SAVE"), TEXT("SEED"), DEFAULT_ERROR_INT, PROFILE);
	firstLButtonNo = GetPrivateProfileInt(TEXT("SAVE"), TEXT("FIRSTLBUTTONNO"), DEFAULT_ERROR_INT, PROFILE);
	timeCount = GetPrivateProfileInt(TEXT("SAVE"), TEXT("TIME"), DEFAULT_TIMECOUNT, PROFILE);
	if (timeCount < 0 || timeCount > 999)
		timeCount = DEFAULT_TIMECOUNT;
	saveFlags = new TCHAR[brickNumber + 1];
	GetPrivateProfileString(TEXT("SAVE"), TEXT("FLAGS"), DEFAULT_ERROR_STR, saveFlags, brickNumber + 1, PROFILE);
	if (seed > 0 && firstLButtonNo >= 0 && firstLButtonNo < brickNumber && saveFlags != DEFAULT_ERROR_STR)
	{
		hasSave = true;
		WritePrivateProfileSection(TEXT("SAVE"), TEXT(""), PROFILE);
	}
	return true;
}

/*
写配置文件
	节 SAVE
	值 SEED FIRSTLBUTTONNO TIME FLAGS
*/
bool writeProfileSave() {
	TCHAR value[11];
	wsprintf(value, TEXT("%d"), seed);
	WritePrivateProfileString(TEXT("SAVE"), TEXT("SEED"), value, PROFILE);
	wsprintf(value, TEXT("%d"), firstLButtonNo);
	WritePrivateProfileString(TEXT("SAVE"), TEXT("FIRSTLBUTTONNO"), value, PROFILE);
	wsprintf(value, TEXT("%d"), timeCount);
	WritePrivateProfileString(TEXT("SAVE"), TEXT("TIME"), value, PROFILE);
	map2save(saveFlags, mapInfo, brickNumber);
	WritePrivateProfileString(TEXT("SAVE"), TEXT("FLAGS"), saveFlags, PROFILE);
	return true;
}

/*
最短时间排序
参数：
	bestTime		最短时间数组
	date			日期数组
返回值：
	0
*/
int sort(int *bestTime, int *date)
{
	int timeMin, dateMin, temp;
	//按日期排序
	for (int i = 0; i < 5; i++)
	{
		dateMin = date[i];
		timeMin = bestTime[i];
		for (int j = i + 1; j < 5; j++)
		{
			if ((unsigned int)date[j] < (unsigned int)dateMin)
			{
				temp = dateMin;
				dateMin = date[j];
				date[j] = temp;
				temp = timeMin;
				timeMin = bestTime[j];
				bestTime[j] = temp;
			}

		}
		date[i] = dateMin;
		bestTime[i] = timeMin;
	}
	//按最短时间排序
	for (int i = 0; i < 5; i++)
	{
		timeMin = bestTime[i];
		dateMin = date[i];
		for (int j = i + 1; j < 5; j++)
		{
			if ((unsigned int)(bestTime[j] / 10) < (unsigned int)(timeMin / 10))
			{
				temp = timeMin;
				timeMin = bestTime[j];
				bestTime[j] = temp;
				temp = dateMin;
				dateMin = date[j];
				date[j] = temp;
			}

		}
		bestTime[i] = timeMin;
		date[i] = dateMin;
	}
	return 0;//
}

/*
读配置文件
	节 section
	值 GAMECOUNT WINCOUNT MOSTWIN MOSTLOSE NOWCOUNT
参数：
	section			节
	valueCount		统计数组
	bestTime		最短时间数组
	date			日期数组
返回值：
	true
*/
bool readProfileStatistic(TCHAR *section, int *valueCount, int *bestTime, int *date)
{
	valueCount[0] = GetPrivateProfileInt(section, TEXT("GAMECOUNT"), DEFAULT_ERROR_INT, PROFILE);
	valueCount[1] = GetPrivateProfileInt(section, TEXT("WINCOUNT"), DEFAULT_ERROR_INT, PROFILE);
	valueCount[3] = GetPrivateProfileInt(section, TEXT("MOSTWIN"), DEFAULT_ERROR_INT, PROFILE);
	valueCount[4] = GetPrivateProfileInt(section, TEXT("MOSTLOSE"), DEFAULT_ERROR_INT, PROFILE);
	valueCount[5] = GetPrivateProfileInt(section, TEXT("NOWCOUNT"), DEFAULT_ERROR_INT, PROFILE);
	if (valueCount[0] < valueCount[1] || valueCount[0] < valueCount[3] + valueCount[4]
		|| (valueCount[5] < 0 && -valueCount[5] > valueCount[4])
		|| (valueCount[5] > 0 && valueCount[5] > valueCount[3]))
	{
		valueCount[0] = 0;
		valueCount[1] = 0;
		valueCount[3] = 0;
		valueCount[4] = 0;
		valueCount[5] = 0;
		WritePrivateProfileString(section, TEXT("GAMECOUNT"), TEXT("0"), PROFILE);
		WritePrivateProfileString(section, TEXT("WINCOUNT"), TEXT("0"), PROFILE);
		WritePrivateProfileString(section, TEXT("MOSTWIN"), TEXT("0"), PROFILE);
		WritePrivateProfileString(section, TEXT("MOSTLOSE"), TEXT("0"), PROFILE);
		WritePrivateProfileString(section, TEXT("NOWCOUNT"), TEXT("0"), PROFILE);
	}
	if (valueCount[0] == 0)
		valueCount[2] = 0;
	else
		valueCount[2] = (int)((float)valueCount[1] / valueCount[0] * 100);

	static TCHAR* best[] = { TEXT("BESTTIME1"), TEXT("BESTTIME2"), TEXT("BESTTIME3"), TEXT("BESTTIME4"), TEXT("BESTTIME5"), TEXT("DATE1"), TEXT("DATE2"), TEXT("DATE3"), TEXT("DATE4"), TEXT("DATE5") };
	bestTime[0] = 10 * GetPrivateProfileInt(section, TEXT("BESTTIME1"), DEFAULT_ERROR_INT, PROFILE);
	bestTime[1] = 10 * GetPrivateProfileInt(section, TEXT("BESTTIME2"), DEFAULT_ERROR_INT, PROFILE);
	bestTime[2] = 10 * GetPrivateProfileInt(section, TEXT("BESTTIME3"), DEFAULT_ERROR_INT, PROFILE);
	bestTime[3] = 10 * GetPrivateProfileInt(section, TEXT("BESTTIME4"), DEFAULT_ERROR_INT, PROFILE);
	bestTime[4] = 10 * GetPrivateProfileInt(section, TEXT("BESTTIME5"), DEFAULT_ERROR_INT, PROFILE);
	for (int i = 0; i < 5; i++)
	{
		if (bestTime[i] < 0)
			bestTime[i] -= i;
		else
			bestTime[i] += i;
	}

	date[0] = GetPrivateProfileInt(section, TEXT("DATE1"), DEFAULT_ERROR_INT, PROFILE);
	date[1] = GetPrivateProfileInt(section, TEXT("DATE2"), DEFAULT_ERROR_INT, PROFILE);
	date[2] = GetPrivateProfileInt(section, TEXT("DATE3"), DEFAULT_ERROR_INT, PROFILE);
	date[3] = GetPrivateProfileInt(section, TEXT("DATE4"), DEFAULT_ERROR_INT, PROFILE);
	date[4] = GetPrivateProfileInt(section, TEXT("DATE5"), DEFAULT_ERROR_INT, PROFILE);

	if (sort(bestTime, date) > valueCount[1])//
	{
		for (int i = valueCount[1]; i <= 4; i++)
		{
			bestTime[i] = bestTime[i] % 10 + 10 * DEFAULT_ERROR_INT;
			date[i] = DEFAULT_ERROR_INT;
			WritePrivateProfileString(section, best[abs(bestTime[i]) % 10], NULL, PROFILE);
			WritePrivateProfileString(section, best[abs(bestTime[i]) % 10 + 5], NULL, PROFILE);
		}
	}

	return true;
}

/*
写配置文件
	节 section
	值 GAMECOUNT WINCOUNT MOSTWIN MOSTLOSE NOWCOUNT
参数：
	section			节
	valueCount		统计数组
	bestTime		最短时间数组
	date			日期数组
	opera			操作类型
返回值：
	true
*/
bool writeProfileStatistic(TCHAR *section, int *valueCount, int *bestTime, int *date, int opera)
{
	static TCHAR* best[] = { TEXT("BESTTIME1"), TEXT("BESTTIME2"), TEXT("BESTTIME3"), TEXT("BESTTIME4"), TEXT("BESTTIME5"), TEXT("DATE1"), TEXT("DATE2"), TEXT("DATE3"), TEXT("DATE4"), TEXT("DATE5") };
	TCHAR value[12];
	if (opera == 0)//重置
	{
		WritePrivateProfileString(section, TEXT("GAMECOUNT"), TEXT("0"), PROFILE);
		WritePrivateProfileString(section, TEXT("WINCOUNT"), TEXT("0"), PROFILE);
		WritePrivateProfileString(section, TEXT("MOSTWIN"), TEXT("0"), PROFILE);
		WritePrivateProfileString(section, TEXT("MOSTLOSE"), TEXT("0"), PROFILE);
		WritePrivateProfileString(section, TEXT("NOWCOUNT"), TEXT("0"), PROFILE);
		for (int i = 0; i < 5; i++)
		{
			WritePrivateProfileString(section, best[i], NULL, PROFILE);
			WritePrivateProfileString(section, best[i + 5], NULL, PROFILE);
		}
	}
	else if (opera == 1)//加1
	{
		wsprintf(value, TEXT("%d"), valueCount[0] + 1);
		WritePrivateProfileString(section, TEXT("GAMECOUNT"), value, PROFILE);
		wsprintf(value, TEXT("%d"), valueCount[1] + 1);
		WritePrivateProfileString(section, TEXT("WINCOUNT"), value, PROFILE);
		wsprintf(value, TEXT("%d"), (valueCount[5] >= 0) ? (valueCount[5] + 1) : 1);
		WritePrivateProfileString(section, TEXT("NOWCOUNT"), value, PROFILE);
		if (((valueCount[5] >= 0) ? (valueCount[5] + 1) : 1) > valueCount[3])
		{
			wsprintf(value, TEXT("%d"), valueCount[3] + 1);
			WritePrivateProfileString(section, TEXT("MOSTWIN"), value, PROFILE);
		}
		if (timeCount < bestTime[4] / 10 || bestTime[4] / 10 == DEFAULT_ERROR_INT)
		{
			wsprintf(value, TEXT("%d"), timeCount);
			WritePrivateProfileString(section, best[abs(bestTime[4]) % 10], value, PROFILE);
			wsprintf(value, TEXT("%d"), (int)time(NULL));
			WritePrivateProfileString(section, best[abs(bestTime[4]) % 10 + 5], value, PROFILE);
		}
	}
	else if (opera == -1)//减1
	{
		wsprintf(value, TEXT("%d"), valueCount[0] + 1);
		WritePrivateProfileString(section, TEXT("GAMECOUNT"), value, PROFILE);
		wsprintf(value, TEXT("%d"), (valueCount[5] <= 0) ? (valueCount[5] - 1) : -1);
		WritePrivateProfileString(section, TEXT("NOWCOUNT"), value, PROFILE);
		if (-((valueCount[5] <= 0) ? (valueCount[5] - 1) : -1) > valueCount[4])
		{
			wsprintf(value, TEXT("%d"), valueCount[4] + 1);
			WritePrivateProfileString(section, TEXT("MOSTLOSE"), value, PROFILE);
		}
	}
	return true;
}

/*
刷新配置文件
参数：
	level			刷新等级
	opera			操作类型
无返回值
*/
void changeStatistic(int level, int opera)
{
	static TCHAR* section[] = { TEXT("JUNIOR"), TEXT("MEDIATE"), TEXT("SENIOR") };
	int valueCount[6], bestTime[5], date[5];
	readProfileStatistic(section[level], valueCount, bestTime, date);
	writeProfileStatistic(section[level], valueCount, bestTime, date, opera);
}

/*
读配置文件
	节 USERDEFINED
	值 UROW UCOL UCOUNT
参数：
	userDefined		用户定义数组
返回值：
	true
*/
bool readProfileUserdefined(int *userDefined)
{
	TCHAR value[5];

	userDefined[0] = GetPrivateProfileInt(TEXT("USERDEFINED"), TEXT("UROW"), DEFAULT_ERROR_INT, PROFILE);
	userDefined[1] = GetPrivateProfileInt(TEXT("USERDEFINED"), TEXT("UCOL"), DEFAULT_ERROR_INT, PROFILE);
	userDefined[2] = GetPrivateProfileInt(TEXT("USERDEFINED"), TEXT("UCOUNT"), DEFAULT_ERROR_INT, PROFILE);
	if (userDefined[0] < 9 || userDefined[0] > 24 || userDefined[1] < 9 || userDefined[1] > 30 || userDefined[2] < 10 || userDefined[2] > 668)
	{
		userDefined[0] = DEFAULT_ROW;
		userDefined[1] = DEFAULT_COL;
		userDefined[2] = DEFAULT_COUNT;

		wsprintf(value, TEXT("%d"), DEFAULT_ROW);
		WritePrivateProfileString(TEXT("USERDEFINED"), TEXT("UROW"), value, PROFILE);
		wsprintf(value, TEXT("%d"), DEFAULT_COL);
		WritePrivateProfileString(TEXT("USERDEFINED"), TEXT("UCOL"), value, PROFILE);
		wsprintf(value, TEXT("%d"), DEFAULT_COUNT);
		WritePrivateProfileString(TEXT("USERDEFINED"), TEXT("UCOUNT"), value, PROFILE);
	}
	return true;
}

/*
写配置文件
	节 USERDEFINED
	值 UROW UCOL UCOUNT
参数：
	userDefined		用户定义数组
返回值：
	true
*/
bool writeProfileUserdefined(int *userDefined)
{
	TCHAR value[5];
	wsprintf(value, TEXT("%d"), userDefined[0]);
	WritePrivateProfileString(TEXT("USERDEFINED"), TEXT("UROW"), value, PROFILE);
	wsprintf(value, TEXT("%d"), userDefined[1]);
	WritePrivateProfileString(TEXT("USERDEFINED"), TEXT("UCOL"), value, PROFILE);
	wsprintf(value, TEXT("%d"), userDefined[2]);
	WritePrivateProfileString(TEXT("USERDEFINED"), TEXT("UCOUNT"), value, PROFILE);
	return true;
}
