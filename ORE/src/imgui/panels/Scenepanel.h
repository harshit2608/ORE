#pragma once

#include "../../core/Core.h"
#include "../../core/Log.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ImGuiFileDialog.h"

namespace ORE
{
    class ScenePanel
    {
    public:
        static void DrawVec3Control(const std::string &label, glm::vec3 &values, const int ID, float resetValue = 0.0f, float columnWidth = 100.0f);

        void drawGui();
        const std::string &GetFilePath() const { return m_FilePath; }

    private:
        std::string m_FilePath = "assets/models/zorkicamera/source/RC_zorki_Reduced/ZORKI_LENS.obj";
    };

} // namespace ORE
