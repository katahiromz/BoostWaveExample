#pragma once

// boost::wave 関連ヘッダをインクルード
#include <boost/wave.hpp>
#include <boost/wave/preprocessing_hooks.hpp>

/// すべてのディレクティブを処理しないようにするフッククラス
class IgnoreDirectiveHook
    :
    public boost::wave::context_policies::default_preprocessing_hooks
{
public:
    /// ディレクティブ発見時の捕捉関数
    template<typename ContextT, typename TokenT>
    bool found_directive(ContextT const&, TokenT const&)
    {
        // true を返すと処理がスキップされる
        return true;
    }
};
