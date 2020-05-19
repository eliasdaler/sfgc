#include <sfgc/sfgc.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include <iostream>

const char* getButtonString(sfgc::Button button)
{
    switch (button) {
    case sfgc::Button::A:
        return "A";
    case sfgc::Button::B:
        return "B";
    case sfgc::Button::X:
        return "X";
    case sfgc::Button::Y:
        return "Y";
    case sfgc::Button::LeftShoulder:
        return "Left shoulder";
    case sfgc::Button::RightShoulder:
        return "Right shoulder";
    case sfgc::Button::LeftTrigger:
        return "Left trigger";
    case sfgc::Button::RightTrigger:
        return "Right trigger";
    case sfgc::Button::LeftStick:
        return "Left stick";
    case sfgc::Button::RightStick:
        return "Right stick";
    case sfgc::Button::Back:
        return "Back";
    case sfgc::Button::Start:
        return "Start";
    case sfgc::Button::Guide:
        return "Guide";
    default:
        return "";
    }
}

const char* getAxisString(sfgc::Axis axis)
{
    switch (axis) {
    case sfgc::Axis::DPadX:
        return "D-pad X";
    case sfgc::Axis::DPadY:
        return "D-pad Y";
    case sfgc::Axis::LeftX:
        return "Left stick X";
    case sfgc::Axis::LeftY:
        return "Left stick Y";
    case sfgc::Axis::RightX:
        return "Right stick X";
    case sfgc::Axis::RightY:
        return "Right stick Y";
    case sfgc::Axis::LeftTrigger:
        return "Left trigger";
    case sfgc::Axis::RightTrigger:
        return "Right trigger";
    default:
        return "";
    }
}

void printSFMLGamepadEvent(const sf::Event& event)
{
    switch (event.type) {
    case sf::Event::JoystickButtonPressed: {
        std::cout << "Gamepad #" << event.joystickButton.joystickId
                  << " button pressed: " << event.joystickButton.button << std::endl;
    } break;
    case sf::Event::JoystickButtonReleased: {
        std::cout << "Gamepad #" << event.joystickButton.joystickId
                  << " button released: " << event.joystickButton.button << std::endl;
    } break;
    case sf::Event::JoystickMoved: {
        std::cout << "Gamepad #" << event.joystickMove.joystickId << " joystick moved: axis - "
                  << event.joystickMove.axis << ", position - " << event.joystickMove.position
                  << std::endl;
    } break;
    }
}

void printGamepadEvent(const sfgc::Event& event)
{
    switch (event.type) {
    case sfgc::Event::Type::ButtonPressed:
        std::cout << "Gamepad button pressed: " << getButtonString(event.button) << std::endl;
        break;
    case sfgc::Event::Type::ButtonReleased:
        std::cout << "Gamepad button released: " << getButtonString(event.button) << std::endl;
        break;
    case sfgc::Event::Type::AxisMoved:
        std::cout << "Gamepad axis moved: " << getAxisString(event.axis)
                  << ", pos: " << event.axisPosition << std::endl;
        ;
        break;
    default:
        break;
    }
}

int main()
{
    // Create the window of the application
    sf::RenderWindow window(sf::VideoMode(400, 680), "Joystick", sf::Style::Close);
    window.setVerticalSyncEnabled(true);

    sfgc::loadMappings();
    for (unsigned int i = 0; i < sf::Joystick::Count; ++i) {
        if (sf::Joystick::isConnected(i)) {
            const auto id = sf::Joystick::getIdentification(i);
            if (sfgc::mappingExists(id)) {
                const auto& mapping = sfgc::getMapping(id);
                std::cout << "Gamepad #" << i << ": " << mapping.getGamepadName() << std::endl;
            } else {
                std::cout << "Gamepad #" << i << ". No mapping found (vendorId: " << id.vendorId
                          << ", productId: " << id.productId << ")" << std::endl;
            }
        }
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            if (sfgc::isGamepadEvent(event)) {
                printSFMLGamepadEvent(event);
                const auto id = sf::Joystick::getIdentification(0);
                if (sfgc::mappingExists(id)) {
                    const auto& mapping = sfgc::getMapping(id);
                    sfgc::Event sfgcEvent = sfgc::fromSFMLEvent(mapping, event);
                    printGamepadEvent(sfgcEvent);
                }
            }
        }

        window.clear();
        window.display();
    }

    return 0;
}
