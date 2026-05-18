LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.numeric_std.ALL;

ENTITY pwm_avalon_interface IS
PORT (
    clock       : IN  STD_LOGIC;
    resetn      : IN  STD_LOGIC;
    read        : IN  STD_LOGIC;
    write       : IN  STD_LOGIC;
    chipselect  : IN  STD_LOGIC;
    writedata   : IN  STD_LOGIC_VECTOR(31 DOWNTO 0);
    byteenable  : IN  STD_LOGIC_VECTOR(3 DOWNTO 0);
    readdata    : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    dc_motor_p_R: OUT STD_LOGIC;
    dc_motor_n_R: OUT STD_LOGIC;
    dc_motor_p_L: OUT STD_LOGIC;
    dc_motor_n_L: OUT STD_LOGIC
);
END pwm_avalon_interface;

ARCHITECTURE Structure OF pwm_avalon_interface IS

    SIGNAL motor_reg      : STD_LOGIC_VECTOR(31 DOWNTO 0);
    SIGNAL motor_right_reg: STD_LOGIC_VECTOR(13 DOWNTO 0);
    SIGNAL motor_left_reg : STD_LOGIC_VECTOR(13 DOWNTO 0);

    COMPONENT PWM_generation
    PORT (
        clk, reset_n     : IN  STD_LOGIC;
        s_writedataR     : IN  STD_LOGIC_VECTOR(13 DOWNTO 0);
        s_writedataL     : IN  STD_LOGIC_VECTOR(13 DOWNTO 0);
        dc_motor_p_R     : OUT STD_LOGIC;
        dc_motor_n_R     : OUT STD_LOGIC;
        dc_motor_p_L     : OUT STD_LOGIC;
        dc_motor_n_L     : OUT STD_LOGIC
    );
    END COMPONENT;

BEGIN

    -- Avalon MM write with byteenable (1 registre 32 bits)
    PROCESS (clock, resetn)
    BEGIN
        IF resetn = '0' THEN
            motor_reg <= (others => '0');
        ELSIF rising_edge(clock) THEN
            IF (chipselect = '1') AND (write = '1') THEN
                IF byteenable(0) = '1' THEN
                    motor_reg(7 DOWNTO 0) <= writedata(7 DOWNTO 0);
                END IF;
                IF byteenable(1) = '1' THEN
                    motor_reg(15 DOWNTO 8) <= writedata(15 DOWNTO 8);
                END IF;
                IF byteenable(2) = '1' THEN
                    motor_reg(23 DOWNTO 16) <= writedata(23 DOWNTO 16);
                END IF;
                IF byteenable(3) = '1' THEN
                    motor_reg(31 DOWNTO 24) <= writedata(31 DOWNTO 24);
                END IF;
            END IF;
        END IF;
    END PROCESS;

    -- Lecture directe du registre
    readdata <= motor_reg;

    -- Decoupage: [13:0] = moteur droit, [27:14] = moteur gauche
    motor_right_reg <= motor_reg(13 DOWNTO 0);
    motor_left_reg  <= motor_reg(27 DOWNTO 14);

    PWM_Core: PWM_generation
    PORT MAP (
        clk => clock,
        reset_n => resetn,
        s_writedataR => motor_right_reg,
        s_writedataL => motor_left_reg,
        dc_motor_p_R => dc_motor_p_R,
        dc_motor_n_R => dc_motor_n_R,
        dc_motor_p_L => dc_motor_p_L,
        dc_motor_n_L => dc_motor_n_L
    );

END Structure;
