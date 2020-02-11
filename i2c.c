#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

#include <linux/i2c.h>

#define MODNAME "i2cdev"
#define i2caddr 0x1d


MODULE_LICENSE("GPL");

static struct i2c_board_info devs[] = {
	{
		I2C_BOARD_INFO("i2c", 0x57),
	},
};

struct i2c_client *new_client =  NULL;
//new_client = (struct i2c_client)kmalloc(sizeof(struct i2c_client), GFP_KERNEL);
//memset(new_client, 0x00, sizeof(*new_client));

static int tiny_detect(struct i2c_client *new_client, struct i2c_board_info *devs) {
	printk("Enter tiny_detect \n");
	return 0;
}

static u32 tiny_func(struct i2c_adapter *adap) {
	printk("enter tiny_func \n");
	return 	I2C_FUNC_SMBUS_QUICK | 
		I2C_FUNC_SMBUS_BYTE |
		I2C_FUNC_SMBUS_BYTE_DATA | 
		I2C_FUNC_SMBUS_WORD_DATA | 
		I2C_FUNC_SMBUS_BLOCK_DATA;
}


static int tiny_access(struct i2c_adapter *adap, u16 addr, unsigned short flags, 
		char read_write, u8 command, int size, union i2c_smbus_data *data) {
	printk("enter tiny_access \n");
	dev_info(&adap->dev, "It is %.4x, the addr\n", addr);
	dev_info(&adap->dev, "It is %.4x, the flag\n", flags);
	dev_info(&adap->dev, "It is %d, the command\n", command);
	dev_info(&adap->dev, "It is %d, the size\n", size);

	switch(size) {
		case I2C_SMBUS_QUICK:
			dev_info(&adap->dev, "size = I2C SMBUS QUICK \n");
			break;
		case I2C_SMBUS_PROC_CALL:
			dev_info(&adap->dev, "size = I2C SMBUS PROC CALL \n");
			break;
		case I2C_SMBUS_BYTE:
			dev_info(&adap->dev, "size = I2C SMBUS BYTE \n");
			break;
		case I2C_SMBUS_BYTE_DATA:
			dev_info(&adap->dev, "size = I2C SMBUS BYTE DATA \n");
			if (read_write == I2C_SMBUS_WRITE) {
				dev_info(&adap->dev, "Command to write %.4x word and %.4x byte \n", data->word, data->byte);
			}
			else {
				dev_info(&adap->dev, "Command to read %.4x word and %.4x byte \n", data->word, data->byte);
			}
			break;
		case I2C_SMBUS_WORD_DATA:
			dev_info(&adap->dev, "size = I2C SMBUS WORD DATA \n");
			break;
		case I2C_SMBUS_BLOCK_DATA:
			dev_info(&adap->dev, "size = I2C SMBUS BLOCK DATA \n");
			break;
		default:
			dev_info(&adap->dev, "size = UNKNOWN OPERATION \n");
			break;

	}

	return 0;
}


static struct i2c_driver chip_driver = {
	.driver = {
		.owner = THIS_MODULE,
		.name = "tiny_chip",
		//.flags = I2c_DF_NOTIFY,
		//.attach_adapter = chip_attach_adapter,
		//.attach_client = chip_attach_client,
	},
	//.probe = probe_i2c,
	.detect = tiny_detect,
};


static struct i2c_algorithm chip_algorithm = {
	//.name = "tiny algorithm",
	//.id = I2C_ALGO_SMBUS,
	.smbus_xfer = tiny_access,
	.functionality = tiny_func,
};

static struct i2c_adapter chip_adapter = {
	.owner = THIS_MODULE,
	.class = 0,
	.algo = &chip_algorithm,
	.name = "tiny_adapter",
};

static int __init myinit(void) {
	printk("enter i2c\n");
	int retval = i2c_add_driver(&chip_driver);
	retval = i2c_add_adapter(&chip_adapter);
	return 0;
}

static void __exit myexit(void) {
	i2c_del_driver(&chip_driver);
	i2c_del_adapter(&chip_adapter);
	printk("exit i2c\n");
}

module_init(myinit);
module_exit(myexit);
