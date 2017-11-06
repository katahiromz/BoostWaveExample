// boost::wave 関連ヘッダをインクルード
#include <boost/wave.hpp>
#include <boost/wave/preprocessing_hooks.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>

#include "wave_utf8.hpp"
#include <iostream>
#include <string>

namespace wave = boost::wave;

int main(int argc, char* argv[])
{
    using namespace std;

    if (argc < 2) { return 1; }

    // Load source and convert it to UTF-8
    const std::string code = wave_utf8::readFileToUtf8(argv[1]);

    // Prepare for context
    typedef
        wave::context<
            std::string::const_iterator,
            wave::cpplexer::lex_iterator< wave::cpplexer::lex_token<> >,
            wave_utf8::Utf8InputPolicy,
            wave::context_policies::default_preprocessing_hooks>
        Context;
    Context ctx(code.begin(), code.end(), argv[1]);

    // Language options
    ctx.set_language(
        wave::language_support(
            wave::support_cpp               |   // C++として処理
            wave::support_option_long_long  |   // long long 型サポート
            wave::support_option_variadics));   // 可変長引数マクロサポート

    ctx.add_macro_definition("_WIN32");
    ctx.add_macro_definition("_MSC_VER=1800");
    ctx.add_sysinclude_path(
        "C:/Program Files/Microsoft Visual Studio 12.0/VC/include");

    try
    {
        // Parse and output tokens
        Context::iterator_type itrEnd = ctx.end();
        for (Context::iterator_type itr = ctx.begin(); itr != itrEnd; ++itr)
        {
            // Convert UTF-8 to narrow
            auto value = (*itr).get_value();
            cout << wave_utf8::utf8_to_narrow(value.c_str());
        }
    }
    catch (const wave::cpp_exception& ex)
    {
        // Exception has happened
        cerr << ex.file_name() << " : " << ex.line_no() << endl;
        cerr << " -> " << ex.description() << endl;
        return 1;
    }

    return 0;
}
