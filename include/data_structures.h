#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// 用户类型枚举 - 仅4种身份
typedef enum {
    PATIENT = 0,    // 患者
    NURSE = 1,      // 护士
    DOCTOR = 2,     // 医生
    ADMIN = 3       // 管理员
} UserType;

// 用户信息结构体
typedef struct User {
    int id;
    char name[50];
    char password[50];
    UserType type;
    char department[30];  // 科室（医生、护士、管理员专属）
    struct User* next;
    struct User* prev;
} User;

// 挂号信息结构体
typedef struct Registration {
    int reg_id;
    int patient_id;
    int doctor_id;
    char patient_name[50];
    char doctor_name[50];
    char department[30];
    time_t reg_time;        // 挂号时间
    time_t appointment_time; // 预约时间
    int status;             // 0-待就诊, 1-正在就诊, 2-已完成, 3-已取消
    struct Registration* next;
    struct Registration* prev;
} Registration;

// 候诊队列节点
typedef struct QueueNode {
    int reg_id;
    int patient_id;
    char patient_name[50];
    time_t queue_time;      // 进入队列时间
    struct QueueNode* next;
} QueueNode;

// 双向链表节点通用结构
typedef struct ListNode {
    void* data;
    struct ListNode* next;
    struct ListNode* prev;
} ListNode;

// 双向链表结构
typedef struct LinkedList {
    ListNode* head;
    ListNode* tail;
    int size;
} LinkedList;

#endif