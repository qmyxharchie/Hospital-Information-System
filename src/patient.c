#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "patient.h"
#include "file_io.h"
#include "utils.h"

//--------------------

//--------------------
//以下为添加病人函数
void addPatient(Patient** head, Patient** tail,
    char name[], char gender[], char idCard[], char phone[]) {
    Patient* newNode = (Patient*)malloc(sizeof(Patient));
    newNode->next = newNode->pre = NULL;                // 申请新节点第一件事就是指针初始化！非常重要
    char id[20];
    generateUniqueId("PT", id);
    // 保存具体信息
    strcpy(newNode->data.cardNo, id);
    strcpy(newNode->data.name, name);
    strcpy(newNode->data.gender, gender);
    strcpy(newNode->data.idCard, idCard);
    strcpy(newNode->data.phone, phone);
    node->data.isActive = 0;
    if (*tail == NULL) {
        // 增加链表第一个节点,该节点即是头又是尾
        *head = newNode;
        *tail = newNode;
    }
    else {
        // 不是第一个节点，则只需接在最后节点(tail)的后面
        (*tail)->next = newNode;
        newNode->pre = *tail;
        *tail = newNode;
    }
    rebuildPatientFile(*head);
    printf("添加成功,卡号为%c", id);

}
//--------------------

//--------------------
//以下为删除病人函数
void delPatient(Patient* head, Patient* tail, PatientData p);
void modifyPatient(Patient* p, char phone[]);                   //修改病人信息
Patient* findPatientByCardNo(Patient* head, char* cardNo);      //按卡号精确查找
Patient* findPatientByName(Patient* head, char* name);          //按姓名模糊/精确查找
void listAllPatients(Patient* head);                            //列表显示