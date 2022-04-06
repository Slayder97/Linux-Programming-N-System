#include <iostream>
#include <string.h>
#include <unistd.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <net/if.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/types.h>

extern "C" {
#include "wpa_ctrl.h"
}   

using namespace std;

#define NETWORK_ID_MAX 8
#define INTERFACE  "wlan0"
#define CTRL_IFACE "/var/run/wpa_supplicant/wlan0"

class WpaController
{
private:
    static WpaController* _mWpaCtrl;
    struct wpa_ctrl *ctrl_conn;
    char *ssid;
    char *psk;
    WpaController();
public:
    static WpaController* getInstance();
    void usage(void);
    int scan_results(void);
    int config_wifi(int, int, char **);
    int add_network();
    int set_network(int netid, char *argv1, char *argv2);
    int select_network(int netid);
    int remove_network(int netid);
    int save_config();
    int reconfigure();
    int disconnect();
    int status();
};

static void parse_to_wifi_name(char *buff);
static int check_status(char *buffer, char *ssid);
static int _wpa_ctrl_cmd(struct wpa_ctrl *ctrl, char *cmd, char *buf);