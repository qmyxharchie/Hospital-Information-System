//---------------
//住院模块
//---------------

#ifndef _HOSPITALIZATION_H_
#define _HOSPITALIZATION_H_

//住院记录
typedef struct HospitalizationData {
<<<<<<< HEAD
    char recordNo[20];       // 住院单号（如"HOS001"）
    char patientCardNo[20];  // 病人卡号
    char patientName[50];   // 病人姓名
    char bedNo[20];         // 床位号
    double prepay;           // 预交金额
    double totalCost;        // 总费用
    char admissionDate[20];  // 入院日期
    char status[20];         // 状态（在院/已出院）
} HospitalizationData;  // 注：团队头文件中 Hospitalization 即为 HospitalizationData

typedef struct HospitalizationNode {
    HospitalizationData data;
    struct Hospitalization* next;
    struct Hospitalization* pre;
} Hospitalization;
=======
    char recordNo[20];           // 住院单号（如HOS001）
    char patientCardNo[20];      // 病人卡号
    char patientName[50];       // 病人姓名
    char bedNo[20];             // 床位号
    double prepay;              // 预交金额
    double totalCost;           // 总费用
    char admissionDate[20];     // 入院日期
    char status[20];            // 状态（在院/已出院）
} HospitalizationData;

typedef struct Hospitalization {
    HospitalizationData data;
    struct Hospitalization* next;
    struct Hospitalization* pre;
}Hospitalization;
>>>>>>> 1ce3e5a8ce9248a9bd6bf01fae9c36890b09dc2a


void addHospitalization(Hospitalization** head, Hospitalization** tail, ...);    //入院登记

<<<<<<< HEAD
void dischargePatient(Hospitalization* h, double totalCost);                     //出院结算

Hospitalization* findHospitalizationByCardNo(Hospitalization* head, char* cardNo);  // 按病人卡号查住院记录

void listAllHospitalizations(Hospitalization* head);  // 所有住院记录列表
=======
void addHospitalization(Hospitalization** head,                             //入院登记
    Hospitalization** tail, ...);
dischargePatient(Hospitalization* h, double totalCost);                     //出院结算

findHospitalizationByCardNo(Hospitalization* head, char* cardNo);           //查询住院信息

listAllHospitalizations(Hospitalization* head);                             //住院信息列表

listAllBeds(Bed* head);                                                     //床位信息列表

listBedsByWard(Bed* head, char* ward);                                      //分病区床位列表
>>>>>>> 1ce3e5a8ce9248a9bd6bf01fae9c36890b09dc2a

#endif