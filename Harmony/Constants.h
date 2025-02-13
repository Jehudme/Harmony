#pragma once

namespace Harmony {

    // Configuration keys
    namespace Config {
        // Window-related keys
        constexpr const char* WINDOW_TITLE = "Window.Title";
        constexpr const char* WINDOW_SIZE_WIDTH = "Window.Size.Width";
        constexpr const char* WINDOW_SIZE_HEIGHT = "Window.Size.Height";
        constexpr const char* WINDOW_FULLSCREEN = "Window.Fullscreen";
        constexpr const char* WINDOW_VERTICAL_SYNC = "Window.VerticalSync";
        constexpr const char* WINDOW_FPS = "Window.FPS";

        // Object-related keys
        constexpr const char* NAME = "Name";
        constexpr const char* UNIQUE_ID = "UniqueId";

        // Rectangle-related keys
        constexpr const char* SIZE = "Size";
        constexpr const char* WIDTH = "Width";
        constexpr const char* HEIGHT = "Height";
        constexpr const char* FILL_COLOR = "FillColor";
        constexpr const char* OUTLINE_COLOR = "OutlineColor";
        constexpr const char* OUTLINE_THICKNESS = "OutlineThickness";
        constexpr const char* R = "R";
        constexpr const char* G = "G";
        constexpr const char* B = "B";
        constexpr const char* A = "A";

        // Scene-related keys
        constexpr const char* SCENE_GRAPH = "SceneGraph";
        constexpr const char* VIEW = "View";
        constexpr const char* VIEW_CENTER = "Center";
        constexpr const char* VIEW_SIZE = "Size";
        constexpr const char* VIEW_ROTATION = "Rotation";
        constexpr const char* VIEW_VIEWPORT = "Viewport";
        constexpr const char* VIEW_X = "X";
        constexpr const char* VIEW_Y = "Y";
        constexpr const char* VIEW_WIDTH = "Width";
        constexpr const char* VIEW_HEIGHT = "Height";
        constexpr const char* VIEW_LEFT = "Left";
        constexpr const char* VIEW_TOP = "Top";

        // SceneNode-related keys
        constexpr const char* POSITION = "Position";
        constexpr const char* SCALE = "Scale";
        constexpr const char* ORIGIN = "Origin";
        constexpr const char* ROTATION = "Rotation";
        constexpr const char* POSITION_VELOCITY = "PositionVelocity";
        constexpr const char* POSITION_ACCELERATION = "PositionAcceleration";
        constexpr const char* ROTATION_VELOCITY = "RotationVelocity";
        constexpr const char* ROTATION_ACCELERATION = "RotationAcceleration";
        constexpr const char* CHILDREN = "Children";
        constexpr const char* TYPE = "Type";
        constexpr const char* VECTOR_X = "X";
        constexpr const char* VECTOR_Y = "Y";

        // State-related keys
        constexpr const char* STATES = "States";
        constexpr const char* INITIAL_STATE = "InitialState";
        constexpr const char* SCRIPT = "Script";
        constexpr const char* SCENES = "Scenes";
        constexpr const char* INITIAL_SCENES = "InitialScenes";
    }

    // Default values
    namespace Default {
        // Window-related defaults
        constexpr const char* WINDOW_TITLE = "Harmony Engine";
        constexpr unsigned int WINDOW_WIDTH = 600;
        constexpr unsigned int WINDOW_HEIGHT = 600;
        constexpr bool FULLSCREEN = false;
        constexpr bool VERTICAL_SYNC = true;
        constexpr unsigned int FPS = 60;

        // Object-related defaults
        constexpr const char* NAME = "Unknown";

        // Rectangle-related defaults
        constexpr float RECTANGLE_WIDTH = 50.0f;
        constexpr float RECTANGLE_HEIGHT = 50.0f;
        constexpr int COLOR_R = 255;
        constexpr int COLOR_G = 255;
        constexpr int COLOR_B = 255;
        constexpr int COLOR_A = 255;
        constexpr int OUTLINE_COLOR_R = 0;
        constexpr int OUTLINE_COLOR_G = 0;
        constexpr int OUTLINE_COLOR_B = 0;
        constexpr int OUTLINE_COLOR_A = 255;
        constexpr float OUTLINE_THICKNESS = 0.0f;
    }

    // Error messages
    namespace Error {
        constexpr const char* NO_CURRENT_STATE = "No current state available";
        constexpr const char* NAME_ALREADY_IN_USE = "Name already in use";
        constexpr const char* NULL_CONFIGURATION = "Configuration cannot be null.";
        constexpr const char* UNKNOWN_TYPE = "Unknown type: ";
        constexpr const char* SCENE_NOT_FOUND = "Scene not found: ";
        constexpr const char* NULL_SCENE = "Scene cannot be null.";
        constexpr const char* NO_STATES_FOUND = "No States Found";
        constexpr const char* STATE_NOT_FOUND = "State not found";
        constexpr const char* FILE_PATH_NOT_SET = "File path is not set.";
        constexpr const char* FILE_OPEN_FAILED = "Failed to open file: ";
        constexpr const char* FILE_WRITE_FAILED = "Failed to open file for writing: ";
        constexpr const char* DYNAMIC_CAST_FAILED = "Dynamic pointer cast failed";
        constexpr const char* OBJECT_NOT_FOUND = "Object not found";
    }
}