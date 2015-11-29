/**
 *	文件：Minesweeper.cpp
 *	Minesweeper源文件
 *
 *	Made by Luo Mingyuan
 *	Email: lmy0217@126.com
 *	谨此纪念娄芳老师
 *	2015-11-28
 */


#include "stdafx.h"
#include "Minesweeper.h"
#include "core.h"
#include "io.h"
#include <time.h>
#include <MMSystem.h>

#pragma comment(lib, "WINMM.LIB")

#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

// 此代码模块中包含的函数的前向声明: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Option(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Statistic(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Win(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Lose(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO:  在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MINESWEEPER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MINESWEEPER));

	// 主消息循环: 
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}

//
//  函数:  MyRegisterClass()
//
//  目的:  注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MINESWEEPER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MINESWEEPER);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数:  InitInstance(HINSTANCE, int)
//
//   目的:  保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


//设置对话框窗口过程函数
INT_PTR CALLBACK Option(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	static int tRow, tCol, tCount, tFlag;
	static int oldUserDefined[3], userDefined[3];//自定义值
	static bool user = false;//是否选择自定义
	static bool flag = true;//某个编辑框是否可写
	HWND hStatic;

	switch (message)
	{
	case WM_INITDIALOG:

		//初始化多选按钮
		if (optionFlag & OF_DRAW)
			CheckRadioButton(hDlg, IDC_CHECK1, IDC_CHECK1, IDC_CHECK1);
		if (optionFlag & OF_SOUND)
			CheckRadioButton(hDlg, IDC_CHECK2, IDC_CHECK2, IDC_CHECK2);
		if (optionFlag & OF_PROM)
			CheckRadioButton(hDlg, IDC_CHECK3, IDC_CHECK3, IDC_CHECK3);
		if (optionFlag & OF_STARTATSAVE)
			CheckRadioButton(hDlg, IDC_CHECK4, IDC_CHECK4, IDC_CHECK4);
		if (optionFlag & OF_SAVE)
			CheckRadioButton(hDlg, IDC_CHECK5, IDC_CHECK5, IDC_CHECK5);
		if (optionFlag & OF_UNKNOW)
			CheckRadioButton(hDlg, IDC_CHECK6, IDC_CHECK6, IDC_CHECK6);

		//初始化难度单选按钮
		if ((optionFlag & 0x03) == OF_USERDEFINED)
		{
			user = true;
			CheckRadioButton(hDlg, IDC_USERDEFINED, IDC_USERDEFINED, IDC_USERDEFINED);
		}
		else if ((optionFlag & 0x03) == OF_SENIOR)
			CheckRadioButton(hDlg, IDC_SENIOR, IDC_SENIOR, IDC_SENIOR);
		else if ((optionFlag & 0x03) == OF_MEDIATE)
			CheckRadioButton(hDlg, IDC_MEDIATE, IDC_MEDIATE, IDC_MEDIATE);
		else if ((optionFlag & 0x03) == OF_JUNIOR)
			CheckRadioButton(hDlg, IDC_JUNIOR, IDC_JUNIOR, IDC_JUNIOR);

		//非自定义禁用自定义区域
		if (!user)
		{
			hStatic = GetDlgItem(hDlg, IDC_STATIC_HEIGHT);
			EnableWindow(hStatic, 0);
			hStatic = GetDlgItem(hDlg, IDC_EDIT_HEIGHT);
			EnableWindow(hStatic, 0);
			hStatic = GetDlgItem(hDlg, IDC_STATIC_WIDTH);
			EnableWindow(hStatic, 0);
			hStatic = GetDlgItem(hDlg, IDC_EDIT_WIDTH);
			EnableWindow(hStatic, 0);
			hStatic = GetDlgItem(hDlg, IDC_STATIC_COUNT);
			EnableWindow(hStatic, 0);
			hStatic = GetDlgItem(hDlg, IDC_EDIT_COUNT);
			EnableWindow(hStatic, 0);
		}
		//读取自定义值并显示
		readProfileUserdefined(oldUserDefined);
		userDefined[0] = oldUserDefined[0];
		userDefined[1] = oldUserDefined[1];
		userDefined[2] = oldUserDefined[2];
		flag = false;
		SetDlgItemInt(hDlg, IDC_EDIT_HEIGHT, oldUserDefined[0], TRUE);
		flag = true;
		flag = false;
		SetDlgItemInt(hDlg, IDC_EDIT_WIDTH, oldUserDefined[1], TRUE);
		flag = true;
		flag = false;
		SetDlgItemInt(hDlg, IDC_EDIT_COUNT, oldUserDefined[2], TRUE);
		flag = true;

		//保存初始值
		tRow = row;
		tCol = col;
		tCount = count;
		tFlag = optionFlag;
		break;
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			if (user)//自定义
			{
				//读取输入的自定义值
				tRow = GetDlgItemInt(hDlg, IDC_EDIT_HEIGHT, FALSE, TRUE);
				userDefined[0] = tRow;
				tCol = GetDlgItemInt(hDlg, IDC_EDIT_WIDTH, FALSE, TRUE);
				userDefined[1] = tCol;
				tCount = GetDlgItemInt(hDlg, IDC_EDIT_COUNT, FALSE, TRUE);
				userDefined[2] = tCount;
				//正确性检查
				if (tRow < 9 || tRow > 24 || tCol < 9 || tCol > 30 || tCount < 10 || tCount > 668)
				{
					if (tRow < 9 || tRow > 24)
					{
						tRow = 9;
						userDefined[0] = tRow;
						SetDlgItemInt(hDlg, IDC_EDIT_HEIGHT, tRow, TRUE);
					}
					if (tCol < 9 || tCol > 30)
					{
						tCol = 9;
						userDefined[1] = tCol;
						SetDlgItemInt(hDlg, IDC_EDIT_WIDTH, tCol, TRUE);
					}
					if (tCount < 10 || tCount > 668)
					{
						tCount = 10;
						userDefined[2] = tCount;
						SetDlgItemInt(hDlg, IDC_EDIT_COUNT, tCount, TRUE);
					}
					break;
				}
				//若是某些值则设置标志为相应等级
				if (tRow == 9 && tCol == 9 && tCount == 10)
				{
					tFlag = (tFlag & 0xFC) + OF_JUNIOR;
					user = false;
				}
				else if (tRow == 16 && tCol == 16 && tCount == 40)
				{
					tFlag = (tFlag & 0xFC) + OF_MEDIATE;
					user = false;
				}
				else if (tRow == 16 && tCol == 30 && tCount == 99)
				{
					tFlag = (tFlag & 0xFC) + OF_SENIOR;
					user = false;
				}
			}
			//自定义值与初始自定义值不相同则写入配置文件
			if (userDefined[0] != oldUserDefined[0] || userDefined[1] != oldUserDefined[1] || userDefined[2] != oldUserDefined[2])
				writeProfileUserdefined(userDefined);
			
			//行列雷数与初始不同
			if (row != tRow || col != tCol || count != tCount)
			{
				if (firstLButtonDown)
				{
					//弹出消息框
					switch (MessageBox(hDlg, TEXT("这些设置不会应用于进行中的游戏!您希望做什么?\n\n退出并以新设置开始新游戏(Y)\n这在统计时记为输!\n\n结束游戏(N)\n新设置会应用于下一个游戏!"), TEXT("已更改游戏设置"), MB_YESNOCANCEL))
					{
					case IDYES://以新设置开始新游戏
						row = tRow;
						col = tCol;
						count = tCount;
						if ((optionFlag & 0x03) < 3)
							changeStatistic((optionFlag & 0x03), -1);//记为输
						optionFlag = tFlag;
						EndDialog(hDlg, 1);//返回1
						break;
					case IDNO://新设置应用于下一个游戏
						tempOption[0] = tRow;
						tempOption[1] = tCol;
						tempOption[2] = tCount;
						tempOption[3] = tFlag;
						optionFlag = (tFlag & 0xFC) + (optionFlag & 0x03);
						EndDialog(hDlg, 2);//返回2
						break;
					}
				}
				else//未开始游戏直接应用新设置
				{
					row = tRow;
					col = tCol;
					count = tCount;
					optionFlag = tFlag;
					EndDialog(hDlg, 1);//返回1
				}
			}
			else
			{
				optionFlag = tFlag;
				EndDialog(hDlg, 0);
			}
			return (INT_PTR)TRUE;
			break;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			return (INT_PTR)TRUE;
			break;
		case IDC_JUNIOR:
			user = false;
			//禁用自定义区域
			hStatic = GetDlgItem(hDlg, IDC_STATIC_HEIGHT);
			EnableWindow(hStatic, 0);
			hStatic = GetDlgItem(hDlg, IDC_EDIT_HEIGHT);
			EnableWindow(hStatic, 0);
			hStatic = GetDlgItem(hDlg, IDC_STATIC_WIDTH);
			EnableWindow(hStatic, 0);
			hStatic = GetDlgItem(hDlg, IDC_EDIT_WIDTH);
			EnableWindow(hStatic, 0);
			hStatic = GetDlgItem(hDlg, IDC_STATIC_COUNT);
			EnableWindow(hStatic, 0);
			hStatic = GetDlgItem(hDlg, IDC_EDIT_COUNT);
			EnableWindow(hStatic, 0);
			//设置标志为相应等级
			tFlag = (tFlag & 0xFC) + OF_JUNIOR;
			tRow = 9;
			tCol = 9;
			tCount = 10;
			break;
		case IDC_MEDIATE:
			user = false;
			//禁用自定义区域
			hStatic = GetDlgItem(hDlg, IDC_STATIC_HEIGHT);
			EnableWindow(hStatic, 0);
			hStatic = GetDlgItem(hDlg, IDC_EDIT_HEIGHT);
			EnableWindow(hStatic, 0);
			hStatic = GetDlgItem(hDlg, IDC_STATIC_WIDTH);
			EnableWindow(hStatic, 0);
			hStatic = GetDlgItem(hDlg, IDC_EDIT_WIDTH);
			EnableWindow(hStatic, 0);
			hStatic = GetDlgItem(hDlg, IDC_STATIC_COUNT);
			EnableWindow(hStatic, 0);
			hStatic = GetDlgItem(hDlg, IDC_EDIT_COUNT);
			EnableWindow(hStatic, 0);
			//设置标志为相应等级
			tFlag = (tFlag & 0xFC) + OF_MEDIATE;
			tRow = 16;
			tCol = 16;
			tCount = 40;
			break;
		case IDC_SENIOR:
			user = false;
			//禁用自定义区域
			hStatic = GetDlgItem(hDlg, IDC_STATIC_HEIGHT);
			EnableWindow(hStatic, 0);
			hStatic = GetDlgItem(hDlg, IDC_EDIT_HEIGHT);
			EnableWindow(hStatic, 0);
			hStatic = GetDlgItem(hDlg, IDC_STATIC_WIDTH);
			EnableWindow(hStatic, 0);
			hStatic = GetDlgItem(hDlg, IDC_EDIT_WIDTH);
			EnableWindow(hStatic, 0);
			hStatic = GetDlgItem(hDlg, IDC_STATIC_COUNT);
			EnableWindow(hStatic, 0);
			hStatic = GetDlgItem(hDlg, IDC_EDIT_COUNT);
			EnableWindow(hStatic, 0);
			//设置标志为相应等级
			tFlag = (tFlag & 0xFC) + OF_SENIOR;
			tRow = 16;
			tCol = 30;
			tCount = 99;
			break;
		case IDC_USERDEFINED:
			user = true;
			//启用自定义区域
			hStatic = GetDlgItem(hDlg, IDC_STATIC_HEIGHT);
			EnableWindow(hStatic, 1);
			hStatic = GetDlgItem(hDlg, IDC_EDIT_HEIGHT);
			EnableWindow(hStatic, 1);
			hStatic = GetDlgItem(hDlg, IDC_STATIC_WIDTH);
			EnableWindow(hStatic, 1);
			hStatic = GetDlgItem(hDlg, IDC_EDIT_WIDTH);
			EnableWindow(hStatic, 1);
			hStatic = GetDlgItem(hDlg, IDC_STATIC_COUNT);
			EnableWindow(hStatic, 1);
			hStatic = GetDlgItem(hDlg, IDC_EDIT_COUNT);
			EnableWindow(hStatic, 1);
			//设置标志为自定义
			tFlag = (tFlag & 0xFC) + OF_USERDEFINED;
			break;
		case IDC_CHECK1:
			tFlag ^= OF_DRAW;
			break;
		case IDC_CHECK2:
			tFlag ^= OF_SOUND;
			break;
		case IDC_CHECK3:
			tFlag ^= OF_PROM;
			break;
		case IDC_CHECK4:
			tFlag ^= OF_STARTATSAVE;
			break;
		case IDC_CHECK5:
			tFlag ^= OF_SAVE;
			break;
		case IDC_CHECK6:
			tFlag ^= OF_UNKNOW;
			break;
		case IDC_EDIT_HEIGHT:
			if (flag && user)
			{
				tRow = GetDlgItemInt(hDlg, IDC_EDIT_HEIGHT, FALSE, TRUE);
				userDefined[0] = tRow;
				//检查另外两个自定义值的正确性
				if (tCol < 9 || tCol > 30)
				{
					tCol = 9;
					userDefined[1] = tCol;
					flag = false;
					SetDlgItemInt(hDlg, IDC_EDIT_WIDTH, tCol, TRUE);
					flag = true;
				}
				if (tCount < 10 || tCount > 668)
				{
					tCount = 10;
					userDefined[2] = tCount;
					flag = false;
					SetDlgItemInt(hDlg, IDC_EDIT_COUNT, tCount, TRUE);
					flag = true;
				}
			}
			break;
		case IDC_EDIT_WIDTH:
			if (flag && user)
			{
				tCol = GetDlgItemInt(hDlg, IDC_EDIT_WIDTH, FALSE, TRUE);
				userDefined[1] = tCol;
				//检查另外两个自定义值的正确性
				if (tRow < 9 || tRow > 24)
				{
					tRow = 9;
					userDefined[0] = tRow;
					flag = false;
					SetDlgItemInt(hDlg, IDC_EDIT_HEIGHT, tRow, TRUE);
					flag = true;
				}
				if (tCount < 10 || tCount > 668)
				{
					tCount = 10;
					userDefined[2] = tCount;
					flag = false;
					SetDlgItemInt(hDlg, IDC_EDIT_COUNT, tCount, TRUE);
					flag = true;
				}
			}
			break;
		case IDC_EDIT_COUNT:
			if (flag && user)
			{
				tCount = GetDlgItemInt(hDlg, IDC_EDIT_COUNT, FALSE, TRUE);
				userDefined[2] = tCount;
				//检查另外两个自定义值的正确性
				if (tRow < 9 || tRow > 24)
				{
					tRow = 9;
					userDefined[0] = tRow;
					flag = false;
					SetDlgItemInt(hDlg, IDC_EDIT_HEIGHT, tRow, TRUE);
					flag = true;
				}
				if (tCol < 9 || tCol > 30)
				{
					tCol = 9;
					userDefined[1] = tCol;
					flag = false;
					SetDlgItemInt(hDlg, IDC_EDIT_WIDTH, tCol, TRUE);
					flag = true;
				}
			}
			break;
		}
		return (INT_PTR)TRUE;
		break;
	}
	return (INT_PTR)FALSE;
}

//统计信息对话框窗口过程函数
INT_PTR CALLBACK Statistic(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static TCHAR* section[] = { TEXT("JUNIOR"), TEXT("MEDIATE"), TEXT("SENIOR") };
	int valueCount[6], bestTime[5], date[5];//统计信息数组 gameCount, winCount, winRate, mostWin, mostLost, nowCount
	static int level;//等级
	static HWND hList;//列表句柄
	TCHAR *pLevel[3] = { TEXT("初级"), TEXT("中级"), TEXT("高级") };

	//用于计算存放最短时间
	tm pTime;
	time_t pTime_t;
	char timeArray[30];
	TCHAR value[30];
	TCHAR best[42];

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:	
		level = (optionFlag & 0x03) % 3;
		
		//初始化列表
		hList = GetDlgItem(hDlg, IDC_LIST);
		SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)pLevel[0]);
		SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)pLevel[1]);
		SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)pLevel[2]);
		SendMessage(hList, LB_SETCURSEL, level, 0);

		//初始化统计信息
		readProfileStatistic(section[level], valueCount, bestTime, date);
		wsprintf(value, TEXT("%d%%"), valueCount[2]);
		SetDlgItemInt(hDlg, IDC_STATIC1, valueCount[0], TRUE);
		SetDlgItemInt(hDlg, IDC_STATIC2, valueCount[1], TRUE);
		SetDlgItemText(hDlg, IDC_STATIC3, value);
		SetDlgItemInt(hDlg, IDC_STATIC4, valueCount[3], TRUE);
		SetDlgItemInt(hDlg, IDC_STATIC5, valueCount[4], TRUE);
		SetDlgItemInt(hDlg, IDC_STATIC6, valueCount[5], TRUE);

		//初始化最短时间
		for (int k = 0; bestTime[k] > 0; k++)
		{
			pTime_t = (time_t)date[k];
			localtime_s(&pTime, &pTime_t);
			asctime_s(timeArray, &pTime);

			ZeroMemory(best, 42);
			wsprintf(best, TEXT("%d\t"), bestTime[k] / 10);
			CharToTchar(timeArray, value);
			_tcscat_s(best, value);

			SetDlgItemText(hDlg, IDC_TIME1 + k, best);
		}
		return (INT_PTR)TRUE;
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
			break;
		case IDC_RETURN:
			//重置统计信息
			writeProfileStatistic(section[level], valueCount, bestTime, date, 0);

			//设置统计信息为0
			SetDlgItemInt(hDlg, IDC_STATIC1, 0, TRUE);
			SetDlgItemInt(hDlg, IDC_STATIC2, 0, TRUE);
			SetDlgItemText(hDlg, IDC_STATIC3, TEXT("0%"));
			SetDlgItemInt(hDlg, IDC_STATIC4, 0, TRUE);
			SetDlgItemInt(hDlg, IDC_STATIC5, 0, TRUE);
			SetDlgItemInt(hDlg, IDC_STATIC6, 0, TRUE);
			for (int i = 0; i < 5; i++)
				SetDlgItemText(hDlg, IDC_TIME1 + i, TEXT(""));
			return (INT_PTR)TRUE;
			break;
		case IDC_LIST:
			level = SendMessage(hList, LB_GETCURSEL, 0, 0);

			//读取并显示统计信息
			readProfileStatistic(section[level], valueCount, bestTime, date);
			wsprintf(value, TEXT("%d%%"), valueCount[2]);
			SetDlgItemInt(hDlg, IDC_STATIC1, valueCount[0], TRUE);
			SetDlgItemInt(hDlg, IDC_STATIC2, valueCount[1], TRUE);
			SetDlgItemText(hDlg, IDC_STATIC3, value);
			SetDlgItemInt(hDlg, IDC_STATIC4, valueCount[3], TRUE);
			SetDlgItemInt(hDlg, IDC_STATIC5, valueCount[4], TRUE);
			SetDlgItemInt(hDlg, IDC_STATIC6, valueCount[5], TRUE);

			for (int k = 0; k < 5; k++)
				SetDlgItemText(hDlg, IDC_TIME1 + k, TEXT(""));
			for (int k = 0; bestTime[k] > 0 && k < 5; k++)
			{
				pTime_t = (time_t)date[k];
				localtime_s(&pTime, &pTime_t);
				asctime_s(timeArray, &pTime);

				ZeroMemory(best, 42);
				wsprintf(best, TEXT("%d\t"), bestTime[k] / 10);
				CharToTchar(timeArray, value);
				_tcscat_s(best, value);

				SetDlgItemText(hDlg, IDC_TIME1 + k, best);
			}
			return (INT_PTR)TRUE;
			break;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

//赢得游戏对话框窗口过程函数
INT_PTR CALLBACK Win(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static TCHAR* Static[] = { TEXT("您所用的时间是这个难度级别的最短时间!"), TEXT("时间："), TEXT("最佳时间："), TEXT("秒"), TEXT("日期："), TEXT("已玩游戏："), TEXT("已胜游戏："), TEXT("百分比：") };
	static TCHAR* section[] = { TEXT("JUNIOR"), TEXT("MEDIATE"), TEXT("SENIOR") };
	static int level;//等级
	int valueCount[6], bestTime[5], date[5];//统计信息数组

	//用于计算存放最短时间
	TCHAR value[30], temp[30];
	tm pTime;
	time_t pTime_t;
	char timeArray[30];

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		if ((level = optionFlag & 0x03) < 3)//不是自定义
		{
			//读取统计信息
			readProfileStatistic(section[level], valueCount, bestTime, date);
			
			//若是最短时间显示"您所用的时间是这个难度级别的最短时间!"
			if (timeCount == bestTime[0] / 10)
				SetDlgItemText(hDlg, IDC_STATIC0, Static[0]);

			//更新并显示各个统计信息
			ZeroMemory(temp, 30);
			_tcscat_s(temp, Static[1]);
			wsprintf(value, TEXT("%d"), timeCount);
			_tcscat_s(temp, value);
			_tcscat_s(temp, Static[3]);
			SetDlgItemText(hDlg, IDC_STATIC1, temp);

			ZeroMemory(temp, 30);
			_tcscat_s(temp, Static[2]);
			wsprintf(value, TEXT("%d"), bestTime[0] / 10);
			_tcscat_s(temp, value);
			_tcscat_s(temp, Static[3]);
			SetDlgItemText(hDlg, IDC_STATIC2, temp);

			ZeroMemory(temp, 30);
			_tcscat_s(temp, Static[4]);
			pTime_t = (time_t)date[0];
			localtime_s(&pTime, &pTime_t);
			asctime_s(timeArray, &pTime);
			CharToTchar(timeArray, value);
			_tcscat_s(temp, value);
			SetDlgItemText(hDlg, IDC_STATIC3, temp);

			ZeroMemory(temp, 30);
			_tcscat_s(temp, Static[5]);
			wsprintf(value, TEXT("%d"), valueCount[0]);
			_tcscat_s(temp, value);
			SetDlgItemText(hDlg, IDC_STATIC4, temp);

			ZeroMemory(temp, 30);
			_tcscat_s(temp, Static[6]);
			wsprintf(value, TEXT("%d"), valueCount[1]);
			_tcscat_s(temp, value);
			SetDlgItemText(hDlg, IDC_STATIC5, temp);

			ZeroMemory(temp, 30);
			_tcscat_s(temp, Static[7]);
			wsprintf(value, TEXT("%d%%"), valueCount[2]);
			_tcscat_s(temp, value);
			SetDlgItemText(hDlg, IDC_STATIC6, temp);

		}

		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:case IDC_NEWGAME://再来一局
			EndDialog(hDlg, 2);//返回2
			return (INT_PTR)TRUE;
			break;
		case IDC_EXIT://退出
			EndDialog(hDlg, 0);//返回0
			return (INT_PTR)TRUE;
			break;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

//游戏失败对话框窗口过程函数
INT_PTR CALLBACK Lose(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static TCHAR* Static[] = { TEXT("时间："), TEXT("秒"), TEXT("已玩游戏："), TEXT("已胜游戏："), TEXT("百分比：") };
	static TCHAR* section[] = { TEXT("JUNIOR"), TEXT("MEDIATE"), TEXT("SENIOR") };
	static int level;//等级
	int valueCount[6], bestTime[5], date[5];//统计信息数组
	TCHAR value[30], temp[30];

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		if ((level = optionFlag & 0x03) < 3)//不是自定义
		{
			//读取统计信息
			readProfileStatistic(section[level], valueCount, bestTime, date);

			//更新并显示各个统计信息
			ZeroMemory(temp, 30);
			_tcscat_s(temp, Static[0]);
			wsprintf(value, TEXT("%d"), timeCount);
			_tcscat_s(temp, value);
			_tcscat_s(temp, Static[1]);
			SetDlgItemText(hDlg, IDC_STATIC1, temp);

			ZeroMemory(temp, 30);
			_tcscat_s(temp, Static[2]);
			wsprintf(value, TEXT("%d"), valueCount[0]);
			_tcscat_s(temp, value);
			SetDlgItemText(hDlg, IDC_STATIC2, temp);

			ZeroMemory(temp, 30);
			_tcscat_s(temp, Static[3]);
			wsprintf(value, TEXT("%d"), valueCount[1]);
			_tcscat_s(temp, value);
			SetDlgItemText(hDlg, IDC_STATIC3, temp);

			ZeroMemory(temp, 30);
			_tcscat_s(temp, Static[4]);
			wsprintf(value, TEXT("%d%%"), valueCount[2]);
			_tcscat_s(temp, value);
			SetDlgItemText(hDlg, IDC_STATIC4, temp);
		}
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:case IDC_NEWGAME://再来一局
			EndDialog(hDlg, 2);//返回2
			return (INT_PTR)TRUE;
			break;
		case IDC_RETURN://重新开始这局游戏
			EndDialog(hDlg, 1);//返回1
			return (INT_PTR)TRUE;
			break;
		case IDC_EXIT://退出
			EndDialog(hDlg, 0);//返回0
			return (INT_PTR)TRUE;
			break;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

//关于对话框窗口过程函数
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


//
//  函数:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;			//贴图目标句柄
	HDC hdcMem;			//贴图源句柄, 存有Minesweeper图
	HDC hdcMem_Frame;	//贴图源句柄, 存有Frame图

	int m, n;
	TCHAR value[11];


	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择: 
		switch (wmId)
		{
		case ID_NEWGAME://新游戏
			hdc = GetDC(hWnd);
			hdcMem = CreateCompatibleDC(hdc);
			SelectObject(hdcMem, hBitmap);

			if (wmEvent == 0 || wmEvent == 1)
			{
				if (firstLButtonDown)
				{
					switch (MessageBox(hWnd, TEXT("游戏正在进行 , 您希望做什么?\n\n退出并开始新游戏(Y)\n本局将作为失败记入统计信息!\n\n重新开始这个游戏(N)\n本局将作为失败记入统计信息!"), TEXT("新游戏"), MB_YESNOCANCEL))
					{
					case IDYES:
						if ((optionFlag & 0x03) < 3)
							changeStatistic((optionFlag & 0x03), -1);
						wmEvent = 3;
						break;
					case IDNO:
						if ((optionFlag & 0x03) < 3)
							changeStatistic((optionFlag & 0x03), -1);
						wmEvent = 2;
						break;
					case IDCANCEL:
						break;
					}
				}
				else
					wmEvent = 3;
			}

			if (wmEvent == 2)//重新开始这个游戏
			{
				startAtSave = false;
				firstPaint = false;
				firstLButtonDown = true;
				timeFlag = true;
				InitInterface(row, col, count, 1, hWnd, hdc, hdcMem);
			}
			else if (wmEvent == 3)//新设置开始新游戏
			{
				startAtSave = false;
				firstPaint = false;
				firstLButtonDown = false;
				if (tempOption[0] >= 9 && tempOption[0] <= 24 && tempOption[1] >= 9 && tempOption[1] <= 30 && tempOption[2] >= 10 && tempOption[2] <= 668 && tempOption[3] >= 0 && tempOption[3] <= 0xFF)
				{
					row = tempOption[0];
					col = tempOption[1];
					count = tempOption[2];
					optionFlag = tempOption[3];
					tempOption[0] = -1;
					tempOption[1] = -1;
					tempOption[2] = -1;
					tempOption[3] = -1;
					//更新窗口大小
					MoveWindow(hWnd, wndX, wndY, wndScale * col + gap.left + gap.right + 2 * DEFAULT_XFRAME, wndScale * row + gap.top + gap.bottom + 2 * DEFAULT_YFRAME + DEFAULT_YTOP, TRUE);
					InvalidateRect(hWnd, NULL, TRUE);
				}
				else
					InitInterface(row, col, count, 0, hWnd, hdc, hdcMem);
			}

			DeleteDC(hdcMem);
			ReleaseDC(hWnd, hdc);
			break;
		case ID_STATISTICS://统计信息
			DialogBox(hInst, MAKEINTRESOURCE(IDD_STATISTIC), hWnd, Statistic);
			break;
		case ID_OPTION://设置
			hdc = GetDC(hWnd);
			hdcMem = CreateCompatibleDC(hdc);
			SelectObject(hdcMem, hBitmap);

			switch (DialogBox(hInst, MAKEINTRESOURCE(IDD_OPTION), hWnd, Option))
			{
			case 1://新设置开始新游戏
				startAtSave = false;
				firstPaint = false;
				firstLButtonDown = false;
				MoveWindow(hWnd, wndX, wndY, wndScale * col + gap.left + gap.right + 2 * DEFAULT_XFRAME, wndScale * row + gap.top + gap.bottom + 2 * DEFAULT_YFRAME + DEFAULT_YTOP, TRUE);
				InvalidateRect(hWnd, NULL, TRUE);
				break;
			}
			writeProfileBasic();

			DeleteDC(hdcMem);
			ReleaseDC(hWnd, hdc);
			break;
		case IDM_ABOUT: //关于
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT: //退出
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;


	case WM_CREATE:
		//初始化全局变量
		saveFlags = NULL;
		mapInfo = NULL;
		hasSave = false;
		startAtSave = false;
		firstPaint = false;
		firstLButtonDown = false;
		hasSpeed = false;
		tempOption[0] = -1;
		tempOption[1] = -1;
		tempOption[2] = -1;
		tempOption[3] = -1;

		//读取配置文件
		readProfileBasic();
		readProfileSave();

		//移动主窗口
		MoveWindow(hWnd, wndX, wndY, wndWidth, wndHeight, TRUE);

		//初始化贴图源句柄
		hBitmap_Frame = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_FRAME));
		GetObject(hBitmap_Frame, sizeof(BITMAP), &bitmap_Frame);
		hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MINESWEEPER));
		GetObject(hBitmap, sizeof(BITMAP), &bitmap);

		//lpvBits数组填充Minesweeper图rgb值
		lpvBits = new char[bitmap.bmWidthBytes * bitmap.bmHeight];
		GetBitmapBits(hBitmap, bitmap.bmWidthBytes * bitmap.bmHeight, lpvBits);

		//初始化块贴图rgb值数组
		pBrick = new unsigned int[INITIAL_BMWDITH * INITIAL_BMHEIGHT];
		//初始化块贴图句柄
		hBitmapT = CreateBitmap(INITIAL_BMWDITH, INITIAL_BMHEIGHT, 1, 32, pBrick);
		break;


	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		hdcMem_Frame = CreateCompatibleDC(hdc);
		SelectObject(hdcMem_Frame, hBitmap_Frame);

		//雷区外四角贴图
		StretchBlt(hdc, 0, 0, gap.left, gap.top, hdcMem_Frame, 0, 0, DEFAULT_GAP_LEFT, DEFAULT_GAP_TOP, SRCCOPY);
		StretchBlt(hdc, gap.left + wndScale * col, 0, gap.right, gap.top, hdcMem_Frame, 571, 0, DEFAULT_GAP_RIGHT, DEFAULT_GAP_TOP, SRCCOPY);
		StretchBlt(hdc, 0, gap.top + wndScale * row, gap.left, gap.bottom, hdcMem_Frame, 0, 319, DEFAULT_GAP_LEFT, DEFAULT_GAP_BOTTOM, SRCCOPY);
		StretchBlt(hdc, gap.left + wndScale * col, gap.top + wndScale * row, gap.right, gap.bottom, hdcMem_Frame, 571, 319, DEFAULT_GAP_RIGHT, DEFAULT_GAP_BOTTOM, SRCCOPY);

		//雷区外上下左右贴图
		StretchBlt(hdc, gap.left, 0, wndScale * col, gap.top, hdcMem_Frame, DEFAULT_GAP_LEFT, 0, 571 - DEFAULT_GAP_LEFT, DEFAULT_GAP_TOP, SRCCOPY);
		StretchBlt(hdc, 0, gap.top, gap.left, wndScale * row, hdcMem_Frame, 0, DEFAULT_GAP_TOP, DEFAULT_GAP_LEFT, 319 - DEFAULT_GAP_TOP, SRCCOPY);
		StretchBlt(hdc, gap.left + wndScale * col, gap.top, gap.right, wndScale * row, hdcMem_Frame, 571, DEFAULT_GAP_TOP, DEFAULT_GAP_RIGHT, 319 - DEFAULT_GAP_TOP, SRCCOPY);
		StretchBlt(hdc, gap.left, gap.top + wndScale * row, wndScale * col, gap.bottom, hdcMem_Frame, DEFAULT_GAP_LEFT, 319, 571 - DEFAULT_GAP_LEFT, DEFAULT_GAP_BOTTOM, SRCCOPY);

		//时间框雷数框贴图
		StretchBlt(hdc, gap.left + (int)((col - 12.5) / 7 * wndScale), gap.top + wndScale * row + (gap.bottom - (int)(25.0 / INITIAL_BMWDITH * wndScale)) / 2, (int)(25.0 / INITIAL_BMWDITH * wndScale), (int)(25.0 / INITIAL_BMWDITH * wndScale), hdcMem_Frame, DEFAULT_GAP_LEFT, DEFAULT_GAP_TOP, 25, 25, SRCCOPY);
		StretchBlt(hdc, gap.left + wndScale * col + (int)((12.5 - col) / 7 * wndScale - 25.0 / INITIAL_BMWDITH * wndScale), gap.top + wndScale * row + (gap.bottom - (int)(25.0 / INITIAL_BMWDITH * wndScale)) / 2, (int)(25.0 / INITIAL_BMWDITH * wndScale), (int)(25.0 / INITIAL_BMWDITH * wndScale), hdcMem_Frame, DEFAULT_GAP_LEFT, DEFAULT_GAP_TOP + 25, 25, 25, SRCCOPY);
		StretchBlt(hdc, gap.left + (int)((col - 12.5 + 12) / 7 * wndScale), gap.top + wndScale * row + (gap.bottom - (int)(22.0 / INITIAL_BMWDITH * wndScale)) / 2, (int)(40.0 / INITIAL_BMWDITH * wndScale), (int)(22.0 / INITIAL_BMWDITH * wndScale), hdcMem_Frame, DEFAULT_GAP_LEFT + 25, DEFAULT_GAP_TOP, 40, 22, SRCCOPY);
		StretchBlt(hdc, gap.left + wndScale * col + (int)((12.5 - col - 17.5) / 7 * wndScale - 25.0 / INITIAL_BMWDITH * wndScale), gap.top + wndScale * row + (gap.bottom - (int)(22.0 / INITIAL_BMWDITH * wndScale)) / 2, (int)(40.0 / INITIAL_BMWDITH * wndScale), (int)(22.0 / INITIAL_BMWDITH * wndScale), hdcMem_Frame, DEFAULT_GAP_LEFT + 25, DEFAULT_GAP_TOP, 40, 22, SRCCOPY);

		DeleteDC(hdcMem_Frame);

		hdcMem = CreateCompatibleDC(hdc);
		SelectObject(hdcMem, hBitmap);

		if (hasSave)//存在保存的游戏
		{
			//贴MF_BRICKDOWN块
			for (m = 0; m < row; m++)
				for (n = 0; n < col; n++)
					LightShadow(hdc, m, n, hdcMem, 4, 0);

			//显示对话框
			if ((optionFlag & OF_STARTATSAVE) || MessageBox(hWnd, TEXT("您想继续保存的游戏吗?"), TEXT("找到保存的游戏"), MB_YESNO) == IDYES)
			{
				//以保存的游戏开始
				startAtSave = true;
				InitMapInfo(seed, firstLButtonNo, saveFlags, hWnd, hdc);
			}
			else
			{
				//以新游戏开始
				startAtSave = false;
				InitInterface(row, col, count, 0, hWnd, hdc, hdcMem);
			}
			hasSave = false;
		}
		else
		{
			//以新游戏开始
			startAtSave = false;
			InitInterface(row, col, count, 0, hWnd, hdc, hdcMem);
		}

		//重绘贴图
		for (m = 0; m < row; m++)
			for (n = 0; n < col; n++)
				LightShadow(hdc, m, n, hdcMem, (mapInfo[n + m * col] & 0x00FF), (mapInfo[n + m * col] & 0x0F00) / 0x0100);

		//显示时间雷数
		showTime(timeCount, hdc);
		showMime(flagCount, hdc);

		DeleteDC(hdcMem);
		EndPaint(hWnd, &ps);
		break;


	case WM_LBUTTONDOWN:
		//获得鼠标状态结构体
		mouseState = getMouseState(LOWORD(lParam), HIWORD(lParam), WM_LBUTTONDOWN);
		if (wParam & MK_RBUTTON)
		{
			//双键按下
			mouseState.mType = WM_LBUTTONDBLCLK;
			OnMouseClick(hWnd, mouseState, WM_LBUTTONDBLCLK);
		}
		else
		{
			mouseState.mType = WM_LBUTTONDOWN;
			OnMouseClick(hWnd, mouseState, WM_LBUTTONDOWN);
		}
		break;


	case WM_LBUTTONUP:
		hdc = GetDC(hWnd);

		//双键弹起过程中
		if (mouseState.mType == WM_RBUTTONDBLCLK)
			break;

		int lastType;
		lastType = mouseState.mType;
		//获得鼠标状态结构体
		mouseState = getMouseState(LOWORD(lParam), HIWORD(lParam), WM_LBUTTONUP);
		//双键按下过程中不初始化雷区块标志数组
		if (lastType == WM_LBUTTONDOWN && mouseState.mCol >= 0 && mouseState.mCol < col && mouseState.mRow >= 0 && mouseState.mRow < row && mapInfo[mouseState.mCol + mouseState.mRow * col] != 0xF006)
			InitMapInfo((unsigned int)time(NULL), mouseState.mCol + mouseState.mRow * col, NULL, hWnd, hdc);

		//启动计时器
		if (timeFlag)
		{
			SetTimer(hWnd, ID_TIMER, 1000, NULL);
			timeFlag = false;
		}

		if (wParam & MK_RBUTTON)
		{
			//双键弹起
			mouseState.mType = WM_RBUTTONDBLCLK;
			OnMouseClick(hWnd, mouseState, WM_RBUTTONDBLCLK);
		}
		else
		{
			OnMouseClick(hWnd, mouseState, WM_LBUTTONUP);
		}
		ReleaseDC(hWnd, hdc);
		break;


	case WM_RBUTTONDOWN:
		//获得鼠标状态结构体
		mouseState = getMouseState(LOWORD(lParam), HIWORD(lParam), WM_RBUTTONDOWN);
		if (wParam & MK_LBUTTON)
		{
			//双键按下
			mouseState.mType = WM_LBUTTONDBLCLK;
			OnMouseClick(hWnd, mouseState, WM_LBUTTONDBLCLK);
		}
		else
		{
			OnMouseClick(hWnd, mouseState, WM_RBUTTONDOWN);
		}
		break;


	case WM_RBUTTONUP:
		//双键弹起过程中
		if (mouseState.mType == WM_RBUTTONDBLCLK)
			break;

		//获得鼠标状态结构体
		mouseState = getMouseState(LOWORD(lParam), HIWORD(lParam), WM_RBUTTONUP);
		if (wParam & MK_LBUTTON)
		{
			//双键弹起
			mouseState.mType = WM_RBUTTONDBLCLK;
			OnMouseClick(hWnd, mouseState, WM_RBUTTONDBLCLK);
		}
		else
		{
			OnMouseClick(hWnd, mouseState, WM_RBUTTONUP);
		}
		break;


	case WM_MOUSEMOVE:
		//获得鼠标状态结构体
		mouseState = getMouseState(LOWORD(lParam), HIWORD(lParam), WM_MOUSEMOVE);
		if (wParam & MK_LBUTTON)
		{
			if (wParam & MK_RBUTTON)
			{
				//双键按下
				mouseState.mType = WM_LBUTTONDBLCLK;
				OnMouseClick(hWnd, mouseState, WM_MOUSEMOVE);
			}
			else
			{
				//左键按下
				mouseState.mType = WM_LBUTTONDOWN;
				OnMouseClick(hWnd, mouseState, WM_MOUSEMOVE);
			}
		}
		else
		{
			//键未按下或只按下右键
			OnMouseClick(hWnd, mouseState, WM_MOUSEMOVE);
		}
		break;


	case WM_TIMER:
		hdc = GetDC(hWnd);
		if (timeCount < 999)
			timeCount++;
		showTime(timeCount, hdc);
		ReleaseDC(hWnd, hdc);
		break;


	case WM_GETMINMAXINFO://控制窗口最小宽高
		MINMAXINFO *mminfo;
		mminfo = (PMINMAXINFO)lParam;
		mminfo->ptMinTrackSize.x = INITIAL_BMWDITH * col + DEFAULT_GAP_LEFT + DEFAULT_GAP_RIGHT + 2 * DEFAULT_XFRAME;// (GetSystemMetrics(SM_CXFRAME) + 2) * 2 + GetSystemMetrics(SM_CXBORDER) * 4;;
		mminfo->ptMinTrackSize.y = INITIAL_BMHEIGHT * row + DEFAULT_GAP_TOP + DEFAULT_GAP_BOTTOM + 2 * DEFAULT_YFRAME + DEFAULT_YTOP;// (GetSystemMetrics(SM_CYFRAME) + 2) * 2 + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYMENU) + GetSystemMetrics(SM_CYBORDER) * 4;
		break;


	case WM_MOVE:
		//移动后的位置记入配置文件
		wndX = LOWORD(lParam) - DEFAULT_XFRAME;
		wsprintf(value, TEXT("%d"), wndX);
		WritePrivateProfileString(TEXT("BASIC"), TEXT("WNDX"), value, PROFILE);
		wndY = HIWORD(lParam) - DEFAULT_YFRAME - DEFAULT_YTOP;
		wsprintf(value, TEXT("%d"), wndY);
		WritePrivateProfileString(TEXT("BASIC"), TEXT("WNDY"), value, PROFILE);
		break;


	case WM_SIZING://等比例放大缩小窗口
		RECT* pRect;
		pRect = (RECT*)lParam;
		float kWidth, kHeight;
		//放大比例
		kWidth = (float)((*pRect).right - (*pRect).left) / (float)(INITIAL_BMWDITH * col + DEFAULT_GAP_LEFT + DEFAULT_GAP_RIGHT + 2 * DEFAULT_XFRAME);
		kHeight = (float)((*pRect).bottom - (*pRect).top) / (float)(INITIAL_BMHEIGHT * row + DEFAULT_GAP_TOP + DEFAULT_GAP_BOTTOM + 2 * DEFAULT_YFRAME + DEFAULT_YTOP);
		switch (wParam)
		{
		case WMSZ_RIGHT:case WMSZ_LEFT:case WMSZ_BOTTOMRIGHT:case WMSZ_BOTTOMLEFT:
			(*pRect).bottom = (*pRect).top + (int)(kWidth * (INITIAL_BMHEIGHT * row + DEFAULT_GAP_TOP + DEFAULT_GAP_BOTTOM + 2 * DEFAULT_YFRAME + DEFAULT_YTOP));
			wndScale = (int)(kWidth * INITIAL_BMWDITH);
			break;
		case WMSZ_TOP:case WMSZ_BOTTOM:
			(*pRect).right = (*pRect).left + (int)(kHeight * (INITIAL_BMWDITH * col + DEFAULT_GAP_LEFT + DEFAULT_GAP_RIGHT + 2 * DEFAULT_XFRAME));
			wndScale = (int)(kHeight * INITIAL_BMWDITH);
			break;
		case WMSZ_TOPLEFT:case WMSZ_TOPRIGHT:
			(*pRect).top = (*pRect).bottom - (int)(kWidth * (INITIAL_BMHEIGHT * row + DEFAULT_GAP_TOP + DEFAULT_GAP_BOTTOM + 2 * DEFAULT_YFRAME + DEFAULT_YTOP));
			wndScale = (int)(kWidth * INITIAL_BMWDITH);
			break;
		}
		//更新主窗口宽高
		wndWidth = (*pRect).right - (*pRect).left;
		wndHeight = (*pRect).bottom - (*pRect).top;
		//更新雷区外背景区宽高
		gap.top = (wndHeight - 2 * DEFAULT_YFRAME - DEFAULT_YTOP - wndScale * row) / 2;
		gap.bottom = wndHeight - 2 * DEFAULT_YFRAME - DEFAULT_YTOP - wndScale * row - gap.top;
		gap.right = (wndWidth - DEFAULT_XFRAME - wndScale * col) / 2;
		gap.left = wndWidth - DEFAULT_XFRAME - wndScale * col - gap.right;

		//记入配置文件
		wsprintf(value, TEXT("%d"), wndScale);
		WritePrivateProfileString(TEXT("BASIC"), TEXT("WNDSCALE"), value, PROFILE);
		break;


	case WM_SIZE:
		static int lastfwSizeType;

		switch (wParam)
		{
		case SIZE_MINIMIZED://最小化
			KillTimer(hWnd, ID_TIMER);
			lastfwSizeType = SIZE_MINIMIZED;
			break;
		case SIZE_MAXIMIZED://最大化
			//更新主窗口宽高
			wndWidth = LOWORD(lParam) + DEFAULT_XFRAME;
			wndHeight = HIWORD(lParam) + 2 * DEFAULT_YFRAME + DEFAULT_YTOP;
			wndScale = min((int)(LOWORD(lParam) / (float)(INITIAL_BMWDITH * col + DEFAULT_GAP_LEFT + DEFAULT_GAP_RIGHT) * INITIAL_BMWDITH),
				(int)(HIWORD(lParam) / (float)(INITIAL_BMHEIGHT * row + DEFAULT_GAP_TOP + DEFAULT_GAP_BOTTOM) * INITIAL_BMWDITH));
			//更新雷区外背景区宽高
			gap.top = (HIWORD(lParam) - wndScale * row) / 2;
			gap.bottom = HIWORD(lParam) - wndScale * row - gap.top;
			gap.right = (LOWORD(lParam) - wndScale * col) / 2;
			gap.left = LOWORD(lParam) - wndScale * col - gap.right;
			//从最小化到最大化启动计时器
			if (lastfwSizeType == SIZE_MINIMIZED && firstLButtonDown)
				SetTimer(hWnd, ID_TIMER, 1000, NULL);
			lastfwSizeType = SIZE_MAXIMIZED;
			break;
		case SIZE_RESTORED://还原
			if (lastfwSizeType == SIZE_MAXIMIZED)
			{
				//更新主窗口宽高
				wndWidth = LOWORD(lParam) + DEFAULT_XFRAME;
				wndHeight = HIWORD(lParam) + 2 * DEFAULT_YFRAME + DEFAULT_YTOP;
				wndScale = min((int)(LOWORD(lParam) / (float)(INITIAL_BMWDITH * col + DEFAULT_GAP_LEFT + DEFAULT_GAP_RIGHT) * INITIAL_BMWDITH),
					(int)(HIWORD(lParam) / (float)(INITIAL_BMHEIGHT * row + DEFAULT_GAP_TOP + DEFAULT_GAP_BOTTOM) * INITIAL_BMWDITH));
				//更新雷区外背景区宽高
				gap.top = (HIWORD(lParam) - wndScale * row) / 2;
				gap.bottom = HIWORD(lParam) - wndScale * row - gap.top;
				gap.right = (LOWORD(lParam) - wndScale * col) / 2;
				gap.left = LOWORD(lParam) - wndScale * col - gap.right;
			}
			//启动计时器
			if (lastfwSizeType == SIZE_MINIMIZED && firstLButtonDown)
				SetTimer(hWnd, ID_TIMER, 1000, NULL);
			lastfwSizeType = SIZE_RESTORED;
			break;
		default:
			/*bug	主窗口移动到桌面左边缘或右边缘被系统放大, 
					没更新客户区, 若更新客户区, 则在改变主窗口大小, 
					并在雷区移动鼠标时, 有时会出现块重绘位置错误*/
			break;
		}
		break;


	case WM_CLOSE:
		int temp;
		//弹出对话框
		if ((!startAtSave && firstLButtonDown) || flagCount != 0)
		{
			if ((optionFlag & OF_SAVE) || (temp = MessageBox(hWnd, TEXT("游戏正在进行 , 您希望保存游戏吗?"), TEXT("退出游戏"), MB_YESNOCANCEL)) == IDYES)
			{
				//保存游戏
				writeProfileSave();
				DestroyWindow(hWnd);
			}
			else if (temp == IDNO)
			{
				//不保存游戏
				DestroyWindow(hWnd);
				break;
			}
		}
		else
		{
			DestroyWindow(hWnd);
		}
		break;


	case WM_DESTROY:
		PostQuitMessage(0);
		break;


	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

tagMouseState getMouseState(int mXpos, int mYpos, int mType) {
	tagMouseState mouseState;
	//光标坐标在雷区
	if (mXpos >= gap.left && mXpos <= gap.left + wndScale * col
		&& mYpos >= gap.top && mYpos <= gap.top + wndScale * row)
	{
		//计算光标所在块的位置
		mouseState.mCol = (mXpos - gap.left) / wndScale;
		mouseState.mRow = (mYpos - gap.top) / wndScale;
	}
	else
	{
		mouseState.mCol = -1;
		mouseState.mRow = -1;
	}
	mouseState.mType = mType;//鼠标按键类型
	return mouseState;
}

bool OnMouseClick(HWND hWnd, tagMouseState mouseState, int nIDMessage) {
	static tagMouseState lastMouseState;//保存的鼠标状态结构体
	bool msgFlag = false;
	int no, temp1;

	HDC hdc = GetDC(hWnd);
	HDC hdcMem = CreateCompatibleDC(hdc);
	SelectObject(hdcMem, hBitmap);

	switch (nIDMessage)
	{
	case WM_LBUTTONDOWN://左键按下
		buttonDown(mouseState, hWnd, hdc, hdcMem);//块按下
		lastMouseState = mouseState;
		break;

	case WM_LBUTTONUP://左键弹起
		buttonUp(mouseState, hWnd, hdc, hdcMem, MF_LIGHTFLAG);//块刷新
		lastMouseState = mouseState;
		break;

	case WM_RBUTTONDOWN://右键按下
		buttonChange(mouseState, hWnd, hdc, hdcMem);//块改变
		lastMouseState = mouseState;
		break;

	case WM_RBUTTONUP://右键弹起
		break;

	case WM_LBUTTONDBLCLK://双键按下
		nineButtonDown(mouseState, hWnd, hdc, hdcMem);//九个块按下
		lastMouseState = mouseState;
		break;

	case WM_RBUTTONDBLCLK://双键弹起
		if (mouseState.mCol >= 0 && mouseState.mCol < col && mouseState.mRow >= 0 && mouseState.mRow < row)
		{
			no = mouseState.mCol + mouseState.mRow * col;

			if ((mapInfo[no] & 0x0F00) == 0x0000 && (mapInfo[no] & 0x00FF) % 3 == 0)//按下的块不是数字
				nineButtonRestore(mouseState, hWnd, hdc, hdcMem);//九个块还原
			else if ((mapInfo[no] & 0x0F00) == 0x0100)//按下的块是数字
			{
				if (dbButtonClickFlag(no, mapInfo, row, col) != mapInfo[no] / 0x1000)//周围红旗数不等于雷数
				{
					nineButtonRestore(mouseState, hWnd, hdc, hdcMem);//九个块还原
					if (optionFlag & OF_SOUND)
						PlaySound(MAKEINTRESOURCE(IDR_WAVE3), NULL, SND_RESOURCE | SND_ASYNC);//播放声音
					//带红叉的数字闪现两次
					for (int i = 0; (optionFlag & OF_DRAW) && i < 2; i++)
					{
						Sleep(150);
						LightShadow(hdc, mouseState.mRow, mouseState.mCol, hdcMem, (mapInfo[no] & 0x00FF) + 9, (mapInfo[no] & 0x0F00) / 0x0100);
						Sleep(150);
						LightShadow(hdc, mouseState.mRow, mouseState.mCol, hdcMem, mapInfo[no] & 0x00FF, (mapInfo[no] & 0x0F00) / 0x0100);
					}
				}
				else if ((temp1 = dbButtonClickMine(no, mapInfo, row, col)) == 0)//红旗位置正确
					nineButtonUp(mouseState, hWnd, hdc, hdcMem);//九个块刷新
				else//红旗位置不正确
				{
					nineButtonRestore(mouseState, hWnd, hdc, hdcMem);//九个块还原
					no += temp1;
					if ((optionFlag & 0x03) < 3)
						changeStatistic((optionFlag & 0x03), -1);//游戏失败
					mineBomb(no / col, no % col, hWnd, hdc, hdcMem);//雷爆炸效果
					showLose(hWnd);//显示失败对话框
				}
			}
		}
		lastMouseState = mouseState;
		break;

	case WM_MOUSEMOVE://鼠标移动
		switch (mouseState.mType)
		{
		case WM_MOUSEMOVE://键未按下
			if (lastMouseState.mCol != mouseState.mCol || lastMouseState.mRow != mouseState.mRow)
			{
				buttonRestore(lastMouseState, hWnd, hdc, hdcMem);//块还原
				buttonLight(mouseState, hWnd, hdc, hdcMem);//块高亮
			}
			lastMouseState = mouseState;
			break;

		case WM_LBUTTONDOWN://左键按下
			if (lastMouseState.mCol != mouseState.mCol || lastMouseState.mRow != mouseState.mRow)
			{
				buttonRestore(lastMouseState, hWnd, hdc, hdcMem);//块还原
				buttonDown(mouseState, hWnd, hdc, hdcMem);//块按下
			}
			lastMouseState = mouseState;
			break;

		case WM_LBUTTONDBLCLK://双键按下
			if (lastMouseState.mCol != mouseState.mCol || lastMouseState.mRow != mouseState.mRow)
			{
				nineButtonRestore(lastMouseState, hWnd, hdc, hdcMem);//九个块还原
				nineButtonDown(mouseState, hWnd, hdc, hdcMem);//九个块按下
			}
			lastMouseState = mouseState;
			break;
		}
		break;
	}


	DeleteDC(hdcMem);
	ReleaseDC(hWnd, hdc);

	return true;
}


void InitInterface(int tRow, int tCol, int tCount, int uType, HWND hWnd, HDC hdc, HDC hdcMem)
{
	if (!firstPaint)//未播放初始化动画效果
	{
		//初始化行列雷数
		row = tRow;
		col = tCol;
		count = tCount;
		brickNumber = row * col;

		//停止计时器, 重置并显示时间计数
		KillTimer(hWnd, ID_TIMER);
		timeCount = DEFAULT_TIMECOUNT;
		showTime(timeCount, hdc);

		//重置红旗数, 初始块数, 显示雷数
		flagCount = 0;
		brickCount = brickNumber;
		showMime(flagCount, hdc);


		unsigned short int *temp;
		temp = new unsigned short int[brickNumber];
		if (uType == 0)//重新布雷
		{
			//重置雷区块标志数组
			if (mapInfo != NULL)
				delete[] mapInfo;
			mapInfo = new unsigned short int[brickNumber];
			for (int i = 0; i < brickNumber; i++)
				mapInfo[i] = MF_NOINIT | MF_BRICKDOWN;
		}
		else if (uType == 1)//不重新布雷
		{
			for (int i = 0; i < brickNumber; i++)
			{
				//保存并重置雷区块标志数组
				temp[i] = (mapInfo[i] & 0xF000) + MF_BRICK;
				mapInfo[i] = MF_NOINIT | MF_BRICKDOWN;
			}
		}

		//重置保存的块贴图信息数组
		if (saveFlags != NULL)
			delete[] saveFlags;
		saveFlags = new TCHAR[brickNumber + 1];

		//贴MF_BRICKDOWN块
		for (int m = 0; m < row; m++)
			for (int n = 0; n < col; n++)
				LightShadow(hdc, m, n, hdcMem, 3, 0);

		if (uType == 0)//重新布雷
		{
			//初始化雷区块标志数组
			for (int i = 0; i < brickNumber; i++)
				mapInfo[i] = MF_INIT;
		}
		if (uType == 1)//不重新布雷
		{
			//还原雷区块标志数组
			for (int i = 0; i < brickNumber; i++)
				mapInfo[i] = temp[i];
		}
		delete[] temp;


		int *ptemp1 = getFirstPaintNo(row, col);//初始化动画贴图块序列数组
		if ((optionFlag & OF_DRAW) && (optionFlag & OF_SOUND))
			PlaySound(MAKEINTRESOURCE(IDR_WAVE1), NULL, SND_RESOURCE | SND_ASYNC);//播放声音
		hasSpeed = false;//重置效果延时控制标志
		for (int i = 0; i < brickNumber; i++)
		{
			LightShadow(hdc, ptemp1[i] / col, ptemp1[i] % col, hdcMem, 4, 0);
			speed((int)(1100.0 / (row * col)), NULL, 0);//效果延时控制
			//bug	WM_LBUTTONDOWN停止动画, WM_LBUTTONUP消息会初始化mapInfo
			//bug	与有道词典一起运行会bug

			/*if (optionFlag & OF_DRAW)
				Sleep((int)(1100.0 / (row * col)));*/
		}
		//PlaySound(NULL, NULL, SND_ASYNC);
		delete[] ptemp1;

		firstPaint = true;
	}
}

void InitMapInfo(unsigned int tSeed, int tFirstLButtonNo, TCHAR *saveFlags, HWND hWnd, HDC hdc)
{
	if (!firstLButtonDown)//游戏未开始
	{
		if (!firstPaint)//未播放初始化效果
		{
			//停止计时器, 显示时间计数
			KillTimer(hWnd, ID_TIMER);
			showTime(timeCount, hdc);

			//重置红旗数, 初始块数, 显示雷数
			flagCount = 0;
			brickCount = brickNumber;
			showMime(flagCount, hdc);

			//重置雷区块标志数组
			if (mapInfo != NULL)
				delete[] mapInfo;
			mapInfo = new unsigned short int[brickNumber];
			for (int i = 0; i < brickNumber; i++)
				mapInfo[i] = MF_INIT;

			firstPaint = true;
		}

		//初始化雷区块标志数组, 随机布雷
		seed = tSeed;
		firstLButtonNo = tFirstLButtonNo;
		initMineCount(mapInfo, seed, firstLButtonNo, row, col, count);
		if (saveFlags != NULL) {
			save2map(mapInfo, saveFlags, brickNumber);//从保存的游戏计算雷区块标志数组
			//计算红旗数初始块数
			for (int i = 0; i < brickNumber; i++)
			{
				if ((mapInfo[i] & 0x0F00) == 0x0000 && (mapInfo[i] & 0x00FF) / 3 == 2)
					flagCount++;
				if ((mapInfo[i] & 0x0F00) != 0x0000 || (mapInfo[i] & 0x00FF) / 3 != 1)
					brickCount--;
			}
		}

		SetTimer(hWnd, ID_TIMER, 1000, NULL);//启动计时器

		firstLButtonDown = true;
	}
}


void LightShadow(HDC hdcDest, int rowDest, int colDest, HDC hdcSrc, int rowSrc, int colSrc)
{
	if ((rowSrc % 3 == 1 && colSrc == 0) || (rowSrc == 6 && colSrc == 0) ||  (rowSrc == 0 && colSrc >= 3 && colSrc <= 7))//光效果
	{
		int r, g, b, dr, dg, db, start;//源rgb值, 贴图rgb值增量, 贴图像素起始位置
		double xi, xj;//行列
		xi = (double)rowDest / row * 24;
		xj = (double)colDest / col * 30;

		//计算贴图rgb值增量
		dr = (int)DR(xj, xi);
		dg = (int)DG(xj, xi);
		db = (int)DB(xj, xi);

		//贴图像素起始位置
		start = rowSrc * INITIAL_BMHEIGHT * bitmap.bmWidth + colSrc * INITIAL_BMWDITH;

		for (int i = 0; i < INITIAL_BMHEIGHT; i++)
		{
			for (int j = 0; j < INITIAL_BMWDITH; j++)
			{
				//计算贴图rgb值
				r = dr + ((unsigned int*)lpvBits)[start + i * bitmap.bmWidth + j] / 0x010000;
				g = dg + (((unsigned int*)lpvBits)[start + i * bitmap.bmWidth + j] & 0x00FF00) / 0x000100;
				b = db + ((unsigned int*)lpvBits)[start + i * bitmap.bmWidth + j] & 0x0000FF;

				//限定范围
				r = min(max(r, 0), 255);
				g = min(max(g, 0), 255);
				b = min(max(b, 0), 255);

				//合并成颜色值并存入数组
				pBrick[i * INITIAL_BMWDITH + j] = r * 0x010000 + g * 0x000100 + b;
			}
		}
		HDC hdcMem1;
		hdcMem1 = CreateCompatibleDC(hdcDest);

		//将颜色数组赋给块贴图句柄
		SetBitmapBits(hBitmapT, INITIAL_BMWDITH * INITIAL_BMHEIGHT * 4, (LPVOID)pBrick);
		SelectObject(hdcMem1, hBitmapT);

		//用块贴图句柄贴图
		StretchBlt(hdcDest, gap.left + wndScale * colDest, gap.top + wndScale * rowDest, wndScale, wndScale, hdcMem1, 0, 0, INITIAL_BMWDITH, INITIAL_BMHEIGHT, SRCCOPY);

		DeleteDC(hdcMem1);
	}
	else if (rowSrc % 3 == 2 && colSrc == 0)//高亮直接贴图
	{
		StretchBlt(hdcDest, gap.left + wndScale * colDest, gap.top + wndScale * rowDest, wndScale, wndScale, hdcSrc, INITIAL_BMWDITH * colSrc, INITIAL_BMHEIGHT * rowSrc, INITIAL_BMWDITH, INITIAL_BMHEIGHT, SRCCOPY);
	}
	else//影效果
	{
		//块贴图
		StretchBlt(hdcDest, gap.left + wndScale * colDest, gap.top + wndScale * rowDest, wndScale, wndScale, hdcSrc, INITIAL_BMWDITH * colSrc, INITIAL_BMHEIGHT * rowSrc, INITIAL_BMWDITH, INITIAL_BMHEIGHT, SRCCOPY);
		
		/*影标志	影的位置
			0		无
			1		上
			2		左
			3		上和左*/
		int shadowFlag = 0x0;
		int temp, temp1;

		//判断影标志
		if (rowDest == 0)
			shadowFlag = (shadowFlag & 0x2) + 0x1;
		else
		{
			temp = mapInfo[(rowDest - 1) * col + colDest];
			if (((temp & 0x0F00) == 0x0000 && (temp & 0x00FF) != 0 && (temp & 0x00FF) != 3) || ((temp & 0x00FF) == 0 && (temp & 0x0F00) >= 0x0300 && (temp & 0x0F00)  <= 0x0700))
				shadowFlag = (shadowFlag & 0x2) + 0x1;
		}
		if (colDest == 0)
			shadowFlag = (shadowFlag & 0x1) + 0x2;
		else
		{
			temp = mapInfo[rowDest * col + colDest - 1];
			if (((temp & 0x0F00) == 0x0000 && (temp & 0x00FF) != 0 && (temp & 0x00FF) != 3) || ((temp & 0x00FF) == 0 && (temp & 0x0F00) >= 0x0300 && (temp & 0x0F00)  <= 0x0700))
				shadowFlag = (shadowFlag & 0x1) + 0x2;
		}

		//影效果贴图
		if (shadowFlag == 1)
			StretchBlt(hdcDest, gap.left + wndScale * colDest, gap.top + wndScale * rowDest, wndScale, (int)(wndScale * 3.0 / 18), hdcSrc, INITIAL_BMWDITH * 2, INITIAL_BMHEIGHT * 1, INITIAL_BMWDITH, 3, SRCCOPY);
		if (shadowFlag == 2)
			StretchBlt(hdcDest, gap.left + wndScale * colDest, gap.top + wndScale * rowDest, (int)(wndScale * 3.0 / 18), wndScale, hdcSrc, INITIAL_BMWDITH * 2, INITIAL_BMHEIGHT * 2, 3, INITIAL_BMHEIGHT, SRCCOPY);
		if (shadowFlag == 3)
		{
			StretchBlt(hdcDest, gap.left + wndScale * colDest, gap.top + wndScale * rowDest, wndScale, (int)(wndScale * 3.0 / 18), hdcSrc, INITIAL_BMWDITH * 2, INITIAL_BMHEIGHT * 3, INITIAL_BMWDITH, 3, SRCCOPY);
			StretchBlt(hdcDest, gap.left + wndScale * colDest, gap.top + wndScale * rowDest + (int)(wndScale * 3.0 / 18), (int)(wndScale * 3.0 / 18), wndScale - (int)(wndScale * 3.0 / 18), hdcSrc, INITIAL_BMWDITH * 2, INITIAL_BMHEIGHT * 3 + 3, 3, INITIAL_BMHEIGHT - 3, SRCCOPY);
		}

		//改变下方块的影效果
		if (rowDest < row - 1)
		{
			temp = mapInfo[(rowDest + 1) * col + colDest];
			if ((temp & 0x0FFF) == MF_BRICKDOWN || (temp & 0x0F00) == 0x0100 || (temp & 0x0F00) == 0x0200 || ((temp & 0x0F00) >= 0x0300 && (temp & 0x0F00) <= 0x0700 && (temp & 0x00FF) != 0x0000))
			{
				StretchBlt(hdcDest, gap.left + wndScale * colDest, gap.top + wndScale * (rowDest + 1), wndScale, wndScale, hdcSrc, INITIAL_BMWDITH * ((temp & 0x0F00) / 0x0100), INITIAL_BMHEIGHT * (temp & 0x00FF), INITIAL_BMWDITH, INITIAL_BMHEIGHT, SRCCOPY);
				if (colDest != 0)
				{
					temp1 = mapInfo[(rowDest + 1) * col + colDest - 1];
					if (((temp1 & 0xF000) != MF_NOINIT) && (((temp1 & 0x0F00) == 0x0000 && (temp1 & 0x00FF) != 0 && (temp1 & 0x00FF) != 3) || ((temp1 & 0x00FF) == 0 && (temp1 & 0x0F00) >= 0x0300 && (temp1 & 0x0F00) <= 0x0700)))
						StretchBlt(hdcDest, gap.left + wndScale * colDest, gap.top + wndScale * (rowDest + 1), (int)(wndScale * 3.0 / 18), wndScale, hdcSrc, INITIAL_BMWDITH * 2, INITIAL_BMHEIGHT * 2, 3, INITIAL_BMHEIGHT, SRCCOPY);
				}
				else
					StretchBlt(hdcDest, gap.left + wndScale * colDest, gap.top + wndScale * (rowDest + 1), (int)(wndScale * 3.0 / 18), wndScale, hdcSrc, INITIAL_BMWDITH * 2, INITIAL_BMHEIGHT * 2, 3, INITIAL_BMHEIGHT, SRCCOPY);
			}
		}
		//改变右方块的影效果
		if (colDest < col - 1)
		{
			temp = mapInfo[rowDest * col + colDest + 1];
			if ((temp & 0x0FFF) == MF_BRICKDOWN || (temp & 0x0F00) == 0x0100 || (temp & 0x0F00) == 0x0200 || ((temp & 0x0F00) >= 0x0300 && (temp & 0x0F00) <= 0x0700 && (temp & 0x00FF) != 0x0000))
			{
				StretchBlt(hdcDest, gap.left + wndScale * (colDest + 1), gap.top + wndScale * rowDest, wndScale, wndScale, hdcSrc, INITIAL_BMWDITH * ((temp & 0x0F00) / 0x0100), INITIAL_BMHEIGHT * (temp & 0x00FF), INITIAL_BMWDITH, INITIAL_BMHEIGHT, SRCCOPY);
				if (rowDest != 0)
				{
					temp1 = mapInfo[(rowDest - 1) * col + colDest + 1];
					if (((temp1 & 0xF000) != MF_NOINIT) && (((temp1 & 0x0F00) == 0x0000 && (temp1 & 0x00FF) != 0 && (temp1 & 0x00FF) != 3) || ((temp1 & 0x00FF) == 0 && (temp1 & 0x0F00) >= 0x0300 && (temp1 & 0x0F00) <= 0x0700)))
						StretchBlt(hdcDest, gap.left + wndScale * (colDest + 1), gap.top + wndScale * rowDest, wndScale, (int)(wndScale * 3.0 / 18), hdcSrc, INITIAL_BMWDITH * 2, INITIAL_BMHEIGHT * 1, INITIAL_BMWDITH, 3, SRCCOPY);
				}
				else
					StretchBlt(hdcDest, gap.left + wndScale * (colDest + 1), gap.top + wndScale * rowDest, wndScale, (int)(wndScale * 3.0 / 18), hdcSrc, INITIAL_BMWDITH * 2, INITIAL_BMHEIGHT * 1, INITIAL_BMWDITH, 3, SRCCOPY);
			}
		}
	}
}


void showTime(int timeCount, HDC hdc)
{
	TCHAR value[10];
	wsprintf(value, TEXT(" %d "), timeCount);
	//显示设置
	//SetBkMode(hdc, TRANSPARENT);
	SetBkColor(hdc, RGB(50, 90, 160));
	SetTextColor(hdc, RGB(196, 218, 235));

	//位置矩阵
	RECT rect;
	rect.top = gap.top + wndScale * row + (gap.bottom - (int)(22.0 / INITIAL_BMWDITH * wndScale)) / 2;
	rect.bottom = gap.top + wndScale * row + (gap.bottom + (int)(22.0 / INITIAL_BMWDITH * wndScale)) / 2;
	rect.left = gap.left + (int)((col - 12.5 + 12) / 7 * wndScale);
	rect.right = gap.left + (int)((col - 12.5 + 12) / 7 * wndScale + 40.0 / INITIAL_BMWDITH * wndScale);

	//显示时间
	DrawText(hdc, value, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void showMime(int flagCount, HDC hdc)
{
	TCHAR value[10];
	wsprintf(value, TEXT(" %d "), count - flagCount);
	//显示设置
	//SetBkMode(hdc, TRANSPARENT);
	SetBkColor(hdc, RGB(50, 90, 160));
	SetTextColor(hdc, RGB(196, 218, 235));

	//位置矩阵
	RECT rect;
	rect.top = gap.top + wndScale * row + (gap.bottom - (int)(22.0 / INITIAL_BMWDITH * wndScale)) / 2;
	rect.bottom = gap.top + wndScale * row + (gap.bottom + (int)(22.0 / INITIAL_BMWDITH * wndScale)) / 2;
	rect.left = gap.left + wndScale * col + (int)((12.5 - col - 17.5) / 7 * wndScale - 25.0 / INITIAL_BMWDITH * wndScale);
	rect.right = gap.left + wndScale * col + (int)((12.5 - col - 17.5) / 7 * wndScale - 25.0 / INITIAL_BMWDITH * wndScale + 40.0 / INITIAL_BMWDITH * wndScale);

	//显示雷数
	DrawText(hdc, value, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void checkWin(HWND hWnd, HDC hdc)
{
	if (flagCount + brickCount == count)//判断游戏胜利
	{
		KillTimer(hWnd, ID_TIMER);
		showMime(count, hdc);
		if ((optionFlag & 0x03) < 3)
			changeStatistic((optionFlag & 0x03), 1);//游戏胜利
		//显示胜利对话框
		switch (DialogBox(hInst, MAKEINTRESOURCE(IDD_WIN), hWnd, Win))
		{
		case 0://退出
			DestroyWindow(hWnd);
			break;
		case 2://再来一局
			PostMessage(hWnd, WM_COMMAND, MAKEWPARAM(ID_NEWGAME, 3), 0);
			break;
		}
	}
}

void showLose(HWND hWnd)
{
	//显示失败对话框
	switch (DialogBox(hInst, MAKEINTRESOURCE(IDD_LOSE), hWnd, Lose))
	{
	case 0://退出
		DestroyWindow(hWnd);
		break;
	case 1://重新开始这局游戏
		PostMessage(hWnd, WM_COMMAND, MAKEWPARAM(ID_NEWGAME, 2), 0);
		break;
	case 2://新游戏
		PostMessage(hWnd, WM_COMMAND, MAKEWPARAM(ID_NEWGAME, 3), 0);
		break;
	}
}


int zeroDevelop(int rowDest, int colDest, HWND hWnd, HDC hdc, HDC hdcMem)
{
	int number = 0;//冲开的块数
	int no = colDest + rowDest * col;//点到的块序号
	mapInfo[no] = (mapInfo[no] & 0xF000) + MF_ZERO;
	LightShadow(hdc, rowDest, colDest, hdcMem, mapInfo[no] & 0x00FF, (mapInfo[no] & 0x0F00) / 0x0100);

	int *ptemp1 = getZeroDevelopNo(no, mapInfo, row, col);//冲开块的序列数组
	if (optionFlag & OF_SOUND)
		PlaySound(MAKEINTRESOURCE(IDR_WAVE2), NULL, SND_RESOURCE | SND_ASYNC);//播放声音

	//冲开块贴图
	for (int i = 0; ptemp1[i] != -1 && i < brickNumber; i++)
	{
		if ((mapInfo[ptemp1[i]] & 0x0F00) != 0x0100 && ((mapInfo[ptemp1[i]] & 0x0F00) != 0x0000 || (mapInfo[ptemp1[i]] & 0x00FF) / 3 != 2))
		{
			mapInfo[ptemp1[i]] = (mapInfo[ptemp1[i]] & 0xF000) + MF_NUMBER(mapInfo[ptemp1[i]] / 0x1000);
			LightShadow(hdc, ptemp1[i] / col, ptemp1[i] % col, hdcMem, mapInfo[ptemp1[i]] & 0x00FF, (mapInfo[ptemp1[i]] & 0x0F00) / 0x0100);
			if (optionFlag & OF_DRAW)
				Sleep(1);//
			number++;
		}
	}
	return 1 + number;
}

void mineBomb(int rowDest, int colDest, HWND hWnd, HDC hdc, HDC hdcMem)
{
	KillTimer(hWnd, ID_TIMER);//停止计时器
	int no = colDest + rowDest * col;//点到的块序号

	//显示所有雷
	for (int i = 0; i < brickNumber; i++)
	{
		if ((mapInfo[i] & 0xF000) == MF_MINE || ((mapInfo[i] & 0x0F00) == 0x0000 && (mapInfo[i] & 0x00FF) / 3 == 2))
		{
			mapInfo[i] = (mapInfo[i] & 0xF000) + ((mapInfo[i] & 0x00FF) / 3 + (mapInfo[i] / 0x1000) / 9 * 3) * 0x0100;
			LightShadow(hdc, i / col, i % col, hdcMem, mapInfo[i] & 0x00FF, (mapInfo[i] & 0x0F00) / 0x0100);
		}
	}//

	mapInfo[no] += 0x0300;
	if (optionFlag & OF_SOUND)
		PlaySound(MAKEINTRESOURCE(IDR_WAVE4), NULL, SND_RESOURCE | SND_ASYNC);//播放开始爆炸声音
	//点到的雷爆炸效果
	for (int i = 1; i <= 25; i++)
	{
		mapInfo[no]++;
		LightShadow(hdc, rowDest, colDest, hdcMem, mapInfo[no] & 0x00FF, (mapInfo[no] & 0x0F00) / 0x0100);
		if (optionFlag & OF_DRAW)
			Sleep(23);
	}

	int *ptemp1 = getMineDevelopNo(no, mapInfo, 25, row, col, count);//雷爆炸贴图的序列数组
	if (optionFlag & OF_SOUND)
		PlaySound(MAKEINTRESOURCE(IDR_WAVE5), NULL, SND_RESOURCE | SND_ASYNC | SND_LOOP);//循环播放爆炸声音

	hasSpeed = false;//重置效果延时控制标志
	//所有雷爆炸效果
	for (int i = 0; ptemp1[i] > -1 && i < count * 25; i++)//
	{
		mapInfo[ptemp1[i]] = (mapInfo[ptemp1[i]] & 0xFF00) + ptemp1[i + count * 25];
		LightShadow(hdc, ptemp1[i] / col, ptemp1[i] % col, hdcMem, mapInfo[ptemp1[i]] & 0x00FF, (mapInfo[ptemp1[i]] & 0x0F00) / 0x0100);
		speed(150, ptemp1, 2 * count * 25);
	}
	if (optionFlag & OF_SOUND)
		PlaySound(MAKEINTRESOURCE(IDR_WAVE6), NULL, SND_RESOURCE | SND_ASYNC);//播放爆炸结束声音
}

void speed(DWORD dwMilliseconds, int *timeCount, int startNo)
{
	MSG msg;
	static bool msgFlag = false;
	static int temp1 = 0, i = 0;

	if (!hasSpeed)
	{
		msgFlag = false;
		temp1 = 0;
		i = 0;
		hasSpeed = true;
	}

	//鼠标消息停止动画效果
	if ((optionFlag & OF_DRAW) && (!PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) || (msg.message != WM_LBUTTONDOWN && msg.message != WM_RBUTTONDOWN)))
	{
		if (!msgFlag)//没有WM_LBUTTONDOWN或WM_RBUTTONDOWN消息
		{
			if (temp1 == 0)//延时控制值
			{
				Sleep(dwMilliseconds);///
				if (timeCount != NULL)
					temp1 = timeCount[i + startNo] / 2;//获得下一个延时控制值
			}
			else
				temp1--;
		}
	}
	else
	{
		msgFlag = true;
		PlaySound(NULL, NULL, SND_ASYNC);//停止播放所有声音
		//PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
	}
	i++;
}


void buttonDown(tagMouseState mouseState, HWND hWnd, HDC hdc, HDC hdcMem)
{
	if (mouseState.mCol >= 0 && mouseState.mCol < col && mouseState.mRow >= 0 && mouseState.mRow < row)
	{
		int no = mouseState.mCol + mouseState.mRow * col;
		if ((mapInfo[no] & 0x0F00) == 0x0000 && (mapInfo[no] & 0x00FF) % 3 != 0)//没有按下
		{
			mapInfo[no] = (mapInfo[no] & 0xF000) + (mapInfo[no] & 0x00FF) / 3 * 3;
			LightShadow(hdc, mouseState.mRow, mouseState.mCol, hdcMem, mapInfo[no] & 0x00FF, (mapInfo[no] & 0x0F00) / 0x0100);
		}
	}
}

void nineButtonDown(tagMouseState mouseState, HWND hWnd, HDC hdc, HDC hdcMem)
{
	tagMouseState tempMouseState;
	if (mouseState.mCol >= 0 && mouseState.mCol < col && mouseState.mRow >= 0 && mouseState.mRow < row)
	{
		for (int i = -1; i <= 1; i++)
		{
			tempMouseState.mCol = mouseState.mCol + i;
			for (int j = -1; j <= 1; j++)
			{
				tempMouseState.mRow = mouseState.mRow + j;
				buttonDown(tempMouseState, hWnd, hdc, hdcMem);
			}
		}
	}
}

void buttonRestore(tagMouseState mouseState, HWND hWnd, HDC hdc, HDC hdcMem)
{
	if (mouseState.mCol >= 0 && mouseState.mCol < col && mouseState.mRow >= 0 && mouseState.mRow < row)
	{
		int no = mouseState.mCol + mouseState.mRow * col;
		if ((mapInfo[no] & 0x0F00) == 0x0000 && (mapInfo[no] & 0x00FF) % 3 != 1)
		{
			mapInfo[no] = (mapInfo[no] & 0xF000) + (mapInfo[no] & 0x00FF) / 3 * 3 + 1;
			LightShadow(hdc, mouseState.mRow, mouseState.mCol, hdcMem, mapInfo[no] & 0x00FF, (mapInfo[no] & 0x0F00) / 0x0100);
		}
	}
}

void nineButtonRestore(tagMouseState mouseState, HWND hWnd, HDC hdc, HDC hdcMem)
{
	tagMouseState tempMouseState;
	if (mouseState.mCol >= 0 && mouseState.mCol < col && mouseState.mRow >= 0 && mouseState.mRow < row)
	{
		for (int i = -1; i <= 1; i++)
		{
			tempMouseState.mCol = mouseState.mCol + i;
			for (int j = -1; j <= 1; j++)
			{
				tempMouseState.mRow = mouseState.mRow + j;
				buttonRestore(tempMouseState, hWnd, hdc, hdcMem);
			}
		}
	}
}

void buttonUp(tagMouseState mouseState, HWND hWnd, HDC hdc, HDC hdcMem, int redFlagUp)
{
	if (mouseState.mCol >= 0 && mouseState.mCol < col && mouseState.mRow >= 0 && mouseState.mRow < row)
	{
		int no = mouseState.mCol + mouseState.mRow * col;
		if ((mapInfo[no] & 0x0F00) == 0x0000 && (mapInfo[no] & 0x00FF) / 3 == 2)//红旗贴图
		{
			mapInfo[no] = (mapInfo[no] & 0xF000) + redFlagUp;// MF_LIGHTFLAG;// MF_FLAG;
			LightShadow(hdc, mouseState.mRow, mouseState.mCol, hdcMem, mapInfo[no] & 0x00FF, (mapInfo[no] & 0x0F00) / 0x0100);
		}
		else if ((mapInfo[no] & 0x0FFF) == MF_UNKNOWDOWN || (mapInfo[no] & 0x0FFF) == MF_BRICKDOWN)
		{
			if ((mapInfo[no] & 0xF000) == MF_ZEROMINE)//块周围没有雷
			{
				brickCount -= zeroDevelop(mouseState.mRow, mouseState.mCol, hWnd, hdc, hdcMem);//冲开没有雷的块
				checkWin(hWnd, hdc);//检查游戏是否胜利
			}
			else if ((mapInfo[no] / 0x1000) / 9 == 0)//数字
			{
				mapInfo[no] = (mapInfo[no] & 0xF000) + MF_NUMBER(mapInfo[no] / 0x1000);
				LightShadow(hdc, mouseState.mRow, mouseState.mCol, hdcMem, mapInfo[no] & 0x00FF, (mapInfo[no] & 0x0F00) / 0x0100);
				brickCount--;//初始块减1
				checkWin(hWnd, hdc);//检查游戏是否胜利
			}
			else if ((mapInfo[no] & 0xF000) == MF_MINE)
			{
				if ((optionFlag & 0x03) < 3)
					changeStatistic((optionFlag & 0x03), -1);//游戏失败
				mineBomb(mouseState.mRow, mouseState.mCol, hWnd, hdc, hdcMem);//雷爆炸效果
				showLose(hWnd);//显示失败对话框
			}
		}
	}
}

void nineButtonUp(tagMouseState mouseState, HWND hWnd, HDC hdc, HDC hdcMem)
{
	tagMouseState tempMouseState;
	if (mouseState.mCol >= 0 && mouseState.mCol < col && mouseState.mRow >= 0 && mouseState.mRow < row)
	{
		for (int i = -1; i <= 1; i++)
		{
			tempMouseState.mCol = mouseState.mCol + i;
			for (int j = -1; j <= 1; j++)
			{
				tempMouseState.mRow = mouseState.mRow + j;
				buttonUp(tempMouseState, hWnd, hdc, hdcMem, MF_FLAG);
			}
		}
	}
}

void buttonLight(tagMouseState mouseState, HWND hWnd, HDC hdc, HDC hdcMem)
{
	if (mouseState.mCol >= 0 && mouseState.mCol < col && mouseState.mRow >= 0 && mouseState.mRow < row)
	{
		int no = mouseState.mCol + mouseState.mRow * col;
		if ((mapInfo[no] & 0x0F00) == 0x0000 && (mapInfo[no] & 0x00FF) % 3 == 1)
		{
			mapInfo[no]++;
			LightShadow(hdc, mouseState.mRow, mouseState.mCol, hdcMem, mapInfo[no] & 0x00FF, (mapInfo[no] & 0x0F00) / 0x0100);
		}
	}
}

void buttonChange(tagMouseState mouseState, HWND hWnd, HDC hdc, HDC hdcMem)
{
	if (mouseState.mCol >= 0 && mouseState.mCol < col && mouseState.mRow >= 0 && mouseState.mRow < row)
	{
		int no = mouseState.mCol + mouseState.mRow * col;
		if ((mapInfo[no] & 0x0F00) == 0x0000 && (mapInfo[no] & 0x00FF) % 3 != 0)
		{
			if ((mapInfo[no] & 0x00FF) / 3 == 1)
			{
				flagCount++;
				brickCount--;
			}
			else if ((mapInfo[no] & 0x00FF) / 3 == 2)
			{
				flagCount--;
				brickCount++;
			}
			mapInfo[no] = (mapInfo[no] & 0xF000) + ((mapInfo[no] & 0x00FF) / 3 * 3 + 5) % 9;
			if (!(optionFlag & OF_UNKNOW) && (mapInfo[no] & 0x00FF) / 3 == 0)
				mapInfo[no] = (mapInfo[no] & 0xF000) + (mapInfo[no] & 0x00FF) + 3;
			LightShadow(hdc, mouseState.mRow, mouseState.mCol, hdcMem, mapInfo[no] & 0x00FF, (mapInfo[no] & 0x0F00) / 0x0100);
			showMime(flagCount, hdc);//显示雷数
		}
	}
}
