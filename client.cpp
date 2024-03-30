#include "headers.hpp"
#include "client.hpp"
#include <iomanip>
#include <openssl/evp.h>
#include <openssl/rand.h>

void sendUDP()
{
    sf::UdpSocket socket;

    // Bind the socket to any available port
    if (socket.bind(sf::Socket::AnyPort) != sf::Socket::Done)
    {
        std::cerr << "Error binding UDP socket" << std::endl;
        return;
    }

    sf::IpAddress recipientIpAddress("127.0.0.1");

    std::string data = "Gunshot!";

    // Send the data to the recipient
    if (socket.send(data.c_str(), data.size(), recipientIpAddress, 4321) != sf::Socket::Done)
    {
        std::cerr << "Error sending UDP packet" << std::endl;
        return;
    }

    std::cout << "Sent UDP packet to " << "127.0.0.1" << ":" << 4321 << std::endl;

}


bool tryLogIn(const string& username, const string& password, string& error)
{
    if (username.size() <= 5)
    {
        error = "Username Must Be Longer Than 5 Characters";
        return false;
    }
    if (password.size() <= 5)
    {
        error = "Password Must Be Longer Than 5 Characters";
        return false;
    }


    sf::TcpSocket socket;
    
    // Connect to server
    if (socket.connect("127.0.0.1", 21567) != sf::Socket::Done)
    {
        error = "Failed To Connect To Server";
        return false;
    }
    
    bigint p("170141183460469231731687303715884105757");
    bigint g("340282366920938463463374607431768211507");
    bigint secret("21569");

    bigint x1 = powm(g, secret, p);


    string message = "X" + x1.str() + "X";

    sendTCP(socket, message);

    void* buffer;
    int buffer_size;
    if (!recvTCP(socket, buffer, buffer_size))
    {
        error = "Failed To Receive Message From Server";
        return false;
    }


    string x2_str((char*)buffer, buffer_size);
    if (x2_str == "ERROR" || x2_str[0] != 'X' || x2_str[buffer_size - 1] != 'X')
    {
        error = "Invalid X2 Received From Server: " + x2_str;
        return false;
    }

    
    bigint x2(x2_str.substr(1, buffer_size - 2));

    // cout << "X2: " << x2 << '\n';

    bigint key = powm(x2, secret, p);

    unsigned char* key_bytes = bigintToBytes(key);

    cout << "Key: " << key << "\n";
    cout << "Key length: " << 16 << "\n";
    cout << "Key Bytes: ";
    PrintBytes(key_bytes, 16);

    string encrypted = encryptAES("Nigger", key_bytes);
    
    cout << "encrypted: ";
    PrintBytes(reinterpret_cast<const unsigned char*>(encrypted.c_str()), encrypted.size());
    sendTCP(socket, encrypted);

    cout << "Decrypted: " << decryptAES(encrypted, key_bytes) << "\n";

    return true;
}

// returns true on success
bool recvTCP(sf::TcpSocket& socket, void*& buffer, int& buffer_size)
{
    //length
    short msg_len = 0;
    size_t amount_received;
    socket.receive(&msg_len, 2, amount_received);
    if (amount_received != 2)
    {
        cout << "Error when receiving msg length\n";
        return false;
    }

    buffer = malloc(msg_len);
    if (buffer == 0)
    {
        cout << "couldn't allocate space for payload\n";
        return false;
    }


    // message string
    socket.receive(buffer, msg_len, amount_received);

    //PrintBytes((char*)payload, payload_size);

    if (amount_received != msg_len)
    {
        cout << "Error when receiving message with length: " << msg_len << "\n";
        return false;
    }

    buffer_size = amount_received;
    return true;
}

void sendTCP(sf::TcpSocket& socket, const string& message)
{
    cout << "Sending: " << message << "\n";

    int payload_size = 2 + message.size();
    void* payload = malloc(payload_size);
    if (payload == 0)
    {
        cout << "couldn't allocate space for payload\n";
        return;
    }

    // length
    short msg_len = message.size();
    memcpy(payload, &msg_len, 2); 

    // message string
    memcpy((char*)payload + 2, message.c_str(), msg_len);


    size_t amount_sent;
    socket.send(payload, payload_size, amount_sent);

    if (amount_sent != payload_size)
        cout << "Error when sending message: " << message << "\n";

    free(payload);
}

void PrintBytes(const unsigned char* pBytes, const uint32_t nBytes) // should more properly be std::size_t
{
    for (uint32_t i = 0; i != nBytes; i++)
    {
        std::cout <<
            std::hex <<           // output in hex
            std::setw(2) <<       // each byte prints as two characters
            std::setfill('0') <<  // fill with 0 if not enough characters
            (int)pBytes[i] << " ";
    }
    cout << "\n";
}

string encryptAES(const string& plaintext, unsigned char* key) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        cout << "Error creating EVP_CIPHER_CTX." << std::endl;
        return "";
    }

    if (EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL) != 1) {
        cout << "Error initializing AES encryption." << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return "";
    }

    string ciphertext(plaintext.size() + EVP_CIPHER_block_size(EVP_aes_128_ecb()), '\0');
    int outLen;
    if (EVP_EncryptUpdate(ctx, reinterpret_cast<unsigned char*>(&ciphertext[0]), &outLen, reinterpret_cast<const unsigned char*>(plaintext.c_str()), plaintext.size()) != 1) {
        cout << "Error encrypting data." << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return "";
    }

    int finalLen;
    if (EVP_EncryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(&ciphertext[outLen]), &finalLen) != 1) {
        cout << "Error finalizing encryption." << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return "";
    }

    EVP_CIPHER_CTX_free(ctx);
    ciphertext.resize(outLen + finalLen);
    return ciphertext;
}

string decryptAES(const string& ciphertext, unsigned char* key) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        cout << "Error creating EVP_CIPHER_CTX." << std::endl;
        return "";
    }

    if (EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL) != 1) {
        cout << "Error initializing AES decryption." << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return "";
    }

    string plaintext(ciphertext.size(), '\0');
    int outLen;
    if (EVP_DecryptUpdate(ctx, reinterpret_cast<unsigned char*>(&plaintext[0]), &outLen, reinterpret_cast<const unsigned char*>(ciphertext.c_str()), ciphertext.size()) != 1) {
        cout << "Error decrypting data." << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return "";
    }

    int finalLen;
    if (EVP_DecryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(&plaintext[outLen]), &finalLen) != 1) {
        cout << "Error finalizing decryption." << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return "";
    }

    EVP_CIPHER_CTX_free(ctx);
    plaintext.resize(outLen + finalLen);
    return plaintext;
}

unsigned char* bigintToBytes(bigint key)
{
    unsigned char* buffer = (unsigned char*)malloc(16);
    if (buffer == nullptr)
        return nullptr;

    for (int i = 15; i >= 0; i--)
    {
        //cout << (key & 255) << " ";
        buffer[i] = (unsigned char) (key & 255);
        key >>= 8;
    }
    return buffer;
}

//string bigintToHexString(const bigint& number) {
//    std::stringstream ss;
//    ss << std::hex << number;
//    std::string hexString = ss.str();
//
//    // Pad with leading zeros if necessary
//    while (hexString.length() < 32) {
//        hexString = "0" + hexString;
//    }
//
//    return hexString;
//}

void c()
{

    /*previous_column = SHRT_MIN;

    steven_sprite.setColor(sf::Color(255, 255, 255, static_cast<unsigned char>(round(255 * std::min<float>(1, 2 * (1 - steven_distance / RENDER_DISTANCE))))));
    steven_sprite.setPosition(round(steven_screen_x - 0.5f * steven_size), round(floor_level - 0.5f * steven_size));
    steven_sprite.setScale(steven_size / static_cast<float>(CELL_SIZE), steven_size / static_cast<float>(CELL_SIZE));
    steven_sprite.setTextureRect(sf::IntRect(static_cast<unsigned short>(CELL_SIZE * floor(shifted_direction / frame_angle)), 0, CELL_SIZE, CELL_SIZE));

    i_window.draw(steven_sprite);*/
}