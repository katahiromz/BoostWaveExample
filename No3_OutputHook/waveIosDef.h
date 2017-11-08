#pragma once

#include <boost/wave.hpp>
#include <boost/wave/preprocessing_hooks.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>

#include <list>
#include <vector>
#include <string>
#include <ostream>
#include <exception>

namespace boost
{
    namespace wave
    {
        // Stream overload against ContextT
        template<
            typename IteratorT,
            typename LexIteratorT, 
            typename InputPolicyT,
            typename HooksT,
            typename DerivedT>
        inline std::ostream& operator<<(
            std::ostream& out,
            const context<
                IteratorT,
                LexIteratorT,
                InputPolicyT,
                HooksT,
                DerivedT>&)
        {
            out << "[context]";
            return out;
        }

        namespace cpplexer
        {
            // Stream overload against TokenT
            template<typename PositionT>
            inline std::ostream& operator<<(
                std::ostream& out,
                const lex_token<PositionT>& token)
            {
                boost::wave::token_id id(token);

                out << "[token(" << boost::wave::get_token_name(id) << ")]";
                if (id == boost::wave::T_NEWLINE)
                {
                    out << " \"\\n\"";
                }
                else if (token.is_valid())
                {
                    out << " \"" << token.get_value() << "\"";
                }

                return out;
            }
        } // namespace cpplexer
    } // namespace wave
} // namespace boost

namespace std
{
    namespace
    {
        // Output tokens
        template<typename IteratorT, typename DelimiterT>
        inline void outputTokens(
            std::ostream& out,
            IteratorT first,
            IteratorT last,
            DelimiterT delimiter)
        {
            for (IteratorT itr = first; itr != last; )
            {
                if (boost::wave::token_id(*itr) == boost::wave::T_NEWLINE)
                {
                    out << "\\n";
                }
                else if ((*itr).is_valid())
                {
                    out << (*itr).get_value();
                }

                std::advance(itr, 1);
                if (itr != last)
                {
                    out << delimiter;
                }
            }
        }
    }

    // Stream overload against ContainerT, DefinitionT
    template<typename PositionT, typename AllocT>
    inline std::ostream& operator<<(
        std::ostream& out,
        const list<
            boost::wave::cpplexer::lex_token<PositionT>,
            AllocT>& tokens)
    {
        out << "[list<token>] \"";
        outputTokens(out, tokens.begin(), tokens.end(), "");
        out << "\"";

        return out;
    }

    /// ParametersT に対する出力ストリームオーバロード
    template<typename PositionT, typename AllocT>
    inline std::ostream& operator<<(
        std::ostream& out,
        const vector<
            boost::wave::cpplexer::lex_token<PositionT>,
            AllocT>& tokens)
    {
        out << "[vector<token>] (";
        outputTokens(out, tokens.begin(), tokens.end(), ", ");
        out << ")";

        return out;
    }

    // Stream overload against std::vector<ContainerT>
    template<typename PositionT, typename VAllocT, typename LAllocT>
    inline std::ostream& operator<<(
        std::ostream& out,
        const vector<
            list<
                boost::wave::cpplexer::lex_token<PositionT>,
                LAllocT>,
            VAllocT>& tokenLists)
    {
        typedef boost::wave::cpplexer::lex_token<PositionT> Token;
        typedef list<Token, LAllocT> TokenList;
        typedef vector<TokenList, VAllocT> TokenListVector;

        out << "[vector<list<token>>] (";
        for (
            typename TokenListVector::const_iterator itr = tokenLists.begin();
            itr != tokenLists.end(); )
        {
            out << "\"";
            outputTokens(out, (*itr).begin(), (*itr).end(), "");
            out << "\"";

            std::advance(itr, 1);
            if (itr != tokenLists.end())
            {
                out << ", ";
            }
        }
        out << ")";

        return out;
    }

    // Stream overload against ExceptionT
    inline std::ostream& operator<<(
        std::ostream& out,
        const std::exception& e)
    {
        out << "[exception] " << e.what();
        return out;
    }
} // naemspace std
