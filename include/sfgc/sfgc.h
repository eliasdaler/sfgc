#ifndef SFGC_H
#define SFGC_H

#pragma once

#include <array>
#include <cstdint>

#include <SFML/Window/Joystick.hpp>

namespace sf
{
class Event;
}

namespace sfgc
{
enum class Button : std::size_t {
    A,
    B,
    X,
    Y,
    LeftShoulder,
    RightShoulder,
    LeftTrigger,
    RightTrigger,
    LeftStick,
    RightStick,
    Back,
    Start,
    Guide,
    None
};

static const std::size_t NUM_BUTTONS = 13;

enum class Axis : std::size_t {
    DPadX,
    DPadY,
    LeftX,
    LeftY,
    RightX,
    RightY,
    LeftTrigger,
    RightTrigger,
    None
};

static const std::size_t NUM_AXES = 8;

using GamepadId = unsigned int;
using SFMLButtonId = unsigned int;

struct AxisMapping {
    enum class Type {
        Axis, // SFGC axis <-> SFML axis
        Button, // SFGC axis <-> SFML button (TODO: needs two buttons to work actually)
        None
    };

    Type type = Type::None;
    sf::Joystick::Axis axis = sf::Joystick::PovX;
    SFMLButtonId button = 0; // TODO: store TWO buttons, because won't work with one
    bool inverted = false; // if true - positive value of axis in SFML will mean negative value in
                           // SFGC
};

struct Mapping {
    void addAxisMapping(Axis axis, const AxisMapping& axisMapping);
    void addButtonMapping(Button button, SFMLButtonId sfmlButton);

    Button getButton(SFMLButtonId button) const;
    Axis getAxis(sf::Joystick::Axis axis) const;
    Axis getAxis(SFMLButtonId button) const;
    bool isAxisInverted(Axis axis) const;

    const std::string& getGamepadName() const;

    std::array<SFMLButtonId, NUM_BUTTONS> buttons; // index - Button
    std::array<AxisMapping, NUM_AXES> axes; // index - sf::Joystick::Axis
    std::string gamepadName;
};

struct Event {
    enum class Type { ButtonPressed, ButtonReleased, AxisMoved, None };
    Type type = Type::None;

    Axis axis = Axis::None;
    Button button = Button::None;

    GamepadId gamepadId = 0;
    float axisPosition = 0.f;
};

void loadMappings();
bool mappingExists(const sf::Joystick::Identification& id);
Mapping& getMapping(const sf::Joystick::Identification& id);

bool isGamepadEvent(const sf::Event& event);
Event fromSFMLEvent(const Mapping& mapping, const sf::Event& event);

} // namespace sfgc

#endif //# IMGUI_SFML_H
