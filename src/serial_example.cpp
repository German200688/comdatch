/***
 * This example expects the serial port has a loopback on it.
 *
 * Alternatively, you could use an Arduino:
 *
 * <pre>
 *  void setup() {
 *    Serial.begin(<insert your baudrate here>);
 *  }
 *
 *  void loop() {
 *    if (Serial.available()) {
 *      Serial.write(Serial.read());
 *    }
 *  }
 * </pre>
 */

#include <string>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <librdkafka/rdkafka.h>
#include <csignal>
#include <cstring>
#include "common.cpp"

#define ARR_SIZE(arr) ( sizeof((arr)) / sizeof((arr[0])) )


 // OS Specific sleep
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "serial/serial.h"
static volatile std::sig_atomic_t run = 1;

using std::string;
using std::exception;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;

/* Optional per-message delivery callback (triggered by poll() or flush())
 * when a message has been successfully delivered or permanently
 * failed delivery (after retries).
 */
static void dr_msg_cb(rd_kafka_t* kafka_handle,
    const rd_kafka_message_t* rkmessage,
    void* opaque) {
    if (rkmessage->err) {
        std::cout << "Message delivery failed: %s" << rd_kafka_err2str(rkmessage->err) << std::endl;
    }
}

void my_sleep(unsigned long milliseconds) {
#ifdef _WIN32
    Sleep(milliseconds); // 100 ms
#else
    usleep(milliseconds * 1000); // 100 ms
#endif
}

void enumerate_ports()
{
    vector<serial::PortInfo> devices_found = serial::list_ports();

    vector<serial::PortInfo>::iterator iter = devices_found.begin();

    while (iter != devices_found.end())
    {
        serial::PortInfo device = *iter++;

        printf("(%s, %s, %s)\n", device.port.c_str(), device.description.c_str(),
            device.hardware_id.c_str());
    }
}

void print_usage()
{
   cerr << "Usage: test_serial {-e|<serial port address>} ";
   cerr << "<baudrate> [test string]" << endl;
}

int run16(int argc, char** argv)
{
    const char* port_my = "COM6";
    const char* baud_my = "57600";

    rd_kafka_t* producer;
    rd_kafka_conf_t* conf;
    char errstr[512];

    // Create client configuration
    conf = rd_kafka_conf_new();

    // User-specific properties that you must set
    set_config(conf, "bootstrap.servers", "127.0.0.1:19092");

    // Fixed properties
    set_config(conf, "acks", "all");

    // Install a delivery-error callback.
    rd_kafka_conf_set_dr_msg_cb(conf, dr_msg_cb);

    // Create the Producer instance.
    producer = rd_kafka_new(RD_KAFKA_PRODUCER, conf, errstr, sizeof(errstr));
    if (!producer) {
        std::cout << "Failed to create new producer:" << errstr << std::endl;

    }

    // Configuration object is now owned, and freed, by the rd_kafka_t instance.
    conf = NULL;

    string value_my;
    //  if (argc < 2) {
   //       print_usage();
    //      return 74;
   //   }

      // Argument 1 is the serial port or enumerate flag
    string port(port_my);
    /*
        if (port == "-e") {
            enumerate_ports();
            return 82;
        }
        else if (argc < 3) {
            print_usage();
            return 86;
        }
    */
    // Argument 2 is the baudrate
    unsigned long baud = 0;
#if defined(WIN32) && !defined(__MINGW32__)
    sscanf_s(baud_my, "%lu", &baud);
#else
    sscanf(argv[2], "%lu", &baud);
#endif

    // port, baudrate, timeout in milliseconds
    serial::Serial my_serial(port, baud, serial::Timeout::simpleTimeout(10));

    cout << "Is the serial port open?";
    if (my_serial.isOpen())
        cout << " Yes." << endl;
    else
        cout << " No." << endl;
    /*
        // Get the Test string
        int count = 0;
        string test_string;
        if (argc == 4) {
            test_string = argv[3];
        }
        else {
            test_string = "Testing.";
        }
    */

    string test_string;
    int count = 0;
    // Test the timeout, there should be 1 second between prints
  //  cout << "Timeout == 1000ms, asking for 1 more byte than written." << endl;

    /*
    while (count < 50) {
  //      size_t bytes_wrote = my_serial.write(test_string);

        string result = my_serial.read(test_string.length() + 1);

        cout << "Iteration: " << count << ", Bytes read:  ";
    //    cout << bytes_wrote << ", Bytes read: ";
        cout << result.length() << ", String read: " << result << endl;

        count += 1;
    }
*/
// Test the timeout at 250ms
//   my_serial.setTimeout(serial::Timeout::max(), 250, 0, 250, 0);
  // count = 0;
 //  cout << "Timeout == 250ms, asking for 1 more byte than written." << endl;
//   while (count < 10) {
//       size_t bytes_wrote = my_serial.write(test_string);
   /*
   int count = 0;
   string test_string;
       string result = my_serial.read(test_string.length() + 1);

       cout << "Iteration: " << count << ", String read:  ";
  //     cout << bytes_wrote << ", Bytes read: ";
       cout << result.length()  << endl;

       count += 1;
  // }
  */
  /*
      // Test the timeout at 250ms, but asking exactly for what was written
      count = 0;
      cout << "Timeout == 250ms, asking for exactly what was written." << endl;
      while (count < 10) {
          size_t bytes_wrote = my_serial.write(test_string);

          string result = my_serial.read(test_string.length());

          cout << "Iteration: " << count << ", Bytes written: ";
          cout << bytes_wrote << ", Bytes read: ";
          cout << result.length() << ", String read: " << result << endl;

          count += 1;
      }

      // Test the timeout at 250ms, but asking for 1 less than what was written
      count = 0;
      cout << "Timeout == 250ms, asking for 1 less than was written." << endl;
      while (count < 10) {
          size_t bytes_wrote = my_serial.write(test_string);

          string result = my_serial.read(test_string.length() - 1);

          cout << "Iteration: " << count << ", Bytes written: ";
          cout << bytes_wrote << ", Bytes read: ";
          cout << result.length() << ", String read: " << result << endl;

          count += 1;
      }
  */
    string   incomingByteIN80 = "0";
    string   incomingByteIN81 = "0";
    string   incomingByteIN82 = "0";
    string   incomingByteIN83 = "0";
    string   incomingByteIN84 = "0";
    string   incomingByteIN85 = "0";
    string   incomingByteIN86 = "0";
    string   incomingByteIN87 = "0";
    string   incomingByteIN88 = "0";
    string   incomingByteIN89 = "0";
    string   incomingByteIN90 = "0";

    int testIN8 = 0;
    while (run) {
        if (testIN8 == 0 && my_serial.available() > 0) //testIN8 == 0 && 
        {  //если есть доступные данные
    //Serial.println(49);
            incomingByteIN80 = my_serial.read(); // считываем байт
            // cout << "poshlo " << incomingByteIN80 << endl;


            while (testIN8 == 0 && incomingByteIN80 != "P") { //&& incomingByteIN80 > 10
                incomingByteIN80 = my_serial.read(); // считываем байт
                //Serial.println(49);
      //          incomingByteIN80 = my_serial.read(); // считываем байт
                //Serial.println("H");

              //  incomingByteIN90 = incomingByteIN89;
                incomingByteIN89 = incomingByteIN88;
                incomingByteIN88 = incomingByteIN87;
                incomingByteIN87 = incomingByteIN86;
                incomingByteIN86 = incomingByteIN85;
                incomingByteIN85 = incomingByteIN84;
                incomingByteIN84 = incomingByteIN83;
                incomingByteIN83 = incomingByteIN82;
                incomingByteIN82 = incomingByteIN81;
                incomingByteIN81 = incomingByteIN80;
              /*
                cout << "incomingByteIN90 " << incomingByteIN90 << endl;
                cout << "incomingByteIN89 " << incomingByteIN89 << endl;
                cout << "incomingByteIN88 " << incomingByteIN88 << endl;
                cout << "incomingByteIN87 " << incomingByteIN87 << endl;
                cout << "incomingByteIN86 " << incomingByteIN86 << endl;
                cout << "incomingByteIN85 " << incomingByteIN85 << endl;
                cout << "incomingByteIN84 " << incomingByteIN84 << endl;
                cout << "incomingByteIN83 " << incomingByteIN83 << endl;
                cout << "incomingByteIN82 " << incomingByteIN82 << endl;
                cout << "incomingByteIN81 " << incomingByteIN81 << endl;
                cout << "incomingByteIN80 " << incomingByteIN80 << endl;
               */ 

            }
            if (testIN8 == 0 && incomingByteIN80 == "P") {
                testIN8 = 1;
                value_my = incomingByteIN89 + incomingByteIN88 + incomingByteIN87 + incomingByteIN86 + incomingByteIN85 + incomingByteIN84 + incomingByteIN83 + incomingByteIN82;// +incomingByteIN81;

                incomingByteIN90 = "0";
                incomingByteIN89 = "0";
                incomingByteIN88 = "0";
                incomingByteIN87 = "0";
                incomingByteIN86 = "0";
                incomingByteIN85 = "0";
                incomingByteIN84 = "0";
                incomingByteIN83 = "0";
                incomingByteIN82 = "0";
                incomingByteIN81 = "0";
                incomingByteIN80 = "0";
                cout << value_my << endl;
            }
        }

            if (testIN8 != 0) {

                
                // Produce data by selecting random values from these lists.
                const char* v_m = value_my.c_str();
                cout << v_m << endl;
                int message_count = 1;
                const char* topic = "prodmozgromb2";
                const char* user_ids[1] = { "1" };
                const char* products[1] = { v_m };
               // cout << value_my.c_str() << endl;
                for (int i = 0; i < message_count; i++) {
                    const char* key = user_ids[rand() % ARR_SIZE(user_ids)];
                    const char* value = products[rand() % ARR_SIZE(products)];
                    size_t key_len = strlen(key);
                    size_t value_len = strlen(value);

                    rd_kafka_resp_err_t err;

                    err = rd_kafka_producev(producer,
                        RD_KAFKA_V_TOPIC(topic),
                        RD_KAFKA_V_MSGFLAGS(RD_KAFKA_MSG_F_COPY),
                        RD_KAFKA_V_KEY((void*)key, key_len),
                        RD_KAFKA_V_VALUE((void*)value, value_len),
                        RD_KAFKA_V_OPAQUE(NULL),
                        RD_KAFKA_V_END);

                    if (err) {
                        std::cout << "Failed to produce to topic %s: %s" << topic << rd_kafka_err2str(err) << std::endl;

                    }
                    else {
                        std::cout << "Produced event to topic %s: key = %12s value = %12s" << topic << key << value << std::endl;
                    }

                    rd_kafka_poll(producer, 0);
                }

                // Block until the messages are all sent.
                std::cout << "Flushing final messages.." << std::endl;
                rd_kafka_flush(producer, 10 * 1000);

                if (rd_kafka_outq_len(producer) > 0) {
                    std::cout << "%d message(s) were not delivered" << rd_kafka_outq_len(producer) << std::endl;
                }

                std::cout << "%d events were produced to topic %s." << message_count << topic << std::endl;

       //         rd_kafka_destroy(producer);

                testIN8 = 0;
            }


        }
        
    }

        
    //        if (incomingByteIN80 > 10) {
 //   return 176;


int main(int argc, char** argv) {
 

    try {
         run16(argc, argv);
    }
    catch (exception& e) {
     //   cerr << "Unhandled Exception: " << e.what() << endl;
    }
}