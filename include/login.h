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

#include <stdbool.h>

typedef enum {
    PATIENT = 0, // 患者
    NURSE = 1,   // 护士
    DOCTOR = 2,  // 医生
    ADMIN = 3    // 管理员
} UserRole;

typedef struct UserData {
    char username[50];           // 用户名
    char password[100];          // 密码（MD5哈希）
    UserRole role;               // 角色 (0=患者, 1=护士, 2=医生, 3=管理员）
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
    LOGIN_EXIT = 3,
    LOGIN_REGISTERED = 4
} LoginStatus;                    // 登录状态

typedef struct {
    int loginStatus;
    UserRole userRole;
    char username[50];
} LoginResult;

extern char g_currentUsername[50];        // 当前登录用户名
extern UserRole g_currentUserRole;        // 当前用户角色：0=用户, 1=护士, 2=医生, 3=管理员
extern bool g_isLoggedIn;                 // 是否已登录

LoginStatus login(char username[], char password[]);                  // 用户登录
int registerUser(char username[], char password[], int role);         // 用户注册
void md5Hash(const char* input, char* output);                        // 密码哈希
int isUsernameTaken(const char* username);                            // 用户名是否已被占用（1=是，0=否）
void listAllUsers(void);                                              // 列出所有用户
int modifyUserRole(const char* username, UserRole newRole);           // 修改用户角色
int resetUserPassword(const char* username);                          // 重置密码为 123456
int deleteUser(const char* username);                                 // 删除用户
User* findUserByName(User* head, char* username);                     // 用户查询
User* getCurrentUser(void);                                           // 获取当前登录用户
UserRole getUserRoleByUsername(const char* username);
int hasPermission(UserRole role, const char* operation);
int isPatient(UserRole role);
int isNurse(UserRole role);
int isDoctor(UserRole role);
int isAdmin(UserRole role);
LoginResult loginWithRole(char* username, char* password);

#endif /* _LOGIN_H_ */