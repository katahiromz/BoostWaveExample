#include <boost/wave.hpp>
#include <boost/wave/preprocessing_hooks.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>

#include <iostream>
#include <fstream>
#include <string>

#ifdef _WIN32
    #include <windows.h>
#endif

#define STRINGIFY1(x) #x
#define STRINGIFY2(x) STRINGIFY1(x)

namespace wave = boost::wave;

int main(int argc, char* argv[])
{
    using namespace std;

    if (argc < 2) { return 1; }

    // Load source
    std::string code;
    {
        std::ifstream fs(argv[1]);
        fs.unsetf(std::ios::skipws);
        code.assign(
            std::istreambuf_iterator<char>(fs.rdbuf()),
            std::istreambuf_iterator<char>());
    }

    // Prepare context
    typedef
        wave::context<
            std::string::const_iterator,
            wave::cpplexer::lex_iterator< wave::cpplexer::lex_token<> >,
            wave::iteration_context_policies::load_file_to_string,
            wave::context_policies::default_preprocessing_hooks>
        Context;
    Context ctx(code.begin(), code.end(), argv[1]);

    // Language options
    ctx.set_language(
        wave::language_support(
            wave::support_cpp               |
            wave::support_option_long_long  |
            wave::support_option_variadics));

#ifdef _WIN32
    ctx.add_macro_definition("_WIN32=1");
    ctx.add_macro_definition("_MSC_VER=" STRINGIFY2(_MSC_VER));
    const int MAX_ENV = 1024;
    char szInclude[MAX_ENV];
    if (GetEnvironmentVariableA("INCLUDE", szInclude, MAX_ENV))
    {
        ctx.add_sysinclude_path(szInclude);
    }
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
            cout << (*itr).get_value();
        }
    }
    catch (const wave::cpp_exception& ex)
    {
        // Exception has happened
        cerr << ex.file_name() << " : " << ex.line_no() << endl;
        cerr << "  -> " << ex.description() << endl;
        return 1;
    }

    return 0;
}
