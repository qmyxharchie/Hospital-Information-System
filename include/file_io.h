//===============
//	  文件IO
//===============

/*读取文件（从文件加载到链表）
 -写入文件（从链表保存到文件）
 -数据备份*/

#ifndef _FILE_IO
#define _FILE_IO

#define DATA_DIR "data/"
#define BACKUP_DIR "backup/"
#define PATIENT_FILE      DATA_DIR "patient.txt"
#define DOCTOR_FILE       DATA_DIR "doctor.txt"
#define MEDICINE_FILE     DATA_DIR "medicine.txt"
#define PURCHASE_FILE     DATA_DIR "purchase.txt"
#define HOSPITAL_FILE     DATA_DIR "hospitalization.txt"
#define BED_FILE          DATA_DIR "bed.txt"
#define REGISTRATION_FILE DATA_DIR "registration.txt"
#define USER_FILE         DATA_DIR "user.txt"

//读取文件
void buildPatientChain(Patient** head, Patient** tail);										 // 加载病人列表
void buildDoctorChain(Doctor** head, Doctor** tail);											 // 加载医生列表
void buildMedicineChain(MedicineNode** head, MedicineNode** tail);							 // 加载药品列表
void buildPurchaseChain(PurchaseNode** head, PurchaseNode** tail);							 // 加载购药记录
void buildHospitalizationChain(HospitalizationNode** head, HospitalizationNode** tail);		 // 加载住院记录
void buildBedChain(BedNode** head, BedNode** tail);											 // 加载床位列表
void buildRegistrationChain(Registration** head, Registration** tail);						 // 加载挂号记录
void buildUserChain(User** head, User** tail);												 // 加载用户列表

//写入文件
void rebuildPatientFile(Patient* head);											// 保存病人列表
void rebuildDoctorFile(Doctor* head);											// 保存医生列表
void rebuildMedicineFile(MedicineNode* head);									// 保存药品列表
void rebuildPurchaseFile(PurchaseNode* head);									// 保存购药记录
void rebuildHospitalizationFile(HospitalizationNode* head);						// 保存住院记录
void rebuildBedFile(BedNode* head);												// 保存床位列表
void rebuildRegistrationFile(Registration* head);								// 保存挂号记录
void rebuildUserFile(User* head);												// 保存用户列表

//释放内存
void freePatientChain(Patient* head);											// 释放病人列表
void freeDoctorChain(Doctor* head);												// 释放医生列表
void freeMedicineChain(MedicineNode* head);										// 释放药品列表
void freePurchaseChain(PurchaseNode* head);										// 释放购药记录
void freeHospitalizationChain(HospitalizationNode* head);						// 释放住院记录
void freeBedChain(BedNode* head);												// 释放床位列表
void freeRegistrationChain(Registration* head);									// 释放挂号记录
void freeUserChain(User* head);													// 释放用户列表
void freeRegistrationResultChain(Registration* head);							// 释放挂号查询列表
void freeDoctorsResultChain(Doctor* head);										// 释放医生查询列表
void freePatientsResultChain(Patient* head);									// 释放病人查询列表
void freeMedicineResultChain(Medicine* head);									// 释放药品查询列表
void freeHospitalizationResultChain(Hospitalization* head);						// 释放住院查询列表
void freeBedResultChain(Bed* head);												// 释放床位查询列表

//数据备份
int backupAllData();								 // 备份所有数据文件
int copyFile(char* source, char* dest);				 // 复制单个文件

#endif