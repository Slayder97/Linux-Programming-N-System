#include "wpa.hpp"

WpaController* WpaController::_mWpaCtrl = NULL;
WpaController::WpaController()
{
    this->ctrl_conn = wpa_ctrl_open(CTRL_IFACE);
    if (this->ctrl_conn == NULL)
        printf("Could not get ctrl interface!\n");
}

WpaController* WpaController::getInstance() 
{
    if (_mWpaCtrl == NULL)
        _mWpaCtrl = new WpaController();
    return _mWpaCtrl;
}

void WpaController::usage(void)
{
    cout << "usage: sudo wpa_ctrl [-h ] [-s ] [-d  ] [-u ] [-c <ssid> <psk> ]" << endl;
    cout << "list option:" << endl;
    cout << "  -h = help (show this usage text)"  << endl;
    cout << "  -s = scan (scan all network)"      << endl;
    cout << "  -c = connect (connect to wifi)"    << endl;
    cout << "       <ssid>: wifi name " << endl;
    cout << "       <psk> : password"   << endl;
    cout << "  -d = disconnect (wifi down)" << endl;
    cout << "  -u = up (wifi up)"  << endl;
    cout << "ex: sudo wpa_ctrl -c \"ALL LUMI\" lumivn274" << endl;
    cout << "default etc path: /etc/wpa_supplicant/wpa_supplicant.conf" << endl;
}

static int _wpa_ctrl_cmd(struct wpa_ctrl *ctrl, char *cmd, char *buf)
{
    int ret;
    size_t len = 4096;

    ret = wpa_ctrl_request(ctrl, cmd, strlen(cmd), buf, &len, NULL);
    if (ret == -2) {
        printf("'%s' command timed out.\n", cmd);
            return -2;
    } else if (ret < 0) {
        printf("'%s' command failed.\n", cmd);
        return -1;
    }
    buf[len -1] = '\0';

    return 0;
}

static void parse_to_wifi_name(char *buffer)
{
    int r_size = 0;
    string myStr(buffer), val, line;
    stringstream ss(myStr);
    vector<vector<string>> array;

    while (getline(ss, line, '\n')) {
        vector<string> row;
        stringstream s(line);
        while (getline(s, val, '\t')) {
            row.push_back (val);      
        }
        array.push_back (row);
        r_size++;   
    }

    for (int i = 1; i < r_size -1;i++) {
        if (array[i].size() != 5)
            continue;
        cout << '\t' << array[i][4] << endl;
    }         
}

static int check_status(char *buffer, char *ssid)
{
    string myStr(buffer), val;
    stringstream ss(myStr);

    while (getline(ss, val)) {
        if (val.size() > 4) {
            if (val.compare(5, strlen(ssid),ssid) == 0) {
                return 0;
            }
        }
    }

    return -1;
}

int WpaController::add_network()
{
    char ch[2];
    int netid;

    if ((_wpa_ctrl_cmd(this->ctrl_conn, "ADD_NETWORK", ch)) < 0)
        return -1;
    sscanf(ch, "%d", &netid);
    
    return netid;
}

int WpaController::set_network(int netid, char *argv1, char *argv2)
{
    char buffer[1024];
    char cmd[64];

    sprintf(cmd, "%s %d %s \"%s\"", "SET_NETWORK", netid, argv1, argv2);
    if ((_wpa_ctrl_cmd(this->ctrl_conn, cmd, buffer)) < 0)
        return -1;

    return 0;
}

int WpaController::select_network(int netid)
{
    char buffer[1024];
    char cmd[64];

    sprintf(cmd, "%s %d", "SELECT_NETWORK", netid);
    if ((_wpa_ctrl_cmd(this->ctrl_conn, cmd, buffer)) < 0)
        return -1;
    cout << "Please wait a few seconds ..." << endl;

    return 0;
}

int WpaController::remove_network(int netid)
{
    char buffer[1024];
    char cmd[64];

    sprintf(cmd, "%s %d", "REMOVE_NETWORK", netid);
    if ((_wpa_ctrl_cmd(this->ctrl_conn, cmd, buffer)) < 0)
        return -1;
    cout << buffer << endl;

    return 0;
}

int WpaController::save_config()
{
    char buffer[1024];

    if ((_wpa_ctrl_cmd(this->ctrl_conn, "SAVE_CONFIG", buffer)) < 0)
        return -1;

    return 0; 
}

int WpaController::reconfigure()
{
    char buffer[1024];

    if ((_wpa_ctrl_cmd(this->ctrl_conn, "RECONFIGURE", buffer)) < 0)
        return -1;

    return 0; 
}

int WpaController::disconnect()
{
    char buffer[1024];

    if ((_wpa_ctrl_cmd(this->ctrl_conn, "DISCONNECT", buffer)) < 0)
        return -1;
    cout << buffer << endl;

    return 0; 
}

int WpaController::status()
{
    char buffer[1024];

    if ((_wpa_ctrl_cmd(this->ctrl_conn, "STATUS", buffer)) < 0)
        return -1;
    
    if ((check_status(buffer, this->ssid)) < 0)
        cout << "Please check wifi name or password" << endl;
    else
        cout << "Wifi connected" << endl;    

    return 0;
}

int WpaController::scan_results(void)
{   
    int ret;
    char *buff = new char[4096];
    char *OK = new char[2];

    this->ctrl_conn = wpa_ctrl_open(CTRL_IFACE);
    if (!this->ctrl_conn){
        printf("Could not get ctrl interface!\n");
        return -1;
    }

    if ((ret = _wpa_ctrl_cmd(this->ctrl_conn, "SCAN", OK)) < 0)
        return -1;
    else
        cout << "scan wifi status: " << OK << endl;

    if ((ret = _wpa_ctrl_cmd(this->ctrl_conn, "SCAN_RESULTS", buff)) < 0) 
        return -1;
    else
        cout << "List SSID: " << endl;
    
    parse_to_wifi_name(buff);

    delete[] buff;
    delete[] OK;

    return 0;
}

int WpaController::config_wifi(int sPoint, int ePoint, char *argv[])
{
    int val, netid;
    char *pos;
    char *buff = new char[64];

    val = ePoint - sPoint;
    if (val < 1) {
        cout << "ex: sudo wpa_ctrl -c \"ALL LUMI\" lumivn274" << endl;
        return -1;
    }
    
    this->ssid = argv[sPoint];

    if (val > 1) {
        this->psk = argv[sPoint + 1];
    } else {
        cout << "Reading psk from stdin" << endl;
        if (fgets(buff, sizeof(buff), stdin) == NULL) {
            cout << "Failed to read psk from stdin" << endl;
            return -1;
        }
        buff[sizeof(buff) -1] = '\0';
        pos = buff;
        while (*pos != '\0') {
            if (*pos == '\r' || *pos == '\n') {
                *pos = '\0';
                break;
            }
            pos++;
        }
        this->psk = buff;
    }
    
    if (strlen(this->psk) < 8 || strlen(this->psk)> 63) {
        cout << "Size: " << strlen(this->psk) << endl << "Psk must be 8...63 characters\n" <<endl;
        return -1;
    }

    netid = add_network();
    if (netid > NETWORK_ID_MAX) {
        netid = NETWORK_ID_MAX;
        remove_network(0);
        save_config();
        reconfigure();
    }
    set_network(netid, "ssid", this->ssid);
    set_network(netid, "psk", this->psk);
    select_network(netid);
    sleep(7);
    status();

    delete[] buff;

    return 0;
}
