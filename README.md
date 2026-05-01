# C Playlist Management Suite

This repository contains two distinct implementations of a C-based playlist management tool. The project explores different paradigms of command-line application design: an argument-driven utility with persistent storage, and an interactive Text User Interface (TUI) with real-time keystroke navigation.

## Project Structure

### 1. `cli-persistent/` (Argument-Driven Version)
A strictly command-line utility focused on data persistence and robust input validation.
*   **Data Persistence:** Reads and writes to a local text file (`playlists.txt`) to maintain state between sessions.
*   **Execution Pattern:** Uses standard command-line arguments (e.g., `./app add_playlist`) to execute specific CRUD operations.
*   **Validation & Sorting:** Enforces strict formatting rules (e.g., capitalized artist names, float bounds for ratings) and automatically sorts playlists by rating.

### 2. `tui-interactive/` (Interactive Menu Version)
A dynamic, visual terminal application utilizing low-level terminal I/O settings.
*   **Interactive TUI:** Uses the `termios.h` library to disable canonical mode, allowing for real-time, single-character keystroke capture without needing to press the Enter key for every command.
*   **Visual Navigation:** Users navigate the menus dynamically using `W` (Up), `S` (Down), and `ENTER` (Select).
*   **ANSI Formatting:** Extensive use of terminal escape codes to create a color-coded, heavily formatted visual interface with confirmation prompts.

---

## Compilation & Usage

You need a standard C compiler (e.g., `gcc`) to build both applications.

### Running the Persistent CLI Version
Navigate to the CLI folder and compile the source:
```bash
cd cli-persistent
gcc main.c -o cli_playlist

# Example usage:
./cli_playlist view
./cli_playlist help
```
### Running the Interactive TUI Version

Navigate to the TUI folder and compile the source:
```bash
cd tui-interactive
gcc main.c -o tui_playlist

# Example usage:
./tui_playlist
```
Note: Make sure Caps Lock is turned off when using the TUI version to ensure proper keystroke registration (`w` and `s`).
