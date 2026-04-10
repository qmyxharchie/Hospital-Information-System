//---------------
//药品模块
//---------------

#ifndef _HOSPITALIZATION_H_
#define _HOSPITALIZATION_H_

//住院记录
typedef struct HospitalizationData {
    char recordNo[20];           // 住院单号（如HOS001）
    char patientCardNo[20];      // 病人卡号
    char patientName[50];       // 病人姓名
    char bedNo[20];             // 床位号
    double prepay;              // 预交金额
    double totalCost;           // 总费用
    char admissionDate[20];     // 入院日期
    char status[20];            // 状态（在院/已出院）
} Hospitalization;

struct HospitalizationNode {
    Hospitalization data;
    struct Hospitalization* next;
    struct Hospitalization* pre;
};

//床位信息
typedef struct BedData {
    char ward[30];               // 病区
    char bedNo[20];             // 床位号
    char patientCardNo[20];    // 病人卡号（空=无病人）
    char patientName[50];      // 病人姓名
    char status[20];           // 状态（空闲/占用/维修）
} Bed;

struct BedNode {
    Bed data;
    struct Bed* next;
    struct Bed* pre;
};

allocateBed(Bed* head, char* patientCardNo, char* patientName);              //自动分配床位

freeBed(Bed* b);                                                             //释放床位

addHospitalization(Hospitalization** head, Hospitalization** tail, ...);    //入院登记

dischargePatient(Hospitalization* h, double totalCost);                     //出院结算

findHospitalizationByCardNo(Hospitalization* head, char* cardNo);           //查询住院信息

listAllHospitalizations(Hospitalization* head);                             //住院信息列表

listAllBeds(Bed* head);                                                     //床位信息列表

listBedsByWard(Bed* head, char* ward);                                      //分病区床位列表

#endif