I created this KNN algorithm visualization project during December 2022 for my programming in C++ Final Project.

Once SFML graphics is downloaded, program can be ran in the command line.

To compile: g++ -std=c++17 -g ./src/*.cpp -o prog -I./include -lsfml-graphics -lsfml-window -lsfml-system -lpthread
To run: ./prog 25 (parameter specifies number of K-neighbors to visualize)

This visualization visualizes the K-Nearest Neighbors Classification process. After clicking anywhere on the screen, a random test point will be displayed in red and the K nearest points will be displayed in red or green depending on their category. 
KNN then takes the majority vote of those K-Nearest Neighbors and will predict it as such. The visualization uses the popup to display that prediction accordingly.


required downloads: SFML graphics
