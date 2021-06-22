import time
import serial
import sys,tty,termios
class _Getch:
    def __call__(self):
        fd = sys.stdin.fileno()
        old_settings = termios.tcgetattr(fd)
        try:
            tty.setraw(sys.stdin.fileno())
            ch = sys.stdin.read(1)
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
        return ch

if len(sys.argv) < 1:
    print ("No port input")
s = serial.Serial(sys.argv[1])
while 1:
    x = float(input("x >= 18: "))
    y = float(input("y >= 7: "))
    x_turn = 18
    y_turn = 6
    x_depth = 27
    x_remain = x_depth + x - x_turn
    y_remain = y - y_turn
    x_fraction = x_remain / 18.0 * 0.8 + 0.3
    y_fraction = y_remain / 18.0 * 0.8 + 0.3
    

    s.write("/goStraight/run 100 {} \n".format(1).encode())
    time.sleep(y_fraction)
    s.write("/turn/run 100 -0.3 \n".encode())
    time.sleep(1.9)
    s.write("/goStraight/run 100 {} \n".format(1).encode())
    time.sleep(x_fraction)
    s.write("/stop/run \n".encode())

