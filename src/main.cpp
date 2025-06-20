#include "../include/ProxyServer/ProxyServer.hpp"
#include "../include/Logger/Logger.hpp"


int main(){
    ProxyServer proxyServer;
    proxyServer.start(8080);
    return 0;
}