#ifndef UTIL_HPP
#define UTIL_HPP

#include <queue>
#include <vector>

#include <opencv2/opencv.hpp>

size_t compute_color_intensity(const cv::Vec3b &color);

// Converts OpenCV matrix to its connected components representation.
// Components are labeled in order of observation, starting from the upper-left
// corner and reading line-wise.
// Notes:
//   - Can only find a maximum of 2^31-1 components
//   - Image data type must be uchar or ushort (more precisely, signed integer
//   of less than 32-bit)
void connected_components(cv::Mat &m);

// Converts pixel values of specified colors to other colors
template <typename T>
void convert(cv::Mat &m, std::map<T,T> conv);
template <typename T>
void convert(cv::Mat &m, T from, T to);

// Bucket fill image using a specified conversion map.
// Notes:
//  - Currently only supports 4-way fills (ie., not diagonal, 8-way fills)
template <typename T>
void fill(cv::Mat &m, std::map<T,T> conv, int R, int C);

// Returns the cropping box of the image when there are white-/ or blackspaces
// around it
cv::Rect get_cropping_box(const cv::Mat &img);

cv::Rect inner_bounds(cv::RotatedRect rr);

void resize(cv::Rect &r, double factor);

double scale_down_to_max(cv::Mat &img, size_t maxImageSize);

void scale_down_to_scale(cv::Mat &img, double scale);

void scale_up_to_scale(cv::Mat &img, double scale);

template <typename T>
void convert(cv::Mat &m, std::map<T,T> conv) {
    for(auto it = m.begin<T>(); it != m.end<T>(); ++it) {
        auto to = conv.find(*it);
        if(to != conv.end())
            *it = (*to).second;
    }
}
template <typename T>
void convert(cv::Mat &m, T from, T to) {
    convert(m, std::map<T,T> {{from, to}});
}

template <typename T>
void fill(cv::Mat &m, std::map<T,T> conv, int R, int C) {
    { // Check first if the pixel in (R,C) is in the domain of the conversion map
        T &pixel = m.at<T>(R,C);
        auto to = conv.find(pixel);
        if(to == conv.end()) return;
        pixel = to->second;
    }

    // q is a queue of all filled-in pixels whose neighbours are yet to be
    // checked
    std::queue< std::pair<int,int> > q;
    q.push(std::make_pair(R,C));
    while(!q.empty()) {
        int r = q.front().first,
            c = q.front().second;

        std::vector< std::pair<int,int> > adj;
        if(r > 0) adj.push_back(std::make_pair(r-1,c));
        if(c > 0) adj.push_back(std::make_pair(r,c-1));
        if(r < m.rows-1) adj.push_back(std::make_pair(r+1,c));
        if(c < m.cols-1) adj.push_back(std::make_pair(r,c+1));

        for(auto a: adj) {
            T &pixel = m.at<T>(a.first, a.second);
            auto to = conv.find(pixel);
            if(to != conv.end()) {
                pixel = to->second;
                q.push(a);
            }
        }

        q.pop();
    }
}

#endif
