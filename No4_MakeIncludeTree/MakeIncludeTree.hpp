#pragma once

#include <boost/wave.hpp>
#include <boost/wave/preprocessing_hooks.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/ref.hpp>
#include <stack>

// A hook class to output the tree
class MakeIncludeTreeHook
    :
    public boost::wave::context_policies::default_preprocessing_hooks
{
private:
    typedef boost::reference_wrapper<boost::property_tree::ptree> ref_ptree;

public:
    MakeIncludeTreeHook(boost::property_tree::ptree& target)
        :
        _target(target),
        _current(target),
        _parentStack(),
        _lastIncFile()
    {
    }

public:
    const boost::property_tree::ptree& getTarget() const
    {
        return _target;
    }

public:
    template <typename ContextT>
    bool found_include_directive(
        ContextT const& ctx,
        std::string const& filename, 
        bool include_next)
    {
        _lastIncFile = filename;

        return false;
    }

    template<typename ContextT>
    void opened_include_file(
        ContextT const&,
        std::string const&, 
        std::string const& absname,
        bool)
    {
        using namespace boost::property_tree;

        _parentStack.push(_current);

        ptree::iterator itr = _current.get().push_back(
            ptree::value_type(
                _lastIncFile,
                boost::property_tree::ptree(absname)));
        _current = boost::ref((*itr).second);
    }

    template<typename ContextT>
    void returning_from_include_file(ContextT const&)
    {
        _current = _parentStack.top();

        _parentStack.pop();
    }

private:
    ref_ptree _target;
    ref_ptree _current;
    std::stack<ref_ptree> _parentStack;
    std::string _lastIncFile;
};
