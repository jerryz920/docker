/*
   The FreeBSD Copyright

   Copyright 1992-2017 The FreeBSD Project. All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.  Redistributions in binary
   form must reproduce the above copyright notice, this list of conditions and
   the following disclaimer in the documentation and/or other materials
   provided with the distribution.  THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND
   CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
   NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
   PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
   OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
   OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
   ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

   The views and conclusions contained in the software and documentation are
   those of the authors and should not be interpreted as representing official
   policies, either expressed or implied, of the FreeBSD Project.


   The export header for external usage of this client side port management library
   Author: Yan Zhai

*/


#ifndef _LIBPORT_H_
#define _LIBPORT_H_

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif 

#define DEFAULT_DAEMON_PATH "/var/run/attguard.sock"
/// init:
//   * initialize the location of metadata service
//   * fill in the local port and exclude range
//   args:
//    * metadata_host: IP address or host name
//    * metadata_service: port address of metadata service


// log setting
void liblatte_set_log_level(int upto);


int liblatte_init(const char *myid, int run_as_iaas, const char *daemon_path);
int liblatte_create_principal_new(uint64_t uuid, const char *image, const char *config,
    int nport, const char *new_ip);
int liblatte_create_principal(uint64_t uuid, const char *image, const char *config,
    int nport);

int liblatte_create_principal_with_allocated_ports(uint64_t uuid, const char *image,
    const char *config, const char * ip, int port_lo, int port_hi);

/// Legacy API
int liblatte_create_image(const char *image_hash, const char *source_url,
    const char *source_rev, const char *misc_conf);

int liblatte_post_object_acl(const char *obj_id, const char *requirement);


/// legacy API
int liblatte_attest_principal_property(const char *ip, uint32_t port, const char *prop);

int liblatte_attest_principal_access(const char *ip, uint32_t port, const char *obj);

int liblatte_delete_principal(uint64_t uuid);
int liblatte_delete_principal_without_allocated_ports(uint64_t uuid);

/// helper

char* liblatte_get_principal(const char *ip, uint32_t lo, char **principal,
    size_t *size);

char* liblatte_get_local_principal(uint64_t uuid, char **principal,
    size_t *size);

int liblatte_get_metadata_config_easy(char *url, size_t *url_sz);

char* liblatte_get_metadata_config(char **metadata_config, size_t *size);
int endorse_principal(const char *ip, uint32_t port, uint64_t gn, int type,
    const char *property);

int liblatte_revoke_principal(const char *, uint32_t , int , const char *);

int liblatte_endorse_image(const char *id, const char *config, const char *property) ;
int liblatte_endorse_source(const char *url, const char *rev, const char *config,
    const char *property) ;

int liblatte_revoke(const char *, const char *, int , const char *);

int liblatte_endorse_membership(const char *ip, uint32_t port, uint64_t gn, const char *master);

int liblatte_endorse_attester(const char *id, const char *config);

int liblatte_endorse_builder(const char *id, const char *config);
int liblatte_endorse_image_source(const char * id, const char * config, const char *url, const char *rev);


int liblatte_check_property(const char *ip, uint32_t port, const char *property);

int liblatte_check_access(const char *ip, uint32_t port, const char *object);
int liblatte_check_worker_access(const char *ip, uint32_t port, const char *object);

char* liblatte_check_attestation(const char *ip, uint32_t port, char **attestation,
    size_t *size);



#ifdef __cplusplus
}
#endif




#endif
