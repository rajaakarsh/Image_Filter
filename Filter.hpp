#ifndef FILTER_HPP
#define FILTER_HPP



#include "Image.hpp"

class Filter {
public:
    Filter() = delete;   

 
    static void grayscale(Image& image);

   
    static void sepia(Image& image);

    
    static void reflect(Image& image);
};

#endif // FILTER_HPP
