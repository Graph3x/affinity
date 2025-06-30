import asyncio
import struct
import websockets
from websockets.server import WebSocketServerProtocol

TCP_CLIENTS: set["TCPClientHandler"] = set()
WEBSOCKET_CLIENTS: set[WebSocketServerProtocol] = set()

UDP_PORT = 14959
TCP_PORT = 14960
WS_PORT = 14961

NORMAL_LOG = "affinity.db"
RAW_LOG = "raw.bin"

DATAGRAM_SIZE = 65


class TCPClientHandler(asyncio.Protocol):
    def __init__(self):
        self.transport = None

    def connection_made(self, transport):
        self.transport = transport
        TCP_CLIENTS.add(self)
        print(f"TCP client connected, count: {len(TCP_CLIENTS)}")

    def data_received(self, data):
        return

    def connection_lost(self, exc):
        TCP_CLIENTS.discard(self)
        print(f"TCP client disconnected, count: {len(TCP_CLIENTS)}")


class UDPServerProtocol(asyncio.DatagramProtocol):

    def parse_datagram(self, datagram) -> str:
        try:
            (
                timestamp,
                temp,
                height,
                accelx,
                accely,
                accelz,
                gpsAlt,
                gpsLon,
                gpsLat,
                launchPin,
                status,
            ) = struct.unpack("<Lffddddddbi", datagram)
            msg = f"{timestamp}, {temp}C, {height}m, pin: {launchPin} status: {status}, accel: {accelx}, {accely}, {accelz} gps: {gpsAlt}, {gpsLon}, {gpsLat}"

        except:
            return ""

        return msg

    def parse_multi_datagram(self, datagram) -> list[str]:
        start = 0
        end = DATAGRAM_SIZE
        datapoints = []
        while end <= len(datagram):
            datapoint = self.parse_datagram(datagram[start:end])
            if datapoint != "":
                datapoints.append(datapoint + "\n")
            start += DATAGRAM_SIZE
            end += DATAGRAM_SIZE
        return datapoints

    def datagram_received(self, data, addr):
        print(f"Received UDP packet from {addr}: {data}")
        data_points = self.parse_multi_datagram(data)

        with open(RAW_LOG, "ab+") as f:
            f.write(data + b"\n")

        with open(NORMAL_LOG, "a+") as f:
            f.writelines(data_points)

        for client in list(TCP_CLIENTS):
            if client.transport.is_closing():
                TCP_CLIENTS.discard(client)
                continue
            for data_point in data_points:
                client.transport.write(data_point.encode())

        asyncio.create_task(self.send_to_websockets(data_points))

    async def send_to_websockets(self, data_points: list[str]):
        disconnected = set()
        for ws in WEBSOCKET_CLIENTS:
            try:
                for point in data_points:
                    await ws.send(point)
            except:
                disconnected.add(ws)

        for ws in disconnected:
            WEBSOCKET_CLIENTS.discard(ws)


async def websocket_handler(websocket: WebSocketServerProtocol):
    WEBSOCKET_CLIENTS.add(websocket)
    print(f"WebSocket client connected, count: {len(WEBSOCKET_CLIENTS)}")
    try:
        await websocket.wait_closed()
    finally:
        WEBSOCKET_CLIENTS.discard(websocket)
        print(
            f"WebSocket client disconnected, count: {len(WEBSOCKET_CLIENTS)}"
        )


async def main():
    loop = asyncio.get_running_loop()

    tcp_server = await loop.create_server(
        lambda: TCPClientHandler(), host="0.0.0.0", port=TCP_PORT
    )
    print(f"TCP server up on port {TCP_PORT}")

    udp_transport, udp_protocol = await loop.create_datagram_endpoint(
        lambda: UDPServerProtocol(), local_addr=("0.0.0.0", UDP_PORT)
    )
    print(f"UDP server up on port {UDP_PORT}")

    ws_server = await websockets.serve(websocket_handler, "0.0.0.0", WS_PORT)
    print(f"WebSocket server up on port {WS_PORT}")

    try:
        await tcp_server.serve_forever()
    except asyncio.CancelledError:
        print("DONE")
    finally:
        udp_transport.close()
        ws_server.close()
        await ws_server.wait_closed()


if __name__ == "__main__":
    asyncio.run(main())
