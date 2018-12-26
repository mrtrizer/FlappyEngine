#pragma once

#include <sstream>
#include <string>
#include <cassert>

namespace flappy {

class FlappyException : public std::exception {
public:
    FlappyException(std::string message) noexcept : m_message(std::move(message))  {
        // TODO: Stack trace
    }

    const char* what() const noexcept override {
        return m_message.c_str();
    }

private:
    std::string m_message;
};

// Use USER_ASSERT assert in public methods or methods, called from public methods.
// It throws an exception for wrong statement. Use when case is exceptional but execution can be continued.
#define USER_ASSERT(statement) { if (!(statement)) { throw FlappyException(sstr(__FILE__, ' ', __LINE__, ' ', #statement)); } }

#define USER_ASSERT_MSG(statement, ...) { if (!(statement)) { throw FlappyException(sstr(__FILE__, ' ', __LINE__, ' ', #statement, ' ', __VA_ARGS__)); } }

// Use DEBUG_ASSERT only for next applications:
// 1. Check class has valid state.
// 2. Error can't be handled. Like memory is broken.
// Notice that DEBUG_ASSERT is turned off in production.
#ifdef NDEBUG
    #define DEBUG_ASSERT (void)
#else
// FIXME: Use std::terminate and custom output
    #define DEBUG_ASSERT(statement) assert(statement && __FILE__ && __LINE__)
    //#define DEBUG_ASSERT (void)
#endif

#ifdef NDEBUG
    #define FORDEBUG(expression)
#else
    #define FORDEBUG(expression) expression
#endif

template <typename... Args>
std::string sstr(Args &&... args) noexcept
{
    std::ostringstream sstr;
    (sstr << ... << args);
    return sstr.str();
}

template <typename BaseT, typename DerivedT>
constexpr void assertDerived() {
    static_assert(std::is_base_of<BaseT, DerivedT>::value, "DerivedT should be derived from BaseT");
}

} // flappy
