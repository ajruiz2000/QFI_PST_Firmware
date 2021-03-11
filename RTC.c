
#include "globals.h"
#include "main.h"


update_RTC(void){
if (RTC_Timer >= 6000){
        RTC_Timer = 0;
        RTC_Minutes = RTC_Minutes +1;
        if (RTC_Minutes >= 60){
            RTC_Minutes = 0;
            RTC_Hours = RTC_Hours +1;
            }
        }
    service_Minutes = service_Minutes + RTC_Minutes;
    servive_Hours = service_Hours + service_Minutes;
    total_Hours = total_Hours + RTC_Hours;
    total_Minutes = total_Minutes + RTC_Minutes;
    
    if (service_Hours > 2000){
        service_Needed = 1;
    }
    else{
        service_Needed = 0;
    }
}