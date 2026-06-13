#pragma once
#include "bvh.h"
#include "triangle_list.h"
#include <memory>
#include <vector>

enum class Node_Type {
	ROOT,
	INTERNAL,
	LEAF,
	EMPTY
};

class bvh_triangles : public bvh_node
{
public:

	// constructor for the shared variables 
	// this will use other c'tors to build the right
	// and left nodes
	bvh_triangles(
		std::unique_ptr<triangle_list> list_,
		BVH_Split_Method split_method_ = BVH_Split_Method::MEDIAN);
	~bvh_triangles();


	void set_left_right(
		size_t start_, size_t end_,
		const int indx,
		const int level_ = 0,
		BVH_Split_Method split_method_ = BVH_Split_Method::MEDIAN);


	bool hit(const ray& r, interval ray_t, hit_record& rec) const override;

	virtual aabb bounding_box(std::string label_, bool& set_) override;



protected:
	// I do not want these c'tors to be accessible from outside this class
	// since these are used by the main c'tor to create internal and leaf node
	
	// default empty c'tor for empty leaves
	bvh_triangles();
	// c'tor for internal nodes
	bvh_triangles(
		size_t start, size_t end,
		const int indx_,
		const int level_,
		BVH_Split_Method split_method_ = BVH_Split_Method::MEDIAN);
	// c'tor for leaves
	bvh_triangles(
		const size_t& box_indx_,
		const size_t& triangle_indx_,
		const aabb& bbox_,
		Node_Type type = Node_Type::LEAF);


	// indexes 
	// index in my own bboxes vector
	size_t box_indx;
	// index from the triangles_struct vector
	size_t triangle_indx = -1;
	// shared vectors
	inline static std::unique_ptr<triangle_list> list;
	// my bbox not the triangle_list bbox!
	inline static std::vector<aabb> bboxes;
	inline static std::vector<triangle_struct> triangles;


	// left and right nodes
	//std::unique_ptr<bvh_triangles> left_bvh, right_bvh;

	// the normal bvh_node for non_triangles objects
	std::unique_ptr<hittable> non_triangles_bvh;

	// flag to check if it is the main node
	Node_Type type = Node_Type::EMPTY;

	// just a tester helper;
	void print_mat_indxes();

};
