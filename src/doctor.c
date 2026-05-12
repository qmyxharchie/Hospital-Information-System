#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "doctor.h"         // 医生模块头文件
#include "registration.h"   // 挂号模块头文件
#include "file_io.h"        // 文件输入输出模块
#include "utils.h"          // 工具函数模块
#include "ui.h" 

// Getter 函数实现 - 用于其他模块访问本模块的全局变量
Doctor* getDoctorHead(void) {
    // 获取医生链表头指针的函数
    return g_doctorHead;
}

Doctor* getDoctorTail(void) {
    // 获取医生链表尾指针的函数
    return g_doctorTail;
}
//--------------------

//--------------------
// 以下为添加医生函数// 功能：向医生链表中添加一个新的医生节点
// 参数：head - 指向链表头指针的指针，tail - 指向链表尾指针的指针
//      name - 姓名，dept - 科室，maxPatients - 每日最大接诊数
void addDoctor(Doctor** head, Doctor** tail,
    char name[], char dept[], int maxPatients) {
    // 1. 为新节点分配内存空间
    Doctor* newNode = (Doctor*)malloc(sizeof(Doctor));

    // 2. 初始化新节点的指针域，防止野指针
    newNode->next = newNode->pre = NULL;                // 申请新节点第一件事就是指针初始化！非常重要

    // 3. 生成唯一的工号
    char id[20];
    generateUniqueId("DOC", id);                        // 生成以"DOC"开头的唯一ID

    // 4. 保存病人具体信息到新节点的数据域
    strcpy(newNode->data.empNo, id);                    // 工号
    strcpy(newNode->data.name, name);                   // 姓名
    strcpy(newNode->data.dept, dept);                   // 科室
    newNode->data.maxPatients = maxPatients;            // 每日最大接诊次数
    newNode->data.currentPatients = 0;                  // 设置今日接诊数为0

    // 5. 将新节点插入到链表尾部
    if (*tail == NULL) {
        // 如果链表为空（尾指针为NULL），则新节点既是头节点也是尾节点
        *head = newNode;                                
        *tail = newNode;                                
    }
    else {
        // 如果链表不为空，则将新节点连接到尾节点之后
        (*tail)->next = newNode;                        
        newNode->pre = *tail;                           
        *tail = newNode;                                
    }

    // 6. 将更新后的链表数据保存到文件
    rebuildDoctorFile(*head);
}
//--------------------

//--------------------
// 以下为删除医生函数
// 功能：从医生链表中删除指定的医生节点
// 参数：head - 指向链表头指针的指针，tail - 指向链表尾指针的指针
//      d - 要删除的医生数据（根据empNo匹配）
void delDoctor(Doctor** head, Doctor** tail, DoctorData d) {
    // 1. 检查链表是否为空
    if (*head == NULL) {
        printf("[ERROR] 暂无医生数据\n");
        return;
    }

    // 2. 遍历链表寻找要删除的节点
    Doctor* cur = *head;                                
    Doctor* pre = NULL;                                 
    while (cur != NULL) {
        if (strcmp(cur->data.empNo, d.empNo) == 0) {    // 当卡号匹配的时候
            break;                                      // 跳出循环
        }
        pre = cur;                                      
        cur = cur->next;                                
    }

    // 3. 检查是否找到要删除的节点
    if (cur == NULL) {
        printf("[ERROR] 未找到该医生，无法删除\n");
        return;
    }

    // 4. 根据要删除节点的位置执行不同的删除操作
    if (cur == *head && cur == *tail) {
        *head = NULL;                                   // 1.链表只有一个节点，删除后链表变空
        *tail = NULL;
    }
    else if (cur == *head) {
        *head = cur->next;                              // 2.删除头节点，头指针指向下一个节点
        (*head)->pre = NULL;                            // 新头节点的前驱设为NULL
    }
    else if (cur == *tail) {
        *tail = cur->pre;                               // 3.删除尾节点，尾指针指向前一个节点
        (*tail)->next = NULL;                           // 新尾节点的后继设为NULL
    }
    else {
        pre->next = cur->next;                          // 4.删除中间节点，前节点的next指向后节点
        cur->next->pre = pre;                           // 后节点的pre指向前节点
    }

    // 5. 释放被删除节点的内存
    free(cur);

    // 6. 输出删除成功的提示信息
    printf("[OK] 删除成功。\n");

    // 7. 将更新后的链表数据保存到文件
    rebuildDoctorFile(*head);
}//--------------------

//--------------------
// 以下为修改医生信息函数
// 功能：修改指定工号的医生信息
// 参数：head - 链表头指针，empNo - 要修改的医生工号，newData - 新的医生数据
// 返回值：1-修改成功，0-修改失败
int modifyDoctor(Doctor* head, char* empNo, DoctorData newData) {
    Doctor* target = NULL;

    // 1. 根据卡号查找要修改的医生节点
    target = findDoctorByEmpNo(head, empNo);

    // 2. 检查是否找到要修改的医生
    if (target == NULL) {
        printf("[ERROR] 未找到工号为%s的医生。\n", empNo);
        return 0;                                       // 返回0表示修改失败
    }

    // 3. 显示当前医生信息
    printf("当前医生信息为：\n");
    printf("工号：%s\n", target->data.empNo);
    printf("姓名：%s\n", target->data.name);
    printf("科室：%s\n", target->data.dept);
    printf("每日最大接诊次数：%d\n", target->data.maxPatients);
    printf("今日已接诊数：%d\n", target->data.currentPatients);

    // 4. 更新医生信息
    safeStringCopy(target->data.name, newData.name, 50);          // 安全复制姓名
    safeStringCopy(target->data.dept, newData.dept, 50);          // 安全复制工号
    target->data.maxPatients = newData.maxPatients;               // 更新每日最大接诊数
    // 注意：currentPatients 是运行时统计量（当日挂号计数），不由修改操作覆盖

    // 5. 将更新后的链表数据保存到文件
    rebuildDoctorFile(head);

    // 6. 输出修改成功的提示信息
    printf("[OK] 医生信息修改成功！\n");

    return 1;                                             // 返回1表示修改成功
}
//--------------------

//--------------------
// 以下为查找医生信息函数

// 按工号精确查找
// 功能：根据工号精确查找医生
// 参数：head - 链表头指针，empNo - 要查找的工号
// 返回值：找到的医生节点指针，未找到返回NULL
Doctor* findDoctorByEmpNo(Doctor* head, char* empNo) {
    Doctor* d = head;                                     
    while (d != NULL) {
        if (strcmp(d->data.empNo, empNo) == 0) {          // 比较工号是否匹配
            return d;                                     // 找到则返回该节点指针
        }
        d = d->next;                                      
    }
    return NULL;                                          
}

// 按姓名模糊
// 功能：根据姓名模糊查找医生（查找姓名中包含指定字符串的医生）
// 参数：head - 链表头指针，name - 要查找的姓名片段
// 返回值：如果找到返回头节点指针，未找到返回NULL
Doctor* findDoctorsByName(Doctor* head, char* name) {
    Doctor* d = head;                                     // 从头节点开始查找
    Doctor* resultHead = NULL;
    Doctor* resultTail = NULL;
    // 遍历整个链表
    while (d != NULL) {
        if (strstr(d->data.name, name) != NULL) {
            Doctor* newNode = (Doctor*)malloc(sizeof(Doctor));
            newNode->data = d->data;
            newNode->next = NULL;
            newNode->pre = NULL;
            if (resultHead == NULL) {
                resultHead = newNode;
                resultTail = newNode;
            }
            else {
                resultTail->next = newNode;
                newNode->pre = resultTail;
                resultTail = newNode;
            }
        }
        d = d->next;                                      // 移动到下一个节点
    }
    return resultHead;    
}

// 按科室查找（返回第一个匹配）
// 功能：根据科室查找医生（返回第一个匹配的医生）
// 参数：head - 链表头指针，dept - 要查找的科室
// 返回值：找到的医生节点指针，未找到返回NULL
Doctor* findDoctorsByDept(Doctor* head, char* dept) {
    Doctor* d = head;                                     // 从头节点开始查找
    while (d != NULL) {
        if (strcmp(d->data.dept, dept) == 0) {            // 比较科室是否匹配
            return d;                                     // 找到则返回该节点指针
        }
        d = d->next;                                      // 移动到下一个节点
    }
    return NULL;                                          // 未找到返回NULL
}
//--------------------

//--------------------
// 查看候诊列表
// 功能：医生查看自己的候诊列表
// 参数：regHead — 挂号链表头，doctorEmpNo — 医生工号
//--------------------
void doctorViewWaitingList(Registration* regHead, char* doctorEmpNo)
{
    int year, month, day;
    getCurrentTime(&year, &month, &day);
    char today[20];
    sprintf(today, "%04d-%02d-%02d", year, month, day);

    // 生成有序候诊队列
    Registration* queue = buildWaitingQueue(regHead, doctorEmpNo, today);

    // 找到医生姓名
    Doctor* d = findDoctorByEmpNo(g_doctorHead, doctorEmpNo);
    char* doctorName = (d != NULL) ? d->data.name : doctorEmpNo;

    listWaitingQueue(queue, doctorName);

    // 释放临时队列（不释放原始数据！）
    Registration* cur = queue;
    while (cur != NULL) {
        Registration* tmp = cur;
        cur = cur->next;
        free(tmp);
    }
}
//--------------------

//--------------------
// 医生叫号入口（封装 callNextPatient）
//--------------------
void doctorCallNextPatient(Registration** regHead, Registration** regTail,
    char* doctorEmpNo)
{
    int result = callNextPatient(regHead, regTail, doctorEmpNo);
    if (result == 1) {
        // 叫号成功后显示剩余候诊人数
        int year, month, day;
        getCurrentTime(&year, &month, &day);
        char today[20];
        sprintf(today, "%04d-%02d-%02d", year, month, day);

        Registration* cur = *regHead;
        int remaining = 0;
        while (cur != NULL) {
            if (strcmp(cur->data.doctorEmpNo, doctorEmpNo) == 0 &&
                strcmp(cur->data.appointmentDate, today) == 0 &&
                cur->data.status == PENDING) {
                remaining++;
            }
            cur = cur->next;
        }
        printf("[INFO] 该医生还有 %d 位患者在候诊\n", remaining);
    }
}
//--------------------

// 以下为医生接诊检查函数

// 医生是否还能接诊（1=能, 0=不能）
// 功能：检查医生是否还能接诊（是否达到最大接诊数）
// 参数：d - 医生节点指针
// 返回值：1-还能接诊，0-已达最大接诊数
int canAcceptPatient(Doctor* d) {
    if (d->data.currentPatients < d->data.maxPatients) {
        return 1;
    }
    else {
        return 0;
    }
}

// 挂号成功后接诊数+1
// 功能：挂号成功后增加医生的接诊数
// 参数：d - 医生节点指针
void incrementPatientCount(Doctor* d) {
    if (d == NULL) return;                              // 边界检查
    if (canAcceptPatient(d)) {
        d->data.currentPatients++;
    }
    else {
        printf("此医生已达每日最大接诊次数\n");
    }
}
//--------------------

//--------------------
// 以下为重置每日接诊数函数
// 功能：重置所有医生的每日接诊数为0
// 参数：head - 链表头指针
void resetDailyPatients(Doctor* head) {
    Doctor* cur = head;
    while (cur != NULL) {
        cur->data.currentPatients = 0;
        cur = cur->next;
    }
}
//--------------------

//--------------------
// 以下为科室统计函数
// 功能：统计指定科室在当前日期的总接待量
// 参数：head - 链表头指针，dept - 科室名称，count - 指向统计结果的指针
void getDeptStats(Doctor* head, char* dept, char* date, int* count) {
    if (head == NULL || dept == NULL || count == NULL) {
        printf("参数错误：头指针、科室名称或计数指针为空\n");
        return;
    }
    char currentDate[20] = { 0 };
    if (date == NULL || strlen(date) == 0) {
        int year, month, day;
        getCurrentTime(&year, &month, &day);
        sprintf(currentDate, "%04d-%02d-%02d", year, month, day);
        date = currentDate;
    }
    Doctor* cur = head;
    *count = 0;
    printf("=== %s科室 %s 日接待量统计 ===\n", dept, date);
    while (cur != NULL) {
        if (strcmp(cur->data.dept, dept) == 0) {
            *count += cur->data.currentPatients;
        }
        cur = cur->next;
    }
    if (*count == 0) {
        printf("%s科室在%s暂无接诊记录。\n", dept, date);
    }
    else {
        printf("%s科室在%s总接待量：%d 人次\n", dept, date, *count);
    }
}
//--------------------

//--------------------
// 以下为列表显示函数
// 功能：遍历并显示所有医生信息
// 参数：head - 链表头指针
void listAllDoctors(Doctor* head) {
    // 1. 检查链表是否为空
    if (head == NULL) {
        printf("暂无医生信息\n");
        return;
    }

    // 2. 输出表头信息
    printf("=== 医生列表 ===\n");
    printPadded("工号", 20); putchar(' ');
    printPadded("姓名", 12); putchar(' ');
    printPadded("科室", 10); putchar(' ');
    printPadded("每日最大接诊数", 16); putchar(' ');
    printPadded("今日已接诊数", 14); putchar('\n');

    // 3. 遍历链表并输出每个医生的信息
    Doctor* current = head;
    while (current != NULL) {
        printPadded(current->data.empNo, 20); putchar(' ');
        printPadded(current->data.name, 12); putchar(' ');
        printPadded(current->data.dept, 10); putchar(' ');
        printf("%-16d %-14d\n",
            current->data.maxPatients,
            current->data.currentPatients);
        current = current->next;                         // 移动到下一个节点
    }
}
//--------------------

//--------------------
// 以下为按科室打印函数
// 功能：按科室分组打印医生人数
// 参数：head - 链表头指针
void statDoctorByDept(void) {
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
    printf("%-20s %-10s\n", "科室", "医生人数");
    printf("------------------------------\n");
    for (int i = 0; i < deptCount; i++) {
        printf("%-20s %-10d\n", depts[i], counts[i]);
    }
}







