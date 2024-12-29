// remember to setup SDK for pico
#include "lwip/apps/httpd.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwipopts.h"
#include "lwip/ip4_addr.h"
#include "lwip/netif.h"
#include "ssi.h"
#include "cgi.h"
#include "networkfiles.h"

// WIFI Credentials - take care if pushing to github! add in networkfiles.h

const char WIFI_SSID[] = WIFINAME;
const char WIFI_PASSWORD[] = WIFIPASSWORD;

int main() {
    stdio_init_all();

    if (cyw43_arch_init())
    {
        printf("Failed to initialize WiFi module\n");
        return -1;
    }

    cyw43_arch_enable_sta_mode();

    // Connect to the WiFi network - loop until connected
    while(cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000) != 0){
        printf("Attempting to connect...\n");
        sleep_ms(5000); // Wait for 5 seconds before retrying
    }
    // Print a success message once connected
    printf("Connected! \n");

    // Get and print the IP address
    struct netif *netif = netif_list;
    if (netif_is_up(netif)) {
        printf("IP address: %s\n", ip4addr_ntoa(netif_ip4_addr(netif)));
    } else {
        printf("Failed to get IP address\n");
    }

    // Initialise web server
    httpd_init();
    printf("Http server initialised\n");

    // Configure SSI and CGI handler
    ssi_init(); 
    printf("SSI Handler initialised\n");
    cgi_init();
    printf("CGI Handler initialised\n");
    
    // Infinite loop
    while(1);
}