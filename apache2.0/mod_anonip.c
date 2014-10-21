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
    int enable;
} anonip_server_cfg;

static void *anonip_create_server_cfg(apr_pool_t *p, server_rec *s) {
    anonip_server_cfg *cfg = (anonip_server_cfg *)apr_pcalloc(p, sizeof(anonip_server_cfg));
    if (!cfg)
        return NULL;

    cfg->enable = 0;

    return (void *)cfg;
}

static const char *anonip_enable(cmd_parms *cmd, void *dummy, int flag) {
    server_rec *s = cmd->server;
    anonip_server_cfg *cfg = (anonip_server_cfg *)ap_get_module_config(s->module_config, 
                                                                   &anonip_module);

    cfg->enable = flag;
    return NULL;
}

static int change_remote_ip(request_rec *r) {
    const char *fwdvalue;
    char *val;
    struct in_addr ip;
    anonip_server_cfg *cfg = (anonip_server_cfg *)ap_get_module_config(r->server->module_config,
                                                                   &anonip_module);
    if (!cfg->enable)
        return DECLINED;

    r->connection->client_ip = apr_pstrdup(r->connection->pool, "localhost");
    r->connection->client_addr->sa.sin.sin_addr.s_addr = inet_addr("127.0.0.1");

    inet_aton(r->connection->client_ip, &ip);
    //((char*)&ip)[0] = 0;
    //((char*)&ip)[1] = 0;
    ((char*)&ip)[2] = 0;
    ((char*)&ip)[3] = 0;
    r->connection->client_ip = apr_pstrdup(r->connection->pool, inet_ntoa(ip));

    return DECLINED;
}

static const command_rec anonip_cmds[] = {
    AP_INIT_FLAG(
                 "AnonipEnable",
                 anonip_enable,
                 NULL,
                 RSRC_CONF,
                 "Enable mod_anonip"
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
