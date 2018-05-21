#include <memory>
#include <iostream>

#include <AppManager.h>
#include <catch.h>

#include <rttr/type>

using namespace rttr;
using namespace std;

TEST_CASE("Method list") {
	auto t = type::get_by_name("test_class");
	for (auto& meth : t.get_methods())
		std::cout << meth.get_name() << ", ";
}

TEST_CASE("Method call") {
}
