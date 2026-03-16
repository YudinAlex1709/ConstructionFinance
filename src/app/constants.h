#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>

namespace Constants {
    // Application info
    constexpr const char* APP_NAME = "Construction Finance";
    constexpr const char* APP_VERSION = "1.0.0";
    constexpr const char* APP_ORGANIZATION = "ConstructionFinance";
    
    // Database
    constexpr const char* DB_NAME = "construction_finance.db";
    constexpr int DB_VERSION = 3;
    
    // Default values
    constexpr double DEFAULT_BUDGET = 0.0;
    constexpr const char* DEFAULT_CURRENCY = "RUB";
    
    // UI
    constexpr int WINDOW_MIN_WIDTH = 1200;
    constexpr int WINDOW_MIN_HEIGHT = 800;
    constexpr int SIDEBAR_WIDTH = 260;
    
    // 3D
    constexpr float DEFAULT_CAMERA_FOV = 45.0f;
    constexpr float DEFAULT_CAMERA_NEAR = 0.1f;
    constexpr float DEFAULT_CAMERA_FAR = 1000.0f;
}

#endif // CONSTANTS_H
