#ifndef DIFF_PATCH_PATCH_CLASS_HPP
#define DIFF_PATCH_PATCH_CLASS_HPP

#include "Diff.hpp"

namespace differ
{
    class Patch
    {
    public:
        Int32 startA{ 0 }, lengthA{ 0 };
        Int32 startB{ 0 }, lengthB{ 0 };

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
            return (startA == 0 && lengthA == 0)
                && (startB == 0 && lengthB == 0)
                && diffs_.size() == 0;
        }

        inline bool HasDiffs() { return !diffs_.empty(); }

        inline void Append(const Diff& diff) { diffs_.push_back(diff); }

        inline void Prepend(const Diff& diff) { diffs_.push_front(diff); }

    private:
        DiffList diffs_{};

    private:
        const String GetLineInfo(UInt32 start, UInt32 length) const;
    };

    using PatchList = typename std::list<Patch>;
}

#endif