
#include "ESPino32CAM.h"
#include "ESPino32CAM_QRCode.h"
ESPino32CAM cam;
ESPino32QRCode qr;
#define BUTTON_QR 0
#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"

#define BUTTON_QR 0
#define CAMERA_MODEL_AI_THINKER
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  20       /* Time ESP32 will go to sleep (in seconds) */


RTC_DATA_ATTR int bootCount = 0;


int print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;
 
  wakeup_reason = esp_sleep_get_wakeup_cause();
 
  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); return 1 ; break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); return 2; break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason);return 0; break;
  }
}

void inicializacion(){
  delay(10000); // de calibracion del sensor PIR 
  Serial.printf("Inicializando ");
   
  }

void captura_codigo_QR(){

  pinMode(4, OUTPUT);
  if (cam.init() != ESP_OK)
  {
    Serial.println(F("Fail"));
    while (1);
  }
  qr.init(&cam);
  sensor_t *s = cam.sensor();
  s->set_framesize(s, FRAMESIZE_VGA);
  s->set_whitebal(s,true);
  int capturado=0;
  int veces = 0;

 
while(capturado == 0 & veces < 10 ){
  camera_fb_t *fb = cam.capture();
  if (!fb)
  {
    Serial.println("Camera capture failed");
    return;
  }
  //Serial.write(fb->buf, fb->len);
  Serial.write("-----------------------------------");
  dl_matrix3du_t *image_rgb;
  if(cam.jpg2rgb(fb,&image_rgb))
  {
     cam.clearMemory(fb);
     if(!digitalRead(BUTTON_QR))
     {
        cam.printDebug("\r\nQR Read:");
        qrResoult res = qr.recognition(image_rgb);
       if(res.status)
       {
          cam.printDebug("");
          cam.printfDebug("Version: %d", res.version);
          cam.printfDebug("ECC level: %c",res.eccLevel);
          cam.printfDebug("Mask: %d", res.mask);
          cam.printDebug("Data type: "+ qr.dataType(res.dataType));
          cam.printfDebug("Length: %d",res.length);
          cam.printDebug("Payload: "+res.payload);
         
                   
          
          digitalWrite(4,HIGH);
          delay(500);
          digitalWrite(4,LOW);
          delay(500);
          Serial.print("Registrado"); 
          capturado=1;
          
       }
       else
          cam.printDebug("FAIL");
          veces = veces+1;
          Serial.print(veces); 
       }
  }
  cam.clearMemory(image_rgb);
}
  

};

void lectura_temperatura(){
   Serial.print(" toma medidas de temperatura");

};




void setup() {
  Serial.begin(115200);
  Serial.println("\r\nESPino32CAM DECODER ");
 //Increment boot number and print it every reboot
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));
 
  //Print the wakeup reason for ESP32
int reason = print_wakeup_reason();

if (reason == 1){
    captura_codigo_QR();
  }else if (reason == 2){
    lectura_temperatura();
    } else {
      inicializacion();
      delay(1000);
      } 
  

    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_13, 1);

     
      Serial.println("Going to sleep now");
      Serial.println("Now in Deep Sleep Mode");
      delay(3000);
      esp_deep_sleep_start();
         
          
}
void loop()
{
   
}
