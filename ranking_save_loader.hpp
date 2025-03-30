#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <locale>
#include <codecvt>

namespace MyGame {

    struct Ranking {
        std::string name;
        int score;
    };

    inline std::vector<Ranking> LoadRankingFromFile(const std::string& filename) {
        std::vector<Ranking> rankings;
        std::ifstream inFile(filename);
        if (!inFile)
            return rankings; // 빈 벡터 반환

        std::string line;
        while (std::getline(inFile, line)) {
            std::istringstream iss(line);
            std::string name;
            int score;
            if (std::getline(iss, name, ',') && (iss >> score)) {
                rankings.push_back({ name, score });
            }
        }

        inFile.close();

        // 점수 기준으로 정렬
        std::sort(rankings.begin(), rankings.end(), [](const Ranking& a, const Ranking& b) {
            return b.score < a.score; // 내림차순 정렬
            });

        return rankings;
    }

    inline void SaveRankingToFile(const std::string& filename, const std::vector<Ranking>& rankings) {
        std::ofstream outFile(filename);
        if (!outFile)
            return;


        for (const auto& rank : rankings) {
            outFile << rank.name << "," << rank.score << "\n";
        }

        outFile.close();
    }

}