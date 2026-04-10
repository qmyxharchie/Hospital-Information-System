//===============
//文件IO
//===============

#define DATA_DIR "data/"
#define BACKUP_DIR "backup/"
#define PATIENT_FILE    DATA_DIR "patient.txt"
#define DOCTOR_FILE     DATA_DIR "doctor.txt"
buildPatientChain(Patient* head, Patient* tail);			//病人链表
buildDoctorChain(Doctor* head, Doctor* tail)；			//医生链表
rebuildPatientFile(Patient head)；			//重建病人链表
rebuildDoctorFile(Doctor head)；			//重建医生链表
