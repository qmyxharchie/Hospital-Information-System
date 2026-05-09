//---------------
//住院模块
//---------------

/*
**住院管理**：
入院登记（自动分配床位）
出院结算（计算费用、释放床位）
住院状态跟踪。
*/

#ifndef _HOSPITALIZATION_H_
#define _HOSPITALIZATION_H_

//住院记录
typedef struct HospitalizationData 
{
    char recordNo[20];       // 住院单号（如"HOS001"）
    char patientCardNo[20];  // 病人卡号
    char patientName[50];   // 病人姓名
    char bedNo[20];         // 床位号
    double prepay;           // 预交金额
    double totalCost;        // 总费用
    char admissionDate[20];  // 入院日期
    char status[20];         // 状态（在院/已出院）
} HospitalizationData;  // 注：团队头文件中 Hospitalization 即为 HospitalizationData

typedef struct Hospitalization {
    HospitalizationData data;
    struct Hospitalization* next;
    struct Hospitalization* pre;
} Hospitalization;

void addHospitalization(Hospitalization** head, Hospitalization** tail, char* patientCardNo,
    char* patientName, double prepay);    //入院登记
void dischargePatient(Hospitalization* h, double totalCost);                     //出院结算
//查找病人
Hospitalization* findHospitalizationByCardNo(Hospitalization* head, char* cardNo);  // 按病人卡号查住院记录
Hospitalization* findHospitalizationByNo(Hospitalization* head, char* recordNo);    // 按住院单号查住院记录
Hospitalization* findCurrentHospitalizations(Hospitalization* head);                  // 查找当前在院病人
void listAllHospitalizations(Hospitalization* head);  // 所有住院记录列表
void addPrepay(Hospitalization* h, double amount); // 追加预交金（amount > 0）
int modifyHospitalization(Hospitalization* head, char* recordNo, char* newBedNo, double addPrepayAmount);// 修改住院信息：转床 + 追加预交金，返回 1=成功，0=失败
#endif