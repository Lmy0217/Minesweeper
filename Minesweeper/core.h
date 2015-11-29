/**
 *	文件：core.h
 *	算法函数集
 *
 *	Made by Luo Mingyuan
 *	Email: lmy0217@126.com
 *	谨此纪念娄芳老师
 *	2015-11-28
 */


#pragma once
#include <math.h>
#include <queue>


/*
获得每个块周围块的雷数
参数：
	mapInfo			雷区块标志数组
	row				雷区行数
	col				雷区列数
无返回值
*/
void getMineCount(unsigned short int *&mapInfo, int row, int col)
{
	unsigned short int *colHisto = new unsigned short int[col];
	unsigned short int coreHisto;
	int pno = 0, padd = 0, psub = 0;

	for (int j = 0; j < col; j++)
		colHisto[j] = 0;

	for (int i = 0; i < row; i++)
	{
		while (padd < (i + 2) * col)
		{
			if (i < row - 1)
				colHisto[padd % col] += ((mapInfo[padd] & 0xF000) == MF_MINE) ? 1 : 0;
			if (i > 1 && (mapInfo[psub++] & 0xF000) == MF_MINE)
				colHisto[padd % col]--;
			padd++;
		}
		coreHisto = 0;
		for (int j = 0; j < col; j++)
		{
			while (pno < j + 2 + i * col)
			{
				if (j < col - 1)
					coreHisto += colHisto[pno % col];
				if (j > 1)
					coreHisto -= colHisto[(pno - 3) % col];
				pno++;
			}
			if ((mapInfo[pno - 2] & 0xF000) != MF_MINE)
				mapInfo[pno - 2] = (mapInfo[pno - 2] & 0x00FF) + coreHisto * 0x1000;
		}
		pno--;
	}
}

/*
随机布雷, 除去初始点到的块及周围的块
参数：
	mapInfo			雷区块标志数组
	seed			随机种子
	firstLButtonNo	初始点到的块序号
	row				雷区行数
	col				雷区列数
	count			雷区雷数
无返回值
*/
void initMineCount(unsigned short int *&mapInfo, unsigned int seed, int firstLButtonNo, int row, int col, int count)
{
	int brickNumber = row * col;
	int n = 0, r;

	srand(seed);//设置随机种子
	//排除点到的块及其周围随机布雷
	while (n < count)
	{
		r = rand() % brickNumber;
		if ((mapInfo[r] & 0xF000) != MF_MINE
			&& r != firstLButtonNo
			&& r != firstLButtonNo - col
			&& r != firstLButtonNo - col + 1
			&& r != firstLButtonNo + 1
			&& r != firstLButtonNo + col + 1
			&& r != firstLButtonNo + col
			&& r != firstLButtonNo + col - 1
			&& r != firstLButtonNo - 1
			&& r != firstLButtonNo - col - 1)
		{
			mapInfo[r] = (mapInfo[r] & 0x00FF) + MF_MINE;
			n++;
		}
	}

	getMineCount(mapInfo, row, col);//计算每个块周围的雷数

}

/*
保存游戏数组计算雷区块标志数组
参数：
	mapInfo			雷区块标志数组
	saveFlags		保存的游戏数组
	brickNumber		雷区块数
无返回值
*/
void save2map(unsigned short int *&mapInfo, TCHAR *&saveFlags, int brickNumber)
{
	for (int i = 0; i < brickNumber; i++)
		switch (TCHAR temp = saveFlags[i])
		{
		case '0':case '3':case '6':
			mapInfo[i] = (mapInfo[i] & 0xF000) + ((mapInfo[i] & 0x00FF) + (temp - '0')) % 9;
			break;
		case '9':
			mapInfo[i] = (mapInfo[i] & 0xF000) + MF_NUMBER(mapInfo[i] / 0x1000);
			break;
		}
}

/*
雷区块标志数组计算保存游戏数组
参数：
	saveFlags		保存的游戏数组
	mapInfo			雷区块标志数组
	brickNumber		雷区块数
无返回值
*/
void map2save(TCHAR *&saveFlags, unsigned short int *&mapInfo, int brickNumber)
{
	for (int i = 0; i < brickNumber; i++)
		switch (mapInfo[i] & 0x0F00)
		{
		case 0x0000:
			saveFlags[i] = '0' + ((mapInfo[i] & 0x00FF) / 3 * 3 + 6) % 9;
			break;
		case 0x0100:
			saveFlags[i] = '9';
			break;
		}
	saveFlags[brickNumber] = '\0';
}

/*
初始化动画效果
参数：
	row				雷区行数
	col				雷区列数
返回值：
	firstPaintNo	初始化动画贴图块序列数组
*/
int* getFirstPaintNo(int row, int col)
{
	int *firstPaintNo = new int[row * col];
	int startNo[4] = { 0,
		col - 1 - col % 2,
		(row - 1 - row % 2) * col,
		(row - row % 2) * col - 1 - col % 2 };
	int max = (row >= col) ? row : col;
	int min = (row >= col) ? col : row;
	int ratio = (int)ceil((float)max / (float)min);
	int n = 0, k;

	for (int i = 0; i < 2 * max; i++)
	{
		for (int j = 0; j < 2 * min; j++)
		{
			if (i / ratio >= j)
			{
				for (k = 0; k < 4; k++)
				{
					if (2 * (i - ratio * j) <= max - 1 - ((max == col) ? (k % 2) : (k / 2))
						&& 2 * j <= min - 1 - ((max == col) ? (k / 2) : (k % 2)))
					{
						firstPaintNo[n++] = startNo[k]
							+ ((max == col) ? ((k % 2) ? -1 : 1) : ((k / 2) ? -1 : 1)) 
							* (i - ratio * j) * 2 * ((max == col) ? 1 : col)
							+ ((max == col) ? ((k / 2) ? -1 : 1) : ((k % 2) ? -1 : 1))
							* j * 2 * ((max == col) ? col : 1);
					}
				}
			}
		}
	}

	return firstPaintNo;
}


/*
双队列函数
	判断条件
参数：
	nextPoint		下一个遍历到的块
	startPoint		初始点到的块
	threshold		判断阈值
	row				雷区行数
	col				雷区列数
返回值：
	是否满足条件
*/
bool require(int nextPoint, int startPoint, float threshold, int row, int col)
{
	int pointX = nextPoint % col;
	int pointY = nextPoint / col;
	int startPointX = startPoint % col;
	int startPointY = startPoint / col;
	return (pointY - startPointY) * (pointY - startPointY)
		+ (pointX - startPointX) * (pointX - startPointX) <= threshold;//距离是否小于半径
}

/*
双队列函数
	判断边界
参数：
	nextType		下一个遍历到的块的类型
	point			正在遍历的块
	row				雷区行数
	col				雷区列数
返回值：
	是否越过边界
*/
bool edge(int nextType, int point, int row, int col)
{
	bool edge[4] = { point >= col, (point % col) < (col - 1),
		point < (row - 1) * col, (point % col) > 0 };
	return (nextType % 2 == 0) ? edge[nextType / 2]
		: (edge[(nextType - 1) / 2] && edge[(nextType + 1) % 8 / 2]);
}

/*
双队列函数
	初始化两个队列
参数：
	startPoint		初始点到的块
	flagid			遍历层次标记
	fast			快队列
	slow			慢队列
	no				遍历块序列数组
	row				雷区行数
	col				雷区列数
无返回值
*/
void dbQueueInit(int startPoint, int flagid, std::queue<int> &fast,
	std::queue<int> &slow, int *&no, int row, int col)
{
	//点到的块及其周围8个块放入慢队列
	for (int k = 0; k < 8; k++)
	{
		int dc = (k % 4 == 0) ? 0 : ((k > 4) ? -1 : 1);
		int dr = (k % 4 == 0) ? (k / 2 - 1) : ((k > 4) ? (2 - k % 4) : (k % 4 - 2));
		int nextPoint = startPoint + dc + dr * col;
		if (edge(k, startPoint, row, col))
		{
			slow.push(nextPoint);
			slow.push(k);
			no[nextPoint + row * col] = flagid;
		}
	}
}

/*
双队列函数
	运行双队列
参数：
	point			正在遍历的块
	type			正在遍历的块的类型
	startPoint		初始点到的块
	threshold		判断阈值
	flagid			遍历层次标记
	fast			块队列
	slow			慢队列
	no				遍历块序列数组
	row				雷区行数
	col				雷区列数
无返回值
*/
void dbQueueRun(int point, int type, int startPoint, float threshold, int flagid,
	std::queue<int> &fast, std::queue<int> &slow, int *&no, int row, int col)
{
	/*int start = 0, end = 5;
	if (point < startPoint && (startPoint - point) % col == 0)
	{
	start = 2;
	end = 3;
	}*/
	for (int k = 0; k < 5; k++)//块下层遍历的5个方向
	{
		int nextType = (type + 6 + k) % 8;//类型, 方向
		int dc = (nextType % 4 == 0) ? 0 : ((nextType > 4) ? -1 : 1);
		int dr = (nextType % 4 == 0) ? (nextType / 2 - 1) : ((nextType > 4) ? (2 - nextType % 4) : (nextType % 4 - 2));
		int nextPoint = point + dc + dr * col;//序号
		if (edge(nextType, point, row, col) && no[nextPoint + row * col] == 0)
		{
			if (require(nextPoint, startPoint, threshold, row, col))//判断条件
			{
				fast.push(nextPoint);
				fast.push(nextType);
			}
			else
			{
				slow.push(nextPoint);
				slow.push(nextType);
			}
			no[nextPoint + row * col] = flagid;
		}
	}
}

/*
双队列函数
	双队列层次遍历, 实现平面圆形扩散
参数：
	startPoint		初始点到的块
	endRequire		遍历类型
	mapInfo			雷区块标志数组
	row				雷区行数
	col				雷区列数
返回值：
	no				遍历块序列数组
*/
int* dbQueue(int startPoint, int endRequire, unsigned short int *&mapInfo, int row, int col)
{
	std::queue<int> fast, slow;
	float r = 0;//初始半径0
	int point, type;
	int i = 0, flagid = 1;

	int *no = new int[2 * row * col];
	for (int k = 0; k < row * col; k++) {
		no[k] = -1;
		no[k + row * col] = 0;
	}

	dbQueueInit(startPoint, flagid, fast, slow, no, row, col);

	while (!slow.empty() || !fast.empty())
	{
		while (!fast.empty())
		{
			point = fast.front();
			fast.pop();
			type = fast.front();
			fast.pop();

			if (endRequire == 0)//遍历类型0：冲开没有雷的块
			{
				no[i++] = point;
				if ((mapInfo[point] & 0xF000) != MF_ZEROMINE)
					continue;
			}
			else if (endRequire == 1)//遍历类型1：雷爆炸
			{
				if ((mapInfo[point] & 0xF000) == MF_MINE)
					no[i++] = point;
			}

			dbQueueRun(point, type, startPoint, r * r, flagid, fast, slow, no, row, col);
		}

		r = r + 0.618f;//半径增长
		if (!slow.empty() && require(slow.front(), startPoint, r * r, row, col))
			flagid++;//遍历层次加1

		while (!slow.empty() && require(slow.front(), startPoint, r * r, row, col))
		{
			point = slow.front();
			slow.pop();
			type = slow.front();
			slow.pop();

			if (endRequire == 0)//遍历类型0：冲开没有雷的块
			{
				no[i++] = point;
				if ((mapInfo[point] & 0xF000) != MF_ZEROMINE)
					continue;
			}
			else if (endRequire == 1)//遍历类型1：雷爆炸
			{
				if ((mapInfo[point] & 0xF000) == MF_MINE)
					no[i++] = point;
			}

			dbQueueRun(point, type, startPoint, r * r, flagid, fast, slow, no, row, col);
		}
	}

	return no;
}


/*
点到雷数为零的块, 获得冲开块的序列
参数：
	startPoint		初始点到的块
	mapInfo			雷区块标志数组
	row				雷区行数
	col				雷区列数
返回值：
	冲开块的序列数组
*/
int* getZeroDevelopNo(int startPoint, unsigned short int *&mapInfo, int row, int col)
{
	return dbQueue(startPoint, 0, mapInfo, row, col);
}

/*
点到雷, 获得雷爆炸贴图的序列
参数：
	startPoint		初始点到的雷
	mapInfo			雷区块标志数组
	bmpNumber		爆炸效果贴图数
	row				雷区行数
	col				雷区列数
	count			雷区雷数
返回值：
	test			雷爆炸贴图的序列数组
*/
int* getMineDevelopNo(int startPoint, unsigned short int *&mapInfo, int bmpNumber, int row, int col, int count)
{
	int *no = dbQueue(startPoint, 1, mapInfo, row, col);

	std::queue<int> run;
	int np = 0, n = 0;//np	同一个遍历层次元素个数
	int t = 0, i = 0, f = 0;//f	遍历层次
	int d, type;//type	爆炸第几张贴图

	int *test = new int[3 * count * bmpNumber];
	for (int k = 0; k < 3 * count * bmpNumber; k++) {
		test[k] = -1;
	}

	//第一个元素进队列
	if (i < row * col && no[i] != -1)
	{
		run.push(no[i]);
		run.push(1);
		f = no[no[i] + row * col];
		np++;//遍历层次元素个数加1
		i++;
	}
	//同一个遍历层次的元素进队列
	while (i < row * col && no[i] != -1 && no[no[i] + row * col] == f)
	{
		run.push(no[i]);
		run.push(1);
		np++;//遍历层次元素个数加1
		i++;
	}
	if (i < row * col && no[i] != -1)
	{
		f = no[no[i] + row * col];
	}

	while (!run.empty())
	{
		d = run.front();
		run.pop();
		type = run.front();
		run.pop();

		test[t] = d;//位置序号
		test[t + count * bmpNumber] = type;//爆炸第几张贴图
		test[t + 2 * count * bmpNumber] = run.size();//队列元素个数, 用来控制爆炸效果延时
		t++;

		if (type < bmpNumber)
		{
			run.push(d);
			run.push(type + 1);
		}

		if (type == 1)//第一张贴图
		{
			np--;//这个遍历层次元素个数减1
			if (i < row * col && no[i] != -1 && no[no[i] + row * col] == f)//下一个遍历层次的一个元素进队列
			{
				run.push(no[i]);
				run.push(1);
				n++;
				i++;
			}
			if (np == 0)//遍历层次元素个数为0
			{
				while (i < row * col && no[i] != -1 && no[no[i] + row * col] == f)//下一个遍历层次剩下的元素进队列
				{
					run.push(no[i]);
					run.push(1);
					n++;
					i++;
				}
				if (i < row * col && no[i] != -1)
				{
					f = no[no[i] + row * col];
				}
				np = n;
				n = 0;
			}
		}
	}

	return test;
}


/*
双键按下时, 获得周围红旗数
参数：
	point			双键按下的块
	mapInfo			雷区块标志数组
	row				雷区行数
	col				雷区列数
返回值：
	flagNumber		周围红旗数
*/
int dbButtonClickFlag(int point, unsigned short int *&mapInfo, int row, int col)
{
	int no, flagNumber = 0;
	for (int i = -1; i <= 1; i++)
	{
		if (point % col + i >= 0 && point % col + i < col)
		{
			for (int j = -1; j <= 1; j++)
			{
				if (point / col + j >= 0 && point / col + j < row)
				{
					no = point + i + j * col;
					if (no >= 0 && no < row * col && (mapInfo[no] & 0x0F00) == 0x0000 && (mapInfo[no] & 0x00FF) / 3 == 2)
					{
						flagNumber++;
					}
				}
			}
		}
	}
	return flagNumber;
}

/*
双键按下时, 获得周围雷数
参数：
	point			双键按下的块
	mapInfo			雷区块标志数组
	row				雷区行数
	col				雷区列数
返回值：
	若周围存在某个块是雷且不是红旗, 则返回这个块的序号, 否则返回0
*/
int dbButtonClickMine(int point, unsigned short int *&mapInfo, int row, int col)
{
	int no;
	for (int i = -1; i <= 1; i++)
	{
		if (point % col + i >= 0 && point % col + i < col)
		{
			for (int j = -1; j <= 1; j++)
			{
				if (point / col + j >= 0 && point / col + j < row)
				{
					no = point + i + j * col;
					if (no >= 0 && no < row * col && (mapInfo[no] & 0xF000) == MF_MINE && (mapInfo[no] & 0x00FF) / 3 != 2)
					{
						return i + j * col;
					}
				}
			}
		}
	}
	return 0;
}


/*
char转换为tchar
参数：
	_char			需要转换的char数组
	tchar			转换输出的tchar数组
*/
void CharToTchar(const char *_char, TCHAR *tchar)
{
	int iLength = MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, tchar, iLength);
}
