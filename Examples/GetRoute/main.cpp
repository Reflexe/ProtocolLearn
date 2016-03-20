#include <iostream>

#include "Route.h"
#include "Ipv4Address.h"

int usage(const char *programName);

int usage(const char *programName) {
    std::cout << "Usage: " << programName << " <Ipv4 Address>" << std::endl;
    return -1;
}

int main(int argc, char *argv[]) {
    if(argc != 2)
        return usage(argv[0]);

    using std::cout;
    using std::endl;

    using namespace ProtocolLearn;

    using Libraries::Route;

    const auto &getRouteAnswer = Route{}.getRoute(Ipv4Address(argv[1]));

    switch (getRouteAnswer.routeType) {
    case Route::RouteType::Gateway:
        cout << "Gateway: " << getRouteAnswer.gateway.toString();
        break;
    case Route::RouteType::Loopback:
        cout << "Loopback";
        break;
    case Route::RouteType::LocalLink:
        cout << "A local address";
        break;
    }

    cout << "; ";

    cout << "Interface: " << getRouteAnswer.interface.getName() << endl;
}
