#include <memory>
#include <iostream>

#include <catch.h>

#include <rttr/type>

using namespace rttr;
using namespace std;

TEST_CASE("Method list") {
	auto t = type::get_by_name("AppManager");
    std::cout << "Name: " << t.get_name() << std::endl;
	for (auto& meth : t.get_methods())
		std::cout << meth.get_name() << ", " << std::endl;
}

TEST_CASE("Method call") {
}
