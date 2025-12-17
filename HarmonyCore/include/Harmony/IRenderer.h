#pragma once

#include "Math.h" // Assumes Vector2, Vector3, Matrix4, Color, Quaternion
#include <entt/entt.hpp>      // Required for passing the Registry
#include <string>
#include <vector>

namespace Harmony {

    // =========================================================================
    // 1. RENDERER DATA STRUCTURES
    // =========================================================================

    using ResourceID = size_t;

    struct TextureHandle {
        ResourceID id = 0;
        bool IsValid() const { return id != 0; }
    };

    struct CameraData {
        Vector3 position;
        Vector3 target;
        Vector3 up = { 0.0f, 1.0f, 0.0f };
        float fov = 45.0f;
        float aspectRatio = 16.0f / 9.0f;
        bool isOrthographic = false;
    };

    struct LightData {
        Vector3 position;
        Color color = { 255, 255, 255, 255 };
        float intensity = 1.0f;
        float radius = 10.0f;
    };

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

        virtual void RenderScene(const entt::registry& registry, const CameraData& mainCamera) = 0;

        virtual void Begin2D() = 0;
        virtual void End2D() = 0;

        virtual void DrawSprite(const TextureHandle& texture, const Vector2& position, const Vector2& size, const Color& tint) = 0;
        virtual void DrawTextStr(const std::string& text, const Vector2& position, float fontSize, const Color& color) = 0;

        // ---------------------------------------------------------------------
        // DEBUG RENDERING
        // ---------------------------------------------------------------------
        // Essential for visualizing physics colliders, raycasts, and AI paths.

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