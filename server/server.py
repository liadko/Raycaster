import socket
import threading

from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.backends import default_backend
import secrets # for randbits, Diffie Hellman

from sql_orm import Users_db

import struct

# Player data structure
class Player:
    def __init__(self, username, id):
        self.username = username
        self.player_id = id
        self.position_x = -1
        self.position_y = -1
        self.rotation_x = -1
        self.rotation_y = -1


# Define server address and port
TCP_PORT = 21567  # Separate port for TCP communication
SERVER_ADDRESS = ("0.0.0.0", 3000)
players:list[Player] = []
struct_format = 'iffffi'
struct_size = struct.calcsize(struct_format)
player_binaries = b'\x00'
current_player_id = 0
key_bytes : dict = {}


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

def send_UDP(socket:socket.socket, address, msg: bytes):
    socket.sendto(msg, address)

# returns msg bytes
def recvUDP(udp_server : socket.socket) -> tuple[bytes, any]:

    try:
        msg_bytes, address = udp_server.recvfrom(33)


        # Receive the actual message data

        if len(msg_bytes) != 33:
            raise ConnectionError("Incomplete message received (len: {0})", len(msg_bytes))

        return msg_bytes, address

    except socket.error as e:
        raise ConnectionError(f"Socket error while receiving data: {e}") from e


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

def clean_bytes(dirty: bytes):
    return ' '.join([format(byte, '02X') for byte in dirty])

def unpad_bytes(padded_bytes: bytes):
    
    padding_size = padded_bytes[-1]  # Get the last byte, which represents the padding size
    if padding_size == 0 or padding_size > len(padded_bytes):
        raise ValueError(f"Invalid padding, looking at final char")
        
    # Verify that the padding bytes are all the same
    expected_padding = bytes([padding_size]) * padding_size
    if not padded_bytes.endswith(expected_padding):
        raise ValueError(f"Invalid padding, looking at the last {padding_size} bytes")

    # Remove the padding bytes
    unpadded_bytes = padded_bytes[:-padding_size]
    return unpadded_bytes

def encrypt_AES(plaintext: bytes, key):
    blocks = pad_bytes(plaintext, 16)
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
    return unpad_bytes(plaintext)

def add_player(username):
    global players, player_binaries, current_player_id
    
    players.append(Player(username, current_player_id))
    
    
    player_binaries += int.to_bytes(current_player_id, 1) * struct_size
    current_player_id += 1
    
    player_binaries = int.to_bytes(len(players), 1) + player_binaries[1:]

def remove_player(player_id):
    # Iterate through the players list
    for player in players:
        # Check if the current player's username matches the target username
        if player.player_id == player_id:
            # Remove the player from the list
            players.remove(player)
            print(f"Player with id #{player_id} removed successfully.")
            return
    # If the player with the specified username is not found
    print(f"Player with id #{player_id} not found.")
    

def handle_client(client_socket, address, users_db:Users_db, lock: threading.Lock):
    global players, key_bytes, current_player_id
    
    

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
    
    # Key
    key = pow(x1, secret, p)
    
    current_key_bytes = key_to_bytes(key)
    
    #print("key_bytes: " + str(key_bytes))
    
    cipherbytes = recvfrom(client_socket)
    # print("cipherbytes: " + ciphertext.hex())
    
    
    message = decrypt_AES(cipherbytes, current_key_bytes)
    
    print("got this: " + str(message))
    
    parts = message.decode().split('~')
    print(parts)
    
    # users_db.insert_new_user(parts[1], parts[2])
    # users_db.remove_user(parts[1])

    lock.acquire()
    
    response = "ERROR~Message Unrecognized"
    if(parts[0] == "LOGIN"):
        if(users_db.user_exists(parts[1], parts[2])):
            response = "SUCCESS~" + str(current_player_id) + "~"
            key_bytes[current_player_id] = current_key_bytes
            
            add_player(parts[1])
            
        else:
            response = "FAIL~Username Or Password Incorrect~"
    elif(parts[0] == "SIGNUP"):
        if(users_db.username_exists(parts[1])):
            response = "FAIL~Username Already Exists~"
        elif(users_db.insert_new_user(parts[1], parts[2])):
            response = "SUCCESS~"
        else:
            response = "FAIL~Can't Add User For Some Reason~"
            
        
    
    lock.release()
    # print("sending this: " + ' '.join([format(byte, '02X') for byte in cipherbytes]))
    print(f"{response=}")
    send_bytes(client_socket, encrypt_AES(response.encode(), current_key_bytes))
    
    
    
    client_socket.close()

def update_player(player_info: bytes):
    global players, player_binaries
    
    player_id, pos_x, pos_y, rot_x, rot_y, flags = struct.unpack(struct_format, player_info)
    
    player = None
    for p in players:
        if(p.player_id == player_id):
            player = p
            break
        
    if player == None:
        print("Got message from nonexistant player, id=", player_id)
        return
    
    player.position_x = pos_x
    player.position_y = pos_y
    player.rotation_x = rot_x
    player.rotation_y = rot_y
    
    has_quit = flags & 8
    
    if(has_quit):
        remove_player(player_id)
        player_binaries = int.to_bytes(len(players), 1) + player_binaries[1:]
        player_info = b''
    
    
    #print("got update from player, here's the playerbinaries: ", clean_bytes(player_binaries))
    
    buffer_index = 1
    while(True):
        if(buffer_index >= len(player_binaries)):
            raise Exception("Something fucked when updating the player binaries", buffer_index)

        if(player_binaries[buffer_index] == player_id):
            break
            
        buffer_index += struct_size
                
    
    player_binaries = player_binaries[:buffer_index] + player_info + player_binaries[buffer_index+struct_size:]
    


# def get_others_info(player_id):
#     global players
    
#     info = b''
#     others_count = 0
#     for i in range(len(players)):
#         if(i == player_id): continue
#         player = players[i]
#         info += struct.pack("ifffi", i, player.position_x, player.position_y, player.rotation_x, player.is_moving)
#         others_count += 1
        
#     info = int.to_bytes(others_count, 4, 'little') + info 
    
#     return info
        
        

def handle_game():
    global key_bytes, player_binaries
    
    # UDP
    udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    udp_socket.bind(('0.0.0.0', 3001))

    
    
    while(True):
        msg, address = recvUDP(udp_socket)
        
        player_id, encrypted = msg[0], msg[1:]
        
        correct_key_bytes = key_bytes[player_id]
        
        
        try:
            player_info = decrypt_AES(encrypted, correct_key_bytes)
        except Exception as e:
            print(f"invalid message from {address}, error: {e}")
            print(f"encrypted:", clean_bytes(encrypted))
            print()
            continue
        
        update_player(player_info)
        
        # print(f"Sending Player #{player_id} These Binaries:", player_binaries[0])
        # for i in range(len(players)):
        #     print(f"player {i}.", clean_bytes(player_binaries[1+struct_size*i:1+struct_size*i+struct_size]))
        # print()
        
        #others = get_others_info(player_id)
        
        udp_socket.sendto(encrypt_AES(player_binaries, correct_key_bytes), address)
        
        
        #print("boutta send UDP: " + ' '.join([format(byte, '02X') for byte in others]))
        

# Main server function
def main():
    global players
    # TCP
    tcp_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    tcp_socket.bind(SERVER_ADDRESS)
    tcp_socket.listen(5)

    
    users_db = Users_db()
    lock = threading.Lock()
    
    threading.Thread(target=handle_game).start()

    # Threading for concurrent client handling

    print("Server started on", SERVER_ADDRESS)

    while True:
        client_socket, address = tcp_socket.accept()
        threading.Thread(target=handle_client, args=(client_socket, address, users_db, lock)).start()

if __name__ == "__main__":
    main()
