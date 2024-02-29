#ifndef IMTEX_UTILS_HPP
#define IMTEX_UTILS_HPP
#include <type_traits>

#include "graphic/graphic_basic.h"
#include "nanovg.h"


// If U isnt a pointer, take by reference
template <typename T, typename U>
typename std::enable_if<!std::is_pointer<U>::value, const T&>::type castTo(const U& other) {
    return dynamic_cast<const T&>(other);
}


// If U is a pointer
template <typename T, typename U>
typename std::enable_if<std::is_pointer<U>::value, const T>::type castTo(U other) {
    return dynamic_cast<const T>(other);
}

NVGcolor toNvgColour(tex::color c) {
    uint8_t r = c & 0xFF000000;
    uint8_t g = c & 0x00FF0000;
    uint8_t b = c & 0x0000FF00;
    uint8_t a = c & 0x000000FF;

    return nvgRGBA(r, g, b, a);
}

#endif  // IMTEX_UTILS_HPP
