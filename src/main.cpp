#include "utils.h"
#include "mybot.hpp"

#include <vector>
#include <atomic>
#include <sys/wait.h>
#include <thread>

void set_global_log(LOG type, std::string message){
    std::time_t nt =
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    tm tt = *localtime(&nt);
    std::ostringstream oss;
    oss << "[" << std::setw(2) << std::setfill('0') << tt.tm_hour << ":"
        << std::setw(2) << std::setfill('0') << tt.tm_min << ":" << std::setw(2)
        << std::setfill('0') << tt.tm_sec << "][" << LOG_name[type] << "] "
        << message << std::endl;
    if (type == LOG::ERROR)
        std::cerr << oss.str();
    else
        std::cout << oss.str();
}

std::vector<int> send_port, receive_port;
std::vector<bot*> bots;

void bot_run(bot *u){
    while(1){
        int k = fork();
        if(k==-1){
            std::cerr<< "Process Error!"<<std::endl;
        } else if(k==0){
            u->run();
            exit(0);
        } else {
            waitpid(k, NULL, 0);
        }
    }
}

void add_new_bot(bot *t){
    bots.push_back(t);
    std::thread u = std::thread(bot_run, t);
    u.detach();
}

int main()
{
    curl_global_init(CURL_GLOBAL_ALL);
    signal(SIGPIPE, SIG_IGN);
    signal(SIGALRM, SIG_IGN);
    std::ifstream iport("./config/port.txt");
    int x, y;
    if (iport.is_open()) {
        while(!iport.eof()){
            iport >> x >> y;
            send_port.push_back(x);
            receive_port.push_back(y);
        }
        iport.close();
    }
    else {
        std::cout << "Please input the send_port: (receive port in go-cqhttp):";
        std::cin >> x;
        std::cout << "Please input the receive_port: (send port in go-cqhttp):";
        std::cin >> y;
        send_port.push_back(x);
        receive_port.push_back(y);
        std::ofstream oport("./config/port.txt");
        if (oport) {
            oport << x << ' ' << y;
            oport.flush();
            oport.close();
        }
    }

    int len = send_port.size();

    add_new_bot(new mybot(receive_port[0], send_port[0]));
    
    while(true) sleep(10);

    //Never goes here~

    for (bot *t : bots) {
        delete t;
    }

    curl_global_cleanup();

    return 0;
}
