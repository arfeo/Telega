#include "VirtualKeyboardHandler.hpp"
#include <bps/event.h>
#include "bps/virtualkeyboard.h"

#include <bb/device/HardwareInfo>

#include <QObject>
#include <QDebug>

VirtualKeyboardHandler::VirtualKeyboardHandler() :
    pHeight(0),
    pVisible(false),
    pIsPhysicalKeyboardDevice(false)
{
    subscribe(virtualkeyboard_get_domain());

    if (bps_initialize() == BPS_SUCCESS) {
        if (virtualkeyboard_request_events(0) != BPS_SUCCESS) {
            qWarning("BPS: Failed to register for virtual keyboard events");
        }
    } else {
        qWarning("BPS: Failed to init");
    }
}

VirtualKeyboardHandler::~VirtualKeyboardHandler()
{
    bps_shutdown();
}

void VirtualKeyboardHandler::event(bps_event_t *event)
{
    const int eventDomain = bps_event_get_domain(event);
    if (eventDomain == virtualkeyboard_get_domain()) {
        uint16_t code = bps_event_get_code(event);
        if (code == VIRTUALKEYBOARD_EVENT_VISIBLE) {
            this->setVisible(true);
            emit this->keyboardShown();
        } else if (code == VIRTUALKEYBOARD_EVENT_HIDDEN) {
            this->setVisible(false);
            emit this->keyboardHidden();
        } else if (code == VIRTUALKEYBOARD_EVENT_INFO) {
            pHeight = virtualkeyboard_event_get_height(event);
            emit this->heightChanged();
        } else {
            qDebug() << "Unhandled keyboard event code: " << code;
        }
    }
}

int VirtualKeyboardHandler::height()
{
    return pHeight;
}

bool VirtualKeyboardHandler::visible()
{
    return pVisible;
}

void VirtualKeyboardHandler::setVisible(bool visibleState)
{
    pVisible = visibleState;
    emit this->visibleChanged();
}

bool VirtualKeyboardHandler::isPhysicalKeyboardDevice()
{
    bb::device::HardwareInfo hi;
    return hi.isPhysicalKeyboardDevice();
}

void VirtualKeyboardHandler::virtualKeyboardVisible(bool command)
{
    if (command) {
        virtualkeyboard_show();
    } else {
        virtualkeyboard_hide();
    }
}

void VirtualKeyboardHandler::toggleVisibility()
{
    if (this->visible()) {
        this->virtualKeyboardVisible(false);
    } else {
        this->virtualKeyboardVisible(true);
    }
}
