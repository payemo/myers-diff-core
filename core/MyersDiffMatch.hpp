#ifndef DIFF_PATCH_MYERS_DIFF_MATCH_HPP
#define DIFF_PATCH_MYERS_DIFF_MATCH_HPP

#include "Diff.hpp"
#include "Patch.hpp"
#include "utils.hpp"

#include <vector>

namespace differ
{
    // Declare testing forward type.
    namespace tests
    {
        class MyersDiffMatchTest;
    }

    class MyersDiffMatch
    {
        friend class tests::MyersDiffMatchTest;

    public:
        DiffList ComputeDiff(const String& textA, const String& textB);

        PatchList MakePatch(const String& text1, const String& textB);

        String PatchesToText(const PatchList& patches);

    protected:
        PatchList MakePatch(const String& text, const DiffList& diffs);

        void AddContext(Patch& patch, const String& text);

    private:
        Int32 patchMargin{ 4 };

    private:
        DiffList Compute(const String& seqA, const String& seqB);

        DiffList MiddleSnake(const String& seqA, const String& seqB);

        DiffList Partition(const String& a, const String& b, Int32 x, Int32 y);

        const String DiffText1(const DiffList& diffs) const;

        const String DiffText2(const DiffList& diffs) const;
    };
}

#endif