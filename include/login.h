//===============
//   登录模块
//===============

/*登录模块负责用户身份验证：
- 用户登录
- 管理员登录
- 用户注册
- 密码加密存储*/

#ifndef _LOGIN_H_
#define _LOGIN_H_

typedef struct UserData {
    char username[50];           // 用户名
    char password[100];          // 密码（MD5哈希）
    int role;                    // 角色 (0=患者, 1=护士, 2=医生, 3=管理员）
    char lastLogin[30];          // 最后登录时间
} UserData;
typedef struct User {
    UserData data;
    struct User* next;
    struct User* pre;
} User;


typedef enum {
    LOGIN_FAILED = 0,
    LOGIN_SUCCESS_USER = 1,
    LOGIN_SUCCESS_ADMIN = 2,
    LOGIN_EXIT = 3
} LoginStatus;                    // 登录状态

LoginStatus login(char username[], char password[]);                  //用户登录
int registerUser(char username[], char password[], int role);         //用户注册
User* findUserByName(User* head, char* username);                     //用户查询
extern char g_currentUsername[50];                                    // 当前登录用户名
extern int g_currentUserRole;                                         // 当前用户角色：0=用户, 1=管理员
extern bool g_isLoggedIn;                                             // 是否已登录
User* getCurrentUser(void);                                           // 获取当前登录用户

#endif 