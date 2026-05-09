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

//颜色定义
#define COLOR_BG           RGB(240, 248, 255)  // 背景色（AliceBlue）
#define COLOR_TITLE        RGB(70, 130, 180)    // 标题色（SteelBlue）
#define COLOR_TEXT         RGB(50, 50, 50)      // 文本色（深灰色）
#define COLOR_BUTTON       RGB(100, 149, 237)  // 按钮色（CornflowerBlue）
#define COLOR_BUTTON_HOVER RGB(70, 130, 180)   // 按钮悬停色
#define COLOR_SUCCESS      RGB(34, 139, 34)    // 成功色（Green）
#define COLOR_WARNING      RGB(255, 140, 0)    // 警告色（DarkOrange）
#define COLOR_ERROR        RGB(220, 20, 60)    // 错误色（Crimson）

//尺寸定义
#define WINDOW_WIDTH   1000   // 窗口宽度
#define WINDOW_HEIGHT  700    // 窗口高度
#define BUTTON_WIDTH   200    // 按钮宽度
#define BUTTON_HEIGHT  50     // 按钮高度
#define INPUT_WIDTH    300    // 输入框宽度
#define INPUT_HEIGHT   40     // 输入框高度

//函数声明
int safeReadInt( int minVal, int maxVal);//安全读取int类型变量
double safeReadDouble(void);//安全读取double类型函数
void safeReadString(char* buf, int maxLen);//安全读取字符串（带长度限制）
void initUI(void);  //初始化界面
void closeUI(void); //关闭界面释放资源
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
/**
 * @brief 消息提示框
 * @param title   标题
 * @param message 提示内容
 */
void showMessage(const char* title, const char* message);
/**
 * @brief 确认对话框
 * @return true = 确认(y), false = 取消(n)
 */
bool confirmDialog(const char* title, const char* message);


#endif