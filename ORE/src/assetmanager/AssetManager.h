#pragma once

#include "../core/Core.h"
#include "../core/Log.h"
#include "../renderer/Shader.h"
#include "../renderer/GraphicManager.h"

namespace ORE
{
    class ModelManager
    {
    public:
        static Ref<ModelManager> Create(const std::string &path, float scaleFactor, bool gamma = false);
        virtual void Draw(Shader &shader) = 0;
    };

} // namespace ORE
