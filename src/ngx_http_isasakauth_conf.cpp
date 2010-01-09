#include "ngx_http_isasakauth_conf.h"

void *ngx_http_isasakauth_create_srv_conf(ngx_conf_t *cf) {
  ngx_http_isasakauth_srv_conf_t  *conf;

  conf = (ngx_http_isasakauth_srv_conf_t*)ngx_pcalloc(cf->pool,
                                              sizeof(ngx_http_isasakauth_srv_conf_t));
  if (conf == NULL) {
    return NGX_CONF_ERROR;
  }

  return conf;
}

char *ngx_http_isasakauth_merge_srv_conf(ngx_conf_t *cf,
                                        void *parent,
                                        void *child) {
  ngx_http_isasakauth_srv_conf_t *prev = (ngx_http_isasakauth_srv_conf_t*)parent;
  ngx_http_isasakauth_srv_conf_t *conf = (ngx_http_isasakauth_srv_conf_t*)child;
  ngx_conf_merge_str_value(conf->authcookie_key,
                           prev->authcookie_key,
                           "authkey");
  return NGX_CONF_OK;
}

void *ngx_http_isasakauth_create_loc_conf(ngx_conf_t *cf) {
  ngx_http_isasakauth_loc_conf_t  *conf;

  conf = (ngx_http_isasakauth_loc_conf_t*)ngx_pcalloc(cf->pool,
         sizeof(ngx_http_isasakauth_loc_conf_t));

  if (conf == NULL) {
    return NGX_CONF_ERROR;
  }

  return conf;
}

char *ngx_http_isasakauth_merge_loc_conf(ngx_conf_t *cf,
                                        void *parent,
                                        void *child) {
  return NGX_CONF_OK;
}

char *ngx_http_isasakauth_authinfo(ngx_conf_t *cf,
                                  ngx_command_t *cmd,
                                  void *conf) {
  ngx_http_isasakauth_srv_conf_t* mcf = (ngx_http_isasakauth_srv_conf_t*)conf;
  //ngx_http_isasakauth_loc_conf_t* mcf = (ngx_http_isasakauth_loc_conf_t*)conf;
  ngx_str_t *value;
  /*
    value[0] is key
    value[1] is value
  */
  value = (ngx_str_t*)cf->args->elts;


  if (value[1].data !=  NULL) {
    mcf->auth_hostname = (char*)value[1].data;
  }

  if (value[2].data !=  NULL) {
    mcf->auth_port = (int*)calloc(1, sizeof(int));

    if (mcf->auth_port == NULL) {
      //allocation error
      return (char*)NGX_CONF_ERROR;
    }

    *(mcf->auth_port) = atoi((char*)value[2].data);
  }

  return NGX_CONF_OK;
}

char *ngx_http_isasakauth_sessioninfo(ngx_conf_t *cf,
                                     ngx_command_t *cmd,
                                     void *conf) {
  ngx_http_isasakauth_srv_conf_t* mcf = (ngx_http_isasakauth_srv_conf_t*)conf;
  ngx_str_t *value;
  /*
    value[0] is key
    value[1] is value
  */
  value = (ngx_str_t*)cf->args->elts;

  if (value[1].data !=  NULL) {
    mcf->session_hostname = (char*)value[1].data;
  }

  if (value[2].data !=  NULL) {
    mcf->session_port = (int*)calloc(1, sizeof(int));

    if (mcf->session_port == NULL) {
      //allocation error
      return (char*)NGX_CONF_ERROR;
    }

    *(mcf->session_port) = atoi((char*)value[2].data);
  }

  return NGX_CONF_OK;
}

char *ngx_http_isasakauth_login_conf(ngx_conf_t *cf,
                                    ngx_command_t *cmd,
                                    void *conf) {
  ngx_http_isasakauth_loc_conf_t* mcf = (ngx_http_isasakauth_loc_conf_t*)conf;
  if (mcf->login_enable == NULL) {
    mcf->login_enable = (bool*)calloc(1, sizeof(bool));
    
    if (mcf->login_enable == NULL) {
      //allocation error
      return (char*)NGX_CONF_ERROR;
    }

    *(mcf->login_enable) = true;
  }
  return NGX_CONF_OK;
}

char *ngx_http_isasakauth_logout_conf(ngx_conf_t *cf,
                                    ngx_command_t *cmd,
                                    void *conf) {
  ngx_http_isasakauth_loc_conf_t* mcf = (ngx_http_isasakauth_loc_conf_t*)conf;
  if (mcf->logout_enable == NULL) {
    mcf->logout_enable = (bool*)calloc(1, sizeof(bool));

    if (mcf->logout_enable == NULL) {
      //allocation error
      return (char*)NGX_CONF_ERROR;
    }

    *(mcf->logout_enable) = true;
  }

  return NGX_CONF_OK;
}

char *ngx_http_isasakauth_noauth_conf(ngx_conf_t *cf,
                                      ngx_command_t *cmd,
                                      void *conf) {
  ngx_http_isasakauth_loc_conf_t* mcf = (ngx_http_isasakauth_loc_conf_t*)conf;
  if (mcf->noauth == NULL) {
    mcf->noauth = (bool*)calloc(1, sizeof(bool));

    if (mcf->noauth == NULL) {
      //allocation error
      return (char*)NGX_CONF_ERROR;
    }

    *(mcf->noauth) = true;
  }

  return NGX_CONF_OK;
}

char *ngx_http_isasakauth_authcookie_key_conf(ngx_conf_t *cf,
                                         ngx_command_t *cmd,
                                         void *conf) {
  ngx_http_isasakauth_srv_conf_t* mcf = (ngx_http_isasakauth_srv_conf_t*)conf;
  ngx_str_t *value;
  /*
    value[0] is key
    value[1] is value
  */
  value = (ngx_str_t*)cf->args->elts;
  
  if (value[1].data !=  NULL) {
    mcf->authcookie_key.data = (u_char*)calloc(value[1].len+1, sizeof(char));
    
    if (mcf->authcookie_key.data == NULL) {
      //allocation error
      return (char*)NGX_CONF_ERROR;
    }
    ngx_cpystrn(mcf->authcookie_key.data, value[1].data, value[1].len+1);
    mcf->authcookie_key.len = value[1].len;
    
  }
  
  return NGX_CONF_OK;
}

char *ngx_http_isasakauth_login_form_path_conf(ngx_conf_t *cf,
                                         ngx_command_t *cmd,
                                         void *conf) {
  ngx_http_isasakauth_srv_conf_t* mcf = (ngx_http_isasakauth_srv_conf_t*)conf;
  ngx_str_t *value;
  /*
    value[0] is key
    value[1] is value
  */
  value = (ngx_str_t*)cf->args->elts;
  
  if (value[1].data !=  NULL) {
    mcf->login_form_path.data = (u_char*)calloc(value[1].len+1, sizeof(char));
    
    if (mcf->login_form_path.data == NULL) {
      //allocation error
      return (char*)NGX_CONF_ERROR;
    }
    ngx_cpystrn(mcf->login_form_path.data, value[1].data, value[1].len+1);
    mcf->login_form_path.len = value[1].len;
    
  }
  
  return NGX_CONF_OK;
}

char *ngx_http_isasakauth_login_top_path_conf(ngx_conf_t *cf,
                                         ngx_command_t *cmd,
                                         void *conf) {
  ngx_http_isasakauth_srv_conf_t* mcf = (ngx_http_isasakauth_srv_conf_t*)conf;
  ngx_str_t *value;
  /*
    value[0] is key
    value[1] is value
  */
  value = (ngx_str_t*)cf->args->elts;
  
  if (value[1].data !=  NULL) {
    mcf->login_top_path.data = (u_char*)calloc(value[1].len+1, sizeof(char));
    
    if (mcf->login_top_path.data == NULL) {
      //allocation error
      return (char*)NGX_CONF_ERROR;
    }
    ngx_cpystrn(mcf->login_top_path.data, value[1].data, value[1].len+1);
    mcf->login_top_path.len = value[1].len;
    
  }
  
  return NGX_CONF_OK;
}
