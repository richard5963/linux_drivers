#include<linux/module.h>
#include<linux/init.h>
#include<linux/fs.h>

/*information*/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Richard");
MODULE_DESCRIPTION("register device number, implement callback functions");

/*this function is called when device file is open*/
static int driver_open(struct inode *device_file, struct file *instance){
	printk("Device file is opened!\n");
	return 0;
}

/*this function is called when device file is closed*/
static int driver_close(struct inode *device_file, struct file *instance){
	printk("Device file is closed!\n");
	return 0;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = driver_open,
	.release = driver_close
};

#define MY_MAJOR_DN 90

static int __init moduleInit(void){
	int retval;
	printk("Hello Kernel!\n");

	/*register new device*/
	retval = register_chrdev(MY_MAJOR_DN, "my_new_dev", &fops);
	if(retval == 0)
		printk("Device register success, MAJOR=%d, MINOR=%d\n", MY_MAJOR_DN, 0);

	else if(retval > 0)
		printk("Device register success, MAJOR=%d, MINOR=%d\n", retval >> 20, retval & 0xfffff);

	else{
		printk("Failed to register device!\n");
		return -1;
	}
	return 0;
}

static void __exit moduleExit(void){
	/*unregister the device*/
	unregister_chrdev(MY_MAJOR_DN, "my_new_dev");

	printk("Goodbye Kernel!\n");
}

module_init(moduleInit);
module_exit(moduleExit);
