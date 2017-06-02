#ifndef _CONF_H_
#define _CONF_H_


#define SIMPLE_POLL 0

#define KERNEL_SECTOR_SIZE   512
#define SECTORS_PER_PAGE  (PAGE_SIZE / KERNEL_SECTOR_SIZE)
#define DEVICE_BOUND 100
#define REQ_ARR_SIZE 10
#define MAX_REQ 1024
#define MERGE_REQ false
#define REQ_POOL_SIZE 1024

#define RDMA_BUFFER_SIZE (1024*1024)
#define CQE_SIZE 4096

#define DEBUG_OUT_REQ 0



#endif



