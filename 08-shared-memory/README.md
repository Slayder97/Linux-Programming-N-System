
# Đặt vấn đề

## Hàm

## ftok(): is use to generate a unique key.

## shmget(): 
    int shmget(key_t,size_tsize,intshmflg); upon successful completion, shmget() returns an identifier for the shared memory segment.

## shmat(): 
    Before you can use a shared memory segment, you have to attach yourself to it using shmat(). void *shmat(int shmid ,void *shmaddr ,int shmflg);
    shmid is shared memory id. shmaddr specifies specific address to use but we should set it to zero and OS will automatically choose the address.

## shmdt(): 
    When you’re done with the shared memory segment, your program should
    detach itself from it using shmdt(). int shmdt(void *shmaddr);

## shmctl(): 
    when you detach from shared memory,it is not destroyed. So, to destroy
    shmctl() is used. shmctl(int shmid,IPC_RMID,NULL);


# Vấn đề với PIPE, FIFO, MESSAGE QUEUE

- Server đọc từ file input.
- Server ghi dữ liệu vào FIFO, PIPE, MESSAGE QUEUE 
- Client đọc dữ liệu từ kệnh IPC tương ứng. Lưu trữ dữ liệu trong buffer IPC của nhân tới client's buffer
- Cuối cùng dữ liệu được sao chép từ client's buffer.
===> Xem ví dụ named_pipe

===> Tổng cộng 4 bản sao dữ liệu được yêu cầu (2 write, 2 read). Với shared memory dữ liệu được sao chép 2 lần. Từ input file tới shared memory và từ shared memory tới output file.