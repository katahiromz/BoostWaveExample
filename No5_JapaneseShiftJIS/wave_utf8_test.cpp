#include "wave_utf8.hpp"

int main(void)
{
    using namespace wave_utf8;
    assert(narrow_to_wide("\x82\xA0\x82\xA2\x82\xA4\x82\xA6\x82\xA8") == L"\x3042\x3044\x3046\x3048\x304A");
    assert(wide_to_narrow(L"\x3042\x3044\x3046\x3048\x304A") == "\x82\xA0\x82\xA2\x82\xA4\x82\xA6\x82\xA8");
}
