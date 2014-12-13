LinuxDriverSample
=================

Samples for Linux driver

(1) The skeleton of Linux device driver: task.c <br />
(2) The simple character driver (support sysfs) : charWithSysfs.c <br />
    # insmod charWithSysfs.ko <br />
    # ls -l /dev/mydev0 <br />
(3) Create kernel thread to detect status: kthread.c <br />

