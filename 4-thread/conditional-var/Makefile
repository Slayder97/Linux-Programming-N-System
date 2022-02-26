# Conditional Variable

.PHONY := conVar pooling

CC := gcc
CFLAGS := -pthread

pooling:
	$(CC) -o exam main.c $(CFLAGS)

conVar:
	$(CC) -o con_var con_var.c $(CFLAGS)

clean:
	rm -rf  exam con_var