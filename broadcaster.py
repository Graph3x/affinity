import asyncio
import json
import websockets
import os
import random

I = 0.5

async def handle_connection(websocket):
    print('Client connected - sending data')

    while True:
        with open("data.cj", "rb") as f:
            try:
                f.seek(-2, os.SEEK_END)
                while f.read(1) != b'\n':
                    f.seek(-2, os.SEEK_CUR)
            except OSError:
                f.seek(0)
            last_line = f.readline().decode()
        data = json.dumps(json.loads(last_line))
        print(data)
        await websocket.send(data)
        await asyncio.sleep(2)


async def debug(websocket):
    print('Client connected - sending data')

    data = json.dumps({"time": 0, "height": f"{random.randrange(0, 999)}.{random.randrange(0, 99)}", "speed": f"{random.randrange(0, 999)}.{random.randrange(0, 99)}",
                      "temp": f"{random.randrange(0, 99)}.{random.randrange(0, 9)}", "pressure": f"{random.randrange(0, 999)}.{random.randrange(0, 99)}", "op_code": 3})
    print(data)
    await websocket.send(data)
    await asyncio.sleep(2)

    # Simulated data recording
    while True:

        global I

        data = json.dumps({"time": I,"height": f"{random.randrange(0, 999)}.{random.randrange(0, 99)}", "speed": f"{random.randrange(0, 999)}.{random.randrange(0, 99)}",
                          "temp": f"{random.randrange(0, 99)}.{random.randrange(0, 9)}", "pressure": f"{random.randrange(0, 999)}.{random.randrange(0, 99)}", "op_code": 4})
        print(data)

        I += 0.5

        await websocket.send(data)
        await asyncio.sleep(0.5)


async def main():
    print("Service online")
    async with websockets.serve(debug, "localhost", 8080):
        await asyncio.Future()


asyncio.run(main())
