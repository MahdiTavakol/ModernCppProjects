#include "bvh_triangles.h"
#include <numeric>

// the c'tor for the main root
bvh_triangles::bvh_triangles(
	std::unique_ptr<triangle_list> list_,
	BVH_Split_Method split_method_) :
	bvh_node{"bvh_triangles",split_method_},
	non_triangles_bvh{ std::make_unique<bvh_node>("non_triangle_bvh",split_method_)},
	type{Node_Type::ROOT}
{
	// number of levels
	size_t num_levels = log2(list_->size()) + 1;
	// the size of boxes array
	size_t array_size = std::pow(2, num_levels + 1) - 1;


	bboxes.resize(array_size);

	// putting the input data
	list = std::move(list_);


	size_t start = 0;
	size_t end = list->size();

	set_left_right(
		start, end,
		0,
		0,
		split_method_);

	// getting the triangles object
	triangles = list->return_triangles();

	// taking care of non_triangles objects
	std::unique_ptr<hittable_list> non_triangles = list->return_non_triangles();
	non_triangles_bvh = std::make_unique<bvh_node>(std::move(non_triangles));

}

bvh_triangles::~bvh_triangles()
{
	switch (type)
	{
	case Node_Type::ROOT:
		//[[fallthrough]]
	case Node_Type::INTERNAL:
		left.reset();
		right.reset();
		break;
	case Node_Type::LEAF:
		//[[fallthrough]]
	case Node_Type::EMPTY:
		//[[fallthrough]]
	default:
		// nothing to do
		break;
	}

	// clearing the shared variables
	if (type == Node_Type::ROOT)
	{
		list.reset();
		bboxes.clear();
		triangles.clear();
	}
}

bvh_triangles::bvh_triangles() :
	bvh_node{},
	non_triangles_bvh{std::unique_ptr<bvh_node>()},
	type{ Node_Type::EMPTY },
	box_indx{ 0 }
{
	// puting this empty on purpose
	// so that in the first line of the hit a false is returned. 
	bbox = aabb::empty;
}

bvh_triangles::bvh_triangles(
	size_t start_, size_t end_,
	const int indx_,
	const int level_,
	BVH_Split_Method split_method_) :
	bvh_node{},
	non_triangles_bvh{ std::unique_ptr<bvh_node>() },
	type{ Node_Type::INTERNAL }
{
	set_left_right(
		start_, end_,
		indx_,
		level_,
		split_method_);
}


bvh_triangles::bvh_triangles(
	const size_t& box_indx_, 
	const size_t& triangle_indx_,
	const aabb& bbox_,
	Node_Type type_) :
	bvh_node{},
	box_indx{ box_indx_ },
	triangle_indx{triangle_indx_},
	non_triangles_bvh{ std::unique_ptr<bvh_node>() },
	type{ type_ }
{
	if (box_indx >= bboxes.size())
	{
		bboxes.resize(box_indx);
	}
	bboxes[box_indx] = bbox_;
}

void bvh_triangles::set_left_right(
	size_t start_, size_t end_,
	const int indx_,
	const int level_,
	BVH_Split_Method split_method_)
{
	bbox = aabb::empty;

	for (size_t object_index = start_; object_index < end_; object_index++)
	{
		aabb bbox_i = list->return_bbox(object_index);
		bbox = aabb(bbox, bbox_i);
	}
	
	size_t bbox_size = bboxes.size();
	int base_indx = std::pow(2, level_) - 1;
	box_indx = base_indx + indx_;
	if (bbox_size <= box_indx)
		bboxes.resize(box_indx);
	bboxes[box_indx] = bbox;


	size_t object_span = end_ - start_;

	int left_indx = 2 * indx_;
	int right_indx = 2 * indx_ + 1;
	

	if (object_span == 0) {
		return;
	}
	else if (object_span == 1) {
		size_t next_base_indx = std::pow(2, level_ + 1) - 1;
		size_t left_bbox_indx = next_base_indx + left_indx;
		size_t right_bbox_indx = next_base_indx + right_indx;
		size_t left_triangle_indx = start_;
		size_t right_triangle_indx = -1;
		aabb bbox_right = aabb::empty;
		aabb bbox_left = list->return_bbox(start_);
		// creating a leaf
		left = 
			std::unique_ptr<bvh_triangles>(
				new bvh_triangles(
					left_bbox_indx, left_triangle_indx,bbox_left,Node_Type::LEAF));
		// empty object list
		right = 
			std::unique_ptr<bvh_triangles>(
				new bvh_triangles(
					right_bbox_indx, right_triangle_indx, bbox_right, Node_Type::EMPTY));
	}
	else if (object_span == 2)
	{
		size_t next_base_indx = std::pow(2, level_ + 1) - 1;
		size_t left_bbox_indx = next_base_indx + left_indx;
		size_t right_bbox_indx = next_base_indx + right_indx;
		size_t left_triangle_indx = start_;
		size_t right_triangle_indx = start_ + 1;
		aabb bbox_left = list->return_bbox(start_);
		aabb bbox_right = list->return_bbox(start_ + 1);
		// creating leaves
		right = std::unique_ptr<bvh_triangles>(
			new bvh_triangles(left_bbox_indx,left_triangle_indx,bbox_left,Node_Type::LEAF));
		left = std::unique_ptr<bvh_triangles>(
			new bvh_triangles(right_bbox_indx,right_triangle_indx,bbox_right,Node_Type::LEAF));
	} 
	else {

		list->sort_range(start_, end_);

		switch (split_mode)
		{
		case BVH_Split_Method::MEDIAN:
		{
			auto mid = start_ + object_span / 2;
			left = std::unique_ptr<bvh_triangles>(
			new bvh_triangles(
				start_, mid,
				left_indx,
				level_ + 1,
				split_method_));
			right = std::unique_ptr<bvh_triangles>(
			new bvh_triangles(
				mid, end_,
				right_indx,
				level_ + 1,
				split_method_));
			break;
		}
		case BVH_Split_Method::SAH_SIMPLE:
		{
			std::vector<aabb>& list_boxes_ref = list->return_bboxes_ref();
			int len = end_ - start_;

			std::vector<aabb> left_boxes(len);
			std::vector<aabb> right_boxes(len);

			aabb bbox_left = aabb::empty;
			aabb bbox_right = aabb::empty;

			left_boxes[0] = bbox_left;

			// the left does not include the divider element
			// while right includes that element.

			for (int i = 1; i < len; i++)
			{
				bbox_left = aabb(bbox_left, list_boxes_ref[start_ + i - 1]);
				left_boxes[i] = bbox_left;
			}

			for (int i = len - 1; i >= 0; i--)
			{
				bbox_right = aabb(bbox_right, list_boxes_ref[start_ + i]);
				right_boxes[i] = bbox_right;
			}

			std::vector<double> Cfactors(len);
			// splits with at least one element on each side
			for (int i = 1; i < len; i++)
			{
				const aabb& left_bx = left_boxes[i];
				const aabb& right_bx = right_boxes[i];
				double left_area = left_bx.surface_area();
				double right_area = right_bx.surface_area();
				Cfactors[i] = i * left_area + (len - i) * right_area;
			}
			auto min_iter = std::min_element(Cfactors.begin() + 1, Cfactors.end());
			int mid = std::distance(Cfactors.begin(), min_iter) + start_;
			left = std::unique_ptr<bvh_triangles>(
			new bvh_triangles(
				start_, mid,
				left_indx,
				level_ + 1,
				split_method_));
			right = std::unique_ptr<bvh_triangles>(
			new bvh_triangles(
				mid, end_,
				right_indx,
				level_ + 1,
				split_method_));
			break;
		}
		default:
			break;
		}

	}
}


bool bvh_triangles::hit(const ray& r_, interval ray_t_, hit_record& rec_) const
{
	// Did the ray hit my own box?
	// I used the shared array on purpose for the cache locality purposes
	aabb& myBox = bboxes[box_indx];
	if (!myBox.hit(r_, ray_t_))
		return false;

	// If it is either internal or root deligate it to the child nodes
	if (type == Node_Type::INTERNAL)
	{
		bool hit_left = left->hit(r_, ray_t_, rec_);
		bool hit_right = right->hit(r_, interval(ray_t_.min, hit_left ? rec_.t : ray_t_.max), rec_);

		return hit_left || hit_right;
	}
	else if (type == Node_Type::LEAF)
	{
		// It is a leaf
		bool hit_me = false;
		triangle_struct& triangle_i = triangles[triangle_indx];
		hit_me = triangle_list::hit_triangle(r_, ray_t_, rec_, triangle_i);
		return hit_me;
	}
	else if (type == Node_Type::ROOT)
	{
		// The root is in charge of objects which are not triangles
		// I put this part here on purpose since there is just one root node and
		// I do not want to check if it is the root for every node!
		bool hit_left = left->hit(r_, ray_t_, rec_);
		interval range = interval(ray_t_.min, hit_left ? rec_.t : ray_t_.max);
		bool hit_right = right->hit(r_, range, rec_);
		range = interval(ray_t_.min, (hit_right || hit_left) ? rec_.t : ray_t_.max);
		bool hit_others = non_triangles_bvh->hit(r_, range, rec_);
		return hit_right || hit_left || hit_others;
	}
	else if (type == Node_Type::EMPTY)
	{
		return false;
	}
}

aabb bvh_triangles::bounding_box(std::string label_, bool& set_)
{
	set_ = false;
	aabb output  = aabb::empty;
	if (left)
	{
		bool leftSet = false;
		aabb leftBox = left->bounding_box(label_, leftSet);
		set_ = set_ || leftSet;
		if (leftSet)
			output = aabb(output, leftBox);
	}
	if (right)
	{
		bool rightSet = false;
		aabb rightBox = right->bounding_box(label_, rightSet);
		set_ = set_ || rightSet;
		if (rightSet)
			output = aabb(output, rightBox);
	}
	if (type == Node_Type::LEAF)
	{
		auto& lbls = triangles[triangle_indx].labels;
		if (lbls.find(label_) != lbls.end())
		{
			set_ = set_ || true;
			aabb myBox = bboxes[box_indx];
			output = aabb(output, myBox);
		}
	}
	return output;
}


void bvh_triangles::print_mat_indxes()
{
	if (type == Node_Type::ROOT || type == Node_Type::INTERNAL)
	{
		bvh_triangles* left_conv = dynamic_cast<bvh_triangles*>(left.get());
		bvh_triangles* right_conv = dynamic_cast<bvh_triangles*>(right.get());
		left_conv->print_mat_indxes();
		right_conv->print_mat_indxes();
	}
	else if (type == Node_Type::LEAF)
	{
		int mat_indx = triangles[triangle_indx].mat_indx;
		if (mat_indx > 2000)
		std::cout << triangle_indx << "-----" << mat_indx << std::endl;
	}
}


