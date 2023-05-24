#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

// Konfigurasi Firebase
// #define FIREBASE_HOST "kendalilampu-234dc-default-rtdb.firebaseio.com"
// #define FIREBASE_AUTH "shGIrBZaUTsGgPHH25ztYORwp9DASreOKmHu39sQ"

// Konfigurasi WiFi
#define WIFI_SSID "Reza.Net"
#define WIFI_PASSWORD "reza2023"

// Firebase Auth
#define USER_EMAIL "kendalilampu@smkcampa.com"
#define USER_PASSWORD "12345678"
#define API_KEY "AIzaSyBfeAm0LnkBZ934ldZdpBi_QeA0LmvHTmI"
#define DATABASE_URL "kendalilampu-234dc-default-rtdb.asia-southeast1.firebasedatabase.app"

// Konfigurasi pin
#define LAMP1_PIN D1
#define LAMP2_PIN D2

// Define the FirebaseAuth data for authentication data
FirebaseAuth auth;

// Define the FirebaseConfig data for config data
FirebaseConfig config;


// Inisialisasi objek Firebase
FirebaseData firebaseData;

void setup() {
  pinMode(LAMP1_PIN, OUTPUT);
  pinMode(LAMP2_PIN, OUTPUT);
  
  Serial.begin(9600);

  // Menghubungkan ke WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi..");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.print("Connected to WiFi. IP address: ");
  Serial.println(WiFi.localIP());

  // Menginisialisasi Firebase
  // Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  // Firebase.reconnectWiFi(true);
  

config.api_key = API_KEY;
  config.database_url = DATABASE_URL;


// Assign the user sign in credentials
auth.user.email = USER_EMAIL;

auth.user.password = USER_PASSWORD;

// Initialize the library with the Firebase authen and config.
Firebase.begin(&config, &auth);

// Optional, set AP reconnection in setup()
Firebase.reconnectWiFi(true);

// Optional, set number of error retry
Firebase.setMaxRetry(firebaseData, 3);

// Optional, set number of error resumable queues
Firebase.setMaxErrorQueue(firebaseData, 30);

// Optional, use classic HTTP GET and POST requests. 
// This option allows get and delete functions (PUT and DELETE HTTP requests) works for 
// device connected behind the Firewall that allows only GET and POST requests.   
Firebase.enableClassicRequest(firebaseData, true);

// Optional, set the size of BearSSL WiFi to receive and transmit buffers
// Firebase may not support the data transfer fragmentation, you may need to reserve the buffer to match
// the data to be transported.
firebaseData.setBSSLBufferSize(1024, 1024); // minimum size is 512 bytes, maximum size is 16384 bytes

// Optional, set the size of HTTP response buffer
// Prevent out of memory for large payload but data may be truncated and can't determine its type.
firebaseData.setResponseSize(1024); // minimum size is 1024 bytes
}

void loop() {
  // Mengecek perubahan di Firebase
  Firebase.getJSON(firebaseData, "/lamp_control");

  if (firebaseData.dataAvailable()) {
    FirebaseJson &json1 = firebaseData.jsonObject();
    FirebaseJsonData l1;
    json1.get(l1, "lamp1");
    FirebaseJsonData l2;
    json1.get(l2, "lamp2");

    // Mengecek nilai pada Firebase
    String lamp1Status = l1.stringValue;
    String lamp2Status = l2.stringValue;
    Serial.print("Status lamp 1 : ");
    Serial.println(lamp1Status);
    Serial.print("Status lamp 2 : ");
    Serial.println(lamp2Status);

    // Mengendalikan lampu 1
    if (lamp1Status == "on") {
      digitalWrite(LAMP1_PIN, LOW);
    } else {
      digitalWrite(LAMP1_PIN, HIGH);
    }

    // Mengendalikan lampu 2
    if (lamp2Status == "on") {
      digitalWrite(LAMP2_PIN, LOW);
    } else {
      digitalWrite(LAMP2_PIN, HIGH);
    }
  }

  delay(100);
}
