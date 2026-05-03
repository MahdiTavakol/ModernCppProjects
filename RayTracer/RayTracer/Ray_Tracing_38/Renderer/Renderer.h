#ifndef RENDERER_H
#define RENDERER_H
#include <memory>
#include <string>

#include "../Algorithms/communicator.h"
#include "../Algorithms/path.h"
#include "../Algorithms/camera.h"
#include "../Output/output.h"
#include "../Algorithms/hittable_list.h"


class renderer
{
public:
	renderer(communicator* para_,std::unique_ptr<path>&& pth_, std::string info_ = "", bool verbose_ = false);

	virtual ~renderer();
	virtual void render(camera* cam_, output* writer_, hittable_list* world_);
	virtual void write_file(output* writer_);
	
	

protected:
	std::string info = "";
	bool verbose = false;
	communicator* para;

	std::unique_ptr<path> pth;

	void message(std::string _text);
	std::string filename;
};

#endif
