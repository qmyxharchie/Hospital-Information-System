#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "login.h"
#include "file_io.h"
#include "utils.h"
#include "patient.h"

// 全局变量定义
static User* g_currUser = NULL;     // 当前登录用户

// Getter 函数实现
// 功能：获取当前登录用户信息
// 参数：无
// 返回值：当前登录用户指针
User* getCurrentUser(void) {
	return g_currUser;
}

//-----------------
//	MD5加密
// 功能：对输入的密码进行MD5加密
// 参数：input - 待加密的字符串，output - 存放加密结果的缓冲区
void md5Hash(const char* input, char* output);
//-----------------

//-----------------
//	用户登录
// 功能：验证用户登录信息并设置登录状态
// 参数：username - 用户名，password - 密码
// 返回值：LoginStatus类型的登录状态
LoginStatus login(char username[], char password[]) {
	buildUserChain(&g_userHead, &g_userTail);		//加载用户链表

	char pwdHash[100];
	md5Hash(password, pwdHash);						//密码转MD5

	User* cur = g_userHead;

	while (cur != NULL) {							//遍历用户链表
		if (strcmp(cur->data.username, username) == 0 &&
			strcmp(cur->data.password, password) == 0) {		//匹配用户名和密码

			strcpy(g_currentUsername, username);
			g_currentUserRole = cur->data.role;
			g_isLoggedIn = 1;									//记录登录状态

			getCurrentTime(cur->data.lastLogin);				//更新登录时间

			rebuildUserFile(g_UserHead);						//保存回文件

			if (cur->data.role)									//返回对应状态
				return LOGIN_SUCCESS_USER;
			else 
				return LOGIN_SUCCESS_ADMIN;
		}
		cur = cur->next;
	}
	return LOGIN_FAILED;										//未找到用户
}
//-----------------

//-----------------
//	用户注册
// 功能：创建新用户账户
// 参数：username - 用户名，password - 密码，role - 用户角色（0-管理员，1-普通用户）
// 返回值：1-注册成功，0-注册失败
int registerUser(char username[], char password[], int role) {
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
		return 0;  // 添加返回值
	}
	strcpy(newUser->data.username, username);		//填充数据

	char hashed[100];								
	md5Hash(password, hashed);						//密码加密
	strcpy(newUser->data.password, hashed);

	newUser->data.role = role;
	newUser->next = NULL;

	if (g_userHead == NULL) {						//尾插法插入链表
		g_userHead = newUser;
		g_userTail = newUser;
	}
	else {
		g_userTail->next = newUser;
		g_userTail = newUser;
	}
	rebuildUserFile(g_userHead);					//保存到文件

	printf("注册成功！\n");
	return 1;
}
//-----------------

//-----------------
//	用户查询
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