#include "MakeIncludeTree.hpp"

// boost::wave 関連ヘッダをインクルード
#include <boost/wave.hpp>
#include <boost/wave/preprocessing_hooks.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <iostream>
#include <fstream>
#include <string>

// 名前空間のエイリアス名定義
namespace wave = boost::wave;

/// メインエントリポイント
int main(int argc, char* argv[])
{
    using namespace std;
    using namespace boost::property_tree;

    if (argc < 2) { return 1; }

    // ソースファイルを読み込む
    std::string code;
    {
        std::ifstream fs(argv[1]);
        fs.unsetf(std::ios::skipws);
        code.assign(
            std::istreambuf_iterator<char>(fs.rdbuf()),
            std::istreambuf_iterator<char>());
    }

    // ツリー用意
    ptree incTree;

    // コンテキスト用意
    typedef
        wave::context<
            std::string::const_iterator,
            wave::cpplexer::lex_iterator< wave::cpplexer::lex_token<> >,
            wave::iteration_context_policies::load_file_to_string,
            MakeIncludeTreeHook> // 独自のフッククラス型を渡す
        Context;
    Context ctx(
        code.begin(),
        code.end(),
        argv[1],
        MakeIncludeTreeHook(incTree)); // ツリーを渡す

    // 言語サポートオプション設定
    ctx.set_language(
        wave::language_support(
            wave::support_cpp               |   // C++として処理
            wave::support_option_long_long  |   // long long 型サポート
            wave::support_option_variadics  |   // 可変長引数マクロサポート
            wave::support_option_include_guard_detection)); // インクルードガード検出

    // コンパイルオプション設定
    // Visual Studio 2013 がインストールされていることを想定している
    ctx.add_macro_definition("_WIN32");
    ctx.add_macro_definition("_MSC_VER=1800");
    ctx.add_sysinclude_path(
        "C:/Program Files/Microsoft Visual Studio 12.0/VC/include");

    try
    {
        // ソースコード解析(トークン出力はしない)
        Context::iterator_type itrEnd = ctx.end();
        for (Context::iterator_type itr = ctx.begin(); itr != itrEnd; ++itr)
        {
            ;
        }
    }
    catch (const wave::cpp_exception& ex)
    {
        // 例外処理
        cerr << ex.file_name() << " : " << ex.line_no() << endl;
        cerr << "  -> " << ex.description() << endl;
        return 1;
    }

    // インクルードツリーの内容を info_parser で出力
    info_parser::write_info(cout, incTree);

    return 0;
}
