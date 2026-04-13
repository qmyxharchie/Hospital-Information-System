#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "medicine.h"
#include "file_io.h"
#include "utils.h"


//--------------------
//以下为库存检查函数
void checkLowStock(Medicine* head)
{
	Medicine* cur = head;
	int n = 0;
	while (cur != NULL)
	{
		if (cur->data.stock <= cur->data.minStock)
		{
			n++;
			printf("药品名：%s   当前库存：%d   最低库存：%d", cur->data.medNo, cur->data.stock, cur->data.minStock);
			cur = cur->next;
		}
	}
	printf("预警药品数量  共 %d 种", n);
}
//购药（扣减库存）
void purchaseMedicine(Medicine* head, char* medNo, int quantity)
{

}