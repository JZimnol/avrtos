#ifndef AVRTOS_UTILS_H_
#define AVRTOS_UTILS_H_

#include "avrtos_config.h"

/**
 * Concatenate two values into one.
 */
#define _CONCAT(Prefix, Suffix) Prefix##Suffix
#define AVRTOS_CONCAT(Prefix, Suffix) _CONCAT(Prefix, Suffix)

/**
 * Convert given value to string.
 */
#define _STRINGIFY(ToStr) #ToStr
#define AVRTOS_STRINGIFY(ToStr) _STRINGIFY(ToStr)

/**
 * Compile-time assertions.
 */
#ifdef AVRTOS_WITH_STATIC_ASSERTS
#define AVRTOS_STATIC_ASSERT(Condition, Message) \
    typedef char static_assertion_##Message[(Condition) ? 1 : -1]
#else // AVRTOS_WITH_STATIC_ASSERTS
#define AVRTOS_STATIC_ASSERT(...)
#endif // AVRTOS_WITH_STATIC_ASSERTS

/**
 * Group of "is-defined" macros. If @p MacroValue is defined to "1", evaluate to
 * @p TrueValue, else evaluate to @p FalseValue.
 */
#define _COMMA_ARGUMENT_1 _DUMMY_ARG,
#define _IS_DEFINED_2(Ignored, Val, ...) Val
#define _IS_DEFINED_1(TrueValue, FalseValue, DummyArg) \
    _IS_DEFINED_2(DummyArg TrueValue, FalseValue, dummy)
#define _IS_DEFINED(MacroValue, TrueValue, FalseValue) \
    _IS_DEFINED_1(TrueValue, FalseValue, _COMMA_ARGUMENT_##MacroValue)

#define AVRTOS_IS_DEFINED(MacroValue, TrueValue, FalseValue) \
    _IS_DEFINED(MacroValue, TrueValue, FalseValue)

/**
 * Helper macros for setting and clearing bits in registers.
 */
#define AVRTOS_SET_BIT_IN_REGISTER(Register, Bit) Register |= (1 << Bit)
#define AVRTOS_CLEAR_BIT_IN_REGISTER(Register, Bit) Register &= ~(1 << Bit)

/**
 * Group of "map" macros. Based on: https://github.com/swansontec/map-macro.
 */
#define _EVAL0(...) __VA_ARGS__
#define _EVAL1(...) _EVAL0(_EVAL0(_EVAL0(__VA_ARGS__)))
#define _EVAL2(...) _EVAL1(_EVAL1(_EVAL1(__VA_ARGS__)))
#define _EVAL3(...) _EVAL2(_EVAL2(_EVAL2(__VA_ARGS__)))
#define _EVAL4(...) _EVAL3(_EVAL3(_EVAL3(__VA_ARGS__)))
#define _EVAL(...) _EVAL4(_EVAL4(_EVAL4(__VA_ARGS__)))

#define _MAP_GET_END2() 0, _MAP_END
#define _MAP_GET_END1(...) _MAP_GET_END2
#define _MAP_GET_END(...) _MAP_GET_END1
#define _MAP_NEXT0(Test, Next, ...) Next _MAP_OUT
#define _MAP_NEXT1(Test, Next) _MAP_NEXT0(Test, Next, 0)
#define _MAP_NEXT(Test, Next) _MAP_NEXT1(_MAP_GET_END Test, Next)
#define _MAP0(Function, FunctionArg, Peek, ...) \
    Function(FunctionArg) _MAP_NEXT(Peek, _MAP1)(Function, Peek, __VA_ARGS__)
#define _MAP1(Function, FunctionArg, Peek, ...) \
    Function(FunctionArg) _MAP_NEXT(Peek, _MAP0)(Function, Peek, __VA_ARGS__)
#define _MAP_END(...)
#define _MAP_OUT

#define AVRTOS_MAP(Function, ...) \
    _EVAL(_MAP1(Function, __VA_ARGS__, ()()(), ()()(), ()()(), 0))

#endif /* AVRTOS_UTILS_H_ */
