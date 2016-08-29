#include <openssl/opensslconf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/syslog.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/ts.h>
#include <openssl/bn.h>
#include "context.h"

/* Name of config entry that defines the OID file. */
#define ENV_OID_FILE "oid_file"

#define B_FORMAT_TEXT 0x8000
#define FORMAT_UNDEF 0
#define FORMAT_TEXT (1 | B_FORMAT_TEXT) /* Generic text */
#define FORMAT_ASN1 4                   /* ASN.1/DER */

static ASN1_OBJECT *txt2obj(const char *oid);

/* Reply related functions. */
static int reply_command(CONF *conf, char *section, char *engine, char *query,
                         char *passin, char *inkey, const EVP_MD *md,
                         char *signer, char *chain, const char *policy,
                         char *in, int token_in, char *out, int token_out,
                         int text);
static TS_RESP *read_PKCS7(BIO *in_bio);
int create_response(rfc3161_context *ct, char *query, TS_RESP_CTX *resp_ctx,
                    int *resp_size, unsigned char **resp);
static ASN1_INTEGER *serial_cb(TS_RESP_CTX *ctx, void *data);
static ASN1_INTEGER *next_serial(const char *serialfile);
static int save_ts_serial(const char *serialfile, ASN1_INTEGER *serial);
TS_RESP_CTX *create_tsctx(rfc3161_context *ct, CONF *conf, const char *section,
                          const char *policy);
int add_oid_section(rfc3161_context *ct, CONF *conf);
