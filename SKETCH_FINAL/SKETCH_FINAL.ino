#include <PubSubClient.h>
#include <ESP8266WiFi.h>

#define BUZZER_PIN D7

#define LED_VERMELHO D5

const char* ssid = "CASA";
const char* password = "empresa2010";
const char* mqtt_server = "mqtt.eclipse.org";
const char* mqttUser = "ambi";             
const char* mqttPassword = "5276";     
const int mqttPort = 1883;                    

char pub_str[100];



float gettemp();

WiFiClient espClient;
PubSubClient client(espClient);
void setup_wifi()
{

delay(10);

Serial.print("Conectando na rede ");
 Serial.println(ssid);
 WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED){
 delay(500);
 Serial.print("-");
 }

Serial.println();
 Serial.println("WiFi Conectado");
 Serial.println("IP ");
 Serial.println();
 Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length){


Serial.print("Mensagem recebida : ");
 Serial.print(topic);
 Serial.print(" : ");
 pinMode(LED_VERMELHO, OUTPUT);
 for (int i = 0; i < length; i++){
 Serial.println((char)payload[i]);
 
 }

 pinMode(LED_VERMELHO, OUTPUT);
 pinMode(BUZZER_PIN, OUTPUT);
 float val = analogRead(A0);
 float c = val * 285.0 / 1024.0;
 if ((char)payload[0] == 'o' && (char)payload[1] == 'n'){
      digitalWrite(LED_VERMELHO, HIGH);
 
      
   }
  else if ((char)payload[0] == 'o' && (char)payload[1] == 'f' && (char)payload[2] == 'f') {
      digitalWrite(LED_VERMELHO, LOW);

 }



 
}


void reconnect(){

while(!client.connected()){
Serial.println("Tentando conexão com o MQTT");
if(client.connect("...")){
Serial.println("Conectado");
client.publish("ambitemp_temperatura","Conectado!");
client.subscribe("ambitemp_led");
Serial.print("Inscrito!");

}
else{
Serial.print("Falha, rc = ");
Serial.print(client.state());
Serial.println("Nova tentativa em 5 segundos ");
delay(5000);
 }
 }
}

void setup(){
 
 pinMode(LED_VERMELHO, OUTPUT);
 pinMode(BUZZER_PIN, OUTPUT);
 digitalWrite(BUZZER_PIN, LOW); 
 Serial.begin(115200);
 setup_wifi();
 client.setServer(mqtt_server, 1883);
 client.setCallback(callback);
 reconnect();
}

void loop()
{

if(!client.connected()){
reconnect();
Serial.print("Desconectado");
}

float tmp = gettemp();
dtostrf(tmp,2,2,pub_str);
Serial.print(pub_str);
Serial.println(tmp);
client.publish("ambitemp_temperatura",pub_str);
delay(1000);



client.loop();
}

float gettemp() {
 float val = analogRead(A0);
 float c = val * 285.0 / 1024.0;

 if (c > 27) {
    digitalWrite(LED_VERMELHO, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
 }
 else{
    digitalWrite(LED_VERMELHO, LOW);
    digitalWrite(BUZZER_PIN, LOW);
 }
  
 Serial.print("Temperatura = ");
 return c;

}
