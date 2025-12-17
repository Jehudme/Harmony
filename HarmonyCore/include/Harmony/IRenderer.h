#pragma once

#include "Math.h" 
#include <string>
#include <vector>
#include <entt/entt.hpp>

namespace Harmony {

    // =========================================================================
    // 1. RENDERER DATA STRUCTURES
    // =========================================================================


    // =========================================================================
    // 2. THE RENDERER INTERFACE
    // =========================================================================

    class IRenderer {
    public:
        virtual ~IRenderer() = default;

        // ---------------------------------------------------------------------
        // LIFECYCLE
        // ---------------------------------------------------------------------
        virtual void Initialize() = 0;
        virtual void Shutdown() = 0;

        // Update viewport/internal buffers when window resizes
        virtual void OnResize(int width, int height) = 0;

        // ---------------------------------------------------------------------
        // MAIN RENDERING
        // ---------------------------------------------------------------------

        virtual void RenderScene(const entt::registry& registry) = 0;

        virtual void Begin2D() = 0;
        virtual void End2D() = 0;

        // ---------------------------------------------------------------------
        // DEBUG RENDERING
        // ---------------------------------------------------------------------

        virtual void DrawLine(const Vector3& start, const Vector3& end, const Color& color) = 0;
        virtual void DrawCubeWireframe(const Vector3& center, const Vector3& size, const Color& color) = 0;
        virtual void DrawSphereWireframe(const Vector3& center, float radius, const Color& color) = 0;
        virtual void DrawGrid(int slices, float spacing) = 0;

        // ---------------------------------------------------------------------
        // GLOBAL STATE
        // ---------------------------------------------------------------------
        virtual void SetClearColor(const Color& color) = 0;
        virtual void SetWireframeMode(bool enabled) = 0;
    };
}