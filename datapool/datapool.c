#include "datapool.h"
#include "iconfig.h"

extern uint8_t gATMsgRxBuffer[AT_MESSAGE_RX_BUFFER_SIZE];
/*
 *   数据池初始化
 */
DataPool *DataPoolInit(uint32_t size) 
{
#if USE_MALLOC
	DataPool *ldp = (DataPool *)malloc(sizeof(DataPool));
	if ( ldp == NULL ) {
		DEBUG_ERROR("ldp null\r\n");
		return NULL;
	}
#else   
	DataPool *ldp = (DataPool *)gATMsgRxBuffer;
#endif
	memset(ldp, 0, sizeof(DataPool));

#if USE_MALLOC
	ldp->pool = (uint8_t *)malloc(sizeof(uint8_t) * size);
	if ( ldp->pool == NULL ) {
		DEBUG_ERROR("ldp->pool null\r\n");
		free(ldp);
		return NULL;
	}
#else
	//printf("sizeof(datapool)=%d\r\n", sizeof(DataPool));

	ldp->pool = (uint8_t *)(gATMsgRxBuffer + sizeof(DataPool));
	size -= sizeof(DataPool);
#endif

	memset(ldp->pool, 0, size * sizeof(uint8_t));

	ldp->u32End = 0;
	ldp->u32Start = 0;
	ldp->u32MaxSize = size;
	ldp->FullFlag = false;

	return ldp;
}

void DataPollReInit(DataPool *ldp)
{
	memset(ldp->pool, 0, ldp->u32MaxSize * sizeof(uint8_t));
	ldp->u32End = 0;
	ldp->u32Start = 0;
	ldp->FullFlag = false;
}

/*
 *   将数据写入到数据池中
 */
bool DataPoolWrite(DataPool *ldp, uint8_t *buf, uint32_t len)
{
	uint32_t i;
	//如果end在start之前
	if ( ldp->u32End < ldp->u32Start ) {
		//如果剩余的空间不能容纳想要写入的数据长度
		if (ldp->u32End + len > ldp->u32Start ) 
			return false;
	} else if ( ldp->u32End > ldp->u32Start ) {
		//如果end在start之后
		if ( ldp->u32End + len > ldp->u32MaxSize ) {
			//如果end加上待写入的数据长度后超出最大范围
			if ( len - (ldp->u32MaxSize - ldp->u32End) > ldp->u32Start ) {
				//如果数据长度超过了start的位置，则失败
				return false;
			}
		}
	} else {
		if (ldp->FullFlag)
			return false;
	}

	for (i = 0; i < len; i++ ) {
		ldp->pool[ldp->u32End++] = buf[i];
		if ( ldp->u32End == ldp->u32MaxSize ) {
			//到达缓冲池的末尾
			ldp->u32End = 0;
		}
		if ( ldp->u32End == ldp->u32Start ) {
			//数据已满，不能存入新的数据,返回已经存入的数据长度
			ldp->FullFlag = true;
			break;
		}
	}

	return true;
}

/*
 *   从数据池中读取当前的开始处字节数据
 *   不改变start的位置
 *   返回值0:未读取到数据
 *   返回值1:读取到数据
 */
bool DataPoolLookByte(DataPool *ldp, uint8_t *data)
{   
	if ((ldp->u32Start == ldp->u32End) && (ldp->FullFlag == false))
		return false;

	*data = ldp->pool[ldp->u32Start];

	return true;
}

/*
 *   从数据池中获取当前start处的数据
 *   start指向下一个数据
 */
bool DataPoolGetByte(DataPool *ldp, uint8_t *data)
{
	if ((ldp->u32Start == ldp->u32End) && (ldp->FullFlag == false))
		return false;

	*data = ldp->pool[ldp->u32Start++];

	if (ldp->u32Start >= ldp->u32MaxSize) {
		ldp->u32Start = 0;
	}
	if(ldp->FullFlag == true)
		ldp->FullFlag = false;

	return true;
}

/*
 *   从数据池中取出多个数据
 *   return: 返回取出的数据长度,0表示出错
 */
uint32_t DataPoolGetNumByte(DataPool *ldp, uint8_t *buf, uint32_t len)
{
	uint32_t u32End = ldp->u32End;
	uint32_t i;

	if ((ldp->u32Start == u32End) && (ldp->FullFlag == false)) {
		return 0;
	} else if (ldp->u32Start < u32End) {
		//正常的写入数据，此刻start在end之前
		if (u32End - ldp->u32Start < len) { 
			//数据不满足长度
			return 0;
		}
	} else if (ldp->u32Start > u32End) {
		if ( ldp->u32MaxSize - ldp->u32Start + u32End < len ) {
			//数据长度不够
			return 0;
		}
	}

	for(i = 0; i < len; i++) {

		buf[i] = ldp->pool[ldp->u32Start++];

		if(ldp->u32Start >= ldp->u32MaxSize)
			ldp->u32Start = 0;

		if(ldp->u32Start == ldp->u32End) {
			i++;
			break;
		}
	}

	if(ldp->FullFlag == true)
		ldp->FullFlag = false;

	return i;
}

/*
 *   恢复数据池中的数据(移动start的位置)
 */
void DataPoolResume(DataPool *ldp, uint32_t len)
{                
	if (len < ldp->u32MaxSize) {
		if (ldp->u32Start >= len) {
			ldp->u32Start -= len;
		} else {
			ldp->u32Start = (ldp->u32MaxSize - (len - ldp->u32Start));
		}

		if (ldp->u32Start == ldp->u32End )
			ldp->FullFlag = true;
	}
}

uint32_t GetDataPoolDataLen(DataPool *ldp)
{
	if (ldp->u32Start >= ldp->u32MaxSize || ldp->u32End >= ldp->u32MaxSize) {
		ldp->u32Start = 0;
		ldp->u32End = 0;
		ldp->FullFlag = false;
		printf("===>GetDataPoolDataLen find error\r\n");
		return 0;
	}

	if (ldp->u32End > ldp->u32Start) {  
		return ldp->u32End - ldp->u32Start;
	} else if (ldp->u32End < ldp->u32Start) {
		return ldp->u32MaxSize - ldp->u32Start + ldp->u32End;
	} else if (ldp->FullFlag) {
		return ldp->u32MaxSize;
	} else {
		return 0;
	}
}




