#include "Patch.hpp"

namespace differ
{
    String Patch::ToString() const
    {
        String lineInfoA = GetLineInfo(startA, lengthA);
        String lineInfoB = GetLineInfo(startB, lengthB);

        return GetPatchBody(lineInfoA, lineInfoB);
    }

    const String Patch::GetLineInfo(UInt32 start, UInt32 length) const
    {
        String lineInfo;

        if (start == 0)
        {
            lineInfo = start + ",0";
        }
        else if (lengthA == 1)
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
    const String Patch::GetPatchBody(const String& lineInfoA, const String& lineInfoB) const
    {
        std::stringstream body("@@ -" + lineInfoA + " +" + lineInfoB + "@@\n");

        /*std::for_each(diffs_.begin(), diffs_.end(), [&body](const Diff& diff)
        {
            Operation op = diff.GetOperation();
            body << std::get<char>(Diff::OperationData(op));
            body << "asd" << std::endl;
        });*/

        return body.str();
    }
}