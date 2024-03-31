import socket
import threading
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.backends import default_backend
import secrets

# Define server address and port
TCP_PORT = 21567  # Separate port for TCP communication
SERVER_ADDRESS = ("localhost", 21567)

# Player data structure
class Player:
    def __init__(self, id, username, position, health):
        self.id = id
        self.username = username
        self.position = position
        self.health = health

# Function to handle TCP login requests
def handle_login_request(client_socket, address):
    # Receive encrypted login credentials
    data = client_socket.recv(1024)
    # Decrypt and parse credentials (implementation of decryption needed)
    username, password = decrypt(data)

    # Validate credentials (replace with your authentication system)
    if username == "admin" and password == "secret":
        player_id = 1  # Replace with unique ID generation
        player = Player(player_id, username, (0, 0), 100)
        send_tcp_message(client_socket, "Login successful", player_id)
        return player
    else:
        send_tcp_message(client_socket, "Login failed", None)
        return None


# gets socket and string to send
def send(client, msg: str):
    send_bytes(client, msg.encode())
    
def send_bytes(client, msg: bytes):
    msg_length = len(msg) # get length int
    msg_length = msg_length.to_bytes(2, byteorder='little') # int to byte
    full_message = msg_length + msg # append msg length to msg 
    client.send(full_message)

# returns msg bytes
def recvfrom(client) -> bytes:

    try:
        msg_length_bytes = client.recv(2)

        if len(msg_length_bytes) != 2:
            raise ConnectionError("Wrong message length received")

        msg_length = int.from_bytes(msg_length_bytes, byteorder='little')

        # Receive the actual message data
        message = client.recv(msg_length)

        if len(message) != msg_length:
            raise ConnectionError("Incomplete message received")

        return message

    except socket.error as e:
        raise ConnectionError(f"Socket error while receiving data: {e}") from e

# # Function to send encrypted messages over TCP
# def send_tcp_message(client_socket, message, data):
#     # Encrypt message and data (implementation of encryption needed)
#     encrypted_data = encrypt((message, data))
#     client_socket.sendall(encrypted_data)

# # Function to handle UDP game updates
# def handle_game_update(client_socket, address, player):
#     # Receive data from client
#     data = client_socket.recvfrom(1024)

#     # Unpack data (replace with your game state representation)
#     position, direction, shooting = pickle.loads(data[0])

#     # Update player state in game world
#     player.position = position
#     # ... perform other game logic based on direction and shooting ...

#     # Broadcast updated game state to all players
#     broadcast_game_state(players)

# # Function to broadcast game state to all players
# def broadcast_game_state(players):
#     # Prepare game state data (replace with your game state representation)
#     game_state = [player.to_dict() for player in players]
#     data = pickle.dumps(game_state)

#     for player in players.values():
#         player.udp_socket.sendto(data, (SERVER_ADDRESS, address[1]))

def key_to_bytes(key):
    s = b''
    for i in range(16):
        s = int.to_bytes(key & 255, 1) + s
        key >>= 8
    
    return s    

def pad_bytes(message_bytes: bytes, block_size):
    padding_size = block_size - (len(message_bytes) % block_size)
    padding = bytes([padding_size]) * padding_size
    return message_bytes + padding

def encrypt_AES(plaintext: bytes, key):
    blocks = pad_bytes(plaintext, 16)
    print("encrypting this: " + ' '.join([format(byte, '02X') for byte in blocks]))
    print(f"{len(blocks)=}")
    backend = default_backend()
    cipher = Cipher(algorithms.AES(key), modes.ECB(), backend=backend)
    encryptor = cipher.encryptor()
    ciphertext = encryptor.update(blocks) + encryptor.finalize()
    return ciphertext

def decrypt_AES(cipherbytes: bytes, key):
    backend = default_backend()
    cipher = Cipher(algorithms.AES(key), modes.ECB(), backend=backend)
    decryptor = cipher.decryptor()
    plaintext = decryptor.update(cipherbytes) + decryptor.finalize()
    return plaintext

def handle_client(client_socket, address):
    global players
    
    # Diffie Hellman
    (p, g) = 170141183460469231731687303715884105757, 340282366920938463463374607431768211507
    secret = secrets.randbits(128)
    
    
    # X2
    x2 = pow(g, secret, p)
    send(client_socket, 'X' + str(x2) + 'X')
    
    
    # X1
    x1 = recvfrom(client_socket).decode()
    if(not (x1[0] == 'X' and x1[-1] == 'X')):
        print("Incorrect X1 received. disconnecting client")
        send(client_socket, "ERROR")
    
    x1 = int(x1[1:-1])
    
    # Encryption
    
    #CREATE AES KEY
    #aes_key = get_random_bytes(32)
    
    # Diffie Hellman
    # common paint
    

    
    # Key
    key = pow(x1, secret, p)
    # print("Key: " + str(key))
    
    key_bytes = key_to_bytes(key)
    
    
    #print("key_bytes: " + str(key_bytes))
    
    cipherbytes = recvfrom(client_socket)
    # print("cipherbytes: " + ciphertext.hex())
    
    message = decrypt_AES(cipherbytes, key_bytes)
    
    print("got this: " + str(message))
    
    cipherbytes = encrypt_AES(b"Bitch Nigga", key_bytes)
    print("sending this: " + ' '.join([format(byte, '02X') for byte in cipherbytes]))
    
    send_bytes(client_socket, cipherbytes)
    
    # player = handle_login_request(client_socket, address)
    # if player:
    #     players[player.id] = player
    #     player.udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    #     # Start separate thread for game updates
    #     # threading.Thread(target=handle_game_update, args=(player.udp_socket, address, player)).start()

    #     # ... handle disconnections and other events ... 

    client_socket.close()

# Main server function
def main():
    # TCP
    tcp_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    tcp_socket.bind(SERVER_ADDRESS)
    tcp_socket.listen(5)

    # UDP
    # udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    # udp_socket.bind(SERVER_ADDRESS)

    players = {}

    # Threading for concurrent client handling

    print("Server started on", SERVER_ADDRESS)

    while True:
        client_socket, address = tcp_socket.accept()
        threading.Thread(target=handle_client, args=(client_socket, address)).start()

if __name__ == "__main__":
    main()
