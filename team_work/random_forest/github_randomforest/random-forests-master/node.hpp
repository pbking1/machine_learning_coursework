#ifndef _NODE_H
#define _NODE_H 
#include <random-forest/common-libraries.hpp>
#include <boost/serialization/scoped_ptr.hpp>
#include <boost/serialization/vector.hpp>
#if VERBOSE_NODE_LEARNING
#include <cstdio>
#endif

namespace RandomForest {

template <typename Derived, typename ParamT, typename Splitter>
class Node {
public:
    bool is_leaf;
    size_t n_samples;
    size_t depth;
    typedef ParamT ParamType;
    ParamType const* params;
    Splitter splitter;
    
    boost::scoped_ptr<Derived> left;
    boost::scoped_ptr<Derived> right;
    std::vector<float> node_dist;

    Node() : is_leaf(true), n_samples(0), depth(-1), params(0) {}
    Node(size_t depth, ParamType const* params) :
        is_leaf(true), n_samples(0), depth(depth), params(params)
    {}

    bool pure(cv::Mat_<int> const& labels, int* sample_idxes) const {
        if (n_samples < 2) 
            return true; // an empty node is by definition pure
        int first_sample_idx = sample_idxes[0];
        int seen_class = labels(first_sample_idx);
        // check if all classes are equal to the first class
        for (size_t i_sample = 1; i_sample < n_samples; ++i_sample) {
            int sample_idx = sample_idxes[i_sample];
            if (labels(sample_idx) != seen_class)
                return false;
        }
        return true;
    }

    float const* votes() const {
        return (float const*)&node_dist[0];
    }

    int partition_samples(cv::Mat_<float> const& samples, int* sample_idxes) {
        // sort samples in bag so that left-samples precede right-samples
        // works like std::partition 
        int low  = 0;
        int high = n_samples;

        while (true) {
            while (true) {
                if (low == high) {
                    return low;
                } else if (!splitter.classify_sample(samples[sample_idxes[low]])) {
                    ++low;
                } else {
                    break;
                }
            }
            --high;
            while (true) {
                if (low == high) {
                    return low;
                } else if (splitter.classify_sample(samples[sample_idxes[high]])) {
                    --high;
                } else {
                    break;
                }
            }
            std::swap(sample_idxes[low], sample_idxes[high]);
            ++low;
        }
    }

    Derived const* split (float const* sample) const {
        if (splitter.classify_sample(sample)) {
            return right.get();
        } else {
            return left.get();
        }
    }

    typedef std::list<Derived const*> NodeList;

    NodeList get_all_childs() {
        NodeList ret;
        ret.push_back(this);
        if (!is_leaf) {
            NodeList left_childs  = left->get_all_childs();
            ret.splice(ret.end(), left_childs);
            NodeList right_childs = right->get_all_childs();
            ret.splice(ret.end(), right_childs);
        }
        return ret;
    }

    template<typename SplitGenerator>
    void determine_best_split(cv::Mat_<float> const&    samples,
                              cv::Mat_<int> const&      labels,
                              int*                      sample_idxes,
                              SplitGenerator            split_generator,
                              RandomGen&                gen
                              )
    {
        KVPairs data_points(n_samples);
        float best_loss = std::numeric_limits<float>::infinity();

        std::vector<int> classes_l;
        std::vector<int> classes_r;

        // pass information about data to split generator
        split_generator.init(samples, 
                             labels, 
                             sample_idxes,
                             n_samples,
                             params->n_classes,
                             gen);

        size_t n_proposals = split_generator.num_proposals();

        std::pair<float, float> results; // (threshold, loss)
        
        for (size_t i_proposal = 0; i_proposal < n_proposals; ++i_proposal) {
            // generate proposal
            Splitter split = split_generator.gen_proposal(gen);
            // map samples to numbers using proposal
            split.map_points(samples, labels, sample_idxes, n_samples, data_points);
            // check best loss using this proposal
            results = static_cast<Derived*>(this)->determine_best_threshold(data_points, classes_l, classes_r, gen);
            if (results.second < best_loss) {
                // Proposal resulted into new optimum
                best_loss = results.second;
                split.set_threshold(results.first);
                splitter = split;
            }
        }
    }

    template<typename SplitGenerator>
    void train(cv::Mat_<float> const& samples, 
               cv::Mat_<int> const& labels, 
               int* sample_idxes, 
               size_t n_samples_, 
               SplitGenerator const& split_generator,
               RandomGen& gen
               ) 
    {
        n_samples = n_samples_;
        node_dist.resize(params->n_classes, 0.0f);
        for (size_t i_sample = 0; i_sample < n_samples; ++i_sample) {
            int label = labels(sample_idxes[i_sample]);
            node_dist[label] += 1.0f/n_samples;
        }
        bool do_split = // Only split if ...
            (n_samples >= params->min_samples_per_node) && // enough samples are available
            !pure(labels, sample_idxes) && // this node is not already pure
            (depth < params->max_depth); // we did not reach max depth
        if (!do_split) {
            return;
        } 

        is_leaf = false; // it's not a leaf if we split

#if VERBOSE_NODE_LEARNING
        std::printf("Determining the best split at depth %zu/%zu\n", depth, params->max_depth);
#endif
        determine_best_split(samples, labels, sample_idxes, split_generator, gen);

        left.reset(new Derived(depth + 1, params));
        right.reset(new Derived(depth + 1, params));

        // sort samples in bag so that left-samples precede right-samples
        int low = partition_samples(samples, sample_idxes);
        int n_samples_left  = low;
        int n_samples_right = n_samples - low;
        int offset_left     = 0;
        int offset_right    = low;
#ifdef TREE_GRAPHVIZ_STREAM
        if (depth <= TREE_GRAPHVIZ_MAX_DEPTH) {
            TREE_GRAPHVIZ_STREAM << "p" << std::hex << (unsigned long)this       
                << " -> " 
                << "p" << std::hex << (unsigned long)left.get() 
                << std::dec << " [label=\"" << n_samples_left << "\"];" <<  std::endl;
            TREE_GRAPHVIZ_STREAM << "p" << std::hex << (unsigned long)this       
                << " -> " 
                << "p" << std::hex << (unsigned long)right.get() 
                << std::dec << " [label=\"" << n_samples_right << "\"];" << std::endl;
        }
#endif
        // train left and right side of split
        left->train (samples, labels, sample_idxes + offset_left,  n_samples_left,  split_generator, gen);
        right->train(samples, labels, sample_idxes + offset_right, n_samples_right, split_generator, gen);
    } 

    template <typename Archive>
    void serialize(Archive& ar, unsigned /*version*/)
    {
        ar & BOOST_SERIALIZATION_NVP(is_leaf);
        ar & BOOST_SERIALIZATION_NVP(n_samples);
        ar & BOOST_SERIALIZATION_NVP(depth);
        ar & BOOST_SERIALIZATION_NVP(params);
        ar & BOOST_SERIALIZATION_NVP(splitter);
        ar & BOOST_SERIALIZATION_NVP(node_dist);
        ar & BOOST_SERIALIZATION_NVP(left);
        ar & BOOST_SERIALIZATION_NVP(right);
    }
};

}
#endif
