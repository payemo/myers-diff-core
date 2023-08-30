#ifndef DIFF_PATCH_PATCH_CLASS_HPP
#define DIFF_PATCH_PATCH_CLASS_HPP

#include "Diff.hpp"

namespace differ
{
    class Patch
    {
    public:
        /**
         * GNU unified format.
         * Header: @@ -9,3 +8,6 @@
         * Indices are printed as 1-based, not 0-based.
         * @return The GNU diff string
         */
        String ToString() const;

        inline bool IsNull()
        {
            return (startA_ == 0 && lengthA_ == 0)
                && (startB_ == 0 && lengthB_ == 0)
                && diffs_.size() == 0;
        }

        inline void Append(const Diff& diff) { diffs_.push_back(diff); }

    private:
        DiffList diffs_{};
        UInt32 startA_{ 0 }, lengthA_{ 0 };
        UInt32 startB_{ 0 }, lengthB_{ 0 };

    private:
        const String GetLineInfo(UInt32 start, UInt32 length) const;

        // TODO: Replace [ !~*'();/?:@&=+$,#] with %-encoding.
        const String GetPatchBody(const String& lineInfoA, const String& lineInfoB) const;
    };
}

#endif