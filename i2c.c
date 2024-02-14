
#include <xc.h>

void init_i2c(void)
{
	/* Set SCL and SDA pins as inputs */
	TRISC3 = 1;
	TRISC4 = 1;
	/* Set I2C master mode */
	SSPCON1 = 0x28;

	SSPADD = 0x31;
	/* Use I2C levels, worked also with '0' */
	CKE = 0;
	/* Disable slew rate control  worked also with '0' */
	SMP = 1;
	/* Clear SSPIF interrupt flag */
	SSPIF = 0;
	/* Clear bus collision flag */
	BCLIF = 0;
}

 /*to check whether the bus is idle or not*/
void i2c_idle(void)
{
	while (!SSPIF);
	SSPIF = 0;
}


/*there is no acknowlegement reqiurement therfore the fucnction is not completed*/
void i2c_ack(void)
{
	if (ACKSTAT)
	{
		/* Do debug print here if required */
	}
}

/*to start th i2c communication */
void i2c_start(void)
{
	SEN = 1;
	i2c_idle();
}

/*to stop th i2c communication */
void i2c_stop(void)
{
	PEN = 1;
	i2c_idle();
}



/*to stop have a 2c communication */
void i2c_rep_start(void)
{
	RSEN = 1;
	i2c_idle();
}


/*inorder to send some data we have to write it to i2c*/
void i2c_write(unsigned char data)
{
	SSPBUF = data;
	i2c_idle();
}


/*fo rreviceing purpose */
void i2c_rx_mode(void)
{
	RCEN = 1;
	i2c_idle();
}

/*12c no ack condition*/
void i2c_no_ack(void)
{
	ACKDT = 1;
	ACKEN = 1;
}

/*to start reading from the buffer*/
unsigned char i2c_read(void)
{
	i2c_rx_mode();
	i2c_no_ack();

	return SSPBUF;
}