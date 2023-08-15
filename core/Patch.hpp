#ifndef DIFF_PATCH_PATCH_CLASS_HPP
#define DIFF_PATCH_PATCH_CLASS_HPP

#include <list>
#include "Diff.hpp"

namespace differ
{
    using DiffList = typename std::list<Diff>;

    class Patch
    {
    public:
        Patch() = default;

        inline bool IsNull()
        {
            return (startA_ == 0 && lengthA_ == 0)
                && (startB_ == 0 && lengthB_ == 0)
                && diffs_.size() == 0;
        }

        inline void Append(const Diff& diff) { diffs_.push_back(diff); }

        /**
         * GNU unified format.
         * Header: @@ -9,3 +8,6 @@
         * Indices are printed as 1-based, not 0-based.
         * @return The GNU diff string
         */
        String ToString() const
        {
            String lineInfoA = GetLineInfo(startA_, lengthA_);
            String lineInfoB = GetLineInfo(startB_, lengthB_);

            return GetPatchBody(lineInfoA, lineInfoB);
        }

    private:
        DiffList diffs_{};
        UInt32 startA_{ 0 }, lengthA_{ 0 };
        UInt32 startB_{ 0 }, lengthB_{ 0 };

    private:
        const String GetLineInfo(UInt32 start, UInt32 length) const
        {
            String lineInfo;

            if (start == 0)
            {
                lineInfo = start + ",0";
            }
            else if (lengthA_ == 1)
            {
                lineInfo = start + 1;
            }
            else
            {
                lineInfo = (start + 1) + "," + length;
            }

            return lineInfo;
        }

        // TODO: Replace [ !~*'();/?:@&=+$,#] with %-encoding.
        const String GetPatchBody(const String& lineInfoA, const String& lineInfoB) const
        {
            std::stringstream body("@@ -" + lineInfoA + " +" + lineInfoB + "@@\n");

            std::for_each(diffs_.begin(), diffs_.end(), [&body](const Diff& diff)
            {
                Operation op = diff.GetOperation();
                body << std::get<char>(Diff::OperationData(op));
                body << diff.Text() << std::endl;
            });

            return body.str();
        }
    };
}

#endif