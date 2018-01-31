
#include "libport.h"

#include <stdio.h>

#define REPORT(name) printf("%s is %s\n", #name, name)
#define REPORTI(name) printf("%s is %d\n", #name, name)
#define REPORTLU(name) printf("%s is %lu\n", #name, name)


int libport_init(const char *server_url, const char *persistence_path, int run_as_iaas)
  __attribute__((weak, alias("__libport_init")));

int __libport_init(const char *server_url, const char *persistence_path, int run_as_iaas) {
  fprintf(stderr, "depracted: should use liblatte_init\n");
  return 0;
}

int liblatte_init(const char *myid, int run_as_iaas, const char *daemon_path)
  __attribute__((weak, alias("__liblatte_init")));
int __liblatte_init(const char *myid, int run_as_iaas, const char *daemon_path) {
  REPORT(myid);
  REPORT(daemon_path);
  REPORTI(run_as_iaas);
  return 0;
}

int libport_reinit(const char *server_url, const char *persistence_path, int run_as_iaas)
  __attribute__((weak, alias("__libport_reinit")));
int __libport_reinit(const char *server_url, const char *persistence_path, int run_as_iaas) {
  fprintf(stderr, "depracted: should use liblatte_init\n");
  return 0;
}

//// DEPRACATED
int create_principal(uint64_t uuid, const char *image, const char *config, int nport) 
  __attribute__((weak, alias("__create_principal")));
int __create_principal(uint64_t uuid, const char *image, const char *config, int nport) {
  REPORTLU(uuid);
  REPORT(image);
  REPORT(config);
  REPORTI(nport);
  return 40000;
}


int create_image(const char *image_hash, const char *source_url,
    const char *source_rev, const char *misc_conf)
  __attribute__((weak, alias("__create_image")));
int __create_image(const char *image_hash, const char *source_url,
    const char *source_rev, const char *misc_conf) {
  REPORT(image_hash);
  REPORT(source_url);
  REPORT(source_rev);
  REPORT(misc_conf);
  return 0;
}

int post_object_acl(const char *obj_id, const char *requirement) 
  __attribute__((weak, alias("__post_object_acl")));
int __post_object_acl(const char *obj_id, const char *requirement) {
  return 0;
}

int endorse_image(const char *image_hash, const char *endorsement)
  __attribute__((weak, alias("__endorse_image")));
int __endorse_image(const char *image_hash, const char *endorsement) {
  return 0;
}

int attest_principal_property(const char *ip, uint32_t port, const char *prop)
  __attribute__((weak, alias("__attest_principal_property")));
int __attest_principal_property(const char *ip, uint32_t port, const char *prop) {
  return 0;
}
int attest_principal_access(const char *ip, uint32_t port, const char *obj) 
  __attribute__((weak, alias("__attest_principal_property")));
int __attest_principal_access(const char *ip, uint32_t port, const char *obj) {
  return 0;
}
//// DEPRACATE_END

/// delete_principal:
//   * remove a principal, and withdraw the mapping, as well as
//   * the statement (the last thing not implemented yet)
int delete_principal(uint64_t uuid) __attribute__((weak, alias("__delete_principal")));
int __delete_principal(uint64_t uuid) {
  REPORTLU(uuid);
  return 0;
}

// log setting
void libport_set_log_level(int upto) __attribute__((weak, alias("__libport_set_log_level")));
void liblatte_set_log_level(int upto) __attribute__((weak, alias("__libport_set_log_level")));
void __libport_set_log_level(int upto) {
}


int liblatte_create_principal_new(uint64_t uuid, const char *image, const char *config,
    int nport, const char *new_ip) 
  __attribute__((weak, alias("__liblatte_create_principal_new")));
int __liblatte_create_principal_new(uint64_t uuid, const char *image, const char *config,
    int nport, const char *new_ip) {
  return 0;
}

int liblatte_create_principal(uint64_t uuid, const char *image, const char *config,
    int nport)
  __attribute__((weak, alias("__liblatte_create_principal")));
int __liblatte_create_principal(uint64_t uuid, const char *image, const char *config,
    int nport) {
  return 0;
}

int liblatte_create_principal_with_allocated_ports(uint64_t uuid, const char *image,
    const char *config, const char * ip, int port_lo, int port_hi)
  __attribute__((weak, alias("__liblatte_create_principal_with_allocated_ports")));
int __liblatte_create_principal_with_allocated_ports(uint64_t uuid, const char *image,
    const char *config, const char * ip, int port_lo, int port_hi) {
  return 0;
}

/// Legacy API
int liblatte_create_image(const char *image_hash, const char *source_url,
    const char *source_rev, const char *misc_conf)
  __attribute__((weak, alias("__liblatte_create_image")));
int __liblatte_create_image(const char *image_hash, const char *source_url,
    const char *source_rev, const char *misc_conf) {
  return 0;
}

int liblatte_post_object_acl(const char *obj_id, const char *requirement)
  __attribute__((weak, alias("__liblatte_post_object_acl")));
int __liblatte_post_object_acl(const char *obj_id, const char *requirement) {
  return 0;
}


/// legacy API
int liblatte_attest_principal_property(const char *ip, uint32_t port, const char *prop)
  __attribute__((weak, alias("__liblatte_attest_principal_property")));
int __liblatte_attest_principal_property(const char *ip, uint32_t port, const char *prop) {
  return 0;
}

int liblatte_attest_principal_access(const char *ip, uint32_t port, const char *obj)
  __attribute__((weak, alias("__liblatte_attest_principal_access")));
int __liblatte_attest_principal_access(const char *ip, uint32_t port, const char *obj) {
  return 0;
}

int liblatte_delete_principal(uint64_t uuid)
  __attribute__((weak, alias("__liblatte_delete_principal")));
int __liblatte_delete_principal(uint64_t uuid) {
  return 0;
}
int liblatte_delete_principal_without_allocated_ports(uint64_t uuid) 
  __attribute__((weak, alias("__liblatte_delete_principal_without_allocated_ports")));
int __liblatte_delete_principal_without_allocated_ports(uint64_t uuid) {
  return 0;
}

/// helper

char* liblatte_get_principal(const char *ip, uint32_t lo, char **principal,
    size_t *size)
  __attribute__((weak, alias("__liblatte_get_principal")));
char* __liblatte_get_principal(const char *ip, uint32_t lo, char **principal,
    size_t *size) {
  return NULL;
}

char* liblatte_get_local_principal(uint64_t uuid, char **principal,
    size_t *size)
  __attribute__((weak, alias("__liblatte_get_local_principal")));
char* __liblatte_get_local_principal(uint64_t uuid, char **principal,
    size_t *size) {
  return NULL;
}

int liblatte_get_metadata_config_easy(char *url, size_t *url_sz)
  __attribute__((weak, alias("__liblatte_get_metadata_config_easy")));
int __liblatte_get_metadata_config_easy(char *url, size_t *url_sz) {
  return 0;
}

char* liblatte_get_metadata_config(char **metadata_config, size_t *size)
  __attribute__((weak, alias("__liblatte_get_metadata_config")));
char* __liblatte_get_metadata_config(char **metadata_config, size_t *size) {
}

int endorse_principal(const char *ip, uint32_t port, uint64_t gn, int type,
    const char *property)
  __attribute__((weak, alias("__endorse_principal")));
int __endorse_principal(const char *ip, uint32_t port, uint64_t gn, int type,
    const char *property) {
  return 0;
}

int liblatte_revoke_principal(const char *, uint32_t , int , const char *)
  __attribute__((weak, alias("__liblatte_revoke_principal")));
int __liblatte_revoke_principal(const char *ip, uint32_t port, int gn, const char *config) {
  return 0;
}

int liblatte_endorse_image(const char *id, const char *config, const char *property) 
  __attribute__((weak, alias("__liblatte_endorse_image")));
int __liblatte_endorse_image(const char *id, const char *config, const char *property) {
  return 0;
}

int liblatte_endorse_source(const char *url, const char *rev, const char *config,
    const char *property) 
  __attribute__((weak, alias("__liblatte_endorse_source")));
int __liblatte_endorse_source(const char *url, const char *rev, const char *config,
    const char *property) {
  return 0;
}

int liblatte_revoke(const char *, const char *, int , const char *)
  __attribute__((weak, alias("__liblatte_revoke")));
int __liblatte_revoke(const char *speaker, const char *ip, int port, const char *config) {
  return 0;
}

int liblatte_endorse_membership(const char *ip, uint32_t port, uint64_t gn, const char *master)
  __attribute__((weak, alias("__liblatte_endorse_membership")));
int __liblatte_endorse_membership(const char *ip, uint32_t port, uint64_t gn, const char *master) {
  return 0;
}

int liblatte_endorse_attester(const char *id, const char *config)
  __attribute__((weak, alias("__liblatte_endorse_attester")));
int __liblatte_endorse_attester(const char *id, const char *config) {
  return 0;
}


int liblatte_endorse_builder(const char *id, const char *config)
  __attribute__((weak, alias("__liblatte_endorse_builder")));
int __liblatte_endorse_builder(const char *id, const char *config) {
  return 0;
}

int liblatte_endorse_image_source(const char * id, const char * config, const char *url, const char *rev)
  __attribute__((weak, alias("__liblatte_endorse_image_source")));
int __liblatte_endorse_image_source(const char * id, const char * config, const char *url, const char *rev) {
  return 0;
}


int liblatte_check_property(const char *ip, uint32_t port, const char *property)
  __attribute__((weak, alias("__liblatte_check_property")));
int __liblatte_check_property(const char *ip, uint32_t port, const char *property) {
  return 0;
}

int liblatte_check_access(const char *ip, uint32_t port, const char *object)
  __attribute__((weak, alias("__liblatte_check_access")));
int __liblatte_check_access(const char *ip, uint32_t port, const char *object) {
  return 0;
}
int liblatte_check_worker_access(const char *ip, uint32_t port, const char *object)
  __attribute__((weak, alias("__liblatte_check_access")));
int __liblatte_check_worker_access(const char *ip, uint32_t port, const char *object) {
  return 0;
}

char* liblatte_check_attestation(const char *ip, uint32_t port, char **attestation,
    size_t *size) 
  __attribute__((weak, alias("__liblatte_check_access")));
char* __liblatte_check_attestation(const char *ip, uint32_t port, char **attestation,
    size_t *size) {
  return NULL;
}


int main() {
  return 0;
}



