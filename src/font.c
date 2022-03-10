#include <stdint.h>
#include "font.h"

static uint8_t const font_8_bitmaps[] = 
{
    // ASCII: SCII: 32, char width: 3
    0x00,  // ---.............
    0x00,  // ---.............
    0x00,  // ---.............
    0x00,  // ---.............
    0x00,  // ---.............
    0x00,  // ---.............
    0x00,  // ---.............
    0x00,  // ---.............

    // ASCII: 33, char width: 3
    0x40,  // -O-.............
    0x40,  // -O-.............
    0x40,  // -O-.............
    0x40,  // -O-.............
    0x40,  // -O-.............
    0x00,  // ---.............
    0x40,  // -O-.............
    0x00,  // ---.............

    // ASCII: 34, char width: 3
    0xa0,  // O-O.............
    0xa0,  // O-O.............
    0x00,  // ---.............
    0x00,  // ---.............
    0x00,  // ---.............
    0x00,  // ---.............
    0x00,  // ---.............
    0x00,  // ---.............

    // ASCII: 35, char width: 5
    0x50,  // -O-O-...........
    0x50,  // -O-O-...........
    0xf0,  // OOOO-...........
    0x50,  // -O-O-...........
    0xf0,  // OOOO-...........
    0xa0,  // O-O--...........
    0xa0,  // O-O--...........
    0x00,  // -----...........

    // ASCII: 36, char width: 5
    0x70,  // -OOO-...........
    0xe8,  // OOO-O...........
    0xe0,  // OOO--...........
    0x70,  // -OOO-...........
    0x28,  // --O-O...........
    0xb8,  // O-OOO...........
    0x70,  // -OOO-...........
    0x20,  // --O--...........

    // ASCII: 37, char width: 8
    0x24,  // --O--O--........
    0x7c,  // -OOOOO--........
    0x28,  // --O-O---........
    0x08,  // ----O---........
    0x1c,  // ---OOO--........
    0x1e,  // ---OOOO-........
    0x14,  // ---O-O--........
    0x00,  // --------........

    // ASCII: 38, char width: 6
    0x30,  // --OO--..........
    0x48,  // -O--O-..........
    0x58,  // -O-OO-..........
    0x60,  // -OO---..........
    0x9c,  // O--OOO..........
    0x98,  // O--OO-..........
    0x7c,  // -OOOOO..........
    0x00,  // ------..........

    // ASCII: 39, char width: 2
    0x80,  // O-..............
    0x80,  // O-..............
    0x00,  // --..............
    0x00,  // --..............
    0x00,  // --..............
    0x00,  // --..............
    0x00,  // --..............
    0x00,  // --..............

    // ASCII: 40, char width: 3
    0x20,  // --O.............
    0x40,  // -O-.............
    0x80,  // O--.............
    0x80,  // O--.............
    0x80,  // O--.............
    0x80,  // O--.............
    0x80,  // O--.............
    0x40,  // -O-.............

    // ASCII: 41, char width: 3
    0x80,  // O--.............
    0x40,  // -O-.............
    0x20,  // --O.............
    0x20,  // --O.............
    0x20,  // --O.............
    0x20,  // --O.............
    0x20,  // --O.............
    0x40,  // -O-.............

    // ASCII: 42, char width: 4
    0xe0,  // OOO-............
    0x40,  // -O--............
    0x00,  // ----............
    0x00,  // ----............
    0x00,  // ----............
    0x00,  // ----............
    0x00,  // ----............
    0x00,  // ----............

    // ASCII: 43, char width: 6
    0x00,  // ------..........
    0x00,  // ------..........
    0x20,  // --O---..........
    0x20,  // --O---..........
    0xf8,  // OOOOO-..........
    0x20,  // --O---..........
    0x20,  // --O---..........
    0x00,  // ------..........

    // ASCII: 44, char width: 3
    0x00,  // ---.............
    0x00,  // ---.............
    0x00,  // ---.............
    0x00,  // ---.............
    0x00,  // ---.............
    0x00,  // ---.............
    0x40,  // -O-.............
    0x40,  // -O-.............

    // ASCII: 45, char width: 4
    0x00,  // ----............
    0x00,  // ----............
    0x00,  // ----............
    0x00,  // ----............
    0xe0,  // OOO-............
    0x00,  // ----............
    0x00,  // ----............
    0x00,  // ----............

    // ASCII: 46, char width: 3
    0x00,  // ---.............
    0x00,  // ---.............
    0x00,  // ---.............
    0x00,  // ---.............
    0x00,  // ---.............
    0x00,  // ---.............
    0x40,  // -O-.............
    0x00,  // ---.............

    // ASCII: 47, char width: 3
    0x20,  // --O.............
    0x20,  // --O.............
    0x40,  // -O-.............
    0x40,  // -O-.............
    0x40,  // -O-.............
    0x80,  // O--.............
    0x80,  // O--.............
    0x00,  // ---.............

    // ASCII: 48, char width: 5
    0x60,  // -OO--...........
    0x90,  // O--O-...........
    0x90,  // O--O-...........
    0x90,  // O--O-...........
    0x90,  // O--O-...........
    0x90,  // O--O-...........
    0x60,  // -OO--...........
    0x00,  // -----...........

    // ASCII: 49, char width: 5
    0x20,  // --O--...........
    0x60,  // -OO--...........
    0x20,  // --O--...........
    0x20,  // --O--...........
    0x20,  // --O--...........
    0x20,  // --O--...........
    0x20,  // --O--...........
    0x00,  // -----...........

    // ASCII: 50, char width: 5
    0x60,  // -OO--...........
    0x90,  // O--O-...........
    0x10,  // ---O-...........
    0x10,  // ---O-...........
    0x20,  // --O--...........
    0x40,  // -O---...........
    0xf0,  // OOOO-...........
    0x00,  // -----...........

    // ASCII: 51, char width: 5
    0x60,  // -OO--...........
    0x90,  // O--O-...........
    0x10,  // ---O-...........
    0x30,  // --OO-...........
    0x10,  // ---O-...........
    0x90,  // O--O-...........
    0x60,  // -OO--...........
    0x00,  // -----...........

    // ASCII: 52, char width: 5
    0x10,  // ---O-...........
    0x30,  // --OO-...........
    0x50,  // -O-O-...........
    0x90,  // O--O-...........
    0xf8,  // OOOOO...........
    0x10,  // ---O-...........
    0x10,  // ---O-...........
    0x00,  // -----...........

    // ASCII: 53, char width: 5
    0x70,  // -OOO-...........
    0x40,  // -O---...........
    0xe0,  // OOO--...........
    0x90,  // O--O-...........
    0x10,  // ---O-...........
    0x90,  // O--O-...........
    0x60,  // -OO--...........
    0x00,  // -----...........

    // ASCII: 54, char width: 5
    0x60,  // -OO--...........
    0x90,  // O--O-...........
    0x80,  // O----...........
    0xe0,  // OOO--...........
    0x90,  // O--O-...........
    0x90,  // O--O-...........
    0x60,  // -OO--...........
    0x00,  // -----...........

    // ASCII: 55, char width: 5
    0xf0,  // OOOO-...........
    0x10,  // ---O-...........
    0x20,  // --O--...........
    0x20,  // --O--...........
    0x40,  // -O---...........
    0x40,  // -O---...........
    0x40,  // -O---...........
    0x00,  // -----...........

    // ASCII: 56, char width: 5
    0x60,  // -OO--...........
    0x90,  // O--O-...........
    0x90,  // O--O-...........
    0x60,  // -OO--...........
    0x90,  // O--O-...........
    0x90,  // O--O-...........
    0x60,  // -OO--...........
    0x00,  // -----...........

    // ASCII: 57, char width: 5
    0x60,  // -OO--...........
    0x90,  // O--O-...........
    0x90,  // O--O-...........
    0x70,  // -OOO-...........
    0x10,  // ---O-...........
    0x90,  // O--O-...........
    0x60,  // -OO--...........
    0x00,  // -----...........

    // ASCII: 58, char width: 3
    0x00,  // ---.............
    0x00,  // ---.............
    0x40,  // -O-.............
    0x00,  // ---.............
    0x00,  // ---.............
    0x00,  // ---.............
    0x40,  // -O-.............
    0x00,  // ---.............

    // ASCII: 59, char width: 3
    0x00,  // ---.............
    0x00,  // ---.............
    0x40,  // -O-.............
    0x00,  // ---.............
    0x00,  // ---.............
    0x00,  // ---.............
    0x40,  // -O-.............
    0x40,  // -O-.............

    // ASCII: 60, char width: 5
    0x00,  // -----...........
    0x10,  // ---O-...........
    0x20,  // --O--...........
    0x40,  // -O---...........
    0x20,  // --O--...........
    0x10,  // ---O-...........
    0x00,  // -----...........
    0x00,  // -----...........

    // ASCII: 61, char width: 5
    0x00,  // -----...........
    0x00,  // -----...........
    0xf0,  // OOOO-...........
    0x00,  // -----...........
    0xf0,  // OOOO-...........
    0x00,  // -----...........
    0x00,  // -----...........
    0x00,  // -----...........

    // ASCII: 62, char width: 5
    0x00,  // -----...........
    0x40,  // -O---...........
    0x20,  // --O--...........
    0x10,  // ---O-...........
    0x20,  // --O--...........
    0x40,  // -O---...........
    0x00,  // -----...........
    0x00,  // -----...........

    // ASCII: 63, char width: 5
    0x70,  // -OOO-...........
    0x88,  // O---O...........
    0x08,  // ----O...........
    0x10,  // ---O-...........
    0x20,  // --O--...........
    0x00,  // -----...........
    0x20,  // --O--...........
    0x00,  // -----...........

    // ASCII: 64, char width: 9
    0x1e,  // ---OOOO--.......
    0x21,  // --O----O-.......
    0x0e,  // ----OOO-O.......
    0x5e,  // -O-OOOO-O.......
    0x54,  // -O-O-O--O.......
    0x5d,  // -O-OOO-O-.......
    0x5e,  // -O-OOOO--.......
    0x21,  // --O----OO.......

    // ASCII: 65, char width: 6
    0x10,  // ---O---.........
    0x38,  // --OOO--.........
    0x28,  // --O-O--.........
    0x44,  // -O---O-.........
    0x7c,  // -OOOOO-.........
    0x44,  // -O---O-.........
    0x82,  // O-----O.........
    0x00,  // -------.........

    // ASCII: 66, char width: 6
    0x70,  // -OOO--..........
    0x48,  // -O--O-..........
    0x48,  // -O--O-..........
    0x78,  // -OOOO-..........
    0x48,  // -O--O-..........
    0x48,  // -O--O-..........
    0x70,  // -OOO--..........
    0x00,  // ------..........

    // ASCII: 67, char width: 7
    0x38,  // --OOO--.........
    0x44,  // -O---O-.........
    0x40,  // -O-----.........
    0x40,  // -O-----.........
    0x40,  // -O-----.........
    0x4c,  // -O--OO-.........
    0x38,  // --OOO--.........
    0x00,  // -------.........

    // ASCII: 68, char width: 7
    0x78,  // -OOOO--.........
    0x44,  // -O---O-.........
    0x44,  // -O---O-.........
    0x44,  // -O---O-.........
    0x44,  // -O---O-.........
    0x44,  // -O---O-.........
    0x78,  // -OOOO--.........
    0x00,  // -------.........

    // ASCII: 69, char width: 6
    0x7c,  // -OOOOO..........
    0x40,  // -O----..........
    0x40,  // -O----..........
    0x7c,  // -OOOOO..........
    0x40,  // -O----..........
    0x40,  // -O----..........
    0x7c,  // -OOOOO..........
    0x00,  // ------..........

    // ASCII: 70, char width: 6
    0x78,  // -OOOO-..........
    0x40,  // -O----..........
    0x40,  // -O----..........
    0x70,  // -OOO--..........
    0x40,  // -O----..........
    0x40,  // -O----..........
    0x40,  // -O----..........
    0x00,  // ------..........

    // ASCII: 71, char width: 7
    0x38,  // --OOO--.........
    0x44,  // -O---O-.........
    0x40,  // -O-----.........
    0x4c,  // -O--OO-.........
    0x44,  // -O---O-.........
    0x44,  // -O---O-.........
    0x38,  // --OOO--.........
    0x00,  // -------.........

    // ASCII: 72, char width: 7
    0x44,  // -O---O-.........
    0x44,  // -O---O-.........
    0x44,  // -O---O-.........
    0x7c,  // -OOOOO-.........
    0x44,  // -O---O-.........
    0x44,  // -O---O-.........
    0x44,  // -O---O-.........
    0x00,  // -------.........

    // ASCII: 73, char width: 3
    0x40,  // -O-.............
    0x40,  // -O-.............
    0x40,  // -O-.............
    0x40,  // -O-.............
    0x40,  // -O-.............
    0x40,  // -O-.............
    0x40,  // -O-.............
    0x00,  // ---.............

    // ASCII: 74, char width: 5
    0x10,  // ---O-...........
    0x10,  // ---O-...........
    0x10,  // ---O-...........
    0x10,  // ---O-...........
    0x10,  // ---O-...........
    0x90,  // O--O-...........
    0x60,  // -OO--...........
    0x00,  // -----...........

    // ASCII: 75, char width: 6
    0x44,  // -O---O..........
    0x48,  // -O--O-..........
    0x50,  // -O-O--..........
    0x70,  // -OOO--..........
    0x70,  // -OOO--..........
    0x48,  // -O--O-..........
    0x44,  // -O---O..........
    0x00,  // ------..........

    // ASCII: 76, char width: 5
    0x40,  // -O---...........
    0x40,  // -O---...........
    0x40,  // -O---...........
    0x40,  // -O---...........
    0x40,  // -O---...........
    0x40,  // -O---...........
    0x78,  // -OOOO...........
    0x00,  // -----...........

    // ASCII: 77, char width: 7
    0x82,  // O-----O.........
    0x82,  // O-----O.........
    0xc6,  // OO---OO.........
    0xaa,  // O-O-O-O.........
    0xaa,  // O-O-O-O.........
    0xba,  // O-OOO-O.........
    0x92,  // O--O--O.........
    0x00,  // -------.........

    // ASCII: 78, char width: 7
    0x44,  // -O---O-.........
    0x64,  // -OO--O-.........
    0x64,  // -OO--O-.........
    0x54,  // -O-O-O-.........
    0x4c,  // -O--OO-.........
    0x4c,  // -O--OO-.........
    0x44,  // -O---O-.........
    0x00,  // -------.........

    // ASCII: 79, char width: 7
    0x38,  // --OOO--.........
    0x44,  // -O---O-.........
    0x44,  // -O---O-.........
    0x44,  // -O---O-.........
    0x44,  // -O---O-.........
    0x44,  // -O---O-.........
    0x38,  // --OOO--.........
    0x00,  // -------.........

    // ASCII: 80, char width: 6
    0x70,  // -OOO--..........
    0x48,  // -O--O-..........
    0x48,  // -O--O-..........
    0x70,  // -OOO--..........
    0x40,  // -O----..........
    0x40,  // -O----..........
    0x40,  // -O----..........
    0x00,  // ------..........

    // ASCII: 81, char width: 7
    0x38,  // --OOO--.........
    0x44,  // -O---O-.........
    0x44,  // -O---O-.........
    0x44,  // -O---O-.........
    0x44,  // -O---O-.........
    0x48,  // -O--O--.........
    0x3c,  // --OOOO-.........
    0x00,  // -------.........

    // ASCII: 82, char width: 7
    0x78,  // -OOOO--.........
    0x44,  // -O---O-.........
    0x44,  // -O---O-.........
    0x78,  // -OOOO--.........
    0x48,  // -O--O--.........
    0x44,  // -O---O-.........
    0x44,  // -O---O-.........
    0x00,  // -------.........

    // ASCII: 83, char width: 6
    0x30,  // --OO--..........
    0x48,  // -O--O-..........
    0x40,  // -O----..........
    0x30,  // --OO--..........
    0x08,  // ----O-..........
    0x48,  // -O--O-..........
    0x30,  // --OO--..........
    0x00,  // ------..........

    // ASCII: 84, char width: 5
    0xf8,  // OOOOO...........
    0x20,  // --O--...........
    0x20,  // --O--...........
    0x20,  // --O--...........
    0x20,  // --O--...........
    0x20,  // --O--...........
    0x20,  // --O--...........
    0x00,  // -----...........

    // ASCII: 85, char width: 7
    0x44,  // -O---O-.........
    0x44,  // -O---O-.........
    0x44,  // -O---O-.........
    0x44,  // -O---O-.........
    0x44,  // -O---O-.........
    0x44,  // -O---O-.........
    0x38,  // --OOO--.........
    0x00,  // -------.........

    // ASCII: 86, char width: 8
    0x82,  // O-----O-........
    0x82,  // O-----O-........
    0x44,  // -O---O--........
    0x44,  // -O---O--........
    0x28,  // --O-O---........
    0x28,  // --O-O---........
    0x10,  // ---O----........
    0x00,  // --------........

    // ASCII: 87, char width: 8
    0x92,  // O--O--O_.......
    0x92,  // O-_O_-O_.......
    0xAA,  // O-O-O-O_.......
    0xAA,  // O-O-O-O_.......
    0xAA,  // O-O-O-O_.......
    0xEE,  // OOO-OOO_.......
    0x44,  // -O---O-_.......
    0x00,  // --------.......

    // ASCII: 88, char width: 5
    0x88,  // O---O...........
    0x50,  // -O-O-...........
    0x70,  // -OOO-...........
    0x20,  // --O--...........
    0x70,  // -OOO-...........
    0x50,  // -O-O-...........
    0x88,  // O---O...........
    0x00,  // -----...........

    // ASCII: 89, char width: 7
    0x44,  // -O---O-.........
    0x28,  // --O-O--.........
    0x38,  // --OOO--.........
    0x10,  // ---O---.........
    0x10,  // ---O---.........
    0x10,  // ---O---.........
    0x10,  // ---O---.........
    0x00,  // -------.........

    // ASCII: 90, char width: 6
    0xfc,  // OOOOOO..........
    0x0c,  // ----OO..........
    0x18,  // ---OO-..........
    0x20,  // --O---..........
    0x40,  // -O----..........
    0x80,  // O-----..........
    0xfc,  // OOOOOO..........
    0x00,  // ------..........

    // ASCII: 91, char width: 3
    0x60,  // -OO.............
    0x40,  // -O-.............
    0x40,  // -O-.............
    0x40,  // -O-.............
    0x40,  // -O-.............
    0x40,  // -O-.............
    0x40,  // -O-.............
    0x60,  // -OO.............

    // ASCII: 92, char width: 3
    0x80,  // O--.............
    0x80,  // O--.............
    0x40,  // -O-.............
    0x40,  // -O-.............
    0x40,  // -O-.............
    0x20,  // --O.............
    0x20,  // --O.............
    0x00,  // ---.............

    // ASCII: 93, char width: 3
    0xc0,  // OO-.............
    0x40,  // -O-.............
    0x40,  // -O-.............
    0x40,  // -O-.............
    0x40,  // -O-.............
    0x40,  // -O-.............
    0x40,  // -O-.............
    0xc0,  // OO-.............

    // ASCII: 94, char width: 3
    0x40,  // -O-.............
    0xe0,  // OOO.............
    0xa0,  // O-O.............
    0x00,  // ---.............
    0x00,  // ---.............
    0x00,  // ---.............
    0x00,  // ---.............
    0x00,  // ---.............

    // ASCII: 95, char width: 5
    0x00,  // -----...........
    0x00,  // -----...........
    0x00,  // -----...........
    0x00,  // -----...........
    0x00,  // -----...........
    0x00,  // -----...........
    0x00,  // -----...........
    0xf8,  // OOOOO...........

    // ASCII: 96, char width: 0
    0x00,  // ................
    0x00,  // ................
    0x00,  // ................
    0x00,  // ................
    0x00,  // ................
    0x00,  // ................
    0x00,  // ................
    0x00,  // ................

    // ASCII: 97, char width: 5
    0x00,  // -----...........
    0x00,  // -----...........
    0xf0,  // OOOO-...........
    0x10,  // ---O-...........
    0x70,  // -OOO-...........
    0x90,  // O--O-...........
    0xf0,  // OOOO-...........
    0x00,  // -----...........

    // ASCII: 98, char width: 5
    0x80,  // O----...........
    0x80,  // O----...........
    0xe0,  // OOO--...........
    0x90,  // O--O-...........
    0x90,  // O--O-...........
    0x90,  // O--O-...........
    0xe0,  // OOO--...........
    0x00,  // -----...........

    // ASCII: 99, char width: 5
    0x00,  // -----...........
    0x00,  // -----...........
    0x60,  // -OO--...........
    0x90,  // O--O-...........
    0x80,  // O----...........
    0x90,  // O--O-...........
    0x60,  // -OO--...........
    0x00,  // -----...........

    // ASCII: 100, char width: 5
    0x10,  // ---O-...........
    0x10,  // ---O-...........
    0x70,  // -OOO-...........
    0x90,  // O--O-...........
    0x90,  // O--O-...........
    0x90,  // O--O-...........
    0x70,  // -OOO-...........
    0x00,  // -----...........

    // ASCII: 101, char width: 5
    0x00,  // -----...........
    0x00,  // -----...........
    0x60,  // -OO--...........
    0x90,  // O--O-...........
    0xf0,  // OOOO-...........
    0x80,  // O----...........
    0x70,  // -OOO-...........
    0x00,  // -----...........

    // ASCII: 102, char width: 4
    0x20,  // --O-............
    0x40,  // -O--............
    0xe0,  // OOO-............
    0x40,  // -O--............
    0x40,  // -O--............
    0x40,  // -O--............
    0x40,  // -O--............
    0x00,  // ----............

    // ASCII: 103, char width: 5
    0x00,  // -----...........
    0x00,  // -----...........
    0x70,  // -OOO-...........
    0x90,  // O--O-...........
    0xd0,  // OO-O-...........
    0x70,  // -OOO-...........
    0x90,  // O--O-...........
    0x70,  // -OOO-...........

    // ASCII: 104, char width: 5
    0x80,  // O----...........
    0x80,  // O----...........
    0xe0,  // OOO--...........
    0x90,  // O--O-...........
    0x90,  // O--O-...........
    0x90,  // O--O-...........
    0x90,  // O--O-...........
    0x00,  // -----...........

    // ASCII: 105, char width: 2
    0x80,  // O-..............
    0x00,  // --..............
    0x80,  // O-..............
    0x80,  // O-..............
    0x80,  // O-..............
    0x80,  // O-..............
    0x80,  // O-..............
    0x00,  // --..............

    // ASCII: 106, char width: 2
    0x80,  // O-..............
    0x00,  // --..............
    0x80,  // O-..............
    0x80,  // O-..............
    0x80,  // O-..............
    0x80,  // O-..............
    0x80,  // O-..............
    0x80,  // O-..............

    // ASCII: 107, char width: 5
    0x80,  // O----...........
    0x80,  // O----...........
    0x90,  // O--O-...........
    0xa0,  // O-O--...........
    0xe0,  // OOO--...........
    0xa0,  // O-O--...........
    0x90,  // O--O-...........
    0x00,  // -----...........

    // ASCII: 108, char width: 2
    0x80,  // O-..............
    0x80,  // O-..............
    0x80,  // O-..............
    0x80,  // O-..............
    0x80,  // O-..............
    0x80,  // O-..............
    0x80,  // O-..............
    0x00,  // --..............

    // ASCII: 109, char width: 8
    0x00,  // --------........
    0x00,  // --------........
    0xfc,  // OOOOOO--........
    0x92,  // O--O--O-........
    0x92,  // O--O--O-........
    0x92,  // O--O--O-........
    0x92,  // O--O--O-........
    0x00,  // --------........

    // ASCII: 110, char width: 5
    0x00,  // -----...........
    0x00,  // -----...........
    0xe0,  // OOO--...........
    0x90,  // O--O-...........
    0x90,  // O--O-...........
    0x90,  // O--O-...........
    0x90,  // O--O-...........
    0x00,  // -----...........

    // ASCII: 111, char width: 5
    0x00,  // -----...........
    0x00,  // -----...........
    0x60,  // -OO--...........
    0x90,  // O--O-...........
    0x90,  // O--O-...........
    0x90,  // O--O-...........
    0x60,  // -OO--...........
    0x00,  // -----...........

    // ASCII: 112, char width: 5
    0x00,  // -----...........
    0x00,  // -----...........
    0xe0,  // OOO--...........
    0x90,  // O--O-...........
    0x90,  // O--O-...........
    0x90,  // O--O-...........
    0xe0,  // OOO--...........
    0x80,  // O----...........

    // ASCII: 113, char width: 5
    0x00,  // -----...........
    0x00,  // -----...........
    0x70,  // -OOO-...........
    0x90,  // O--O-...........
    0x90,  // O--O-...........
    0x90,  // O--O-...........
    0x70,  // -OOO-...........
    0x10,  // ---O-...........

    // ASCII: 114, char width: 3
    0x00,  // ---.............
    0x00,  // ---.............
    0xe0,  // OOO.............
    0x80,  // O--.............
    0x80,  // O--.............
    0x80,  // O--.............
    0x80,  // O--.............
    0x00,  // ---.............

    // ASCII: 115, char width: 5
    0x00,  // -----...........
    0x00,  // -----...........
    0x70,  // -OOO-...........
    0xc0,  // OO---...........
    0x60,  // -OO--...........
    0x30,  // --OO-...........
    0xe0,  // OOO--...........
    0x00,  // -----...........

    // ASCII: 116, char width: 3
    0x00,  // ---.............
    0x80,  // O--.............
    0xc0,  // OO-.............
    0x80,  // O--.............
    0x80,  // O--.............
    0x80,  // O--.............
    0xc0,  // OO-.............
    0x00,  // ---.............

    // ASCII: 117, char width: 5
    0x00,  // -----...........
    0x00,  // -----...........
    0x90,  // O--O-...........
    0x90,  // O--O-...........
    0x90,  // O--O-...........
    0x90,  // O--O-...........
    0x70,  // -OOO-...........
    0x00,  // -----...........

    // ASCII: 118, char width: 6
    0x00,  // ------..........
    0x00,  // ------..........
    0x88,  // O---O-..........
    0x50,  // -O-O--..........
    0x50,  // -O-O--..........
    0x70,  // -OOO--..........
    0x20,  // --O---..........
    0x00,  // ------..........

    // ASCII: 119, char width: 6
    0x00,  // ------..........
    0x00,  // ------..........
    0xf8,  // OOOOO-..........
    0xf8,  // OOOOO-..........
    0xf8,  // OOOOO-..........
    0xf8,  // OOOOO-..........
    0x70,  // -OOO--..........
    0x00,  // ------..........

    // ASCII: 120, char width: 5
    0x00,  // -----...........
    0x00,  // -----...........
    0x90,  // O--O-...........
    0x60,  // -OO--...........
    0x60,  // -OO--...........
    0x60,  // -OO--...........
    0x90,  // O--O-...........
    0x00,  // -----...........

    // ASCII: 121, char width: 6
    0x00,  // ------..........
    0x00,  // ------..........
    0x88,  // O---O-..........
    0x50,  // -O-O--..........
    0x50,  // -O-O--..........
    0x70,  // -OOO--..........
    0x20,  // --O---..........
    0x20,  // --O---..........

    // ASCII: 122, char width: 4
    0x00,  // ----............
    0x00,  // ----............
    0xe0,  // OOO-............
    0x20,  // --O-............
    0x40,  // -O--............
    0x80,  // O---............
    0xe0,  // OOO-............
    0x00,  // ----............

    // ASCII: 123, char width: 7
    0x00,  // ................
    0x00,  // ................
    0x00,  // ................
    0x00,  // ................
    0x00,  // ................
    0x00,  // ................
    0x00,  // ................
    0x00,  // ................

    // ASCII: 124, char width: 0
    0x00,  // ................
    0x00,  // ................
    0x00,  // ................
    0x00,  // ................
    0x00,  // ................
    0x00,  // ................
    0x00,  // ................
    0x00,  // ................

    // ASCII: 125  char width: 7
    0x00,  // ................
    0x00,  // ................
    0x00,  // ................
    0x00,  // ................
    0x00,  // ................
    0x00,  // ................
    0x00,  // ................
    0x00,  // ................


    // ASCII: 126, char width: 0
    0x00,  // ................
    0x00,  // ................
    0x00,  // ................
    0x00,  // ................
    0x00,  // ................
    0x00,  // ................
    0x00,  // ................
    0x00,  // ................

    // ASCII: 127, char width: 0
    0x00,  // ................
    0x00,  // ................
    0x00,  // ................
    0x00,  // ................
    0x00,  // ................
    0x00,  // ................
    0x00,  // ................
    0x00,  // ................
};

static uint8_t const font_8_widths[] = 
{
 /*  32 */ 3,  3,  3,  5,  5,  8,  6,  2,
 /*  40 */ 3,  3,  4,  6,  3,  4,  3,  3,
 /*  48 */ 5,  5,  5,  5,  5,  5,  5,  5,
 /*  56 */ 5,  5,  3,  3,  5,  5,  5,  5,
 /*  64 */ 9,  6,  6,  7,  7,  7,  6,  7,
 /*  72 */ 7,  3,  5,  6,  6,  8,  7,  7,
 /*  80 */ 6,  7,  7,  6,  5,  7,  7,  8,
 /*  88 */ 5,  7,  6,  3,  3,  3,  3,  5,
 /*  96 */ 0,  5,  5,  5,  5,  5,  4,  5,
 /* 104 */ 5,  2,  2,  5,  2,  8,  5,  5,
 /* 112 */ 5,  5,  3,  5,  3,  5,  6,  6,
 /* 120 */ 5,  6,  4,  0,  0,  0,  0,  0,

};

fontStyle_t FontStyle_font_8 = 
{
    96, // Glyph count
    32, // First ascii code
    1, // Glyph width (bytes)
    8, // Glyph height (bytes)
    0, // Fixed width or 0 if variable
    font_8_widths,
    font_8_bitmaps
};
