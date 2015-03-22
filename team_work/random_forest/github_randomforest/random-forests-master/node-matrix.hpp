#ifndef _NODE_MATRIX_H
#define _NODE_MATRIX_H 
#include <random-forest/node.hpp>
#include <random-forest/common-libraries.hpp>

namespace RandomForest {

struct MatrixParams : public ForestParams {
    cv::Mat_<double> loss_matrix;
};


template <typename Splitter>
class NodeMatrix : public Node< NodeMatrix<Splitter>, MatrixParams, Splitter > {
public:
    typedef typename Node< NodeMatrix<Splitter>, MatrixParams, Splitter>::ParamType ParamType;
    using Node< NodeMatrix<Splitter>, MatrixParams, Splitter>::params;
    NodeMatrix() {}
    NodeMatrix(int depth, ParamType const* params) :
        Node< NodeMatrix<Splitter>, MatrixParams, Splitter>(depth, params)
    { 
    }

    double matrix_loss(std::vector<int> const& frequencies) const {
        double ret = 0;
        for (size_t i_y = 0; i_y < frequencies.size(); ++i_y) {
            for (size_t i_x = 0; i_x < frequencies.size(); ++i_x) {
                ret += frequencies[i_x] * frequencies[i_y] * params->loss_matrix(i_y, i_x);
            }
        }
        return ret;
    }

    void update_loss(double& loss, std::vector<int> const& frequencies, int cls, int direction) const {
        double linear_term_1 = 0;
        for (size_t i_y = 0; i_y < frequencies.size(); ++i_y) {
            linear_term_1 += params->loss_matrix(i_y, cls) * frequencies[i_y];
        }
        double linear_term_2 = 0;
        for (size_t i_x = 0; i_x < frequencies.size(); ++i_x) {
            linear_term_2 += params->loss_matrix(cls, i_x) * frequencies[i_x];
        }
        double constant_term = params->loss_matrix(cls, cls);
        loss += direction * (linear_term_1 + linear_term_2) + constant_term;
    }

    std::pair<float, float> determine_best_threshold(KVPairs&          data_points,
                                                     std::vector<int>& classes_l,
                                                     std::vector<int>& classes_r,
                                                     RandomGen&        gen)
    {
        float best_loss = std::numeric_limits<float>::infinity();
        float best_thresh = 0.0;

        UnitDist fraction_dist;
        classes_l.assign(params->n_classes, 0);
        classes_r.assign(params->n_classes, 0);
        double n_l = 0;
        double n_r = 0;
        for (int i_sample = 0; i_sample < data_points.size(); ++i_sample) {
            classes_r[data_points[i_sample].second]++;
            n_r += 1;
        }
        std::sort(data_points.begin(), data_points.end());
        // find best threshold
        double loss_left  = 0; // classes_l is 0
        double loss_right = matrix_loss(classes_r);
        for (size_t i_point = 1; i_point < data_points.size(); ++i_point) {
            int cls = data_points[i_point-1].second;
            classes_l[cls]++;
            classes_r[cls]--;
            n_l += 1;
            n_r -= 1;
            update_loss(loss_left,  classes_l, cls, +1);
            update_loss(loss_right, classes_r, cls, -1);
            if (data_points[i_point-1].first == data_points[i_point].first)
                continue;
            //                  float loss_left  = matrix_loss(classes_l);
            //                  float loss_right = matrix_loss(classes_r);
            float loss =  1.0 / (n_l * (n_l + n_r)) * loss_left + 1.0 / ((n_l + n_r) * n_r) * loss_right;
            if (loss < best_loss) {
                best_loss = loss;
                double fraction = fraction_dist(gen);
                best_thresh = fraction * data_points[i_point-1].first + (1-fraction) * data_points[i_point].first;
            }
        }
        return std::make_pair(best_thresh, best_loss);
    }
    template <typename Archive>
    void serialize(Archive& ar, unsigned /* version */)
    {
        ar & boost::serialization::make_nvp("base", boost::serialization::base_object< Node< NodeMatrix<Splitter>, ForestParams, Splitter > >(*this));
    }
};

}

#endif
