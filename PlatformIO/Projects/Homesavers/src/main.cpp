#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

#define TRIG1 25
#define ECHO1 26
#define TRIG2 19
#define ECHO2 17
#define LED1  12
#define LED2  5

#define SOUND_SPEED 0.034
#define DISTANCE_THRESHOLD 5 


const char* ap_ssid = "ESP32_HomeSaver";
const char* ap_password = "12345678";


AsyncWebServer server(80);

int led1State = 0;
int led2State = 0;
int distanceCm1 = 0;
int distanceCm2 = 0;



const char* html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>Home Saver</title>
<style>
body { font-family: Arial; background: #52d24c; color: white; text-align: center; margin: 0; }
header { display:flex; align-items:center; justify-content:center; gap:15px; padding:20px; }
header img { width:60px; border-radius:8px; }
header h1 { margin:0; }
.box { background:#52d24c; padding:30px; margin:20px auto; width:320px; border-radius:15px; box-shadow:0 0 15px rgba(0,0,0,0.5); position:relative; }
.status { font-size:18px; margin-bottom:10px; }
input[type=range] { width:100%; }
img.gift { display:none; width:80px; margin:10px auto; }
</style>
</head>
<body>

<header>
  <!-- BASE64 logo embedded -->
  <h1>🏠 Home Saver</h1>
</header>

<div class="box">
  <h2>Front Door</h2>
  <p class="status" id="frontStatus">Distance: 0 cm<br>Status: SAFE</p>
  <input type="range" min="0" max="1" value="0" id="frontSlider" disabled>
</div>

<div class="box">
  <h2>Back Door</h2>
  <p class="status" id="backStatus">Distance: 0 cm<br>Status: SAFE</p>
  <input type="range" min="0" max="1" value="0" id="backSlider" disabled>
</div>

<script>
const frontSlider = document.getElementById("frontSlider");
const backSlider  = document.getElementById("backSlider");
const frontStatus = document.getElementById("frontStatus");
const backStatus  = document.getElementById("backStatus");
const frontGift   = document.getElementById("frontGift");
const backGift    = document.getElementById("backGift");

function updateStatus() {
  fetch("/status")
    .then(res => res.json())
    .then(data => {
      frontSlider.value = data.led1;
      backSlider.value  = data.led2;

      frontStatus.innerHTML =
        "Distance: <b>" + data.d1 + " cm</b><br>Status: <b>" + (data.led1 ? "Movement/Open" : "Close") + "</b>";

      backStatus.innerHTML =
        "Distance: <b>" + data.d2 + " cm</b><br>Status: <b>" + (data.led2 ? "Movement/Open" : "Close") + "</b>";
    });
}

setInterval(updateStatus, 1000);
</script>

</body>
</html>
)rawliteral";



void setup() {
  Serial.begin(115200);

  pinMode(TRIG1, OUTPUT); pinMode(ECHO1, INPUT);
  pinMode(TRIG2, OUTPUT); pinMode(ECHO2, INPUT);
  pinMode(LED1, OUTPUT); pinMode(LED2, OUTPUT);
  digitalWrite(LED1, LOW); digitalWrite(LED2, LOW);

 
  WiFi.softAP(ap_ssid, ap_password);
  Serial.print("ESP32 AP IP: ");
  Serial.println(WiFi.softAPIP());


  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", html);
  });


  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = "{";
    json += "\"d1\":" + String(distanceCm1) + ",";
    json += "\"d2\":" + String(distanceCm2) + ",";
    json += "\"led1\":" + String(led1State) + ",";
    json += "\"led2\":" + String(led2State);
    json += "}";
    request->send(200, "application/json", json);
  });

  server.begin();
}


void loop() {
  // Sensor 1
  // Clears the trigPin
  digitalWrite(TRIG1, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(TRIG1, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG1, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  int duration1 = pulseIn(ECHO1, HIGH, 30000);


  
  // Calculate the distance
  distanceCm1 = duration1 * SOUND_SPEED/2;
  
  // Prints the distance in the Serial Monitor
  delay(1000);

  // Sensor 2
  // Clears the trigPin
  digitalWrite(TRIG2, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(TRIG2, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG2, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  int duration2 = pulseIn(ECHO2, HIGH, 30000);
  
  // Calculate the distance
  distanceCm2 = duration2 * SOUND_SPEED/2;

    
    if (distanceCm1 <= DISTANCE_THRESHOLD) {
      digitalWrite(LED1, LOW);
      led1State = 0;
    } else {
      digitalWrite(LED1, HIGH);
      led1State = 1;
    }

    if (distanceCm2 <= DISTANCE_THRESHOLD) {
      digitalWrite(LED2, LOW);
      led2State = 0;
    } else {
      digitalWrite(LED2, HIGH);
      led2State = 1;
    }
    
    Serial.print("Front: ");
    Serial.print(distanceCm1);
    Serial.print(" cm | Back: ");
    Serial.print(distanceCm2);
    Serial.println(" cm");
}