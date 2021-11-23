/**************************************************************************************************************
 <Copyright PHONGLT9>
Product:
Module:
Version: 1.0
Author:  phonglt9
Created: 11/10/2021
***************************************************************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>

/**************************************************************************************************************
Macro definitions
***************************************************************************************************************/
/* for debbugging purposees only */
//#define DEBUG   0x01

#ifdef DEBUG
#undef DEBUG
#define DEBUG(fmt, args...) { log_printf (LOG_ERROR, "%d: ", __LINE__); log_printf (LOG_ERROR, fmt, ## args); }
#else
#undef DEBUG
#define DEBUG(fmt, args...) { log_printf (LOG_NORMAL, "%d: ", __LINE__); log_printf (LOG_NORMAL, fmt, ## args); }
#endif

/* errors level */
#define LOG_NORMAL      0x01
#define LOG_ERROR       0x02

/**************************************************************************************************************
Constants variables

/**************************************************************************************************************
Global variables
***************************************************************************************************************/
#define KERNEL_PARTITION    "/dev/mtd1"
#define ROOTFS_PARTITION    "/dev/mtd2"

#define LOG     0x01
#define log(...) {  \
    do {    \
        if ((LOG)) { \
            printf("\n[%s:%s:%d] ==>> ", __FILE__, __func__, __LINE__); \
            printf(__VA_ARGS__); \
            printf("\n"); \
        } \
    } while (0); \
} // Logging

/**************************************************************************************************************
Function declaration
***************************************************************************************************************/


/**************************************************************************************************************
Function implementations
***************************************************************************************************************/

/*!
*   @brief          Print log
*                   <BR>
*                   This function will print log
*
*   @param[in]      level   LOG_NORMAL/LOG_ERROR
*   @param[in]      fmt     Format to print   
*   @return         NONE
*/
static void log_printf(int level, const char *fmt, ...) 
{
    FILE *fp = level == LOG_NORMAL ? stdout : stderr;
    va_list ap; //avarage parameters
    va_start(ap, fmt);
    vfprintf(fp, fmt, ap);
    va_end(ap);
    fflush(fp);
}

int main(int argc, char const *argv[])
{
    struct stat sb;

    if (argc != 2) {
        log("Usage: ./file-infor /path/to/file");
        exit(EXIT_FAILURE);
    }

    if (-1 == lstat(argv[1], &sb)) {
        log("lstat");
        exit(EXIT_FAILURE);
    }

    printf("File Type: ");
    switch (sb.st_mode & S_IFMT) {
        case S_IFBLK:   printf("block device\n");       break;
        case S_IFCHR:   printf("character device\n");   break;
        case S_IFDIR:   printf("directory\n");          break;
        case S_IFIFO:   printf("FIFO/pipe\n");          break;
        case S_IFLNK:   printf("symlink\n");            break;
        case S_IFREG:   printf("regular file\n");       break;
        case S_IFSOCK:  printf("socket\n");             break;
        default:        printf("unknown?\n");
    }
    
    printf("\nI-node number: %ju, sizeof(uintmax_t): %jd\n\n", (uintmax_t)sb.st_ino, sizeof(uintmax_t));
    printf("File size: %jd bytes\n\n", (intmax_t)sb.st_size);

    printf("Last status change:     %s\n", ctime(&sb.st_ctim.tv_sec));
    printf("Last file access:       %s\n", ctime(&sb.st_atim.tv_sec));
    printf("Last file modification: %s\n", ctime(&sb.st_mtim.tv_sec));

    
    return 0;
}
