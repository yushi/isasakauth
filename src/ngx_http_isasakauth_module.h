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
#define DDEBUG 0
#include "ddebug.h"
  
}
#include <string>
#include <iostream>
#include "ngx_http_isasakauth_conf.h"
#include "auth.h"

static ngx_int_t ngx_http_isasakauth_handler(ngx_http_request_t *r);

static ngx_int_t ngx_http_isasakauth_init(ngx_conf_t *cf);

static ngx_http_module_t  ngx_http_isasakauth_module_ctx = {
  NULL,                          /* preconfiguration */
  ngx_http_isasakauth_init,              /* postconfiguration */
  
  NULL,                          /* create main configuration */
  NULL,                          /* init main configuration */
  
  ngx_http_isasakauth_create_srv_conf,   /* create server configuration */
  ngx_http_isasakauth_merge_srv_conf,    /* merge server configuration */
  
  ngx_http_isasakauth_create_loc_conf,  /* create location configuration */
  ngx_http_isasakauth_merge_loc_conf    /* merge location configuration */
};

ngx_command_t  ngx_http_isasakauth_commands[] = {
  {
    ngx_string("authinfo"),
    NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LMT_CONF|NGX_CONF_TAKE2,
    ngx_http_isasakauth_authinfo,
    NGX_HTTP_SRV_CONF_OFFSET,
    0,
    NULL },
  {
    ngx_string("sessioninfo"),
    NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LMT_CONF|NGX_CONF_TAKE2,
    ngx_http_isasakauth_sessioninfo,
    NGX_HTTP_SRV_CONF_OFFSET,
    0,
    NULL },
  {
    ngx_string("login"),
    NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_HTTP_LMT_CONF|NGX_CONF_NOARGS,
    ngx_http_isasakauth_login_conf,
    NGX_HTTP_LOC_CONF_OFFSET,
    0,
    NULL },
  {
    ngx_string("logout"),
    NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_HTTP_LMT_CONF|NGX_CONF_NOARGS,
    ngx_http_isasakauth_logout_conf,
    NGX_HTTP_LOC_CONF_OFFSET,
    0,
    NULL },
  {
    ngx_string("noauth"),
    NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_HTTP_LMT_CONF|NGX_CONF_NOARGS,
    ngx_http_isasakauth_noauth_conf,
    NGX_HTTP_LOC_CONF_OFFSET,
    0,
    NULL },
  {
    ngx_string("authcookie"),
    NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LMT_CONF|NGX_CONF_TAKE1,
    ngx_http_isasakauth_authcookie_key_conf,
    NGX_HTTP_SRV_CONF_OFFSET,
    0,
    NULL },
  {
    ngx_string("login_form_path"),
    NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LMT_CONF|NGX_CONF_TAKE1,
    ngx_http_isasakauth_login_form_path_conf,
    NGX_HTTP_SRV_CONF_OFFSET,
    0,
    NULL },
  {
    ngx_string("login_top_path"),
    NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LMT_CONF|NGX_CONF_TAKE1,
    ngx_http_isasakauth_login_top_path_conf,
    NGX_HTTP_SRV_CONF_OFFSET,
    0,
    NULL },
  ngx_null_command
};

ngx_module_t  ngx_http_isasakauth_module = {
  NGX_MODULE_V1,
  &ngx_http_isasakauth_module_ctx, /* module context */
  ngx_http_isasakauth_commands,   /* module directives */
  NGX_HTTP_MODULE,               /* module type */
  NULL,                          /* init master */
  NULL,                          /* init module */
  NULL,                          /* init process */
  NULL,                          /* init thread */
  NULL,                          /* exit thread */
  NULL,                          /* exit process */
  NULL,                          /* exit master */
  NGX_MODULE_V1_PADDING
};

