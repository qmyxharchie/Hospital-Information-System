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
LoginStatus login(char username[], char password[]);                         //用户登录
int registerUser(char username[], char password[], int role);                //用户注册
void buildUserChain(User** head, User** tail);                               // 从文件加载用户列表
void rebuildUserFile(User* head);                                            // 保存用户列表到文件（团队头文件中参数为 User 而非 User*）
void freeUserChain(User* head);                                              // 释放用户链表内存
User* findUserByName(User* head, char* username);                            //用户查询
extern char g_currentUsername[50];                                           // 当前登录用户名
extern int g_currentUserRole;                                                // 当前用户角色：0=用户, 1=管理员
extern bool g_isLoggedIn;                                                    // 是否已登录
#endif 