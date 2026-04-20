#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "patient.h"        // 病人模块头文件
#include "doctor.h"         // 医生模块头文件
#include "registration.h"	// 挂号模块头文件
#include "file_io.h"        // 文件输入输出模块
#include "utils.h"          // 工具函数模块

//--------------------

//--------------------
//以下为挂号函数
int addRegistration(Registration** head, Registration** tail,
    char* patientCardNo, char* patientName,
    char* doctorEmpNo, char* doctorName, char* dept) {
    Doctor* d = findDoctorByEmpNo(getDoctorHead(), doctorEmpNo);
    if (d == NULL) {
        printf("未找到此医生\n");
        return 0;
    }
    if (!canAcceptPatient(d)) {
        printf("此医生今日号已满\n");
        return 0;
    }
    Registration* newNode = (Registration*)malloc(sizeof(Registration));
    newNode->next = newNode->pre = NULL;
    char id[20];
    generateUniqueId("REG", id);
    strcpy(newNode->data.regNo, id);                    // 挂号单号
    strcpy(newNode->data.patientCardNo, patientCardNo); // 病人卡号
    strcpy(newNode->data.patientName, patientName);     // 病人姓名
    strcpy(newNode->data.doctorEmpNo, doctorEmpNo);     // 医生工号
    strcpy(newNode->data.doctorName, doctorName);       // 医生姓名
    strcpy(newNode->data.dept, dept);                   // 科室
    int year, month, day;
    char currentDate[20] = { 0 };
    getCurrentTime(&year, &month, &day);
    sprintf(currentDate, "%04d-%02d-%02d", year, month, day);
    strcpy(newNode->data.date, currentDate);            // 挂号日期
    strcpy(newNode->data.status, "待就诊");
    if (*tail == NULL) {
        *head = newNode;
        *tail = newNode;
    }
    else {
        (*tail)->next = newNode;
        newNode->pre = *tail;
        *tail = newNode;
    }
    incrementPatientCount(d);
    rebuildRegistrationFile(*head);
    printf("挂号成功！挂号单号：%s，病人：%s，医生：%s，科室：%s，日期：%s\n",
        newNode->data.regNo, newNode->data.patientName,
        newNode->data.doctorName, newNode->data.dept, newNode->data.date);
    return 1;
}
//--------------------

//--------------------
//以下为取消挂号函数
int cancelRegistration(Registration** head, Registration** tail,
    Registration* r) {
    if (r == NULL) {
        printf("未找到此患者\n");
        return 0;
    }
    if (strcmp(r->data.status, "已完成") == 0) {
        printf("已完成的就诊无法取消\n");
        return 0;
    }
    Doctor* d = findDoctorByEmpNo(getDoctorHead(),r->data.doctorEmpNo);
    d->data.currentPatients--;
    strcpy(r->data.status, "已取消");
    rebuildRegistrationFile(*head);
    printf("取消成功\n");
    return 1;
}
//--------------------

//--------------------
//以下为完成就诊函数
int completeRegistration(Registration* r) {
    if (r == NULL) {
        printf("病人信息错误，请重新输入\n");
        return 0;
    }
    if (strcmp(r->data.status, "待就诊") != 0) {
        printf("病人信息有误\n");
        return 0;
    }
    strcpy(r->data.status, "已就诊");
    printf("病人%s已完成就诊", r->data.patientName);
    return 1;
}
//--------------------

//--------------------
//以下为查询函数

//按挂号单号查询
Registration* findRegistrationByNo(Registration* head, char* regNo) {
    Registration* r = head;
    while (r != NULL) {
        if (strcmp(r->data.regNo, regNo) == 0) {          // 比较挂号单号是否匹配
            return r;                                     // 找到则返回该节点指针
        }
        r = r->next;                                      
    }
    return NULL;
}

//按病人卡号查询
Registration* findRegistrationsByPatient(Registration* head,
    char* patientCardNo) {
    Registration* r = head;
    while (r != NULL) {
        if (strcmp(r->data.patientCardNo, patientCardNo) == 0) { // 比较病人卡号是否匹配
            return r;                                            // 找到则返回该节点指针
        }
        r = r->next;
    }
    return NULL;
}

//按医生查询
Registration* findRegistrationsByDoctor(Registration* head,
    char* doctorEmpNo) {
    Registration* r = head;
    Registration* resultHead = NULL;
    Registration* resultTail = NULL;
    while (r != NULL) {
        if (strcmp(r->data.doctorEmpNo, doctorEmpNo) == 0){
            Registration* newNode = (Registration*)malloc(sizeof(Registration));
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
        }
        r = r->next;
    }
    return resultHead;
}

//按时间范围查询
Registration* findRegistrationsByDateRange(Registration* head, int y1,
    int m1, int d1, int y2, int m2, int d2) {                       //1>2
    Registration* r = head;
    Registration* resultHead = NULL;
    Registration* resultTail = NULL;
    while (r != NULL) {
        int regYear, regMonth, regDay;
        sscanf(r->data.date, "%d-%d-%d", &regYear, &regMonth, &regDay);
        // 使用compareDate函数比较日期
        // 如果注册日期 >= 开始日期 且 注册日期 <= 结束日期
        if (compareDate(regYear, regMonth, regDay, y1, m1, d1) >= 0 &&
            compareDate(regYear, regMonth, regDay, y2, m2, d2) <= 0) {
            Registration* newNode = (Registration*)malloc(sizeof(Registration));
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
        }
        r = r->next;
    }
    return resultHead;
}
//--------------------

//--------------------
//以下为列表显示函数
void listAllRegistrations(Registration* head) {
    // 1. 检查链表是否为空
    if (head == NULL) {
        printf("暂无挂号信息\n");
        return;
    }

    // 2. 输出表头信息
    printf("=== 挂号列表 ===\n");
    printf("%-20s %-50s %-50s %-50s %-20s %-20s\n",
        "挂号单号", "病人姓名", " 医生姓名", "科室", "挂号日期", "状态");

    // 3. 遍历链表并输出每个挂号的信息
    Registration* current = head;                              // 从头节点开始
    while (current != NULL) {
        // 格式化输出当前挂号的各项信息
        printf("%-20s %-50s %-50s %-50s %-20s %-20s\n",
            current->data.empNo,                         // 挂号单号
            current->data.patientName,                   // 病人姓名
            current->data.doctorName,                    // 医生姓名
            current->data.dept,                          // 科室
            current->data.date,                          // 每日最大接诊数
            current->data.status;                        // 今日已接诊数
        current = current->next;                         // 移动到下一个节点
    }
}



