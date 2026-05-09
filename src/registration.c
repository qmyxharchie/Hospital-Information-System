#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "patient.h"        // 患者模块头文件
#include "doctor.h"         // 医生模块头文件
#include "registration.h"	// 挂号模块头文件
#include "login.h"          // 登录模块头文件
#include "file_io.h"        // 文件输入输出模块
#include "utils.h"          // 工具函数模块
#include "ui.h" 




//-------------------------
//函数名：addRegistration
//功能：为患者办理挂号手续，将患者与医生关联
//参数：head - 指向挂号记录链表头部的指针的指针
//   tail - 指向挂号记录链表尾部的指针的指针
//   patientCardNo - 患者卡号，patientName - 患者姓名
//   doctorEmpNo - 医生工号，doctorName - 医生姓名，dept - 科室
//   appointmentDate — 预约日期（YYYY-MM-DD），现场挂号时传当天日期
//   appointmentTime — 预约时间（HH:MM），现场挂号时传当前时间
//   createdBy — PATIENT（患者预约）或 NURSE（护士现场挂号）
//返回值：成功返回1，失败返回0
int addRegistration(Registration** head, Registration** tail, 
    char* patientCardNo, char* patientName,
    char* doctorEmpNo, char* doctorName, char* dept,
    char* appointmentDate, char* appointmentTime, UserRole createdBy){
    
    // 1. 查找指定医生是否存在
    Doctor* d = findDoctorByEmpNo(getDoctorHead(), doctorEmpNo);
    if (d == NULL) {
        printf("[ERROR]未找到工号为%s 医生\n",doctorEmpNo);
        return 0;
    }


    // 2. 检查医生是否还有名额（仅对当天挂号进行号源检查）
    int year, month, day;
    getCurrentTime(&year, &month, &day);
    char today[20];
    sprintf(today, "%04d-%02d-%02d", year, month, day);

    if (strcmp(appointmentDate, today) == 0) {
        if (!canAcceptPatient(d)) {
            printf("[ERROR] 该医生今日号源已满（最大接诊 %d 人）\n", d->data.maxPatients);
            return 0;
        }
    }
    
    // 3. 重复挂号检查：同患者同一天同医生不能重复挂 PENDING 状态
    Registration* temp = *head;
    while (temp != NULL) {
        if (strcmp(temp->data.patientCardNo, patientCardNo) == 0 &&
            strcmp(temp->data.doctorEmpNo, doctorEmpNo) == 0 &&
            strcmp(temp->data.appointmentDate, appointmentDate) == 0 &&
            (temp->data.status == PENDING || temp->data.status == IN_PROGRESS)) {
            printf("[ERROR] 该患者已在 %s 预约了该医生，不能重复挂号\n", appointmentDate);
            return 0;
        }
        temp = temp->next;
    }

    // 4. 分配新节点
    Registration* newNode = (Registration*)malloc(sizeof(Registration));
    if (newNode == NULL) {
        printf("[ERROR] 内存分配失败\n");
        return 0;
    }
    newNode->next = newNode->pre = NULL;
    
    // 5. 生成挂号单号
    char id[20];
    generateUniqueId("REG", id);
    strcpy(newNode->data.regNo, id);                    // 挂号编号
    
    // 6. 填充患者和医生信息
    strcpy(newNode->data.patientCardNo, patientCardNo); // 患者卡号
    strcpy(newNode->data.patientName, patientName);     // 患者姓名
    strcpy(newNode->data.doctorEmpNo, doctorEmpNo);     // 医生工号
    strcpy(newNode->data.doctorName, doctorName);       // 医生姓名
    strcpy(newNode->data.dept, dept);                   // 科室
    
    // 7. 设置挂号日期时间（系统当前时间）
    int hour, minute, second;
    time_t now = time(0);
    struct tm* timeinfo = localtime(&now);
    getCurrentTime(&year, &month, &day);
    hour = timeinfo->tm_hour;
    minute = timeinfo->tm_min;
    second = timeinfo->tm_sec;

    sprintf(newNode->data.date, "%04d-%02d-%02d", year, month, day);
    sprintf(newNode->data.time, "%02d:%02d:%02d", hour, minute, second);

    // 8. 设置预约日期时间（由调用方传入）
    strcpy(newNode->data.appointmentDate, appointmentDate);
    strcpy(newNode->data.appointmentTime, appointmentTime);

    // 9. 设置初始状态和挂号方式
    newNode->data.status = PENDING;
    newNode->data.createdBy = createdBy;
    newNode->data.consultationFee = 0.0f;
    strcpy(newNode->data.remarks, "");

    // 10. 尾插法插入链表
    if (*tail == NULL) {
        *head = newNode;
        *tail = newNode;
    }
    else {
        (*tail)->next = newNode;
        newNode->pre = *tail;
        *tail = newNode;
    }
    
    // 11. 增加医生的当前患者数量
    incrementPatientCount(d);
    
    // 12. 保存到文件
    rebuildRegistrationFile(*head);

    // 13. 输出成功信息（根据挂号方式区分提示）
    if (createdBy == PATIENT) {
        printf("[OK] 预约挂号成功！\n");
        printf("  挂号编号：%s\n", newNode->data.regNo);
        printf("  患者：%s\n", newNode->data.patientName);
        printf("  医生：%s（%s）\n", newNode->data.doctorName, newNode->data.dept);
        printf("  预约日期：%s %s\n", newNode->data.appointmentDate, newNode->data.appointmentTime);
    }
    else {
        printf("[OK] 现场挂号成功！\n");
        printf("  挂号编号：%s\n", newNode->data.regNo);
        printf("  患者：%s\n", newNode->data.patientName);
        printf("  医生：%s（%s）\n", newNode->data.doctorName, newNode->data.dept);
        printf("  挂号日期：%s（今日）\n", newNode->data.date);
    }
  
    return 1;
}
//-------------------------

//-------------------------
//函数名：cancelRegistration
//功能：取消患者的挂号记录
//参数：head - 指向挂号记录链表头部的指针的指针
//      tail - 指向挂号记录链表尾部的指针的指针
//      r - 要取消的挂号记录节点指针
//返回值：成功返回1，失败返回0
int cancelRegistration(Registration** head, Registration** tail,
    Registration* r) {

    if (r == NULL) {
        printf("[ERROR] 未找到对应记录\n");
        return 0;
    }

    // 只有 PENDING 状态的才能取消
    if (r->data.status != PENDING) {
        printf("[ERROR] 只有待就诊状态的挂号才能取消（当前状态：%d）\n", r->data.status);
        return 0;
    }

    // 获取当天日期
    int year, month, day;
    getCurrentTime(&year, &month, &day);
    char today[20];
    sprintf(today, "%04d-%02d-%02d", year, month, day);

    // 只有当天挂号才减少医生接诊数（释放号源）
    if (strcmp(r->data.appointmentDate, today) == 0) {
        Doctor* d = findDoctorByEmpNo(g_doctorHead, r->data.doctorEmpNo);
        if (d != NULL && d->data.currentPatients > 0) {
            d->data.currentPatients--;
        }
    }

    // 修改状态为已取消
    r->data.status = CANCELLED;

    // 保存文件
    rebuildRegistrationFile(*head);
    printf("[OK] 挂号 %s 已取消，号源已释放\n", r->data.regNo);
    return 1;
}
//-------------------------

//-------------------------
//函数名：completeRegistration
//功能：完成挂号记录（标记为已就诊）
//参数：r - 挂号记录节点指针
//返回值：成功返回1，失败返回0
int completeRegistration(Registration* r) {
    if (r == NULL) {
        printf("挂号信息不能为空\n");
        return 0;
    }

    // 检查挂号状态是否为"待就诊"或"就诊中"
    if (r->data.status != PENDING &&
        r->data.status != IN_PROGRESS) {
        printf("挂号信息状态异常，无法完成就诊\n");
        return 0;
    }

    // 修改状态为"已完成"
    r->data.status = COMPLETED;

    printf("患者%s就诊完成\n", r->data.patientName);

    return 1;
}
//-------------------------

//-------------------------
// 生成指定医生的当天候诊队列
// 功能：筛选出 doctorEmpNo 匹配、当天预约、PENDING 状态的挂号，按时间排序
// 参数：head — 挂号链表头，doctorEmpNo — 医生工号，today — 当天日期（YYYY-MM-DD）
// 返回值：新的有序链表头指针（调用方负责释放）
Registration* buildWaitingQueue(Registration* head, char* doctorEmpNo, char* today){
    Registration* resultHead = NULL;
    Registration* resultTail = NULL;
    Registration* cur = head;

    while (cur != NULL) {
        // 筛选条件：同医生、当天、待就诊
        if (strcmp(cur->data.doctorEmpNo, doctorEmpNo) == 0 &&
            strcmp(cur->data.appointmentDate, today) == 0 &&
            cur->data.status == PENDING) {

            // 创建新节点复制数据
            Registration* newNode = (Registration*)malloc(sizeof(Registration));
            if (newNode == NULL) continue;
            newNode->data = cur->data;
            newNode->next = newNode->pre = NULL;

            // 按 appointmentTime 插入有序链表（升序）
            if (resultHead == NULL) {
                resultHead = newNode;
                resultTail = newNode;
            }
            else {
                Registration* pos = resultHead;
                Registration* prev = NULL;
                while (pos != NULL &&
                    strcmp(pos->data.appointmentTime, newNode->data.appointmentTime) < 0) {
                    prev = pos;
                    pos = pos->next;
                }
                if (prev == NULL) {
                    // 插入头部
                    newNode->next = resultHead;
                    resultHead->pre = newNode;
                    resultHead = newNode;
                }
                else {
                    newNode->next = pos;
                    newNode->pre = prev;
                    prev->next = newNode;
                    if (pos != NULL) pos->pre = newNode;
                    else resultTail = newNode;
                }
            }
        }
        cur = cur->next;
    }

    return resultHead;
}
//-------------------------

//-------------------------
//函数名：listWaitingQueue
//功能：显示指定医生的候诊队列
//参数：queueHead - 候诊链表
//      doctorEmpNo - 医生姓名
void listWaitingQueue(Registration* queueHead, char* doctorName)
{
    if (queueHead == NULL) {
        printf("[INFO] %s 医生当前无候诊患者\n", doctorName);
        return;
    }

    printf("\n========== %s 医生 候诊队列 ==========\n", doctorName);
    printf("%-6s %-20s %-15s %-10s\n", "序号", "患者姓名", "预约时间", "挂号方式");
    printf("─────────────────────────────────────────────\n");

    Registration* cur = queueHead;
    int seq = 1;
    while (cur != NULL) {
        char* method = (cur->data.createdBy == PATIENT) ? "预约" : "现场";
        printf("%-6d %-20s %-15s %-10s\n",
            seq, cur->data.patientName, cur->data.appointmentTime, method);
        cur = cur->next;
        seq++;
    }
    printf("─────────────────────────────────────────────\n");
    printf("共 %d 人候诊\n", seq - 1);
}
//-------------------------

//-------------------------
// 医生叫号
// 功能：按挂号时间顺序叫下一位待就诊患者
// 参数：head/tail — 挂号链表，doctorEmpNo — 医生工号
// 返回值：1=叫号成功，0=无待就诊患者
int callNextPatient(Registration** head, Registration** tail, char* doctorEmpNo) {
    if (head == NULL || *head == NULL) {
        printf("[INFO] 暂无挂号记录\n");
        return 0;
    }

    // 1. 查找该医生的第一个 PENDING 状态挂号（按预约时间排序）
    Registration* current = *head;
    Registration* target = NULL;

    while (current != NULL) {
        if (strcmp(current->data.doctorEmpNo, doctorEmpNo) == 0 &&
            current->data.status == PENDING) {
            // 找到第一个匹配的后，与 target 比较预约时间，取更早的
            if (target == NULL) {
                target = current;
            }
            else {
                // 比较 appointmentDate，更早的优先
                int cmpDate = compareDateStr(current->data.appointmentDate,
                    target->data.appointmentDate);
                if (cmpDate < 0) {
                    target = current;
                }
                else if (cmpDate == 0) {
                    // 日期相同则比较 appointmentTime
                    if (strcmp(current->data.appointmentTime,
                        target->data.appointmentTime) < 0) {
                        target = current;
                    }
                }
            }
        }
        current = current->next;
    }

    // 2. 检查是否找到待就诊患者
    if (target == NULL) {
        printf("[INFO] 该医生暂无待就诊患者\n");
        return 0;
    }

    // 3. 修改状态为"就诊中"
    target->data.status = IN_PROGRESS;

    // 4. 保存到文件
    rebuildRegistrationFile(*head);

    // 5. 输出叫号信息（模拟诊室广播）
    printf("\n╔═══════════════════════════════════════════╗\n");
    printf("║           叫 号 通 知                     ║\n");
    printf("╠═══════════════════════════════════════════╣\n");
    printf("║  请患者 %-20s 到 %-10s 诊室 ║\n",
        target->data.patientName, target->data.doctorName);
    printf("║  挂号编号：%-30s     ║\n", target->data.regNo);
    printf("╚═══════════════════════════════════════════╝\n");

    return 1;
}
//-------------------------

//-------------------------
//函数名：findRegistrationByNo
//功能：根据挂号编号查询
//参数：head - 挂号记录链表头指针
//      regNo - 挂号编号
//返回值：返回找到的挂号记录节点指针，未找到返回NULL
Registration* findRegistrationByNo(Registration* head, char* regNo) {
    Registration* r = head;
    while (r != NULL) {
        if (strcmp(r->data.regNo, regNo) == 0) {          // 比较挂号编号是否匹配
            return r;                                     // 找到则返回该节点指针
        }
        r = r->next;                                      
    }
    return NULL;
}
//-------------------------

//-------------------------
//函数名：findRegistrationsByPatient
//功能：根据患者卡号查询
//参数：head - 挂号记录链表头指针
//      patientCardNo - 患者卡号
//返回值：返回找到的挂号记录节点指针，未找到返回NULL
Registration* findRegistrationsByPatient(Registration* head,
    char* patientCardNo) {
    Registration* r = head;
    while (r != NULL) {
        if (strcmp(r->data.patientCardNo, patientCardNo) == 0) { // 比较患者卡号是否匹配
            return r;                                            // 找到则返回该节点指针
        }
        r = r->next;
    }
    return NULL;
}
//-------------------------

//-------------------------
//函数名：findRegistrationsByDoctor
//功能：根据医生查询
//参数：head - 挂号记录链表头指针
//      doctorEmpNo - 医生工号
//返回值：返回包含符合条件挂号记录的新链表头指针
Registration* findRegistrationsByDoctor(Registration* head,
    char* doctorEmpNo) {
    Registration* r = head;
    Registration* resultHead = NULL;
    Registration* resultTail = NULL;
    
    int count = 0;
    while (r != NULL) {
        if (strcmp(r->data.doctorEmpNo, doctorEmpNo) == 0){
            // 创建新节点并复制数据
            Registration* newNode = (Registration*)malloc(sizeof(Registration));
            if (newNode != NULL) {
                newNode->data = r->data;
                newNode->next = NULL;
                newNode->pre = NULL;
                if (resultHead == NULL){
                    resultHead = newNode;
                    resultTail = newNode;
                }
                else{
                    resultTail->next = newNode;
                    newNode->pre = resultTail;
                    resultTail = newNode;
                }
                count++;
            }
        }
        r = r->next;
    }
    
    return resultHead;
}
//-------------------------

//-------------------------
//函数名：findRegistrationsByDateRange
//功能：按日期范围查询
//参数：head - 挂号记录链表头指针
//      y1 - 起始年份
//      m1 - 起始月份
//      d1 - 起始日期
//      y2 - 结束年份
//      m2 - 结束月份
//      d2 - 结束日期
//返回值：返回包含符合条件挂号记录的新链表头指针
Registration* findRegistrationsByDateRange(Registration* head, int y1,
    int m1, int d1, int y2, int m2, int d2) {
    Registration* r = head;
    Registration* resultHead = NULL;
    Registration* resultTail = NULL;
    
    int count = 0;
    while (r != NULL) {
        int regYear, regMonth, regDay;
        sscanf(r->data.date, "%d-%d-%d", &regYear, &regMonth, &regDay);
        // 使用compareDate函数比较日期
        // 注意：>= 起始日期 且 <= 结束日期
        if (compareDate(regYear, regMonth, regDay, y1, m1, d1) >= 0 &&
            compareDate(regYear, regMonth, regDay, y2, m2, d2) <= 0) {
            Registration* newNode = (Registration*)malloc(sizeof(Registration));
            if (newNode != NULL) {
                newNode->data = r->data;
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
                count++;
            }
        }
        r = r->next;
    }
    
    return resultHead;
}
//-------------------------

//-------------------------
//函数名：findRegistrationsByStatus
//功能：按状态查询
//参数：head - 挂号记录链表头指针
//      status - 状态
// 返回值：新分配的结果链表头指针（需调用方释放）
Registration* findRegistrationsByStatus(Registration* head, RegStatus status)
{
    Registration* resultHead = NULL;
    Registration* resultTail = NULL;
    Registration* cur = head;

    while (cur != NULL) {
        if (cur->data.status == status) {
            Registration* newNode = (Registration*)malloc(sizeof(Registration));
            if (newNode != NULL) {
                newNode->data = cur->data;
                newNode->next = newNode->pre = NULL;
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
        }
        cur = cur->next;
    }

    return resultHead;
}
//-------------------------

//-------------------------
//函数名：listAllRegistrations
//功能：显示所有挂号列表
//参数：head - 挂号记录链表头指针
//返回值：无
void listAllRegistrations(Registration* head) {
    if (head == NULL) {
        printf("[ERROR] 暂无挂号信息\n");
        return;
    }

    printf("\n=== 挂号记录列表 ===\n");
    printf("%-14s %-10s %-10s %-10s %-12s %-10s %-10s %-8s\n",
        "挂号编号", "患者姓名", "医生姓名", "科室",
        "预约时间", "状态", "方式", "日期");

    Registration* cur = head;
    int count = 0;
    while (cur != NULL) {
        // 状态转字符串
        char* statusStr;
        switch (cur->data.status) {
        case PENDING:     statusStr = "待就诊"; break;
        case IN_PROGRESS: statusStr = "就诊中"; break;
        case COMPLETED:   statusStr = "已完成"; break;
        case CANCELLED:   statusStr = "已取消"; break;
        default:          statusStr = "未知";
        }

        // 方式转字符串
        char* methodStr = (cur->data.createdBy == PATIENT) ? "预约" : "现场";

        printf("%-14s %-10s %-10s %-10s %-12s %-10s %-10s %-8s\n",
            cur->data.regNo,
            cur->data.patientName,
            cur->data.doctorName,
            cur->data.dept,
            cur->data.appointmentTime,
            statusStr,
            methodStr,
            cur->data.appointmentDate);

        cur = cur->next;
        count++;
    }

    printf("[OK] 共 %d 条记录\n", count);
}

//-------------------------