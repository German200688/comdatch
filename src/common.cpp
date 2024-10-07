//#include <glib.h>
#include <librdkafka/rdkafka.h>
#include <iostream>
#include <cstdlib>

/* Wrapper to set config values and error out if needed.
 */
static void set_config(rd_kafka_conf_t* conf, char* key, char* value) {
    std::cout << "55541" << std::endl;
    char errstr[512];
    std::cout << "55542" << std::endl;
    rd_kafka_conf_res_t res;
    std::cout << "55543" << std::endl;

    res = rd_kafka_conf_set(conf, key, value, errstr, sizeof(errstr));
    std::cout << "55544" << std::endl;
    if (res != RD_KAFKA_CONF_OK) {
        std::cout << "Unable to set config: %s" << errstr << std::endl;
        exit(1);
    }
}
