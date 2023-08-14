#ifndef DIFF_PATH_SEQUENCE_HPP
#define DIFF_PATH_SEQUENCE_HPP

#include <assert.h>
#include <algorithm>
#include "defs.hpp"

namespace differ
{
    class Sequence
    {
    public:
        explicit Sequence(const String& text)
            : Sequence(text.begin(), text.end()) { }

        bool operator==(const Sequence& other) const
        {
            if (end_ - begin_ != other.end_ - other.begin_)
            {
                return false;
            }

            ConstIter aFrom = this->begin_, bFrom = other.begin_;

            while (*aFrom++ == *bFrom++)
                ;

            return (aFrom == end_ && bFrom == other.end_);
        }

        bool operator!=(const Sequence& other) const
        {
            return !(*this == other);
        }

        inline UInt32 Size() const { return end_ - begin_; }

        Char operator[](std::uint32_t index) const
        {
            assert(index < Size());
            return *(begin_ + index);
        }

        UInt32 IndexAt(const Sequence& inStr) const
        {
            const ConstIter at = std::search(begin_, end_, inStr.begin_, inStr.end_);
            return (at < end_) ? at - begin_ : -1;
        }

        Sequence Substring(UInt32 from) const
        {
            assert(from <= Size());
            return Sequence(begin_ + from, end_);
        }

        Sequence Substring(UInt32 from, UInt32 to) const
        {
            assert(from <= to);
            assert(from <= Size());

            if (to > Size()) to = Size();
            return Sequence(begin_ + from, begin_ + to);
        }

        const ConstIter& Begin() const { return begin_; }
        const ConstIter& End() const { return end_; }

    private:
        Sequence(ConstIter from, ConstIter to) 
            : begin_(from), end_(to) { }

        ConstIter begin_;
        ConstIter end_;
    };
}

#endif // DIFF_PATH_SEQUENCE_HPP