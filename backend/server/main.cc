#include <drogon/drogon.h>
#include <drogon/orm/DbClient.h>

int main() {

    drogon::app().loadConfigFile("../config.json");

    drogon::app().run();
    return 0;
}
