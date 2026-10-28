#ifndef ACTIONMANAGER_HPP
#define ACTIONMANAGER_HPP



#include "Image.hpp"

#include <cstddef>  // std::size_t
#include <queue>
#include <stack>
#include <string>
#include <vector>

class ActionManager {
public:

    void enqueueFilters(const std::string& flags);


    void executePipeline(Image& image);

   
    std::size_t historyCount() const { return undoStack_.size(); }

private:
    /// FIFO queue of single-character filter flags.
    std::queue<char> filterQueue_;

    /// LIFO stack of pre-filter pixel-grid snapshots for undo support.
    std::stack<std::vector<std::vector<RGBTRIPLE>>> undoStack_;

    
    void saveState(const Image& image);

  
    void applyFilter(char flag, Image& image);
};

#endif // ACTIONMANAGER_HPP
