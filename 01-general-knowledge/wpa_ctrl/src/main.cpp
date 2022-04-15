#include "wpa.hpp"

int main(int argc, char *argv[])
{
    int opt;
    struct WpaController *mWpaCtrl = WpaController::getInstance();

    for(;;) {
        opt = getopt(argc, argv, "hsdcu");
        if (opt < 0)
            break;
        switch (opt) {
            case 'c':
                mWpaCtrl->config_wifi(optind, argc, argv);
                break;
            case 'd':
                mWpaCtrl->disconnect();
                break;
            case 'h':
                mWpaCtrl->usage();
                return 0;
            case 'u':
                break;
            case 's':
                mWpaCtrl->scan_results();
                break;
            case '?':
                cout << "unknow option: " << (char)optopt << endl;
                break;
            default:
                mWpaCtrl->usage();
                return 1;
        }
    }
}