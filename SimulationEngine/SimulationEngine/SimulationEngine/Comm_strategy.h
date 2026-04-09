#pragma once

class Comm_strategy {
public:
	Comm_strategy() = default;
	int getRank();
	int getSize();

	virtual void update(double* buffer_,const int size_, const int tag_, const int src_, const int dst_) = 0;
	virtual void send(double* buffer_, const int size_, const int dst_, const int tag_) = 0;
	virtual void recv(double* buffer_, const int size_, const int src_, const int tag_) = 0;
	virtual void waitAll() = 0;

protected:
	int rank, size;

};
