#ifndef DIFF_PATH_SEQUENCE_HPP
#define DIFF_PATH_SEQUENCE_HPP

#include <assert.h>
#include <algorithm>
#include <iterator>
#include <iostream>
#include "defs.hpp"

namespace differ
{
    class Sequence
    {
    public:
        Sequence(const String& text)
            : Sequence(text.begin(), text.end()) { }

        Sequence(const ConstIter& begin, const ConstIter& end)
            : begin_(begin), end_(end) { }

        Sequence(const Sequence& other)
            : begin_(other.begin_), end_(other.end_) { }

        Sequence& operator=(const Sequence other)
        {
            if (this == &other)
            {
                return *this;
            }

            begin_ = other.begin_;
            end_ = other.end_;

            return *this;
        }

        bool operator==(const Sequence& other) const
        {
            return std::equal(begin_, end_, other.begin_);
        }

        bool operator!=(const Sequence& other) const
        {
            return !(*this == other);
        }

        const Char operator*() const { return *this->begin_; }

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

        const Sequence& Substring(UInt32 from) const
        {
            assert(from <= Size());

            return Sequence(begin_ + from, end_);
        }

        const Sequence& Substring(UInt32 from, UInt32 to) const
        {
            assert(from <= to);
            assert(from <= Size());

            if (to > Size()) to = Size();

            return Sequence(begin_ + from, begin_ + to);
        }

        ConstIter Begin() const { return begin_; }
        ConstIter End() const { return end_; }

    private:
        ConstIter begin_;
        ConstIter end_;
    };
}

#endif // DIFF_PATH_SEQUENCE_HPP