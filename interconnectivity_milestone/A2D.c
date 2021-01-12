#include "A2D.h"

// use this for mapping to correct distances
// FORMAT: correct value, value computed by ADC
int distance_mapping[12][2] = {
    {40, 30},
    {80, 56},
    {120, 79},
    {160, 110},
    {200, 134},
    {240, 164},
    {280, 199},
    {320, 220},
    {360, 242},
    {400, 273},
    {440, 308},
    {480, 300}
};


// min dist: 40mm, max dist: 500mm
int getIRSensorReading(ADC_Handle adc) {

    dbgOutputLoc(GET_IR_SENSOR_READING);

    uint16_t     adcValue0;
    int          dist_mm = 0;
    uint32_t     adcValue0MicroVolt;
    int_fast16_t res;

    /* Blocking mode conversion */
    res = ADC_convert(adc, &adcValue0);

    if (res == ADC_STATUS_SUCCESS)
    {
        adcValue0MicroVolt = ADC_convertRawToMicroVolts(adc, adcValue0);
        dist_mm = (int)(10*(1 / (0.2633 * ((double)adcValue0MicroVolt / 1000000))));
    }

    if (dist_mm < 50)
        return -1;
    if (dist_mm > 200)
        return -1;

    // now - map this based on our table we created
//    int i;
//    for (i = 0; i < 12; i ++)
//    {
//        // if we're smaller than this number, let's compare based on last two
//        if (dist_mm < distance_mapping[i][1])
//        {
//            // else, reassign a better value
//            int adc_error = (int)(dist_mm - distance_mapping[i-1][1]);
//
//            dist_mm = distance_mapping[i-1][0] + adc_error;
//        }
//    }

    // error-checking is already done - should be good to go
    return dist_mm;
}
