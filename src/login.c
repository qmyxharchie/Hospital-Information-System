#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "login.h"
#include "file_io.h"
#include "utils.h"
#include "patient.h"
#include "ui.h"

// 全局变量定义
static User* g_currUser = NULL;     // 当前登录用户

// Getter 函数实现
User* getCurrentUser(void) {
	return g_currUser;
}

//-----------------
// 简化版MD5哈希函数（仅用于演示，不安全）
void md5Hash(const char* input, char* output) {
	// 这里只是一个模拟实现，实际应用中需要使用真正的MD5算法
	unsigned int hash = 0;
	size_t len = strlen(input);

	for (unsigned int i = 0; i < len; i++) {
		hash = hash * 37 + input[i];
	}

	// 将哈希值转换为32位十六进制字符串（MD5标准长度）
	sprintf(output, "%08x%08x%08x%08x", hash, hash, hash, hash);

	// 确保字符串结束
	output[32] = '\0';
}//-----------------

//-----------------
// 用户登录
// 功能：验证用户登录信息并设置登录状态
// 参数：username - 用户名，password - 密码
// 返回值：LoginStatus类型的登录状态
LoginStatus login(char username[], char password[]) {
	freeUserChain(&g_userHead);						//释放旧链表，防止内存泄漏
	buildUserChain(&g_userHead, &g_userTail);		//加载用户链表

	char pwdHash[100];
	md5Hash(password, pwdHash);						//密码转MD5

	User* cur = g_userHead;

	while (cur != NULL) {							//遍历用户链表
		if (strcmp(cur->data.username, username) == 0 &&
			strcmp(cur->data.password, pwdHash) == 0) {			//匹配用户名和密码哈希值

			strcpy(g_currentUsername, username);
			g_currentUserRole = cur->data.role;
			g_isLoggedIn = 1;									//记录登录状态

			int y=0, m=0, d=0;
			getCurrentTime(&y, &m, &d);
			sprintf(cur->data.lastLogin, "%04d-%02d-%02d", y, m, d);				//更新登录时间

			rebuildUserFile(g_userHead);						//保存回文件

			//管理员角色为ADMIN(3)，普通用户为PATIENT(0)/NURSE(1)/DOCTOR(2)
			if (cur->data.role == PATIENT || cur->data.role == NURSE || 
				cur->data.role == DOCTOR){
				return LOGIN_SUCCESS_USER;
			}
			else{
				return LOGIN_SUCCESS_ADMIN;
			}
		}
		cur = cur->next;
	}
	return LOGIN_FAILED;										//未找到用户
}
//-----------------

//-----------------
// 用户注册
// 功能：创建新用户账户
// 参数：username - 用户名，password - 密码，
//		 role - 用户角色（0-管理员，1-护士，2-医生，3-管理员）
// 返回值：1-注册成功，0-注册失败

int registerUser(char username[], char password[], int role) {
	freeUserChain(&g_userHead);						//释放旧链表，防止内存泄漏
	buildUserChain(&g_userHead, &g_userTail);		//加载用户链表

	User* cur = g_userHead;							//检查用户是否存在
	while (cur != NULL) {
		if (strcmp(cur->data.username, username) == 0) {
			printf("用户名已存在！\n");
			return 0;
		}
		cur = cur->next;
	}
	User* newUser = (User*)malloc(sizeof(User));	//创建新节点
	if (newUser == NULL) {							//检查是否创建成功
		printf("内存分配失败！\n");
		return 0;
	}
	memset(&newUser->data, 0, sizeof(UserData));	//清零所有字段，避免未初始化字节写入文件
	strcpy(newUser->data.username, username);		//填充数据

	char hashed[100];								
	md5Hash(password, hashed);						//密码加密
	strcpy(newUser->data.password, hashed);

	newUser->data.role = (UserRole)role;
	newUser->next = NULL;

	if (g_userHead == NULL) {						//尾插法插入链表
		g_userHead = newUser;

		g_userTail = newUser;						//修正：初始化尾指针

	}
	else {
		g_userTail->next = newUser;
		newUser->pre = g_userTail;					//维护双向链表
		g_userTail = newUser;
	}
	rebuildUserFile(g_userHead);					//保存到文件

	printf("注册成功！\n");
	return 1;
}
//-----------------

//-----------------
// 用户查询
// 功能：根据用户名查找用户信息
// 参数：head - 用户链表头指针，username - 要查找的用户名
// 返回值：找到的用户节点指针，未找到返回NULL

User* findUserByName(User* head, char* username) {
	User* cur = head;
	while (cur != NULL) {										//遍历链表
		if (strcmp(cur->data.username, username) == 0) {		//比较用户名
			return cur;
		}
		cur = cur->next;
	}
	return NULL;												//遍历没有找到
}
//-----------------

//-----------------
// 根据用户名获取用户角色
// 功能：从用户链表中查找指定用户并返回其角色
// 参数：username - 要查询的用户名
// 返回值：UserRole类型的用户角色，未找到则返回PATIENT
UserRole getUserRoleByUsername(const char* username) {
	freeUserChain(&g_userHead);					   //释放旧链表，防止内存泄漏
	buildUserChain(&g_userHead, &g_userTail); //加载用户链表
	User* user = findUserByName(g_userHead, (char*)username);
	if (user != NULL) {
		return user->data.role;
	}
	return PATIENT; //默认返回患者角色
}
//-----------------

//-----------------
// 权限检查
// 功能：检查指定角色是否拥有某项操作权限
// 参数：role - 用户角色，operation - 要检查的操作类型
// 返回值：1-有权限，0-无权限
int hasPermission(UserRole role, const char* operation) {
	if (strcmp(operation, "view_own_profile") == 0) {
		return 1; // 所有角色都能查看自己的数据
	}

	if (strcmp(operation, "view_registration") == 0) {
		return role >= PATIENT; // 所有角色都能查看挂号
	}

	if (strcmp(operation, "manage_patients") == 0) {
		return role >= NURSE; // 护士及以上
	}

	if (strcmp(operation, "manage_doctors") == 0) {
		return role >= DOCTOR; // 医生及以上
	}

	if (strcmp(operation, "system_admin") == 0) {
		return role == ADMIN; // 仅管理员
	}

	if (strcmp(operation, "book_appointment") == 0) {
		return role == PATIENT || role == NURSE || role == ADMIN;
	}

	return 0; // 默认拒绝
}
//-----------------

//-----------------
// 检查是否为某角色
// 参数：role - 用户角色
// 返回值：1-是，0-不是
// 功能：判断用户角色是否为患者
int isPatient(UserRole role) {
	return role == PATIENT;
}
//-----------------

//-----------------
// 检查是否为护士角色
int isNurse(UserRole role) {
	return role == NURSE;
}
//-----------------

//-----------------
// 检查是否为医生角色
int isDoctor(UserRole role) {
	return role == DOCTOR;
}
//-----------------

//-----------------
// 检查是否为管理员角色
int isAdmin(UserRole role) {
	return role == ADMIN;
}
//-----------------

//-----------------
// 带角色信息的登录
// 功能：执行用户登录并返回详细的登录结果（包括角色信息）
// 参数：username - 用户名，password - 密码
// 返回值：LoginResult结构体，包含登录状态、用户角色和用户名
LoginResult loginWithRole(char* username, char* password) {
	LoginResult result = { 0 };

	// 调用您现有的登录函数
	LoginStatus status = login(username, password);
	result.loginStatus = status;

	if (status == LOGIN_SUCCESS_USER || status == LOGIN_SUCCESS_ADMIN) {
		strcpy(result.username, username);
		result.userRole = (UserRole)g_currentUserRole; // 使用全局变量中的角色
	}

	return result;
}
//-----------------

