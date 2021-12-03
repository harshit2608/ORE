#pragma once

#include "../../core/Core.h"
#include "../../core/Log.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace ORE
{
    class ScenePanel
    {
    public:
        static void DrawVec3Control(const std::string &label, glm::vec3 &values, const int ID, float resetValue = 0.0f, float columnWidth = 100.0f);
    };

} // namespace ORE
