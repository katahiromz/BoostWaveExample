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
    setup_context(ctx);

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
