#pragma once

// boost::wave �֘A�w�b�_���C���N���[�h
#include <boost/wave.hpp>
#include <boost/wave/preprocessing_hooks.hpp>

/// ���ׂẴf�B���N�e�B�u���������Ȃ��悤�ɂ���t�b�N�N���X
class IgnoreDirectiveHook
    :
    public boost::wave::context_policies::default_preprocessing_hooks
{
public:
    /// �f�B���N�e�B�u�������̕ߑ��֐�
    template<typename ContextT, typename TokenT>
    bool found_directive(ContextT const&, TokenT const&)
    {
        // true ��Ԃ��Ə������X�L�b�v�����
        return true;
    }
};
