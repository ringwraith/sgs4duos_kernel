/*
 * Definitions for USB serial mobile broadband cards
 */

#ifndef __LINUX_USB_USB_WWAN
#define __LINUX_USB_USB_WWAN

extern void usb_wwan_dtr_rts(struct usb_serial_port *port, int on);
extern int usb_wwan_open(struct tty_struct *tty, struct usb_serial_port *port);
extern void usb_wwan_close(struct usb_serial_port *port);
extern int usb_wwan_startup(struct usb_serial *serial);
extern void usb_wwan_disconnect(struct usb_serial *serial);
extern void usb_wwan_release(struct usb_serial *serial);
extern int usb_wwan_write_room(struct tty_struct *tty);
extern void usb_wwan_set_termios(struct tty_struct *tty,
				 struct usb_serial_port *port,
				 struct ktermios *old);
extern int usb_wwan_tiocmget(struct tty_struct *tty);
extern int usb_wwan_tiocmset(struct tty_struct *tty,
			     unsigned int set, unsigned int clear);
extern int usb_wwan_ioctl(struct tty_struct *tty,
			  unsigned int cmd, unsigned long arg);
extern int usb_wwan_send_setup(struct usb_serial_port *port);
extern int usb_wwan_write(struct tty_struct *tty, struct usb_serial_port *port,
			  const unsigned char *buf, int count);
extern int usb_wwan_chars_in_buffer(struct tty_struct *tty);
#ifdef CONFIG_PM
extern int usb_wwan_suspend(struct usb_serial *serial, pm_message_t message);
extern int usb_wwan_resume(struct usb_serial *serial);
#endif

/* per port private data */

#ifdef CONFIG_MDM_HSIC_PM
#define N_IN_URB 1
#else
#define N_IN_URB 5
#endif
#define N_OUT_URB 5
#define IN_BUFLEN 65536
#define OUT_BUFLEN 65536

struct usb_wwan_intf_private {
	spinlock_t susp_lock;
	unsigned int suspended:1;
	int in_flight;
	int (*send_setup) (struct usb_serial_port *port);
#ifdef CONFIG_MDM_HSIC_PM
	int buf_reuse;
#endif
#if defined(CONFIG_MACH_JA_KOR_LGT)
	atomic_t sync_read_cnt;
	struct workqueue_struct *wq;
	struct delayed_work sync_check_work;
	struct tty_struct *temp_tty;
#endif

	void *private;
};

struct usb_wwan_port_private {
	/* Input endpoints and buffer for this port */
	struct urb *in_urbs[N_IN_URB];
	u8 *in_buffer[N_IN_URB];
	/* Output endpoints and buffer for this port */
	struct urb *out_urbs[N_OUT_URB];
	u8 *out_buffer[N_OUT_URB];
	unsigned long out_busy;	/* Bit vector of URBs in use */
	int opened;
	struct usb_anchor delayed;

	/* Settings for the port */
	int rts_state;		/* Handshaking pins (outputs) */
	int dtr_state;
	int cts_state;		/* Handshaking pins (inputs) */
	int dsr_state;
	int dcd_state;
	int ri_state;

	unsigned long tx_start_time[N_OUT_URB];
};

#endif /* __LINUX_USB_USB_WWAN */
