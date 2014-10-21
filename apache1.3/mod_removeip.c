

#include "httpd.h"
#include "http_config.h"
#include "http_core.h"
#include "http_log.h"
#include "http_protocol.h"
#include "http_vhost.h"

module MODULE_VAR_EXPORT removeip_module;

typedef struct {
    int enable;
} removeip_server_cfg;

static void *removeip_create_server_cfg(pool *p, server_rec *s) {
    removeip_server_cfg *cfg = (removeip_server_cfg *)ap_pcalloc(p, sizeof(removeip_server_cfg));
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

    r->connection->remote_ip = ap_pstrdup(r->connection->pool, "localhost");
    r->connection->remote_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    return DECLINED;
}

static command_rec removeip_cmds[] = {
    { "REMOVEIPenable", removeip_enable, NULL,
      RSRC_CONF, FLAG, "Enable mod_removeip" },
    { NULL }
};

module MODULE_VAR_EXPORT removeip_module = {
    STANDARD_MODULE_STUFF,
    NULL,                              /* initializer */
    NULL,                              /* dir config creator */
    NULL,                              /* dir config merger */
    removeip_create_server_cfg,            /* server config */
    NULL,                              /* merge server config */
    removeip_cmds,                         /* command table */
    NULL,                              /* handlers */
    NULL,                              /* filename translation */
    NULL,                              /* check_user_id */
    NULL,                              /* check auth */
    NULL,                              /* check access */
    NULL,                              /* type_checker */
    NULL,                              /* fixups */
    NULL,                              /* logger */
    NULL,                              /* header parser */
    NULL,                              /* child_init */
    NULL,                              /* child_exit */
    change_remote_ip                   /* post read-request */
};

