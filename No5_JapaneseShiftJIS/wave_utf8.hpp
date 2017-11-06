#pragma once

#include <boost/wave/language_support.hpp>
#include <boost/wave/cpp_exceptions.hpp>
#include <utility>
#include <string>
#include <fstream>
#include <exception>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <iconv.h>
#endif

namespace wave_utf8
{
    // Convert narrow string to wide
    inline std::wstring narrow_to_wide(const std::string& ansi)
    {
        std::wstring ret;

        int len = int(ansi.size());
        int cch = ::MultiByteToWideChar(CP_ACP, 0, ansi.c_str(), len, NULL, 0);
        if (cch == 0)
            return ret;

        ret.resize(cch);
        ::MultiByteToWideChar(CP_ACP, 0, ansi.c_str(), len, &ret[0], cch);
        return ret;
    }

    // Convert wide string to narrow
    inline std::string wide_to_narrow(const std::wstring& wide)
    {
        std::string ret;

        int len = int(wide.size());
        int cch = ::WideCharToMultiByte(CP_ACP, 0, wide.c_str(), len, NULL, 0, NULL, NULL);
        if (cch == 0)
            return ret;

        ret.resize(cch);
        ::WideCharToMultiByte(CP_ACP, 0, wide.c_str(), len, &ret[0], cch, NULL, NULL);
        return ret;
    }

    // Convert UTF-8 string to wide
    inline std::wstring utf8_to_wide(const std::string& utf8)
    {
        std::wstring ret;

        int len = int(utf8.size());
        int cch = ::MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), len, NULL, 0);
        if (cch == 0)
            return ret;

        ret.resize(cch);
        ::MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), len, &ret[0], cch);
        return ret;
    }

    // Convert wide string to UTF-8
    inline std::string wide_to_utf8(const std::wstring& wide)
    {
        std::string ret;

        int len = int(wide.size());
        int cch = ::WideCharToMultiByte(CP_UTF8, 0, wide.c_str(), len, NULL, 0, NULL, NULL);
        if (cch == 0)
            return ret;

        ret.resize(cch);
        ::WideCharToMultiByte(CP_UTF8, 0, wide.c_str(), len, &ret[0], cch, NULL, NULL);
        return ret;
    }

    inline std::string utf8_to_narrow(const std::string& utf8)
    {
        std::wstring wide = utf8_to_wide(utf8);
        return wide_to_narrow(wide);
    }
    inline std::string narrow_to_utf8(const std::string& ansi)
    {
        std::wstring wide = narrow_to_wide(ansi);
        return wide_to_utf8(wide);
    }

    // Read narrow text file as UTF-8
    std::string readFileToUtf8(const char* filePath)
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

        // Convert to UTF-8 string
        return narrow_to_utf8(text);
    }


    // Input policy to read narrow text file as UTF-8.
    // Based on boost::wave::iteration_context_policies::load_file_to_string.
    class Utf8InputPolicy
    {
    public:
        template<typename IterContextT>
        class inner
        {
        public:
            template<typename PositionT>
            static void init_iterators(
                IterContextT& iterContext,
                const PositionT& pos,
                boost::wave::language_support language)
            {
                try
                {
                    iterContext.code = readFileToUtf8(iterContext.filename.c_str());
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
}