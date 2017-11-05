#pragma once

// boost::wave �֘A�w�b�_���C���N���[�h
#include <boost/wave.hpp>
#include <boost/wave/preprocessing_hooks.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/ref.hpp>
#include <stack>

/// �C���N���[�h�c���[���쐬����t�b�N�N���X
class MakeIncludeTreeHook
    :
    public boost::wave::context_policies::default_preprocessing_hooks
{
private:
    typedef boost::reference_wrapper<boost::property_tree::ptree> ref_ptree;

public:
    /// �R���X�g���N�^
    MakeIncludeTreeHook(boost::property_tree::ptree& target)
        :
        _target(target),
        _current(target),
        _parentStack(),
        _lastIncFile()
    {
    }

public:
    /// �C���N���[�h�c���[�擾
    const boost::property_tree::ptree& getTarget() const
    {
        return _target;
    }

public:
    /// #include �f�B���N�e�B�u�������̕ߑ��֐�
    template<typename ContextT>
    bool found_include_directive(
        ContextT const& ctx,
        std::string const& filename, 
        bool include_next)
    {
        // �t�@�C�����w�蕶�����ۑ����Ă���
        _lastIncFile = filename;

        return false;
    }

    /// �C���N���[�h�t�@�C����͊J�n���̕ߑ��֐�
    template<typename ContextT>
    void opened_include_file(
        ContextT const&,
        std::string const&, 
        std::string const& absname,
        bool)
    {
        using namespace boost::property_tree;

        // ���݂̃J�����g��e�X�^�b�N�փv�b�V��
        _parentStack.push(_current);

        // ���݂̃J�����g�Ɏq��ǉ����A������J�����g�Ƃ���
        ptree::iterator itr = _current.get().push_back(
            ptree::value_type(
                _lastIncFile, // found_include_directive �ŕۑ�����������
                boost::property_tree::ptree(absname)));
        _current = boost::ref((*itr).second);
    }

    /// �C���N���[�h�t�@�C����͊������̕ߑ��֐�
    template<typename ContextT>
    void returning_from_include_file(ContextT const&)
    {
        // ���݂̐e���J�����g�Ƃ���
        _current = _parentStack.top();

        // ���݂̐e��e�X�^�b�N����|�b�v
        _parentStack.pop();
    }

private:
    /// �Ώۃc���[�Q��
    ref_ptree _target;

    /// �J�����g�c���[�Q��
    ref_ptree _current;

    /// �e�c���[�Q�ƃX�^�b�N
    std::stack<ref_ptree> _parentStack;

    /// ���߂� #include �w��
    std::string _lastIncFile;
};
