#include "TCPSocket.h"
#include "UDPSocket.h"
#include <pthread.h>
#include <map>
#include <unordered_map>

#include <string>
#include <iostream>
#include <time.h>
#include <sys/time.h>

#define THREADNUM 28
#define LENGTH  18 
#define LENGTH2 160
using namespace std;

struct IPPort{
    string dstip;
    unsigned int dstport;
    IPPort(string ip, unsigned int port):dstip(ip), dstport(port){} 
};

unordered_map<unsigned int, IPPort> mp;

void buildmap(){
    for(unsigned int i = 0; i < 65000; i++){
        string IP = "162.105.85.235";
        unsigned int port = i + 100;
        unsigned int ueid = port + 200;;
        IPPort ipt(IP, port);
        mp.insert(make_pair(ueid, ipt));
    }
}

int main(){

    struct timeval startTime, endTime;
    
    TCPClient tcpclient;
    string dstip = "162.105.85.235";
    unsigned int dstport = 20000;
    if(0 != tcpclient.tcpconnect(dstip, dstport)){
        cout << "client connect error" << endl;
        return -1;
    }

    char buf[LENGTH];
    char buf1[LENGTH2];
    memset(buf, 0x01, sizeof(buf));
    gettimeofday(&startTime, NULL);
    
    struct timeval tcpstart, tcpend;
    float timeuse;
    float maxtimeuse = -1.0;
    float mintimeuse = 10000000;
    for(int i = 0; i < 1000000; i++){
        gettimeofday(&tcpstart, NULL);
        tcpclient.sendbuf(buf, sizeof(buf));
        tcpclient.recvbuf(buf1, LENGTH2);
        
        gettimeofday(&tcpend, NULL);
        timeuse = 1000000 * (tcpend.tv_sec - tcpstart.tv_sec) + (tcpend.tv_usec - tcpstart.tv_usec);
        maxtimeuse = max(maxtimeuse, timeuse);
        mintimeuse = min(mintimeuse, timeuse);
    }
    gettimeofday(&endTime, NULL);
    float totaltimeuse;
    totaltimeuse = 1000000 * (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec);
    totaltimeuse /= 1000000;
    cout << "maxtimeuse : " << maxtimeuse << " us" << endl;
    cout << "mintimeuse : " << mintimeuse << " us" << endl;
    cout << "totaltimeuse : " << totaltimeuse <<" s" << endl;
    tcpclient.tcpclose();

    return 0;

}
int test1(int a){
    return a;
}
