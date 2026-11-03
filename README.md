# Qt-Reaction-Game

A reaction-time game developed using C++ and Qt 6, featuring randomized traffic-light signals, user authentication, and personal best-time tracking.

## About the Project

The goal is to test how quickly a player reacts to a visual signal.

The game features a traffic-light-style indicator. The player waits for the light to turn green and clicks **Stop** as quickly as possible. The application measures reaction time and saves each user's personal best. Users can register or log in to track their records.

## Features

- Traffic-light-style visual indicator
- Random waiting time before the green signal
- Reaction-time measurement
- Individual best-record tracking
- User registration and login
- Graphical user interface built with Qt Widgets

## Technologies Used

- C++
- Qt 6
- Qt Widgets
- Qt Layouts
- QPainter
- QKeyEvent
- QTimer and QElapsedTimer
- File I/O (`fstream`)

## How to Run

1. Clone or download this repository.
2. Open `ReactionGame.pro` in Qt Creator.
3. Configure the project with a compatible Qt 6 kit.
4. Build and run the application.

## Project Structure

- `main.cpp` — Application entry point
- `reaction_game.h` / `reaction_game.cpp` — Main game logic and interface
- `traffic_light.h` / `traffic_light.cpp` — Traffic light widget
- `settings_window.h` / `settings_window.cpp` — Settings window and game configuration
- `login_window.h` / `login_window.cpp` — User login and registration

## Note

This project uses basic file-based authentication for demonstration purposes. Passwords are currently stored in plain text, and the authentication system is not intended for production use.

## Future Improvements

- Sound effects
- Improved interface design
- More detailed reaction-time statistics
- Global leaderboard to compare players' best scores

## Author

Zoya Shegunts
