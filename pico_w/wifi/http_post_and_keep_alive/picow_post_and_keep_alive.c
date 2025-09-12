/**
 * Copyright (c) 2023 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdio.h"
#include "pico/cyw43_arch.h"
#include "pico/async_context.h"
#include "lwip/altcp_tls.h"
#include "example_http_client_util.h"

#define HOST "HOST_ADDRESS" // This should be your computers ip address
#define GET_REQUEST "/get_button"
#define POST_REQUEST "/post_button"

static int keep_alive_result_fn(EXAMPLE_HTTP_REQUEST_T *req, httpc_result_t httpc_result, u32_t rx_content_len, u32_t srv_res, err_t err) {
    char *data = "Hello, I'm some new posted data using the same connection.";
    u16_t data_len = strlen(data) * sizeof(char);
    httpc_post_next(POST_REQUEST, data_len, data, &req->settings, req->http_state);

    // dont keep the connection open
    return 0;
}

int main() {
    stdio_init_all();

    if (cyw43_arch_init()) {
        printf("failed to initialise\n");
        return 1;
    }

    cyw43_arch_enable_sta_mode();
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("failed to connect\n");
        return 1;
    }

    // Do a post request, then keep alive
    EXAMPLE_HTTP_REQUEST_T req = {0};
    req.hostname = HOST;
    req.url = POST_REQUEST;
    req.headers_fn = http_client_header_print_fn;
    req.recv_fn = http_client_receive_print_fn;
    req.port = 5000;
    req.result_fn = keep_alive_result_fn;

    char *data = "Hello, I'm some posted data.";
    u16_t data_len = strlen(data) * sizeof(char);

    int post_result = http_client_post_request_sync(cyw43_arch_async_context(), &req, data_len, data);

    // wait a bit to allow second request to come through
    sleep_ms(1000);

    cyw43_arch_deinit();
    printf("done.\n");
    return 0;
}