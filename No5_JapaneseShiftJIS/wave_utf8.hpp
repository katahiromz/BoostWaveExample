#pragma once

#include <boost/wave/language_support.hpp>
#include <boost/wave/cpp_exceptions.hpp>
#include <utility>
#include <string>
#include <fstream>
#include <exception>

#if defined(_WIN32) && !defined(USE_ICONV)
    #include <windows.h>
#else
    #include <iconv.h>
#endif

namespace wave_utf8
{
    inline const char *get_wide_charset(void)
    {
        switch (sizeof(wchar_t))
        {
        case 2:
            return "UCS-2";
        case 4:
            return "UCS-4";
        default:
            assert(0);
            return NULL;
        }
    }

    // Convert narrow string to wide
    inline std::wstring narrow_to_wide(const std::string& ansi)
    {
        std::wstring ret;

#if defined(_WIN32) && !defined(USE_ICONV)
        int len = int(ansi.size());
        int cch = ::MultiByteToWideChar(CP_ACP, 0, ansi.c_str(), len, NULL, 0);
        if (cch == 0)
            return ret;

        ret.resize(cch);
        ::MultiByteToWideChar(CP_ACP, 0, ansi.c_str(), len, &ret[0], cch);
#else
        iconv_t ic = iconv_open(get_wide_charset(), "SHIFT_JIS");
        if ((iconv_t)-1 != ic)
        {
            size_t ansi_len = ansi.size();
            #ifdef ICONV_SECOND_ARGUMENT_IS_CONST
                const char *ansi_ptr  = ansi.c_str();
            #else
                char *ansi_ptr  = const_cast<char *>(ansi.c_str());
            #endif
            size_t wide_len = (ansi.size() + 1) * sizeof(wchar_t);
            char *wide_ptr  = (char *)malloc(wide_len);
            if (wide_ptr &&
                (size_t)-1 != iconv(ic, &ansi_ptr, &ansi_len, &wide_ptr, &wide_len))
            {
                ret.resize(wide_len / sizeof(wchar_t));
                memcpy(&ret[0], wide_ptr, wide_len);
            }
            free(wide_ptr);
            iconv_close(ic);
        }
#endif

        return ret;
    }

    // Convert wide string to narrow
    inline std::string wide_to_narrow(const std::wstring& wide)
    {
        std::string ret;

#if defined(_WIN32) && !defined(USE_ICONV)
        int len = int(wide.size());
        int cch = ::WideCharToMultiByte(CP_ACP, 0, wide.c_str(), len, NULL, 0, NULL, NULL);
        if (cch == 0)
            return ret;

        ret.resize(cch);
        ::WideCharToMultiByte(CP_ACP, 0, wide.c_str(), len, &ret[0], cch, NULL, NULL);
#else
        iconv_t ic = iconv_open("SHIFT_JIS", get_wide_charset());
        if ((iconv_t)-1 != ic)
        {
            size_t wide_len = wide.size();
            #ifdef ICONV_SECOND_ARGUMENT_IS_CONST
                const char *wide_ptr  = reinterpret_cast<const char *>(wide.c_str());
            #else
                char *wide_ptr  = reinterpret_cast<char *>(const_cast<wchar_t *>(wide.c_str()));
            #endif
            size_t ansi_len = (wide.size() + 1) * sizeof(wchar_t);
            char *ansi_ptr  = (char *)malloc(ansi_len);
            if (ansi_ptr &&
                (size_t)-1 != iconv(ic, &wide_ptr, &wide_len, &ansi_ptr, &ansi_len))
            {
                ret.resize(wide_len / sizeof(wchar_t));
                memcpy(&ret[0], wide_ptr, wide_len);
            }
            free(ansi_ptr);
            iconv_close(ic);
        }
#endif

        return ret;
    }

    // Convert UTF-8 string to wide
    inline std::wstring utf8_to_wide(const std::string& utf8)
    {
        std::wstring ret;

#if defined(_WIN32) && !defined(USE_ICONV)
        int len = int(utf8.size());
        int cch = ::MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), len, NULL, 0);
        if (cch == 0)
            return ret;

        ret.resize(cch);
        ::MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), len, &ret[0], cch);
#else
        iconv_t ic = iconv_open(get_wide_charset(), "UTF-8");
        if ((iconv_t)-1 != ic)
        {
            size_t utf8_len = utf8.size();
            #ifdef ICONV_SECOND_ARGUMENT_IS_CONST
                const char *utf8_ptr  = utf8.c_str();
            #else
                char *utf8_ptr  = const_cast<char *>(utf8.c_str());
            #endif
            size_t wide_len = (utf8.size() + 1) * sizeof(wchar_t);
            char *wide_ptr  = (char *)malloc(wide_len);
            if (wide_ptr &&
                (size_t)-1 != iconv(ic, &utf8_ptr, &utf8_len, &wide_ptr, &wide_len))
            {
                ret.resize(wide_len / sizeof(wchar_t));
                memcpy(&ret[0], wide_ptr, wide_len);
            }
            free(wide_ptr);
            iconv_close(ic);
        }
#endif
        return ret;
    }

    // Convert wide string to UTF-8
    inline std::string wide_to_utf8(const std::wstring& wide)
    {
        std::string ret;

#if defined(_WIN32) && !defined(USE_ICONV)
        int len = int(wide.size());
        int cch = ::WideCharToMultiByte(CP_UTF8, 0, wide.c_str(), len, NULL, 0, NULL, NULL);
        if (cch == 0)
            return ret;

        ret.resize(cch);
        ::WideCharToMultiByte(CP_UTF8, 0, wide.c_str(), len, &ret[0], cch, NULL, NULL);
#else
        iconv_t ic = iconv_open("UTF-8", get_wide_charset());
        if ((iconv_t)-1 != ic)
        {
            size_t wide_len = wide.size();
            #ifdef ICONV_SECOND_ARGUMENT_IS_CONST
                const char *wide_ptr  = reinterpret_cast<const char *>(wide.c_str());
            #else
                char *wide_ptr  = reinterpret_cast<char *>(const_cast<wchar_t *>(wide.c_str()));
            #endif
            size_t utf8_len = (wide.size() + 1) * 3;
            char *utf8_ptr  = (char *)malloc(utf8_len);
            if (utf8_ptr &&
                (size_t)-1 != iconv(ic, &wide_ptr, &wide_len, &utf8_ptr, &utf8_len))
            {
                ret.resize(utf8_len);
                memcpy(&ret[0], utf8_ptr, utf8_len);
            }
            free(utf8_ptr);
            iconv_close(ic);
        }
#endif

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
