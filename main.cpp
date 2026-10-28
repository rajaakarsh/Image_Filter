#include "ActionManager.hpp"
#include "Image.hpp"

#include <cstdlib>
#include <iostream>
#include <string>

namespace {

void printUsage(const std::string& programName) {
    std::cerr
        << "Usage:\n"
        << "  Batch Queue Mode:   " << programName
            << " -q <flags> <input.bmp> <output.bmp>\n"
        << "  Single Filter Mode: " << programName
            << " -g|-s|-r  <input.bmp> <output.bmp>\n"
        << "\n"
        << "Flags:\n"
        << "  g  Grayscale\n"
        << "  s  Sepia\n"
        << "  r  Reflect\n"
        << "\n"
        << "Examples:\n"
        << "  " << programName << " -q gsr  sample.bmp output.bmp\n"
        << "  " << programName << " -g      sample.bmp output.bmp\n";
}

} 

int main(int argc, char* argv[]) {
    // Argument validation 
    // Batch Queue Mode:   progname -q <flags> <in> <out>   → argc == 5
    // Single Filter Mode: progname -flag       <in> <out>  → argc == 4
    if (argc != 4 && argc != 5) {
        printUsage(argv[0]);
        return EXIT_FAILURE;
    }

    try {
        ActionManager manager;
        std::string inputFile;
        std::string outputFile;

        if (argc == 5) {
          
            const std::string mode = argv[1];
            if (mode != "-q") {
                std::cerr << "Error: expected '-q' for batch mode, got '"
                          << mode << "'\n";
                printUsage(argv[0]);
                return EXIT_FAILURE;
            }
            const std::string flags = argv[2];
            inputFile  = argv[3];
            outputFile = argv[4];

            manager.enqueueFilters(flags);

        } else {
            
            const std::string flag = argv[1];
            inputFile  = argv[2];
            outputFile = argv[3];

            if (flag == "-g") {
                manager.enqueueFilters("g");
            } else if (flag == "-s") {
                manager.enqueueFilters("s");
            } else if (flag == "-r") {
                manager.enqueueFilters("r");
            } else {
                std::cerr << "Error: unknown flag '" << flag << "'\n";
                printUsage(argv[0]);
                return EXIT_FAILURE;
            }
        }

        Image image;
        image.read(inputFile);

        manager.executePipeline(image);

        image.write(outputFile);

        std::cout << "Successfully wrote " << outputFile
                  << "  (" << manager.historyCount()
                  << " undo snapshot(s) stored)\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
