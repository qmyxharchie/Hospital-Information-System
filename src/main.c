#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "login.h"
#include "registration.h"
#include "patient.h"
#include "doctor.h"
#include "file_io.h"


// 全局变量声明
char g_currentUsername[50] = {0};  // 当前登录用户名
int g_currentUserRole = -1;        // 当前用户角色
bool g_isLoggedIn = false;         // 是否已登录

// 声明全局链表头尾指针（在实际项目中这些应该在对应的头文件中声明）
User* g_userHead = NULL;
User* g_userTail = NULL;
Patient* g_patientHead = NULL;
Patient* g_patientTail = NULL;
Doctor* g_doctorHead = NULL;
Doctor* g_doctorTail = NULL;
Registration* g_registrationHead = NULL;
Registration* g_registrationTail = NULL;

// 函数声明
void showMainMenu(void);
void patientMenu(void);
void doctorMenu(void);
void adminMenu(void);

int main() {
    printf("=== 医疗信息管理系统 ===\n");
    

    
    // 加载所有数据
    buildUserChain(&g_userHead, &g_userTail);
    buildPatientChain(&g_patientHead, &g_patientTail);
    buildDoctorChain(&g_doctorHead, &g_doctorTail);
    buildRegistrationChain(&g_registrationHead, &g_registrationTail);
    
    printf("数据加载完成\n");
    
    // 主循环
    int choice;
    do {
        printf("\n=== 主菜单 ===\n");
        printf("1. 用户登录\n");
        printf("2. 用户注册\n");
        printf("0. 退出系统\n");
        printf("请选择: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: {
                char username[50], password[100];
                printf("请输入用户名: ");
                scanf("%s", username);
                printf("请输入密码: ");
                scanf("%s", password);
                
                LoginStatus status = login(username, password);
                
                if (status == LOGIN_SUCCESS_USER || status == LOGIN_SUCCESS_ADMIN) {
                    
                    // 显示主菜单
                    showMainMenu();
                    
                } else if (status == LOGIN_FAILED) {
                    printf("登录失败！用户名或密码错误。\n");
                }
                break;
            }
            case 2: {
                char username[50], password[100];
                int role;
                printf("请输入新用户名: ");
                scanf("%s", username);
                printf("请输入密码: ");
                scanf("%s", password);
                printf("请选择角色 (0-普通用户, 1-管理员): ");
                scanf("%d", &role);
                
                if (registerUser(username, password, role)) {
                    printf("注册成功！\n");
                } else {
                    printf("注册失败！\n");
                }
                break;
            }
            case 0:
                printf("感谢使用，再见！\n");
                break;
            default:
                printf("无效选择，请重新输入\n");
                break;
        }
    } while (choice != 0);
    

    
    return 0;
}

void showMainMenu(void) {
    if (!g_isLoggedIn) {
        printf("请先登录！\n");
        return;
    }
    
    if (g_currentUserRole == 0) {  // 普通用户（患者）
        patientMenu();
    } else {  // 管理员
        adminMenu();
    }
}

void patientMenu(void) {
    int choice;
    do {
        printf("\n=== 患者菜单 ===\n");
        printf("1. 预约挂号\n");
        printf("2. 查看我的挂号记录\n");
        printf("3. 取消我的挂号\n");
        printf("0. 返回主菜单\n");
        printf("请选择: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: {
                // 患者预约挂号的实现
                char patientCardNo[20], patientName[50];
                char doctorEmpNo[20], doctorName[50], dept[50];
                
                printf("请输入您的卡号: ");
                scanf("%s", patientCardNo);
                
                // 从患者链表中获取患者姓名
                Patient* patient = findPatientByCardNo(g_patientHead, patientCardNo);
                if(patient == NULL) {
                    printf("未找到对应患者信息\n");
                    break;
                }
                strcpy(patientName, patient->data.name);
                
                printf("请输入医生工号: ");
                scanf("%s", doctorEmpNo);
                
                // 从医生链表中获取医生信息
                Doctor* doctor = findDoctorByEmpNo(g_doctorHead, doctorEmpNo);
                if(doctor == NULL) {
                    printf("未找到对应医生\n");
                    break;
                }
                strcpy(doctorName, doctor->data.name);
                strcpy(dept, doctor->data.dept);
                
                addRegistration(&g_registrationHead, &g_registrationTail,
                              patientCardNo, patientName,
                              doctorEmpNo, doctorName, dept);
                break;
            }
            case 2: {
                // 查看患者自己的挂号记录
                Registration* reg = findRegistrationsByPatient(g_registrationHead, g_currentUsername);
                if (reg != NULL) {
                    printf("挂号编号: %s, 患者: %s, 医生: %s, 科室: %s, 日期: %s, 状态: %s\n",
                           reg->data.regNo, reg->data.patientName, 
                           reg->data.doctorName, reg->data.dept, 
                           reg->data.date, reg->data.status);
                } else {
                    printf("未找到您的挂号记录\n");
                }
                break;
            }
            case 0:
                printf("返回主菜单\n");
                break;
            default:
                printf("无效选择，请重新输入\n");
                break;
        }
    } while(choice != 0);
}

void adminMenu(void) {
    int choice;
    do {
        printf("\n=== 管理员菜单 ===\n");
        printf("1. 用户管理\n");
        printf("2. 患者管理\n");
        printf("3. 医生管理\n");
        printf("4. 挂号管理\n");
        printf("5. 查看所有挂号记录\n");
        printf("0. 返回主菜单\n");
        printf("请选择: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                // 用户管理功能
                printf("用户管理功能（待实现）\n");
                break;
            case 2:
                // 患者管理功能
                printf("患者管理功能（待实现）\n");
                break;
            case 3:
                // 医生管理功能
                printf("医生管理功能（待实现）\n");
                break;
            case 4:
                // 挂号管理功能
                printf("挂号管理功能（待实现）\n");
                break;
            case 5:
                // 查看所有挂号记录
                listAllRegistrations(g_registrationHead);
                break;
            case 0:
                printf("返回主菜单\n");
                break;
            default:
                printf("无效选择，请重新输入\n");
                break;
        }
    } while(choice != 0);
}