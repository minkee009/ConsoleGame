#pragma once
#include "IScene.hpp"
#include <unordered_map>
#include <string>

namespace MyGame
{
    class SceneManager
    {
    public:
        void CheckIsSceneChanged()
        {
            if (m_currentSceneState != m_nextSceneState)
            {
                m_currentSceneState = m_nextSceneState;
                m_currentScene = GetScene(m_currentSceneState);

                m_currentScene->Initialize();
            }
        }

        void UpdateScene()
        {
            m_currentScene->Update();
        }

        void RenderScene()
        {
            m_currentScene->Render();
        }

        void ChangeScene(SceneState state)
        {
            m_nextSceneState = state;
        }

        void AddScene(IScene* scene)
        {
            m_sceneMap[scene->GetSceneState()] = scene;
        }

        IScene* GetScene(SceneState name)
        {
            auto it = m_sceneMap.find(name);
            return (it != m_sceneMap.end()) ? it->second : nullptr;
        }

    private:
        std::unordered_map<SceneState, IScene*> m_sceneMap;

        IScene* m_currentScene;

        SceneState m_currentSceneState = None;
        SceneState m_nextSceneState = Menu;
    };
}
