zbus_msg_invoke.processor:1,2,3
S_H_



//////////////////zbus////////////////////////////////////

#include <stdbool.h>
///////////////zbus-api///////////////////////////////


#define CURRRENT_VERSION	"01"


#define ASYNC_REASON_TIME_OUT	0
#define ASYNC_REASON_FINISH		1

#define	NODE_LOCAL_LOOP			0x0000
#define NODE_BROAD_CAST			0xffff
#define NODE_THIS_BROKER		0xfffe

#define	ZBUS_RETURN_SUCCESS				0
#define ZBUS_RETURN_FAIL				-1
#define ZBUS_RETURN_SYNC_TIME_OUT		-10

#define COND_CONTINUE		false//�����жϼ���;
#define COND_STOP			true //�����жϼ���;


typedef enum _meta_attr_name_t {
	attr_srcnode,//Դ�ڵ�id;
	attr_srcmoduleid,//Դ�ڵ�ģ��id;
	/*�������첽ͶƱ������Ϣ;*/
	attr_dstnodeid,// ͶƱ�鲥��㲥�ڵ�;
	attr_reqmsgid,// ��ȡͶƱ������Ϣ����;
	attr_desire_count,// ͶƱ����Ӧ����;
	attr_timeout,// ͶƱ��ʱʱ��;
	attr_timestamp//ͶƱʱ���;
}meta_attr_name_t, *pmeta_attr_name_t;

typedef struct _groupid_t
{
	int gid;
}groupid_t, *pgroup_t;

//����ص��ӿ�;

#define PROCESSOR_STATE_BEFORE_INIT		1
#define PROCESSOR_STATE_AFTER_INIT		2
#define PROCESSOR_STATE_BEFORE_UNINIT	3
#define PROCESSOR_STATE_AFTER_UNINIT	4

typedef void(*processor_state_change_t)(void* processor, int nstate, void* user_data);

/*
zmsg����msgid��opt_header��data��zbus-api�⸺���ͷ�;
*/ 
typedef int(*request_handle_t)(void* processor , int srcnode, int msgid, void* recv_msg, int recv_len, void* meta);


/*
�첽��Ϣ����Ӧ��;
*/

typedef void(*async_finalize_t)(void* processor, int nreason, void* meta, void* async_user_data);


/*
����true��ʾֹͣ����ͶƱ;
*/
typedef bool(*stop_cond_t)(int srcnode, int msgid, void* recv_msg, int recv_len, void* meta, void* cond_user_data);


//zbus-api;

//˵����zbus_xxx������������api��zmbus_xxx���ڹ�������api��zibus_xxx���ڽ�������api��

//��Ϣ��;
/*
�ͷ�ͬ��������ͬ��ͶƱ���첽ͶƱ�Ľ��;
*/
int bus_msg_destroy(void* meta);

/*
��ȡmeta����;
*/
int zmeta_get_int_attr(meta_attr_name_t attr_name, void *meta, int *err);


/*
����ͬ�����첽����ͶƱ�Զ������ݣ�ʧ��user_data����NULL;
*/
int bus_cond_user_data(void *meta, void** cond_user_data);

int get_tid(void* processor);

//��ȡmeta�а�������Ϣ����;
int zbus_get_msg_count(void* meta);

/*
����ͬ�����첽ͶƱ��һ�������ʧ�ܷ���NULL;
*/
int zbus_list_meta_first_msg(void* list_meta, int* srcnode, int* msgid, void** buffer, int* buf_len, void** tmp_meta);

/*
����ͬ�����첽ͶƱ��һ�������ʧ�ܷ���NULL;
*/
int zbus_list_meta_next_msg(void* list_meta, int* srcnode, int* msgid, void** buffer, int* buf_len,  void** tmp_meta);

//ʧ�ܷ���0���ɹ����ش���0;
int zbus_get_nodeid(void* module);

int zbus_nodeid(void* processor);
//���ù�����;

/*
	�����������ߵ�zmq context;
	�����������ߣ���������˽����������������������Զ����ӵ���������;
	����ģ��������ı����ڽ��̵�ȫ��hash�� moduleid ==> module_ctx_t;
	����module;
	���nodeid��0��������ļ���ȡid,����nodeidָ����id��������;
*/
void* zbus_init(int mod_id, bool has_inproc_bus, int nodeid);

/*
	����ʼ��������Ҫ�����ÿ��ģ���hash��������Դ���ر��׽��ֺ�recv�᷵��-1;
*/
void zbus_release(void* module);

/*
�����¼�ѭ������inproc_bus�Ϸ��ͣ���Ϣ����MGNT_PUB_START��Ϣ������start��start��Ϣ����;
�õ����Ƿ���������; 
*/
void zbus_start();

/*
��ֹ�¼�ѭ������inproc_bus�Ϸ���stop����Ϣ����MGNT_PUB_STOP��Ϣ������start����Ϣ���ȴ�����module�ϵ�processor�˳�(pthread_join)���ͷ�������Դ�󷵻�;
�õ�������������;
*/
void zbus_stop();

/*��ֹĳ��processor;*/
int processor_stop(void* processor);

/*
	�����¼��������������ע��ͬ�����첽��Ϣ;
	����processor;
*/
void* zbus_create_processor(void* module, void *user_data, char* name, processor_state_change_t change_state);

void* zmbus_create_processor(void* module, void *user_data, char* name, processor_state_change_t change_state);

/*
	��������Ϣϵͳ������Ϣ�������ݾ��������ע���첽��Ϣ;
	����processor;
*/
void* zbus_create_channel(void* module, char* name);

void* zmbus_create_channel(void* module, char* name);

/*
	��ȡ�û����õ��߳�ȫ������;
*/
void* bus_get_user_data(void* processor);


/*
ע��ͬ��Ӧ����Ϣ��ͬ���¼�ֻ����Ӧ;
*/
int zbus_reg_sync_reply(void* processor, int msgid);

int zmbus_reg_sync_reply(void* processor, int msgid);


/*
	ע���첽������Ϣ���첽�¼���Ϊ�����첽����ʹ����첽Ӧ������;
	�첽Ӧ��ص���ʹ�ûص�����ʱָ��;
*/

//1ע���첽��ϢӦ��;

int zbus_reg_async_reply(void* processor, int msgid);

int zmbus_reg_async_reply(void* processor, int msgid);

int zibus_reg_async_reply(void* processor, int msgid);

//2ע���첽��Ϣ��������;
int zbus_reg_request_handle(void* processor, int msgid, request_handle_t ev_handler);

int zmbus_reg_request_handle(void* processor, int msgid, request_handle_t ev_handler);

int zibus_reg_request_handle(void* processor, int msgid, request_handle_t ev_handler);


//����ͨ����;
/*
�������ݻ��Զ��½�����ţ�msg_send��msg_recv������Ϊ�գ�msg_recv��zbus_msg_destroy�ͷ�msg_recv;
��Ϣ�������ڲ��ڷ���request���ȴ�����һ����Ϣ��;
������ͬ�����յĲ�����meta������Ч�߳�id��Ϣ���ڷ��ͻظ�ʱ����ʹ��;
������ͬ��ͶƱʵ��;
 */
int zbus_msg_invoke(void* processor, int dstnodeid, int msgid, void* send_buf, int send_len, int ntime_out, void** recv_buf, int* recv_len, void** meta);

int zmbus_msg_invoke(void* processor, int dstnodeid, int msgid, void* send_buf, int send_len, int ntime_out, void** recv_buf, int* recv_len, void** meta);

/*
ͬ��ͶƱ���ڲ��ڷ���request���ȴ����գ�pollʵ�֣�;
������ͬ�����յĲ�����meta������Ч�߳�id��Ϣ���ڷ��ͻظ�ʱ����ʹ��;
*/
int zbus_sync_vote(void* processor, groupid_t* grp, int msgid, void* send_buf, int send_len, int ndesire_count, int ntime_out, void** meta);

int zmbus_sync_vote(void* processor, groupid_t* grp, int msgid, void* send_buf, int send_len, int ndesire_count, int ntime_out, void** meta);

/*
ͬ������ͶƱ;
*/
int zbus_sync_cond_vote(void* processor, groupid_t* grp, int msgid, void* send_buf, int send_len, int ndesire_count, int ntime_out, stop_cond_t cond_stop, void* cond_user_data, void** meta);

int zmbus_sync_cond_vote(void* processor, groupid_t* grp, int msgid, void* send_buf, int send_len, int ndesire_count, int ntime_out, stop_cond_t cond_stop, void* cond_user_data, void** meta);



/*
����������Զ��½������,nodeheader����msgid,�ò����϶�û��Ӧ�𣬷����Ҳ�����Ӧ��meta; 
*/
int zbus_msg_send(void* processor, int dstnodeid, int msgid, void* send_buf, int send_len); 

int zmbus_msg_send(void* processor, int dstnodeid, int msgid, void* send_buf, int send_len); 

int zibus_msg_send(void* processor, int msgid, void* send_buf, int send_len);

/*
��Ϣ�м�meta��Ϣ�ڷ��ͼ�broker�ϲ��ܱ��޸ģ���ʵ����send��������Ҫ�ж�opt_head�е�Դ�ڵ�id����Ϊ0���Լ���id;
*/
int zbus_msg_relay(void* processor, int dstnodeid, int msgid, void* send_buf, int send_len, void* meta); 

int zmbus_msg_relay(void* processor, int dstnodeid, int msgid, void* send_buf, int send_len, void* meta); 

/*
����Ӧ�𣬵�meta�а�����Ч���߳�id��Ϣʱ�Ž��߳�id+msgid(8bytes)��Ϊnodeheader,��meta�е�nthreadid����������Чid 0����taskno��Ϊ�ô������Ӧ���ʶ��meta������ֻ����һ����Ϣ������֪���ǻظ����ĸ��ڵ�;
*/
int zbus_msg_return(void* processor, int msgid, void* send_buf, int send_len, void* meta);

int zmbus_msg_return(void* processor, int msgid, void* send_buf, int send_len, void* meta);

int zibus_msg_return(void* processor, int msgid, void* send_buf, int send_len, void* meta);

/*
�첽��Ϣ����;
*/
int zbus_msg_async_invoke (void* processor,  int dstnodeid, int msgid, void* send_buf, int send_len, int ntime_out, async_finalize_t async_finalize, void* async_user_data);

int zmbus_msg_async_invoke (void* processor,  int dstnodeid, int msgid, void* send_buf, int send_len, int ntime_out, async_finalize_t async_finalize, void* async_user_data);

int zibus_msg_async_invoke (void* processor,  int msgid, void* send_buf, int send_len, int ntime_out, async_finalize_t async_finalize, void* async_user_data);

//int zbus_async_multi_invoke (void* processor,  int* grp, int grp_size, int* msgid, void** send_buf, int* send_len, int ntime_out, stop_cond_t cond_stop, void* cond_user_data, async_finalize_t async_finalize, void* async_user_data);
int zbus_async_multi_invoke (void* processor,  int grp[], int grp_size, int msgid[], void* send_buf[], int send_len[], int ntime_out, stop_cond_t cond_stop, void* cond_user_data, async_finalize_t async_finalize, void* async_user_data);

/*
�첽ͶƱ����taskid�����ڲ����ö�ʱ��;
*/
int zbus_async_vote (void* processor, groupid_t* grp, int msgid, void* send_buf, int send_len, int ndesire_count, int ntime_out, async_finalize_t async_finalize, void* async_user_data);

int zmbus_async_vote (void* processor,  groupid_t* grp, int msgid, void* send_buf, int send_len, int ndesire_count, int ntime_out, async_finalize_t async_finalize, void* async_user_data);

/*
�첽����ͶƱ;
*/
int zbus_async_cond_vote (void* processor,  groupid_t* grp, int msgid, void* send_buf, int send_len, int ndesire_count, int ntime_out, stop_cond_t cond_stop, void* cond_user_data, async_finalize_t async_finalize, void* async_user_data);

int zmbus_async_cond_vote (void* processor,  groupid_t* grp, int msgid, void* send_buf, int send_len, int ndesire_count, int ntime_out, stop_cond_t cond_stop, void* cond_user_data, async_finalize_t async_finalize, void* async_user_data);

void set_mod_debug_level(void* module, int level);

void get_mod_debug_level(void* module);

void set_bus_debug_level(void* module, int level);

void get_bus_debug_level(void* module);

#endif
