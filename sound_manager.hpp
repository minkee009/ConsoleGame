#pragma once
#include <iostream>
#include <unordered_map>
#include <string>
#include <windows.h>
#include <fmod.hpp>

namespace MyGame
{
    class SoundManager {
    public:
        SoundManager() : system(nullptr), bgmChannel(nullptr), seChannel1(nullptr), seChannel2(nullptr) {
            if (FMOD::System_Create(&system) != FMOD_OK) {
                std::cerr << "FMOD 시스템 생성 실패!" << std::endl;
            }
            system->init(512, FMOD_INIT_NORMAL, nullptr);
        }

        ~SoundManager() {
            for (auto& pair : sounds) {
                pair.second->release();
            }
            system->close();
            system->release();
        }

        void LoadSound(const std::string& filename, bool isBGM = false) {
            FMOD::Sound* sound = nullptr;
            FMOD_MODE mode = isBGM ? FMOD_LOOP_NORMAL : FMOD_DEFAULT;
            if (system->createSound(filename.c_str(), mode, nullptr, &sound) == FMOD_OK) {
                sounds[filename] = sound;
            }
            else {
                std::cerr << "사운드 로드 실패: " << filename << std::endl;
            }
        }

        void LoadAllSoundsFromFolder(const std::string& folderPath, bool isBGM = false) {
            std::string searchPath = folderPath + "\\*.mp3";
            WIN32_FIND_DATAW findFileData;
            HANDLE hFind = FindFirstFileW(std::wstring(searchPath.begin(), searchPath.end()).c_str(), &findFileData);

            if (hFind != INVALID_HANDLE_VALUE) {
                do {
                    std::wstring wFilename = findFileData.cFileName;
                    std::string filename(wFilename.begin(), wFilename.end());
                    LoadSound(folderPath + "\\" + filename, isBGM);
                } while (FindNextFileW(hFind, &findFileData) != 0);
                FindClose(hFind);
            }
            else {
                std::cerr << "폴더를 열 수 없습니다: " << folderPath << std::endl;
            }
        }

        void PlayBGM(const std::string& filename) {
            auto it = sounds.find(filename);
            if (it != sounds.end()) {
                if (bgmChannel) {
                    bool isPlaying = false;
                    bgmChannel->isPlaying(&isPlaying);
                    if (isPlaying) {
                        bgmChannel->stop();
                    }
                }
                system->playSound(it->second, nullptr, false, &bgmChannel);
            }
            else {
                std::cerr << "BGM을 찾을 수 없음: " << filename << std::endl;
            }
        }

        void PlaySE(const std::string& filename, int channelNumber) {
            auto it = sounds.find(filename);
            if (it != sounds.end()) {
                FMOD::Channel** targetChannel = (channelNumber == 1) ? &seChannel1 : &seChannel2;
                if (*targetChannel) {
                    bool isPlaying = false;
                    (*targetChannel)->isPlaying(&isPlaying);
                    if (!isPlaying) {
                        *targetChannel = nullptr;
                    }
                }
                system->playSound(it->second, nullptr, false, targetChannel);
            }
            else {
                std::cerr << "사운드를 찾을 수 없음: " << filename << std::endl;
            }
        }

        void StopBGM()
        {
            if (bgmChannel) {
                bool isPlaying = false;
                bgmChannel->isPlaying(&isPlaying);
                if (isPlaying) {
                    bgmChannel->stop();
                }
            }
        }

        void Update() {
            system->update();
        }

    private:
        FMOD::System* system;
        FMOD::Channel* bgmChannel;
        FMOD::Channel* seChannel1;
        FMOD::Channel* seChannel2;
        std::unordered_map<std::string, FMOD::Sound*> sounds;
    };
}

//int main() {
//    SoundManager soundManager;
//    soundManager.LoadAllSoundsFromFolder("SE");
//    soundManager.LoadSound("bgm.mp3", true);
//    
//    std::cout << "재생할 사운드를 입력하세요 (bgm/se1/se2): ";
//    std::string type, filename;
//    std::cin >> type >> filename;
//
//    if (type == "bgm") {
//        soundManager.PlayBGM(filename);
//    } else if (type == "se1") {
//        soundManager.PlaySE(filename, 1);
//    } else if (type == "se2") {
//        soundManager.PlaySE(filename, 2);
//    } else {
//        std::cerr << "잘못된 입력!" << std::endl;
//    }
//    
//    std::cout << "Press Enter to stop..." << std::endl;
//    std::cin.ignore();
//    std::cin.get();
//    
//    return 0;
//}
