#pragma once

#include <boost/wave.hpp>
#include <boost/wave/preprocessing_hooks.hpp>

// A preprocessing hook to ignore all the directives
class IgnoreDirectiveHook
    :
    public boost::wave::context_policies::default_preprocessing_hooks
{
public:
    template<typename ContextT, typename TokenT>
    bool found_directive(ContextT const&, TokenT const&)
    {
        return true;    // true is skip
    }
};
