#include "httpd.h"
#include "http_config.h"
#include "http_core.h"
#include "http_log.h"
#include "http_protocol.h"
#include "http_vhost.h"
#include "apr_strings.h"
#include <arpa/inet.h>

module AP_MODULE_DECLARE_DATA anonip_module;

typedef struct {
    int mask;
} anonip_server_cfg;

static void *anonip_create_server_cfg(apr_pool_t *p, server_rec *s) {
    anonip_server_cfg *cfg = (anonip_server_cfg *)apr_pcalloc(p, sizeof(anonip_server_cfg));
    if (!cfg)
        return NULL;

    cfg->mask = 0;

    return (void *)cfg;
}

static const char *anonip_mask(cmd_parms *cmd, void *dummy, const char* mask) {
    server_rec *s = cmd->server;
    anonip_server_cfg *cfg = (anonip_server_cfg *)ap_get_module_config(s->module_config, 
                                                                   &anonip_module);

    cfg->mask = atoi(mask);
    return NULL;
}

static int change_remote_ip(request_rec *r) {
	int i;
    struct in_addr ip;
    anonip_server_cfg *cfg = (anonip_server_cfg *)ap_get_module_config(r->server->module_config,
                                                                   &anonip_module);
    if (cfg->mask<=0 || cfg->mask>4)
        return DECLINED;

    inet_aton(r->connection->client_ip, &ip);
	for (i=0; i<cfg->mask; i++)
		((char*)&ip)[3-i] = 0;

#if AP_SERVER_MAJORVERSION_NUMBER > 2 || \
(AP_SERVER_MAJORVERSION_NUMBER == 2 && AP_SERVER_MINORVERSION_NUMBER >= 4)
    r->connection->client_ip = apr_pstrdup(r->connection->pool, inet_ntoa(ip));
    r->connection->client_addr->sa.sin.sin_addr = ip;
    r->useragent_ip = apr_pstrdup(r->connection->pool, inet_ntoa(ip));
    r->useragent_addr->sa.sin.sin_addr = ip;
#else
    r->connection->remote_ip = apr_pstrdup(r->connection->pool, inet_ntoa(ip));
    r->connection->remote_addr->sa.sin.sin_addr = ip;
#endif

    return DECLINED;
}

static const command_rec anonip_cmds[] = {
    AP_INIT_TAKE1(
                 "AnonipMask",
                 anonip_mask,
                 NULL,
                 RSRC_CONF,
                 "Set the bytemask for mod_anonip (e.g. 0=192.168.1.1, 3=192.0.0.0)"
                 ),
    { NULL }
};

static void register_hooks(apr_pool_t *p) {
    ap_hook_post_read_request(change_remote_ip, NULL, NULL, APR_HOOK_MIDDLE);
}

module AP_MODULE_DECLARE_DATA anonip_module = {
    STANDARD20_MODULE_STUFF,
    NULL,
    NULL,
    anonip_create_server_cfg,
    NULL,
    anonip_cmds,
    register_hooks,
};
