//
// Created by Ben Tunney on 12/9/22.
//
//
// Created by Ben Tunney on 12/6/22.
//
#include "ArgumentSettings.hpp"
#include <iostream>
#include <string>
#include <vector>

namespace neu {

    // Handles the program arguments for this particular program
    ArgumentSettings::ArgumentSettings(int argc, char* argv[]){

        // Ensure the programmer provided enough arguments.
        if(argc != 2){
            std::cout << "Error running program" << ":\n"
                      << "Try running:" << "\n"
                      << "e.g. ./prog 7\n"
                      << "e.g. ./prog 16\n"
                      << "Argument is number of K- neighbors (1-100)"
                      << std::endl;
            //exit(1);
        }

        else {
            std::vector<std::string> v;

            for(int i = 1;i <61; i++){
                v.push_back(std::to_string(i));
            }

            if(std::find(v.begin(), v.end(), argv[1]) != v.end()){
                KNeighbors = std::stoi(argv[1]);
            }
            else{
                std::cout << argv[1] << " is not an int value 1-60... defaulted to 20 ";
                KNeighbors = 20;
            }

        }
    }
}
