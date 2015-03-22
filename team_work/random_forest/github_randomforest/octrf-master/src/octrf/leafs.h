#pragma once

#include "common.h"

namespace octrf {
    namespace leafs {
        template <typename YType, typename T>
        class Avg {
        public:
            T v_;
            Avg() : v_(0){}
            Avg(const std::vector<YType>& Y, const std::vector<int>& idxs) : v_(0){
                for(auto it = idxs.begin(); it != idxs.end(); ++it){
                    v_ += (T)Y[*it];
                }
                v_ /= (T)idxs.size();
            }

            static T set2result(const std::vector<Avg<YType, T> >& vs){
                T avg = 0;
                for(int i = 0; i < vs.size(); ++i){
                    avg += vs[i].v_;
                }
                return avg / (T)vs.size();
            }

            std::string serialize() const {
                std::stringstream ss;
                ss << v_;
                return ss.str();
            }
            void deserialize(const std::string& s){
                std::stringstream ss(s);
                ss >> v_;
            }
        };
    } // leafs
} // octrf
