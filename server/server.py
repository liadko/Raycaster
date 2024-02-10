import socket

UDP_IP = "0.0.0.0"  
UDP_PORT = 4321


sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)


sock.bind((UDP_IP, UDP_PORT))

print("Server listening on UDP port", UDP_PORT)

while True:
    # Receive data and the address of the client
    data, addr = sock.recvfrom(1024)  # Buffer size is 1024 bytes
    
    # Print received data and the client's address
    # print("Received UDP packet from:", addr)
    print("Data:", data.decode())