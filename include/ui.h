#ifndef _UI_H_
#define _UI_H_


#include "patient.h"
#include "doctor.h"
#include "registration.h"
#include "medicine.h"
#include "hospitalization.h"
#include "bed.h"

//全域链表指针定义
extern Patient* g_patientHead;
extern Patient* g_patientTail;
extern Doctor* g_doctorHead;
extern Doctor* g_doctorTail;
extern Registration* g_regHead;
extern Registration* g_regTail;
extern Medicine* g_medHead;
extern Medicine* g_medTail;
extern Purchase* g_purHead;
extern Purchase* g_purTail;
extern Hospitalization* g_hosHead;
extern Hospitalization* g_hosTail;
extern Bed* g_bedHead;
extern Bed* g_bedTail;
extern User* g_userHead;
extern User* g_userTail;

//函数声明
int safeReadInt(const char* prompt, int minVal, int maxVal);//安全读取int类型变量
double safeReadDouble(const char* prompt);//安全读取double类型函数
void safeReadString(const char* prompt, char* buf, int maxLen);//安全读取字符串（带长度限制）
void initUI(void);  //初始化界面
int showLoginPage(void);//显示登录状态
//* 0 = LOGIN_FAILED      （登录失败）
//* 1 = LOGIN_SUCCESS_USER  （普通用户登录成功）
//* 2 = LOGIN_SUCCESS_ADMIN （管理员登录成功）
//* 3 = LOGIN_EXIT         （退出系统）
int showMainMenuByRole(int userRole, char* username);//按照不同身份显示菜单
void showPatientManagement(void);//病人管理菜单
void showDoctorManagement(void);//医生管理菜单
void showRegistrationManagement(void);//挂号管理菜单
void showMedicineManagement(void);//药品管理菜单
void showHospitalizationManagement(void);//住院管理菜单
void showBedManagement(void);//床位管理菜单
void showStatisticsMenu(void);//统计报表菜单
void showQueryMenu(void);//查询菜单


#endif