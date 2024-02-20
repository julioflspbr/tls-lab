#ifndef CLIENT_HELLO_H
#define CLIENT_HELLO_H

struct data {
  void * payload;
  unsigned int size;
};

enum handshake_type: unsigned char {
  handshake_type_client_hello = 1,
  handshake_type_server_hello = 2,
  handshake_type_new_session_ticket = 4,
  handshake_type_end_of_early_data = 5,
  handshake_type_encrypted_extensions = 8,
  handshake_type_certificate = 11,
  handshake_type_certificate_request = 13,
  handshake_type_certificate_verify = 15,
  handshake_type_finished = 20,
  handshake_type_key_update = 24,
  handshake_type_message_hash = 254
};

enum extension_type: unsigned short int {
  extension_type_server_name = 0,
  extension_type_max_fragment_length = 1,
  extension_type_status_request = 5,
  extension_type_supported_groups = 10,
  extension_type_signature_algorithms = 13,
  extension_type_use_srtp = 14,
  extension_type_heartbeat = 15,
  extension_type_application_layer_protocol_negotiation = 16,
  extension_type_signed_certificate_timestamp = 18,
  extension_type_client_certificate_type = 19,
  extension_type_server_certificate_type = 20,
  extension_type_padding = 21,
  extension_type_pre_shared_key = 41,
  extension_type_early_data = 42,
  extension_type_supported_versions = 43,
  extension_type_cookie = 44,
  extension_type_psk_key_exchange_modes = 45,
  extension_type_certificate_authorities = 47,
  extension_type_oid_filters = 48,
  extension_type_post_handshake_auth = 49,
  extension_type_signature_algorithms_cert = 50,
  extension_type_key_share = 51
};

enum supported_version: unsigned short int {
  supported_version_tls_1_3 = 0x0304
};

struct supported_versions_extension {
  unsigned short int count;
  enum supported_version * supported_version;
};

enum named_group: unsigned short int {
  named_group_secp256r1 = 0x0017
};

struct supported_groups_extension {
  unsigned short int count;
  enum named_group * supported_group;
};

enum signature_algorithm: unsigned short int {
  signature_algorithm_ecdsa_secp256r1_sha256 = 0x0403
};

struct signature_algorithms_extension {
  unsigned short int count;
  enum signature_algorithm * signature_algorithm;
};

struct extension {
  enum extension_type type;
  void * data;
};

struct extensions {
  struct extension * extension;
  unsigned short int count;
};

enum cipher_suite: unsigned short int {
  cipher_suite_tls_aes_128_gcm_sha256 = 0x1301
};

struct cipher_suites {
  unsigned short int count;
  enum cipher_suite * cipher_suite;
};

enum legacy_compression_method: unsigned char {
  legacy_compression_method_null = 0x0
};

struct legacy_compression_methods {
  unsigned char count;
  enum legacy_compression_method compression_method;
};

struct client_hello {
  unsigned short legacy_version;
  unsigned char random[32];
  unsigned char legacy_session_id;
  struct cipher_suites cipher_suites;
  struct legacy_compression_methods legacy_compression_methods;
  struct extensions extensions;
};

struct client_hello * make_client_hello(void);
struct data * encode_client_hello(struct client_hello *);
void destroy_client_hello(struct client_hello *);
void destroy_data(struct data *);

#endif

