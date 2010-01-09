#include "ngx_http_isasakauth_module.h"

using namespace std;

static char* get_authcookie_key(ngx_http_request_t *r) {
  ngx_http_isasakauth_srv_conf_t  *mscf;
  mscf = (ngx_http_isasakauth_srv_conf_t*)ngx_http_get_module_srv_conf(r, ngx_http_isasakauth_module);
    
  return (char*)mscf->authcookie_key.data;
}

static char* get_login_form_path(ngx_http_request_t *r) {
  ngx_http_isasakauth_srv_conf_t  *mscf;
  mscf = (ngx_http_isasakauth_srv_conf_t*)ngx_http_get_module_srv_conf(r, ngx_http_isasakauth_module);
    
  return (char*)mscf->login_form_path.data;
}

static char* get_login_top_path(ngx_http_request_t *r) {
  ngx_http_isasakauth_srv_conf_t  *mscf;
  mscf = (ngx_http_isasakauth_srv_conf_t*)ngx_http_get_module_srv_conf(r, ngx_http_isasakauth_module);
    
  return (char*)mscf->login_top_path.data;
}

static int get_auth_port(ngx_http_request_t *r) {
  ngx_http_isasakauth_srv_conf_t  *mlcf;
  mlcf = (ngx_http_isasakauth_srv_conf_t*)ngx_http_get_module_srv_conf(r, ngx_http_isasakauth_module);
  int ret = 1978;

  if (mlcf->auth_port != NULL) {
    ret = *(mlcf->auth_port);
  }      
  
  return ret;
}

static string get_auth_hostname(ngx_http_request_t *r) {
  ngx_http_isasakauth_srv_conf_t  *mlcf;
  mlcf = (ngx_http_isasakauth_srv_conf_t*)ngx_http_get_module_srv_conf(r, ngx_http_isasakauth_module);
  string ret = string("localhost");

  if (mlcf->auth_hostname != NULL) {
    ret = string(mlcf->auth_hostname);
  }

  return ret;
}

static int get_session_port(ngx_http_request_t *r) {
  ngx_http_isasakauth_srv_conf_t  *mlcf;
  mlcf = (ngx_http_isasakauth_srv_conf_t*)ngx_http_get_module_srv_conf(r, ngx_http_isasakauth_module);
  int ret = 11211;

  if (mlcf->session_port != NULL) {
    ret = *(mlcf->session_port);
  }      

  return ret;
}

static string get_session_hostname(ngx_http_request_t *r) {
  ngx_http_isasakauth_srv_conf_t  *mlcf;
  mlcf = (ngx_http_isasakauth_srv_conf_t*)ngx_http_get_module_srv_conf(r, ngx_http_isasakauth_module);
  string ret = string("localhost");

  if (mlcf->session_hostname != NULL) {
    ret = string(mlcf->session_hostname);
  }

  return ret;
}

ngx_int_t redirect_response(ngx_http_request_t *r, char *to){
  u_char *location;
  r->headers_out.status = NGX_HTTP_MOVED_TEMPORARILY;
  r->headers_out.location = (ngx_table_elt_t*)ngx_list_push(&r->headers_out.headers);
  if (r->headers_out.location == NULL) {
    return NGX_ERROR;
  }
  location = (u_char*)ngx_pnalloc(r->pool, strlen(to));
  if (location == NULL) {
    return NGX_ERROR;
  }

  r->headers_out.content_length_n = 0;
  ngx_memcpy(location, to, strlen(to));
  r->headers_out.location->hash = 1;
  r->headers_out.location->key.len = strlen("Location");
  r->headers_out.location->key.data = (u_char *) "Location";
  r->headers_out.location->value.len = strlen(to);
  r->headers_out.location->value.data = location;
  ngx_http_send_header(r);
  ngx_http_finalize_request(r, r->headers_out.status);
  
  return NGX_DONE;
}

void auth_response(ngx_http_request_t *r, bool result, string *username = NULL) {
  if (result) {
    r->headers_out.status = NGX_HTTP_OK;
    //auth header
    ngx_table_elt_t *set_cookie;
    set_cookie = (ngx_table_elt_t*)ngx_list_push(&r->headers_out.headers);

    if (set_cookie != NULL) {
      Session new_session(get_session_hostname(r), get_session_port(r), *username);
      string key = string("Set-Cookie");
      string val = string(get_authcookie_key(r)) +
        string("=") +
        new_session.session_id.to_s() +
        string(";") +
        string(" path=/");
      
      set_cookie->hash = 1;
      set_cookie->key.len = key.size();
      set_cookie->key.data = (u_char *) key.c_str();
      set_cookie->value.len = val.size();

      set_cookie->value.data = (u_char *)val.c_str();
      char* to = get_login_top_path(r);
      redirect_response(r, to);
    }
  } else {
    r->headers_out.status = NGX_HTTP_UNAUTHORIZED;
    char* to = get_login_form_path(r);
    redirect_response(r, to);
  }
}

static void ngx_http_isasakauth_logout(ngx_http_request_t *r) {
  ngx_str_t cookie_str;
  ngx_str_t key;
  key.data = (u_char*)get_authcookie_key(r);
  key.len = strlen((char*)key.data);
  ngx_int_t n = ngx_http_parse_multi_header_lines(
                                                  &r->headers_in.cookies,
                                                  &key,
                                                  &cookie_str);

  if (n != NGX_DECLINED) {
    string cookie_string = string((char*)cookie_str.data, cookie_str.len);
    try{
      SessionID id = SessionID(cookie_string);
      Session session = Session(get_session_hostname(r),
                                get_session_port(r),
                                id);
      session.expire();
    }catch(invalid_session e){
      DD("LOGOUT:\tinvalid session detected");
    }
  }
  
  return;
}

static bool ngx_http_isasakauth_is_authenticated(ngx_http_request_t *r) {
  bool ret = false;
  ngx_str_t cookie_str;
  ngx_str_t key;
  key.data = (u_char*)get_authcookie_key(r);
  key.len = strlen((char*)key.data);

  ngx_int_t n = ngx_http_parse_multi_header_lines(
                  &r->headers_in.cookies,
                  &key,
                  &cookie_str);

  if (n != NGX_DECLINED) {
    string cookie_string = string((char*)cookie_str.data, cookie_str.len);
    try{

      SessionID id = SessionID(cookie_string);
      Session session = Session(get_session_hostname(r),
                                get_session_port(r),
                                id);
      ret = true;
    }catch(invalid_session e){
      DD("CHECK AUTH:\tinvalid session detected");
    }
    
  }
  

  return ret;
}

static void ngx_http_isasakauth_login(ngx_http_request_t *r) {
  ngx_http_isasakauth_loc_conf_t  *mlcf;
  mlcf = (ngx_http_isasakauth_loc_conf_t*)ngx_http_get_module_loc_conf(r, ngx_http_isasakauth_module);

  size_t len = r->headers_in.content_length_n;
  string postdata;

  if (len == 0) {
    postdata = string("");
  } else {
    postdata = string((char*)r->request_body->buf->start, len);
  }

  string* input_user = get_param(postdata, "username");
  string* input_pass = get_param(postdata, "password");

  Authenticator auth = Authenticator(get_auth_hostname(r), get_auth_port(r));
  bool authed  = auth.is_valid_account(*input_user, *input_pass);

  if (authed) {
    auth_response(r, true, input_user);
  } else {
    auth_response(r, false);
  }


  if (input_user != NULL) {
    delete(input_user);
    input_user = NULL;
  }

  if (input_pass != NULL) {
    delete(input_pass);
    input_pass = NULL;
  }
}


static ngx_int_t ngx_http_isasakauth_handler(ngx_http_request_t *r) {
  string *uri  = new string((char*)(r->uri).data, (r->uri).len);
  delete uri;

  ngx_http_isasakauth_loc_conf_t  *mlcf;
  mlcf = (ngx_http_isasakauth_loc_conf_t*)ngx_http_get_module_loc_conf(r, ngx_http_isasakauth_module);

  if (mlcf->noauth){
    return NGX_OK;
  }

  if (mlcf->logout_enable){
    // logout handle
    ngx_http_isasakauth_logout(r);
    char *to = get_login_form_path(r);
    return redirect_response(r,to);
  }
  
  if (mlcf->login_enable) {
    // login handle
    if (r->method  == NGX_HTTP_POST) {
      ngx_int_t rc;
      rc = ngx_http_read_client_request_body(r, ngx_http_isasakauth_login);


      if (rc >= NGX_HTTP_SPECIAL_RESPONSE) {
        return rc;
      }

      return NGX_DONE;
    }else{
      auth_response(r, false);
      return NGX_DONE;
    }
  }
  bool authed= ngx_http_isasakauth_is_authenticated(r);
  
  if (authed) {
    return NGX_OK;
  }

  char* to = get_login_form_path(r);
  return redirect_response(r, to);
  
}

static ngx_int_t ngx_http_isasakauth_init(ngx_conf_t *cf) {
  // global handler initialization
  ngx_http_handler_pt        *h;
  ngx_http_core_main_conf_t  *cmcf;
  cmcf = (ngx_http_core_main_conf_t*)
         ngx_http_conf_get_module_main_conf(cf,
                                            ngx_http_core_module);
  h = (ngx_http_handler_pt*)
      ngx_array_push(&cmcf->phases[NGX_HTTP_ACCESS_PHASE].handlers);

  if (h == NULL) {
    return NGX_ERROR;
  }

  *h = ngx_http_isasakauth_handler;
  
  return NGX_OK;
}

