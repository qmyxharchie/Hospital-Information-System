//===============
//   登录模块
//===============
// login.h
#ifndef _LOGIN_H_
#define _LOGIN_H_
typedef struct UserData {
    char username[50];           // 用户名
    char password[100];          // 密码（MD5哈希）
    int role;                    // 角色（0=用户，1=管理员）
    char lastLogin[30];          // 最后登录时间
} UserData;
typedef struct User {
    UserData data;
    struct User* next;
    struct User* pre;
} User;

// 登录状态
typedef enum {
    LOGIN_FAILED = 0,
    LOGIN_SUCCESS_USER = 1,
    LOGIN_SUCCESS_ADMIN = 2,
    LOGIN_EXIT = 3
} LoginStatus;
login(char username[], char password[]);            //用户登录
registerUser(char username[], char password[], int role);            //用户注册
buildUserChain(User* head, User* tail);           //用户链表
rebuildUserFile(User head)；            //重建用户链表
