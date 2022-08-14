/*******************************************************************
 Connect to Twtich Chat with a Bot
 Created with code from TheOtherLoneStar (https://www.twitch.tv/theotherlonestar)
 Hackaday IO: https://hackaday.io/otherlonestar
 By Brian Lough (https://www.twitch.tv/brianlough)
 YouTube: https://www.youtube.com/channel/UCezJOfu7OtqGzd5xrP3q6WA
 Created with code from noycebru www.twitch.tv/noycebru
*******************************************************************/
 
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <IRCClient.h>

#define IRC_SERVER   "irc.chat.twitch.tv"
#define IRC_PORT     6667
 
const char ssid[] = "";       // SSID da sua rede
const char password[] = "";  // Senha da sua rede
 
// Nome do seu canal da Twitch
const String twitchChannelName = ""; //é case sensitive!
 
//Nome do BOT
#define TWITCH_BOT_NAME ""
 
//OAuth Key for your twitch bot
// https://twitchapps.com/tmi/
#define TWITCH_OAUTH_TOKEN ""

#define RED 5
#define GREEN 4
#define BLUE 14
 
String ircChannel = "";
 
WiFiClient wiFiClient;
IRCClient client(IRC_SERVER, IRC_PORT, wiFiClient);

void sendTwitchMessage(String message) {
  client.sendMessage(ircChannel, message);
}

void set_led_color (int r, int g, int b) {
  analogWrite(RED, r);
  analogWrite(GREEN, g);
  analogWrite(BLUE, b);
} 
 
void callback(IRCMessage ircMessage) {
  if (ircMessage.command == "PRIVMSG" && ircMessage.text[0] != '\001') {
    ircMessage.nick.toUpperCase();
 
    String message("<" + ircMessage.nick + "> " + ircMessage.text);
 
    //prints chat to serial
    Serial.println(message);

    //onde configura a cor selecionada na compra do item;
    if (ircMessage.text.indexOf("Vermelho") > -1 && ircMessage.nick == "STREAMELEMENTS"){
        set_led_color(255, 0 , 0);
        delay(2000);
    }

    if (ircMessage.text.indexOf("Verde") > -1 && ircMessage.nick == "STREAMELEMENTS"){
        set_led_color(0, 255 , 0);
        delay(2000);
    }

    if (ircMessage.text.indexOf("Azul") > -1 && ircMessage.nick == "STREAMELEMENTS"){
        set_led_color(0, 0 , 255);
        delay(2000);
    }

    if (ircMessage.text.indexOf("Amarelo") > -1 && ircMessage.nick == "STREAMELEMENTS"){
        set_led_color(255, 255 , 0);
        delay(2000);
    }

    if (ircMessage.text.indexOf("Roxo") > -1 && ircMessage.nick == "STREAMELEMENTS"){
        set_led_color(128, 0, 128);
        delay(2000);
    }

    if (ircMessage.text.indexOf("Rosa") > -1 && ircMessage.nick == "STREAMELEMENTS"){
        set_led_color(255, 0, 127);
        delay(2000);
    }

    return;
  }
}
 
// put your setup code here, to run once:
void setup() {
 
  Serial.begin(115200);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  Serial.println();
 
  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
 
  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  ircChannel = "#" + twitchChannelName;
 
  client.setCallback(callback);
}
 
void loop() {
 
  // Try to connect to chat. If it loses connection try again
  if (!client.connected()) {
    Serial.println("Attempting to connect to " + ircChannel );
    // Attempt to connect
    // Second param is not needed by Twtich
    if (client.connect(TWITCH_BOT_NAME, "", TWITCH_OAUTH_TOKEN)) {
      client.sendRaw("JOIN " + ircChannel);
      Serial.println("conectado e funcionando");
      sendTwitchMessage("O BOT ta on ein...");
    } else {
      Serial.println("failed... try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
    return;
  }
  client.loop();
}