extern "C" {
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_event.h>
#include <ngx_event_connect.h>
#include <ngx_event_pipe.h>

#include <ngx_http.h>

#include <ngx_mail.h>
#include <ngx_mail_pop3_module.h>
#include <ngx_mail_imap_module.h>
#include <ngx_mail_smtp_module.h>
#include "ddebug.h"

}

typedef struct {
  bool *login_enable;
  bool *logout_enable;
  bool *noauth;
  ngx_array_t  *session_enable;
} ngx_http_isasakauth_loc_conf_t;

typedef struct {
  ngx_str_t authcookie_key;
  ngx_str_t login_form_path;
  ngx_str_t login_top_path;
  char *auth_hostname;
  int *auth_port;
  char *session_hostname;
  int *session_port;
} ngx_http_isasakauth_srv_conf_t;

void *ngx_http_isasakauth_create_srv_conf(ngx_conf_t *cf);

char *ngx_http_isasakauth_merge_srv_conf(ngx_conf_t *cf,
    void *parent,
    void *child);

void *ngx_http_isasakauth_create_loc_conf(ngx_conf_t *cf);

char *ngx_http_isasakauth_merge_loc_conf(ngx_conf_t *cf,
    void *parent,
    void *child);

char *ngx_http_isasakauth_authinfo(ngx_conf_t *cf,
                                   ngx_command_t *cmd,
                                   void *conf);

char *ngx_http_isasakauth_sessioninfo(ngx_conf_t *cf,
                                      ngx_command_t *cmd,
                                      void *conf);

char *ngx_http_isasakauth_login_conf(ngx_conf_t *cf,
                                     ngx_command_t *cmd,
                                     void *conf);

char *ngx_http_isasakauth_logout_conf(ngx_conf_t *cf,
                                      ngx_command_t *cmd,
                                      void *conf);

char *ngx_http_isasakauth_noauth_conf(ngx_conf_t *cf,
                                      ngx_command_t *cmd,
                                      void *conf);

char *ngx_http_isasakauth_authcookie_key_conf(ngx_conf_t *cf,
    ngx_command_t *cmd,
    void *conf);

char *ngx_http_isasakauth_login_form_path_conf(ngx_conf_t *cf,
    ngx_command_t *cmd,
    void *conf);

char *ngx_http_isasakauth_login_top_path_conf(ngx_conf_t *cf,
    ngx_command_t *cmd,
    void *conf);

