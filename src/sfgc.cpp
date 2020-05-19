#include <sfgc/sfgc.h>

#include <cassert>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

#include <SFML/Window/Event.hpp>

namespace
{
std::vector<std::pair<sf::Joystick::Identification, sfgc::Mapping>> s_mappings;
void loadFromString(const std::string& line);
}

namespace sfgc
{
void Mapping::addAxisMapping(Axis axis, const AxisMapping& axisMapping)
{
    axes[static_cast<std::size_t>(axis)] = axisMapping;
}

void Mapping::addButtonMapping(Button button, SFMLButtonId sfmlButton)
{
    buttons[static_cast<std::size_t>(button)] = sfmlButton;
}

Button Mapping::getButton(unsigned int buttonId) const
{
    for (std::size_t i = 0; i < NUM_BUTTONS; ++i) {
        if (buttons[i] == buttonId) {
            return static_cast<Button>(i);
        }
    }

    return Button::None;
}

Axis Mapping::getAxis(sf::Joystick::Axis axis) const
{
    for (std::size_t i = 0; i < NUM_AXES; ++i) {
        const auto& mapping = axes[i];
        if (mapping.type == AxisMapping::Type::Axis && mapping.axis == axis) {
            return static_cast<Axis>(i);
        }
    }
    return Axis::None;
}

bool Mapping::isAxisInverted(Axis axis) const
{
    return axes[static_cast<std::size_t>(axis)].inverted;
}

const std::string& Mapping::getGamepadName() const
{
    return gamepadName;
}

Axis Mapping::getAxis(unsigned int buttonId) const
{
    for (std::size_t i = 0; i < NUM_AXES; ++i) {
        const auto& mapping = axes[i];
        if (mapping.type == AxisMapping::Type::Button && mapping.button == buttonId) {
            return static_cast<Axis>(i);
        }
    }
    return Axis::None;
}

Mapping loadMapping(const std::string& str)
{}

void loadMappings()
{
    const auto mappingFile = R"foo(
1133,49693,"Logitech Gamepad F310 (XInput)",Linux,dpx:PovX,dpy:PovY,leftx:X,lefty:Y,rightx:U,righty:V,lefttrigger:Z,righttrigger:R,a:0,b:1,x:2,y:3,leftshoulder:4,rightshoulder:5,back:6,start:7,guide:8,leftstick:9,rightstick:10
1133,49686,"Logitech Gamepad F310 (DInput)",Linux,dpx:PovX,dpy:PovY,leftx:X,lefty:Y,rightx:Z,righty:R,a:1,b:2,x:0,y:3,leftshoulder:4,rightshoulder:5,lefttrigger:6,righttrigger:7,back:8,start:9,guide:,leftstick:10,rightstick:11
1356,2508,"Sony DualShock 4 Wireless",Linux,dpx:PovX,dpy:PovY,leftx:X,lefty:Y,rightx:U,righty:V,lefttrigger:Z,righttrigger:R,a:0,b:1,x:3,y:2,leftshoulder:4,rightshoulder:5,back:8,start:9,guide:10,leftstick:11,rightstick:12
)foo";

    std::stringstream ss(mappingFile);
    std::string line;
    while (std::getline(ss, line)) {
        if (!line.empty()) {
            std::cout << line << std::endl;
            ::loadFromString(line);
        }
    }
}

bool mappingExists(const sf::Joystick::Identification& id)
{
    const auto it = std::find_if(s_mappings.begin(), s_mappings.end(), [&id](const auto& elem) {
        return elem.first.vendorId == id.vendorId && elem.first.productId == id.productId;
    });
    return it != s_mappings.end();
}

Mapping& getMapping(const sf::Joystick::Identification& id)
{
    assert(mappingExists(id));
    const auto it = std::find_if(s_mappings.begin(), s_mappings.end(), [&id](const auto& elem) {
        return elem.first.vendorId == id.vendorId && elem.first.productId == id.productId;
    });
    return it->second;
}

bool isGamepadEvent(const sf::Event& event)
{
    switch (event.type) {
    case sf::Event::JoystickButtonPressed:
    case sf::Event::JoystickButtonReleased:
    case sf::Event::JoystickMoved:
    case sf::Event::JoystickDisconnected:
        return true;
    default:
        return false;
    }
}

Event fromSFMLEvent(const Mapping& mapping, const sf::Event& event)
{
    Event e;

    switch (event.type) {
    case sf::Event::JoystickButtonPressed: {
        e.gamepadId = event.joystickButton.joystickId;
        const auto button = mapping.getButton(event.joystickButton.button);
        if (button != Button::None) {
            e.type = Event::Type::ButtonPressed;
            e.button = button;
        } else {
            auto axis = mapping.getAxis(event.joystickButton.button);
            if (axis != Axis::None) {
                e.type = Event::Type::AxisMoved;
                e.axis = axis;
                e.axisPosition = 100.f;
            }
        }
    } break;
    case sf::Event::JoystickButtonReleased: {
        e.gamepadId = event.joystickButton.joystickId;
        const auto button = mapping.getButton(event.joystickButton.button);
        if (button != Button::None) {
            e.type = Event::Type::ButtonReleased;
            e.button = button;
        } else {
            auto axis = mapping.getAxis(event.joystickButton.button);
            if (axis != Axis::None) {
                e.type = Event::Type::AxisMoved;
                e.axis = axis;
                e.axisPosition = 0.f;
            }
        }
    } break;
    case sf::Event::JoystickMoved: {
        e.gamepadId = event.joystickMove.joystickId;
        const auto axis = mapping.getAxis(event.joystickMove.axis);
        if (axis != Axis::None) {
            e.type = Event::Type::AxisMoved;
            e.axis = axis;

            e.axisPosition = mapping.isAxisInverted(axis) ? -event.joystickMove.position :
                                                            event.joystickMove.position;
        } else {
            // TODO: axis is button?
        }
    } break;
    default:
        e.type = Event::Type::None;
        break;
    }

    return e;
}

} // namespace sfgc

namespace
{
/* clang-format off */
std::unordered_map<std::string, sfgc::Axis> stringToSFGCAxis = {
    {"dpx", sfgc::Axis::DPadX},
    {"dpy", sfgc::Axis::DPadY},
    {"leftx", sfgc::Axis::LeftX},
    {"lefty", sfgc::Axis::LeftY},
    {"rightx", sfgc::Axis::RightX},
    {"righty", sfgc::Axis::RightY},
    {"lefttrigger", sfgc::Axis::LeftTrigger},
    {"righttrigger", sfgc::Axis::RightTrigger},
};

std::unordered_map<std::string, sf::Joystick::Axis> stringToSFMLAxis = {
    {"X", sf::Joystick::Axis::X},
    {"Y", sf::Joystick::Axis::Y},
    {"Z", sf::Joystick::Axis::Z},
    {"R", sf::Joystick::Axis::R},
    {"U", sf::Joystick::Axis::U},
    {"V", sf::Joystick::Axis::V},
    {"PovX", sf::Joystick::Axis::PovX},
    {"PovY", sf::Joystick::Axis::PovY},
};

std::unordered_map<std::string, sfgc::Button> stringToSFGCButton = {
    {"a", sfgc::Button::A},
    {"b", sfgc::Button::B},
    {"x", sfgc::Button::X},
    {"y", sfgc::Button::Y},
    {"leftshoulder", sfgc::Button::LeftShoulder},
    {"rightshoulder", sfgc::Button::RightShoulder},
    {"lefttrigger", sfgc::Button::LeftTrigger},
    {"righttrigger", sfgc::Button::RightTrigger},
    {"leftstick", sfgc::Button::LeftStick},
    {"rightstick", sfgc::Button::RightStick},
    {"back", sfgc::Button::Back},
    {"start", sfgc::Button::Start},
    {"guide", sfgc::Button::Guide},
};
/* clang-format on */

// This function is inefficient and horribly written, but it works
// It doesn't do error checking and does lots of string allocations
// Anyone is welcome to create PRs improving it, they'll be very welcome
void loadFromString(const std::string& line)
{
    sfgc::Mapping mapping;

    std::stringstream lineStream(line);
    std::string token;

    sf::Joystick::Identification id;

    // get vendor id
    std::getline(lineStream, token, ',');
    std::cout << "vendor id: " << token << std::endl;
    id.vendorId = std::stoi(token);

    // get product id
    std::getline(lineStream, token, ',');
    std::cout << "product id: " << token << std::endl;
    id.productId = std::stoi(token);

    // get gamepad name
    lineStream.ignore(); // ignore starting quote
    std::getline(lineStream, token, '"');
    std::cout << "gamepad name: " << token << std::endl;
    mapping.gamepadName = token;

    lineStream.ignore(); // ignore comma after gamepad name

    // get platform
    std::getline(lineStream, token, ',');
    std::cout << "platform: " << token << std::endl;

    // get the rest of the mapping
    while (std::getline(lineStream, token, ',')) {
        std::stringstream tokenStream(token);
        std::string key, value;

        std::getline(tokenStream, key, ':');
        std::getline(tokenStream, value);

        if (value.empty()) {
            continue;
        }

        std::cout << "\t" << key << " -> " << value << std::endl;

        // try to read as axis mapping
        {
            auto it = stringToSFGCAxis.find(key);
            if (it != stringToSFGCAxis.end()) {
                auto it2 = stringToSFMLAxis.find(value);
                if (it2 != stringToSFMLAxis.end()) {
                    const auto& sfgcAxis = it->second;
                    const auto& sfmlAxis = it2->second;
                    mapping.addAxisMapping(sfgcAxis,
                                           sfgc::AxisMapping{sfgc::AxisMapping::Type::Axis,
                                                             sfmlAxis, 0, false});
                    continue;
                }
            }
        }

        // try to read as button mapping
        {
            auto it = stringToSFGCButton.find(key);
            if (it != stringToSFGCButton.end()) {
                const auto& sfgcButton = it->second;
                const auto sfmlButtonId = static_cast<sfgc::SFMLButtonId>(std::stoi(value));

                mapping.addButtonMapping(sfgcButton, sfmlButtonId);
                continue;
            }
        }

        std::cerr << "unknown SFGC token pair: " << token << std::endl;
    }

    s_mappings.push_back({id, mapping});
}
} // end of anonymous namespace
