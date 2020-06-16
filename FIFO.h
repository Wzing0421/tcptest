#ifndef _FIFO_H_
#define _FIFO_H_
#include <cstring>
#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

struct SoundFrame {
	int length;
	char *data;
	SoundFrame(int len, char * sounddata) :length(len), data(sounddata){}
};

template<typename T>
class FIFO {
private:
std::queue<T> q;
std::mutex mut_;
std::condition_variable cv_;
int max;
public:
FIFO(int max_ = 100000){
	max = max_;
}
int push(T &data){
	std::unique_lock<std::mutex> lk(mut_);
	if((int)q.size() >=(int) max) {
		return -1;
	}
	q.push(data);
	if(q.size() == 1) {
		cv_.notify_one();
	}
	return 0;
}
int pop_nolock(T &data){
	std::unique_lock<std::mutex> lk(mut_);
	if(q.empty()) {
		return -1;
	}
	data = q.front();
	q.pop();
	return 0;
}
int pop(T &data){
	std::unique_lock<std::mutex> lk(mut_);
	while(q.empty()) {
		cv_.wait(lk);
	}
	data = q.front();
	q.pop();
	return 0;
}
};
#endif
