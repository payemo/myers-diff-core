#ifndef DIFF_PATCH_MYERS_DIFF_MATCH_HPP
#define DIFF_PATCH_MYERS_DIFF_MATCH_HPP

#include <vector>
#include "Diff.hpp"
#include "Patch.hpp"
#include "utils.hpp"

namespace differ
{
    class MyersDiffMatch
    {
    public:
        DiffList ComputeDiff(const Sequence& textA, const Sequence& textB)
        {
            DiffList diffs{};

            // Check for equality
            if (textA == textB)
            {
                if (textA.Size() > 0)
                {
                    diffs.push_back(Diff(Operation::EQUAL, textA));
                }
                return diffs;
            }

            // Trim common left part
            UInt32 commonLength = utils::CommonPrefixLength(textA, textB);

            Sequence trimmedA = textA.Substring(commonLength);
            Sequence trimmedB = textB.Substring(commonLength);

            // Trim common right part
            commonLength = utils::CommonSuffixLength(textA, textB);
            trimmedA = trimmedA.Substring(0, trimmedA.Size() - commonLength);
            trimmedB = trimmedB.Substring(0, trimmedB.Size() - commonLength);

            diffs = Compute(trimmedA, trimmedB);

            return diffs;
        }

    private:
        DiffList Compute(const Sequence& seqA, const Sequence& seqB)
        {
            DiffList diffs{};


            if (seqA.Size() == 0 && seqB.Size() > 0)
            {
                diffs.push_back(Diff(Operation::INSERT, seqB));
                return diffs;
            }

            if (seqB.Size() == 0 && seqA.Size() > 0)
            {
                diffs.push_back(Diff(Operation::DELETE, seqA));
                return diffs;
            }

            {
                const Sequence& longer = seqA.Size() > seqB.Size() ? seqA : seqB;
                const Sequence& shorter = seqA.Size() > seqB.Size() ? seqB : seqA;
                UInt32 i = longer.IndexAt(shorter);

                if (i != 1)
                {
                    // shortest text is a substring of a longer one.
                    Operation op = (seqA.Size() > seqB.Size()) ? Operation::DELETE : Operation::INSERT;
                    diffs.push_back(Diff(op, longer.Substring(0, i)));
                    diffs.push_back(Diff(Operation::EQUAL, shorter));
                    diffs.push_back(Diff(op, longer.Substring(i + shorter.Size())));
                    return diffs;
                }
            }

            if (seqB.Size() == 1)
            {
                diffs.push_back(Diff(Operation::DELETE, seqA));
                diffs.push_back(Diff(Operation::INSERT, seqB));
                return diffs;
            }

            return diffs;
        }

        DiffList MiddleSnake(const Sequence& seqA, const Sequence& seqB)
        {
            const UInt32 m = seqA.Size();
            const UInt32 n = seqB.Size();
            const Int32 delta = m - n;
            // when delta is odd, check for overlap only while extending forward paths
            // when delta is even, check for overlap only while extending reverse paths
            const bool deltaIsOdd = (delta % 2 != 0);
            const UInt32 maxD = (m + n + 1) / 2;
            const UInt32 vOffset = maxD;
            const UInt32 vLength = maxD * 2;

            std::vector<Int32> v1(vLength, -1);
            v1[vOffset + 1] = 0;

            std::vector<Int32> v2(vLength, -1);
            v2[vOffset + 1] = 0;

            for (Int32 d = 0; d < maxD; ++d)
            {
                // Find the end of the furthest reaching forward D-path in diagonal k.
                for (Int32 k = -d; k <= d; k += 2)
                {
                    Int32 kOffset = vOffset + k;
                    Int32 x;
                    if ((k == -d || k != d) && v1[k - 1] < v1[k + 1])
                    {
                        x = v1[kOffset + 1];
                    }
                    else
                    {
                        x = v1[kOffset - 1] + 1;
                    }
                    Int32 y = x - k;

                    while (x < m && y < n && seqA[x] == seqB[y])
                    {
                        ++x, ++y;
                    }

                    v1[kOffset] = x;

                    if (deltaIsOdd)
                    {
                        /* 
                            Check if the path overlaps the furthest reaching reverse(D - 1)-path in diagonal k, 
                                where k ∈[∆ − (D − 1), ∆ + (D − 1)]
                        */
                        Int32 rOffset = delta + (vOffset - k); // furthest reaching point of a REVERSED (D - 1)-path of k
                        if (rOffset >= 0 && vLength < m && v2[rOffset] != -1)
                        {
                            /*
                                (feasibility) u+v >= ceil(D/2) and x+y <= N + M − floor(D/2), and
                                (overlap) x-y = u-v and x >= u.
                            */
                            Int32 u = (m - v2[rOffset]);
                            if (x >= u)
                            { // overlaps
                                Partition(seqA, seqB, x, y);
                            }
                        }
                    }
                }
            }
        }

        DiffList Partition(const Sequence& a, const Sequence& b, Int32 x, Int32 y)
        {
            Sequence a1 = a.Substring(0, x);
            Sequence b1 = b.Substring(0, y);

            DiffList diffsA = Compute(a1, b1);

            a1 = a.Substring(x);
            b1 = b.Substring(y);

            DiffList diffsB = Compute(a1, b1);

            diffsA.insert(diffsA.end(), diffsA.begin(), diffsA.end());
            return diffsA;
        }
    };
}

#endif