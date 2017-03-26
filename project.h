#include <stdint.h>

typedef uint8_t tWord;
typedef uint8_t tByte;
typedef uint32_t tLong;

#define RED_LED BIT0
#define GREEN_LED BIT6
#define BUTTON BIT3

#define DCO_FREQ 16*1e6
//In milliseconds
#define TIMER_INTERVAL 10

#define DIVIDER 8
#define TMRC (DCO_FREQ / DIVIDER / 1000 * TIMER_INTERVAL)
