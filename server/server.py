import socket
import threading


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
def send(client, msg):
    message = msg.encode() # string to bytes
    msg_length = len(message) # get length int
    msg_length = msg_length.to_bytes(2, byteorder='little') # int to byte
    full_message = msg_length + message # append msg length to msg 
    client.send(full_message)

# returns msg bytes
def recvfrom(client):

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

# Function to send encrypted messages over TCP
def send_tcp_message(client_socket, message, data):
    # Encrypt message and data (implementation of encryption needed)
    encrypted_data = encrypt((message, data))
    client_socket.sendall(encrypted_data)

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

def handle_client(client_socket, address):
    global players
    
    # x = client_socket.recv(1000)
    # print(x)
    
    x1 = recvfrom(client_socket).decode()
    
    if(not (x1[0] == 'X' and x1[-1] == 'X')):
        print("Incorrect X1 received. disconnecting client")
        send(client_socket, "ERROR")
    
    x1 = int(x1[1:-1])
    
    print("DEBUG: X1 is " + str(x1))
    # Encryption
    
    #CREATE AES KEY
    #aes_key = get_random_bytes(32)
    
    # Diffie Hellman
    # common paint
    (p, g) = 170141183460469231731687303715884105757, 340282366920938463463374607431768211507
    secret = 5261
    

    # secret color and mix
    x2 = pow(g, secret, p)
    
    print("DEBUG: X2 is " + str(x2))
    send(client_socket, 'X' + str(x2) + 'X')
    
    
    key = pow(x1, secret, p)
    print("Key: " + str(key))
    #ENCRYPT AES KEY WITH RSA
    
    #SEND AES KEY TO CLIENT
    
    
    
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
