#include "httpd.h"
#include "http_config.h"
#include "http_core.h"
#include "http_log.h"
#include "http_protocol.h"
#include "http_vhost.h"
#include "apr_strings.h"

module AP_MODULE_DECLARE_DATA removeip_module;

typedef struct {
    int enable;
} removeip_server_cfg;

static void *removeip_create_server_cfg(apr_pool_t *p, server_rec *s) {
    removeip_server_cfg *cfg = (removeip_server_cfg *)apr_pcalloc(p, sizeof(removeip_server_cfg));
    if (!cfg)
        return NULL;

    cfg->enable = 0;

    return (void *)cfg;
}

static const char *removeip_enable(cmd_parms *cmd, void *dummy, int flag) {
    server_rec *s = cmd->server;
    removeip_server_cfg *cfg = (removeip_server_cfg *)ap_get_module_config(s->module_config, 
                                                                   &removeip_module);

    cfg->enable = flag;
    return NULL;
}

static int change_remote_ip(request_rec *r) {
    const char *fwdvalue;
    char *val;
    removeip_server_cfg *cfg = (removeip_server_cfg *)ap_get_module_config(r->server->module_config,
                                                                   &removeip_module);

    if (!cfg->enable)
        return DECLINED;

    r->connection->remote_ip = apr_pstrdup(r->connection->pool, "localhost");
    r->connection->remote_addr->sa.sin.sin_addr.s_addr = inet_addr("127.0.0.1");

    return DECLINED;
}

static const command_rec removeip_cmds[] = {
    AP_INIT_FLAG(
                 "REMOVEIPenable",
                 removeip_enable,
                 NULL,
                 RSRC_CONF,
                 "Enable mod_removeip"
                 ),
    { NULL }
};

static void register_hooks(apr_pool_t *p) {
    ap_hook_post_read_request(change_remote_ip, NULL, NULL, APR_HOOK_MIDDLE);
}

module AP_MODULE_DECLARE_DATA removeip_module = {
    STANDARD20_MODULE_STUFF,
    NULL,
    NULL,
    removeip_create_server_cfg,
    NULL,
    removeip_cmds,
    register_hooks,
};
