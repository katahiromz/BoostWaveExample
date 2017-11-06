#include "MakeIncludeTree.hpp"

// boost::wave �֘A�w�b�_���C���N���[�h
#include <boost/wave.hpp>
#include <boost/wave/preprocessing_hooks.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <iostream>
#include <fstream>
#include <string>

// ���O��Ԃ̃G�C���A�X����`
namespace wave = boost::wave;

/// ���C���G���g���|�C���g
int main(int argc, char* argv[])
{
    using namespace std;
    using namespace boost::property_tree;

    if (argc < 2) { return 1; }

    // �\�[�X�t�@�C����ǂݍ���
    std::string code;
    {
        std::ifstream fs(argv[1]);
        fs.unsetf(std::ios::skipws);
        code.assign(
            std::istreambuf_iterator<char>(fs.rdbuf()),
            std::istreambuf_iterator<char>());
    }

    // �c���[�p��
    ptree incTree;

    // �R���e�L�X�g�p��
    typedef
        wave::context<
            std::string::const_iterator,
            wave::cpplexer::lex_iterator< wave::cpplexer::lex_token<> >,
            wave::iteration_context_policies::load_file_to_string,
            MakeIncludeTreeHook> // �Ǝ��̃t�b�N�N���X�^��n��
        Context;
    Context ctx(
        code.begin(),
        code.end(),
        argv[1],
        MakeIncludeTreeHook(incTree)); // �c���[��n��

    // ����T�|�[�g�I�v�V�����ݒ�
    ctx.set_language(
        wave::language_support(
            wave::support_cpp               |   // C++�Ƃ��ď���
            wave::support_option_long_long  |   // long long �^�T�|�[�g
            wave::support_option_variadics  |   // �ϒ������}�N���T�|�[�g
            wave::support_option_include_guard_detection)); // �C���N���[�h�K�[�h���o

    // �R���p�C���I�v�V�����ݒ�
    // Visual Studio 2013 ���C���X�g�[������Ă��邱�Ƃ�z�肵�Ă���
    ctx.add_macro_definition("_WIN32");
    ctx.add_macro_definition("_MSC_VER=1800");
    ctx.add_sysinclude_path(
        "C:/Program Files/Microsoft Visual Studio 12.0/VC/include");

    try
    {
        // �\�[�X�R�[�h���(�g�[�N���o�͂͂��Ȃ�)
        Context::iterator_type itrEnd = ctx.end();
        for (Context::iterator_type itr = ctx.begin(); itr != itrEnd; ++itr)
        {
            ;
        }
    }
    catch (const wave::cpp_exception& ex)
    {
        // ��O����
        cerr << ex.file_name() << " : " << ex.line_no() << endl;
        cerr << "  -> " << ex.description() << endl;
        return 1;
    }

    // �C���N���[�h�c���[�̓��e�� info_parser �ŏo��
    info_parser::write_info(cout, incTree);

    return 0;
}