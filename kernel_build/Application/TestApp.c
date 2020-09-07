#include "ApollonDriver.h"

#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    
    SensorInitialization();
    
    uint64_t loop_count = 500; // 5sec example
    uint64_t i;
    
    struct timeval start, end;
    
    double local_x = 0,      local_y = 0;
    int    toogle_x = 0,     toogle_y = 0;
    
    for(i=0;i<loop_count;i++){
    
        printf("\n --- Gem5SimulatedTime: %d ms ---\n", Gem5SimulatedTime());
        
        gettimeofday(&start, NULL);
        
        double PositionXOfBall1 = PtolemySensor("ball1", "PositionX");
        printf("The value of PositionX Of Ball1 is: %f\n",PositionXOfBall1);
        
        double PositionYOfBall1 = PtolemySensor("ball1", "PositionY");
        printf("The value of PositionY Of Ball1 is: %f\n",PositionYOfBall1);
        
        /* Set random x in range (0,500) *
         * Set random y in range (0,100) */
        if(toogle_x == 0){
            local_x++;
            if(local_x>=500)
                toogle_x = 1;
        }
        else{
            local_x--;
            if(local_x <= 0)
                toogle_x = 0;
        }
        
        if(toogle_y == 0){
            local_y++;
            if(local_y>=100)
                toogle_y = 1;
        }
        else{
            local_y--;
            if(local_y <= 0)
                toogle_y = 0;
        }
        
        
        PtolemyActuator("ball2", "PositionX", local_x, "PositionY", local_y);
        
        gettimeofday(&end, NULL);

        printf("%ld\n", ((end.tv_sec * 1000000 + end.tv_usec)
		  - (start.tv_sec * 1000000 + start.tv_usec)));
        
        usleep(10000); // waits for 10ms
    
    }
    
    
    SensorFinalization();
 
    return 0;
}
