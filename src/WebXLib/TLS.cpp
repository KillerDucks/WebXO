#include "TLS.hpp"


/*

Using the following resource: https://tls12.xargs.org/#client-hello

*/

namespace WebXO {
    void TLS::ServerHello(TLS *) {
        // Send ServerHello message to client
        // Include protocol version, random number, session ID, cipher suite, and extensions

        // Wait for ClientHello message from client
        // Parse ClientHello message and extract protocol version, cipher suites, and extensions

        // Select a cipher suite that is supported by both server and client

        // Send ServerHelloDone message to client

        // Wait for ClientKeyExchange message from client

        // Use selected cipher suite to generate session keys

        // Send ChangeCipherSpec message to client

        // Send Finished message to client
        // return TLS::ServerHelloDone(CompBuffer((char*)"NULL", -1));
        return;
    }

  void TLS::RecvClientHello(CompBuffer tls)
    {
        // Record 
        RecordHeader rh;
        rh.type = tls.first[0];
        rh.version.first = tls.first[1];
        rh.version.second = tls.first[2];
        rh.length = (tls.first[3] << 8) | tls.first[4];

        // Handshake
        HandshakeHeader hh;
        hh.msg_type = tls.first[5];
        // Get the length of the handshake message 3 bytes long
        hh.length = (static_cast<uint8_t>(tls.first[6]) << 16) | (static_cast<uint8_t>(tls.first[7]) << 8) | static_cast<uint8_t>(tls.first[8]);

        // Client Version
        ClientVersion cv;
        cv.major = tls.first[9];
        cv.minor = tls.first[10];

        // Client Random
        ClientRandom cr;
        // GMT Unix Time Uses 4 bytes static casted to uint8_t
        cr.gmt_unix_time[0] = (static_cast<uint8_t>(tls.first[11] << 24)) | (static_cast<uint8_t>(tls.first[12] << 16)) | (static_cast<uint8_t>(tls.first[13] << 8)) | static_cast<uint8_t>(tls.first[14]);
        memcpy(cr.random_bytes, &tls.first[11], 32);

        // Session ID
        int session_id_length = tls.first[43];

        // Grab session_id_legnth bytes from the buffer
        char session_id[session_id_length];
        memcpy(session_id, &tls.first[44], session_id_length);

        // Get the first 2 bytes of the cipher suites length
        int cipher_suites_length = (static_cast<uint8_t>(tls.first[44 + session_id_length]) << 8) | static_cast<uint8_t>(tls.first[45 + session_id_length]);

        // Grab cipher_suites_length bytes from the buffer
        char cipher_suites[cipher_suites_length];
        memcpy(cipher_suites, &tls.first[46 + session_id_length], cipher_suites_length);

        // Get the compression methods 2 bytes
        int compression_methods_length = static_cast<uint8_t>(tls.first[46 + session_id_length + cipher_suites_length]);
        // Read the next byte
        char compression_methods[compression_methods_length];
        memcpy(compression_methods, &tls.first[47 + session_id_length + cipher_suites_length], compression_methods_length);

        // Get the Extensions Length (2 bytes)
        int extensions_length = (static_cast<uint8_t>(tls.first[47 + session_id_length + cipher_suites_length + compression_methods_length]) << 8) | static_cast<uint8_t>(tls.first[48 + session_id_length + cipher_suites_length + compression_methods_length]);

        // Grab the extensions_length bytes from the buffer
        char extensions[extensions_length];
        memcpy(extensions, &tls.first[49 + session_id_length + cipher_suites_length + compression_methods_length], extensions_length);

       

        // Print Record Header Type
        printf("Record Header Type: 0x%x\n", rh.type);

        // Print Record Header Version
        printf("Record Header Version: %d.%d\n", rh.version.first, rh.version.second);

        // Print Record Header Length
        printf("Record Header Length: %d\n", rh.length);

        // Print Handshake Header Message Type
        printf("Handshake Header Message Type: 0x%x\n", hh.msg_type);

        // Print Handshake Header Length
        printf("Handshake Header Length: %d\n", hh.length);

        // Print Client Version combined
        printf("Client Version: %d.%d\n", cv.major, cv.minor);

        // Print Client Random GMT Unix Time
        printf("Client Random GMT Unix Time: %d\n", ((uint8_t)cr.gmt_unix_time[0] << 24) | ((uint8_t)cr.gmt_unix_time[1] << 16) | ((uint8_t)cr.gmt_unix_time[2] << 8) | (uint8_t)cr.gmt_unix_time[3]);

        // Print Client Random Random Bytes
        printf("Client Random Random Bytes: ");
        for (int i = 0; i < 28; i++) {
            printf("%02X ", (uint8_t)cr.random_bytes[i]);
        }
        printf("\n");

        // Print Session ID Length
        printf("Session ID Length: %d\n", session_id_length);

        // Print Session ID
        printf("Session ID: ");
        for (int i = 0; i < session_id_length; i++) {
            printf("%02X ", (uint8_t)session_id[i]);
        }
        printf("\n");

        // Print Cipher Suites Length
        printf("Cipher Suites Length: %d\n", cipher_suites_length);

        // Print Cipher Suites
        printf("Cipher Suites: ");
        for (int i = 0; i < cipher_suites_length; i++) {
            printf("%02X ", (uint8_t)cipher_suites[i]);
        }
        printf("\n");

        // Print Compression Methods Length
        printf("Compression Methods Length: %d\n", compression_methods_length);

        // Print Compression Methods
        printf("Compression Methods: ");
        for (int i = 0; i < compression_methods_length; i++) {
            printf("%02X ", (uint8_t)compression_methods[i]);
        }
        printf("\n");

        // Print Extensions Length
        printf("Extensions Length: %d\n", extensions_length);

        // Print Extensions
        printf("Extensions: ");
        // Check the first 2 bytes of the extensions to determine the type
        switch (extensions[0] << 8 | extensions[1])
        {
            case 00:
                {           
                // Server Name Indication
                    printf("Server Name Indication\n");
                    // Print out the length of the extension
                    printf("Length: %d\n", (extensions[2] << 8 | extensions[3]));
                    // Get the bytes of the first list length
                    int list_length = (extensions[4] << 8 | extensions[5]);
                    // Print out the list length
                    printf("List Length: %d\n", list_length);
                    // Get the type of the list
                    int list_type = extensions[6];
                    // Print out the list type
                    printf("List Type: %d\n", list_type);
                    // Get the length of the list
                    int list_length2 = (extensions[7] << 8 | extensions[8]);
                    // Print out the list length
                    printf("List Length: %d\n", list_length2);
                    // Get the dns hostname (print characters until the end of the list)
                    printf("DNS Hostname: ");
                    for (int i = 9; i < list_length2 + 9; i++) {
                        printf("%c", extensions[i]);
                    }
                    printf("\n");   
                }
                break;
            
            default:
                printf("Unknown Extension\n");
                break;
        }



       return;
    }
}