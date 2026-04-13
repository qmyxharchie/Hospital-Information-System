#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "patient.h"
#include "file_io.h"
#include "utils.h"

//--------------------

//--------------------
//以下为添加病人函数
void addPatient(Patient* head, Patient* tail,
    char name[], char gender[], char idCard[], char phone[]) {

}
void delPatient(Patient* head, Patient* tail, PatientData p)    //删除病人
void modifyPatient(Patient* p, char phone[]);                   //修改病人信息
Patient* findPatientByCardNo(Patient* head, char* cardNo);      //按卡号精确查找
Patient* findPatientByName(Patient* head, char* name);          //按姓名模糊/精确查找
void listAllPatients(Patient* head);                            //列表显示