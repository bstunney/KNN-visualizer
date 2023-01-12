//
// Created by Ben Tunney on 12/9/22.
//

#ifndef ARGUMENTSETTINGS_HPP
#define ARGUMENTSETTINGS_HPP

#include <cstddef>

#include <string>

namespace neu{
    struct ArgumentSettings{

        // Populate the program arguments
        int KNeighbors;

        // Constructor
        ArgumentSettings(int argc, char* argv[]);

    };

} // end namespace neu

#endif



