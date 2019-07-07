import serial
import tkinter as tk



class Controller:

    def __init__(self):
        self.BAUD = 57600
        self.SER = serial.Serial("/dev/ttyUSB0", self.BAUD)
        self.pressed = {}
        self.prevPressed = {}
        self._initPresses()
        self._create_ui()


    def _sendCommand(command):
        self.SER.write(command)

    def _initPresses(self):
        self.pressed["w"] = False
        self.pressed["a"] = False
        self.pressed["s"] = False
        self.pressed["d"] = False
        self.pressed["q"] = False
        self.pressed["e"] = False
        self.pressed["p"] = False
        self.pressed["k"] = False
        self.pressed["i"] = False
        self.pressed["1"] = False
        self.pressed["2"] = False
        self.pressed["3"] = False
        self.prevPressed["w"] = False
        self.prevPressed["a"] = False
        self.prevPressed["s"] = False
        self.prevPressed["d"] = False
        self.prevPressed["q"] = False
        self.prevPressed["e"] = False
        self.prevPressed["p"] = False
        self.prevPressed["k"] = False
        self.prevPressed["i"] = False
        self.prevPressed["1"] = False
        self.prevPressed["2"] = False
        self.prevPressed["3"] = False

    def start(self):
        self._check_key_press()
        self.root.mainloop()

    def _check_for_press(self, key, command):
        if self._is_pressed(key):
            self.prevPressed[key] = True
            self.SER.write(key.encode())
            print(key + " pressed")

    def _check_for_release(self, key, command):
        if self._is_released(key):
            self.prevPressed[key] = False
            # self.SER.write(key.encode())
            print(key + " released")

    def _check_key_press(self):
        self._check_for_press("w", 'w')
        self._check_for_release("w", 'w')
        self._check_for_press("s", 's')
        self._check_for_release("s", 's')
        self._check_for_press("d", 'd')
        self._check_for_release("d", 'd')
        self._check_for_press("a", 'a')
        self._check_for_release("a", 'a')
        self._check_for_press("q", 'q')
        self._check_for_release("q", 'q')
        self._check_for_press("e", 'e')
        self._check_for_release("e", 'e')
        self._check_for_press("p", 'p')
        self._check_for_release("p", 'p')
        self._check_for_press("k", 'k')
        self._check_for_release("k", 'k')
        self._check_for_press("i", 'i')
        self._check_for_release("i", 'i')
        self._check_for_press("1", '1')
        self._check_for_release("1", '1')
        self._check_for_press("2", '2')
        self._check_for_release("2", '2')
        self._check_for_press("3", '3')
        self._check_for_release("3", '3')

        self.root.after(20, self._check_key_press)

    def _is_pressed(self, key):
        return self.pressed[key] and self.prevPressed[key] == False

    def _is_released(self, key):
        return self.pressed[key] == False and self.prevPressed[key]

    def _create_ui(self):
        self.root = tk.Tk()
        self.root.geometry('400x300')
        self._set_bindings()

    def _set_bindings(self):
        for char in ["w","s","d", "a", "q", "e", "p", "k", "i", "1", "2", "3"]:
            self.root.bind("<KeyPress-%s>" % char, self._pressed)
            self.root.bind("<KeyRelease-%s>" % char, self._released)
            self.pressed[char] = False

    def _pressed(self, event):
        self.pressed[event.char] = True

    def _released(self, event):
        self.pressed[event.char] = False


def main():
    try:
        p = Controller()
        p.start()

    except KeyboardInterrupt:
        print("\n\nProgram Closed")

def usage():
    print("Available options:")
    print("-h, --host  Define RC car IP address")


if __name__ == "__main__":
    main()