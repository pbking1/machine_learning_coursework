#ifndef _NODE_GINI_H
#define _NODE_GINI_H 
#include <random-forest/node.hpp>
#include <random-forest/common-libraries.hpp>

namespace RandomForest {

/*
template <typename T>
class X : Y<X> {}
-> http://en.wikipedia.org/wiki/Curiously_recurring_template_pattern#Static_polymorphism
*/

template <typename Splitter>
class NodeGini : public Node< NodeGini<Splitter>, ForestParams, Splitter > {
public:
    typedef typename Node< NodeGini<Splitter>, ForestParams, Splitter>::ParamType ParamType;
    using Node< NodeGini<Splitter>, ForestParams, Splitter>::params;
    NodeGini() {}
    NodeGini(int depth, ParamType const* params) :
        Node< NodeGini<Splitter>, ForestParams, Splitter>(depth, params)
    { 
    }

    std::pair<float, float> determine_best_threshold(KVPairs&          data_points,
                                                     std::vector<int>& classes_l,
                                                     std::vector<int>& classes_r,
                                                     RandomGen&        gen)
    {
        float best_loss = std::numeric_limits<float>::infinity();
        float best_thresh = 0;

        UnitDist fraction_dist;
        classes_l.assign(params->n_classes, 0);
        classes_r.assign(params->n_classes, 0);
        double n_l = 0;
        double n_r = 0;
        for (size_t i_sample = 0; i_sample < data_points.size(); ++i_sample) {
            classes_r[data_points[i_sample].second]++;
            n_r += 1;
        }
        // sort data so thresholding is easy based on position in array
        std::sort(data_points.begin(), data_points.end());
        // loop over data, update class distributions left&right
        for (size_t i_point = 1; i_point < data_points.size(); ++i_point) {
            int cls = data_points[i_point-1].second;
            classes_l[cls]++; // sample with class cls gets moved to left ...
            classes_r[cls]--; // from right
            n_l += 1;
            n_r -= 1;
            // don't split here if values are the same
            if (data_points[i_point-1].first == data_points[i_point].first)
                continue;
            // gini impurity = 1 - x^T x; x = class distribution
            float gini_left  = 1 - std::inner_product( classes_l.begin(), classes_l.end(), classes_l.begin(), 0.0) / (n_l * n_l);
            float gini_right = 1 - std::inner_product( classes_r.begin(), classes_r.end(), classes_r.begin(), 0.0) / (n_r * n_r);
            // weighted average
            float gini = n_l / (n_l + n_r) * gini_left + n_r / (n_l + n_r) * gini_right;
            if (gini < best_loss) {
                best_loss = gini;
                double fraction = fraction_dist(gen);
                best_thresh = fraction * data_points[i_point-1].first + (1-fraction) * data_points[i_point].first;
            }
        }
        return std::make_pair(best_thresh, best_loss);
    }

    template <typename Archive>
    void serialize(Archive& ar, unsigned /* version */)
    {
        ar & boost::serialization::make_nvp("base",  boost::serialization::base_object< Node< NodeGini<Splitter>, ForestParams, Splitter > >(*this));
    }
};

}
#endif
