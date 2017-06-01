/*
 * A sample, extra-simple block driver. Updated for kernel 2.6.31.
 *
 * (C) 2003 Eklektix, Inc.
 * (C) 2010 Pat Patterson <pat at superpat dot com>
 * Redistributable under the terms of the GNU GPL.
 * Modified by Sangjin Han (sangjin@eecs.berkeley.edu) and Peter Gao (petergao@berkeley.edu)
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/kernel.h> /* printk() */
#include <linux/fs.h>     /* everything... */
#include <linux/errno.h>  /* error codes */
#include <linux/types.h>  /* size_t */
#include <linux/vmalloc.h>
#include <linux/genhd.h>
#include <linux/blkdev.h>
#include <linux/hdreg.h>
#include <linux/random.h>
#include <linux/un.h>
#include <net/sock.h>
#include <linux/socket.h>
#include <linux/delay.h>
#include <linux/bio.h>
#include <linux/version.h>

#include <linux/time.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/cpufreq.h>
#include "rdma_library.h"
#include "log.h"
#include "conf.h"

MODULE_LICENSE("Dual BSD/GPL");


rdma_ctx_t rdma_ctx;

static int __init rmem_init(void) {
  int ret;
  char output[128] = "hellordma";
  char input[128];
  rdma_request req;

  LOG_KERN(LOG_INFO, "Start rmem_rdma. rdma_library_init() CPU freq %d kHz\n", cpu_khz);
  ret = rdma_library_init();
  pr_err("init success? %d\n", ret);

  while(!rdma_library_ready());
  LOG_KERN(LOG_INFO, "init done\n");

  rdma_ctx = rdma_init(100, "10.10.49.89", 19888);
  if(rdma_ctx == NULL){
    pr_info("rdma_init() failed\n");
    goto out;
  }

  pr_info("rmem_rdma successfully loaded!\n");

  req.rw = RDMA_WRITE;
  req.length = 10;
  req.dma_addr = rdma_map_address(output, 10);
  req.remote_offset = 0;

  rdma_op(rdma_ctx, &req, 1);

  req.rw = RDMA_READ;
  req.dma_addr = rdma_map_address(input, 10);
  rdma_op(rdma_ctx, &req, 1);
  
  pr_info("result %s", input);

  return 0;

out:
  rdma_library_exit();
  return -ENOMEM;
}

static void __exit rmem_exit(void)
{
  rdma_exit(rdma_ctx);
  rdma_library_exit();
  pr_info("rmem: bye!\n");
}

module_init(rmem_init);
module_exit(rmem_exit);
