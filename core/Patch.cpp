#include "Patch.hpp"

#include <algorithm>

namespace differ
{
    String Patch::ToString() const
    {
        String lineInfoA = GetLineInfo(startA, lengthA);
        String lineInfoB = GetLineInfo(startB, lengthB);

        std::stringstream body;
        body << "@@ -" << lineInfoA << " +" << lineInfoB << " @@\n";

        std::for_each(diffs_.begin(), diffs_.end(), [&body](const Diff& diff)
        {
            Operation op = diff.GetOperation();

            switch (op)
            {
            case Operation::INSERT:
                body << "+";
                break;
            case Operation::DELETE:
                body << "-";
                break;
            case Operation::EQUAL:
                body << " ";
                break;
            }

            // TODO: Replace [ !~*'();/?:@&=+$,#] with %-encoding.
            body << diff.Text() << "\n";
        });

        return body.str();
    }

    const String Patch::GetLineInfo(UInt32 start, UInt32 length) const
    {
        std::stringstream ss;

        if (length == 0)
        {
            ss << start << ",0";
        }
        else if (lengthA == 1)
        {
            ss << start + 1;
        }
        else
        {
            ss << (start + 1) << "," << length;
        }

        return ss.str();
    }
}