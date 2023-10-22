#include "Compression.hpp"
#include "IO.hpp"

#include <stdint.h>

namespace WebXO
{
    class TLS
    {
        private:
            // typedef enum {
            //     hello_request(0), client_hello(1), server_hello(2),
            //     certificate(11), server_key_exchange (12),
            //     certificate_request(13), server_hello_done(14),
            //     certificate_verify(15), client_key_exchange(16),
            //     finished(20), (255)
            // } HandshakeType;

            // struct {
            //     HandshakeType msg_type;    /* handshake type */
            //     uint24 length;             /* bytes in message */
            //     void HandleHandshakeMessage() {
            //         switch (msg_type) {
            //             case hello_request: HelloRequest();
            //             case client_hello: ClientHello();
            //             case server_hello: ServerHello();
            //             case certificate: Certificate();
            //             case server_key_exchange: ServerKeyExchange();
            //             case certificate_request: CertificateRequest();
            //             case server_hello_done: ServerHelloDone();
            //             case certificate_verify: CertificateVerify();
            //             case client_key_exchange: ClientKeyExchange();
            //             case finished: Finished();
            //         }
            //     }
            // } Handshake;

        public:
            // Record Header
            struct RecordHeader {
                char type;
                std::pair<char, char> version;
                int length;
            };

            // Handshake Header
            struct HandshakeHeader {
                char msg_type;
                int length;
            };

            // Client Version
            struct ClientVersion {
                char major;
                char minor;
            };

            // Client Random
            struct ClientRandom {
                int gmt_unix_time[4];
                char random_bytes[28];
            };

            // Session ID
            int session_id_length;


            // Constructor
            TLS() = default;
            // Destructor
            ~TLS() = default;

            // ClientHello (RFC 8446) function
            void RecvClientHello(CompBuffer tls);

            // ServerHello (RFC 8446) function
            void ServerHello(TLS* tls);
            // ServerHelloDone (RFC 8446) function
            static void ServerHelloDone(CompBuffer tls);
    };
}