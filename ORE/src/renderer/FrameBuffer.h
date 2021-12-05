#pragma once

#include "../core/Core.h"
#include "../core/Log.h"

namespace ORE
{
    class FrameBuffer
    {

    public:
        FrameBuffer(/* args */);
        ~FrameBuffer();

    private:
        uint32_t m_RendererID = 0;
    };

} // namespace ORE
