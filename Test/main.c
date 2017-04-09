#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include "time.h"
#include "types.h"
#include "mft.h"
#include "attrib.h"
#include "layout.h"
#include "inode.h"
#include "dir.h"
#include "list.h"
#include "ntfstime.h"
#include "volume.h"
#include "logging.h"
#include "unix_io.h"

#define MAX_PATH 1024
static int depth = 0;
static struct ntfs_list_head *dir_list_insert_pos = NULL;

struct dir {
    struct ntfs_list_head list;
    ntfs_inode *ni;
    char name[MAX_PATH];
    int depth;
};

/**
 * utils_set_locale
 */
int utils_set_locale(void)
{
    const char *locale;

    locale = setlocale(LC_ALL, "");
    if (!locale) {
        locale = setlocale(LC_ALL, NULL);
        ntfs_log_error("Failed to set locale, using default '%s'.\n",
                       locale);
        return 1;
    } else {
        return 0;
    }
}

typedef struct {
    ntfs_volume *vol;
} ntfsls_dirent;
/**
 * list_dir_entry
 *
 * FIXME: Should we print errors as we go along? (AIA)
 */
static int list_dir_entry(ntfsls_dirent * dirent, const ntfschar * name,
                          const int name_len, const int name_type,
                          const s64 pos __attribute__((unused)),
                          const MFT_REF mref, const unsigned dt_type)
{
    char *filename = NULL;
    int result = 0;

    struct dir *dir = NULL;

    filename = calloc(1, MAX_PATH);
    if (!filename)
        return -1;

    if (ntfs_ucstombs(name, name_len, &filename, MAX_PATH) < 0) {
        ntfs_log_error("Cannot represent filename in current locale.\n");
        goto free;
    }

    result = 0;					// These are successful
    if ((MREF(mref) < FILE_first_user) && (!0))
        goto free;
    if (name_type == FILE_NAME_POSIX && !0)
        goto free;
    if (((name_type & FILE_NAME_WIN32_AND_DOS) == FILE_NAME_WIN32) && 1)
        goto free;
    if (((name_type & FILE_NAME_WIN32_AND_DOS) == FILE_NAME_DOS) && !1)
        goto free;
    if (dt_type == NTFS_DT_DIR && 0)
        sprintf(filename + strlen(filename), "/");

    if (dt_type == NTFS_DT_DIR && 0
        && strcmp(filename, ".") && strcmp(filename, "./")
        && strcmp(filename, "..") && strcmp(filename, "../"))
    {
        dir = (struct dir *)calloc(1, sizeof(struct dir));

        if (!dir) {
            ntfs_log_error("Failed to allocate for subdir.\n");
            result = -1;
            goto free;
        }

        strcpy(dir->name, filename);
        dir->ni = NULL;
        dir->depth = depth;
    }

    if (0) {
        if (0)
            printf("%s\n", filename);
        else
            printf("%7llu %s\n", (unsigned long long)MREF(mref),
                   filename);
        result = 0;
    } else {
        s64 filesize = 0;
        ntfs_inode *ni;
        ntfs_attr_search_ctx *ctx = NULL;
        FILE_NAME_ATTR *file_name_attr;
        ATTR_RECORD *attr;
        struct timespec change_time;
        char t_buf[26];

        result = -1;				// Everything else is bad

        ni = ntfs_inode_open(dirent->vol, mref);
        if (!ni)
            goto release;

        ctx = ntfs_attr_get_search_ctx(ni, NULL);
        if (!ctx)
            goto release;

        if (ntfs_attr_lookup(AT_FILE_NAME, AT_UNNAMED, 0, 0, 0, NULL,
                             0, ctx))
            goto release;
        attr = ctx->attr;

        file_name_attr = (FILE_NAME_ATTR *)((char *)attr +
                                            le16_to_cpu(attr->value_offset));
        if (!file_name_attr)
            goto release;

        change_time = ntfs2timespec(file_name_attr->last_data_change_time);
        strcpy(t_buf, ctime(&change_time.tv_sec));
        memmove(t_buf+16, t_buf+19, 5);
        t_buf[21] = '\0';

        if (dt_type != NTFS_DT_DIR) {
            if (!ntfs_attr_lookup(AT_DATA, AT_UNNAMED, 0, 0, 0,
                                  NULL, 0, ctx))
                filesize = ntfs_get_attribute_value_length(
                        ctx->attr);
        }

        if (0)
            printf("%7llu    %8lld %s %s\n",
                   (unsigned long long)MREF(mref),
                   (long long)filesize, t_buf + 4,
                   filename);
        else
            printf("%8lld %s %s\n", (long long)filesize, t_buf + 4,
                   filename);

        if (dir) {
            dir->ni = ni;
            ni = NULL;	/* so release does not close inode */
        }

        result = 0;
        release:
        /* Release attribute search context and close the inode. */
        if (ctx)
            ntfs_attr_put_search_ctx(ctx);
        if (ni)
            ntfs_inode_close(ni);
    }

    if (dir) {
        if (result == 0) {
            ntfs_list_add(&dir->list, dir_list_insert_pos);
            dir_list_insert_pos = &dir->list;
        } else {
            free(dir);
            dir = NULL;
        }
    }
    free:
    free(filename);
    return result;
}

int main() {
    s64 pos;
    ntfs_volume *vol;
    ntfs_inode *ni;
    ntfsls_dirent dirent;
    utils_set_locale();
    char *name = "/dev/disk2";
    struct ntfs_device *dev;

    ntfs_device_unix_io_ops.open = ntfs_device_unix_io_open;
    ntfs_device_unix_io_ops.close = ntfs_device_unix_io_close;
    ntfs_device_unix_io_ops.seek = ntfs_device_unix_io_seek;
    ntfs_device_unix_io_ops.read = ntfs_device_unix_io_read;
    ntfs_device_unix_io_ops.write = ntfs_device_unix_io_write;
    ntfs_device_unix_io_ops.pread = ntfs_device_unix_io_pread;
    ntfs_device_unix_io_ops.pwrite = ntfs_device_unix_io_pwrite;
    ntfs_device_unix_io_ops.sync = ntfs_device_unix_io_sync;
    ntfs_device_unix_io_ops.stat = ntfs_device_unix_io_stat;
    ntfs_device_unix_io_ops.ioctl = ntfs_device_unix_io_ioctl;

    //dev = ntfs_device_alloc(name, 0, &ntfs_device_unix_io_ops, NULL,346030080);
    //vol = ntfs_device_mount(dev, NTFS_MNT_RDONLY);
    vol = ntfs_mount(name, NTFS_MNT_RDONLY,346030080);
    if (!vol) {
        printf("error.....\n");
    }
/*
    printf("%d\n",(vol->mft_na->type));
    printf("vol_name=%s\n",vol->vol_name);
    printf("sector_size=%d\n",vol->sector_size);
    printf("sector_size_bits=%d\n",vol->sector_size_bits);
    printf("cluster_size=%d\n",vol->cluster_size);
    printf("mft_record_size=%d\n",vol->mft_record_size);
    printf("indx_record_size=%d\n",vol->indx_record_size);
    printf("cluster_size_bits=%d\n",vol->cluster_size_bits);
    printf("mft_record_size_bits=%d\n",vol->mft_record_size_bits);
    printf("indx_record_size_bits=%d\n",vol->indx_record_size_bits);
    printf("mft_data_pos=%lld\n",vol->mft_data_pos);
    printf("mft_zone_start=%lld\n",vol->mft_zone_start);
    printf("mft_zone_end=%lld\n",vol->mft_zone_end);
    printf("data1_zone_pos=%lld\n",vol->data1_zone_pos);
    printf("mftmirr_size=%d\n",vol->mftmirr_size);
*/
    ni = ntfs_pathname_to_inode(vol, NULL,"/");

    memset(&dirent, 0, sizeof(dirent));
    dirent.vol = vol;

    if (ni->mrec->flags & MFT_RECORD_IS_DIRECTORY) {
        ntfs_readdir(ni, &pos, &dirent,
                     (ntfs_filldir_t) list_dir_entry);
    }
    return 0;
}