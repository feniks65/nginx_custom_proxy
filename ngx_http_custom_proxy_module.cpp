// ngx_http_custom_proxy_module.cpp
extern "C" {
    #include <ngx_config.h>
    #include <ngx_core.h>
    #include <ngx_http.h>
}

// define a custom configuration structure
typedef struct {
    ngx_str_t upstream_host;
} ngx_http_custom_proxy_conf_t;

// directive handler to set the upstream host
static char *
ngx_http_custom_proxy_set_host(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
    ngx_http_custom_proxy_conf_t *mycf = (ngx_http_custom_proxy_conf_t *) conf;
    ngx_str_t *value = (ngx_str_t *) cf->args->elts;

    if (mycf->upstream_host.data) {
        return (char *) "is duplicate";
    }

    mycf->upstream_host = value[1];
    return NGX_CONF_OK;
}

// define the commands for this module
static ngx_command_t ngx_http_custom_proxy_commands[] = {
    { ngx_string("custom_proxy_pass"),
      NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
      ngx_http_custom_proxy_set_host,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_custom_proxy_conf_t, upstream_host),
      nullptr },
    ngx_null_command
};

// request handler function
static ngx_int_t ngx_http_custom_proxy_handler(ngx_http_request_t *r) {
    ngx_http_custom_proxy_conf_t *mycf;
    mycf = (ngx_http_custom_proxy_conf_t *) ngx_http_get_module_loc_conf(r, ngx_http_custom_proxy_module);

    if (mycf->upstream_host.len == 0) {
        return NGX_DECLINED;
    }

    // modify headers before sending the request to the upstream
    ngx_table_elt_t *h = ngx_list_push(&r->headers_in.headers);
    if (h == nullptr) {
        return NGX_ERROR;
    }

    h->key = ngx_string("X-Header");
    h->value = ngx_string("0xFFFFFFFF");
    h->hash = 1;

    ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "Custom proxy handler called, forwarding to %V", &mycf->upstream_host);

    // setup the proxy pass
    ngx_http_upstream_t *u = (ngx_http_upstream_t *) ngx_pcalloc(r->pool, sizeof(ngx_http_upstream_t));
    if (u == nullptr) {
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }

    r->upstream = u;

    ngx_str_t host = mycf->upstream_host;

    u->conf = &ngx_http_upstream_module;

    // set up the proxy pass URL
    u->resolved = (ngx_http_upstream_resolved_t *) ngx_pcalloc(r->pool, sizeof(ngx_http_upstream_resolved_t));
    if (u->resolved == nullptr) {
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }

    u->resolved->host = host;
    u->resolved->port = 80;
    u->resolved->no_port = 0;
    u->resolved->naddrs = 1;

    ngx_http_set_ctx(r, u, ngx_http_custom_proxy_module);

    ngx_http_core_run_phases(r);

    return NGX_DONE;
}

// define the module's context and hooks
static ngx_http_module_t ngx_http_custom_proxy_module_ctx = {
    nullptr,                          // preconfiguration
    nullptr,                          // postconfiguration
    nullptr,                          // create main configuration
    nullptr,                          // init main configuration
    nullptr,                          // create server configuration
    nullptr,                          // merge server configuration
    nullptr,                          // create location configuration
    nullptr                           // merge location configuration
};

// define the module
ngx_module_t ngx_http_custom_proxy_module = {
    NGX_MODULE_V1,
    &ngx_http_custom_proxy_module_ctx, // module context
    ngx_http_custom_proxy_commands,    // module directives
    NGX_HTTP_MODULE,                   // module type
    nullptr,                           // init master
    nullptr,                           // init module
    nullptr,                           // init process
    nullptr,                           // init thread
    nullptr,                           // exit thread
    nullptr,                           // exit process
    nullptr,                           // exit master
    NGX_MODULE_V1_PADDING
};

