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
        SoundManager() : m_system(nullptr), m_bgmChannel(nullptr), m_seChannel1(nullptr), m_seChannel2(nullptr) {
            if (FMOD::System_Create(&m_system) != FMOD_OK) {
                std::cerr << "FMOD �ý��� ���� ����!" << std::endl;
            }
            m_system->init(512, FMOD_INIT_NORMAL, nullptr);
        }

        ~SoundManager() {
            for (auto& pair : m_sounds) {
                pair.second->release();
            }
            m_system->close();
            m_system->release();
        }

        void LoadSound(const std::string& filename, bool isBGM = false) {
            FMOD::Sound* sound = nullptr;
            FMOD_MODE mode = isBGM ? FMOD_LOOP_NORMAL : FMOD_DEFAULT;
            if (m_system->createSound(filename.c_str(), mode, nullptr, &sound) == FMOD_OK) {
                m_sounds[filename] = sound;
            }
            else {
                std::cerr << "���� �ε� ����: " << filename << std::endl;
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
                std::cerr << "������ �� �� �����ϴ�: " << folderPath << std::endl;
            }
        }

        void PlayBGM(const std::string& filename) {
            auto it = m_sounds.find(filename);
            if (it != m_sounds.end()) {
                if (m_bgmChannel) {
                    bool isPlaying = false;
                    m_bgmChannel->isPlaying(&isPlaying);
                    if (isPlaying) {
                        m_bgmChannel->stop();
                    }
                }
                m_system->playSound(it->second, nullptr, false, &m_bgmChannel);
            }
            else {
                std::cerr << "BGM�� ã�� �� ����: " << filename << std::endl;
            }
        }

        void PlaySE(const std::string& filename, int channelNumber) {
            auto it = m_sounds.find(filename);
            if (it != m_sounds.end()) {
                FMOD::Channel** targetChannel = (channelNumber == 1) ? &m_seChannel1 : &m_seChannel2;
                if (*targetChannel) {
                    bool isPlaying = false;
                    (*targetChannel)->isPlaying(&isPlaying);
                    if (!isPlaying) {
                        *targetChannel = nullptr;
                    }
                }
                m_system->playSound(it->second, nullptr, false, targetChannel);
            }
            else {
                std::cerr << "���带 ã�� �� ����: " << filename << std::endl;
            }
        }

        bool IsBGMPlaying()
        {
            bool isPlaying = false;
            m_bgmChannel->isPlaying(&isPlaying);
            return isPlaying;
        }

        void PauseBGM(bool pause)
        {
            m_bgmChannel->setPaused(pause);
        }

        void StopBGM()
        {
            if (m_bgmChannel) {
                bool isPlaying = false;
                m_bgmChannel->isPlaying(&isPlaying);
                if (isPlaying) {
                    m_bgmChannel->stop();
                }
            }
        }

        void Update() {
            m_system->update();
        }

    private:
        FMOD::System* m_system;
        FMOD::Channel* m_bgmChannel;
        FMOD::Channel* m_seChannel1;
        FMOD::Channel* m_seChannel2;
        std::unordered_map<std::string, FMOD::Sound*> m_sounds;
    };
}

//int main() {
//    SoundManager soundManager;
//    soundManager.LoadAllSoundsFromFolder("SE");
//    soundManager.LoadSound("bgm.mp3", true);
//    
//    std::cout << "����� ���带 �Է��ϼ��� (bgm/se1/se2): ";
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
//        std::cerr << "�߸��� �Է�!" << std::endl;
//    }
//    
//    std::cout << "Press Enter to stop..." << std::endl;
//    std::cin.ignore();
//    std::cin.get();
//    
//    return 0;
//}
