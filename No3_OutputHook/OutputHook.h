#pragma once

#include "waveIosDef.h"

// boost::wave 関連ヘッダをインクルード
#include <boost/wave.hpp>
#include <boost/wave/preprocessing_hooks.hpp>

#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/container/generation/make_vector.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/sequence/io/out.hpp>
#include <boost/io/ios_state.hpp>
#include <boost/ref.hpp>
#include <algorithm>
#include <iostream>

// 関数名マクロ __func__
#if !defined(__func__)
#if defined(__FUNCTION__)
#define __func__ __FUNCTION__
#elif defined(__FUNCSIG__)
#define __func__ __FUNCSIG__
#else
#define __func__ "__func__"
#endif
#endif // !defined(__func__)

/// 捕捉情報を出力するフッククラス
class OutputInfoHook
    :
    public boost::wave::context_policies::default_preprocessing_hooks
{
private:
    /// ベースクラス型
    typedef boost::wave::context_policies::default_preprocessing_hooks Base;

public:
    /// コンストラクタ
    OutputInfoHook(std::ostream& out) : _out(out)
    {
    }

private:
    /// 関数呼び出し情報出力
    template<typename TArgs>
    void outputFuncInfo(const char* funcName, const TArgs& args)
    {
        std::ostream& o = _out;
        boost::io::ios_all_saver iosSaver(o);

        o << funcName
          << boost::fusion::tuple_open("(\n    ")
          << boost::fusion::tuple_delimiter(",\n    ")
          << boost::fusion::tuple_close(");")
          << args
          << std::endl;
    }

public:
    /// ディレクティブ発見時の捕捉関数
    template<
        typename ContextT,
        typename TokenT>
    bool found_directive(
        ContextT const& ctx,
        TokenT const& directive)
    {
        outputFuncInfo(
            __func__,
            boost::fusion::make_vector(
                boost::cref(ctx),
                boost::cref(directive)));

        return Base::found_directive(ctx, directive);
    }

    /// #include ディレクティブ発見時の捕捉関数
    template<typename ContextT>
    bool found_include_directive(
        ContextT const& ctx,
        std::string const& filename, 
        bool include_next)
    {
        outputFuncInfo(
            __func__,
            boost::fusion::make_vector(
                boost::cref(ctx),
                boost::cref(filename),
                boost::cref(include_next)));

        return Base::found_include_directive(ctx, filename, include_next);
    }

    /// #warning ディレクティブ発見時の捕捉関数
    template<
        typename ContextT,
        typename ContainerT>
    bool found_warning_directive(
        ContextT const& ctx,
        ContainerT const& message)
    {
        outputFuncInfo(
            __func__,
            boost::fusion::make_vector(
                boost::cref(ctx),
                boost::cref(message)));

        return Base::found_warning_directive(ctx, message);
    }

    /// #error ディレクティブ発見時の捕捉関数
    template<
        typename ContextT,
        typename ContainerT>
    bool found_error_directive(
        ContextT const& ctx,
        ContainerT const& message)
    {
        outputFuncInfo(
            __func__,
            boost::fusion::make_vector(
                boost::cref(ctx),
                boost::cref(message)));

        return Base::found_error_directive(ctx, message);
    }

    /// #line ディレクティブ発見時の捕捉関数
    template<
        typename ContextT,
        typename ContainerT>
    void found_line_directive(
        ContextT const& ctx,
        ContainerT const& arguments,
        unsigned int line,
        std::string const& filename)
    {
        outputFuncInfo(
            __func__,
            boost::fusion::make_vector(
                boost::cref(ctx),
                boost::cref(arguments),
                boost::cref(line),
                boost::cref(filename)));

        Base::found_line_directive(ctx, arguments, line, filename);
    }

    /// 条件分岐ディレクティブ展開時の捕捉関数
    template<
        typename ContextT,
        typename TokenT,
        typename ContainerT>
    bool evaluated_conditional_expression(
        ContextT const& ctx, 
        TokenT const& directive,
        ContainerT const& expression, 
        bool expression_value)
    {
        outputFuncInfo(
            __func__,
            boost::fusion::make_vector(
                boost::cref(ctx),
                boost::cref(directive),
                boost::cref(expression),
                boost::cref(expression_value)));

        return Base::evaluated_conditional_expression(
            ctx,
            directive,
            expression,
            expression_value);
    }

    /// トークンスキップ時の捕捉関数
    template<
        typename ContextT,
        typename TokenT>
    void skipped_token(
        ContextT const& ctx,
        TokenT const& token)
    {
        outputFuncInfo(
            __func__,
            boost::fusion::make_vector(
                boost::cref(ctx),
                boost::cref(token)));

        Base::skipped_token(ctx, token);
    }

    /// トークン生成時の捕捉関数
    template<
        typename ContextT,
        typename TokenT>
    TokenT const& generated_token(
        ContextT const& ctx,
        TokenT const& token)
    {
        outputFuncInfo(
            __func__,
            boost::fusion::make_vector(
                boost::cref(ctx),
                boost::cref(token)));

        return Base::generated_token(ctx, token);
    }

    /// マクロ定義時の捕捉関数
    template<
        typename ContextT,
        typename TokenT,
        typename ParametersT, 
        typename DefinitionT>
    void defined_macro(
        ContextT const& ctx,
        TokenT const& macro_name, 
        bool is_functionlike,
        ParametersT const& parameters, 
        DefinitionT const& definition,
        bool is_predefined)
    {
        outputFuncInfo(
            __func__,
            boost::fusion::make_vector(
                boost::cref(ctx),
                boost::cref(macro_name),
                boost::cref(is_functionlike),
                boost::cref(parameters),
                boost::cref(definition),
                boost::cref(is_predefined)));

        Base::defined_macro(
            ctx,
            macro_name,
            is_functionlike,
            parameters,
            definition,
            is_predefined);
    }

    /// マクロ削除時の捕捉関数
    template<
        typename ContextT,
        typename TokenT>
    void undefined_macro(
        ContextT const& ctx,
        TokenT const& macro_name)
    {
        outputFuncInfo(
            __func__,
            boost::fusion::make_vector(
                boost::cref(ctx),
                boost::cref(macro_name)));

        Base::undefined_macro(ctx, macro_name);
    }

    /// 文字列置換マクロ展開時の捕捉関数
    template<
        typename ContextT,
        typename TokenT,
        typename ContainerT>
    bool expanding_object_like_macro(
        ContextT const& ctx,
        TokenT const& macro_name, 
        ContainerT const& definition,
        TokenT const& macrocall)
    {
        outputFuncInfo(
            __func__,
            boost::fusion::make_vector(
                boost::cref(ctx),
                boost::cref(macro_name),
                boost::cref(definition),
                boost::cref(macrocall)));

        return Base::expanding_object_like_macro(
            ctx,
            macro_name,
            definition,
            macrocall);
    }

    /// 引数リスト付きマクロ展開時の捕捉関数
    template<
        typename ContextT,
        typename TokenT,
        typename ContainerT,
        typename IteratorT>
    bool expanding_function_like_macro(
        ContextT const& ctx,
        TokenT const& macro_name,
        std::vector<TokenT> const& parameters, 
        ContainerT const& definition,
        TokenT const& macrocall,
        std::vector<ContainerT> const& arguments,
        IteratorT const& seqstart,
        IteratorT const& seqend)
    {
        IteratorT last = seqend;
        std::advance(last, 1);
        std::list<TokenT> seq(seqstart, last);

        outputFuncInfo(
            __func__,
            boost::fusion::make_vector(
                boost::cref(ctx),
                boost::cref(macro_name),
                boost::cref(parameters),
                boost::cref(definition),
                boost::cref(macrocall),
                boost::cref(arguments),
                boost::cref(seq)));

        return Base::expanding_function_like_macro(
            ctx,
            macro_name,
            parameters,
            definition,
            macrocall,
            arguments,
            seqstart,
            seqend);
    }

    /// マクロ展開完了時の捕捉関数
    template<
        typename ContextT,
        typename ContainerT>
    void expanded_macro(
        ContextT const& ctx,
        ContainerT const& result)
    {
        outputFuncInfo(
            __func__,
            boost::fusion::make_vector(
                boost::cref(ctx),
                boost::cref(result)));

        Base::expanded_macro(ctx, result);
    }

    /// マクロ展開完全完了時の捕捉関数
    template<
        typename ContextT,
        typename ContainerT>
    void rescanned_macro(
        ContextT const& ctx,
        ContainerT const& result)
    {
        outputFuncInfo(
            __func__,
            boost::fusion::make_vector(
                boost::cref(ctx),
                boost::cref(result)));

        Base::rescanned_macro(ctx, result);
    }

    /// インクルードパス設定処理関数
    template<typename ContextT>
    bool locate_include_file(
        ContextT& ctx,
        std::string& file_path, 
        bool is_system,
        char const* current_name,
        std::string& dir_path, 
        std::string& native_name)
    {
        const char* curName = (current_name == 0) ? "" : current_name;

        outputFuncInfo(
            "[before] " __func__,
            boost::fusion::make_vector(
                boost::cref(ctx),
                boost::cref(file_path),
                boost::cref(is_system),
                boost::cref(curName),
                boost::cref(dir_path),
                boost::cref(native_name)));

        bool result = Base::locate_include_file(
            ctx,
            file_path,
            is_system,
            current_name,
            dir_path,
            native_name);

        outputFuncInfo(
            "[after ] " __func__,
            boost::fusion::make_vector(
                boost::cref(ctx),
                boost::cref(file_path),
                boost::cref(is_system),
                boost::cref(curName),
                boost::cref(dir_path),
                boost::cref(native_name)));

        return result;
    }

    /// インクルードファイル解析開始時の捕捉関数
    template<typename ContextT>
    void opened_include_file(
        ContextT const& ctx,
        std::string const& relname, 
        std::string const& absname,
        bool is_system_include)
    {
        outputFuncInfo(
            __func__,
            boost::fusion::make_vector(
                boost::cref(ctx),
                boost::cref(relname),
                boost::cref(absname),
                boost::cref(is_system_include)));

        Base::opened_include_file(
            ctx,
            relname,
            absname,
            is_system_include);
    }

    /// インクルードファイル解析完了時の捕捉関数
    template<typename ContextT>
    void returning_from_include_file(ContextT const& ctx)
    {
        outputFuncInfo(
            __func__,
            boost::fusion::make_vector(boost::cref(ctx)));

        Base::returning_from_include_file(ctx);
    }

    /// インクルードガード検出時の捕捉関数
    template<typename ContextT>
    void detected_include_guard(
        ContextT const& ctx,
        std::string const& filename,
        std::string const& include_guard)
    {
        outputFuncInfo(
            __func__,
            boost::fusion::make_vector(
                boost::cref(ctx),
                boost::cref(filename),
                boost::cref(include_guard)));

        Base::detected_include_guard(ctx, filename, include_guard);
    }

    /// #pragma once 検出時の捕捉関数
    template<
        typename ContextT,
        typename TokenT>
    void detected_pragma_once(
        ContextT const& ctx,
        TokenT const& pragma_token,
        std::string const& filename)
    {
        outputFuncInfo(
            __func__,
            boost::fusion::make_vector(
                boost::cref(ctx),
                boost::cref(pragma_token),
                boost::cref(filename)));

        Base::detected_pragma_once(ctx, pragma_token, filename);
    }

    /// 例外送出処理関数
    template<
        typename ContextT,
        typename ExceptionT>
    void throw_exception(
        ContextT const& ctx,
        ExceptionT const& e)
    {
        outputFuncInfo(
            __func__,
            boost::fusion::make_vector(
                boost::cref(ctx),
                boost::cref(e)));

        Base::throw_exception(ctx, e);
    }

    /// トークンスキップ処理関数
    template<
        typename ContextT,
        typename TokenT>
    bool may_skip_whitespace(
        ContextT const& ctx,
        TokenT& token,
        bool& skipped_newline)
    {
        outputFuncInfo(
            __func__,
            boost::fusion::make_vector(
                boost::cref(ctx),
                boost::cref(token),
                boost::cref(skipped_newline)));

        return Base::may_skip_whitespace(ctx, token, skipped_newline);
    }

private:
    /// 出力ストリーム参照
    /// コピーできるように reference_wrapper で持つ
    boost::reference_wrapper<std::ostream> _out;
};
