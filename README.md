SFGC - Simple Fast Game Controller library (WIP)
================================================

SFGC is a library for SFML which makes working with game controllers easier.

**This library is not complete and is not being worked on at the moment**. Until it hits v1.0,
it'll be unstable and so **it's not recommended for use in production**. Expect
breaking changes in API and behaviour, since it'll be good to fix the problems
with SFGC as soon as possible. Plus, the format of the mapping DB might change
if it's discovered that it doesn't describe some game controllers properly.

Contributions are welcome!

Building and using SFGC
-----------------------

* This library uses C++14, so you'll need a compiler which can build C++14 code.
* SFML >= 2.5.0

TODO

Intro
-----

A 'game controllers' are gamepads, joysticks, joycons, dance pads and other things that
`sf::Joystick` can work with. I'll use "gamepads" and "game controllers"
interchangeably in the following text.

Dealing with gamepads is difficult. Each of them behaves differently. They have
different sets of buttons, their axes map to different SFML's
`sf::Joystick::Axis`, the axes have different coordinate systems (some are
inverted, some are not), so it's really hard to implement proper gamepad input
in your game.

For SDL [SDL_GameControllerDB](https://github.com/gabomdq/SDL_GameControllerDB)
exists. Unfortunately, there's no way to reuse it for SFML, since SDL and SFML
use different OS APIs for working with gamepads, so no mapping exists between SDL
axis and SFML axis.

This library is greatly inspired by SDL_GameControllerDB and uses
similar format for the mapping file. Even though SFGC won't have as many mapping
as SDL_GameControllerDB, having the most popular game controllers (like Xbox 360 and
DualShock 3 and 4 game pads) will work for most of gamepad users.

Virtual gamepad
---------------

Real gamepad buttons are mapped to a virtual gamepad which has a structure
similar to an Xbox 360/Xbox One gamepad. It has a D-pad, two clickable analog
sticks, four action buttons (A, B, X, Y), "Back", "Guide" and "Start" buttons in the
center and four buttons on top (LB, RB, LT, RT).

LT and RT represent left and right triggers. Unfortunately, SFML can't work with
them properly on Windows, recognizing them as one axis. To recognize them as
separate axes, XInput needs to be used. That's why SFGC instead maps them to
virtual buttons, not axes.

Adding New Mappings
-------------------

TODO

Mapping tools
-------------

TODO

License
-------

This library is licensed under the MIT License, see LICENSE for more information.
