#include "stdafx.h"
#include "CGestureListener.h"
#include "CGestureMatcher.h"

void ClearConsole()
{
    HANDLE l_console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO l_csbi;
    if(!GetConsoleScreenBufferInfo(l_console, &l_csbi))
    {
        return;
    }

    COORD l_coordScreen = { 0, 0 };
    DWORD l_charsWritten;
    DWORD l_conSize = l_csbi.dwSize.X * l_csbi.dwSize.Y;
    if(!FillConsoleOutputCharacter(l_console, (TCHAR) ' ', l_conSize, l_coordScreen, &l_charsWritten)) return;
    if(!GetConsoleScreenBufferInfo(l_console, &l_csbi))
    {
        return;
    }
    if(!FillConsoleOutputAttribute(l_console, l_csbi.wAttributes, l_conSize, l_coordScreen, &l_charsWritten)) return;
    SetConsoleCursorPosition(l_console, l_coordScreen);
}

void CGestureListener::onInit(const Leap::Controller&)
{
    std::cout << "Initialized" << std::endl;
}

void CGestureListener::onConnect(const Leap::Controller&)
{
    std::cout << "Connected" << std::endl;
}

void CGestureListener::onDisconnect(const Leap::Controller&)
{
    // Note: not dispatched when running in a debugger.
    std::cout << "Disconnected" << std::endl;
}

void CGestureListener::onExit(const Leap::Controller&)
{
    std::cout << "Exited" << std::endl;
}

void CGestureListener::onFrame(const Leap::Controller& controller)
{
    ClearConsole();

    Leap::Frame l_frame = controller.frame();
    std::vector<float> l_scoresLeft, l_scoresRight;
    CGestureMatcher::GetGestures(l_frame, CGestureMatcher::WH_LeftHand, l_scoresLeft);
    CGestureMatcher::GetGestures(l_frame, CGestureMatcher::WH_RightHand, l_scoresRight);
    for(size_t i = 0U; i < CGestureMatcher::GT_GesturesCount; i++)
    {
        std::string l_gestureName;
        CGestureMatcher::GetGestureName(static_cast<CGestureMatcher::GestureType>(i),l_gestureName);
        if(!l_gestureName.empty()) fprintf(stderr, "%-24s -> %.4f\t%.4f\n", l_gestureName.c_str(), l_scoresLeft[i],l_scoresRight[i]);
    }
}

void CGestureListener::onFocusGained(const Leap::Controller&)
{
    std::cout << "Focus Gained" << std::endl;
}

void CGestureListener::onFocusLost(const Leap::Controller&)
{
    std::cout << "Focus Lost" << std::endl;
}

void CGestureListener::onDeviceChange(const Leap::Controller& controller)
{
    std::cout << "Device changed:" << std::endl;
    const Leap::DeviceList l_devices = controller.devices();
    for(auto l_device : l_devices)
    {
        std::cout << "\tid: " << l_device.toString();
        std::cout << ", isStreaming " << (l_device.isStreaming() ? "true" : "false");
        std::cout << ", isSmudged " << (l_device.isSmudged() ? "true" : "false");
        std::cout << ", isLightingBad " << (l_device.isLightingBad() ? "true" : "false") << std::endl;
    }
}

void CGestureListener::onServiceConnect(const Leap::Controller&)
{
    std::cout << "Service Connected" << std::endl;
}

void CGestureListener::onServiceDisconnect(const Leap::Controller&)
{
    std::cout << "Service Disconnected" << std::endl;
}

void CGestureListener::onServiceChange(const Leap::Controller&)
{
    std::cout << "Service Changed" << std::endl;
}

void CGestureListener::onDeviceFailure(const Leap::Controller& controller)
{
    std::cout << "Device error:" << std::endl;
    const Leap::FailedDeviceList l_devices = controller.failedDevices();
    for(auto l_device : l_devices)
    {
        std::cout << "\tPNP ID " << l_device.pnpId();
        std::cout << ", failure type " << l_device.failure() << std::endl;;
    }
}

void CGestureListener::onLogMessage(const Leap::Controller&, Leap::MessageSeverity severity, int64_t timestamp, const char* msg)
{
    switch(severity)
    {
        case Leap::MESSAGE_CRITICAL:
            std::cout << "[Critical]";
            break;
        case Leap::MESSAGE_WARNING:
            std::cout << "[Warning]";
            break;
        case Leap::MESSAGE_INFORMATION:
            std::cout << "[Info]";
            break;
        case Leap::MESSAGE_UNKNOWN:
            std::cout << "[Unknown]";
    }
    std::cout << '[' << timestamp << "] " << msg << std::endl;
}