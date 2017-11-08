#include "../Common.hpp"
#include "JapaneseEncoding.hpp"

int main(int argc, char* argv[])
{
    using namespace std;
    namespace wave = boost::wave;

    if (argc < 2) { return 1; }

    // Load source and convert it to UTF-8
    const std::string code = JapaneseEncoding::readFileToUtf8(argv[1]);

    // Prepare context
    typedef
        wave::context<
            std::string::const_iterator,
            wave::cpplexer::lex_iterator< wave::cpplexer::lex_token<> >,
            JapaneseEncoding::Utf8InputPolicy,
            wave::context_policies::default_preprocessing_hooks>
        Context;
    Context ctx(code.begin(), code.end(), argv[1]);

    // Language options
    ctx.set_language(
        wave::language_support(
            wave::support_cpp               |   // C++として処理
            wave::support_option_long_long  |   // long long 型サポート
            wave::support_option_variadics));   // 可変長引数マクロサポート

    // Setup preprocessor
#ifdef _WIN32
    const int MAX_ENV = 512;
    ctx.add_macro_definition("_WIN32=1");
    #ifdef _MSC_VER
        ctx.add_macro_definition("_MSC_VER=" STRINGIFY2(_MSC_VER));
        char szInclude[MAX_ENV];
        if (GetEnvironmentVariableA("INCLUDE", szInclude, MAX_ENV))
        {
            ctx.add_sysinclude_path(szInclude);
        }
    #elif defined(__MINGW32__) || defined(__clang__)
        ctx.add_macro_definition("__GNUC__");
        char szInclude[MAX_ENV], szHost[MAX_ENV];
        if (GetEnvironmentVariableA("MINGW_PREFIX", szInclude, MAX_ENV))
        {
            lstrcatA(szInclude, "/include");
            ctx.add_sysinclude_path(szInclude);
            std::cout << szInclude << std::endl;
        }
        if (GetEnvironmentVariableA("MINGW_PREFIX", szInclude, MAX_ENV) &&
            GetEnvironmentVariableA("MINGW_CHOST", szHost, MAX_ENV))
        {
            lstrcatA(szInclude, "/");
            lstrcatA(szInclude, szHost);
            lstrcatA(szInclude, "/include");
            ctx.add_sysinclude_path(szInclude);
            std::cout << szInclude << std::endl;
        }
    #endif
    else
#endif
    {
        ctx.add_sysinclude_path("/usr/include");
    }

    try
    {
        // Parse and output tokens
        Context::iterator_type itrEnd = ctx.end();
        for (Context::iterator_type itr = ctx.begin(); itr != itrEnd; ++itr)
        {
            // Convert UTF-8 to narrow
            auto value = (*itr).get_value();
            cout << JapaneseEncoding::utf8_to_narrow(value.c_str());
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
