#include "../Common.hpp"

int main(int argc, char **argv)
{
    namespace wave = boost::wave;
    using namespace std;

    if (argc < 2)
    {
        cout <<
            "cpp [options] input-file.h\n"
            "Options:\n"
            "-Dmacro\n"
            "-Dmacro=def    Defines a macro\n"
            "-Umacro        Undefines a macro\n";
        return 1;
    }

    // Load source
    std::string code;
    {
        std::ifstream fs(argv[argc - 1]);
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
            wave::context_policies::default_preprocessing_hooks>
        Context;
    Context ctx(code.begin(), code.end(), argv[argc - 1]);
    setup_context(ctx, argc, argv);

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
