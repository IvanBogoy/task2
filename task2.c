/* **************** LDD:1.0 s_26/lab1_transmit_simple.c **************** */
/*
 * The code herein is: Copyright Jerry Cooperstein, 2009
 *
 * This Copyright is retained for the purpose of protecting free
 * redistribution of source.
 *
 *     URL:    http://www.coopj.com
 *     email:  coop@coopj.com
 *
 * Building a Transmitting Network Driver (simple solution)
 *
 * Extend your stub network device driver to include a transmission
 * function, which means supplying a method for
 * dev->hard_start_xmit().
 *
 * While you are at it, you may want to add other entry points to see
 * how you may exercise them.
 *
 * Once again, you should be able to exercise it with:
 *
 *   insmod lab_network.ko
 *   ifconfig mynet0 up 192.168.3.197
 *   ping -I mynet0 localhost
 *       or
 *   ping -bI mynet0 192.168.3
 *
 * Make sure your chosen address is not being used by anything else.
 *
 @*/
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/init.h>

static struct net_device *dev;

static int my_hard_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
	dev->trans_start = jiffies;
	printk(KERN_INFO " Test lenght = %x\n", skb->len);

	dev_kfree_skb(skb);
	return 0;
}

static int my_open(struct net_device *dev)
{
	printk(KERN_INFO "Hit: my_open(%s)\n", dev->name);

	/* start up the transmission queue */
	netif_start_queue(dev);
	return 0;
}

static int my_close(struct net_device *dev)
{
	printk(KERN_INFO "Hit: my_close(%s)\n", dev->name);
	/* shutdown the transmission queue */

	netif_stop_queue(dev);
	return 0;
}

static struct net_device_ops ndo = {
   .ndo_open = my_open,
   .ndo_stop = my_close,
   .ndo_start_xmit = my_hard_start_xmit,
};

static void my_setup(struct net_device *dev)
{
	int j;
	printk(KERN_INFO "my_setup(%s)\n", dev->name);

	/* Fill in the MAC address with a phoney */

	for (j = 0; j < ETH_ALEN; ++j) {
		dev->dev_addr[j] = (char)j;
	}
	ether_setup(dev);
	dev->netdev_ops = &ndo;
	dev->flags |= IFF_NOARP;
}

static int __init my_init(void)
{
	printk(KERN_INFO "Loading transmitting network module:....");
	dev = alloc_netdev(0, "test%d", NET_NAME_UNKNOWN, my_setup);
	if (register_netdev(dev)) {
		printk(KERN_INFO " Failed to register\n");
		free_netdev(dev);
		return -1;
	}
	printk(KERN_INFO "Succeeded in loading %s!\n\n", dev_name(&dev->dev));
	return 0;
}

static void __exit my_exit(void)
{
	printk(KERN_INFO "Unloading transmitting network module\n\n");
	unregister_netdev(dev);
	free_netdev(dev);
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("Bill Shubert");
MODULE_AUTHOR("Jerry Cooperstein");
MODULE_DESCRIPTION("LDD:1.0 s_26/lab1_transmit_simple.c");
MODULE_LICENSE("GPL v2");
