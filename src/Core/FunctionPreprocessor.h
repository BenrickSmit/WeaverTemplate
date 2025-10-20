/**
 * @file FunctionPreprocessor.h
 * @author B.G. Smit
 * @brief Defines a cross-compiler macro for retrieving the current function's name.
 *
 * This utility header provides the `CALLING_FUNCTION_NAME` macro, which adapts
 * to different compilers (MSVC, Clang, GCC) to consistently capture the name
 * of the function from which it is invoked. This is primarily used for logging and debugging
 * purposes.
 * @copyright Copyright (c) 2025
 */
#ifndef FUNCTION_PREPROCESSOR_H
#define FUNCTION_PREPROCESSOR_H

// Detect compiler and define a consistent macro for function name
#if defined(_MSC_VER) && !defined(__clang__)
// Microsoft Visual C++: __FUNCTION__ is available
#define CALLING_FUNCTION_NAME __FUNCTION__
#elif defined(__clang__)
// Clang: supports both __func__ and __FUNCTION__, but __func__ is standard
#define CALLING_FUNCTION_NAME __func__
#elif defined(__GNUC__)
// GCC: __func__ is standard (C++11), available since GCC 2.95
#define CALLING_FUNCTION_NAME __func__
#else
// Fallback for any other compiler
#define CALLING_FUNCTION_NAME __func__
#endif

#endif  // FUNCTION_PREPROCESSOR_H