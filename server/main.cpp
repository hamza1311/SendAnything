#include "headers/server.hpp"

int main() {
    server::Server server(9002);
    server.start();

    return 0;
}
