#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>

#define SHARED_MEM_SIZE     100             /* the size (in bytes) of shared memory object */
#define FILE_NAME           "phong_mmap"     /* name of the shared memory object */

/**
 * 1. Tạo fd
 * 2. Set size
 * 3. map/unmap process memory 
 * 4. Xóa shared memory segment
 */

int main() 
{ 
    /**
     *  int shm_open (const char *name, int oflag, mode_t mode);
     *  @brief 
     * 
     *  @param[in] name File path
     *  @param[in] oflag O_RDONLY --> process open shared memory object với quyền ReadOnly.
     *                   O_RDWR   --> process open shared memory object với quyền Read/Write.
     *                   O_CREAT  --> tạo shared memory segment nếu không tồn tại.
     *                   O_EXCL   --> nếu như shared memory object với file được đưa ra đang tồn tại thì trả về lỗi EEXIST.
     *                   O_TRUNC  --> nếu shared memory object đang tồn tại thực hiện truncate tới giá trị 0.
     *  @param[in] mode permission của shared memory object được tạo. Nếu oflag không sử dụng cờ O_CREAT hay 
     *                  shared memory object đang tồn tại thì mode sẽ bị bỏ qua.
     *                  
     *  @return Sau khi tạo thành công thì có thể thấy file liên kết với shared memory trong đường dẫn, -1 nếu lỗi.
     */
    int shm_fd = shm_open(FILE_NAME, O_CREAT | O_RDWR, 0666); 
    if (shm_fd < 0) {
        printf("shm_open() is failed, %s.\n", strerror(errno));
        return -1;
    }

    /**
     * int ftruncate (int shm_fd, off_t length);
     * @brief Configure the size of the shared memory object.
     *        After opening, the size of the shared memory object is 0 
     * 
     * @param[in] shm_fd shm file descriptor
     * @param[in] length the size (in bytes) of shared memory object
     * 
     * @return
     */ 
    ftruncate(shm_fd, SHARED_MEM_SIZE);
    
    /**
     * void *mmap (void *addr, size_t length, int prot, int flags, int fd, off_t offset);
     * @brief Configure the size of the shared memory object.
     * 
     * @param[in] addr địa chỉ memory của calling process được map. Tương tự như shm nên 
     *                 set giá trị là 0 và kernel sẽ allocation addr phù hợp
     * @param[in] length độ dài của shared memory object được map (> 0).
     * @param[in] prot memory protection of mapping (không được conflict với file permission).
     *                 Có thể có các giá trị sau:
     *                 PROT_EXEC  ---> Pages may be executed.
     *                 PROT_READ  ---> Pages may be read.
     *                 PROT_WRITE ---> Pages may be written.
     *                 PROT_NONE  ---> Pages may not be accessed. 
     * @param[in] flags cờ cho phép các process có thể thấy được cập nhật của các process vào 
     *                  shared memory hay không.
     *                  MAP_SHARED  ---> cập nhật của process này vào shared memory là available với các process khác. 
     *                  MAP_PRIVATE ---> kernel sử dụng phương thức copy-on-write để tạo ra vùng nhớ riêng cho thay 
     *                              đổi của process. Các thay đổi này sẽ không visiable đối với các process khác.
     *                  Ngoài ra, ta có thể dùng phép OR để kết hợp với một số cờ sau:
     *                  MAP_ANONYMOUS ---> Tạo ra anonymous mapping. Toàn bộ ô nhớ trên vùng này được khởi tạo là 0.
     *                  Không cần quân tâm tới fd.
     *                  MAP_UNINITIALIZED ---> Không khởi tạo vùng anonymous mapping.
     *                              
     * @param[in] fd shm file descriptor
     * @param[in] offset offset của shared memory object sẽ được maping vào virtual memory của process.
     * 
     * @return  On success, mmap() returns a pointer to the mapped area.
     *          On error, the value MAP_FAILED (that is, (void *) -1) is returned,  
     *          and errno is set to indicate the cause of the error.
     */
    char *data = (char *)mmap(0, SHARED_MEM_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE, shm_fd, 0);

    strcpy(data, "MMAP Hello world !");
    printf("%s: Write data: %s\n", __FILE__, data);

    /**
     * int munmap(void *addr, size_t length);
     * @brief Unmap shared memory object khỏi calling process.
     * 
     * @param[in] addr địa chỉ memory của calling process được map.
     * @param[in] length độ dài của shared memory object được map (> 0).
     * 
     * @return On success, munmap() returns 0. 
     *         On failure, it returns -1, and errno is set to 
     *         indicate the cause of the error (probably to EINVAL).
     */ 
    munmap(data, SHARED_MEM_SIZE);

    //close(shm_fd);
  
    return 0; 
}
