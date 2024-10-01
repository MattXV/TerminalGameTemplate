#pragma once
#include <chrono>
#include <string>

// Rare case where I'd use using namespace 
using namespace std::chrono;


/*
Convenience Class that holds Deltatime info.
*/
class Time {
public:
    static void Init() { lastFrameTime = steady_clock::now(); }
    static void Update() {
        const steady_clock::time_point now = steady_clock::now();
        const duration<double> dt_ms = now - lastFrameTime;
        deltaTime = dt_ms.count();

    }
    static void SetLastFrameTime() { lastFrameTime = steady_clock::now(); }

    static double getDeltaTime() { return deltaTime; }
private:
    static steady_clock::time_point lastFrameTime;
    static double deltaTime;
};
// These guys live in static memory
double Time::deltaTime = 0;
steady_clock::time_point Time::lastFrameTime = steady_clock::now();



/**
 * Convenience string format utility function I did when I was bored and copy pasted here.
 * 
 *
 * @param format The string containing C format specifiers (see table below).
 * 
 * @param pars values associated with format specifiers
 * 
 * Example usage: formatString("A float: %f, Some integer: %i, Some string: %s", myFloat, myInt, myStr);
 * 
 * 
 * Format Table 
 
%d or %i:           It is used to print the signed integer value where signed integer means that the variable can hold both positive and negative values.
%u                  It is used to print the unsigned integer value where the unsigned integer means that the variable can hold only positive value.
%o                  It is used to print the octal unsigned integer where octal integer value always starts with a 0 value.
%x                  It is used to print the hexadecimal unsigned integer where the hexadecimal integer value always starts with a 0x value. In this, alphabetical characters are printed in small letters such as a, b, c, etc.
%X                  It is used to print the hexadecimal unsigned integer, but %X prints the alphabetical characters in uppercase such as A, B, C, etc.
%f                  It is used for printing the decimal floating-point values. By default, it prints the 6 values after '.'.
%e/%E               It is used for scientific notation. It is also known as Mantissa or Exponent.
%g                  It is used to print the decimal floating-point values, and it uses the fixed precision, i.e., the value after the decimal in input would be exactly the same as the value in the output.
%p                  It is used to print the address in a hexadecimal form.
%c                  It is used to print the unsigned character.
%s                  It is used to print the strings.
%ld                 It is used to print the long-signed integer value.

 */
template<typename... FormatParameters>
std::string formatString(const char* format, const FormatParameters&... pars) {
    int stringSize = snprintf(nullptr, 0, format, pars...) + 1;
    if (stringSize < 0) return std::string();

    const int stackThresh = 64;
    char stackAll[stackThresh];
    char* str = nullptr;
    if (stringSize < stackThresh) {
        str = &stackAll[0];
    }
    else {
        str = new char[stringSize];
    }

    snprintf(str, stringSize, format, pars...);
    std::string outString((const char*)str);
    if (!(stringSize < stackThresh)) delete[] str;

    return outString;
}
