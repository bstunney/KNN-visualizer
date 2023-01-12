//
// Created by Ben Tunney on 12/6/22.
//

// C++ Standard Libraries
#include <iostream>
// Third Party Libraries
#include <SFML/Graphics.hpp>

#include <vector>
#include <fstream>
#include <string>
#include <math.h>
#include <random>

#include "ArgumentSettings.hpp"

// to compile: g++ -std=c++17 -g ./src/*.cpp -o prog -I./include -lsfml-graphics -lsfml-window -lsfml-system -lpthread
// to run: ./prog 25

std::vector<std::string> LoadTxt(const std::string filename){

    // define vector, myline, and myfile for reading in file
    std::vector<std::string> text;
    std::ifstream myfile (filename);
    std::string myline;

    // if conditional to check if file opened correctly
    if ( myfile.is_open() ) {

        // read all lines from file into vector
        while ( myfile ) {
            std::getline(myfile, myline);
            text.push_back(myline);
        }
    }

    // return filled vector
    return text;
}

std::vector<std::vector<std::string> > CleanData(std::vector<std::string> text, std::vector<int> indices){

    // make char for quote
    std::string quotation;
    quotation += '"';

    // make vector to store rows of data
    std::vector<std::vector<std::string> > data;

    // for each row x in data
    for (std::string x: text){

        // set bool for if there are empty vals in row
        bool containsEmpty = false;

        // make vector for current line's data
        std::vector <std::string> row;

        // make string for each line of text
        std::string line = x;

        // string for first piece of info in row
        std::string info = "";

        // bool val to ensure not splitting on commas between quotes
        bool WithinQuotes = false;

        // for each character in line
        for (auto y: line) {

            // get char
            std::string this_char;
            this_char = y;

            // flip bool value every time entering/leaving quotes
            if (this_char == quotation){
                WithinQuotes = !WithinQuotes;
            }

            // if char is a comma
            if (this_char == "," and WithinQuotes == false) {

                // push back current stored info
                row.push_back(info);


                // start new info
                info = "";
            }

                // if char
            else {

                // add char to info
                info = info + y;
            }
        }

        // add last coord to coords
        row.push_back(info);

        // check if there was an empty val in indices of interest
        for(int index: indices){
            if (row[index] == "") {
                containsEmpty = true;
            }
        }

        // add row to node vector if there were no empty vals
        if (containsEmpty != true) {
            data.push_back(row);
        }
    }

    // delete headers
    data.erase(data.begin());
    return data;
}

float GetEuclidean(float x1, float y1, float x2, float y2){

    // return euclid distance between two points
    return pow((pow((x1 - x2) ,2) + pow((y1 - y2) , 2)) , 0.5);
}

std::vector <float> GetMinMax(std::vector<std::vector<std::string> > data, std::vector<int> indices, std::string minmax){

    // make vectors to store maxes and mins for each of the features of data in indices
    std::vector<float> minsmaxes;

    // for each index of interest
    for(int i = 0; i < indices.size(); i++) {

        float standard;

        // set super small max and super high min
        if(minmax == "min"){
            standard = INFINITY;
        }
        else if(minmax == "max"){
            standard = -INFINITY;
        }

        // for each row of data
        for (auto row: data) {

            float val;
            val = std::stof(row[indices[i]]);
            //float val = row[indices[i]];

            if(minmax == "min") {
                // if data at that index is less than stored min
                if (val < standard) {

                    // store value as new min
                    standard = val;
                }
            }

            if(minmax == "max") {
                // if data at that index is less than stored min
                if (val > standard) {

                    // store value as new min
                    standard = val;
                }
            }
        }

        // add min or max points to vector
        if(minmax == "min") {
            minsmaxes.push_back(standard);
        }
        if(minmax == "max") {
            minsmaxes.push_back(standard);
        }
    }
    return minsmaxes;
}

std::vector<std::vector<std::string> > PrepData(std::vector<std::vector<std::string> > data, std::vector<int> indices){

    // make vectors to store maxes and mins for each of the features of data in indices
    std::vector<float> mins = GetMinMax(data, indices, "min");
    std::vector<float> maxes = GetMinMax(data, indices, "max");

    // for each index of interest (2)
    for(int i = 0; i < indices.size(); i++) {

        // for each row of data (all rows)
        for (int x = 0; x<  data.size(); x++) {

            // get val of row's index of interest
            float val = std::stof(data[x][indices[i]]);

            // row and indices specified column set equal to scaled min-max val
            data[x][indices[i]] = std::to_string((val - mins[i]) / (maxes[i] - mins[i]));
        }
    }

    return data;
}

std::vector <int> GetIndices(std::vector <std::string> text, std::vector <std::string> desired_indices){

    // make vector to store int, indices
    std::vector<int> indices;

    // make vector to store header strings
    std::vector<std::string> headers;

    // make string for header
    std::string header = text[0];

    // string for first piece of info in row
    std::string info = "";

    // for each character in headers
    for (auto y: header) {

        // get char
        std::string this_char;
        this_char = y;

        // if char is a comma
        if (this_char == ",") {

            // push back current stored info
            headers.push_back(info);

            // start new info
            info = "";
        }

        // if char
        else {
            // add char to info
            info = info + y;
        }
    }

    // add last coord to coords
    headers.push_back(info);

    // for each header
    for(int i = 0; i < headers.size(); i++){

        // for each desired index
        for(auto desired_index: desired_indices){

            // check if current header equals any of the desired headers
            if(headers[i]== desired_index){

                // store header indices of interest
                indices.push_back(i);
            }
        }
    }

    return indices;
}

std::vector<std::vector<std::string> > GetEuclids(std::vector<std::vector<std::string> > TrainData,
                                                  std::vector<std::string> TestPoint,
                                                  std::vector<int> TestIndices, std::vector<int> TrainIndices, int K){

    // make vector to store k nearest points to point requested
    std::vector<std::vector<std::string> > Nearest;

    // copy all of training data to nearest vector
    for(auto TrainPoint: TrainData){
        Nearest.push_back(TrainPoint);
    }

    // iterate through each training point
    for(int i = 0; i < Nearest.size(); i ++){
        float x1 = std::stof(TestPoint[TestIndices[0]]);
        float y1 = std::stof(TestPoint[TestIndices[1]]);
        float x2 = std::stof(Nearest[i][TrainIndices[0]]);
        float y2 = std::stof(Nearest[i][TrainIndices[1]]);
        float euclid = GetEuclidean(x1, y1, x2, y2);
        Nearest[i].push_back(std::to_string(euclid));
    }


    // https://stackoverflow.com/questions/45494567/c-how-to-sort-the-rows-of-a-2d-vector-by-the-values-in-each-rows-column
    // sort nearest vector by euclid value
    std::sort(Nearest.begin(),
              Nearest.end(),
              [] (const std::vector<std::string> &Point1, const std::vector<std::string> &Point2)
              {
                  return std::stof(Point1.back()) < std::stof(Point2.back());
              });


    std::vector<std::vector<std::string> > KNearest;

    // copy all of training data to nearest vector
    for(int i = 0; i < K +1; i ++){
        KNearest.push_back(Nearest[i]);
    }

    return KNearest;
}

// Entry point into the program
int main(int argc, char* argv[]) {

    // check arg settings
    neu::ArgumentSettings args(argc, argv);

    // set k neighbors based off of arguments
    int KNeighbors = args.KNeighbors;

    // get text data
    std::vector <std::string> TrainText = LoadTxt("train.csv");
    std::vector <std::string> TestText = LoadTxt("test.csv");

    // get indices of features of interest
    std::vector <std::string> desired_indices{"Age", "Fare"};
    std::vector <int> TestIndices = GetIndices(TestText, desired_indices);
    std::vector <int> TrainIndices = GetIndices(TrainText, desired_indices);

    // put text data into vectors
    std::vector<std::vector<std::string> > TrainData = CleanData(TrainText, TrainIndices);
    std::vector<std::vector<std::string> > TestData = CleanData(TestText, TestIndices);

    // get vectors of clean train and test data
    std::vector<std::vector<std::string> > CleanTest = PrepData(TestData, TestIndices);
    std::vector<std::vector<std::string> > CleanTrain = PrepData(TrainData, TrainIndices);

    // get index for name label
    std::vector <std::string> name_label{"Name"};
    std::vector <int> name_index = GetIndices(TestText, name_label);


    // Create an SFML Window
    // This window allows us to 'render graphics' onto a Window.
    sf::RenderWindow window(sf::VideoMode(600, 600), "Assignment 12!");


    // This is an infinite loop
    // This loop will continuously run our application
    // until we receive an event to close the window.

    // set k index to 900 (nonsense number)
    int KIndex = 900;

    // get index for survived label
    std::vector <std::string> category_label{"Survived"};
    std::vector <int> category_index = GetIndices(TrainText, category_label);

    // load font file
    sf::Font font;
    if (!font.loadFromFile("OpenSans-Regular.ttf"))
    {
        // error...
    }

    // make bool for if user has clicked plot
    bool hasClicked = false;

    while (window.isOpen()){

        // Within our loop (local scope) we create
        // an event. It is the first thing that we
        // do.
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                window.close();
            }
        }



        // Handle mouse clicks
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){

            // Retrieve the position of the mouse
            sf::Vector2i position = sf::Mouse::getPosition(window);

            // if pressing the plot
            if(position.x <550 and position.x>50 and position.y >50 and position.y<550) {

                hasClicked = true;
                // get random test data point
                int range = CleanTest.size() - 0 + 1;
                KIndex = rand() % range;
            }
        }
        else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)){
        }

        // Clear the window at the start of every frame
        window.clear();

        // make white background
        sf::RectangleShape rectangle(sf::Vector2f(600, 600));
        rectangle.setFillColor(sf::Color(255, 255, 255));
        window.draw(rectangle);

        // make plot gray rect
        sf::RectangleShape plot(sf::Vector2f(503, 500));
        plot.setFillColor(sf::Color(220,220,220));
        plot.setPosition(50,50);
        window.draw(plot);

        // make xaxis line
        sf::RectangleShape xaxis(sf::Vector2f(3, 500));
        xaxis.setPosition(47,50);
        xaxis.setFillColor(sf::Color::Black);
        window.draw(xaxis);

        // make vertical .25, .5, .75, and 1 lines
        sf::RectangleShape x25line(sf::Vector2f(1, 500));
        x25line.setPosition(176.5,50);
        x25line.setFillColor(sf::Color::White);
        window.draw(x25line);

        sf::RectangleShape x50line(sf::Vector2f(1, 500));
        x50line.setPosition(301.5,50);
        x50line.setFillColor(sf::Color::White);
        window.draw(x50line);

        sf::RectangleShape x75line(sf::Vector2f(1, 500));
        x75line.setPosition(426.5,50);
        x75line.setFillColor(sf::Color::White);
        window.draw(x75line);

        sf::RectangleShape x100line(sf::Vector2f(1, 500));
        x100line.setPosition(551.5,50);
        x100line.setFillColor(sf::Color::White);
        window.draw(x100line);

        // make y axis line
        sf::RectangleShape yaxis(sf::Vector2f(506, 3));
        yaxis.setPosition(47,550);
        yaxis.setFillColor(sf::Color::Black);
        window.draw(yaxis);

        // make horizontal .25, .5, .75, and 1 lines
        sf::RectangleShape y75line(sf::Vector2f(500, 1));
        y75line.setPosition(50,176.5);
        y75line.setFillColor(sf::Color::White);
        window.draw(y75line);

        sf::RectangleShape y50line(sf::Vector2f(500, 1));
        y50line.setPosition(50,301.5);
        y50line.setFillColor(sf::Color::White);
        window.draw(y50line);

        sf::RectangleShape y100line(sf::Vector2f(500, 1));
        y100line.setPosition(50,51.5);
        y100line.setFillColor(sf::Color::White);
        window.draw(y100line);

        sf::RectangleShape y25line(sf::Vector2f(500, 1));
        y25line.setPosition(50,426.5);
        y25line.setFillColor(sf::Color::White);
        window.draw(y25line);

        // make box for legend
        sf::RectangleShape legend(sf::Vector2f(90, 55));
        legend.setPosition(460,60);
        legend.setFillColor(sf::Color::White);
        legend.setOutlineThickness(2.5);
        legend.setOutlineColor(sf::Color(136, 139, 141));
        window.draw(legend);

        // make legend title
        std::string legendtitletext = "Legend";
        sf::Text legendtitle(legendtitletext, font);
        legendtitle.setCharacterSize(12);
        legendtitle.setFillColor(sf::Color::Black);
        legendtitle.setPosition(463,63);
        legendtitle.setStyle(sf::Text::Regular);
        window.draw(legendtitle);

        // make durvived label
        std::string cat1text = "= Survived";
        sf::Text cat1(cat1text, font);
        cat1.setCharacterSize(9);
        cat1.setFillColor(sf::Color::Black);
        cat1.setPosition(475,80);
        cat1.setStyle(sf::Text::Regular);
        window.draw(cat1);

        // point for survived
        float x = 465;
        float y = 82;
        std::pair<float, float> p = std::make_pair(x, y);
        sf::CircleShape shape(3.0f);
        shape.setFillColor(sf::Color::Green);
        shape.setPosition(p.first, p.second);
        window.draw(shape);

        // make did not survive label
        std::string cat2text = "= Did Not Survive";
        sf::Text cat2(cat2text, font);
        cat2.setCharacterSize(9);
        cat2.setFillColor(sf::Color::Black);
        cat2.setPosition(475,92);
        cat2.setStyle(sf::Text::Regular);
        window.draw(cat2);

        // make did not survive point
        x = 465;
        y = 94;
        p = std::make_pair(x, y);
        sf::CircleShape shape1(3.0f);
        shape1.setFillColor(sf::Color::Blue);
        shape1.setPosition(p.first, p.second);
        window.draw(shape1);

        // make random test label
        std::string randpointtext = "= Rand. Test Point";
        sf::Text randpoint(randpointtext, font);
        randpoint.setCharacterSize(9);
        randpoint.setFillColor(sf::Color::Black);
        randpoint.setPosition(475,104);
        randpoint.setStyle(sf::Text::Regular);
        window.draw(randpoint);

        // make random test point
        x = 465;
        y = 106;
        p = std::make_pair(x, y);
        sf::CircleShape shape2(3.0f);
        shape2.setFillColor(sf::Color::Red);
        shape2.setPosition(p.first, p.second);
        window.draw(shape2);


        // For each training point
        for(auto point: CleanTrain){

            // get x, y pos of point
            float x,y;
            x = std::stof(point[TrainIndices[0]]) * 500 + 50;
            y = 550 - (std::stof(point[TrainIndices[1]]) * 500);
            std::pair<float,float> p  = std::make_pair(x,y);

            // Draw a shape
            sf::CircleShape shape(1.70f);
            shape.setFillColor(sf::Color(100,71,0));
            shape.setPosition(p.first, p.second);
            window.draw(shape);
        }

        // if plot has been clicked
        if(KIndex != 900) {

            // get K nearest neighbors for point of interest
            std::vector <std::vector<std::string>> euclids = GetEuclids(CleanTrain, CleanTest[KIndex],
                                                                        TestIndices, TrainIndices, 34);

            // get x, y position of red point of interest
            float x, y;
            x = std::stof(CleanTest[KIndex][TestIndices[0]]) * 500 + 50;
            y = 550 - (std::stof(CleanTest[KIndex][TestIndices[1]]) * 500);
            std::pair<float, float> p = std::make_pair(x, y);

            // Draw a shape
            sf::CircleShape shape(2.70f);
            shape.setFillColor(sf::Color::Red);
            shape.setPosition(p.first, p.second);
            window.draw(shape);

            // make line for point label
            sf::RectangleShape pointline(sf::Vector2f(50, 2));
            pointline.setPosition(p.first + 2, p.second + 2);
            pointline.setRotation(305.0);
            pointline.setFillColor(sf::Color(186, 189, 191));
            window.draw(pointline);

            // make box for point label
            sf::RectangleShape pointrect(sf::Vector2f(75, 50));
            pointrect.setPosition(p.first + 29, p.second - 41 - 46);
            pointrect.setFillColor(sf::Color::White);
            pointrect.setOutlineThickness(2.5);
            pointrect.setOutlineColor(sf::Color(136, 139, 141));
            window.draw(pointrect);

            // make name label for point label
            std::string nametext;
            nametext += name_label[0];
            nametext += ": ";
            nametext += (CleanTest[KIndex][name_index[0]].substr(1,6));
            nametext += "...";
            sf::Text name(nametext, font);
            name.setCharacterSize(10);
            name.setFillColor(sf::Color::Black);
            name.setPosition(p.first + 31, p.second - 41 - 46);
            name.setStyle(sf::Text::Regular);
            window.draw(name);

            // make feat 1 label
            std::string f1text = desired_indices[0];
            f1text += ": ";
            f1text += std::to_string(std::stof(CleanTest[KIndex][TestIndices[0]]));
            sf::Text f1(f1text, font);
            f1.setCharacterSize(10);
            f1.setFillColor(sf::Color::Black);
            f1.setPosition(p.first + 31, p.second - 41 - 33);
            f1.setStyle(sf::Text::Regular);
            window.draw(f1);

            // make feat 2 label
            std::string f2text = desired_indices[1];
            f2text += ": ";
            f2text += std::to_string(std::stof(CleanTest[KIndex][TestIndices[1]]));
            sf::Text f2(f2text, font);
            f2.setCharacterSize(10);
            f2.setFillColor(sf::Color::Black);
            f2.setPosition(p.first + 31, p.second - 41 - 20);
            f2.setStyle(sf::Text::Regular);
            window.draw(f2);

            // counters for cat 1 and 2
            int count0 = 0;
            int count1 = 0;

            // for each k nearest neighbor
            for (auto KNeighbor: euclids) {

                // get x, y pos of point
                float x, y;
                x = std::stof(KNeighbor[TrainIndices[0]]) * 550 + 50;
                y = 550 - (std::stof(KNeighbor[TrainIndices[1]]) * 550);
                std::pair<float, float> p = std::make_pair(x, y);

                // Draw a shape
                sf::CircleShape shape(2.30f);

                // color depending on category and count for each category
                if (KNeighbor[category_index[0]] == "0") {
                    shape.setFillColor(sf::Color::Blue);
                    count0 += 1;
                } else {
                    shape.setFillColor(sf::Color::Green);
                    count1 += 1;
                }
                shape.setPosition(p.first, p.second);
                window.draw(shape);
            }

            // make category label for red point
            std::string cattext = category_label[0];
            cattext += ": ";
            if (count0 > count1) {
                cattext += "No";
            }
            else{
                cattext += "Yes";
            }
            sf::Text cat(cattext, font);
            cat.setCharacterSize(10);
            cat.setFillColor(sf::Color::Black);
            cat.setPosition(p.first + 31, p.second - 41 - 8);
            cat.setStyle(sf::Text::Regular);
            window.draw(cat);
        }

        // make plot title
        std::string title = "K-Nearest Neighbors - ";
        title += std::to_string(KNeighbors);
        title += " neighbors";
        sf::Text Title(title, font);
        Title.setCharacterSize(16);
        Title.setFillColor(sf::Color::Black);
        Title.setPosition(160, 20);
        Title.setStyle(sf::Text::Regular);
        window.draw(Title);

        // make xaxis title
        std::string xtitle = desired_indices[0];
        xtitle += " (scale normalized)";
        sf::Text XTitle(xtitle, font);
        XTitle.setCharacterSize(15);
        XTitle.setFillColor(sf::Color::Black);
        XTitle.setPosition(220, 570);
        XTitle.setStyle(sf::Text::Regular);
        window.draw(XTitle);

        // make yaxis title
        std::string ytitle = desired_indices[1];
        ytitle += " (scale normalized)";
        sf::Text YTitle(ytitle, font);
        YTitle.setCharacterSize(15);
        YTitle.setFillColor(sf::Color::Black);
        YTitle.setPosition(7, 370);
        YTitle.setRotation(270.0);
        YTitle.setStyle(sf::Text::Regular);
        window.draw(YTitle);

        // draw x index labels
        sf::Text zero("0", font);
        zero.setCharacterSize(10);
        zero.setFillColor(sf::Color::Black);
        zero.setPosition(45, 555);
        zero.setStyle(sf::Text::Regular);
        window.draw(zero);

        sf::Text p25("0.25", font);
        p25.setCharacterSize(10);
        p25.setFillColor(sf::Color::Black);
        p25.setPosition(170, 555);
        p25.setStyle(sf::Text::Regular);
        window.draw(p25);

        sf::Text p50("0.5", font);
        p50.setCharacterSize(10);
        p50.setFillColor(sf::Color::Black);
        p50.setPosition(295, 555);
        p50.setStyle(sf::Text::Regular);
        window.draw(p50);

        sf::Text p75("0.75", font);
        p75.setCharacterSize(10);
        p75.setFillColor(sf::Color::Black);
        p75.setPosition(420, 555);
        p75.setStyle(sf::Text::Regular);
        window.draw(p75);

        sf::Text p100("1", font);
        p100.setCharacterSize(10);
        p100.setFillColor(sf::Color::Black);
        p100.setPosition(545, 555);
        p100.setStyle(sf::Text::Regular);
        window.draw(p100);

        // make y index labels
        sf::Text yp25("0.25", font);
        yp25.setCharacterSize(10);
        yp25.setFillColor(sf::Color::Black);
        yp25.setPosition(25, 420);
        yp25.setStyle(sf::Text::Regular);
        window.draw(yp25);

        sf::Text yp50("0.5", font);
        yp50.setCharacterSize(10);
        yp50.setFillColor(sf::Color::Black);
        yp50.setPosition(32, 298);
        yp50.setStyle(sf::Text::Regular);
        window.draw(yp50);

        sf::Text yp75("0.75", font);
        yp75.setCharacterSize(10);
        yp75.setFillColor(sf::Color::Black);
        yp75.setPosition(25, 170);
        yp75.setStyle(sf::Text::Regular);
        window.draw(yp75);

        sf::Text y1("1", font);
        y1.setCharacterSize(10);
        y1.setFillColor(sf::Color::Black);
        y1.setPosition(35, 50);
        y1.setStyle(sf::Text::Regular);
        window.draw(y1);


        // if user has not clicked yet, display open message
        if(hasClicked == false){

            // make box for into
            sf::RectangleShape intro(sf::Vector2f(400, 200));
            intro.setPosition(100,100);
            intro.setFillColor(sf::Color::White);
            intro.setOutlineThickness(2.5);
            intro.setOutlineColor(sf::Color(136, 139, 141));
            window.draw(intro);

            // make intro title
            std::string introtitletext = "Welcome to KNN Visualizer!";
            sf::Text introtitle(introtitletext, font);
            introtitle.setCharacterSize(30);
            introtitle.setFillColor(sf::Color::Black);
            introtitle.setPosition(108,120);
            introtitle.setStyle(sf::Text::Regular);
            window.draw(introtitle);

            // make intro text1
            std::string intro1text = "- Click anywhere on plot to visualize k nearest";
            sf::Text intro1(intro1text, font);
            intro1.setCharacterSize(16);
            intro1.setFillColor(sf::Color::Black);
            intro1.setPosition(125,170);
            intro1.setStyle(sf::Text::Regular);
            window.draw(intro1);

            // make intro text2
            std::string intro2text = "  points and KNN prediction for randomly ";
            sf::Text intro2(intro2text, font);
            intro2.setCharacterSize(16);
            intro2.setFillColor(sf::Color::Black);
            intro2.setPosition(125,190);
            intro2.setStyle(sf::Text::Regular);
            window.draw(intro2);

            // make intro text3
            std::string intro3text = "  generated test points from Titanic dataset";
            sf::Text intro3(intro3text, font);
            intro3.setCharacterSize(16);
            intro3.setFillColor(sf::Color::Black);
            intro3.setPosition(125,210);
            intro3.setStyle(sf::Text::Regular);
            window.draw(intro3);
        }

        // Display to the window
        window.display();
    }

    return 0;
}






