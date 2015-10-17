/*
  CS179f File System Project
*/

#include "myparams.h"

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <fuse.h>
#include <libgen.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#ifdef HAVE_SYS_XATTR_H
#include <sys/xattr.h>
#endif

static void get_path(char fpath[PATH_MAX], const char *path)
{
    strcpy(fpath, STATIC_DATA->rootdir);
    strncat(fpath, path, PATH_MAX);
}

/** Get file attributes.
 *
 * Similar to stat().  The 'st_dev' and 'st_blksize' fields are
 * ignored.  The 'st_ino' field is ignored except if the 'use_ino'
 * mount option is given.
 */
int my_getattr(const char *path, struct stat *statbuf)
{
    statbuf->st_dev=0;     /* ID of device containing file */
    statbuf->st_ino=0;     /* inode number */
    statbuf->st_mode=040777;    /* protection */
    statbuf->st_nlink=0;   /* number of hard links */
    statbuf->st_uid=0;     /* user ID of owner */
    statbuf->st_gid=0;     /* group ID of owner */
    statbuf->st_rdev=0;    /* device ID (if special file) */
    statbuf->st_size=0;    /* total size, in bytes */
    statbuf->st_blksize=0; /* blocksize for file system I/O */
    statbuf->st_blocks=0;  /* number of 512B blocks allocated */
    statbuf->st_atime=0;   /* time of last access */
    statbuf->st_mtime=0;   /* time of last modification */
    statbuf->st_ctime=0;   /* time of last status change */

    return 0;
}

/** Read the target of a symbolic link
 *
 * The buffer should be filled with a null terminated string.  The
 * buffer size argument includes the space for the terminating
 * null character.  If the linkname is too long to fit in the
 * buffer, it should be truncated.  The return value should be 0
 * for success.
 */
int my_readlink(const char *path, char *link, size_t size)
{
}

/** Create a file node
 *
 * If there is no create() operation, mknod() will be called for
 * creation of all non-directory, non-symlink nodes.
 */
int my_mknod(const char *path, mode_t mode, dev_t dev)
{
}

/** Create a directory */
int my_mkdir(const char *path, mode_t mode)
{
}

/** Remove a file */
int my_unlink(const char *path)
{
}

/** Remove a directory */
int my_rmdir(const char *path)
{
}

/** Create a symbolic link */
int my_symlink(const char *path, const char *link)
{
}

/** Rename a file */
int my_rename(const char *path, const char *newpath)
{
}

/** Create a hard link to a file */
int my_link(const char *path, const char *newpath)
{
}

/** Change the permission bits of a file */
int my_chmod(const char *path, mode_t mode)
{
}

/** Change the owner and group of a file */
int my_chown(const char *path, uid_t uid, gid_t gid)
{
}

/** Change the size of a file */
int my_truncate(const char *path, off_t newsize)
{
}

/** Change the access and/or modification times of a file */
int my_utime(const char *path, struct utimbuf *ubuf)
{
}

/** File open operation
 *
 * No creation, or truncation flags (O_CREAT, O_EXCL, O_TRUNC)
 * will be passed to open().  Open should check if the operation
 * is permitted for the given flags.  Optionally open may also
 * return an arbitrary filehandle in the fuse_file_info structure,
 * which will be passed to all file operations.
 */
int my_open(const char *path, struct fuse_file_info *fi)
{
}

/** Read data from an open file
 *
 * Read should return exactly the number of bytes requested except
 * on EOF or error, otherwise the rest of the data will be
 * substituted with zeroes.  An exception to this is when the
 * 'direct_io' mount option is specified, in which case the return
 * value of the read system call will reflect the return value of
 * this operation.
 */
int my_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
}

/** Write data to an open file
 *
 * Write should return exactly the number of bytes requested
 * except on error.  An exception to this is when the 'direct_io'
 * mount option is specified (see read operation).
 */
int my_write(const char *path, const char *buf, size_t size, off_t offset,
	     struct fuse_file_info *fi)
{
}

/** Get file system statistics
 *
 * The 'f_frsize', 'f_favail', 'f_fsid' and 'f_flag' fields are ignored
 */
int my_statfs(const char *path, struct statvfs *statv)
{
}

/** Possibly flush cached data
 *
 * BIG NOTE: This is not equivalent to fsync().  It's not a
 * request to sync dirty data.
 *
 * Flush is called on each close() of a file descriptor.  So if a
 * filesystem wants to return write errors in close() and the file
 * has cached dirty data, this is a good place to write back data
 * and return any errors.  Since many applications ignore close()
 * errors this is not always useful.
 *
 * NOTE: The flush() method may be called more than once for each
 * open().  This happens if more than one file descriptor refers
 * to an opened file due to dup(), dup2() or fork() calls.  It is
 * not possible to determine if a flush is final, so each flush
 * should be treated equally.  Multiple write-flush sequences are
 * relatively rare, so this shouldn't be a problem.
 *
 * Filesystems shouldn't assume that flush will always be called
 * after some writes, or that if will be called at all.
 */
int my_flush(const char *path, struct fuse_file_info *fi)
{
}

/** Release an open file
 *
 * Release is called when there are no more references to an open
 * file: all file descriptors are closed and all memory mappings
 * are unmapped.
 *
 * For every open() call there will be exactly one release() call
 * with the same flags and file descriptor.  It is possible to
 * have a file opened more than once, in which case only the last
 * release will mean, that no more reads/writes will happen on the
 * file.  The return value of release is ignored.
 *
 * Changed in version 2.2
 */
int my_release(const char *path, struct fuse_file_info *fi)
{
}

/** Synchronize file contents
 *
 * If the datasync parameter is non-zero, then only the user data
 * should be flushed, not the meta data.
 *
 * Changed in version 2.2
 */
int my_fsync(const char *path, int datasync, struct fuse_file_info *fi)
{
}

#ifdef HAVE_SYS_XATTR_H
/** Set extended attributes */
int my_setxattr(const char *path, const char *name, const char *value, size_t size, int flags)
{
}

/** Get extended attributes */
int my_getxattr(const char *path, const char *name, char *value, size_t size)
{
}

/** List extended attributes */
int my_listxattr(const char *path, char *list, size_t size)
{
}

/** Remove extended attributes */
int my_removexattr(const char *path, const char *name)
{
}
#endif

/** Open directory
 *
 * This method should check if the open operation is permitted for
 * this  directory
 */
int my_opendir(const char *path, struct fuse_file_info *fi)
{
}

/** Read directory
 *
 * This supersedes the old getdir() interface.  New applications
 * should use this.
 *
 * The filesystem may choose between two modes of operation:
 *
 * 1) The readdir implementation ignores the offset parameter, and
 * passes zero to the filler function's offset.  The filler
 * function will not return '1' (unless an error happens), so the
 * whole directory is read in a single readdir operation.  This
 * works just like the old getdir() method.
 *
 * 2) The readdir implementation keeps track of the offsets of the
 * directory entries.  It uses the offset parameter and always
 * passes non-zero offset to the filler function.  When the buffer
 * is full (or an error happens) the filler function will return
 * '1'.
 */
int my_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset,
	       struct fuse_file_info *fi)
{
}

/** Release directory */
int my_releasedir(const char *path, struct fuse_file_info *fi)
{
}

/** Synchronize directory contents
 *
 * If the datasync parameter is non-zero, then only the user data
 * should be flushed, not the meta data
 */
int my_fsyncdir(const char *path, int datasync, struct fuse_file_info *fi)
{
}

/**
 * Initialize filesystem
 *
 * The return value will passed in the private_data field of
 * fuse_context to all file operations and as a parameter to the
 * destroy() method.
 */
void *my_init(struct fuse_conn_info *conn)
{
    return STATIC_DATA;
}

/**
 * Clean up filesystem
 *
 * Called on filesystem exit.
 */
void my_destroy(void *userdata)
{
}

/**
 * Check file access permissions
 *
 * This will be called for the access() system call.  If the
 * 'default_permissions' mount option is given, this method is not
 * called.
 */
int my_access(const char *path, int mask)
{
}

/**
 * Create and open a file
 *
 * If the file does not exist, first create it with the specified
 * mode, and then open it.
 *
 * If this method is not implemented or under Linux kernel
 * versions earlier than 2.6.15, the mknod() and open() methods
 * will be called instead.
 */
int my_create(const char *path, mode_t mode, struct fuse_file_info *fi)
{
}

/**
 * Change the size of an open file
 *
 * This method is called instead of the truncate() method if the
 * truncation was invoked from an ftruncate() system call.
 *
 * If this method is not implemented or under Linux kernel
 * versions earlier than 2.6.15, the truncate() method will be
 * called instead.
 */
int my_ftruncate(const char *path, off_t offset, struct fuse_file_info *fi)
{
}

/**
 * Get attributes from an open file
 *
 * This method is called instead of the getattr() method if the
 * file information is available.
 *
 * Currently this is only called after the create() method if that
 * is implemented (see above).  Later it may be called for
 * invocations of fstat() too.
 */
int my_fgetattr(const char *path, struct stat *statbuf, struct fuse_file_info *fi)
{
}

struct fuse_operations prefix_oper = {
  .getattr = my_getattr,
  .readlink = my_readlink,
  .getdir = NULL,
  .mknod = my_mknod,
  .mkdir = my_mkdir,
  .unlink = my_unlink,
  .rmdir = my_rmdir,
  .symlink = my_symlink,
  .rename = my_rename,
  .link = my_link,
  .chmod = my_chmod,
  .chown = my_chown,
  .truncate = my_truncate,
  .utime = my_utime,
  .open = my_open,
  .read = my_read,
  .write = my_write,
  .statfs = my_statfs,
  .flush = my_flush,
  .release = my_release,
  .fsync = my_fsync,
  
#ifdef HAVE_SYS_XATTR_H
  .setxattr = my_setxattr,
  .getxattr = my_getxattr,
  .listxattr = my_listxattr,
  .removexattr = my_removexattr,
#endif
  
  .opendir = my_opendir,
  .readdir = my_readdir,
  .releasedir = my_releasedir,
  .fsyncdir = my_fsyncdir,
  .init = my_init,
  .destroy = my_destroy,
  .access = my_access,
  .create = my_create,
  .ftruncate = my_ftruncate,
  .fgetattr = my_fgetattr
};


int main(int argc, char *argv[])
{
    umask(0);

    // Check to see there's the right number of arguments
    if (argc != 3)
    {
        fprintf(stderr, "usage:  myfs [Root Directory] [Mount Point]\n");
        return -1;        
    }

    // Create a struct for data that we need to keep.
    // We can access this any time via STATIC_DATA
    struct file_state *static_data;
    static_data = malloc(sizeof(struct file_state));

    // Keep root directory path as static data
    static_data->rootdir = realpath(argv[argc-2], NULL);

    // Remove root directory path from arguments
    argv[argc-2] = argv[argc-1];
    argv[argc-1] = NULL;
    argc--;

    return fuse_main(argc, argv, &prefix_oper, static_data);
}
