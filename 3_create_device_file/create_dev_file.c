#include<linux/module.h>
#include<linux/init.h>
#include<linux/fs.h>
#include<linux/cdev.h>

/*information*/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Richard");
MODULE_DESCRIPTION("register device number, implement callback functions");

/*variables for device and device class*/
static dev_t my_device_nr;
static struct class *my_class;
static struct cdev my_device;

#define DRIVER_NAME "dummyDriver"
#define DRIVER_CLASS "myModuleClass"

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

	printk("Hello Kernel!\n");

	/*allocate device number*/
	if(alloc_chrdev_region(&my_device_nr, 0, 1, DRIVER_NAME) < 0){
		printk("Device nr could not be allocated\n");
	}

	printk("Device MAJOR=%d MINOR=%d!\n", my_device_nr >> 20, my_device_nr & 0xfffff);

	/*create device class*/
	if((my_class = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL){
		printk("Device class could not be created!\n");
		goto classError;
	}

	/*create device file*/
	if(device_create(my_class, NULL, my_device_nr, NULL, DRIVER_NAME) == NULL){
		printk("Cannot create device file\n");
		goto fileError;
	}

	/*initialise device file*/
	cdev_init(&my_device, &fops);

	/*registering device to kernel*/
	if(cdev_add(&my_device, my_device_nr, 1) == 1){
		printk("register failed!\n");
		goto addError;
	}
	return 0;

addError:
	device_destroy(my_class, my_device_nr);
fileError:
	class_destroy(my_class);
classError:
	unregister_chrdev(my_device_nr, DRIVER_NAME);
	return -1;
}

static void __exit moduleExit(void){

	/*unregister the device*/
	cdev_del(&my_device);
	device_destroy(my_class, my_device_nr);
	class_destroy(my_class);
	unregister_chrdev(my_device_nr, DRIVER_NAME);

	printk("Goodbye Kernel!\n");
}

module_init(moduleInit);
module_exit(moduleExit);
