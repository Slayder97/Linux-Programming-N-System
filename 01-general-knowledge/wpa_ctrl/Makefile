.PHONY: all clean copy

PRO_DIR     := .
PROJ_NAME   := wpa_ctrl
OUTPUT_PATH := .

SRC_DIR := $(PRO_DIR)/src
INC_DIR := $(PRO_DIR)/inc
CFLAGS  :=  -O2 -Wall -Wextra -g  \
  -Iusr_ARM/include/libnl3 \
  -DCONFIG_BACKEND_FILE -DCONFIG_IEEE80211W  -DCONFIG_DRIVER_WEXT \
  -DCONFIG_WIRELESS_EXTENSION  -DCONFIG_DRIVER_NL80211 -DCONFIG_LIBNL20 \
  -DEAP_PSK -DIEEE8021X_EAPOL -DCONFIG_SHA256 -DCONFIG_CTRL_IFACE \
  -DCONFIG_CTRL_IFACE_UNIX  -DCONFIG_SME

C_FILES   :=  wpa_debug.c os_unix.c eloop.c edit_simple.c  wpa_ctrl.c common.c
OBJ_FILES := $(patsubst %.c, $(SRC_DIR)/%.o, $(C_FILES))

COMPILER_DIR 	:= /home/pi/work_space/wpa_ctrl/mt9050-cross-sdk-3.0.4/host/usr
PREFIX_GCC_COMPILER	:= arm-linux-gnueabihf
CC  := $(COMPILER_DIR)/bin/$(PREFIX_GCC_COMPILER)-gcc
CPP := $(COMPILER_DIR)/bin/$(PREFIX_GCC_COMPILER)-g++

%.o: %.c $(INC_DIR)
	gcc -c -o $@ $< -I $(INC_DIR) $(CFLAGS)

my_obj:
	g++ -c -o $(SRC_DIR)/wpa.o $(SRC_DIR)/wpa.cpp -I $(INC_DIR) $(CFLAGS)
	g++ -c -o $(SRC_DIR)/main.o $(SRC_DIR)/main.cpp -I $(INC_DIR) $(CFLAGS)

all: clean my_obj $(OBJ_FILES)
	g++ $(CFLAGS) -lrt -o wpa_ctrl $(SRC_DIR)/main.o  $(SRC_DIR)/wpa.o $(OBJ_FILES) -I $(INC_DIR)

clean:
	rm -rf wpa_ctrl
	rm -rf $(SRC_DIR)/*.o $(SRC_DIR)/*.d
