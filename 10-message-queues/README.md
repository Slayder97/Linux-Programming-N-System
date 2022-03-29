# Vấn đề với SHARED MEMORY với MESSAGE QUEUE

- Nếu dữ liệu giao tiếp kích thước nhỏ.
- Dữ liệu shared memory cần được bảo vệ với synchronization khi mutil process truy cập cùng một thời điểm.
- Điều gì xảy ra nếu rất ít tiến trình cần truy cập vào shared memory. Sẽ tốt hơn nếu dùng hàng đợi.
- Nếu ta muốn gửi các gói dữ liệu khác nhau. 
    Process A gửi message type1 tới process B, message type 2 tới C, type 20 tới D. Trong trường hơp này, cách đơn giản nhất là dùng message queue. 
- Thứ tự của hàng đợi là FIFO.


https://www.tutorialspoint.com/inter_process_communication/inter_process_communication_shared_memory.htm
https://eslinuxprogramming.blogspot.com/2015/06/posix-vs-system-v.html