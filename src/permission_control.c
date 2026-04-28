#include "permission_control.h"
#include <string.h>

// 检查用户是否有执行特定操作的权限
int check_permission(UserType user_type, const char* operation) {
    if (strcmp(operation, "view_own_records") == 0) {
        // 所有用户都可以查看自己的记录
        return 1;
    }
    else if (strcmp(operation, "make_appointment") == 0) {
        // 患者可以预约挂号
        return (user_type == PATIENT);
    }
    else if (strcmp(operation, "view_patient_data") == 0) {
        // 医生和护士可以查看患者数据
        return (user_type == DOCTOR || user_type == NURSE || user_type == ADMIN);
    }
    else if (strcmp(operation, "update_medical_record") == 0) {
        // 仅医生可以更新病历
        return (user_type == DOCTOR);
    }
    else if (strcmp(operation, "manage_users") == 0) {
        // 仅管理员可以管理用户
        return (user_type == ADMIN);
    }
    else if (strcmp(operation, "call_patient") == 0) {
        // 医生和护士可以叫号
        return (user_type == DOCTOR || user_type == NURSE);
    }
    else if (strcmp(operation, "cancel_registration") == 0) {
        // 患者可以取消自己的挂号，医护人员可以取消待就诊的挂号
        return (user_type == PATIENT || user_type == DOCTOR || user_type == NURSE);
    }
    else if (strcmp(operation, "access_system_config") == 0) {
        // 仅管理员可以访问系统配置
        return (user_type == ADMIN);
    }

    return 0; // 默认不允许
}

// 检查用户是否可以访问特定患者数据
int can_access_patient_data(UserType user_type, int requesting_user_id, int target_patient_id) {
    switch (user_type) {
    case PATIENT:
        // 患者只能访问自己的数据
        return (requesting_user_id == target_patient_id);
    case NURSE:
    case DOCTOR:
        // 医护人员可以访问他们负责的患者数据
        // 实际实现中可能需要检查科室或其他关联关系
        return 1; // 简化实现
    case ADMIN:
        // 管理员可以访问所有患者数据
        return 1;
    default:
        return 0;
    }
}

// 检查用户是否可以管理特定挂号
int can_manage_registration(UserType user_type, int requesting_user_id, int reg_patient_id) {
    switch (user_type) {
    case PATIENT:
        // 患者只能管理自己的挂号
        return (requesting_user_id == reg_patient_id);
    case NURSE:
    case DOCTOR:
        // 医护人员可以管理待就诊的挂号
        return 1; // 实际中还需要检查状态
    case ADMIN:
        // 管理员可以管理所有挂号
        return 1;
    default:
        return 0;
    }
}

// 其他权限检查函数的实现...
int can_update_medical_record(UserType user_type) {
    return (user_type == DOCTOR);
}

int can_view_financial_data(UserType user_type) {
    return (user_type == ADMIN);
}

int can_manage_users(UserType user_type) {
    return (user_type == ADMIN);
}

int can_access_system_config(UserType user_type) {
    return (user_type == ADMIN);
}

// 角色检查函数
int is_patient(UserType type) {
    return (type == PATIENT);
}

int is_nurse(UserType type) {
    return (type == NURSE);
}

int is_doctor(UserType type) {
    return (type == DOCTOR);
}

int is_admin(UserType type) {
    return (type == ADMIN);
}