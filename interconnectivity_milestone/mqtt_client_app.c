#include "interconnectivity_milestone.h"

extern int32_t ti_net_SlNet_initConfig();

#define APPLICATION_NAME         "MQTT client"
#define APPLICATION_VERSION      "2.0.0"

#define SL_TASKSTACKSIZE            2048
#define SPAWN_TASK_PRIORITY         9

// un-comment this if you want to connect to an MQTT broker securely
//#define MQTT_SECURE_CLIENT

#define MQTT_MODULE_TASK_PRIORITY   2
#define MQTT_MODULE_TASK_STACK_SIZE 2048

#define MQTT_WILL_TOPIC             "jesus_cc32xx_will_topic"
#define MQTT_WILL_MSG               "will_msg_works"
#define MQTT_WILL_QOS               MQTT_QOS_2
#define MQTT_WILL_RETAIN            false

#define MQTT_CLIENT_PASSWORD        NULL
#define MQTT_CLIENT_USERNAME        NULL
#define MQTT_CLIENT_KEEPALIVE       0
#define MQTT_CLIENT_CLEAN_CONNECT   true
#define MQTT_CLIENT_MQTT_V3_1       true
#define MQTT_CLIENT_BLOCKING_SEND   true

#define MQTT_CONNECTION_FLAGS           MQTTCLIENT_NETCONN_URL
#define MQTT_CONNECTION_ADDRESS         "mqtt.eclipse.org"
#define MQTT_CONNECTION_PORT_NUMBER     1883

/* Client ID                                                                 */
/* If ClientId isn't set, the MAC address of the device will be copied into  */
/* the ClientID parameter.                                                   */
char ClientId[13] = {'\0'};

MQTT_IF_InitParams_t mqttInitParams =
{
     MQTT_MODULE_TASK_STACK_SIZE,   // stack size for mqtt module - default is 2048
     MQTT_MODULE_TASK_PRIORITY      // thread priority for MQTT   - default is 2
};

MQTTClient_Will mqttWillParams =
{
     MQTT_WILL_TOPIC,    // will topic
     MQTT_WILL_MSG,      // will message
     MQTT_WILL_QOS,      // will QoS
     MQTT_WILL_RETAIN    // retain flag
};

MQTT_IF_ClientParams_t mqttClientParams =
{
     ClientId,                  // client ID
     MQTT_CLIENT_USERNAME,      // user name
     MQTT_CLIENT_PASSWORD,      // password
     MQTT_CLIENT_KEEPALIVE,     // keep-alive time
     MQTT_CLIENT_CLEAN_CONNECT, // clean connect flag
     MQTT_CLIENT_MQTT_V3_1,     // true = 3.1, false = 3.1.1
     MQTT_CLIENT_BLOCKING_SEND, // blocking send flag
     &mqttWillParams            // will parameters
};

MQTTClient_ConnParams mqttConnParams =
{
     MQTT_CONNECTION_FLAGS,         // connection flags
     MQTT_CONNECTION_ADDRESS,       // server address
     MQTT_CONNECTION_PORT_NUMBER,   // port number of MQTT server
     0,                             // method for secure socket
     0,                             // cipher for secure socket
     0,                             // number of files for secure connection
     NULL                           // secure files
};

//*****************************************************************************
//!
//! Set the ClientId with its own mac address
//! This routine converts the mac address which is given
//! by an integer type variable in hexadecimal base to ASCII
//! representation, and copies it into the ClientId parameter.
//!
//! \param  macAddress  -   Points to string Hex.
//!
//! \return void.
//!
//*****************************************************************************
int32_t SetClientIdNamefromMacAddress()
{
    int32_t ret = 0;
    uint8_t Client_Mac_Name[2];
    uint8_t Index;
    uint16_t macAddressLen = SL_MAC_ADDR_LEN;
    uint8_t macAddress[SL_MAC_ADDR_LEN];

    /*Get the device Mac address */
    ret = sl_NetCfgGet(SL_NETCFG_MAC_ADDRESS_GET, 0, &macAddressLen,
                       &macAddress[0]);

    /*When ClientID isn't set, use the mac address as ClientID               */
    if(ClientId[0] == '\0')
    {
        /*6 bytes is the length of the mac address                           */
        for(Index = 0; Index < SL_MAC_ADDR_LEN; Index++)
        {
            /*Each mac address byte contains two hexadecimal characters      */
            /*Copy the 4 MSB - the most significant character                */
            Client_Mac_Name[0] = (macAddress[Index] >> 4) & 0xf;
            /*Copy the 4 LSB - the least significant character               */
            Client_Mac_Name[1] = macAddress[Index] & 0xf;

            if(Client_Mac_Name[0] > 9)
            {
                /*Converts and copies from number that is greater than 9 in  */
                /*hexadecimal representation (a to f) into ascii character   */
                ClientId[2 * Index] = Client_Mac_Name[0] + 'a' - 10;
            }
            else
            {
                /*Converts and copies from number 0 - 9 in hexadecimal       */
                /*representation into ascii character                        */
                ClientId[2 * Index] = Client_Mac_Name[0] + '0';
            }
            if(Client_Mac_Name[1] > 9)
            {
                /*Converts and copies from number that is greater than 9 in  */
                /*hexadecimal representation (a to f) into ascii character   */
                ClientId[2 * Index + 1] = Client_Mac_Name[1] + 'a' - 10;
            }
            else
            {
                /*Converts and copies from number 0 - 9 in hexadecimal       */
                /*representation into ascii character                        */
                ClientId[2 * Index + 1] = Client_Mac_Name[1] + '0';
            }
        }
    }
    return(ret);
}

// this timer callback toggles the LED once per second until the device connects to an AP
void timerLEDCallback(Timer_Handle myHandle)
{
    GPIO_toggle(CONFIG_GPIO_LED_0);
}

void MQTT_EventCallback(int32_t event){

    switch(event){

        case MQTT_EVENT_CONNACK:
        {
            set_connect_flag();
            LOG_INFO("MQTT_EVENT_CONNACK\r\n");
            break;
        }

        case MQTT_EVENT_SUBACK:
        {
            LOG_INFO("MQTT_EVENT_SUBACK\r\n");
            break;
        }

        case MQTT_EVENT_PUBACK:
        {
            LOG_INFO("MQTT_EVENT_PUBACK\r\n");
            break;
        }

        case MQTT_EVENT_UNSUBACK:
        {
            LOG_INFO("MQTT_EVENT_UNSUBACK\r\n");
            break;
        }

        case MQTT_EVENT_DESTROY:
        {
            LOG_INFO("MQTT_EVENT_DESTROY\r\n");
            break;
        }
    }
}

/*
 * Subscribe topic callbacks
 * Topic and payload data is deleted after topic callbacks return.
 * User must copy the topic or payload data if it needs to be saved.
 */
void BrokerCB(char* topic, char* payload)
{
//    LOG_INFO("[BrokerCB]: %s \tPAYLOAD: %s\r\n", topic, payload);
}

int32_t DisplayAppBanner(char* appName, char* appVersion){

    int32_t ret = 0;
    uint8_t macAddress[SL_MAC_ADDR_LEN];
    uint16_t macAddressLen = SL_MAC_ADDR_LEN;
    uint16_t ConfigSize = 0;
    uint8_t ConfigOpt = SL_DEVICE_GENERAL_VERSION;
    SlDeviceVersion_t ver = {0};

    ConfigSize = sizeof(SlDeviceVersion_t);

    // get the device version info and MAC address
    ret = sl_DeviceGet(SL_DEVICE_GENERAL, &ConfigOpt, &ConfigSize, (uint8_t*)(&ver));
    ret |= (int32_t)sl_NetCfgGet(SL_NETCFG_MAC_ADDRESS_GET, 0, &macAddressLen, &macAddress[0]);

    UART_PRINT("\n\r\t============================================\n\r");
    UART_PRINT("\t   %s Example Ver: %s",appName, appVersion);
    UART_PRINT("\n\r\t============================================\n\r\n\r");

    UART_PRINT("\t CHIP: 0x%x\n\r",ver.ChipId);
    UART_PRINT("\t MAC:  %d.%d.%d.%d\n\r",ver.FwVersion[0],ver.FwVersion[1],
               ver.FwVersion[2],
               ver.FwVersion[3]);
    UART_PRINT("\t PHY:  %d.%d.%d.%d\n\r",ver.PhyVersion[0],ver.PhyVersion[1],
               ver.PhyVersion[2],
               ver.PhyVersion[3]);
    UART_PRINT("\t NWP:  %d.%d.%d.%d\n\r",ver.NwpVersion[0],ver.NwpVersion[1],
               ver.NwpVersion[2],
               ver.NwpVersion[3]);
    UART_PRINT("\t ROM:  %d\n\r",ver.RomVersion);
    UART_PRINT("\t HOST: %s\n\r", SL_DRIVER_VERSION);
    UART_PRINT("\t MAC address: %02x:%02x:%02x:%02x:%02x:%02x\r\n", macAddress[0],
               macAddress[1], macAddress[2], macAddress[3], macAddress[4],
               macAddress[5]);
    UART_PRINT("\n\r\t============================================\n\r");

    return(ret);
}

int WifiInit()
{
    int32_t ret;
    SlWlanSecParams_t security_params;
    pthread_t spawn_thread = (pthread_t) NULL;
    pthread_attr_t pattrs_spawn;
    struct sched_param pri_param;

    pthread_attr_init(&pattrs_spawn);
    pri_param.sched_priority = SPAWN_TASK_PRIORITY;
    ret = pthread_attr_setschedparam(&pattrs_spawn, &pri_param);
    ret |= pthread_attr_setstacksize(&pattrs_spawn, SL_TASKSTACKSIZE);
    ret |= pthread_attr_setdetachstate(&pattrs_spawn, PTHREAD_CREATE_DETACHED);
    ret = pthread_create(&spawn_thread, &pattrs_spawn, sl_Task, NULL);
    if(ret != 0)
        dbgErrorRoutine(ERROR_WIFI_INIT);

    Network_IF_ResetMCUStateMachine();
    Network_IF_DeInitDriver();

    ret = Network_IF_InitDriver(ROLE_STA);
    if(ret < 0)
        dbgErrorRoutine(ERROR_WIFI_INIT);

    DisplayAppBanner(APPLICATION_NAME, APPLICATION_VERSION);

    SetClientIdNamefromMacAddress();

    GPIO_toggle(CONFIG_GPIO_LED_2);

    security_params.Key = (signed char*)SECURITY_KEY;
    security_params.KeyLen = strlen(SECURITY_KEY);
    security_params.Type = SECURITY_TYPE;

    ret = Network_IF_ConnectAP(SSID_NAME, security_params);
    if(ret < 0)
    {
        LOG_ERROR("Connection to an AP failed\n\r");
    }
    else
    {
        SlWlanSecParams_t securityParams;

        securityParams.Type = SECURITY_TYPE;
        securityParams.Key = (signed char*)SECURITY_KEY;
        securityParams.KeyLen = strlen((const char *)securityParams.Key);

        ret = sl_WlanProfileAdd((signed char*)SSID_NAME, strlen(SSID_NAME), 0, &securityParams, NULL, 7, 0);
        if (ret < 0)
        {
            LOG_ERROR("failed to add profile %s\r\n", SSID_NAME);
        }
        else
        {
            LOG_INFO("profile added %s\r\n", SSID_NAME);
        }
    }
    return ret;
}

void mainThread(void * args)
{
    int32_t ret;
    Timer_Params params;
    Timer_Handle timer0;

    struct msgQueue msg_to_pub;

    uartHandle = InitTerm();
    UART_control(uartHandle, UART_CMD_RXDISABLE, NULL);

    GPIO_init();
    SPI_init();
    Timer_init();
    ADC_init();

    // for our ADC
    ADC_Params adcParams;

    // set up ADC for IR distance sensor
    ADC_Params_init(&adcParams);
    adc = ADC_open(CONFIG_ADC_0, &adcParams);
    if (adc == NULL)
    {
        dbgOutputLoc(ERROR_INIT_ADC);
        dbgErrorRoutine(ERROR_INIT_ADC);
    }

    ret = ti_net_SlNet_initConfig();
    if(0 != ret)
        LOG_ERROR("Failed to initialize SlNetSock\n\r");

    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_OFF);
    GPIO_write(CONFIG_GPIO_LED_1, CONFIG_GPIO_LED_OFF);
    GPIO_write(CONFIG_GPIO_LED_2, CONFIG_GPIO_LED_OFF);

    // configuring the timer to toggle an LED until the AP is connected
    Timer_Params_init(&params);
    params.period = 1000000;
    params.periodUnits = Timer_PERIOD_US;
    params.timerMode = Timer_CONTINUOUS_CALLBACK;
    params.timerCallback = (Timer_CallBackFxn)timerLEDCallback;

    timer0 = Timer_open(CONFIG_TIMER_0, &params);
    if (timer0 == NULL)
        dbgErrorRoutine(ERROR_TIMER_INIT);
    Timer_start(timer0);

    ret = WifiInit();
    if(ret < 0)
        dbgErrorRoutine(ERROR_WIFI_INIT);
    Timer_stop(timer0);
    Timer_close(timer0);

    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_OFF);
    GPIO_write(CONFIG_GPIO_LED_1, CONFIG_GPIO_LED_OFF);
    GPIO_write(CONFIG_GPIO_LED_2, CONFIG_GPIO_LED_OFF);

    ret = MQTT_IF_Init(mqttInitParams);
    if(ret < 0)
        dbgErrorRoutine(ERROR_MQTT_INIT);


    // connect to the MQTT client interface, then subscribe to topics
    mqttClientHandle = MQTT_IF_Connect(mqttClientParams, mqttConnParams, MQTT_EventCallback);
    if(mqttClientHandle < 0)
        dbgErrorRoutine(ERROR_MQTT_CONNECTION);

    ret = MQTT_IF_Subscribe(mqttClientHandle, "Broker/To/cc32xx", MQTT_QOS_0, BrokerCB);

    // subscribe to the correct chain based on our CHIAIN_NUM
    if (CHAIN_NUM == SPENCER)
    {
        ret |= MQTT_IF_Subscribe(mqttClientHandle, "spencer/Chain1", MQTT_QOS_0, ChainCB);
    }
    else if (CHAIN_NUM == MICHAEL) // listen to spencer's IR publishing
    {
        ret |= MQTT_IF_Subscribe(mqttClientHandle, "michael/Chain2", MQTT_QOS_0, ChainCB);
        ret |= MQTT_IF_Subscribe(mqttClientHandle, "spencer/task1", MQTT_QOS_0, Task1CB);
    }
    else if (CHAIN_NUM == CHARLIE)
    {
        ret |= MQTT_IF_Subscribe(mqttClientHandle, "charlie/Chain3", MQTT_QOS_0, ChainCB);
    }
    else if (CHAIN_NUM == CHRISTOS) // listen to charlie's IR publishing
    {
        ret |= MQTT_IF_Subscribe(mqttClientHandle, "christos/Chain4", MQTT_QOS_0, ChainCB);
        ret |= MQTT_IF_Subscribe(mqttClientHandle, "charlie/task1", MQTT_QOS_0, Task1CB);
    }

    if(ret < 0)
        dbgErrorRoutine(ERROR_MQTT_SUB);
    else
        LOG_INFO("Subscribed to all topics successfully\r\n");

    /////////////////////////////////////////// MAIN WHILE ////////////////////////////////////////////
    while(1)
    {
        if (event_connected() == 1)
        {
            // receive from our custom queue - then handle data based on event
            msg_to_pub = receiveData(qh);

            switch (msg_to_pub.event)
            {
                ////////////////////////////////////////////////
                case TASK1_PUB_SENSOR:
                    if (CHAIN_NUM == SPENCER)
                    {
                        dbg_msg.numPubT1 ++;
                        MQTT_IF_Publish(mqttClientHandle,
                        "spencer/task1",
                        msg_to_pub.payload,
                        msg_to_pub.data_len,
                        MQTT_QOS_0);
                    }
                    else if (CHAIN_NUM == CHARLIE)
                    {
                        dbg_msg.numPubT1 ++;
                        MQTT_IF_Publish(mqttClientHandle,
                        "charlie/task1",
                        msg_to_pub.payload,
                        msg_to_pub.data_len,
                        MQTT_QOS_0);
                    }
                    break;

                ////////////////////////////////////////////////
                case TASK1_PUB_AVERAGE:
                    if (CHAIN_NUM == MICHAEL)
                    {
                        dbg_msg.numPubT1 ++;
                        MQTT_IF_Publish(mqttClientHandle,
                            "michael/task1_avg",
                            msg_to_pub.payload,
                            msg_to_pub.data_len,
                            MQTT_QOS_0);
                    }
                    else if (CHAIN_NUM == CHRISTOS)
                    {
                        dbg_msg.numPubT1 ++;
                        MQTT_IF_Publish(mqttClientHandle,
                            "christos/task1_avg",
                            msg_to_pub.payload,
                            msg_to_pub.data_len,
                            MQTT_QOS_0);
                    }
                    break;

               ////////////////////////////////////////////////
               case PUB_ANALYTICS:
                    if (CHAIN_NUM == SPENCER)
                    {
                       MQTT_IF_Publish(mqttClientHandle,
                           "spencer/analytics",
                           msg_to_pub.payload,
                           msg_to_pub.data_len,
                           MQTT_QOS_0);
                    }
                    else if (CHAIN_NUM == MICHAEL)
                    {
                       MQTT_IF_Publish(mqttClientHandle,
                           "michael/analytics",
                           msg_to_pub.payload,
                           msg_to_pub.data_len,
                           MQTT_QOS_0);
                    }
                    else if (CHAIN_NUM == CHARLIE)
                    {
                       MQTT_IF_Publish(mqttClientHandle,
                           "charlie/analytics",
                           msg_to_pub.payload,
                           msg_to_pub.data_len,
                           MQTT_QOS_0);
                    }
                    else if (CHAIN_NUM == CHRISTOS)
                    {
                       MQTT_IF_Publish(mqttClientHandle,
                           "christos/analytics",
                           msg_to_pub.payload,
                           msg_to_pub.data_len,
                           MQTT_QOS_0);
                    }
                    break;
               ////////////////////////////////////////////////
               case TASK2_PUB_MSG:

                   // increment number published
                   dbg_msg.numPubT2 ++;

                   if (CHAIN_NUM == SPENCER)
                   {
                       MQTT_IF_Publish(mqttClientHandle,
                           "michael/Chain2",
                           msg_to_pub.payload,
                           msg_to_pub.data_len,
                           MQTT_QOS_0);
                   }
                   else if (CHAIN_NUM == MICHAEL)
                   {
                       MQTT_IF_Publish(mqttClientHandle,
                           "charlie/Chain3",
                           msg_to_pub.payload,
                           msg_to_pub.data_len,
                           MQTT_QOS_0);
                   }
                   else if (CHAIN_NUM == CHARLIE)
                   {
                       MQTT_IF_Publish(mqttClientHandle,
                           "christos/Chain4",
                           msg_to_pub.payload,
                           msg_to_pub.data_len,
                           MQTT_QOS_0);
                   }
                   else if (CHAIN_NUM == CHRISTOS)
                   {
                       MQTT_IF_Publish(mqttClientHandle,
                           "cc32xx/ChainResult",
                           msg_to_pub.payload,
                           msg_to_pub.data_len,
                           MQTT_QOS_0);
                   }
                   break;

               ////////////////////////////////////////////////
               default:
                   break;
            }
        }
    }
}
