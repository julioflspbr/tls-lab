#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

#include "client_hello.h"

static inline void fill_in_cipher_suites(struct client_hello *);
static inline void fill_in_random(struct client_hello *);
static inline void fill_in_extensions(struct client_hello *);

struct client_hello * make_client_hello() {
	struct client_hello * hello = malloc(sizeof(struct client_hello));
	*hello = (struct client_hello){
		.legacy_version = 0x0303,
		.legacy_session_id = 0,
		.legacy_compression_methods = { 1, legacy_compression_method_null }
	};

	fill_in_cipher_suites(hello);
	fill_in_random(hello);
	fill_in_extensions(hello);

	return hello;
}

struct data * encode_client_hello(struct client_hello * hello) {
}

void destroy_client_hello(struct client_hello * client_hello) {
	free(client_hello->cipher_suites.cipher_suite);

	struct extensions * extensions = &client_hello->extensions;
	for (unsigned int i = 0; i < extensions->count; i++) {
		switch (extensions->extension[i].type) {
			case extension_type_supported_versions: {
				struct supported_versions_extension * extension = extensions->extension[i].data;
				free(extension->supported_version);
				break;
			}
			case extension_type_supported_groups: {
				struct supported_groups_extension * extension = extensions->extension[i].data;
				free(extension->supported_group);
			}
			default:
				break;
		}
		free(extensions->extension[i].data);
	}
	free(client_hello);
}

void destroy_data(struct data * data) {
	free(data->payload);
	free(data);
}

void fill_in_random(struct client_hello * hello) {
	arc4random_buf(hello->random, sizeof(hello->random));
}

void fill_in_cipher_suites(struct client_hello * hello) {
	hello->cipher_suites.count = 1;
	hello->cipher_suites.cipher_suite = malloc(sizeof(enum cipher_suite) * hello->cipher_suites.count);
	hello->cipher_suites.cipher_suite[0] = cipher_suite_tls_aes_128_gcm_sha256;
}

void fill_in_extensions(struct client_hello * hello) {
	hello->extensions.count = 3;
	hello->extensions.extension = malloc(sizeof(struct extension) * hello->extensions.count);

	// supported versions extension
	hello->extensions.extension[0].type = extension_type_supported_versions;
	struct supported_versions_extension * supported_versions_extension = hello->extensions.extension[0].data = malloc(sizeof(struct supported_versions_extension));
	supported_versions_extension->count = 1;
	supported_versions_extension->supported_version = malloc(sizeof(enum supported_version) * supported_versions_extension->count);
	supported_versions_extension->supported_version[0] = supported_version_tls_1_3;

	// supported named groups extension
	hello->extensions.extension[1].type = extension_type_supported_groups;
	struct supported_groups_extension * supported_groups_extension = hello->extensions.extension[1].data = malloc(sizeof(struct supported_groups_extension));
	supported_groups_extension->count = 1;
	supported_groups_extension->supported_group = malloc(sizeof(enum named_group) * supported_groups_extension->count);
	supported_groups_extension->supported_group[0] = named_group_secp256r1;

	// signature algorithm extension
	hello->extensions.extension[2].type = extension_type_signature_algorithms;
	struct signature_algorithms_extension * signature_algorithms_extension = hello->extensions.extension[2].data = malloc(sizeof(struct signature_algorithms_extension));
	signature_algorithms_extension->count = 1;
	signature_algorithms_extension->signature_algorithm = malloc(sizeof(enum signature_algorithm) * signature_algorithms_extension->count);
	signature_algorithms_extension->signature_algorithm[0] = signature_algorithm_ecdsa_secp256r1_sha256;
}

