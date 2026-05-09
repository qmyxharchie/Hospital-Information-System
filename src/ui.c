#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ui.h"

// ============================================================
// 全局链表指针定义（唯一定义点，所有模块共享）
// ============================================================
Patient* g_patientHead = NULL;
Patient* g_patientTail = NULL;
Doctor* g_doctorHead = NULL;
Doctor* g_doctorTail = NULL;
Registration* g_regHead = NULL;
Registration* g_regTail = NULL;
Medicine* g_medHead = NULL;
Medicine* g_medTail = NULL;
Purchase* g_purHead = NULL;
Purchase* g_purTail = NULL;
Hospitalization* g_hosHead = NULL;
Hospitalization* g_hosTail = NULL;
Bed* g_bedHead = NULL;
Bed* g_bedTail = NULL;
User* g_userHead = NULL;
User* g_userTail = NULL;

// 登录相关全局
char g_currentUsername[50] = "";
UserRole g_currentUserRole = PATIENT;
bool g_isLoggedIn = false;

/* 安全读取 int，带范围校验和错误清缓冲 */
void initUI(void){}
int safeReadInt(const char* prompt, int minVal, int maxVal) {
    int choice;
    char buf[100];
    while (true) {
        printf("%s", prompt);
        if (scanf("%d", &choice) != 1) {
            fgets(buf, sizeof(buf), stdin);
            printf("[ERROR] 输入无效，请输入数字！\n");
            continue;
        }
        getchar();
        if (choice < minVal || choice > maxVal) {
            printf("[ERROR] 请输入 %d~%d 之间的数字\n", minVal, maxVal);
            continue;
        }
        return choice;
    }
}

/* 安全读取 double */
double safeReadDouble(const char* prompt) {
    double val;
    char buf[100];
    while (true) {
        printf("%s", prompt);
        if (scanf("%lf", &val) != 1) {
            fgets(buf, sizeof(buf), stdin);
            printf("[ERROR] 输入无效，请输入数字！\n");
            continue;
        }
        getchar();
        return val;
    }
}

/* 安全读取字符串 */
void safeReadString(const char* prompt, char* buf, int maxLen) {
    printf("%s", prompt);
    if (scanf("%99s", buf) == 1) {
        if ((int)strlen(buf) >= maxLen) {
            printf("[WARNING] 输入过长，已截断至 %d 字符\n", maxLen - 1);
            buf[maxLen - 1] = '\0';
        }
    }
    getchar();
}


//--------------------
// 以下为登录界面函数
//--------------------
int showLoginPage(void) {
    char username[50] = { 0 };
    char password[50] = { 0 };

    printf("\n╔═══════════════════════════════════════════════════╗\n");
    printf("║       医院综合信息管理系统 - 登录界面           ║\n");
    printf("╠═══════════════════════════════════════════════════╣\n");
    printf("║  1. 用户登录                                     ║\n");
    printf("║  2. 管理员登录                                   ║\n");
    printf("║  3. 用户注册                                     ║\n");
    printf("║  0. 退出系统                                     ║\n");
    printf("╚═══════════════════════════════════════════════════╝\n");

  
    int choice = safeReadInt("请选择: ", 0, 3);

    switch (choice) {
    case 1:
        printf("\n请输入用户名: "); scanf("%49s", username);
        printf("请输入密码: ");     scanf("%49s", password);
        return login(username, password);
    case 2:
        printf("\n请输入管理员用户名: "); scanf("%49s", username);
        printf("请输入密码: ");            scanf("%49s", password);
        return login(username, password);
    case 3:
        printf("\n请输入要注册的用户名: "); scanf("%49s", username);
        printf("请输入密码: ");             scanf("%49s", password);
        registerUser(username, password, 0);
        return LOGIN_FAILED;
    case 0:
        return LOGIN_EXIT;
    default:
        printf("[ERROR] 无效选择！\n");
        return LOGIN_FAILED;
    }
}


//--------------------
//主菜单显示
int showMainMenuByRole(int userRole, char* username) {
    printf("\n╔═════════════════════════════════════════╗\n");
    printf("║ 医院综合信息管理系统 - 主菜单           ║\n");
    printf("║ 当前用户: %-15s 角色: ", username);
    switch (userRole) {
    case 0: printf("患者"); break;
    case 1: printf("护士"); break;
    case 2: printf("医生"); break;
    case 3: printf("管理员"); break;
    default: printf("未知"); break;
    }
    printf("    ║\n");
    printf("╠═════════════════════════════════════════╣\n");

    if (userRole >= 0) {
        printf("║ 1. 个人信息管理                         ║\n");
        printf("║ 2. 挂号预约                             ║\n");
    }
    if (userRole >= 1) {
        printf("║ 3. 患者信息管理                         ║\n");
        printf("║ 4. 床位管理                             ║\n");
    }
    if (userRole >= 2) {
        printf("║ 5. 医生信息管理                         ║\n");
        printf("║ 6. 药品管理                             ║\n");
        printf("║ 7. 住院管理                             ║\n");
    }
    if (userRole >= 3) {
        printf("║ 8. 统计报表                             ║\n");
        printf("║ 9. 系统管理                             ║\n");
    }
    printf("║ 0. 退出系统                             ║\n");
    printf("╚═════════════════════════════════════════╝\n");

    int maxOption = 0;
    if (userRole >= 3) maxOption = 9;
    else if (userRole >= 2) maxOption = 7;
    else if (userRole >= 1) maxOption = 4;
    else maxOption = 2;
    return safeReadInt("请选择功能模块: ", 0, maxOption);
}


//--------------------
// 以下为病人管理菜单显示函数
//--------------------
void showPatientManagement(void) {
    int choice;
    char name[50], gender[10], idCard[20], phone[15], cardNo[20];
    int age;
    Patient* p;
    while (1) {
        printf("\n-------- 病人信息管理 --------\n");
        printf("1. 添加病人   4. 查找病人\n");
        printf("2. 删除病人   5. 显示所有病人\n");
        printf("3. 修改病人信息\n");
        printf("0. 返回上级菜单\n");

        choice = safeReadInt("请选择: ", 0, 5);

        switch (choice) {
        case 1:
            printf("\n请输入病人信息:");
            printf("\n姓名: ");     scanf("%49s", name);

            age = safeReadInt("\n年龄: ", 0, 150);
            printf("\n性别: ");     scanf("%9s", gender);
            printf("\n身份证号: "); scanf("%19s", idCard);
            printf("\n联系电话: "); scanf("%14s", phone);
            addPatient(&g_patientHead, &g_patientTail,
                       name, age, gender, idCard, phone);
            break;
        case 2:
            safeReadString("请输入要删除的病人卡号: ", cardNo, 20);
            p = findPatientByCardNo(g_patientHead, cardNo);
            if (p) {
                delPatient(&g_patientHead, &g_patientTail, p->data);
            } else {
                printf("[ERROR] 未找到该病人！\n");
            }
            break;
        case 3: {
            PatientData newData;
            printf("\n请输入要修改的病人卡号："); scanf("%19s", cardNo);
            printf("\n请输入修改后的病人信息:");
            printf("\n姓名: ");     scanf("%49s", newData.name);
            newData.age = safeReadInt("\n年龄: ", 0, 150);
            printf("\n性别: ");     scanf("%9s", newData.gender);
            printf("\n身份证号: "); scanf("%19s", newData.idCard);
            printf("\n联系电话: "); scanf("%14s", newData.phone);
            modifyPatient(g_patientHead, cardNo, newData);
            break;
        }
        case 4: {
            printf("请选择查找方式：\n");
            printf("1、按照卡号查找\n");
            printf("2、按照姓名查找\n");
            int choice1 = safeReadInt("请选择: ", 1, 2);
            switch (choice1) {
            case 1:
                safeReadString("请输入卡号：", cardNo, 20);
                p = findPatientByCardNo(g_patientHead, cardNo);
                if (p == NULL) {
                    printf("[ERROR] 未找到病人信息\n");
                } else {
                    printf("[OK] 病人信息如下\n");
                    printf("%-20s %-50s %-10s %-10s %-20s %-15s %-10s\n",
                           "卡号", "姓名", "年龄", "性别", "身份证", "电话", "住院状态");
                    printf("%-20s %-50s %-10d %-10s %-20s %-15s %-10s\n",
                           p->data.cardNo, p->data.name, p->data.age,
                           p->data.gender, p->data.idCard, p->data.phone,
                           p->data.isActive ? "住院" : "非住院");
                }
                break;
            case 2:
                safeReadString("请输入姓名：", name, 50);
                p = findPatientsByName(g_patientHead, name);
                if (p == NULL) {
                    printf("[ERROR] 未找到病人信息\n");
                } else {
                    printf("[OK] 病人信息如下\n");
                    printf("%-20s %-50s %-10s %-10s %-20s %-15s %-10s\n",
                           "卡号", "姓名", "年龄", "性别", "身份证", "电话", "住院状态");
                    printf("%-20s %-50s %-10d %-10s %-20s %-15s %-10s\n",
                           p->data.cardNo, p->data.name, p->data.age,
                           p->data.gender, p->data.idCard, p->data.phone,
                           p->data.isActive ? "住院" : "非住院");
                }
                break;
            }
            break;
        }
        case 5:
            listAllPatients(g_patientHead);
            break;
        case 0:
            return;
        default:
            printf("[ERROR] 无效选择！\n");
        }
        system("pause");
        system("cls");
    }
}


//--------------------
// 以下为医生管理菜单显示函数
//--------------------
void showDoctorManagement(void) {
    int choice;
    char name[50], dept[50], schedule[100], empNo[20];
    int maxPatients;
    Doctor* d;
    while (1) {
        printf("\n-------- 医生信息管理 --------\n");
        printf("1. 添加医生   4. 查找医生\n");
        printf("2. 删除医生   5. 显示所有医生\n");
        printf("3. 修改医生信息\n");
        printf("0. 返回上级菜单\n");
        choice = safeReadInt("请选择: ", 0, 5);

        switch (choice) {
        case 1:
            printf("\n请输入医生信息:");
            printf("\n姓名: ");      scanf("%49s", name);
            printf("\n科室: ");      scanf("%49s", dept);
            printf("\n出诊时间: ");  scanf("%99s", schedule);
            maxPatients = safeReadInt("\n每日最大接诊数: ", 1, 999);
            addDoctor(&g_doctorHead, &g_doctorTail,
                      name, dept, schedule, maxPatients);
            printf("[OK] 医生添加成功！\n");
            break;
        case 2:
            safeReadString("请输入要删除的医生工号：", empNo, 20);
            d = findDoctorByEmpNo(g_doctorHead, empNo);
            if (d) {
                delDoctor(&g_doctorHead, &g_doctorTail, d->data);
            } else {
                printf("[ERROR] 未找到该医生！\n");
            }
            break;

        case 3: {
            DoctorData newData;
            safeReadString("\n请输入要修改的医生工号: ", empNo, 20);
            printf("\n请输入医生信息:");
            printf("\n姓名: ");      scanf("%49s", newData.name);
            printf("\n科室: ");      scanf("%49s", newData.dept);
            printf("\n出诊时间: ");  scanf("%99s", newData.schedule);
            newData.maxPatients = safeReadInt("\n每日最大接诊数: ", 1, 999);
            modifyDoctor(g_doctorHead, empNo, newData);
            break;
        }
        case 4: {
            printf("请选择查找方式：\n");
            printf("1、按照工号查找\n");
            printf("2、按照姓名查找\n");
            printf("3、按照科室查找\n");
            int choice2 = safeReadInt("请选择: ", 1, 3);
            switch (choice2) {
            case 1:
                safeReadString("请输入要查找医生的工号: ", empNo, 20);
                d = findDoctorByEmpNo(g_doctorHead, empNo);
                if (d == NULL) {
                    printf("[ERROR] 未找到医生信息\n");
                } else {
                    printf("[OK] 医生信息如下\n");
                    printf("%-20s %-50s %-50s %-100s %-10s %-10s\n",
                           "工号", "姓名", "科室", "出诊时间", "每日最大接诊数", "今日已接诊数");
                    printf("%-20s %-50s %-50s %-100s %-10d %-10d\n",
                           d->data.empNo, d->data.name, d->data.dept,
                           d->data.schedule, d->data.maxPatients,
                           d->data.currentPatients);
                }
                break;

            case 2:
                safeReadString("请输入要查找医生的姓名: ", name, 50);
                d = findDoctorsByName(g_doctorHead, name);
                if (d == NULL) {
                    printf("[ERROR] 未找到医生信息\n");
                } else {
                    printf("[OK] 医生信息如下\n");
                    listAllDoctors(d);
                    /* 注意：findDoctorsByName 返回新链表，需释放 */
                    Doctor* cur = d;
                    while (cur) { Doctor* tmp = cur; cur = cur->next; free(tmp); }
                }
                break;
            case 3:
                safeReadString("请输入要查找医生的科室: ", dept, 50);
                d = findDoctorsByDept(g_doctorHead, dept);
                if (d) {
                    printf("%-20s %-50s %-50s %-100s %-10s %-10s\n",
                           "工号", "姓名", "科室", "出诊时间", "每日最大接诊数", "今日已接诊数");
                    printf("%-20s %-50s %-50s %-100s %-10d %-10d\n",
                           d->data.empNo, d->data.name, d->data.dept,
                           d->data.schedule, d->data.maxPatients,
                           d->data.currentPatients);
                } else {
                    printf("[ERROR] 未找到该科室的医生！\n");
                }
                break;
            default:
                printf("[ERROR] 无效选择！");
            }
            break;
        }
        case 5:
            listAllDoctors(g_doctorHead);
            break;
        case 0:
            return;
        default:
            printf("[ERROR] 无效选择！");
            break;
        }
    }
}



//--------------------
// 以下为药品管理菜单显示函数
//--------------------
void showMedicineManagement(void) {
    int choice;
    char medNo[20], name[50], patientCardNo[20], date[20];
    int quantity;
    double totalCost;
    Medicine* m;
    while (1) {
        printf("\n-------- 药品管理 --------\n");
        printf("1. 按编号查询   4. 购药登记\n");
        printf("2. 按名称查询   5. 显示所有药品\n");
        printf("3. 补充库存     6. 库存预警检查\n");
        printf("0. 返回上级菜单\n");

        choice = safeReadInt("请选择: ", 0, 6);

        switch (choice) {
        case 1:
            safeReadString("请输入药品编号: ", medNo, 20);
            m = findMedicineByNo(g_medHead, medNo);
            if (m) {
                printf("%-10s %-20s %-20s %-15s %-10s %-10s %-10s\n",
                       "编号", "通用名", "商品名", "规格", "单价", "库存", "最低库存");
                printf("%-10s %-20s %-20s %-15s %-10.2f %-10d %-10d\n",
                       m->data.medNo, m->data.genericName, m->data.brandName,
                       m->data.spec, m->data.price, m->data.stock, m->data.minStock);
            } else {
                printf("[ERROR] 未找到该药品！\n");
            }
            break;
        case 2:
            safeReadString("请输入药品名称: ", name, 50);
            m = findMedicineByName(g_medHead, name);
            if (m) {
                printf("%-10s %-20s %-20s %-15s %-10s %-10s %-10s\n",
                       "编号", "通用名", "商品名", "规格", "单价", "库存", "最低库存");
                printf("%-10s %-20s %-20s %-15s %-10.2f %-10d %-10d\n",
                       m->data.medNo, m->data.genericName, m->data.brandName,
                       m->data.spec, m->data.price, m->data.stock, m->data.minStock);
            } else {
                printf("[ERROR] 未找到该药品！\n");
            }
            break;
        case 3: {
            safeReadString("请输入药品编号: ", medNo, 20);
            quantity = safeReadInt("请输入补充数量: ", 1, 99999);
            m = findMedicineByNo(g_medHead, medNo);
            if (m) {
                replenishStock(g_medHead, medNo, quantity);
                printf("[OK] 库存补充成功！当前库存: %d\n", m->data.stock);
            } else {
                printf("[ERROR] 未找到该药品！\n");
            }
            break;
        }
        case 4: {
            printf("\n请输入病人卡号: ");     scanf("%19s", patientCardNo);
            printf("\n请输入药品编号: ");     scanf("%19s", medNo);
            quantity = safeReadInt("\n请输入购买数量: ", 1, 99999);
            totalCost = safeReadDouble("\n请输入总费用: ");
            printf("\n请输入购药日期(YYYY-MM-DD): "); scanf("%19s", date);
            m = findMedicineByNo(g_medHead, medNo);
            if (m && m->data.stock >= quantity) {
                addPurchaseRecord(&g_purHead, &g_purTail,
                                  patientCardNo, medNo, quantity, totalCost, date);
                printf("[OK] 购药登记成功！\n");
            } else {
                printf("[ERROR] 药品不存在或库存不足！\n");
            }
            break;
        }
        case 5:
            listAllMedicines(g_medHead);
            break;
        case 6:
            checkLowStock(g_medHead);
            break;
        case 0:
            return;
        default:
            printf("[ERROR] 无效选择！\n");
        }
    }
}


//--------------------
// 以下为住院管理菜单函数
//--------------------
void showHospitalizationManagement(void) {
    int choice;
    char recordNo[20], patientCardNo[20], patientName[50];
    char bedNo[20];
    double prepay, totalCost;
    Hospitalization* h;
    while (1) {
        printf("\n-------- 住院管理 --------\n");
        printf("1. 入院登记        5. 显示当前在院病人\n");
        printf("2. 出院结算        6. 显示所有住院记录\n");
        printf("3. 按卡号查询      7. 追加预交金\n");
        printf("4. 按住院单号查询  8. 修改住院信息（转床/追加押金）\n");
        printf("0. 返回上级菜单\n");

        choice = safeReadInt("请选择: ", 0, 8);

        switch (choice) {
        case 1:
            printf("\n请输入入院信息:");
            printf("\n病人卡号: "); scanf("%19s", patientCardNo);
            printf("\n病人姓名: "); scanf("%49s", patientName);
            prepay = safeReadDouble("\n预交金额: ");
            addHospitalization(&g_hosHead, &g_hosTail,
                               patientCardNo, patientName, prepay);
            break;
        case 2: {
            safeReadString("请输入住院单号: ", recordNo, 20);
            h = findHospitalizationByNo(g_hosHead, recordNo);
            if (h) {
                printf("当前状态: %s，预交金额: %.2f\n",
                       h->data.status, h->data.prepay);
                totalCost = safeReadDouble("请输入总费用: ");
                dischargePatient(h, totalCost);
            } else {
                printf("[ERROR] 未找到该住院记录！\n");
            }
            break;
        }
        case 3:
            safeReadString("请输入病人卡号: ", patientCardNo, 20);
            h = findHospitalizationByCardNo(g_hosHead, patientCardNo);
            if (h) {
                printf("%-12s %-12s %-10s %-10s %-10s %-10s %-10s\n",
                       "住院单号", "姓名", "床位号", "预交金", "总费用", "入院日期", "状态");
                printf("%-12s %-12s %-10s %-10.2f %-10.2f %-10s %-10s\n",
                       h->data.recordNo, h->data.patientName, h->data.bedNo,
                       h->data.prepay, h->data.totalCost,
                       h->data.admissionDate, h->data.status);
            } else {
                printf("[ERROR] 未找到该病人的住院记录！\n");
            }
            break;
        case 4:
            safeReadString("请输入住院单号: ", recordNo, 20);
            h = findHospitalizationByNo(g_hosHead, recordNo);
            if (h) {
                printf("%-12s %-12s %-10s %-10s %-10s %-10s %-10s\n",
                       "住院单号", "姓名", "床位号", "预交金", "总费用", "入院日期", "状态");
                printf("%-12s %-12s %-10s %-10.2f %-10.2f %-10s %-10s\n",
                       h->data.recordNo, h->data.patientName, h->data.bedNo,
                       h->data.prepay, h->data.totalCost,
                       h->data.admissionDate, h->data.status);
            } else {
                printf("[ERROR] 未找到该住院记录！\n");
            }
            break;
        case 5:
            h = findCurrentHospitalizations(g_hosHead);
            if (h) {
                listAllHospitalizations(h);
                freeHospitalizationResultChain(h);
            } else {
                printf("当前无在院病人。\n");
            }
            break;
        case 6:
            listAllHospitalizations(g_hosHead);
            break;
        case 7: {
            safeReadString("请输入住院单号: ", recordNo, 20);
            prepay = safeReadDouble("请输入追加金额: ");
            h = findHospitalizationByNo(g_hosHead, recordNo);
            if (h) {
                addPrepay(h, prepay);
            } else {
                printf("[ERROR] 未找到该住院记录！\n");
            }
            break;
        }
        case 8: {
            safeReadString("请输入住院单号: ", recordNo, 20);
            safeReadString("请输入新床位号(不转床输0): ", bedNo, 20);
            prepay = safeReadDouble("请输入追加预交金(不追加输0): ");
            if (strcmp(bedNo, "0") == 0) {
                strcpy(bedNo, "");
            }
            modifyHospitalization(g_hosHead, recordNo, bedNo, prepay);
            break;
        }
        case 0:
            return;
        default:
            printf("[ERROR] 无效选择！\n");
        }
    }
}


//--------------------
// 以下是床位管理菜单函数
//--------------------
void showBedManagement(void) {
    int choice;
    char ward[30], bedNo[20];
    int total, occupied;
    Bed* b;
    while (1) {
        printf("\n-------- 床位管理 --------\n");
        printf("1. 添加床位     5. 查找空闲床位\n");
        printf("2. 删除床位     6. 病区统计\n");
        printf("3. 按床位号查询 7. 显示所有床位\n");
        printf("4. 按病区查询   0. 返回上级菜单\n");

        choice = safeReadInt("请选择: ", 0, 7);

        switch (choice) {
        case 1:
            printf("\n请输入床位信息:\n");
            safeReadString("病区: ", ward, 30);
            safeReadString("床位号: ", bedNo, 20);
            addBed(&g_bedHead, &g_bedTail, ward, bedNo);
            printf("[OK] 床位添加成功！\n");
            break;
        case 2:
            safeReadString("请输入要删除的床位号: ", bedNo, 20);
            b = findBedByNo(g_bedHead, bedNo);
            if (b) {
                delBed(&g_bedHead, &g_bedTail, b);
                printf("[OK] 床位已删除！\n");
            } else {
                printf("[ERROR] 未找到该床位！\n");
            }
            break;
        case 3:
            safeReadString("请输入床位号: ", bedNo, 20);
            b = findBedByNo(g_bedHead, bedNo);
            if (b) {
                printf("%-10s %-10s %-12s %-10s %-10s\n",
                       "病区", "床位号", "病人卡号", "病人姓名", "状态");
                printf("%-10s %-10s %-12s %-10s %-10s\n",
                       b->data.ward, b->data.bedNo, b->data.patientCardNo,
                       b->data.patientName, b->data.status);
            } else {
                printf("[ERROR] 未找到该床位！\n");
            }
            break;
        case 4:
            safeReadString("请输入病区: ", ward, 30);
            listBedsByWard(g_bedHead, ward);
            break;
        case 5:
            b = findAvailableBeds(g_bedHead);
            if (b) {
                printf("空闲床位: %s (病区: %s)\n", b->data.bedNo, b->data.ward);
            } else {
                printf("当前无空闲床位。\n");
            }
            break;
        case 6:
            safeReadString("请输入病区: ", ward, 30);
            getWardStats(g_bedHead, ward, &total, &occupied);
            printf("病区 %s：总床位 %d，已占用 %d，空闲 %d\n",
                   ward, total, occupied, total - occupied);
            break;
        case 7:
            listAllBeds(g_bedHead);
            break;
        case 0:
            return;
        default:
            printf("[ERROR] 无效选择！\n");
        }
    }
}



//--------------------
// 以下为挂号管理菜单
//--------------------
void showRegistrationManagement(void) {
    int choice;
    char patientCardNo[20], patientName[50];
    char doctorEmpNo[20], doctorName[50], dept[50];
    char regNo[20], appointmentDate[20], appointmentTime[20];
    int year, month, day;
    int role = g_currentUserRole;  /*获取当前角色 */

    while (1) {
        printf("\n========== 挂号管理系统 ==========\n");

        /*根据角色显示不同菜单 */
        if (role == PATIENT) {
            printf("--- 患者功能 ---\n");
            printf("1. 预约挂号\n");
            printf("2. 取消我的挂号\n");
            printf("3. 查看我的挂号\n");
            printf("0. 返回上级菜单\n");
            choice = safeReadInt("请选择: ", 0, 3);
        }
        else if (role == NURSE) {
            printf("--- 护士功能 ---\n");
            printf("1. 现场挂号\n");
            printf("2. 查看科室候诊队列\n");
            printf("3. 取消挂号\n");
            printf("0. 返回上级菜单\n");
            choice = safeReadInt("请选择: ", 0, 3);
        }
        else if (role == DOCTOR) {
            printf("--- 医生功能 ---\n");
            printf("1. 叫号（下一位）\n");
            printf("2. 查看我的候诊列表\n");
            printf("3. 完成当前就诊\n");
            printf("0. 返回上级菜单\n");
            choice = safeReadInt("请选择: ", 0, 3);
        }
        else {
            /* [MOD-10] 管理员显示全部 */
            printf("--- 患者功能 ---\n");
            printf("1. 预约挂号        5. 查看我的挂号\n");
            printf("2. 取消我的挂号\n");
            printf("--- 护士功能 ---\n");
            printf("3. 现场挂号        6. 查看科室候诊队列\n");
            printf("--- 医生功能 ---\n");
            printf("7. 叫号（下一位）  8. 查看我的候诊列表\n");
            printf("9. 完成当前就诊\n");
            printf("--- 通用功能 ---\n");
            printf("10. 显示全部挂号记录\n");
            printf("0. 返回上级菜单\n");
            choice = safeReadInt("请选择: ", 0, 10);
            /* 管理员选项号映射为统一内部编号 */
            if (choice == 10) choice = 99;
        }

        /* [MOD-10] 角色选项号 → 统一处理编号 */
        int action = choice;
        if (role == PATIENT) {
            if (choice == 3) action = 5;      /* 查看挂号 → 统一编号 */
        }
        else if (role == NURSE) {
            if (choice == 2) action = 6;      /* 候诊队列 */
            if (choice == 3) action = 2;      /* 取消挂号 */
        }
        else if (role == DOCTOR) {
            if (choice == 1) action = 7;      /* 叫号 */
            if (choice == 2) action = 8;      /* 候诊列表 */
            if (choice == 3) action = 9;      /* 完成就诊 */
        }

        switch (action) {
        /* ──────────── 预约挂号（患者/管理员） ──────────── */
        case 1: {
            printf("\n--- 预约挂号 ---\n");
            safeReadString("请输入您的卡号: ", patientCardNo, 20);
            Patient* p = findPatientByCardNo(g_patientHead, patientCardNo);
            if (p == NULL) {
                printf("[ERROR] 未找到该患者，请先注册\n");
                break;
            }
            strcpy(patientName, p->data.name);

            /*列出所有医生供选择，不再要求输入工号 */
            printf("\n可选医生列表：\n");
            printf("%-4s %-12s %-10s %-10s %-20s\n",
                   "序号", "工号", "姓名", "科室", "出诊时间");
            Doctor* doc = g_doctorHead;
            int docCount = 0;
            while (doc != NULL) {
                printf("%-4d %-12s %-10s %-10s %-20s\n",
                       ++docCount, doc->data.empNo, doc->data.name,
                       doc->data.dept, doc->data.schedule);
                doc = doc->next;
            }
            if (docCount == 0) {
                printf("[ERROR] 暂无医生信息\n");
                break;
            }
            int sel = safeReadInt("请选择医生序号: ", 1, docCount);
            doc = g_doctorHead;
            for (int i = 1; i < sel; i++) doc = doc->next;
            strcpy(doctorEmpNo, doc->data.empNo);
            strcpy(doctorName, doc->data.name);
            strcpy(dept, doc->data.dept);

            getCurrentTime(&year, &month, &day);
            printf("请输入预约日期 (YYYY-MM-DD，至少今天 %04d-%02d-%02d): ",
                   year, month, day);
            scanf("%19s", appointmentDate);
            safeReadString("请输入预约时间 (HH:MM): ", appointmentTime, 20);

            addRegistration(&g_regHead, &g_regTail,
                            patientCardNo, patientName,
                            doctorEmpNo, doctorName, dept,
                            appointmentDate, appointmentTime, PATIENT);
            break;
        }

        /* ──────────── 取消挂号（患者/护士/管理员） ──────────── */
        case 2: {
            /* 患者：列出所有 PENDING 挂号供选择序号 */
            if (role == PATIENT || role == DOCTOR) {
                safeReadString("请输入您的卡号: ", patientCardNo, 20);
            } else {
                safeReadString("请输入患者卡号: ", patientCardNo, 20);
            }

            /* 列出该患者的 PENDING 挂号 */
            printf("\n您的待就诊挂号：\n");
            printf("%-4s %-16s %-10s %-10s %-12s %-10s\n",
                   "序号", "挂号编号", "医生", "科室", "预约时间", "方式");
            Registration* cur = g_regHead;
            Registration** pendingArr = NULL;
            int pCount = 0, arrCap = 10;
            pendingArr = (Registration**)malloc(arrCap * sizeof(Registration*));

            while (cur != NULL) {
                if (strcmp(cur->data.patientCardNo, patientCardNo) == 0 &&
                    cur->data.status == PENDING) {
                    if (pCount >= arrCap) {
                        arrCap *= 2;
                        pendingArr = (Registration**)realloc(pendingArr, arrCap * sizeof(Registration*));
                    }
                    pendingArr[pCount] = cur;
                    char* mStr = (cur->data.createdBy == PATIENT) ? "预约" : "现场";
                    printf("%-4d %-16s %-10s %-10s %-12s %-10s\n",
                           pCount + 1, cur->data.regNo, cur->data.doctorName,
                           cur->data.dept, cur->data.appointmentTime, mStr);
                    pCount++;
                }
                cur = cur->next;
            }

            if (pCount == 0) {
                printf("暂无待就诊的挂号记录\n");
                free(pendingArr);
                break;
            }

            int sel = safeReadInt("请输入要取消的序号: ", 1, pCount);
            Registration* target = pendingArr[sel - 1];
            patientCancelRegistration(&g_regHead, &g_regTail,
                                      patientCardNo, target->data.regNo);
            free(pendingArr);
            break;
        }

        /* ──────────── 现场挂号（护士/管理员） ──────────── */
        case 3: {
            printf("\n--- 现场挂号 ---\n");
            safeReadString("请输入患者卡号: ", patientCardNo, 20);
            Patient* p = findPatientByCardNo(g_patientHead, patientCardNo);
            if (p == NULL) {
                printf("[ERROR] 未找到该患者\n");
                break;
            }
            strcpy(patientName, p->data.name);

            /*同样列出医生供选择 */
            printf("\n可选医生列表：\n");
            printf("%-4s %-12s %-10s %-10s\n",
                   "序号", "工号", "姓名", "科室");
            Doctor* doc = g_doctorHead;
            int docCount = 0;
            while (doc != NULL) {
                printf("%-4d %-12s %-10s %-10s\n",
                       ++docCount, doc->data.empNo, doc->data.name, doc->data.dept);
                doc = doc->next;
            }
            if (docCount == 0) {
                printf("[ERROR] 暂无医生\n");
                break;
            }
            int sel = safeReadInt("请选择医生序号: ", 1, docCount);
            doc = g_doctorHead;
            for (int i = 1; i < sel; i++) doc = doc->next;
            strcpy(doctorEmpNo, doc->data.empNo);
            strcpy(doctorName, doc->data.name);
            strcpy(dept, doc->data.dept);

            getCurrentTime(&year, &month, &day);
            sprintf(appointmentDate, "%04d-%02d-%02d", year, month, day);
            time_t now = time(0);
            struct tm* ti = localtime(&now);
            sprintf(appointmentTime, "%02d:%02d", ti->tm_hour, ti->tm_min);

            addRegistration(&g_regHead, &g_regTail,
                            patientCardNo, patientName,
                            doctorEmpNo, doctorName, dept,
                            appointmentDate, appointmentTime, NURSE);
            break;
        }

        /* ──────────── 查看我的挂号 ──────────── */
        case 5: {
            safeReadString("请输入您的卡号: ", patientCardNo, 20);
            patientViewOwnRegistrations(g_regHead, patientCardNo);
            break;
        }

        /* ──────────── 查看科室候诊队列 ──────────── */
        case 6: {
            safeReadString("请输入科室: ", dept, 50);
            safeReadString("请输入医生工号(查看全部输0): ", doctorEmpNo, 20);
            getCurrentTime(&year, &month, &day);
            char today[20];
            sprintf(today, "%04d-%02d-%02d", year, month, day);
            if (strcmp(doctorEmpNo, "0") == 0) {
                Doctor* d = g_doctorHead;
                while (d != NULL) {
                    if (strcmp(d->data.dept, dept) == 0) {
                        Registration* queue = buildWaitingQueue(g_regHead,
                            d->data.empNo, today);
                        listWaitingQueue(queue, d->data.name);
                        Registration* cur = queue;
                        while (cur != NULL) {
                            Registration* tmp = cur;
                            cur = cur->next;
                            free(tmp);
                        }
                    }
                    d = d->next;
                }
            } else {
                doctorViewWaitingList(g_regHead, doctorEmpNo);
            }
            break;
        }

        /* ──────────── 叫号 ──────────── */
        case 7: {
            safeReadString("请输入您的工号: ", doctorEmpNo, 20);
            doctorCallNextPatient(&g_regHead, &g_regTail, doctorEmpNo);
            break;
        }

        /* ──────────── 查看候诊列表 ──────────── */
        case 8: {
            safeReadString("请输入您的工号: ", doctorEmpNo, 20);
            doctorViewWaitingList(g_regHead, doctorEmpNo);
            break;
        }

        /* ──────────── 完成就诊 ──────────── */
        case 9: {
            safeReadString("请输入挂号编号: ", regNo, 20);
            Registration* r = findRegistrationByNo(g_regHead, regNo);
            if (r) {
                completeRegistration(r);
            } else {
                printf("[ERROR] 未找到该挂号\n");
            }
            break;
        }

        /* ──────────── 显示全部 ──────────── */
        case 99:
            listAllRegistrations(g_regHead);
            break;

        case 0:
            return;

        default:
            printf("[ERROR] 无效选择\n");
        }
    }
}


//--------------------
//主函数
//--------------------
int main(void) {
    initUI();
    while (1) {
        int status = showLoginPage();
        if (status == LOGIN_EXIT) {
            printf("感谢使用，再见！\n");
            break;
        }
        if (status == LOGIN_FAILED) {
            printf("登录失败，请重试。\n");
            continue;
        }

        /* 登录成功后根据角色进入主循环 */
        while (1) {
            /* showMainMenuByRole 返回用户选择的模块号 */
            int module = showMainMenuByRole(g_currentUserRole, g_currentUsername);

            switch (module) {
            case 1: showPatientManagement();         break;
            case 2: showRegistrationManagement();    break;
            case 3: showDoctorManagement();          break;
            case 4: showBedManagement();             break;
            case 5: showMedicineManagement();         break;
            case 6: showHospitalizationManagement(); break;
            case 7: showStatisticsMenu();           break;
            case 8: showQueryMenu();                 break;
            case 0:
                printf("正在退出到登录界面...\n");
                /* 跳出内层循环，回到登录 */
                goto logout;
            default:
                printf("[ERROR] 无效选择！\n");
            }
        }
    logout:;
    }

    return 0;
}
