//
//  Score.hpp
//  Websocket Game Server
//
//  Created by Avi Miller on 2/15/18.
//  Copyright © 2018 Avi Miller. All rights reserved.
//

#ifndef Score_hpp
#define Score_hpp

#include <stdio.h>
#include <vector>
#include <string>

#define DEFAULT_SCORE_NAME "None"

class Score{
public:
    Score(){scoreID = currentID++; scores.push_back(this);}
    
    int score = 0;
    std::string ownerName = DEFAULT_SCORE_NAME;
    int getID(){return scoreID;}
    
    static std::vector<Score*> getScores(){return Score::scores;}
private:
    int scoreID;
    
    static int currentID;
    static std::vector<Score*> scores;
};

#endif /* Score_hpp */
