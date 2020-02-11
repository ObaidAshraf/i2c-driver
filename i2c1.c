#include <linux/i2c.h>
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

#define MODNAME "i2cdev"
#define i2caddr 0x1d

static struct i2c_driver chip_driver = {
	.driver = {
		.owner = THIS_MODULE,
		.name = "tiny_chip",
		//.flags = I2c_DF_NOTIFY,
		//.attach_adapter = chip_attach_adapter,
		//.attach_client = chip_attach_client,
	},
	//.probe = probe_i2c,
};



static int __init myinit(void) {
	printk("enter i2c\n");
	int retval = i2c_add_driver(&chip_driver);
	printk("Retval %d\n", retval);
	return 0;
}

static void __exit myexit(void) {
	i2c_del_driver(&chip_driver);
	printk("exit i2c\n");
}

module_init(myinit);
module_exit(myexit);
