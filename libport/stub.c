
#include "libport.h"

#include <stdio.h>

#define REPORT(name) printf("%s is %s\n", #name, name)
#define REPORTI(name) printf("%s is %d\n", #name, name)
#define REPORTLU(name) printf("%s is %lu\n", #name, name)


int libport_init(const char *server_url, const char *persistence_path, int run_as_iaas)
  __attribute__((weak, alias("__libport_init")));

int __libport_init(const char *server_url, const char *persistence_path, int run_as_iaas) {
  REPORT(server_url);
  REPORT(persistence_path);
  REPORTI(run_as_iaas);
  return 0;
}

int libport_reinit(const char *server_url, const char *persistence_path, int run_as_iaas)
  __attribute__((weak, alias("__libport_reinit")));
int __libport_reinit(const char *server_url, const char *persistence_path, int run_as_iaas) {
  REPORT(server_url);
  REPORT(persistence_path);
  REPORTI(run_as_iaas);
  return 0;
}

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
void __libport_set_log_level(int upto) {
}
