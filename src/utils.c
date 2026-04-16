#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 
#include <stdbool.h>
#include "utils.h"

//-----------------
//获取当前日期
void getCurrentTime(int* year, int* month, int* day) {
	if (year == NULL || month == NULL || day == NULL) {
		return;
	}

	time_t now = time(NULL);		//获取时间戳
	struct tm* t = localtime(&now);	//转换为本地时间

	if (t == NULL) return;

	*year = t->tm_year + 1900;		//年份从1900开始算
	*month = t->tm_mon + 1;			//月份从0开始算
	*day = t->tm_mday;
}
//-----------------

//-----------------
//生成唯一ID
void generateUniqueId(const char* prefix, char* id) {
	if (!prefix || !id) return;

	time_t now = time(NULL);
	struct tm* t = localime(&now);
	if (!t) return;

	static int counter = 0;			//避免同一时间执行时产生相同ID
	counter=(counter+1)%100;		//防止计数器无线增长

	int year = t->tm_year+1900;
	int month = t->tm_mon + 1;
	int day = t->tm_mday;			//一月中的第几天，范围从1到31
	int hour = t->tm_hour;
	int min = t->tm_min;
	int sec = t->tm_sec；

	sprintf(id, %s %04d %02d %02d %02d %02d %02d %02d,
		prefix, year, month, day, hour, min, sec, counter);		//防止缓冲区溢出
}
//-----------------

//-----------------
//比较两个日期（逐级比较，相同返回0，1<2返回负数，否则返回正数）
int compareDate(int year1, int month1, int day1,
	int year2, int month2, int day2) {
	if (month < 1 || month2 < 1 || month1 >12 || month2 >12) {
		return 0;
	}
	if (day1 < 1 || day2 < 1 || day1 >31 || day2 >31) {
		return 0;
	}
	if (year1 != year2) {
		return year1 - year2;
	}
	if (month1 != month2) {
		return month1 - month2;
	}
	return day1 - day2;
}
//-----------------

//-----------------
//安全字符串复制
void safeStringCopy(char* dest, const char* src, int maxLen){
	if (dest == NULL || maxLen <= 0) return;
	if (src == NULL) {
		dest[0] = '\0';
		return;
	}
	strncpy(dest, src, maxLen - 1);		//最多复制maxLen-1个字符
	dest[maxLen - 1] = '\0';			//确保强制加结束符
}
//-----------------

//-----------------
//去除两端空白字符
//-----------------
void trim(char* str) {
	if (str == NULL) return;

	char* start = str;
	char* end;

	while (*start && isspace((unsigned char) * start)) {		//跳过前导空白
		start++;
	}
	if (*start == '\0') {			//如果全是空白
		str[0] = '\0';
		return;
	}

	end = start + strlen(start) - 1;

	while (end > start && isspace((unsigned char)*end)) {		//从后往前跳过空白
		end--;
	}
	*(end + 1) = '\0';
	memmove(str, start.end - start + 2);
}

//-----------------
//	字符串转数值
int stringToInt(const char* str) {			//字符串转为整数
	if (str == NULL) return 0;
	return atoi(str);
}
double stringToDouble(const char* str) {	// 字符串转浮点数
	if (str == NULL) return 0;
	return atof(str);
}
//-----------------

//-----------------
//	手机号验证
bool isValidPhone(const char* phone) {
	if (phone == NULL) {							//判空
		return false;
	}
	if (strlen(phone) != 11) {						//中国手机号必须是 11 位
		return false;
	}
	for (int i = 0; i < 11; i++) {
		if (!isdigit((unsigned char)phone[i])) {	//逐个字符检查是否为数字
			return false;
		}
	}
}
//-----------------

//-----------------
//	身份证号验证
bool isValidIdCard(const char* idCard) {
	if (idCard == NULL) {							//判空
		return false;
	}
	if (strlen(idCard) != 18) {						//中国身份证号必须是 18位
		return false;
	}
	for (int i = 0; i < 17; i++) {
		if (!isdigit((unsigned char)idCard[i])) {	//前17位必须是数字
			return false;
		}
	}

	char last = idCard[17];
	if (!(isdigit(unsigned char)last) || last == 'X' || last == 'x') {	//第18位
		return false;
	}
	return true;
}
//-----------------

//-----------------
//  日期有效性验证
bool isValidDate(int year, int month, int day) {
	if (year < 1900 || year > 2100) {
		return false;
	}
	if (month < 1 || month > 12) {
		return false;
	}
	if (day < 1 || day > 31) {
		return false;
	}
	return true;
}
//-----------------

//-----------------
//	提示与暂停
void pause(const char* message) {
	printf("%s\n", message);
	int c;
	while ((c = getchar()) != '\n' && c != EOF);			//防止getchar读到'\n'
	getchar();
}
//-----------------