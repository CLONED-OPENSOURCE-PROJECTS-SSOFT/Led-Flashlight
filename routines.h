
#define STOP 0x00
#define CK 0x01
#define CK8 0x02
#define CK64 0x03
#define CK256 0x04
#define CK1024 0x05
#define EXTERNAL_PIN_T0_FALLING_EDGE 0x06
#define EXTERNAL_PIN_T0_RISING_EDGE 0x07
#define SHUTDOWN_LATCH 750
void init_mpu(void);
void start_timer0(int frequency);
void start_timer1_pwm(unsigned short pwm);
void timer0_ovf_corrector(void);
