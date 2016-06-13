#include <cmath>
#include <functional>

#include "gtools.h"

/// Common tools for model
namespace GTools {
using namespace std;

//http://stackoverflow.com/questions/108318/whats-the-simplest-way-to-test-whether-a-number-is-a-power-of-2-in-c
bool isPowOfTwo(int x) {
    return !(x == 0) && !(x & (x - 1));
}

}
