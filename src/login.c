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
		printf("[ERROR] 内存分配失败！\n");
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
// 用户名是否已被占用
// 功能：加载最新用户列表后检查
// 返回：1=已占用，0=可用
int isUsernameTaken(const char* username) {
	freeUserChain(&g_userHead);
	buildUserChain(&g_userHead, &g_userTail);
	User* u = findUserByName(g_userHead, (char*)username);
	return (u != NULL) ? 1 : 0;
}
//-----------------

//-----------------
// 列出所有用户
void listAllUsers(void) {
	freeUserChain(&g_userHead);
	buildUserChain(&g_userHead, &g_userTail);
	if (g_userHead == NULL) {
		printf("暂无用户\n");
		return;
	}
	printf("=== 用户列表 ===\n");
	printPadded("用户名", 20); putchar(' ');
	printPadded("角色", 10);   putchar(' ');
	printPadded("最后登录", 12); putchar('\n');
	User* cur = g_userHead;
	while (cur != NULL) {
		printPadded(cur->data.username, 20); putchar(' ');
		char* roleStr;
		switch (cur->data.role) {
		case PATIENT: roleStr = "患者"; break;
		case NURSE:   roleStr = "护士"; break;
		case DOCTOR:  roleStr = "医生"; break;
		case ADMIN:   roleStr = "管理员"; break;
		default:      roleStr = "未知"; break;
		}
		printPadded(roleStr, 10); putchar(' ');
		printPadded(cur->data.lastLogin[0] ? cur->data.lastLogin : "从未登录", 12);
		putchar('\n');
		cur = cur->next;
	}
}
//-----------------

//-----------------
// 修改用户角色
int modifyUserRole(const char* username, UserRole newRole) {
	freeUserChain(&g_userHead);
	buildUserChain(&g_userHead, &g_userTail);
	User* u = findUserByName(g_userHead, (char*)username);
	if (u == NULL) return 0;
	u->data.role = newRole;
	rebuildUserFile(g_userHead);
	return 1;
}
//-----------------

//-----------------
// 重置密码为 123456
int resetUserPassword(const char* username) {
	freeUserChain(&g_userHead);
	buildUserChain(&g_userHead, &g_userTail);
	User* u = findUserByName(g_userHead, (char*)username);
	if (u == NULL) return 0;
	char hashed[100];
	md5Hash("123456", hashed);
	strcpy(u->data.password, hashed);
	rebuildUserFile(g_userHead);
	return 1;
}
//-----------------

//-----------------
// 删除用户
int deleteUser(const char* username) {
	freeUserChain(&g_userHead);
	buildUserChain(&g_userHead, &g_userTail);
	User* u = findUserByName(g_userHead, (char*)username);
	if (u == NULL) return 0;
	if (u->pre) u->pre->next = u->next;
	else g_userHead = u->next;
	if (u->next) u->next->pre = u->pre;
	else g_userTail = u->pre;
	free(u);
	rebuildUserFile(g_userHead);
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


