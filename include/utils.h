//=============
//	工具函数
//=============

/*工具函数是项目中所有模块共享的底层辅助函数，包括：
- 时间处理
- 字符串处理
- 输入验证
- 全局时间状态*/

#ifndef _UTILS_H																			
#define _UTILS_H	

extern int g_currentYear;										//系统当前年份
extern int g_currentMonth;										//系统当前月份
extern int g_currentDay;										//系统当前日期

void getCurrentTime(int* year, int* month, int* day);			//获取当前时间
void generateUniqueId(const char* prefix, char* id);			//生成唯一ID
/*   生成ID格式为prefix前缀（例如PT、DOC)+年月日时分秒+2位静态计数器   */

int compareDate(int year1, int month1, int day1,
	int year2, int month2, int day2);							//比较两个日期
/*   逐级比较，相同返回0，1<2返回负数，否则返回正数   */

void safeStringCopy(char* dest, const char* src, int maxLen);	//安全字符串复制
/*   将src中的字符串复制到dest，字符串最大长度为maxLen   */

void trim(char* str);											//去除两端空白字符
/*   去掉头尾的空格   */
int compareDateStr(char* date1, char* date2);					//比较两个 YYYY-MM-DD 格式的日期字符串
int stringToInt(const char* str);								//字符串转整数
double stringToDouble(const char* str);							//字符串转浮点数
bool isValidPhone(const char* phone);							//11位手机号验证
bool isValidIdCard(const char* idCard);							//18位身份证号验证
bool isValidDate(int year, int month, int day);					//日期有效性验证
void pause(const char* message);								//提示与验证

int strDisplayWidth(const char* s);								//UTF-8 字符串的显示宽度（ASCII=1格，其他=2格）
void printPadded(const char* s, int targetWidth);				//左对齐打印字符串到指定显示宽度

#endif
