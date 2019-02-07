#include <linux/module.h>
#include <linux/tty.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <asm/uacess.h>

#define MODULE_VERSION "1.0"
#define MODULE_NAME "myproc"

#define FOO_FILE "foo"
#define JIFFIE_FILE "jiffies"

#define FOOBAR_LEN 8

struct fb_data_t
{
	char name[FOOBAR_LEN+1];
	char value[FOOBAR_LEN+1];
};
struct fb_data_t foo_data;

static struct proc_dir_entry *exmaple_dir, *foo_file, *jiffie_file;

static int proc_read_jiffies(char *page, char **start, off_t off, int count, nt *eof, void *data)
{
	int len;

	MOD_INC_USE_COUNT;

	len = sptrintf(page, "jiffies = %ld\n", jiffies);

	printk("<1> read jiffies = %ld\n", jiffies);
	MOD_DEC_USE_COUNT;

	return len;
}
