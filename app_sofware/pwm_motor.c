/*
 * PWM Avalon MM base address (from nios_system_sdram.qsys)
 * pwm_avalon_interface_0.avalon_slave_0 baseAddress = 0x1000
 */
#define PWM_BASE_ADDR   0x00001000

/* Motor command format (14 bits):
 * bit13 = GO (1=enable)
 * bit12 = DIR (0=forward, 1=backward)
 * bits11..0 = speed (0..4095)
 */
#define MOTOR_GO_MASK    0x2000
#define MOTOR_DIR_MASK   0x1000
#define MOTOR_SPEED_MASK 0x0FFF

#define IOWR(base, data) (*((volatile unsigned int*)(base)) = (data))

static unsigned int motor_cmd(unsigned int speed, unsigned int dir, unsigned int go)
{
    unsigned int cmd = (speed & MOTOR_SPEED_MASK);
    if (dir) {
        cmd |= MOTOR_DIR_MASK;
    }
    if (go) {
        cmd |= MOTOR_GO_MASK;
    }
    return cmd;
}

static void pwm_write(unsigned int right_cmd, unsigned int left_cmd)
{
    /* Packing: [13:0] = right, [27:14] = left */
    unsigned int value = ((left_cmd & 0x3FFF) << 14) | (right_cmd & 0x3FFF);
    IOWR(PWM_BASE_ADDR, value);
}

int main(void)
{
    /* Exemple simple: deux moteurs en avant, vitesse 1200 */
    unsigned int right = motor_cmd(1200, 0, 1);
    unsigned int left  = motor_cmd(1200, 0, 1);

    pwm_write(right, left);

    /* Arret (GO=0) */
    right = motor_cmd(0, 0, 0);
    left  = motor_cmd(0, 0, 0);
    pwm_write(right, left);

    while (1) {
    }

    return 0;
}
