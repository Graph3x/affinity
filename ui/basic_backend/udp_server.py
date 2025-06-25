import socket
import sqlite3
import signal
import sys

def main():
    conn = sqlite3.connect("affinity.db")
    cursor = conn.cursor()

    cursor.execute('''
    CREATE TABLE IF NOT EXISTS data (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        value1 INTEGER,
        value2 REAL,
        value3 INTEGER,
        description TEXT
    )
    ''')
    conn.commit()

    def graceful_exit(signum, frame):
        print("Shutting down...")
        conn.close()
        sys.exit(0)

    signal.signal(signal.SIGINT, graceful_exit)

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind(('0.0.0.0', 14959))

    print("Listening for UDP packets on port 14959...")
    while True:
        data, addr = sock.recvfrom(1024)
        print(f"Received from {addr}: {data.hex()}")
        """cursor.execute('INSERT INTO data (value1, value2, value3, description) VALUES (?, ?, ?, ?)',
                    (1, 2.3, 3, data))
        conn.commit()

        cursor.execute('SELECT * FROM data ORDER BY id DESC LIMIT 1')
        last_row = cursor.fetchone()"""

if __name__ == '__main__':
    main()