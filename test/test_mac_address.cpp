#include <Arduino.h>
#include <unity.h>
// https://docs.platformio.org/en/latest/plus/unit-testing.html
// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

// ls -l /usr/local/bin/python*
// ln -s -f /usr/local/bin/python3.7 /usr/local/bin/python
// https://docs.platformio.org/en/latest/core/installation.html

void test_string_to_mac_address(void) {
  Serial.println("Begin test");
  String m0 = "44";
  String m1 = "17";
  String m2 = "93";
  String m3 = "1e";
  String m4 = "8e";
  String m5 = "f5";
 
  // char myMacStr[] = "44:17:93:1E:8e:f5";
  // char* endPtr;
  // Serial.print(F("0x"));
  // Serial.println(strtol(myMacStr, &endPtr, 16), HEX);
  uint8_t macinfo[6];
  char * pEnd;
  macinfo[0] = strtol(m0.c_str(),&pEnd,16);
  macinfo[1] = strtol(m1.c_str(),&pEnd,16);
  macinfo[2] = strtol(m2.c_str(),&pEnd,16);
  macinfo[3] = strtol(m3.c_str(),&pEnd,16);
  macinfo[4] = strtol(m4.c_str(),&pEnd,16);
  macinfo[5] = strtol(m5.c_str(),&pEnd,16);
  
  // int i = 0;
  // while (*endPtr) {
  //   Serial.print(F("0x"));
  //   Serial.println(strtol(endPtr + 1, &endPtr, 16), HEX);
  //   macinfo[i++] = strtol(endPtr + 1, &endPtr, 16);
  //   // Serial.printf("%c ", macinfo[i]);
  // }
  // Serial.println("\n");
  uint8_t peer[6] = {0x44, 0x17, 0x93, 0x1e, 0x8e,0xf5};
  
  for(int i = 0; i< 6; i++){
    Serial.printf("%X  ==  %X\n", macinfo[i],peer[i]);
    TEST_ASSERT_EQUAL(macinfo[i],peer[i]);
  }

  
}

void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    // delay(2000);

    UNITY_BEGIN();    // IMPORTANT LINE!
    RUN_TEST(test_string_to_mac_address);

}

// uint8_t i = 0;
// uint8_t max_blinks = 5;

void loop() {
    // if (i < max_blinks)
    // {
    //     RUN_TEST(test_led_state_high);
    //     delay(500);
    //     RUN_TEST(test_led_state_low);
    //     delay(500);
    //     i++;
    // }
    // else if (i == max_blinks) {
      UNITY_END(); // stop unit testing
    // }
}