#ifndef BOOST_WAVE_EXAMPLE_COMMON_HPP
#define BOOST_WAVE_EXAMPLE_COMMON_HPP

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

#define STRINGIFY(x) #x
#define XSTRINGIFY(x) STRINGIFY(x)

class BasicInputPolicy
{
public:
    template <typename IterContextT>
    class inner
    {
    public:
        static std::string readFile(const char* filePath)
        {
            // Open file
            std::ifstream fs(filePath);
            if (!fs)
            {
                std::string msg = "Cannot open file '";
                msg += (filePath == nullptr) ? "(nullptr)" : filePath;
                msg += "'.";
                throw std::runtime_error(msg.c_str());
            }

            // Read
            fs.unsetf(std::ios::skipws);
            std::string text(
                std::istreambuf_iterator<char>(fs.rdbuf()),
                std::istreambuf_iterator<char>());

            return text;
        }

        template<typename PositionT>
        static void init_iterators(
            IterContextT& iterContext,
            const PositionT& pos,
            boost::wave::language_support language)
        {
            try
            {
                iterContext.code = readFile(iterContext.filename.c_str());
                iterContext.code += "\n";
            }
            catch (const std::exception&)
            {
                BOOST_WAVE_THROW_CTX(
                    iterContext.ctx,
                    boost::wave::preprocess_exception,
                    bad_include_file,
                    iterContext.filename.c_str(),
                    pos);
                return;
            }

            typedef typename IterContextT::iterator_type iterator_type;
            iterContext.first =
                iterator_type(
                    iterContext.code.begin(),
                    iterContext.code.end(),
                    PositionT(iterContext.filename),
                    language);
            iterContext.last = iterator_type();
        }

    protected:
        std::string code;
    };
};

inline void show_help(void)
{
    std::cout <<
        "cpp [options] input-file.h\n"
        "Options:\n"
        "  -Dmacro        Defines a macro\n"
        "  -Dmacro=def    Defines a macro\n"
        "  -Umacro        Undefines a macro\n"
        "  -Ipath         Add include path\n"
        "  -Spath         Add system include path" << std::endl;
}

template <typename T_CONTEXT>
inline void setup_context(T_CONTEXT& ctx, int argc, char **argv)
{
    using namespace boost;

    // Language options
    ctx.set_language(
        wave::language_support(
            wave::support_cpp               |
            wave::support_option_long_long  |
            wave::support_option_variadics));

    #include "predefined.h"

    for (int i = 1; i < argc; ++i)
    {
        if (argv[i][0] == '-' && argv[i][1])
        {
            std::string str = &(argv[i][2]);
            switch (argv[i][1])
            {
            case 'D':
                ctx.add_macro_definition(str);
                break;
            case 'U':
                ctx.remove_macro_definition(str);
                break;
            case 'I':
                ctx.add_include_path(str.c_str());
                break;
            case 'S':
                ctx.add_sysinclude_path(str.c_str());
                break;
            }
        }
    }

#ifdef _WIN32
    const int MAX_ENV = 512;
    #if defined(_MSC_VER) || defined(__BORLANDC__)
        char szInclude[MAX_ENV];
        if (GetEnvironmentVariableA("INCLUDE", szInclude, MAX_ENV))
        {
            ctx.add_sysinclude_path(szInclude);
        }
    #elif defined(__MINGW32__) || defined(__CYGWIN__) || defined(__clang__)
        char szInclude[MAX_ENV], szHost[MAX_ENV];
        if (GetEnvironmentVariableA("MINGW_PREFIX", szInclude, MAX_ENV))
        {
            strcat(szInclude, "/include");
            ctx.add_sysinclude_path(szInclude);
        }
        if (GetEnvironmentVariableA("MINGW_PREFIX", szInclude, MAX_ENV) &&
            GetEnvironmentVariableA("MINGW_CHOST", szHost, MAX_ENV))
        {
            strcat(szInclude, "/");
            strcat(szInclude, szHost);
            strcat(szInclude, "/include");
            ctx.add_sysinclude_path(szInclude);
        }
    #endif
    else
#endif
    {
        ctx.add_sysinclude_path("/usr/include");
    }
}

#endif  // ndef BOOST_WAVE_EXAMPLE_COMMON_HPP
