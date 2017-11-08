#include "../Common.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include "MakeIncludeTree.hpp"

int main(int argc, char* argv[])
{
    using namespace std;
    namespace wave = boost::wave;
    using namespace boost::property_tree;

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

    // Tree
    ptree incTree;

    // Prepare context
    typedef
        wave::context<
            std::string::const_iterator,
            wave::cpplexer::lex_iterator< wave::cpplexer::lex_token<> >,
            InputPolicy,
            MakeIncludeTreeHook> // Original hook
        Context;
    Context ctx(
        code.begin(),
        code.end(),
        argv[1],
        MakeIncludeTreeHook(incTree)); // Pass the tree

    // Language options
    ctx.set_language(
        wave::language_support(
            wave::support_cpp               |
            wave::support_option_long_long  |
            wave::support_option_variadics  |
            wave::support_option_include_guard_detection));

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
        // Parse source code
        Context::iterator_type itrEnd = ctx.end();
        for (Context::iterator_type itr = ctx.begin(); itr != itrEnd; ++itr)
        {
            ;
        }
    }
    catch (const wave::cpp_exception& ex)
    {
        // Exception has happened
        cerr << ex.file_name() << " : " << ex.line_no() << endl;
        cerr << "  -> " << ex.description() << endl;
        return 1;
    }

    // Output include tree
    info_parser::write_info(cout, incTree);

    return 0;
}
