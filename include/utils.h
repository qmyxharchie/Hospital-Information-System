// utils.h
#ifndef UTILS_H																				//如果没有utils.h
#define UTILS_H																				//创建
extern int g_currentYear;																	//系统当前年份
extern int g_currentMonth；																	//系统当前月份
extern int g_currentDay;																	//系统当前日期
void getCurrentTime(int* year, int* month, int* day);										//获取当前时间
void generateUniqueId(const char* prefix, char* id);										//生成唯一ID
int compareDate(int year1, int month1, int day1,int year2, int month2, int day2);			//比较两个日期
void safeStringCopy(char* dest, const char* src, int maxLen);								//安全字符串复制
void trim(char* str);																		//去除两端空白字符
int stringToInt(const char* str);															//字符串转整数
double stringToDouble(const char* str);														//字符串转浮点数
bool isValidPhone(const char* phone);														//手机号验证
bool isValidIdCard(const char* idCard);														//身份证号验证
bool isValidDate(int year, int month, int day);												//日期有效性验证
void pause(const char* message);															//提示与验证
#endif