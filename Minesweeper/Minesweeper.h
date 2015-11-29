/**
 *	文件：Minesweeper.h
 *	Minesweeper头文件
 *
 *	Made by Luo Mingyuan
 *	Email: lmy0217@126.com
 *	谨此纪念娄芳老师
 *	2015-11-28
 */


#pragma once
#include "resource.h"


//*****************************************************************************
/*Basic Flag*/

//默认配置文件
#define PROFILE				TEXT(".\\Minesweeper.ini")

//默认雷区行列数及雷数
#define DEFAULT_ROW			9
#define DEFAULT_COL			9
#define DEFAULT_COUNT		10

//主窗口左上角坐标x,y
//两个-1表示屏幕居中
#define DEFAULT_WNDX		-1
#define DEFAULT_WNDY		-1

//放大倍数 = wndScale / DEFAULT_WNDSCALE
//放大倍数基数,默认贴图宽或高
#define DEFAULT_WNDSCALE	18

//计时器初始值
#define DEFAULT_TIMECOUNT	0

//默认雷区外背景区域宽或高
#define DEFAULT_GAP_TOP		30
#define DEFAULT_GAP_BOTTOM	32
#define DEFAULT_GAP_LEFT	30
#define DEFAULT_GAP_RIGHT	29

//主窗口宽
#define DEFAULT_XFRAME		((GetSystemMetrics(SM_CXFRAME) + 2) + GetSystemMetrics(SM_CXBORDER) * 2)
//主窗口高 = DEFAULT_YFRAME + DEFAULT_YTOP
#define DEFAULT_YFRAME		((GetSystemMetrics(SM_CYFRAME) + 2) + GetSystemMetrics(SM_CYBORDER) * 2)
#define DEFAULT_YTOP		(GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYMENU))

//默认错误值
#define DEFAULT_ERROR_INT	-2
#define DEFAULT_ERROR_STR	TEXT("")

//默认贴图宽高
#define INITIAL_BMWDITH		18
#define INITIAL_BMHEIGHT	18

//计时器ID
#define ID_TIMER			1
//*****************************************************************************


//*****************************************************************************
/*雷区块标志
//0xabcd
//a		雷标志
//		0-8		周围雷数
//		C		雷
//		F		未初始化
//bcd	块标志
//b		
//		0-7		Minesweeper图第b+1列
//cd			Minesweeper图第cd+1行*/

//Minesweeper图第1列
#define MF_UNKNOWDOWN		0x0000
#define MF_UNKNOW			0x0001
#define MF_LIGHTUNKNOW		0x0002
#define MF_BRICKDOWN		0x0003
#define MF_BRICK			0x0004
#define MF_LIGHTBRICK		0x0005
#define MF_FLAGDOWN			0x0006
#define MF_FLAG				0x0007
#define MF_LIGHTFLAG		0x0008

//Minesweeper图第2列
#define MF_ZERO				0x0100
#define MF_NUMBER(N)		(MF_ZERO + N)
#define MF_CROSSNUMBER(N)	(MF_ZERO + 0x0009 + N)

#define MF_NOMINE			0x0200
#define MF_MINEUNKNOW		0x0300
#define MF_MINEBRICK		0x0400
#define MF_MINEFLAG			0x0500
#define MF_REDMINEUNKNOW	0x0600
#define MF_REDMINEBRICK		0x0700

//雷标志0xa000
#define MF_ZEROMINE			0x0000
#define MF_MINENUMBER(N)	(MF_ZEROMINE + N * 0x1000)
#define MF_MINE				0xC000
#define MF_NOINIT			0xF000
#define MF_INIT				(MF_NOINIT | MF_BRICK)
//*****************************************************************************


//*****************************************************************************
/*设置对话框标志
//0x00-0xFF*/
#define OF_DRAW				0x80
#define OF_SOUND			0x40
#define OF_PROM				0x20
#define OF_STARTATSAVE		0x10
#define OF_SAVE				0x08
#define OF_UNKNOW			0x04
#define OF_USERDEFINED		0x03
#define OF_SENIOR			0x02
#define OF_MEDIATE			0x01
#define OF_JUNIOR			0x00
#define DEFAULT_OPTIONFLAG	(OF_DRAW | OF_SOUND | OF_PROM | OF_UNKNOW | OF_JUNIOR)
//*****************************************************************************


//*****************************************************************************
/*光影效果rgb值增量计算函数
//win7扫雷上截图, 每个块取一个最大R值、最大G值、最大B值, 然后根据行列值分别用三次函数三维拟合*/
#define DR(x, y)			(213.1031 - 4.1249*x + 1.0402*y - 0.0397*x*x - 0.6143*y*y - 0.0707*x*y + 0.0023*x*x*x + 0.0166*y*y*y + 0.0007*x*x*y + 0.0077*x*y*y - 113.6045)
#define DG(x, y)			(234.0942 + 0.3211*x + 7.4699*y - 0.2486*x*x - 1.0551*y*y - 0.4910*x*y + 0.0048*x*x*x + 0.0241*y*y*y + 0.0070*x*x*y + 0.0175*x*y*y - 154.3656)
#define DB(x, y)			(239.0961 + 0.8147*x + 2.2898*y - 0.0085*x*x - 0.1965*y*y - 0.1034*x*y - 0.000063*x*x*x + 0.0024*y*y*y + 0.0008*x*x*y + 0.0025*x*y*y - 215.1669)
//*****************************************************************************


//*****************************************************************************
/*全局变量*/

//鼠标状态结构体
struct tagMouseState {
	int mCol;		//位于雷区列数
	int mRow;		//位于雷区行数
	int mType;		/*鼠标按键类型
						WM_LBUTTONDOWN			左键按下
						WM_LBUTTONUP			左键弹起
						WM_RBUTTONDOWN			右键按下
						WM_RBUTTONUP			右键弹起
						WM_LBUTTONDBLCLK		双键按下开始
						WM_RBUTTONDBLCLK		双键按下结束
						WM_MOUSEMOVE			鼠标移动*/
} mouseState;

//雷区外背景区域宽高结构体
struct tagGap {
	int bottom;		//顶部高度
	int left;		//左边宽度
	int right;		//右边宽度
	int top;		//底部高度
} gap;

//bitmap句柄
//hBitmap			Minesweeper.bmp
//hBitmap_Frame		Frame.bmp
HBITMAP hBitmap, hBitmap_Frame;
BITMAP bitmap, bitmap_Frame;

LPVOID lpvBits;				//Minesweeper图rgb值数组
HBITMAP hBitmapT;			//块贴图句柄
unsigned int *pBrick;		//块贴图rgb值数组

int row;					//雷区行数
int col;					//雷区列数
int count;					//雷区雷数
int wndX;					//主窗口左上角坐标x
int wndY;					//主窗口左上角坐标y
int wndScale;				//块贴图宽或高 = 放大倍数 * DEFAULT_WNDSCALE
int optionFlag;				//设置对话框标志

//随机布雷
unsigned int seed;			//随机种子, 时间戳
int firstLButtonNo;			//第一次左键按下的块序号
int timeCount;				//计时器时间计数
TCHAR *saveFlags;			//保存的块贴图信息数组

int brickNumber;			//雷区块数
int wndWidth;				//主窗口宽
int wndHeight;				//主窗口高

unsigned short int *mapInfo;//雷区块标志数组

int flagCount;				//红旗计数
int brickCount;				//初始块计数

//基本标志
bool hasSave;				//是否存在保存的游戏
bool startAtSave;			//是否由保存的游戏开始
bool firstPaint;			//初始化效果是否已播放
bool firstLButtonDown;		//左键是否已第一次按下，游戏是否已开始
bool hasSpeed;				//效果延时控制是否进行中
bool timeFlag;				//用以控制计时器, 程序是否最小化

int tempOption[4];			//用以暂时保存雷区行列数雷数及设置对话框标志
//*****************************************************************************


/*
获得鼠标状态结构体
参数：
	mXpos			光标在客户区的水平坐标
	mYpos			光标在客户区的垂直坐标
	mType			鼠标按键类型
返回值：
	mouseState		鼠标状态结构体
*/
tagMouseState getMouseState(int mXpos, int mYpos, int mType);

/*
鼠标事件处理函数
参数：
	hWnd			主窗口句柄
	mouseState		鼠标状态结构体
	nIDMessage		自定义鼠标事件消息, 这里直接用系统的消息来代表自定义消息
					WM_LBUTTONDOWN			左键按下
					WM_LBUTTONUP			左键弹起
					WM_RBUTTONDOWN			右键按下
					WM_RBUTTONUP			右键弹起
					WM_LBUTTONDBLCLK		双键按下开始
					WM_RBUTTONDBLCLK		双键按下结束
					WM_MOUSEMOVE			鼠标移动
											WM_MOUSEMOVE			双键都未按下
											WM_LBUTTONDOWN			左键按下
											WM_LBUTTONDBLCLK		双键按下
返回值：
	true
*/
bool OnMouseClick(HWND hWnd, tagMouseState mouseState, int nIDMessage);


/*
初始化部分全局变量, 实现初始化动画效果
参数：
	tRow			新游戏雷区行数
	tCol			新游戏雷区列数
	tCount			新游戏雷区雷数
	uType			是否重新开始上盘的游戏
					0				新游戏
					1				重新开始上盘的游戏
	hWnd			主窗口句柄
	hdc				贴图目标句柄
	hdcMem			贴图源句柄，存有Minesweeper图
无返回值
*/
void InitInterface(int tRow, int tCol, int tCount, int uType, HWND hWnd, HDC hdc, HDC hdcMem);

/*
初始化雷区块标志数组
参数：
	tSeed			新游戏随机种子
	tFirstLButtonNo	新游戏第一次左键按下的块序号
	saveFlags		保存的块贴图信息数组
	hWnd			主窗口句柄
	hdc				贴图目标句柄
无返回值
*/
void InitMapInfo(unsigned int tSeed, int tFirstLButtonNo, TCHAR *saveFlags, HWND hWnd, HDC hdc);


/*
实现光影效果贴图
参数：
	hdcDest			贴图目标句柄
	rowDest			目标行序号
	colDest			目标列序号
	hdcSrc			贴图源句柄，存有Minesweeper图
	rowSrc			源行序号
	colSrc			源列序号
无返回值
*/
void LightShadow(HDC hdcDest, int rowDest, int colDest, HDC hdcSrc, int rowSrc, int colSrc);


/*
显示计时器
参数：
	timeCount		时间计数
	hdc				显示目标句柄
无返回值
*/
void showTime(int timeCount, HDC hdc);

/*
显示雷数
参数：
	flagCount		红旗计数
	hdc				显示目标句柄
无返回值
*/
void showMime(int flagCount, HDC hdc);

/*
检查游戏胜利
参数：
	hWnd			主窗口句柄
	hdc				显示目标句柄
无返回值
*/
void checkWin(HWND hWnd, HDC hdc);

/*
显示游戏失败
参数：
	hWnd			主窗口句柄
无返回值
*/
void showLose(HWND hWnd);


/*
点到雷数为零的块, 冲开周围的块
参数：
	rowDest			点到块的行序号
	colDest			点到块的列序号
	hWnd			主窗口句柄
	hdc				贴图目标句柄
	hdcMem			贴图源句柄，存有Minesweeper图
返回值：
	1+number		1+冲开的块数
*/
int zeroDevelop(int rowDest, int colDest, HWND hWnd, HDC hdc, HDC hdcMem);

/*
点到雷, 爆炸效果
参数：
	rowDest			点到块的行序号
	colDest			点到块的列序号
	hWnd			主窗口句柄
	hdc				贴图目标句柄
	hdcMem			贴图源句柄，存有Minesweeper图
无返回值
*/
void mineBomb(int rowDest, int colDest, HWND hWnd, HDC hdc, HDC hdcMem);

/*
控制各个动画效果贴图间的延时
参数：
	dwMilliseconds	效果贴图间的基本延时
	timeCount		延时控制数组
	startNo			控制数组起始序号
无返回值
*/
void speed(DWORD dwMilliseconds, int *timeCount, int startNo);


/*
块按下
参数：
	mouState		鼠标状态结构体
	hWnd			主窗口句柄
	hdc				贴图目标句柄
	hdcMem			贴图源句柄，存有Minesweeper图
无返回值
*/
void buttonDown(tagMouseState mouseState, HWND hWnd, HDC hdc, HDC hdcMem);

/*
九个块按下
参数：
	mouState		鼠标状态结构体
	hWnd			主窗口句柄
	hdc				贴图目标句柄
	hdcMem			贴图源句柄，存有Minesweeper图
无返回值
*/
void nineButtonDown(tagMouseState mouseState, HWND hWnd, HDC hdc, HDC hdcMem);

/*
块还原
参数：
	mouState		鼠标状态结构体
	hWnd			主窗口句柄
	hdc				贴图目标句柄
	hdcMem			贴图源句柄，存有Minesweeper图
无返回值
*/
void buttonRestore(tagMouseState mouseState, HWND hWnd, HDC hdc, HDC hdcMem);

/*
九个块还原
参数：
	mouState		鼠标状态结构体
	hWnd			主窗口句柄
	hdc				贴图目标句柄
	hdcMem			贴图源句柄，存有Minesweeper图
无返回值
*/
void nineButtonRestore(tagMouseState mouseState, HWND hWnd, HDC hdc, HDC hdcMem);

/*
块刷新, 由按下状态变为数字或雷
参数：
	mouState		鼠标状态结构体
	hWnd			主窗口句柄
	hdc				贴图目标句柄
	hdcMem			贴图源句柄，存有Minesweeper图
	redFlagUp		红旗贴图是否高亮
					MF_LIGHTFLAG			高亮
					MF_FLAG					不高亮
无返回值
*/
void buttonUp(tagMouseState mouseState, HWND hWnd, HDC hdc, HDC hdcMem, int redFlagUp);

/*
九个块刷新
参数：
mouState		鼠标状态结构体
hWnd			主窗口句柄
hdc				贴图目标句柄
hdcMem			贴图源句柄，存有Minesweeper图
无返回值
*/
void nineButtonUp(tagMouseState mouseState, HWND hWnd, HDC hdc, HDC hdcMem);

/*
块高亮
参数：
	mouState		鼠标状态结构体
	hWnd			主窗口句柄
	hdc				贴图目标句柄
	hdcMem			贴图源句柄，存有Minesweeper图
无返回值
*/
void buttonLight(tagMouseState mouseState, HWND hWnd, HDC hdc, HDC hdcMem);

/*
块改变
参数：
	mouState		鼠标状态结构体
	hWnd			主窗口句柄
	hdc				贴图目标句柄
	hdcMem			贴图源句柄，存有Minesweeper图
无返回值
*/
void buttonChange(tagMouseState mouseState, HWND hWnd, HDC hdc, HDC hdcMem);
