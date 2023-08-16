#ifndef DIFF_PATCH_UTILS_HPP
#define DIFF_PATCH_UTILS_HPP

namespace differ
{
    namespace utils
    {
        UInt32 CommonPrefixLength(const Sequence& a, const Sequence& b)
        {
            UInt32 n = static_cast<UInt32>(std::min(a.Size(), b.Size()));
            for (UInt32 i = 0; i < n; ++i)
            {
                if (a[i] != b[i])
                {
                    return i;
                }
            }

            return n;
        }

        UInt32 CommonSuffixLength(const Sequence& a, const Sequence& b)
        {
            UInt32 al = a.Size(), bl = b.Size();
            UInt32 n = static_cast<UInt32>(std::min(a.Size(), b.Size()));

            for (UInt32 i = 1; i <= n; ++i)
            {
                if (a[al - i] != b[bl - i])
                {
                    return i - 1;
                }
            }

            return n;
        }
    }
}

#endif