Great, thanks for the additional information. Here's the updated README:

---

# Raycaster

Raycaster is a C++ SFML networking game featuring a 3D environment with real-time multiplayer interactions. Players can log in or sign up, move around, shoot, and engage in deathmatches with other players in the game.

## Features

- **Login/Signup**: Secure login and signup with AES encryption and Diffie-Hellman key exchange between the C++ client and Python server.
- **3D Environment**: Walk around and explore a 3D world with walls and textures.
- **Multiplayer Interaction**: See other players in the world, including their movements and actions.
- **Combat Mechanics**: Shoot other players, with visual and audio feedback for shooting and getting shot.
- **Death Animation**: Players have a death animation and respawn mechanism after being shot three times.
- **Leaderboard**: Tracks and displays scores based on the number of kills.
- **Notifications**: In-game notifications for kills.
- **Sky Texture**: Dynamic environment with a sky texture.

## Security Details

- **Encryption**: Utilizes Diffie-Hellman to create keys for AES encryption. All login information is passed using TCP for lossless transmission.
- **Password Handling**: Passwords are hashed and salted before being stored, ensuring no plaintext passwords are saved on the server.
- **Networking Protocol**: After successful login, game communication switches to UDP for faster performance. A custom protocol is used for game data, and all UDP communication is secured using AES encryption with the previously established key.

## Installation

### Client (C++)

1. **Requirements**:
   - C++ compiler (Visual Studio recommended)
   - SFML library
   - OpenSSL library

2. **Steps**:
   - Clone the repository.
   - Ensure SFML and OpenSSL libraries are installed and configured in your project.
   - Open the project in Visual Studio.
   - Build the project.

### Server (Python)

1. **Requirements**:
   - Python 3.x
   - Libraries: `cryptography`, `secrets`

2. **Steps**:
   - Install the required Python libraries:
     ```sh
     pip install cryptography secrets
     ```
   - Configure the server by editing `serveraddress.txt` in the server folder:
     ```
     <server_ip_address>
     <server_tcp_port>
     <server_udp_port>
     ```
   - Run the server:
     ```sh
     python server.py
     ```

## Usage

1. **Starting the Game**:
   - Run the server as described above.
   - Start the client application.
   - Use the login or signup page to enter the game.

2. **Controls**:
   - **Movement**: Use the keyboard to move around.
   - **Look Around**: Use the mouse to look up, down, and around.
   - **Shoot**: Use the mouse to shoot.

## Media

Screenshots and video demonstration links can be added here.

## Contact Information

For any questions or feedback, please contact:

Liad Koren  
Email: liadkoren@gmail.com

---

Feel free to add any images or video links, and let me know if there are any final adjustments you'd like to make!