
#include "catch_amalgamated.hpp"

class intMatMatcher : public Catch::Matchers::MatcherGenericBase
{
public:
	intMatMatcher(int** expected_, int size_x_, int size_y_) :
		expected{ expected_ }, size_x{ size_x_ }, size_y{ size_y_ } {
	}
	bool match(int** value_) const
	{
		for (int i = 0; i < size_x; i++)
			for (int j = 0; j < size_y; j++)
				if (value_[i][j] != expected[i][j])
					return false;
		return true;
	}
	std::string describe() const override
	{
		std::string output = "A custom 2D array matcher";
		return output;
	}
private:
	int size_x, size_y;
	int** expected;
};

class vec3Matcher : public Catch::Matchers::MatcherGenericBase {
public:
	vec3Matcher(const vec3& expected_, double tol_) :
		tol{ tol_ },
		expectedRef{ std::ref(expected_) }
	{
	}

	bool match(const vec3& value_) const
	{
		return equal(expectedRef.get(), value_, tol);
	}

	std::string describe() const override
	{
		std::string message = "Comparing two vec3 with the tolerance of " + std::to_string(tol);
		return message;
	}

private:
	double tol;
	std::reference_wrapper<const vec3> expectedRef;
};

class colorArrayMatcher : public Catch::Matchers::MatcherGenericBase {
public:
	colorArrayMatcher(
		color_array* expected_,
		double tol_,
		std::vector<std::array<int, 2>>& ignoredCoors_
	) :
		tol{ tol_ },
		expected{ expected_ },
		ignoredCoors{ std::move(ignoredCoors_) }
	{
	}

	colorArrayMatcher(
		color_array* expected_,
		double tol_
	) :
		tol{ tol_ },
		expected{ expected_ }
	{
		ignoredCoors = std::vector<std::array<int, 2>>{};
	}

	bool match(color_array* value_) const
	{
		return expected->equal(value_, tol, ignoredCoors);
	}

	std::string describe() const override
	{
		std::string message = "Comparing two color_arrays with the tolerance of " + std::to_string(tol);
		return message;
	}

private:
	double tol;
	color_array* expected;
	std::vector<std::array<int, 2>> ignoredCoors;
};

class hittableListMatcher : public Catch::Matchers::MatcherGenericBase {
public:
	hittableListMatcher(hittable_list* expected_, double tol_) :
		expected{ expected_ }, tol{ tol_ }
	{
		if (expected_ == nullptr)
			throw std::runtime_error("The expected in the hittableListMatcher is nullptr!");
	}

	bool match(hittable_list* value_) const
	{
		if (value_ == nullptr)
			throw std::runtime_error("The value in the hittableListMatcher is nullptr!");


		int size = static_cast<int>(value_->size());
		int expectedsize = static_cast<int>(expected->size());

		if (size != expectedsize) {
			std::cout << "Hittable_list size mismatch" << std::endl;
			return false;
		}

		for (int i = 0; i < size; i++)
		{
			hittable* objectVal = (*value_)[i];
			hittable* objectExpected = (*expected)[i];

			material* matVal = objectVal->get_material();
			material* matExpected = objectExpected->get_material();

			if (objectVal->compare(objectExpected, tol)) {
				std::cout << "Object mismatch" << std::endl;
				return false;
			}

			if (matVal == nullptr)
				std::cout << "The original material pointer is nullptr" << std::endl;
			if (matExpected == nullptr)
				std::cout << "The expected material pointer is nullptr" << std::endl;

			if (matVal->compare(matExpected, tol)) {
				std::cout << "Material mismatch" << std::endl;
				return false;
			}

		}
		return true;
	}

	std::string describe() const override
	{
		std::string message = "Comparing two hittable_list with tolerance of " + std::to_string(tol);
		return message;
	}

protected:
	hittable_list* expected;
	double tol;
};


class point3Matcher : public Catch::Matchers::MatcherGenericBase {
public:
	point3Matcher(const point3& expected_, double tol_) :
		tol{ tol_ },
		expectedRef{ std::ref(expected_) }
	{
	}
	bool match(const point3& value_) const
	{
		if (std::abs(expectedRef.get().x() - value_.x() >= tol))
			return false;
		else if (std::abs(expectedRef.get().y() - value_.y() >= tol))
			return false;
		else if (std::abs(expectedRef.get().z() - value_.z() >= tol))
			return false;
		return true;
	}
	std::string describe() const override
	{
		std::string message = "Comparing two point3 with the tolerance of " + std::to_string(tol);
		return message;
	}

private:
	std::reference_wrapper<const point3> expectedRef;
	double tol;
};

class StringStreamMatcher : public Catch::Matchers::MatcherGenericBase {
public:
	StringStreamMatcher(std::vector<std::string>* expectedData_) :
		expectedData{ expectedData_ }
	{
	}
	bool match(const std::stringstream* value_) const
	{
		std::string outputString = value_->str();
		std::istringstream iss(outputString);
		std::string stringI;
		std::vector<std::string>& expectedRef =
			*expectedData;
		int nExpected = static_cast<int>(expectedRef.size());
		int i = 0;
		while (getline(iss, stringI))
		{
			// expectedRef bounds check
			if (i >= nExpected)
				return false;
			// comparing the stringI with data i and incrementing the i
			if (stringI.compare(expectedRef[i++]))
				return false;
		}
		return true;
	}
	std::string describe() const override
	{
		std::string message = "Comparing a std::stringstream contents with a pointer of vector of strings";
		return message;
	}

private:
	std::vector<std::string>* expectedData;

};


class P6Matcher : public Catch::Matchers::MatcherGenericBase {
public:
	P6Matcher(std::string* expectedData_,
		const int expectedWidth_, 
		const int expectedHeight_,
		const int expectedNColors_,
		const int rank_) :
		expectedData{ expectedData_ },
		expectedWidth{expectedWidth_},
		expectedHeight{expectedHeight_},
		expectedNColors{expectedNColors_},
		rank{rank_}
	{}
	bool match(std::stringstream* value_) const
	{
		if (false) {
			// checking the format
			std::string format;
			*value_ >> format;
			REQUIRE(format == expectedFormat);

			// checking the size
			int width, height;
			*value_ >> width;
			*value_ >> height;
			if (width != expectedWidth)
				return false;
			if (height != expectedHeight)
				return false;

			// checking the number of colors
			int nColors;
			*value_ >> nColors;
			if (nColors != expectedNColors)
				return false;
		}

		// checking the data
		// getting the full text
		std::string text = value_->str();
		// current pos
		std::streampos pos = value_->tellg();
		// the rest of the data
		std::string rest = text; // text.substr(pos);

		// comparing the data
		if (expectedData->compare(rest))
			return false;

		return true;
	}
	std::string describe() const override
	{
		std::string message = "Checking a P6 file contents";
		return message;
	}

private:
	const std::string expectedFormat = "P6";
	const int expectedWidth, expectedHeight, expectedNColors;
	const int rank;
	std::string* expectedData;
};