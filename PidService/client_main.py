

import zmq
import time


def main():
    ctx = zmq.Context.instance()
    socket = ctx.socket(zmq.REQ)
    socket.connect("tcp://127.0.0.1:12355")
    while True:
        for temp in [225, 226]:
            socket.send_json({ "pid_config":{"set_point": temp} })
            message = socket.recv_json()
            print(message)
            time.sleep(5)





if __name__ == "__main__":
    main()