Directory structure: 
- flock : file locking by flock
- fcntl : file locking by fcntl
- file-infor: get file's information using <struct stat>.
- open-rw   : basic linux system call open/read/write/lseek/close.
- rw-sync   : file asynchronous with aio_read/aio_write.

In each folder, perform below instructions:

- make: build excutable file.
- make clean: clean everything.
