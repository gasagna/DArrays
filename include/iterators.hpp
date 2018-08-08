#pragma once
#include <iostream>
#include <iterator>
#include <numeric>
#include <array>
#include <cmath>

namespace DArrays::Iterators {

////////////////////////////////////////////////////////////////
// Integer linear range, from FROM to TO, end point excluded. //
// This has the same semantics of the python range function   //
////////////////////////////////////////////////////////////////
class LinRange {
private:
    // range iterator
    class _LinRangeIter {
        private:
            int _state;
            int  _step;
        public:
            _LinRangeIter (int state, int step)
                : _state (state)
                , _step  (step ) {}

            inline int operator * () const {
                return _state;
            }

            inline _LinRangeIter operator ++ () {
                _state += _step;
                return *this;
            }

            inline bool operator != (_LinRangeIter& other) const {
                return _state != other._state;
            }
        };

    // range end points: included
    int _from, _to, _step;
public:
    LinRange(int from, int to, int step) : _from (from), _to (to), _step (step) {}
    LinRange(int from, int to)           : _from (from), _to (to), _step (1)    {}
    LinRange(int to)                     : _from (0),    _to (to), _step (1)    {}
    _LinRangeIter begin() { return {_from, _step}; }
    _LinRangeIter   end() { return {static_cast<int>(_from +
                            _step*std::ceil((_to - _from )/(double)_step)), _step}; }
};

}