#define SIMPLE_MACRO 12345

#define LOGCRAZY(Origin, LogFormat, ...) \
		MyLoG(LOG_LEVEL_CRAZY, Origin, LogFormat, ##__VA_ARGS__)


extern void vAssertCalled( uint32_t ulLine, const char *pcFilename );

extern uint32_t MyExternVar;

const char LogTextOrigin[LOG_ORIGIN_COUNT][5] = { 
    "ABCD", 
    "EFGH ", 
    "IJLK" };

void myFunction(LOG_ORIGIN_COUNT x) {
    LOG_ORIGIN_COUNT();
   // printf("O valor de x é: %d\n", x);
}

void myFunction(int x);

typedef unsigned int myTypedef;

struct MyStruct {
    int id;
    char LOG_ORIGIN_COUNT[50];
};

enum MyEnum {
    VALUE_ONE,
    VALUE_TWO,
    VALUE_THREE
};

union MyUnion {
    int intValue;
    float floatValue;
};

__IO uint32_t uwTick;

__weak void HAL_IncTick(void)
{
  uwTick++;
}

UartClient debugClient;

#if( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
	BaseType_t xTaskCreate(	TaskFunction_t pxTaskCode,
							const char * const pcName,	/*lint !e971 Unqualified char types are allowed for strings and single characters only. */
							const configSTACK_DEPTH_TYPE usStackDepth,
							void * const pvParameters,
							UBaseType_t uxPriority,
							TaskHandle_t * const pxCreatedTask ) PRIVILEGED_FUNCTION;
#endif

extern STRUCT_READONLY_HW deviceID;