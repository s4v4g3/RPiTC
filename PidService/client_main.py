

import zmq
import time


def main():
    ctx = zmq.Context.instance()
    socket = ctx.socket(zmq.REQ)
    socket.connect("tcp://10.4.1.110:12355")
    while True:
        for temp in [225, 226, 0]:
            socket.send_json({ "pid_config":{"set_point": temp} })
            message = socket.recv_json()
            print(message)
            time.sleep(15)





if __name__ == "__main__":
    main()