#include <Adafruit_MCP4725.h>//12 bit DAC
#include <Adafruit_ADS1X15.h>//ADC
#include <Wire.h>

Adafruit_ADS1115 ads1115;//Construct an ads1115
Adafruit_MCP4725 mcp4725;//construct an mcp4725

const int bias = 8;//pin for sample bias output

void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);//init serial comm at baudRate = 9600
  pinMode(bias,OUTPUT);//outputs 5V dc 

  ads1115.begin();//Initialize adc
  mcp4725.begin();//initialize dac
  

}

void loop() {
  // put your main code here, to run repeatedly:

  //set bias
  digitalWrite(bias,HIGH);

  //adjust input gain to get range of ± 256 mV with a resolution of 7.8125uV
  ads1115.setGain(GAIN_SIXTEEN);

  ///////////////////PI control///////////////////////

  int z = 0;//z driver signal 
  int Prop = 2;//proportional constant
  int Int = 1e-10;//integral constant 

  while(1) {

    //get preamp output and call it "ampOut"
    int ampOut;
    ampOut = (ads1115.readADC_Differential_0_1());//adc resolution is 16 bits
    int err; 
    err = ampOut - 128; //preamp output minus the set point. first try 2^7 
    
    //calculate the proportional of the preamp output and the integral of the previous z value and add them
    z = (err*Prop)+(z*Int);

    //write z to the dac
    mcp4725.setVoltage(z, false);  
    
    //output z signal value: 
    int results;
    results = z;
    Serial.print(results*3);//output in mV
    Serial.println();
     
  }
}
