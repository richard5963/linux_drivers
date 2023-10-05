#include<linux/module.h>
#include<linux/init.h>

/*information*/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Richard");
MODULE_DESCRIPTION("linux hello module");


static int __init moduleInit(void){
	printk("Hello Kernel!\n");
	return 0;
}

static void __exit moduleExit(void){
	printk("Goodbye Kernel!\n");
}

module_init(moduleInit);
module_exit(moduleExit);
