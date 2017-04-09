//
// Created by 肖伟 on 2017/4/5.
//

#ifndef NTFS_TEST_UNIX_IO_H
#define NTFS_TEST_UNIX_IO_H


#include "types.h"
#include "mst.h"
#include "debug.h"
#include "device.h"
#include "logging.h"
#include "misc.h"

int ntfs_fsync(int fildes);
int ntfs_device_unix_io_open(struct ntfs_device *dev, int flags);
int ntfs_device_unix_io_close(struct ntfs_device *dev);
s64 ntfs_device_unix_io_seek(struct ntfs_device *dev, s64 offset,int whence);
s64 ntfs_device_unix_io_read(struct ntfs_device *dev, void *buf, s64 count);
s64 ntfs_device_unix_io_write(struct ntfs_device *dev, const void *buf, s64 count);
s64 ntfs_device_unix_io_pread(struct ntfs_device *dev, void *buf, s64 count, s64 offset);
s64 ntfs_device_unix_io_pwrite(struct ntfs_device *dev, const void *buf, s64 count, s64 offset);
int ntfs_device_unix_io_sync(struct ntfs_device *dev);
int ntfs_device_unix_io_stat(struct ntfs_device *dev, struct stat *buf);
int ntfs_device_unix_io_ioctl(struct ntfs_device *dev, int request, void *argp);

struct ntfs_device_operations  ntfs_device_unix_io_ops;


//struct ntfs_device_operations  ntfs_device_unix_io_ops;

#endif //NTFS_TEST_UNIX_IO_H
