import asyncio
import json
import websockets
import random


async def handle_connection(websocket):
    print('Client connected')

    # Simulated data recording
    while True:
        i = random.randrange(200, 300)
        random_data = json.dumps({"test": str(i)})
        await websocket.send(random_data)
        await asyncio.sleep(1)


async def main():
    async with websockets.serve(handle_connection, "localhost", 8080):
        await asyncio.Future()


asyncio.run(main())
