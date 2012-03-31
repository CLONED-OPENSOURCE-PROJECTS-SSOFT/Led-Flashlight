#define TIMER1_PREDIVISOR_1024 0x05
#define TIMER1_PREDIVISOR_256 0x04
#define TIMER1_PREDIVISOR_64 0x03
#define TIMER1_PREDIVISOR_8 0x02
#define TIMER1_PREDIVISOR_1 0x01
#define TIMER1_OFF 0x00

#define PERIOD 0
#define FREQUENCY 1
void start_timer1_ctc(char mode,unsigned int value);

#define SHUTDOWN_LATCH 750
void init_mpu(void);

void set_brightness(unsigned short mode);
void indicator(unsigned int adc);
