#pragma once

// boost::wave 関連ヘッダをインクルード
#include <boost/wave.hpp>
#include <boost/wave/preprocessing_hooks.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/ref.hpp>
#include <stack>

/// インクルードツリーを作成するフッククラス
class MakeIncludeTreeHook
    :
    public boost::wave::context_policies::default_preprocessing_hooks
{
private:
    typedef boost::reference_wrapper<boost::property_tree::ptree> ref_ptree;

public:
    /// コンストラクタ
    MakeIncludeTreeHook(boost::property_tree::ptree& target)
        :
        _target(target),
        _current(target),
        _parentStack(),
        _lastIncFile()
    {
    }

public:
    /// インクルードツリー取得
    const boost::property_tree::ptree& getTarget() const
    {
        return _target;
    }

public:
    /// #include ディレクティブ発見時の捕捉関数
    template<typename ContextT>
    bool found_include_directive(
        ContextT const& ctx,
        std::string const& filename, 
        bool include_next)
    {
        // ファイル名指定文字列を保存しておく
        _lastIncFile = filename;

        return false;
    }

    /// インクルードファイル解析開始時の捕捉関数
    template<typename ContextT>
    void opened_include_file(
        ContextT const&,
        std::string const&, 
        std::string const& absname,
        bool)
    {
        using namespace boost::property_tree;

        // 現在のカレントを親スタックへプッシュ
        _parentStack.push(_current);

        // 現在のカレントに子を追加し、それをカレントとする
        ptree::iterator itr = _current.get().push_back(
            ptree::value_type(
                _lastIncFile, // found_include_directive で保存した文字列
                boost::property_tree::ptree(absname)));
        _current = boost::ref((*itr).second);
    }

    /// インクルードファイル解析完了時の捕捉関数
    template<typename ContextT>
    void returning_from_include_file(ContextT const&)
    {
        // 現在の親をカレントとする
        _current = _parentStack.top();

        // 現在の親を親スタックからポップ
        _parentStack.pop();
    }

private:
    /// 対象ツリー参照
    ref_ptree _target;

    /// カレントツリー参照
    ref_ptree _current;

    /// 親ツリー参照スタック
    std::stack<ref_ptree> _parentStack;

    /// 直近の #include 指定
    std::string _lastIncFile;
};
