#pragma once

class Comm_strategy {
public:
	Comm_strategy() = default;
	int getRank();
	int getSize();

protected:
	int rank, size;

};
