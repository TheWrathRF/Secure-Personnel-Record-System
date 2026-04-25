#ifndef AUDIT_H
#define AUDIT_H

#include "crypto.h"
#include <time.h>

/* encrypted audit trail gets dumped here */
#define AUDIT_FILE "audit.log.enc"

/* max length for the action description text */
#define MAX_ACTION_LEN 128

/* max username len - same as auth.h but we dont want circular deps */
#define AUDIT_USER_LEN 32

/*
 * single audit log entry.
 * stored encrypted on disk so you cant just hexedit the log.
 * each entry also carries an HMAC so we can detect tampering
 */
typedef struct {
    time_t    timestamp;                     /* unix epoch time */
    char      username[AUDIT_USER_LEN];      /* who did it */
    char      action[MAX_ACTION_LEN];        /* what they did */
    unsigned char hmac[32];                  /* HMAC-SHA256 of the above fields */
} AuditEntry;

/*
 * these get stored on disk as encrypted blobs. we wrap each AuditEntry
 * in this container so we know how big the ciphertext is
 */
typedef struct {
    int enc_len;                              /* how many bytes of ciphertext */
    unsigned char enc_data[256];             /* encrypted AuditEntry blob */
} AuditRecord;


/*
 * audit_log - call this whenever something interesting happens.
 * builds an AuditEntry, computes its HMAC, encrypts the whole thing
 * with AES, and appends it to the audit file.
 *
 * username: who triggered the action (or "SYSTEM" for automated stuff)
 * action:   printf-style description like "Added record ID 42"
 */
void audit_log(const char *username, const char *action);

/*
 * audit_view - reads and decrypts all audit log entries, verifies
 * each HMAC, and prints them out. admin-only obv.
 * if an HMAC doesnt match, we flag that entry as TAMPERED
 */
void audit_view(void);

/*
 * audit_compute_hmac - computes HMAC-SHA256 over the timestamp+username+action
 * fields of an AuditEntry. used both for writing and verification.
 * output must be at least 32 bytes
 */
void audit_compute_hmac(const AuditEntry *entry, unsigned char *out_hmac);

#endif /* AUDIT_H */
