#ifndef DIFF_PATCH_MYERS_DIFF_MATCH_HPP
#define DIFF_PATCH_MYERS_DIFF_MATCH_HPP

#include "Diff.hpp"
#include "Patch.hpp"
#include "utils.hpp"

namespace differ
{
    class MyersDiffMatch
    {
    public:
        DiffList ComputeDiff(const String& textA, const String& textB)
        {
            DiffList diffs{};

            // Check for equality
            if (textA == textB)
            {
                if (!textA.size() > 0)
                {
                    diffs.push_back(Diff(Operation::EQUAL, Sequence(textA)));
                }
                return diffs;
            }

            Sequence s1(textA), s2(textB);

            // Trim common prefix
            UInt32 commonLength = CommonPrefixLength(s1, s2);

            Sequence trimmedA = s1.Substring(commonLength);
            Sequence trimmedB = s2.Substring(commonLength);

            commonLength = CommonSuffixLength(trimmedA, trimmedB);
            trimmedA = trimmedA.Substring(0, trimmedA.Size() - commonLength);
            trimmedB = trimmedB.Substring(0, trimmedB.Size() - commonLength);

            diffs = Compute(trimmedA, trimmedB);

            return {};
        }

    private:
        DiffList Compute(const Sequence& seqA, const Sequence& seq2)
        {
            DiffList diffs{};


            return {};
        }

        UInt32 CommonPrefixLength(const Sequence& seqA, const Sequence& seqB)
        {
            auto a = seqA.Begin(), b = seqB.Begin();

            while (*a++ != *b++)
                ;

            return static_cast<UInt32>(std::distance(a, seqA.Begin()));
        }

        UInt32 CommonSuffixLength(const Sequence& seqA, const Sequence& seqB)
        {
            auto a = seqA.End(), b = seqB.End();

            while (*a-- != *b--)
                ;

            return static_cast<UInt32>(std::distance(a, seqA.End()));
        }
    };
}

#endif