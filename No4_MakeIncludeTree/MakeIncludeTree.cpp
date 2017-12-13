#include "../Common.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include "MakeIncludeTree.hpp"

int main(int argc, char **argv)
{
    using namespace std;
    namespace wave = boost::wave;
    using namespace boost::property_tree;

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
            BasicInputPolicy,
            MakeIncludeTreeHook> // Original hook
        Context;
    Context ctx(
        code.begin(),
        code.end(),
        argv[1],
        MakeIncludeTreeHook(incTree)); // Pass the tree
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

    // Output include tree
    info_parser::write_info(cout, incTree);

    return 0;
}
