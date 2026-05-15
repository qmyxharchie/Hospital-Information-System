#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "ui.h"
#include "utils.h"
#include "file_io.h"          



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

void initUI(void) {}

/* 清空 stdin 到行尾（含 EOF 保护，避免死循环） */
static void flushStdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* 带校验的性别输入（只接受 男/女） */
static void readGender(const char* prompt, char* buf, int maxLen) {
    while (1) {
        safeReadString(prompt, buf, maxLen);
        if (strcmp(buf, "男") == 0 || strcmp(buf, "女") == 0) return;
        printf("[ERROR] 请输入 男 或 女\n");
    }
}

/* 带校验的身份证输入（18 位） */
static void readIdCard(const char* prompt, char* buf, int maxLen) {
    while (1) {
        safeReadString(prompt, buf, maxLen);
        if (isValidIdCard(buf)) return;
        printf("[ERROR] 身份证号格式不正确（18位数字，末位可为X）\n");
    }
}

/* 带校验的电话输入（11 位数字） */
static void readPhone(const char* prompt, char* buf, int maxLen) {
    while (1) {
        safeReadString(prompt, buf, maxLen);
        if (isValidPhone(buf)) return;
        printf("[ERROR] 电话号码格式不正确（11位数字）\n");
    }
}

/* 带校验的时间输入（HH:MM，00:00~23:59） */
static void readTime(const char* prompt, char* buf, int maxLen) {
    while (1) {
        safeReadString(prompt, buf, maxLen);
        int h = -1, m = -1;
        if (strlen(buf) == 5 && buf[2] == ':' &&
            sscanf(buf, "%d:%d", &h, &m) == 2 &&
            h >= 0 && h <= 23 && m >= 0 && m <= 59) {
            return;
        }
        printf("[ERROR] 时间格式不正确，请输入 HH:MM（如 09:30）\n");
    }
}

/* 带校验的日期输入（YYYY-MM-DD） */
static void readDate(const char* prompt, char* buf, int maxLen) {
    while (1) {
        safeReadString(prompt, buf, maxLen);
        int y = 0, m = 0, d = 0;
        if (strlen(buf) == 10 && buf[4] == '-' && buf[7] == '-' &&
            sscanf(buf, "%d-%d-%d", &y, &m, &d) == 3 &&
            isValidDate(y, m, d)) {
            return;
        }
        printf("[ERROR] 日期格式不正确，请输入 YYYY-MM-DD\n");
    }
}

/* 提示用户输入姓名+身份证号，验证通过返回 Patient*，否则返回 NULL
 * purpose: 用于提示信息（如"病人身份验证"） */
static Patient* promptFindPatient(const char* purpose) {
    char name[50], idCard[20];
    printf("\n-- %s --\n", purpose);
    safeReadString("姓名: ", name, 50);
    safeReadString("身份证号: ", idCard, 20);
    Patient* p = findPatientByNameAndId(g_patientHead, name, idCard);
    if (p == NULL) {
        printf("[ERROR] 姓名和身份证号不匹配，请核对后重试\n");
        return NULL;
    }
    return p;
}

/* 二次确认 y/N，默认 N
 * 返回 1=确认执行，0=取消 */
static int confirmYesNo(const char* msg) {
    char buf[10] = {0};
    printf("%s [y/N]: ", msg);
    safeReadString("", buf, 10);
    return (buf[0] == 'y' || buf[0] == 'Y');
}

/* 安全读取 int，带范围校验和错误清缓冲 */
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
        flushStdin();
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
        flushStdin();
        return val;
    }
}

/* 安全读取字符串 */
void safeReadString(const char* prompt, char* buf, int maxLen) {
    char fmt[16];
    sprintf(fmt, "%%%ds", maxLen - 1);
    printf("%s", prompt);
    if (scanf(fmt, buf) != 1) {
        buf[0] = '\0';
    }
    flushStdin();
}

//------------------------
// 辅助：按科室分组打印医生人数
//------------------------
static void statDoctorByDept(void) {
    char depts[20][50];
    int  counts[20] = { 0 };
    int  deptCount = 0;

    Doctor* d = g_doctorHead;
    while (d != NULL) {
        int found = 0;
        for (int i = 0; i < deptCount; i++) {
            if (strcmp(depts[i], d->data.dept) == 0) {
                counts[i]++;
                found = 1;
                break;
            }
        }
        if (!found && deptCount < 20) {
            strcpy(depts[deptCount], d->data.dept);
            counts[deptCount] = 1;
            deptCount++;
        }
        d = d->next;
    }

    printf("\n--- 科室分布 ---\n");
    printPadded("科室", 10);     putchar(' ');
    printPadded("医生人数", 10); putchar('\n');
    printf("------------------------------\n");
    for (int i = 0; i < deptCount; i++) {
        printPadded(depts[i], 10); putchar(' ');
        printf("%-10d\n", counts[i]);
    }
}

//--------------------
// 以下为登录界面函数
//--------------------
int showLoginPage(void) {
    char username[50] = { 0 };
    char password[50] = { 0 };

    printf("\n╔═════════════════════════════════════════╗\n");
    printf("║    医院综合信息管理系统 - 登录界面      ║\n");
    printf("╠═════════════════════════════════════════╣\n");
    printf("║  1. 用户登录                            ║\n");
    printf("║  2. 管理员登录                          ║\n");
    printf("║  3. 用户注册                            ║\n");
    printf("║  0. 退出系统                            ║\n");
    printf("╚═════════════════════════════════════════╝\n");

  
    int choice = safeReadInt("请选择: ", 0, 3);

    switch (choice) {
    case 1:
        safeReadString("\n请输入用户名: ", username, 50);
        safeReadString("请输入密码: ", password, 50);
        return login(username, password);
    case 2: {
        safeReadString("\n请输入管理员用户名: ", username, 50);
        safeReadString("请输入密码: ", password, 50);
        LoginStatus s = login(username, password);
        if (s == LOGIN_SUCCESS_ADMIN) return s;
        if (s == LOGIN_SUCCESS_USER) {
            printf("[ERROR] 此账号不是管理员，请从 1. 用户登录 进入\n");
            return LOGIN_FAILED;
        }
        return s;
    }
    case 3: {
        /* 注册流程：先收齐用户名+密码+档案信息，全部校验通过再落地 */
        /* 先加载现有病人链表，避免 rebuildPatientFile 覆盖已有数据 */
        freePatientChain(&g_patientHead);
        buildPatientChain(&g_patientHead, &g_patientTail);

        char regName[50], regIdCard[20], regGender[10], regPhone[15];
        int regAge;

        safeReadString("\n请输入要注册的用户名: ", username, 50);
        if (isUsernameTaken(username)) {
            printf("[ERROR] 用户名已被占用，请换一个。\n");
            return LOGIN_FAILED;
        }
        safeReadString("请输入密码: ", password, 50);

        printf("\n接下来请录入您的病人档案（不可跳过）:\n");
        safeReadString("姓名: ", regName, 50);
        regAge = safeReadInt("年龄: ", 0, 150);
        readGender("性别: ", regGender, 10);
        readIdCard("身份证号: ", regIdCard, 20);
        if (findPatientByIdCard(g_patientHead, regIdCard)) {
            printf("[ERROR] 该身份证已登记档案，注册取消。\n");
            return LOGIN_FAILED;
        }
        readPhone("联系电话: ", regPhone, 15);

        /* 两步校验均通过：写 User 和 Patient */
        if (!registerUser(username, password, 0)) {
            return LOGIN_FAILED;
        }
        addPatient(&g_patientHead, &g_patientTail,
                   regName, regAge, regGender, regIdCard, regPhone, username);
        return LOGIN_REGISTERED;
    }
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
	printPadded("║ 当前用户: ", 9);
    printPadded(username, 15);
    printPadded("角色:", 5);
    switch (userRole) {
    case 0: printPadded("患者", 10); break;
    case 1: printPadded("护士", 10); break;
    case 2: printPadded("医生", 10); break;
    case 3: printPadded("管理员", 10); break;
    default: printPadded("未知", 10); break;
    }
    printf("║\n");
    printf("╠═════════════════════════════════════════╣\n");

    if (userRole >= 0) {
        printf("║ 1. 挂号管理                             ║\n");
        printf("║ 2. 患者信息管理                         ║\n");
    }
    if (userRole >= 1) {
        printf("║ 3. 床位管理                             ║\n");
        printf("║ 4. 药品管理                             ║\n");
        printf("║ 5. 住院管理                             ║\n");
    }
    if (userRole >= 3) {
        printf("║ 6. 医生信息管理                         ║\n");
        printf("║ 7. 统计报表                             ║\n");
        printf("║ 8. 综合查询                             ║\n");
        printf("║ 9. 用户管理                             ║\n");
    }
    printf("║ 0. 退出系统                             ║\n");
    printf("╚═════════════════════════════════════════╝\n");

    int maxOption = 0;
    if (userRole >= 3) maxOption = 9;
    else if (userRole >= 1) maxOption = 5;
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

    /* 患者视角：只能查看/修改自己的档案 */
    if (g_currentUserRole == PATIENT) {
        while (1) {
            printf("\n-------- 患者信息管理（我的档案）--------\n");
            printf("1. 查看我的档案\n");
            printf("2. 修改我的档案\n");
            printf("3. 修改密码\n");
            printf("0. 返回上级菜单\n");
            int myChoice = safeReadInt("请选择: ", 0, 3);
            if (myChoice == 0) return;

            if (myChoice == 3) {
                char oldPwd[50], newPwd[50], confirmPwd[50], hashed[100];
                safeReadString("请输入旧密码: ", oldPwd, 50);
                md5Hash(oldPwd, hashed);
                freeUserChain(&g_userHead);
                buildUserChain(&g_userHead, &g_userTail);
                User* me = findUserByName(g_userHead, g_currentUsername);
                if (me == NULL || strcmp(me->data.password, hashed) != 0) {
                    printf("[ERROR] 旧密码错误\n");
                    continue;
                }
                safeReadString("请输入新密码: ", newPwd, 50);
                safeReadString("请再次输入新密码: ", confirmPwd, 50);
                if (strcmp(newPwd, confirmPwd) != 0) {
                    printf("[ERROR] 两次输入不一致\n");
                    continue;
                }
                md5Hash(newPwd, hashed);
                strcpy(me->data.password, hashed);
                rebuildUserFile(g_userHead);
                printf("[OK] 密码修改成功\n");
                continue;
            }

            Patient* mine = findPatientByOwner(g_patientHead, g_currentUsername);
            if (mine == NULL) {
                printf("[ERROR] 系统中找不到您的档案，请联系管理员\n");
                continue;
            }

            if (myChoice == 1) {
                printPadded("卡号", 20); putchar(' ');
                printPadded("姓名", 10); putchar(' ');
                printPadded("年龄", 6);  putchar(' ');
                printPadded("性别", 6);  putchar(' ');
                printPadded("身份证", 20); putchar(' ');
                printPadded("电话", 14); putchar(' ');
                printPadded("住院状态", 10); putchar('\n');
                printPadded(mine->data.cardNo, 20); putchar(' ');
                printPadded(mine->data.name, 10);   putchar(' ');
                printf("%-6d ", mine->data.age);
                printPadded(mine->data.gender, 6);  putchar(' ');
                printPadded(mine->data.idCard, 20); putchar(' ');
                printPadded(mine->data.phone, 14);  putchar(' ');
                printPadded(mine->data.isActive ? "住院" : "非住院", 10); putchar('\n');
            }
            else if (myChoice == 2) {
                PatientData newData;
                printf("\n请输入修改后的档案信息:");
                safeReadString("\n姓名: ", newData.name, 50);
                newData.age = safeReadInt("\n年龄: ", 0, 150);
                readGender("\n性别: ", newData.gender, 10);
                readIdCard("\n身份证号: ", newData.idCard, 20);
                readPhone("\n联系电话: ", newData.phone, 15);
                modifyPatient(g_patientHead, mine->data.cardNo, newData);
            }
        }
    }

    /* 医生视角：只读查看 */
    if (g_currentUserRole == DOCTOR) {
        while (1) {
            printf("\n-------- 患者信息管理（只读）--------\n");
            printf("1. 按姓名+身份证查找\n");
            printf("2. 按姓名模糊查找\n");
            printf("3. 显示所有病人\n");
            printf("0. 返回上级菜单\n");
            int dChoice = safeReadInt("请选择: ", 0, 3);
            if (dChoice == 0) return;

            if (dChoice == 1) {
                Patient* p = promptFindPatient("查找病人");
                if (p == NULL) continue;
                printf("[OK] 病人信息如下\n");
                printPadded("卡号", 20); putchar(' ');
                printPadded("姓名", 10); putchar(' ');
                printPadded("年龄", 6);  putchar(' ');
                printPadded("性别", 6);  putchar(' ');
                printPadded("身份证", 20); putchar(' ');
                printPadded("电话", 14); putchar(' ');
                printPadded("住院状态", 10); putchar('\n');
                printPadded(p->data.cardNo, 20); putchar(' ');
                printPadded(p->data.name, 10);   putchar(' ');
                printf("%-6d ", p->data.age);
                printPadded(p->data.gender, 6);  putchar(' ');
                printPadded(p->data.idCard, 20); putchar(' ');
                printPadded(p->data.phone, 14);  putchar(' ');
                printPadded(p->data.isActive ? "住院" : "非住院", 10); putchar('\n');
            }
            else if (dChoice == 2) {
                char name[50];
                safeReadString("请输入姓名：", name, 50);
                Patient* p = findPatientsByName(g_patientHead, name);
                if (p == NULL) {
                    printf("[ERROR] 未找到病人信息\n");
                } else {
                    printf("[OK] 病人信息如下\n");
                    listAllPatients(p);
                    Patient* cur = p;
                    while (cur) { Patient* tmp = cur; cur = cur->next; free(tmp); }
                }
            }
            else if (dChoice == 3) {
                listAllPatients(g_patientHead);
            }
        }
    }

    /* 护士及以上：原全量菜单 */
    while (1) {
        printf("\n-------- 患者信息管理 --------\n");
        printf("1. 添加病人   4. 查找病人\n");
        printf("2. 删除病人   5. 显示所有病人\n");
        printf("3. 修改病人信息\n");
        printf("0. 返回上级菜单\n");

        choice = safeReadInt("请选择: ", 0, 5);

        switch (choice) {
        case 1:
            printf("\n请输入病人信息:");
            safeReadString("\n姓名: ", name, 50);

            age = safeReadInt("\n年龄: ", 0, 150);
            readGender("\n性别: ", gender, 10);
            readIdCard("\n身份证号: ", idCard, 20);
            if (findPatientByIdCard(g_patientHead, idCard)) {
                printf("[ERROR] 该身份证已登记档案，添加取消\n");
                break;
            }
            readPhone("\n联系电话: ", phone, 15);
            addPatient(&g_patientHead, &g_patientTail,
                       name, age, gender, idCard, phone, "admin");
            break;
        case 2:
            p = promptFindPatient("删除病人");
            if (p == NULL) break;
            printf("将删除：%s（身份证 %s，卡号 %s）\n",
                   p->data.name, p->data.idCard, p->data.cardNo);
            if (!confirmYesNo("确认删除？")) {
                printf("已取消。\n");
                break;
            }
            delPatient(&g_patientHead, &g_patientTail, p->data);
            break;
        case 3: {
            PatientData newData;
            Patient* target = promptFindPatient("修改病人");
            if (target == NULL) break;
            strcpy(cardNo, target->data.cardNo);
            printf("\n请输入修改后的病人信息:");
            safeReadString("\n姓名: ", newData.name, 50);
            newData.age = safeReadInt("\n年龄: ", 0, 150);
            readGender("\n性别: ", newData.gender, 10);
            readIdCard("\n身份证号: ", newData.idCard, 20);
            readPhone("\n联系电话: ", newData.phone, 15);
            printf("\n");
            modifyPatient(g_patientHead, cardNo, newData);
            break;
        }
        case 4: {
            printf("请选择查找方式：\n");
            printf("1、按姓名+身份证精确查找\n");
            printf("2、按姓名模糊查找\n");
            int choice1 = safeReadInt("请选择: ", 1, 2);
            switch (choice1) {
            case 1:
                p = promptFindPatient("查找病人");
                if (p == NULL) {
                    break;
                } else {
                    printf("[OK] 病人信息如下\n");
                    printPadded("卡号", 20); putchar(' ');
                    printPadded("姓名", 10); putchar(' ');
                    printPadded("年龄", 6);  putchar(' ');
                    printPadded("性别", 6);  putchar(' ');
                    printPadded("身份证", 20); putchar(' ');
                    printPadded("电话", 14); putchar(' ');
                    printPadded("住院状态", 10); putchar('\n');
                    printPadded(p->data.cardNo, 20); putchar(' ');
                    printPadded(p->data.name, 10);   putchar(' ');
                    printf("%-6d ", p->data.age);
                    printPadded(p->data.gender, 6);  putchar(' ');
                    printPadded(p->data.idCard, 20); putchar(' ');
                    printPadded(p->data.phone, 14);  putchar(' ');
                    printPadded(p->data.isActive ? "住院" : "非住院", 10); putchar('\n');
                }
                break;
            case 2:
                safeReadString("请输入姓名：", name, 50);
                p = findPatientsByName(g_patientHead, name);
                if (p == NULL) {
                    printf("[ERROR] 未找到病人信息\n");
                } else {
                    printf("[OK] 病人信息如下\n");
                    printPadded("卡号", 20); putchar(' ');
                    printPadded("姓名", 10); putchar(' ');
                    printPadded("年龄", 6);  putchar(' ');
                    printPadded("性别", 6);  putchar(' ');
                    printPadded("身份证", 20); putchar(' ');
                    printPadded("电话", 14); putchar(' ');
                    printPadded("住院状态", 10); putchar('\n');
                    printPadded(p->data.cardNo, 20); putchar(' ');
                    printPadded(p->data.name, 10);   putchar(' ');
                    printf("%-6d ", p->data.age);
                    printPadded(p->data.gender, 6);  putchar(' ');
                    printPadded(p->data.idCard, 20); putchar(' ');
                    printPadded(p->data.phone, 14);  putchar(' ');
                    printPadded(p->data.isActive ? "住院" : "非住院", 10); putchar('\n');
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
    }
}


//--------------------
// 以下为医生管理菜单显示函数
//--------------------
void showDoctorManagement(void) {
    int choice;
    char name[50], dept[50], empNo[20];
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
        case 1: {
            printf("\n请输入医生信息:");
            safeReadString("\n姓名: ", name, 50);
            safeReadString("\n科室: ", dept, 50);
            maxPatients = safeReadInt("\n每日最大接诊数: ", 1, 999);
            char bindUser[50];
            safeReadString("绑定用户名（暂不绑定输 无）: ", bindUser, 50);
            addDoctor(&g_doctorHead, &g_doctorTail,
                      name, dept, maxPatients, bindUser);
            printf("[OK] 医生添加成功！\n");
            break;
        }
        case 2:
            safeReadString("请输入要删除的医生工号：", empNo, 20);
            d = findDoctorByEmpNo(g_doctorHead, empNo);
            if (d) {
                printf("将删除：%s  %s  %s科\n",
                       d->data.empNo, d->data.name, d->data.dept);
                if (!confirmYesNo("确认删除？")) {
                    printf("已取消。\n");
                    break;
                }
                delDoctor(&g_doctorHead, &g_doctorTail, d->data);
            } else {
                printf("[ERROR] 未找到该医生！\n");
            }
            break;

        case 3: {
            DoctorData newData;
            safeReadString("\n请输入要修改的医生工号: ", empNo, 20);
            Doctor* target = findDoctorByEmpNo(g_doctorHead, empNo);
            if (target == NULL) {
                printf("[ERROR] 未找到该医生\n");
                break;
            }
            printf("当前绑定用户名：%s\n", target->data.ownerUsername);
            printf("\n请输入医生信息:");
            safeReadString("\n姓名: ", newData.name, 50);
            safeReadString("\n科室: ", newData.dept, 50);
            newData.maxPatients = safeReadInt("\n每日最大接诊数: ", 1, 999);
            safeReadString("绑定用户名（不改输 无，清除绑定输 清除）: ", newData.ownerUsername, 50);
            if (strcmp(newData.ownerUsername, "清除") == 0) {
                strcpy(newData.ownerUsername, "无");
            } else if (strcmp(newData.ownerUsername, "无") == 0) {
                strcpy(newData.ownerUsername, target->data.ownerUsername);
            }
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
                    printPadded("工号", 20); putchar(' ');
                    printPadded("姓名", 12); putchar(' ');
                    printPadded("科室", 10); putchar(' ');
                    printPadded("每日最大接诊数", 16); putchar(' ');
                    printPadded("今日已接诊数", 14); putchar('\n');
                    printPadded(d->data.empNo, 20); putchar(' ');
                    printPadded(d->data.name, 12); putchar(' ');
                    printPadded(d->data.dept, 10); putchar(' ');
                    printf("%-16d %-14d\n",
                           d->data.maxPatients,
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
                if (d == NULL) {
                    printf("[ERROR] 未找到该科室的医生！\n");
                } else {
                    printf("[OK] %s 医生列表如下\n", dept);
                    listAllDoctors(d);
                    /* 注意：findDoctorsByDept 返回新链表，需释放 */
                    Doctor* cur = d;
                    while (cur) { Doctor* tmp = cur; cur = cur->next; free(tmp); }
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
    Medicine* m;

    /* 医生视角：只读查询 */
    if (g_currentUserRole == DOCTOR) {
        while (1) {
            printf("\n-------- 药品管理（只读）--------\n");
            printf("1. 按编号查询\n");
            printf("2. 按名称查询\n");
            printf("3. 显示所有药品\n");
            printf("0. 返回上级菜单\n");
            choice = safeReadInt("请选择: ", 0, 3);
            if (choice == 0) return;

            if (choice == 1) {
                safeReadString("请输入药品编号: ", medNo, 20);
                m = findMedicineByNo(g_medHead, medNo);
                if (m) {
                    printPadded("编号", 16);   putchar(' ');
                    printPadded("通用名", 14); putchar(' ');
                    printPadded("商品名", 16); putchar(' ');
                    printPadded("规格", 12);   putchar(' ');
                    printPadded("单价", 10);   putchar(' ');
                    printPadded("库存", 10);   putchar(' ');
                    printPadded("最低库存", 10); putchar('\n');
                    printPadded(m->data.medNo, 16);       putchar(' ');
                    printPadded(m->data.genericName, 14); putchar(' ');
                    printPadded(m->data.brandName, 16);   putchar(' ');
                    printPadded(m->data.spec, 12);        putchar(' ');
                    printf("%-10.2f %-10d %-10d\n",
                           m->data.price, m->data.stock, m->data.minStock);
                } else {
                    printf("[ERROR] 未找到该药品！\n");
                }
            }
            else if (choice == 2) {
                safeReadString("请输入药品名称: ", name, 50);
                m = findMedicineByName(g_medHead, name);
                if (m) {
                    printPadded("编号", 16);   putchar(' ');
                    printPadded("通用名", 14); putchar(' ');
                    printPadded("商品名", 16); putchar(' ');
                    printPadded("规格", 12);   putchar(' ');
                    printPadded("单价", 10);   putchar(' ');
                    printPadded("库存", 10);   putchar(' ');
                    printPadded("最低库存", 10); putchar('\n');
                    printPadded(m->data.medNo, 16);       putchar(' ');
                    printPadded(m->data.genericName, 14); putchar(' ');
                    printPadded(m->data.brandName, 16);   putchar(' ');
                    printPadded(m->data.spec, 12);        putchar(' ');
                    printf("%-10.2f %-10d %-10d\n",
                           m->data.price, m->data.stock, m->data.minStock);
                } else {
                    printf("[ERROR] 未找到该药品！\n");
                }
            }
            else if (choice == 3) {
                listAllMedicines(g_medHead);
            }
        }
    }

    /* 护士/管理员：全权限 */
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
                printPadded("编号", 16);   putchar(' ');
                printPadded("通用名", 14); putchar(' ');
                printPadded("商品名", 16); putchar(' ');
                printPadded("规格", 12);   putchar(' ');
                printPadded("单价", 10);   putchar(' ');
                printPadded("库存", 10);   putchar(' ');
                printPadded("最低库存", 10); putchar('\n');
                printPadded(m->data.medNo, 16);       putchar(' ');
                printPadded(m->data.genericName, 14); putchar(' ');
                printPadded(m->data.brandName, 16);   putchar(' ');
                printPadded(m->data.spec, 12);        putchar(' ');
                printf("%-10.2f %-10d %-10d\n",
                       m->data.price, m->data.stock, m->data.minStock);
            } else {
                printf("[ERROR] 未找到该药品！\n");
            }
            break;
        case 2:
            safeReadString("请输入药品名称: ", name, 50);
            m = findMedicineByName(g_medHead, name);
            if (m) {
                printPadded("编号", 16);   putchar(' ');
                printPadded("通用名", 14); putchar(' ');
                printPadded("商品名", 16); putchar(' ');
                printPadded("规格", 12);   putchar(' ');
                printPadded("单价", 10);   putchar(' ');
                printPadded("库存", 10);   putchar(' ');
                printPadded("最低库存", 10); putchar('\n');
                printPadded(m->data.medNo, 16);       putchar(' ');
                printPadded(m->data.genericName, 14); putchar(' ');
                printPadded(m->data.brandName, 16);   putchar(' ');
                printPadded(m->data.spec, 12);        putchar(' ');
                printf("%-10.2f %-10d %-10d\n",
                       m->data.price, m->data.stock, m->data.minStock);
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
            Patient* pp = promptFindPatient("购药登记");
            if (pp == NULL) break;
            strcpy(patientCardNo, pp->data.cardNo);
            safeReadString("\n请输入药品编号: ", medNo, 20);
            quantity = safeReadInt("\n请输入购买数量: ", 1, 99999);

            safeReadString("\n请输入购药日期(YYYY-MM-DD): ", date, 20);
            m = findMedicineByNo(g_medHead, medNo);
            if (m && m->data.stock >= quantity) {
                addPurchaseRecord(&g_purHead, &g_purTail, g_medHead,
                                  patientCardNo, medNo, quantity,  date);
                printf("[OK] 购药登记成功！\n");
				printf("总花费：%.2f 元\n", m->data.price * quantity);
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

    /* 医生视角：只读查看 */
    if (g_currentUserRole == DOCTOR) {
        while (1) {
            printf("\n-------- 住院管理（只读）--------\n");
            printf("1. 按卡号查询\n");
            printf("2. 按住院单号查询\n");
            printf("3. 显示当前在院病人\n");
            printf("4. 显示所有住院记录\n");
            printf("0. 返回上级菜单\n");
            choice = safeReadInt("请选择: ", 0, 4);
            if (choice == 0) return;

            if (choice == 1) {
                Patient* pp = promptFindPatient("查询住院记录");
                if (pp == NULL) continue;
                strcpy(patientCardNo, pp->data.cardNo);
                h = findHospitalizationByCardNo(g_hosHead, patientCardNo);
                if (h) {
                    printPadded("住院单号", 20); putchar(' ');
                    printPadded("姓名", 10);     putchar(' ');
                    printPadded("床位号", 10);   putchar(' ');
                    printPadded("预交金", 12);   putchar(' ');
                    printPadded("总费用", 12);   putchar(' ');
                    printPadded("入院日期", 12); putchar(' ');
                    printPadded("状态", 10);     putchar('\n');
                    printPadded(h->data.recordNo, 20);      putchar(' ');
                    printPadded(h->data.patientName, 10);   putchar(' ');
                    printPadded(h->data.bedNo, 10);         putchar(' ');
                    printf("%-12.2f %-12.2f ", h->data.prepay, h->data.totalCost);
                    printPadded(h->data.admissionDate, 12); putchar(' ');
                    printPadded(h->data.status, 10);        putchar('\n');
                } else {
                    printf("[ERROR] 未找到该病人的住院记录！\n");
                }
            }
            else if (choice == 2) {
                safeReadString("请输入住院单号: ", recordNo, 20);
                h = findHospitalizationByNo(g_hosHead, recordNo);
                if (h) {
                    printPadded("住院单号", 20); putchar(' ');
                    printPadded("姓名", 10);     putchar(' ');
                    printPadded("床位号", 10);   putchar(' ');
                    printPadded("预交金", 12);   putchar(' ');
                    printPadded("总费用", 12);   putchar(' ');
                    printPadded("入院日期", 12); putchar(' ');
                    printPadded("状态", 10);     putchar('\n');
                    printPadded(h->data.recordNo, 20);      putchar(' ');
                    printPadded(h->data.patientName, 10);   putchar(' ');
                    printPadded(h->data.bedNo, 10);         putchar(' ');
                    printf("%-12.2f %-12.2f ", h->data.prepay, h->data.totalCost);
                    printPadded(h->data.admissionDate, 12); putchar(' ');
                    printPadded(h->data.status, 10);        putchar('\n');
                } else {
                    printf("[ERROR] 未找到该住院记录！\n");
                }
            }
            else if (choice == 3) {
                listAllHospitalizations(g_hosHead);
            }
            else if (choice == 4) {
                listAllHospitalizations(g_hosHead);
            }
        }
    }

    /* 护士/管理员：全权限 */
    while (1) {
        printf("\n-------- 住院管理 --------\n");
        printf("1. 入院登记        5. 显示当前在院病人\n");
        printf("2. 出院结算        6. 显示所有住院记录\n");
        printf("3. 按卡号查询      7. 追加预交金\n");
        printf("4. 按住院单号查询  8. 修改住院信息（转床/追加押金）\n");
        printf("0. 返回上级菜单\n");

        choice = safeReadInt("请选择: ", 0, 8);

        switch (choice) {
        case 1: {
            printf("\n请输入入院信息:");
            Patient* pp = promptFindPatient("入院登记");
            if (pp == NULL) break;
            strcpy(patientCardNo, pp->data.cardNo);
            strcpy(patientName, pp->data.name);
            prepay = safeReadDouble("\n预交金额: ");
            addHospitalization(&g_hosHead, &g_hosTail,
                               patientCardNo, patientName, prepay);
            printf("住院单号为：%20s\n", g_hosTail->data.recordNo);
            printf("床位号为：%20s", g_hosTail->data.bedNo);

            break;
        }
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
        case 3: {
            Patient* pp = promptFindPatient("查询住院记录");
            if (pp == NULL) break;
            strcpy(patientCardNo, pp->data.cardNo);
            h = findHospitalizationByCardNo(g_hosHead, patientCardNo);
            if (h) {
                printPadded("住院单号", 20); putchar(' ');
                printPadded("姓名", 10);     putchar(' ');
                printPadded("床位号", 10);   putchar(' ');
                printPadded("预交金", 12);   putchar(' ');
                printPadded("总费用", 12);   putchar(' ');
                printPadded("入院日期", 12); putchar(' ');
                printPadded("状态", 10);     putchar('\n');
                printPadded(h->data.recordNo, 20);      putchar(' ');
                printPadded(h->data.patientName, 10);   putchar(' ');
                printPadded(h->data.bedNo, 10);         putchar(' ');
                printf("%-12.2f %-12.2f ", h->data.prepay, h->data.totalCost);
                printPadded(h->data.admissionDate, 12); putchar(' ');
                printPadded(h->data.status, 10);        putchar('\n');
            } else {
                printf("[ERROR] 未找到该病人的住院记录！\n");
            }
            break;
        }
        case 4:
            safeReadString("请输入住院单号: ", recordNo, 20);
            h = findHospitalizationByNo(g_hosHead, recordNo);
            if (h) {
                printPadded("住院单号", 20); putchar(' ');
                printPadded("姓名", 10);     putchar(' ');
                printPadded("床位号", 10);   putchar(' ');
                printPadded("预交金", 12);   putchar(' ');
                printPadded("总费用", 12);   putchar(' ');
                printPadded("入院日期", 12); putchar(' ');
                printPadded("状态", 10);     putchar('\n');
                printPadded(h->data.recordNo, 20);      putchar(' ');
                printPadded(h->data.patientName, 10);   putchar(' ');
                printPadded(h->data.bedNo, 10);         putchar(' ');
                printf("%-12.2f %-12.2f ", h->data.prepay, h->data.totalCost);
                printPadded(h->data.admissionDate, 12); putchar(' ');
                printPadded(h->data.status, 10);        putchar('\n');
            } else {
                printf("[ERROR] 未找到该住院记录！\n");
            }
            break;
        case 5:
            h = findCurrentHospitalizations(g_hosHead);
            if (h) {
                listAllHospitalizations(h);
                freeHospitalizationChain(&h);
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

    /* 医生视角：只读查看 */
    if (g_currentUserRole == DOCTOR) {
        while (1) {
            printf("\n-------- 床位管理（只读）--------\n");
            printf("1. 按床位号查询\n");
            printf("2. 按科室查询\n");
            printf("3. 查找空闲床位\n");
            printf("4. 显示所有床位\n");
            printf("0. 返回上级菜单\n");
            choice = safeReadInt("请选择: ", 0, 4);
            if (choice == 0) return;

            if (choice == 1) {
                safeReadString("请输入床位号: ", bedNo, 20);
                b = findBedByNo(g_bedHead, bedNo);
                if (b) {
                    printPadded("科室", 10);     putchar(' ');
                    printPadded("床位号", 18);   putchar(' ');
                    printPadded("病人卡号", 20); putchar(' ');
                    printPadded("病人姓名", 10); putchar(' ');
                    printPadded("状态", 10);     putchar('\n');
                    printPadded(b->data.ward, 10);          putchar(' ');
                    printPadded(b->data.bedNo, 18);         putchar(' ');
                    printPadded(b->data.patientCardNo, 20); putchar(' ');
                    printPadded(b->data.patientName, 10);   putchar(' ');
                    printPadded(b->data.status, 10);        putchar('\n');
                } else {
                    printf("[ERROR] 未找到该床位！\n");
                }
            }
            else if (choice == 2) {
                safeReadString("请输入科室: ", ward, 30);
                listBedsByWard(g_bedHead, ward);
            }
            else if (choice == 3) {
                Bed* avail = findAvailableBeds(g_bedHead);
                if (avail == NULL) {
                    printf("当前无空闲床位。\n");
                } else {
                    printf("\n=== 空闲床位列表 ===\n");
                    printPadded("科室", 10);   putchar(' ');
                    printPadded("床位号", 18); putchar('\n');
                    Bed* cur = avail;
                    while (cur != NULL) {
                        printPadded(cur->data.ward, 10);  putchar(' ');
                        printPadded(cur->data.bedNo, 18); putchar('\n');
                        cur = cur->next;
                    }
                    cur = avail;
                    while (cur) { Bed* tmp = cur; cur = cur->next; free(tmp); }
                }
            }
            else if (choice == 4) {
                listAllBeds(g_bedHead);
            }
        }
    }

    /* 护士/管理员：全权限 */
    while (1) {
        printf("\n-------- 床位管理 --------\n");
        printf("1. 添加床位     5. 查找空闲床位\n");
        printf("2. 删除床位     6. 科室统计\n");
        printf("3. 按床位号查询 7. 显示所有床位\n");
        printf("4. 按科室查询   0. 返回上级菜单\n");

        choice = safeReadInt("请选择: ", 0, 7);

        switch (choice) {
        case 1:
            printf("\n请输入床位信息:\n");
            safeReadString("科室: ", ward, 30);
            addBed(&g_bedHead, &g_bedTail, ward);
            printf("[OK] 床位添加成功！\n");
            break;
        case 2:
            safeReadString("请输入要删除的床位号: ", bedNo, 20);
            b = findBedByNo(g_bedHead, bedNo);
            if (b) {
                printf("将删除：%s床位（科室 %s，状态 %s）\n",
                       b->data.bedNo, b->data.ward, b->data.status);
                if (!confirmYesNo("确认删除？")) {
                    printf("已取消。\n");
                    break;
                }
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
                printPadded("科室", 10);     putchar(' ');
                printPadded("床位号", 18);   putchar(' ');
                printPadded("病人卡号", 20); putchar(' ');
                printPadded("病人姓名", 10); putchar(' ');
                printPadded("状态", 10);     putchar('\n');
                printPadded(b->data.ward, 10);          putchar(' ');
                printPadded(b->data.bedNo, 18);         putchar(' ');
                printPadded(b->data.patientCardNo, 20); putchar(' ');
                printPadded(b->data.patientName, 10);   putchar(' ');
                printPadded(b->data.status, 10);        putchar('\n');
            } else {
                printf("[ERROR] 未找到该床位！\n");
            }
            break;
        case 4: {
            /* 收集当前有哪些科室 */
            char wardList[20][30];
            int wardCount = 0;
            Bed* tmp = g_bedHead;
            while (tmp != NULL) {
                int found = 0;
                for (int i = 0; i < wardCount; i++) {
                    if (strcmp(wardList[i], tmp->data.ward) == 0) { found = 1; break; }
                }
                if (!found && wardCount < 20) {
                    strcpy(wardList[wardCount], tmp->data.ward);
                    wardCount++;
                }
                tmp = tmp->next;
            }
            if (wardCount == 0) {
                printf("[ERROR] 暂无床位信息\n");
                break;
            }
            printf("\n当前科室列表：\n");
            for (int i = 0; i < wardCount; i++) {
                printf("  %d. %s\n", i + 1, wardList[i]);
            }
            int wSel = safeReadInt("请选择科室序号: ", 1, wardCount);
            strcpy(ward, wardList[wSel - 1]);
            listBedsByWard(g_bedHead, ward);
            break;
        }
        case 5: {
            Bed* avail = findAvailableBeds(g_bedHead);
            if (avail == NULL) {
                printf("当前无空闲床位。\n");
            } else {
                printf("\n=== 空闲床位列表 ===\n");
                printPadded("科室", 10);   putchar(' ');
                printPadded("床位号", 18); putchar('\n');
                Bed* cur = avail;
                while (cur != NULL) {
                    printPadded(cur->data.ward, 10);  putchar(' ');
                    printPadded(cur->data.bedNo, 18); putchar('\n');
                    cur = cur->next;
                }
                /* 释放结果链表 */
                cur = avail;
                while (cur) { Bed* tmp = cur; cur = cur->next; free(tmp); }
            }
            break;
        }
        case 6: {
            /* 收集当前有哪些科室 */
            char wardList2[20][30];
            int wardCount2 = 0;
            Bed* tmp2 = g_bedHead;
            while (tmp2 != NULL) {
                int found = 0;
                for (int i = 0; i < wardCount2; i++) {
                    if (strcmp(wardList2[i], tmp2->data.ward) == 0) { found = 1; break; }
                }
                if (!found && wardCount2 < 20) {
                    strcpy(wardList2[wardCount2], tmp2->data.ward);
                    wardCount2++;
                }
                tmp2 = tmp2->next;
            }
            if (wardCount2 == 0) {
                printf("[ERROR] 暂无床位信息\n");
                break;
            }
            printf("\n当前科室列表：\n");
            for (int i = 0; i < wardCount2; i++) {
                printf("  %d. %s\n", i + 1, wardList2[i]);
            }
            int wSel2 = safeReadInt("请选择科室序号: ", 1, wardCount2);
            strcpy(ward, wardList2[wSel2 - 1]);
            getWardStats(g_bedHead, ward, &total, &occupied);
            printf("科室 %s：总床位 %d，已占用 %d，空闲 %d\n",
                   ward, total, occupied, total - occupied);
            break;
        }
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

    /* 患者角色：自动获取自己的卡号，后续不再手动输入 */
    char myCardNo[20] = "";
    char myName[50] = "";
    if (role == PATIENT) {
        Patient* me = findPatientByOwner(g_patientHead, g_currentUsername);
        if (me != NULL) {
            strcpy(myCardNo, me->data.cardNo);
            strcpy(myName, me->data.name);
        }
    }

    while (1) {
        printf("\n========== 挂号管理 ==========\n");

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
            /* 管理员显示全部功能（连续编号） */
            printf("--- 患者功能 ---\n");
            printf("1. 预约挂号\n");
            printf("2. 取消挂号\n");
            printf("--- 护士功能 ---\n");
            printf("3. 现场挂号\n");
            printf("--- 查询功能 ---\n");
            printf("4. 查看我的挂号\n");
            printf("5. 查看科室候诊队列\n");
            printf("--- 医生功能 ---\n");
            printf("6. 叫号（下一位）\n");
            printf("7. 查看我的候诊列表\n");
            printf("8. 完成当前就诊\n");
            printf("--- 通用功能 ---\n");
            printf("9. 显示全部挂号记录\n");
            printf("0. 返回上级菜单\n");
            choice = safeReadInt("请选择: ", 0, 9);
            /* 管理员选项号映射为统一内部编号 */
            switch (choice) {
            case 1: choice = 1; break;
            case 2: choice = 2; break;
            case 3: choice = 3; break;
            case 4: choice = 5; break;
            case 5: choice = 6; break;
            case 6: choice = 7; break;
            case 7: choice = 8; break;
            case 8: choice = 9; break;
            case 9: choice = 99; break;
            }
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
            if (role == PATIENT) {
                if (myCardNo[0] == '\0') {
                    printf("[ERROR] 系统中找不到您的档案，请联系管理员\n");
                    break;
                }
                strcpy(patientCardNo, myCardNo);
                strcpy(patientName, myName);
            } else {
                Patient* pp = promptFindPatient("预约挂号");
                if (pp == NULL) break;
                strcpy(patientCardNo, pp->data.cardNo);
                strcpy(patientName, pp->data.name);
            }

            /* 先选科室 */
            char deptList[20][50];
            int deptCount = 0;
            Doctor* tmp = g_doctorHead;
            while (tmp != NULL) {
                int found = 0;
                for (int i = 0; i < deptCount; i++) {
                    if (strcmp(deptList[i], tmp->data.dept) == 0) { found = 1; break; }
                }
                if (!found && deptCount < 20) {
                    strcpy(deptList[deptCount], tmp->data.dept);
                    deptCount++;
                }
                tmp = tmp->next;
            }
            if (deptCount == 0) {
                printf("[ERROR] 暂无科室信息\n");
                break;
            }
            printf("\n可选科室：\n");
            for (int i = 0; i < deptCount; i++) {
                printf("  %d. %s\n", i + 1, deptList[i]);
            }
            int deptSel = safeReadInt("请选择科室序号: ", 1, deptCount);
            char selectedDept[50];
            strcpy(selectedDept, deptList[deptSel - 1]);

            /* 再列出该科室的医生 */
            printf("\n%s 可选医生：\n", selectedDept);
            printPadded("序号", 6);  putchar(' ');
            printPadded("工号", 20); putchar(' ');
            printPadded("姓名", 10); putchar('\n');
            Doctor* doc = g_doctorHead;
            int docCount = 0;
            Doctor* docArr[100] = {0};
            while (doc != NULL) {
                if (strcmp(doc->data.dept, selectedDept) == 0) {
                    docArr[docCount] = doc;
                    printf("%-6d ", ++docCount);
                    printPadded(doc->data.empNo, 20); putchar(' ');
                    printPadded(doc->data.name, 10);  putchar('\n');
                }
                doc = doc->next;
            }
            if (docCount == 0) {
                printf("[ERROR] 该科室暂无医生\n");
                break;
            }
            int sel = safeReadInt("请选择医生序号: ", 1, docCount);
            doc = docArr[sel - 1];
            strcpy(doctorEmpNo, doc->data.empNo);
            strcpy(doctorName, doc->data.name);
            strcpy(dept, doc->data.dept);
            int year=month=day=0;
            getCurrentTime(&year, &month, &day);
            printf("请输入预约日期 (YYYY-MM-DD，至少今天 %04d-%02d-%02d): ",
                   year, month, day);
            readDate("", appointmentDate, 20);
            readTime("请输入预约时间 (HH:MM): ", appointmentTime, 20);

            addRegistration(&g_regHead, &g_regTail,
                            patientCardNo, patientName,
                            doctorEmpNo, doctorName, dept,
                            appointmentDate, appointmentTime, PATIENT);
            break;
        }

        /* ──────────── 取消挂号（患者/护士/管理员） ──────────── */
        case 2: {
            /* 患者/医生：自动填卡号；护士/管理员：手动查找 */
            if (role == PATIENT) {
                if (myCardNo[0] == '\0') {
                    printf("[ERROR] 系统中找不到您的档案\n");
                    break;
                }
                strcpy(patientCardNo, myCardNo);
            } else if (role == DOCTOR) {
                safeReadString("请输入患者卡号: ", patientCardNo, 20);
            } else {
                Patient* pp = promptFindPatient("取消挂号");
                if (pp == NULL) break;
                strcpy(patientCardNo, pp->data.cardNo);
            }

            /* 列出该患者的 PENDING 挂号 */
            printf("\n您的待就诊挂号：\n");
            printPadded("序号", 6);      putchar(' ');
            printPadded("挂号编号", 20); putchar(' ');
            printPadded("医生", 10);     putchar(' ');
            printPadded("科室", 10);     putchar(' ');
            printPadded("预约日期", 12); putchar(' ');
            printPadded("预约时间", 12); putchar(' ');
            printPadded("方式", 8);      putchar('\n');
            Registration* cur = g_regHead;
            Registration** pendingArr = NULL;
            int pCount = 0, arrCap = 10;
            pendingArr = (Registration**)malloc(arrCap * sizeof(Registration*));
            if (pendingArr == NULL) {
                printf("[ERROR] 内存分配失败\n");
                break;
            }

            while (cur != NULL) {
                if (strcmp(cur->data.patientCardNo, patientCardNo) == 0 &&
                    cur->data.status == PENDING) {
                    if (pCount >= arrCap) {
                        arrCap *= 2;
                        Registration** tmp = (Registration**)realloc(pendingArr, arrCap * sizeof(Registration*));
                        if (tmp == NULL) {
                            printf("[ERROR] 内存扩容失败\n");
                            free(pendingArr);
                            pendingArr = NULL;
                            break;
                        }
                        pendingArr = tmp;
                    }
                    pendingArr[pCount] = cur;
                    char* mStr = (cur->data.createdBy == PATIENT) ? "预约" : "现场";
                    printf("%-6d ", pCount + 1);
                    printPadded(cur->data.regNo, 20);              putchar(' ');
                    printPadded(cur->data.doctorName, 10);         putchar(' ');
                    printPadded(cur->data.dept, 10);               putchar(' ');
                    printPadded(cur->data.appointmentDate, 12);    putchar(' ');
                    printPadded(cur->data.appointmentTime, 12);    putchar(' ');
                    printPadded(mStr, 8);                          putchar('\n');
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
            Patient* p = promptFindPatient("现场挂号");
            if (p == NULL) break;
            strcpy(patientCardNo, p->data.cardNo);
            strcpy(patientName, p->data.name);

            /* 先选科室 */
            char deptList2[20][50];
            int deptCount2 = 0;
            Doctor* tmp2 = g_doctorHead;
            while (tmp2 != NULL) {
                int found = 0;
                for (int i = 0; i < deptCount2; i++) {
                    if (strcmp(deptList2[i], tmp2->data.dept) == 0) { found = 1; break; }
                }
                if (!found && deptCount2 < 20) {
                    strcpy(deptList2[deptCount2], tmp2->data.dept);
                    deptCount2++;
                }
                tmp2 = tmp2->next;
            }
            if (deptCount2 == 0) {
                printf("[ERROR] 暂无科室信息\n");
                break;
            }
            printf("\n可选科室：\n");
            for (int i = 0; i < deptCount2; i++) {
                printf("  %d. %s\n", i + 1, deptList2[i]);
            }
            int deptSel2 = safeReadInt("请选择科室序号: ", 1, deptCount2);
            char selectedDept2[50];
            strcpy(selectedDept2, deptList2[deptSel2 - 1]);

            /* 再列出该科室的医生 */
            printf("\n%s 可选医生：\n", selectedDept2);
            printPadded("序号", 6);  putchar(' ');
            printPadded("工号", 20); putchar(' ');
            printPadded("姓名", 10); putchar('\n');
            Doctor* doc = g_doctorHead;
            int docCount = 0;
            Doctor* docArr2[100] = {0};
            while (doc != NULL) {
                if (strcmp(doc->data.dept, selectedDept2) == 0) {
                    docArr2[docCount] = doc;
                    printf("%-6d ", ++docCount);
                    printPadded(doc->data.empNo, 20); putchar(' ');
                    printPadded(doc->data.name, 10);  putchar('\n');
                }
                doc = doc->next;
            }
            if (docCount == 0) {
                printf("[ERROR] 该科室暂无医生\n");
                break;
            }
            int sel = safeReadInt("请选择医生序号: ", 1, docCount);
            doc = docArr2[sel - 1];
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
            if (role == PATIENT) {
                if (myCardNo[0] == '\0') {
                    printf("[ERROR] 系统中找不到您的档案\n");
                    break;
                }
                strcpy(patientCardNo, myCardNo);
            } else {
                Patient* pp = promptFindPatient("查看挂号记录");
                if (pp == NULL) break;
                strcpy(patientCardNo, pp->data.cardNo);
            }
            patientViewOwnRegistrations(g_regHead, patientCardNo);
            break;
        }

        /* ──────────── 查看科室候诊队列 ──────────── */
        case 6: {
            safeReadString("请输入科室: ", dept, 50);
            safeReadString("请输入医生工号(查看全部输0): ", doctorEmpNo, 20);
            int year=month=day=0;
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
            if (role == DOCTOR) {
                Doctor* me = findDoctorByOwner(g_doctorHead, g_currentUsername);
                if (me == NULL) {
                    printf("[ERROR] 您的账号未绑定医生工号，请联系管理员\n");
                    break;
                }
                strcpy(doctorEmpNo, me->data.empNo);
            } else {
                safeReadString("请输入医生工号: ", doctorEmpNo, 20);
            }
            doctorCallNextPatient(&g_regHead, &g_regTail, doctorEmpNo);
            break;
        }

        /* ──────────── 查看候诊列表 ──────────── */
        case 8: {
            if (role == DOCTOR) {
                Doctor* me = findDoctorByOwner(g_doctorHead, g_currentUsername);
                if (me == NULL) {
                    printf("[ERROR] 您的账号未绑定医生工号，请联系管理员\n");
                    break;
                }
                strcpy(doctorEmpNo, me->data.empNo);
            } else {
                safeReadString("请输入医生工号: ", doctorEmpNo, 20);
            }
            doctorViewWaitingList(g_regHead, doctorEmpNo);
            break;
        }

        /* ──────────── 完成就诊 ──────────── */
        case 9: {
            /* 获取医生工号 */
            char myEmpNo[20] = "";
            if (role == DOCTOR) {
                Doctor* me = findDoctorByOwner(g_doctorHead, g_currentUsername);
                if (me == NULL) {
                    printf("[ERROR] 您的账号未绑定医生工号，请联系管理员\n");
                    break;
                }
                strcpy(myEmpNo, me->data.empNo);
            } else {
                safeReadString("请输入医生工号: ", myEmpNo, 20);
            }

            /* 列出该医生当天 IN_PROGRESS 的记录 */
            int year9, month9, day9;
            getCurrentTime(&year9, &month9, &day9);
            char today9[20];
            sprintf(today9, "%04d-%02d-%02d", year9, month9, day9);

            Registration* cur9 = g_regHead;
            Registration* inProgress[50] = {0};
            int ipCount = 0;
            while (cur9 != NULL) {
                if (strcmp(cur9->data.doctorEmpNo, myEmpNo) == 0 &&
                    strcmp(cur9->data.appointmentDate, today9) == 0 &&
                    cur9->data.status == IN_PROGRESS) {
                    inProgress[ipCount++] = cur9;
                }
                cur9 = cur9->next;
            }

            if (ipCount == 0) {
                printf("[INFO] 当前无就诊中的患者\n");
                break;
            }

            printf("\n当前就诊中的患者：\n");
            printPadded("序号", 6); putchar(' ');
            printPadded("挂号编号", 20); putchar(' ');
            printPadded("患者姓名", 10); putchar(' ');
            printPadded("预约时间", 12); putchar('\n');
            for (int i = 0; i < ipCount; i++) {
                printf("%-6d ", i + 1);
                printPadded(inProgress[i]->data.regNo, 20); putchar(' ');
                printPadded(inProgress[i]->data.patientName, 10); putchar(' ');
                printPadded(inProgress[i]->data.appointmentTime, 12); putchar('\n');
            }

            int sel9 = safeReadInt("请选择要完成就诊的序号: ", 1, ipCount);
            completeRegistration(inProgress[sel9 - 1]);
            rebuildRegistrationFile(g_regHead);
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

/* ================================================================
 * 7. 统计报表菜单 — showStatisticsMenu()
 * ================================================================ */

void showStatisticsMenu(void) {
    int choice;
    char buf[20];

    while (1) {
        printf("\n========== 统计报表 ==========\n");
        printf("1. 病人统计\n");
        printf("2. 医生统计\n");
        printf("3. 药品统计\n");
        printf("4. 住院与床位统计\n");
        printf("5. 挂号统计\n");
        printf("6. 财务统计\n");
        printf("7. 按日期范围查挂号统计\n");
        printf("0. 返回上级菜单\n");

        choice = safeReadInt("请选择: ", 0, 7);

        switch (choice) {

            /* ──────────── 1. 病人统计 ──────────── */
        case 1: {
            int total = 0, hospitalized = 0;
            int male = 0, female = 0;
            int age0_18 = 0, age19_35 = 0, age36_60 = 0, age60plus = 0;

            Patient* p = g_patientHead;
            while (p != NULL) {
                total++;
                if (p->data.isActive) hospitalized++;

                if (strcmp(p->data.gender, "男") == 0) male++;
                else if (strcmp(p->data.gender, "女") == 0) female++;

                int age = p->data.age;
                if (age <= 18) age0_18++;
                else if (age <= 35) age19_35++;
                else if (age <= 60) age36_60++;
                else age60plus++;

                p = p->next;
            }

            printf("\n========== 病人统计报表 ==========\n");
            printf("总病人数:          %d\n", total);
            printf("当前住院人数:      %d\n", hospitalized);
            printf("\n--- 性别分布 ---\n");
            printf("  男: %d (%.1f%%)\n", male, total > 0 ? male * 100.0 / total : 0);
            printf("  女: %d (%.1f%%)\n", female, total > 0 ? female * 100.0 / total : 0);
            printf("\n--- 年龄分布 ---\n");
            printf("  0-18岁:   %d\n", age0_18);
            printf("  19-35岁:  %d\n", age19_35);
            printf("  36-60岁:  %d\n", age36_60);
            printf("  60岁以上: %d\n", age60plus);
            printf("==================================\n");
            break;
        }

              /* ──────────── 2. 医生统计 ──────────── */
        case 2: {
            int total = 0, fullLoad = 0;
            int totalMax = 0, totalCurrent = 0;

            Doctor* d = g_doctorHead;
            while (d != NULL) {
                total++;
                totalMax += d->data.maxPatients;
                totalCurrent += d->data.currentPatients;
                if (d->data.currentPatients >= d->data.maxPatients) fullLoad++;
                d = d->next;
            }

            printf("\n========== 医生统计报表 ==========\n");
            printf("总医生数:              %d\n", total);
            printf("今日满负荷接诊医生:    %d\n", fullLoad);
            printf("今日总接诊上限:        %d\n", totalMax);
            printf("今日已接诊人数:        %d\n", totalCurrent);
            printf("整体接诊率:            %.1f%%\n",
                totalMax > 0 ? totalCurrent * 100.0 / totalMax : 0);

            statDoctorByDept();
            printf("==================================\n");
            break;
        }

              /* ──────────── 3. 药品统计 ──────────── */
        case 3: {
            int total = 0, lowStock = 0;
            double totalValue = 0.0;
            Medicine* maxPriceMed = NULL;

            Medicine* m = g_medHead;
            while (m != NULL) {
                total++;
                totalValue += m->data.price * m->data.stock;
                if (m->data.stock <= m->data.minStock) lowStock++;
                if (maxPriceMed == NULL || m->data.price > maxPriceMed->data.price) {
                    maxPriceMed = m;
                }
                m = m->next;
            }

            printf("\n========== 药品统计报表 ==========\n");
            printf("药品总品种数:    %d\n", total);
            printf("库存预警品种数:  %d\n", lowStock);
            printf("库存总价值:      %.2f 元\n", totalValue);
            if (maxPriceMed) {
                printf("最高单价药品:    %s (%s) %.2f 元\n",
                    maxPriceMed->data.medNo, maxPriceMed->data.genericName,
                    maxPriceMed->data.price);
            }
            printf("==================================\n");
            break;
        }

              /* ──────────── 4. 住院与床位统计 ──────────── */
        case 4: {
            int totalBeds = 0, occupied = 0, available = 0, maintenance = 0;
            int totalInHospital = 0, totalDischarged = 0;
            double totalCost = 0.0, totalPrepay = 0.0;

            Bed* b = g_bedHead;
            while (b != NULL) {
                totalBeds++;
                if (strcmp(b->data.status, "占用") == 0) occupied++;
                else if (strcmp(b->data.status, "空闲") == 0) available++;
                else if (strcmp(b->data.status, "维修") == 0) maintenance++;
                b = b->next;
            }

            Hospitalization* h = g_hosHead;
            while (h != NULL) {
                if (strcmp(h->data.status, "住院") == 0) {
                    totalInHospital++;
                    totalPrepay += h->data.prepay;
                    totalCost += h->data.totalCost;
                }
                else {
                    totalDischarged++;
                }
                h = h->next;
            }

            printf("\n========== 住院与床位统计 ==========\n");
            printf("--- 床位 ---\n");
            printf("  总床位:  %d\n", totalBeds);
            printf("  已占用:  %d\n", occupied);
            printf("  空闲:    %d\n", available);
            printf("  维修中:  %d\n", maintenance);
            printf("  使用率:  %.1f%%\n", totalBeds > 0 ? occupied * 100.0 / totalBeds : 0);
            printf("\n--- 住院 ---\n");
            printf("  在院人数:  %d\n", totalInHospital);
            printf("  历史出院:  %d\n", totalDischarged);
            printf("  预交金总额: %.2f 元\n", totalPrepay);
            printf("  费用总额:   %.2f 元\n", totalCost);
            printf("====================================\n");
            break;
        }

              /* ──────────── 5. 挂号统计 ──────────── */
        case 5: {
            int year, month, day;
            getCurrentTime(&year, &month, &day);
            sprintf(buf, "%04d-%02d-%02d", year, month, day);

            int totalToday = 0, pending = 0, inProgress = 0;
            int completed = 0, cancelled = 0;
            int onsite = 0, online = 0;

            Registration* r = g_regHead;
            while (r != NULL) {
                if (strcmp(r->data.appointmentDate, buf) == 0) {
                    totalToday++;
                    if (r->data.createdBy == NURSE) onsite++;
                    else online++;
                }
                switch (r->data.status) {
                case 0: pending++; break;
                case 1: inProgress++; break;
                case 2: completed++; break;
                case 3: cancelled++; break;
                }
                r = r->next;
            }

            printf("\n========== 挂号统计报表 ==========\n");
            printf("统计日期: %s\n", buf);
            printf("\n--- 今日挂号 ---\n");
            printf("  今日总挂号数: %d\n", totalToday);
            printf("  现场挂号:     %d\n", onsite);
            printf("  线上预约:     %d\n", online);
            printf("\n--- 全部挂号状态分布 ---\n");
            printf("  待就诊:  %d\n", pending);
            printf("  就诊中:  %d\n", inProgress);
            printf("  已完成:  %d\n", completed);
            printf("  已取消:  %d\n", cancelled);
            printf("==================================\n");
            break;
        }

              /* ──────────── 6. 财务统计 ──────────── */
        case 6: {
            double totalMedicineCost = 0.0;
            double totalHospitalCost = 0.0;
            double totalPrepay = 0.0;

            Purchase* pur = g_purHead;
            while (pur != NULL) {
                totalMedicineCost += pur->data.totalCost;
                pur = pur->next;
            }

            Hospitalization* h = g_hosHead;
            while (h != NULL) {
                totalHospitalCost += h->data.totalCost;
                totalPrepay += h->data.prepay;
                h = h->next;
            }

            printf("\n========== 财务统计报表 ==========\n");
            printf("药品销售总额:       %.2f 元\n", totalMedicineCost);
            printf("住院诊疗费用总额:   %.2f 元\n", totalHospitalCost);
            printf("预交金收入总额:     %.2f 元\n", totalPrepay);
            printf("总收入（估算）:     %.2f 元\n",
                totalMedicineCost + totalHospitalCost);
            printf("==================================\n");
            break;
        }

        case 7: {
            char startDate[20], endDate[20];
            printf("\n--- 按日期范围查挂号统计 ---\n");
            readDate("起始日期 (YYYY-MM-DD): ", startDate, 20);
            readDate("结束日期 (YYYY-MM-DD): ", endDate, 20);

            int total7 = 0, pending7 = 0, inProgress7 = 0;
            int completed7 = 0, cancelled7 = 0;
            int onsite7 = 0, online7 = 0;

            Registration* r7 = g_regHead;
            while (r7 != NULL) {
                if (compareDateStr(r7->data.appointmentDate, startDate) >= 0 &&
                    compareDateStr(r7->data.appointmentDate, endDate) <= 0) {
                    total7++;
                    if (r7->data.createdBy == NURSE) onsite7++;
                    else online7++;
                    switch (r7->data.status) {
                    case 0: pending7++; break;
                    case 1: inProgress7++; break;
                    case 2: completed7++; break;
                    case 3: cancelled7++; break;
                    }
                }
                r7 = r7->next;
            }

            printf("\n========== 挂号统计（%s ~ %s）==========\n", startDate, endDate);
            printf("  总挂号数:  %d\n", total7);
            printf("  现场挂号:  %d\n", onsite7);
            printf("  线上预约:  %d\n", online7);
            printf("\n--- 状态分布 ---\n");
            printf("  待就诊:  %d\n", pending7);
            printf("  就诊中:  %d\n", inProgress7);
            printf("  已完成:  %d\n", completed7);
            printf("  已取消:  %d\n", cancelled7);
            printf("==========================================\n");
            break;
        }

        case 0:
            return;

        default:
            printf("[ERROR] 无效选择！\n");
        }
    }
}


/* ================================================================
 * 8. 查询功能菜单 — showQueryMenu()
 * ================================================================ */

void showQueryMenu(void) {
    int choice;
    char cardNo[20], date[20], dept[50];
    int found;

    while (1) {
        printf("\n========== 综合查询 ==========\n");
        printf("1. 按姓名+身份证查全部关联记录\n");
        printf("2. 按医生查其挂号记录\n");
        printf("3. 按日期查挂号记录\n");
        printf("4. 按日期查住院记录\n");
        printf("5. 按科室查医生和病人\n");
        printf("6. 药品库存明细查询\n");
        printf("0. 返回上级菜单\n");

        choice = safeReadInt("请选择: ", 0, 6);

        switch (choice) {

            /* ──────────── 1. 按姓名+身份证查全部关联 ──────────── */
        case 1: {
            Patient* p = promptFindPatient("综合查询");
            if (p == NULL) break;
            strcpy(cardNo, p->data.cardNo);

            printf("\n========== 病人综合信息 ==========\n");
            printf("【基本信息】\n");
            printf("  卡号: %s  姓名: %s  性别: %s  年龄: %d\n",
                p->data.cardNo, p->data.name,
                p->data.gender, p->data.age);
            printf("  电话: %s  身份证: %s\n",
                p->data.phone, p->data.idCard);
            printf("  住院状态: %s\n", p->data.isActive ? "住院中" : "未住院");

            printf("\n【挂号记录】\n");
            found = 0;
            Registration* r = g_regHead;
            while (r != NULL) {
                if (strcmp(r->data.patientCardNo, cardNo) == 0) {
                    found++;
                    char* st = (r->data.status == 0) ? "待就诊" :
                        (r->data.status == 1) ? "就诊中" :
                        (r->data.status == 2) ? "已完成" : "已取消";
                    printf("  %s %s %s %s %s\n",
                        r->data.regNo, r->data.doctorName,
                        r->data.dept, r->data.appointmentDate, st);
                }
                r = r->next;
            }
            if (!found) printf("  无挂号记录\n");

            printf("\n【住院记录】\n");
            found = 0;
            Hospitalization* h = g_hosHead;
            while (h != NULL) {
                if (strcmp(h->data.patientCardNo, cardNo) == 0) {
                    found++;
                    printf("  %s 床位:%s 预交:%.2f 费用:%.2f 状态:%s\n",
                        h->data.recordNo, h->data.bedNo,
                        h->data.prepay, h->data.totalCost, h->data.status);
                }
                h = h->next;
            }
            if (!found) printf("  无住院记录\n");

            printf("\n【购药记录】\n");
            found = 0;
            Purchase* pur = g_purHead;
            while (pur != NULL) {
                if (strcmp(pur->data.patientCardNo, cardNo) == 0) {
                    found++;
                    printf(" 药品:%s 数量:%d 费用:%.2f %s\n",
                   pur->data.medNo,
                        pur->data.quantity, pur->data.totalCost,
                        pur->data.date);
                }
                pur = pur->next;
            }
            if (!found) printf("  无购药记录\n");

            printf("==================================\n");
            break;
        }

              /* ──────────── 2. 按医生查挂号记录 ──────────── */
        case 2: {
            /* 先选科室 */
            char qDeptList[20][50];
            int qDeptCount = 0;
            Doctor* qtmp = g_doctorHead;
            while (qtmp != NULL) {
                int qf = 0;
                for (int i = 0; i < qDeptCount; i++) {
                    if (strcmp(qDeptList[i], qtmp->data.dept) == 0) { qf = 1; break; }
                }
                if (!qf && qDeptCount < 20) {
                    strcpy(qDeptList[qDeptCount], qtmp->data.dept);
                    qDeptCount++;
                }
                qtmp = qtmp->next;
            }
            if (qDeptCount == 0) { printf("[ERROR] 暂无医生\n"); break; }
            printf("\n可选科室：\n");
            for (int i = 0; i < qDeptCount; i++) {
                printf("  %d. %s\n", i + 1, qDeptList[i]);
            }
            int qds = safeReadInt("请选择科室序号: ", 1, qDeptCount);

            /* 再选该科室医生 */
            printf("\n%s 医生列表：\n", qDeptList[qds - 1]);
            printPadded("序号", 6); putchar(' ');
            printPadded("工号", 20); putchar(' ');
            printPadded("姓名", 10); putchar('\n');
            Doctor* qdoc = g_doctorHead;
            Doctor* qDocArr[50] = {0};
            int qDocCount = 0;
            while (qdoc != NULL) {
                if (strcmp(qdoc->data.dept, qDeptList[qds - 1]) == 0) {
                    qDocArr[qDocCount] = qdoc;
                    printf("%-6d ", ++qDocCount);
                    printPadded(qdoc->data.empNo, 20); putchar(' ');
                    printPadded(qdoc->data.name, 10); putchar('\n');
                }
                qdoc = qdoc->next;
            }
            if (qDocCount == 0) { printf("[ERROR] 该科室暂无医生\n"); break; }
            int qsel = safeReadInt("请选择医生序号: ", 1, qDocCount);
            Doctor* d = qDocArr[qsel - 1];
            strcpy(cardNo, d->data.empNo);

            printf("\n【%s（%s）的挂号记录】\n", d->data.name, d->data.dept);
            printPadded("挂号编号", 20); putchar(' ');
            printPadded("患者", 10);     putchar(' ');
            printPadded("预约时间", 12); putchar(' ');
            printPadded("状态", 10);     putchar('\n');
            printf("-----------------------------------------------\n");

            found = 0;
            Registration* r = g_regHead;
            while (r != NULL) {
                if (strcmp(r->data.doctorEmpNo, cardNo) == 0) {
                    found++;
                    char* st = (r->data.status == 0) ? "待就诊" :
                        (r->data.status == 1) ? "就诊中" :
                        (r->data.status == 2) ? "已完成" : "已取消";
                    printPadded(r->data.regNo, 20);           putchar(' ');
                    printPadded(r->data.patientName, 10);     putchar(' ');
                    printPadded(r->data.appointmentTime, 12); putchar(' ');
                    printPadded(st, 10);                      putchar('\n');
                }
                r = r->next;
            }
            if (!found) printf("  暂无挂号记录\n");
            printf("共 %d 条记录\n", found);
            break;
        }

              /* ──────────── 3. 按日期查挂号 ──────────── */
        case 3: {
            safeReadString("请输入日期 (YYYY-MM-DD): ", date, 20);

            printf("\n【%s 挂号记录】\n", date);
            printPadded("挂号编号", 20); putchar(' ');
            printPadded("患者", 10);     putchar(' ');
            printPadded("医生", 10);     putchar(' ');
            printPadded("预约时间", 12); putchar(' ');
            printPadded("状态", 10);     putchar('\n');
            printf("-----------------------------------------------------\n");

            found = 0;
            Registration* r = g_regHead;
            while (r != NULL) {
                if (strcmp(r->data.appointmentDate, date) == 0) {
                    found++;
                    char* st = (r->data.status == 0) ? "待就诊" :
                        (r->data.status == 1) ? "就诊中" :
                        (r->data.status == 2) ? "已完成" : "已取消";
                    printPadded(r->data.regNo, 20);           putchar(' ');
                    printPadded(r->data.patientName, 10);     putchar(' ');
                    printPadded(r->data.doctorName, 10);      putchar(' ');
                    printPadded(r->data.appointmentTime, 12); putchar(' ');
                    printPadded(st, 10);                      putchar('\n');
                }
                r = r->next;
            }
            if (!found) printf("  该日无挂号记录\n");
            printf("共 %d 条\n", found);
            break;
        }

              /* ──────────── 4. 按日期查住院 ──────────── */
        case 4: {
            safeReadString("请输入日期 (YYYY-MM-DD): ", date, 20);

            printf("\n【%s 住院记录】\n", date);
            printPadded("住院单号", 20); putchar(' ');
            printPadded("患者", 10);     putchar(' ');
            printPadded("床位", 10);     putchar(' ');
            printPadded("预交金", 12);   putchar(' ');
            printPadded("状态", 10);     putchar('\n');
            printf("------------------------------------------------\n");

            found = 0;
            Hospitalization* h = g_hosHead;
            while (h != NULL) {
                if (strcmp(h->data.admissionDate, date) == 0) {
                    found++;
                    printPadded(h->data.recordNo, 20);    putchar(' ');
                    printPadded(h->data.patientName, 10); putchar(' ');
                    printPadded(h->data.bedNo, 10);       putchar(' ');
                    printf("%-12.2f ", h->data.prepay);
                    printPadded(h->data.status, 10);      putchar('\n');
                }
                h = h->next;
            }
            if (!found) printf("  该日无入院记录\n");
            printf("共 %d 条\n", found);
            break;
        }

              /* ──────────── 5. 按科室查医生和病人 ──────────── */
        case 5: {
            safeReadString("请输入科室: ", dept, 50);

            printf("\n========== %s 科室信息 ==========\n", dept);

            printf("\n【医生列表】\n");
            found = 0;
            Doctor* d = g_doctorHead;
            printPadded("  工号", 22); putchar(' ');
            printPadded("姓名", 10); putchar(' ');
            printPadded("接诊", 12); putchar('\n');
            while (d != NULL) {
                if (strcmp(d->data.dept, dept) == 0) {
                    found++;
                    printf("  ");
                    printPadded(d->data.empNo, 20); putchar(' ');
                    printPadded(d->data.name, 10);  putchar(' ');
                    printf("%d/%d\n", d->data.currentPatients, d->data.maxPatients);
                }
                d = d->next;
            }
            if (!found) printf("  该科室暂无医生\n");

            printf("\n【今日挂号患者】\n");
            found = 0;
            Registration* r = g_regHead;
            printPadded("  患者卡号", 22); putchar(' ');
            printPadded("姓名", 10); putchar(' ');
            printPadded("预约日期", 12); putchar(' ');
            printPadded("预约时间", 10); putchar('\n');
            while (r != NULL) {
                if (strcmp(r->data.dept, dept) == 0 && r->data.status != 3) {
                    found++;
                    printf("  ");
                    printPadded(r->data.patientCardNo, 20);    putchar(' ');
                    printPadded(r->data.patientName, 10);      putchar(' ');
                    printPadded(r->data.appointmentDate, 12);  putchar(' ');
                    printPadded(r->data.appointmentTime, 10);  putchar('\n');
                }
                r = r->next;
            }
            if (!found) printf("  暂无挂号患者\n");
            printf("==================================\n");
            break;
        }

              /* ──────────── 6. 药品库存明细 ──────────── */
        case 6: {
            printf("\n========== 药品库存明细 ==========\n");
            printPadded("编号", 16);     putchar(' ');
            printPadded("通用名", 14);   putchar(' ');
            printPadded("单价", 10);     putchar(' ');
            printPadded("库存", 10);     putchar(' ');
            printPadded("最低库存", 10); putchar(' ');
            printPadded("状态", 10);     putchar('\n');
            printf("----------------------------------------------------------------\n");

            Medicine* m = g_medHead;
            while (m != NULL) {
                char* status = (m->data.stock <= m->data.minStock) ? "【预警】" : "正常";
                printPadded(m->data.medNo, 16);       putchar(' ');
                printPadded(m->data.genericName, 14); putchar(' ');
                printf("%-10.2f %-10d %-10d ",
                    m->data.price, m->data.stock, m->data.minStock);
                printPadded(status, 10);              putchar('\n');
                m = m->next;
            }
            printf("==================================\n");
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
// 用户管理（管理员专用）
//--------------------
void showUserManagement(void) {
    int choice;
    char username[50];
    while (1) {
        printf("\n-------- 用户管理 --------\n");
        printf("1. 查看所有用户\n");
        printf("2. 修改用户角色\n");
        printf("3. 重置用户密码\n");
        printf("4. 删除用户\n");
        printf("0. 返回上级菜单\n");
        choice = safeReadInt("请选择: ", 0, 4);

        switch (choice) {
        case 1:
            listAllUsers();
            break;
        case 2: {
            safeReadString("请输入要修改的用户名: ", username, 50);
            printf("选择新角色：\n");
            printf("  0. 患者\n  1. 护士\n  2. 医生\n  3. 管理员\n");
            int newRole = safeReadInt("新角色: ", 0, 3);
            if (modifyUserRole(username, (UserRole)newRole)) {
                printf("[OK] 角色修改成功\n");
            } else {
                printf("[ERROR] 未找到该用户\n");
            }
            break;
        }
        case 3:
            safeReadString("请输入要重置密码的用户名: ", username, 50);
            if (resetUserPassword(username)) {
                printf("[OK] 密码已重置为 123456\n");
            } else {
                printf("[ERROR] 未找到该用户\n");
            }
            break;
        case 4:
            safeReadString("请输入要删除的用户名: ", username, 50);
            if (strcmp(username, g_currentUsername) == 0) {
                printf("[ERROR] 不能删除当前登录的账号\n");
                break;
            }
            printf("将删除用户：%s\n", username);
            if (!confirmYesNo("确认删除？")) {
                printf("已取消。\n");
                break;
            }
            if (deleteUser(username)) {
                printf("[OK] 用户已删除\n");
            } else {
                printf("[ERROR] 未找到该用户\n");
            }
            break;
        case 0:
            return;
        }
    }
}

//--------------------
// 首次运行时从 data_seed 复制出 data 目录
//--------------------
static void ensureDataDir(void) {
    FILE* fp = fopen("data/doctor.txt", "r");
    if (fp != NULL) {
        fclose(fp);
        return;
    }
    printf("检测到首次运行，正在从 data_seed/ 初始化数据...\n");
#ifdef _WIN32
    system("xcopy /E /I /Q /Y data_seed data >nul");
#else
    system("cp -r data_seed data");
#endif
    printf("数据初始化完成。\n");
}

//--------------------
//主函数
//--------------------
int main(void) {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif
    ensureDataDir();
    initUI();
    while (1) {
        int status = showLoginPage();
        if (status == LOGIN_EXIT) {
            printf("感谢使用，再见！\n");
            break;
        }
        if (status == LOGIN_FAILED) {
            printf("[ERROR] 登录失败，请重试。\n");
            continue;
        }
        if (status == LOGIN_REGISTERED) {
            printf("注册成功，请登录。\n");
            continue;
        }

        /* 登录成功后根据角色进入主循环 */
        buildPatientChain(&g_patientHead, &g_patientTail);
        buildDoctorChain(&g_doctorHead, &g_doctorTail);
        buildRegistrationChain(&g_regHead, &g_regTail);
        buildMedicineChain(&g_medHead, &g_medTail);
        buildPurchaseChain(&g_purHead, &g_purTail);
        buildHospitalizationChain(&g_hosHead, &g_hosTail);
        buildBedChain(&g_bedHead, &g_bedTail);

        while (1) {
            /* showMainMenuByRole 返回用户选择的模块号 */
            int module = showMainMenuByRole(g_currentUserRole, g_currentUsername);

            switch (module) {
            case 1: showRegistrationManagement();    break;
            case 2: showPatientManagement();         break;
            case 3: showBedManagement();             break;
            case 4: showMedicineManagement();        break;
            case 5: showHospitalizationManagement(); break;
            case 6: showDoctorManagement();          break;
            case 7: showStatisticsMenu();            break;
            case 8: showQueryMenu();                 break;
            case 9: showUserManagement();            break;
            case 0:
                printf("正在退出到登录界面...\n");
                freePatientChain(&g_patientHead);
                freeDoctorChain(&g_doctorHead);
                freeRegistrationChain(&g_regHead);
                freeMedicineChain(&g_medHead);
                freePurchaseChain(&g_purHead);
                freeHospitalizationChain(&g_hosHead);
                freeBedChain(&g_bedHead);
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
