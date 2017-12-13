#include "../Common.hpp"
#include "OutputHook.hpp"

int main(int argc, char **argv)
{
    using namespace std;
    namespace wave = boost::wave;

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
    {
        show_help();
        return 1;
    }

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
            BasicInputPolicy,
            OutputInfoHook> // Original hook
        Context;
    Context ctx(code.begin(), code.end(), argv[1], OutputInfoHook(std::cout));
    setup_context(ctx, argc, argv);

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

    return 0;
}
