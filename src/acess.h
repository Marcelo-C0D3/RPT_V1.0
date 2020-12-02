#include <Arduino.h>

class Acess
{
private:
    const String ssid = "*************";                                // REPLACE mySSID WITH YOUR WIFI SSID
    const String pass = "************";                                      // REPLACE myPassword YOUR WIFI PASSWORD, IF ANY
    const String token = "**********************"; // REPLACE myToken WITH YOUR TELEGRAM BOT TOKEN

public:
    
    const int64_t somMaior = -402693523;
    String checkWiFiConect();
};
