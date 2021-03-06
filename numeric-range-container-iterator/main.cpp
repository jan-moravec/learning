// -*- C++ -*-
// Copyright (c) 2017, Just Software Solutions Ltd
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or
// without modification, are permitted provided that the
// following conditions are met:
//
// 1. Redistributions of source code must retain the above
// copyright notice, this list of conditions and the following
// disclaimer.
//
// 2. Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following
// disclaimer in the documentation and/or other materials
// provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of
// its contributors may be used to endorse or promote products
// derived from this software without specific prior written
// permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
// CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
// INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// Taken from https://www.justsoftwaresolutions.co.uk/cplusplus/generating_sequences.html

#include <stdexcept>
#include <iostream>
#include <functional>

template<typename T>
struct IncrementValue{
    void operator()(T& x) const{
        ++x;
    }
};

template<typename T>
struct IncrementBy{

    T delta;

    IncrementBy(T delta_):
        delta(std::move(delta_)){}

    void operator()(T& x) const{
        x+=delta;
    }
};

template<typename T,typename Increment=IncrementValue<T>>
class numeric_range{
public:
    enum class direction{increasing,decreasing};
private:
    T current;
    T final;
    Increment inc;
    direction dir;

    bool at_end(){
        if(dir==direction::increasing){
            return current>=final;
        } else {
            return current<=final;
        }
    }


public:
    class iterator{
        numeric_range* range;

        void check_done(){
            if(range->at_end()){
                range=nullptr;
            }
        }

        class postinc_return{
            T value;
        public:
            postinc_return(T value_):
                value(std::move(value_)){}
            T operator*(){
                return std::move(value);
            }
        };

    public:
        using value_type = T;
        using reference = T;
        using iterator_category=std::input_iterator_tag;
        using pointer = T*;
        using difference_type = void;

        iterator(numeric_range* range_):
            range(range_){
            if(range) check_done();
        }

        T operator*() const{
            return range->current;
        }

        T * operator->() const{
            return &range->current;
        }

        iterator& operator++(){
            if(!range)
                throw std::runtime_error("Increment a past-the-end iterator");
            range->inc(range->current);
            check_done();
            return *this;
        }

        postinc_return operator++(int){
            postinc_return temp(**this);
            ++*this;
            return temp;
        }

        friend bool operator==(iterator const& lhs,iterator const& rhs){
            return lhs.range==rhs.range;
        }
        friend bool operator!=(iterator const& lhs,iterator const& rhs){
            return !(lhs==rhs);
        }

    };

    iterator begin(){
        return iterator(this);
    }

    iterator end(){
        return iterator(nullptr);
    }


    numeric_range(T initial_,T final_):
        current(std::move(initial_)),final(std::move(final_)),dir(direction::increasing){}
    numeric_range(T initial_,T final_,Increment inc_):
        current(std::move(initial_)),final(std::move(final_)),inc(std::move(inc_)),
        dir(direction::increasing){}
    numeric_range(T initial_,T final_,Increment inc_,direction dir_):
        current(std::move(initial_)),final(std::move(final_)),inc(std::move(inc_)),
        dir(dir_){}
};

template<typename T>
numeric_range<T> range(T from,T to){
    if(to<from)
        throw std::runtime_error("Cannot count down ");
    return numeric_range<T>(std::move(from),std::move(to));
}

template<typename T>
numeric_range<T> range(T to){
    return range(T(),std::move(to));
}

template<typename T>
numeric_range<T,IncrementBy<T>> range(T from,T to,T delta){
    if(!delta)
        throw std::runtime_error("Step must be non-zero");
    using direction=typename numeric_range<T,IncrementBy<T>>::direction;
    direction const dir= (delta>T())?direction::increasing:direction::decreasing;
    return numeric_range<T,IncrementBy<T>>(
        std::move(from),std::move(to),IncrementBy<T>(std::move(delta)),dir);
}

int main(){
    for(int x: range(10)){
        std::cout<<x<<",";
    }
    std::cout<<std::endl;
    for(int x: range(0)){
        std::cout<<x<<",";
    }
    std::cout<<std::endl;
    for(int x: range(1,11)){
        std::cout<<x<<",";
    }
    std::cout<<std::endl;
    for(int x: range(0,30,5)){
        std::cout<<x<<",";
    }
    std::cout<<std::endl;
    for(int x: range(0,-10,-1)){
        std::cout<<x<<",";
    }
    std::cout<<std::endl;
    for(int x: range(0,31,5)){
        std::cout<<x<<",";
    }
    std::cout<<std::endl;
    for(int x: range(0,10,3)){
        std::cout<<x<<",";
    }
    std::cout<<std::endl;
    for(double x: range(0.1,30.1)){
        std::cout<<x<<",";
    }
    std::cout<<std::endl;
    for(double x: range(0.1,1.1,0.1)){
        std::cout<<x<<",";
    }
    std::cout<<std::endl;
    auto myrange=range(5);
    for(auto it=myrange.begin();it!=myrange.end();){
        std::cout<<*it++<<",";
    }
    std::cout<<std::endl;
}
