#define INDICATE 4
#define OFF	0
#define SHUTDOWN_DELAY 750
#define WAKE_DELAY 750
#define MAX_BRIGHTNESS 7
#define LOW_AV_BR 3
#define HIGH_AV_BR 5
#define TIMER1_PREDIVISOR_1024 0x05
#define TIMER1_PREDIVISOR_256 0x04
#define TIMER1_PREDIVISOR_64 0x03
#define TIMER1_PREDIVISOR_8 0x02
#define TIMER1_PREDIVISOR_1 0x01
#define TIMER1_OFF 0x00
#define SHUTDOWN_LATCH 750

char start_timer1_ctc(unsigned int value);

void init_mpu(void);

void set_brightness(unsigned short mode);
void indicator(unsigned int adc);
