#ifndef PERMISSION_CONTROL_H
#define PERMISSION_CONTROL_H

#include "login.h"
#include "registration.h"
#include "patient.h"
#include "doctor.h"

// 扩展角色定义
typedef enum {
    PATIENT_ROLE = 0,    // 患者
    NURSE_ROLE = 1,      // 护士
    DOCTOR_ROLE = 2,     // 医生
    ADMIN_ROLE = 3       // 管理员
} ExtendedRole;

// 权限验证函数声明
ExtendedRole getUserExtendedRole(const char* username);  // 获取用户的扩展角色
int checkPermission(int basicRole, ExtendedRole extendedRole, const char* operation);  // 检查权限
int canAccessPatientData(int basicRole, ExtendedRole extendedRole, const char* requestingUser, const char* targetPatient);  // 检查访问患者数据权限
int canManageRegistration(int basicRole, ExtendedRole extendedRole, const char* requestingUser, const char* regPatientCardNo);  // 检查管理挂号权限
int canUpdateMedicalRecord(int basicRole, ExtendedRole extendedRole);  // 检查更新病历权限
int canViewFinancialData(int basicRole, ExtendedRole extendedRole);  // 检查查看财务数据权限
int canManageUsers(int basicRole, ExtendedRole extendedRole);  // 检查管理用户权限
int canAccessSystemConfig(int basicRole, ExtendedRole extendedRole);  // 检查访问系统配置权限

// 角色检查函数
int isPatient(int basicRole, ExtendedRole extendedRole);
int isNurse(ExtendedRole extendedRole);
int isDoctor(ExtendedRole extendedRole);
int isAdmin(int basicRole);

#endif