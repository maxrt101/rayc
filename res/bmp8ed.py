#!/usr/bin/env python3
from dataclasses import dataclass
from enum import Enum
import curses


@dataclass
class Position:
    y: int = 0
    x: int = 0


class ColorPair(Enum):
    TITLE = 1
    ONE = 2
    ZERO = 3
    CURSOR = 4

    def color(self):
        return curses.color_pair(self.value)


class CursorState(Enum):
    INVISIBLE = 0
    NORMAL = 1
    HIGH_VISIBILITY = 2


class Editor:
    BLOCK = '█'
    GRID_SIZE = 8
    FLIP_X = True

    def __init__(self, stdscr):
        self.scale = Position(2, 4)
        self.pattern_char_scale = 2
        self.cursor = Position(0, 0)
        self.center = Position(curses.LINES // 2, curses.COLS // 2)
        self.start = Position(5, self.center.x - self.GRID_SIZE // 2 * self.scale.x)

        self._pattern = [['0' for _ in range(self.GRID_SIZE)] for _ in range(self.GRID_SIZE)]

        curses.init_pair(ColorPair.TITLE.value, curses.COLOR_WHITE, curses.COLOR_BLACK)
        curses.init_pair(ColorPair.ONE.value, curses.COLOR_RED, curses.COLOR_BLACK)
        curses.init_pair(ColorPair.ZERO.value, curses.COLOR_WHITE, curses.COLOR_BLACK)
        curses.init_pair(ColorPair.CURSOR.value, curses.COLOR_GREEN, curses.COLOR_BLACK)

        curses.curs_set(CursorState.INVISIBLE.value)

        self.stdscr = stdscr

    def cleanup(self):
        curses.curs_set(CursorState.NORMAL.value)

    def write_string_center(self, y: int, msg: str, color: ColorPair, attrs: int = 0):
        self.stdscr.addstr(y, self.center.x - len(msg) // 2, msg, curses.color_pair(color.value) | attrs)

    def write_pattern(self):
        for y in range(self.GRID_SIZE):
            for x in range(self.GRID_SIZE):
                ch = self.pattern(y, x)
                color = ColorPair.ZERO.color() if ch == '0' else ColorPair.ONE.color()

                if self.cursor.y == y and self.cursor.x == x:
                    color = ColorPair.CURSOR.color()

                self.stdscr.addstr(
                    self.start.y + y * self.scale.y,
                    self.start.x + x * self.scale.x,
                    self.BLOCK * self.pattern_char_scale,
                    color
                )

    def process_input(self, ch):
        if ch == curses.KEY_LEFT:
            if self.cursor.x > 0:
                self.cursor.x -= 1
        elif ch == curses.KEY_RIGHT:
            if self.cursor.x < self.GRID_SIZE - 1:
                self.cursor.x += 1
        elif ch == curses.KEY_DOWN:
            if self.cursor.y < self.GRID_SIZE - 1:
                self.cursor.y += 1
        elif ch == curses.KEY_UP:
            if self.cursor.y > 0:
                self.cursor.y -= 1
        elif ch == ord(' '):
            if self.pattern(self.cursor.y, self.cursor.x) == '0':
                self.pattern(self.cursor.y, self.cursor.x, '1')
            else:
                self.pattern(self.cursor.y, self.cursor.x, '0')

    def run(self):
        self.write_string_center(2, 'Bitmap 8x8 Editor', ColorPair.TITLE, curses.A_BOLD)

        self.write_string_center(
            self.start.y + self.GRID_SIZE * self.scale.y,
            'Use arrow keys to move',
            ColorPair.TITLE
        )

        self.write_string_center(
            self.start.y + self.GRID_SIZE * self.scale.y + 1,
            'Press space to change cell state',
            ColorPair.TITLE
        )

        self.write_string_center(
            self.start.y + self.GRID_SIZE * self.scale.y + 1,
            'Press ^C to exit and print the pattern',
            ColorPair.TITLE
        )

        try:
            while 1:
                self.write_pattern()

                ch = self.stdscr.getch(
                    self.start.y + self.cursor.y * self.scale.y,
                    self.start.x + self.cursor.x * self.scale.x
                )

                self.process_input(ch)

                self.stdscr.move(
                    self.start.y + self.cursor.y * self.scale.y,
                    self.start.x + self.cursor.x * self.scale.x
                )

                self.stdscr.refresh()

        except KeyboardInterrupt:
            return

    def pattern(self, y: int, x: int, value: str = None) -> str | None:
        if value:
            self._pattern[y][x] = value
            return None
        else:
            return self._pattern[y][x]

    def dump(self, add_comma: bool = True):
        str_bytes = []
        for y in self._pattern:
            if self.FLIP_X:
                y.reverse()
            str_bytes.append('0b' + ''.join(y))
            if add_comma:
                str_bytes[-1] += ','
        return str_bytes


def main(stdscr):
    editor = Editor(stdscr)
    editor.run()
    editor.cleanup()
    return editor.dump()


if __name__ == '__main__':
    result = curses.wrapper(main)
    print('\n'.join(result))
