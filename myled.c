#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/delay.h>

MODULE_AUTHOR("Ryuichi Ueda, Seiichiro Sugiyama");
MODULE_DESCRIPTION("driver for LED control");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.0.3");

static dev_t dev;
static struct cdev cdv;
static struct class *cls = NULL;
static volatile u32 *gpio_base = NULL;

void ton(void){
	gpio_base[10] = 1 << 25;
	gpio_base[7] = 1 << 24;
	mdelay(200);
	gpio_base[10] = 1 << 24;
	mdelay(100);
}

void tsu(void){
	gpio_base[10] = 1 << 24;
	gpio_base[7] = 1 << 25;
	mdelay(600);
	gpio_base[10] = 1 << 25;
	mdelay(100);
}

void end(void){
	mdelay(500);
}

static ssize_t led_write(struct file* filp, const char* buf, size_t count, loff_t* pos)
{
	char c;
	if(copy_from_user(&c, buf, sizeof(char)))
		return -EFAULT;

	printk(KERN_INFO "receive %c\n", c);
	
	switch(c){
		case '0':
			ton();
			break;
		case '1':
			tsu();
			break;
		case 'a':
			ton();
			tsu();
			end();
			break;
		case 'b':
			tsu();
			ton();
			ton();
			ton();
			end();
			break;
		case 'c':
			tsu();
			ton();
			tsu();
			ton();
			end();
			break;
		case 'd':
			tsu();
			ton();
			ton();
			end();
			break;
		case 'e':
			ton();
			end();
			break;
		case 'f':
			ton();
			ton();
			tsu();
			ton();
			end();
			break;
		case 'g':
			tsu();
			tsu();
			ton();
			end();
			break;
		case 'h':
			ton();
			ton();
			ton();
			ton();
			end();
			break;
		case 'i':
			ton();
			ton();
			end();
			break;
		case 'j':
			ton();
			tsu();
			tsu();
			tsu();
			end();
			break;
		case 'k':
			tsu();
			ton();
			tsu();
			end();
			break;
		case 'l':
			ton();
			tsu();
			ton();
			ton();
			end();
			break;
		case 'm':
			tsu();
			tsu();
			end();
			break;
		case 'n':
			tsu();
			ton();
			end();
			break;
		case 'o':
			tsu();
			tsu();
			tsu();
			end();
			break;
		case 'p':
			ton();
			tsu();
			tsu();
			ton();
			end();
			break;
		case 'q':
			tsu();
			tsu();
			ton();
			tsu();
			end();
			break;
		case 'r':
			ton();
			tsu();
			ton();
			end();
			break;
		case 's':
			ton();
			ton();
			ton();
			end();
			break;
		case 't':
			tsu();
			end();
			break;
		case 'u':
			ton();
			ton();
			tsu();
			end();
			break;
		case 'v':
			ton();
			ton();
			ton();
			tsu();
			end();
			break;
		case 'w':
			ton();
			tsu();
			tsu();
			end();
			break;
		case 'x':
			tsu();
			ton();
			ton();
			tsu();
			break;
		case 'y':
			tsu();
			ton();
			tsu();
			tsu();
			end();
			break;
		case 'z':
			tsu();
			tsu();
			ton();
			ton();
			end();
			break;
	}

	return 1;
}

static ssize_t sushi_read(struct file* filp, char* buf, size_t count, loff_t* pos)
{
	int size = 0;
	char sushi[] = {'S', 'U', 'S', 'H', 'I'};
	if(copy_to_user(buf+size, (const char *)sushi, sizeof(sushi))){
		printk(KERN_ERR "sushi:copy_to_user failed.\n");
		return -EFAULT;
	}

	size += sizeof(sushi);
	return size;
}

static struct file_operations led_fops = {
	.owner = THIS_MODULE,
	.write = led_write,
	.read = sushi_read
};

static int __init init_mod(void)
{
	int retval;
	retval = alloc_chrdev_region(&dev, 0, 1, "myled");
	if(retval < 0){
		printk(KERN_ERR "Alloc_chrdev_region failed.\n");
		return retval;
	}
	printk(KERN_INFO "%s is loaded. major:%d\n", __FILE__, MAJOR(dev));

	cdev_init(&cdv, &led_fops);
	retval = cdev_add(&cdv, dev, 1);
	if(retval < 0){
		printk(KERN_ERR "cdev_add failed. major:%d, minor:%d\n", MAJOR(dev), MINOR(dev));
		return retval;
	}

	cls = class_create(THIS_MODULE, "myled");
	if(IS_ERR(cls)){
		printk(KERN_ERR "class_create failed.\n");
		return PTR_ERR(cls);
	}

	device_create(cls, NULL, dev, NULL, "myled%d", MINOR(dev));

	gpio_base = ioremap_nocache(0x3f200000, 0xA0);

	const u32 led_A = 24;
	const u32 led_B = 25;
	const u32 index = led_A/10;
	const u32 shift_A = (led_A%10)*3;
	const u32 shift_B = (led_B%10)*3;
	const u32 mask = ~((0x7 << shift_A) + (0x7 << shift_B));
	gpio_base[index] = (gpio_base[index] & mask) | ((0x1 << shift_A) + (0x1 << shift_B));
		
	return 0;
}

static void __exit cleanup_mod(void)
{
	cdev_del(&cdv);
	device_destroy(cls, dev);
	class_destroy(cls);
	unregister_chrdev_region(dev, 1);
	printk(KERN_INFO "%s is unloded. major:%d\n", __FILE__, MAJOR(dev));
}

module_init(init_mod);
module_exit(cleanup_mod);
