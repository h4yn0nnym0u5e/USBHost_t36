#include <stdint.h>


typedef struct Device_struct    Device_t;
typedef struct Pipe_struct      Pipe_t;
typedef struct Transfer_struct  Transfer_t;

typedef union {
 struct {
  union {
   struct {
        uint8_t bmRequestType;
        uint8_t bRequest;
   };
        uint16_t wRequestAndType;
  };
        uint16_t wValue;
        uint16_t wIndex;
        uint16_t wLength;
 };
 struct {
        uint32_t word1;
        uint32_t word2;
 };
} setup_t;


struct Device_struct {
	Pipe_t   *control_pipe;
	Device_t *next;
	setup_t  setup;
	uint8_t  speed; // 0=12, 1=1.5, 2=480 Mbit/sec
	uint8_t  address;
	uint8_t  hub_address;
	uint8_t  hub_port;
};

struct Pipe_struct {
	// Queue Head (QH), EHCI page 46-50
	struct {  // must be aligned to 32 byte boundary
		uint32_t horizontal_link;
		uint32_t capabilities[2];
		uint32_t current;
		uint32_t next;
		uint32_t alt_next;
		uint32_t token;
		uint32_t buffer[5];
	} qh;
	Device_t *device;
	uint8_t  type; // 0=control, 1=isochronous, 2=bulk, 3=interrupt
	uint8_t  direction; // 0=out, 1=in
	//uint8_t  endpoint;  // 0 to 15
	//uint8_t  data01;    // next packet DATA0 or DATA1
	uint8_t  unusedbyte[2];
	uint32_t unused[2];
};


struct Transfer_struct {
	// Queue Element Transfer Descriptor (qTD), EHCI pg 40-45
	struct {  // must be aligned to 32 byte boundary
		uint32_t next;
		uint32_t alt_next;
		uint32_t token;
		uint32_t buffer[5];
	} qtd;
	Pipe_t   *pipe;
	void     *callback;
	void     *callback_arg;
	uint32_t unused[5];
};

#if 0
#define EHCI_QH_CAPABILITIES1( \
		nak_count_reload, \
		control_endpoint_flag, \
		max_packet_length, \
		head_of_list, \
		data_toggle_control, \
		speed, \
		endpoint_number, \
		inactivate, \
		address) \
	( ((nak_count_reload) << 28) | \
	((control_endpoint_flag) << 27) | \
	((max_packet_length) << 16) | \
	((head_of_list) << 15) | \
	((data_toggle_control) << 14) | \
	((speed) << 12) | \
	((endpoint_number) << 8) | \
	((inactivate) << 7) | \
	((address) << 0) )

#define EHCI_QH_CAPABILITIES2( \
		high_bw_mult, \
		hub_port_number, \
		hub_address, \
		split_completion_mask, \
		interrupt_schedule_mask) \
	( ((high_bw_mult) << 30) | \
	((hub_port_number) << 23) | \
	((hub_address) << 16) | \
	((split_completion_mask) << 8) | \
	((interrupt_schedule_mask) << 0) )
#endif

Device_t * allocate_Device(void);
void free_Device(Device_t *q);
Pipe_t * allocate_Pipe(void);
void free_Pipe(Pipe_t *q);
Transfer_t * allocate_Transfer(void);
void free_Transfer(Transfer_t *q);
