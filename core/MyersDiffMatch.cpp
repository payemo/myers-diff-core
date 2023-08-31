﻿#include "MyersDiffMatch.hpp"

#include <cmath>

namespace differ
{
    DiffList MyersDiffMatch::ComputeDiff(const String& textA, const String& textB)
    {
        DiffList diffs{};

        // Check for equality
        if (textA == textB)
        {
            if (textA.size() > 0)
            {
                diffs.push_back(Diff(Operation::EQUAL, textA));
            }
            return diffs;
        }

        // Trim common left part
        UInt32 commonLength = utils::CommonPrefixLength(textA, textB);
        const String& commonPrefix = textA.substr(0, commonLength);

        String trimmedA = textA.substr(commonLength);
        String trimmedB = textB.substr(commonLength);

        // Trim common right part
        commonLength = utils::CommonSuffixLength(textA, textB);
        const String& commonSuffix = trimmedA.substr(trimmedA.size() - commonLength);

        trimmedA = trimmedA.substr(0, trimmedA.size() - commonLength);
        trimmedB = trimmedB.substr(0, trimmedB.size() - commonLength);

        diffs = Compute(trimmedA, trimmedB);

        if (!commonPrefix.empty())
        {
            diffs.push_front(Diff{ Operation::EQUAL, commonPrefix });
            //diffs.insert(diffs.begin(), Diff{ Operation::EQUAL, commonPrefix });
        }
        if (!commonSuffix.empty())
        {
            diffs.push_back(Diff{ Operation::EQUAL, commonSuffix });
        }

        return diffs;
    }

    DiffList MyersDiffMatch::Compute(const String& seqA, const String& seqB)
    {
        DiffList diffs{};


        if (seqA.size() == 0 && seqB.size() > 0)
        {
            diffs.push_back(Diff(Operation::INSERT, seqB));
            return diffs;
        }

        if (seqB.size() == 0 && seqA.size() > 0)
        {
            diffs.push_back(Diff(Operation::DELETE, seqA));
            return diffs;
        }

        {
            const String& longer = seqA.size() > seqB.size() ? seqA : seqB;
            const String& shorter = seqA.size() > seqB.size() ? seqB : seqA;
            Int32 i = utils::SearchIndexAt(longer, shorter);

            if (i != -1)
            {
                // shortest text is a substring of a longer one.
                Operation op = (seqA.size() > seqB.size()) ? Operation::DELETE : Operation::INSERT;
                diffs.push_back(Diff(op, longer.substr(0, i)));
                diffs.push_back(Diff(Operation::EQUAL, shorter));
                diffs.push_back(Diff(op, longer.substr(i + shorter.size())));
                return diffs;
            }

            if (shorter.size() == 1)
            {
                diffs.push_back(Diff(Operation::DELETE, seqA));
                diffs.push_back(Diff(Operation::INSERT, seqB));
                return diffs;
            }
        }

        return MiddleSnake(seqA, seqB);
    }

    DiffList MyersDiffMatch::MiddleSnake(const String& seqA, const String& seqB)
    {
        const Int32 m = seqA.size();
        const Int32 n = seqB.size();
        const Int32 delta = std::abs(m - n);
        // when delta is odd, check for overlap only while extending forward paths
        // when delta is even, check for overlap only while extending reverse paths
        const bool deltaIsOdd = (delta % 2 != 0);
        const Int32 maxD = (m + n + 1) / 2;
        const Int32 vOffset = maxD;
        const Int32 vLength = maxD * 2;
        Int32 fStart = 0, fEnd = 0;
        Int32 rStart = 0, rEnd = 0;

        std::vector<Int32> fV(vLength, -1);
        fV[vOffset + 1] = 0;

        std::vector<Int32> rV(vLength, -1);
        rV[vOffset + 1] = 0;

        for (Int32 d = 0; d < maxD; ++d)
        {
            // Find the end of the furthest reaching forward D-path in diagonal k.
            for (Int32 k = -d + fStart; k <= d - fEnd; k += 2)
            {
                Int32 kOffset = vOffset + k;
                Int32 x;
                if (k == -d || (k != d && fV[kOffset - 1] < fV[kOffset + 1]))
                {
                    x = fV[kOffset + 1];
                }
                else
                {
                    x = fV[kOffset - 1] + 1;
                }

                Int32 y = x - k;

                while (x < m && y < n && seqA[x] == seqB[y])
                {
                    ++x, ++y;
                }

                fV[kOffset] = x;

                if (x > m)
                {
                    fEnd += 2;
                }
                else if (y > n)
                {
                    fStart += 2;
                }
                else if (deltaIsOdd)
                {
                    /*
                        Check if the path overlaps the furthest reaching reverse(D - 1)-path in diagonal k,
                            where k ∈[∆ − (D − 1), ∆ + (D − 1)]
                    */
                    Int32 rOffset = delta + (vOffset - k); // furthest reaching point of a REVERSED (D - 1)-path of k
                    if (rOffset >= 0 && rOffset < vLength && rV[rOffset] != -1)
                    {
                        /*
                            (feasibility) u+v >= ceil(D/2) and x+y <= N + M − floor(D/2), and
                            (overlap) x-y = u-v and x >= u.
                        */
                        Int32 u = (m - rV[rOffset]);
                        if (x >= u)
                        { // overlaps
                            return Partition(seqA, seqB, x, y);
                        }
                    }
                }
            }

            // Find the end of the furthest reaching forward D-path in diagonal k + delta.
            for (Int32 k = -d + rStart; k <= d - rEnd; k += 2)
            {
                Int32 kOffset = vOffset + k;
                Int32 u;
                if (k == -d || (k != d && (rV[kOffset - 1] < rV[kOffset + 1])))
                {
                    u = rV[kOffset + 1];
                }
                else
                {
                    u = rV[kOffset - 1] + 1;
                }

                Int32 v = u - k;

                while (u < m && v < n && seqA[m - u - 1] == seqB[n - v - 1])
                {
                    ++u, ++v;
                }

                rV[kOffset] = u;

                if (u > m)
                {
                    rEnd += 2;
                }
                else if (v > n)
                {
                    rStart += 2;
                }
                else if (!deltaIsOdd)
                {
                    Int32 fOffset = delta + (vOffset - k);
                    if (fOffset >= 0 && fOffset < vLength && fV[fOffset] != -1)
                    {
                        Int32 x = fV[fOffset];
                        Int32 y = x + (vOffset - fOffset);

                        // Check overlapping
                        if (x >= (m - u))
                        {
                            return Partition(seqA, seqB, x, y);
                        }
                    }
                }
            }
        }

        DiffList diffs;
        diffs.push_back(Diff(Operation::DELETE, seqA));
        diffs.push_back(Diff(Operation::INSERT, seqB));

        return diffs;
    }

    DiffList MyersDiffMatch::Partition(const String& a, const String& b, Int32 x, Int32 y)
    {
        String a1 = a.substr(0, x);
        String b1 = b.substr(0, y);

        DiffList diffsA = ComputeDiff(a1, b1);

        a1 = a.substr(x);
        b1 = b.substr(y);

        DiffList diffsB = ComputeDiff(a1, b1);

        diffsA.insert(diffsA.end(), diffsB.begin(), diffsB.end());
        return diffsA;
    }

    const String MyersDiffMatch::DiffText1(const DiffList& diffs) const
    {
        String out;

        for (auto di = diffs.begin(); di != diffs.end(); ++di)
        {
            const auto& d = *di;

            if (d.GetOperation() != Operation::INSERT)
            {
                out += d.Text();
            }
        }

        return out;
    }

    const String MyersDiffMatch::DiffText2(const DiffList& diffs) const
    {
        String out;

        for (auto di = diffs.begin(); di != diffs.end(); ++di)
        {
            const auto& d = *di;

            if (d.GetOperation() != Operation::DELETE)
            {
                out += d.Text();
            }
        }

        return out;
    }
}