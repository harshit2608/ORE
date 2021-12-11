#include "AssetManager.h"
#include "Model.h"

namespace ORE
{
    Ref<ModelManager> ModelManager::Create(const std::string &path, float scaleFactor, bool gamma)
    {
        return CreateRef<Model>(path, scaleFactor, gamma);
    }
} // namespace ORE
