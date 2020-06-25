#include "ApollonDriver.h"



int main(int argc, char *argv[])
{
    
    SensorInitialization();
    
    uint64_t loop_count = 10;
    uint64_t i;
    
    for(i=0;i<loop_count;i++){
    
        double PositionXOfBall1 = PtolemySensor("ball1", "PositionX");
        printf("The value of PositionX Of Ball1 is: %f\n",PositionXOfBall1);
        
        double PositionYOfBall1 = PtolemySensor("ball1", "PositionY");
        printf("The value of PositionY Of Ball1 is: %f\n",PositionYOfBall1);
        
        
        PtolemyActuator("ball2", "PositionX", 123.789, "PositionY", 555.321);
        
        delay(1000); // waits for a second
    
    }
    
    
    SensorFinalization();
 
    return 0;
}
