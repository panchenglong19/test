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

#define COND_CONTINUE		false//条件判断继续;
#define COND_STOP			true //条件判断继续;


typedef enum _meta_attr_name_t {
	attr_srcnode,//源节点id;
	attr_srcmoduleid,//源节点模块id;
	/*以下是异步投票参数信息;*/
	attr_dstnodeid,// 投票组播或广播节点;
	attr_reqmsgid,// 获取投票请求消息类型;
	attr_desire_count,// 投票期望应答数;
	attr_timeout,// 投票超时时间;
	attr_timestamp//投票时间戳;
}meta_attr_name_t, *pmeta_attr_name_t;

typedef struct _groupid_t
{
	int gid;
}groupid_t, *pgroup_t;

//定义回调接口;

#define PROCESSOR_STATE_BEFORE_INIT		1
#define PROCESSOR_STATE_AFTER_INIT		2
#define PROCESSOR_STATE_BEFORE_UNINIT	3
#define PROCESSOR_STATE_AFTER_UNINIT	4

typedef void(*processor_state_change_t)(void* processor, int nstate, void* user_data);

/*
zmsg包含msgid、opt_header、data由zbus-api库负责释放;
*/ 
typedef int(*request_handle_t)(void* processor , int srcnode, int msgid, void* recv_msg, int recv_len, void* meta);


/*
异步消息请求应答;
*/

typedef void(*async_finalize_t)(void* processor, int nreason, void* meta, void* async_user_data);


/*
返回true表示停止条件投票;
*/
typedef bool(*stop_cond_t)(int srcnode, int msgid, void* recv_msg, int recv_len, void* meta, void* cond_user_data);


//zbus-api;

//说明：zbus_xxx属于数据总线api、zmbus_xxx属于管理总线api、zibus_xxx属于进程总线api。

//消息类;
/*
释放同步交换、同步投票、异步投票的结果;
*/
int bus_msg_destroy(void* meta);

/*
获取meta属性;
*/
int zmeta_get_int_attr(meta_attr_name_t attr_name, void *meta, int *err);


/*
访问同步或异步条件投票自定义数据，失败user_data返回NULL;
*/
int bus_cond_user_data(void *meta, void** cond_user_data);

int get_tid(void* processor);

//获取meta中包含的消息数量;
int zbus_get_msg_count(void* meta);

/*
访问同步或异步投票第一条结果，失败返回NULL;
*/
int zbus_list_meta_first_msg(void* list_meta, int* srcnode, int* msgid, void** buffer, int* buf_len, void** tmp_meta);

/*
访问同步或异步投票下一条结果，失败返回NULL;
*/
int zbus_list_meta_next_msg(void* list_meta, int* srcnode, int* msgid, void** buffer, int* buf_len,  void** tmp_meta);

//失败返回0，成功返回大于0;
int zbus_get_nodeid(void* module);

int zbus_nodeid(void* processor);
//配置管理类;

/*
	创建进程总线的zmq context;
	创建进程总线，如果创建了进程总线其他处理器都会自动连接到进程总线;
	所有模块的上下文保存在进程的全局hash中 moduleid ==> module_ctx_t;
	返回module;
	如果nodeid是0则从配置文件读取id,否则按nodeid指定的id进行设置;
*/
void* zbus_init(int mod_id, bool has_inproc_bus, int nodeid);

/*
	反初始化进程需要保存的每个模块的hash和其他资源，关闭套接字后recv会返回-1;
*/
void zbus_release(void* module);

/*
启动事件循环，在inproc_bus上发送（消息号是MGNT_PUB_START消息内容是start）start消息返回;
该调用是非阻塞掉用; 
*/
void zbus_start();

/*
终止事件循环，在inproc_bus上发送stop（消息号是MGNT_PUB_STOP消息内容是start）消息，等待所有module上的processor退出(pthread_join)并释放所有资源后返回;
该调用是阻塞调用;
*/
void zbus_stop();

/*终止某个processor;*/
int processor_stop(void* processor);

/*
	创建事件处理句柄，可以注册同步和异步消息;
	返回processor;
*/
void* zbus_create_processor(void* module, void *user_data, char* name, processor_state_change_t change_state);

void* zmbus_create_processor(void* module, void *user_data, char* name, processor_state_change_t change_state);

/*
	创建与消息系统进行消息交互数据句柄，不能注册异步消息;
	返回processor;
*/
void* zbus_create_channel(void* module, char* name);

void* zmbus_create_channel(void* module, char* name);

/*
	获取用户设置的线程全局数据;
*/
void* bus_get_user_data(void* processor);


/*
注册同步应答消息，同步事件只有响应;
*/
int zbus_reg_sync_reply(void* processor, int msgid);

int zmbus_reg_sync_reply(void* processor, int msgid);


/*
	注册异步类型消息，异步事件分为处理异步请求和处理异步应答两种;
	异步应答回调在使用回调函数时指定;
*/

//1注册异步消息应答;

int zbus_reg_async_reply(void* processor, int msgid);

int zmbus_reg_async_reply(void* processor, int msgid);

int zibus_reg_async_reply(void* processor, int msgid);

//2注册异步消息处理函数;
int zbus_reg_request_handle(void* processor, int msgid, request_handle_t ev_handler);

int zmbus_reg_request_handle(void* processor, int msgid, request_handle_t ev_handler);

int zibus_reg_request_handle(void* processor, int msgid, request_handle_t ev_handler);


//数据通信类;
/*
发送数据会自动新建任务号，msg_send、msg_recv都不能为空，msg_recv用zbus_msg_destroy释放msg_recv;
信息交互（内部在发送request并等待接收一个消息）;
属于有同步接收的操作，meta包含有效线程id信息，在发送回复时可以使用;
可以用同步投票实现;
 */
int zbus_msg_invoke(void* processor, int dstnodeid, int msgid, void* send_buf, int send_len, int ntime_out, void** recv_buf, int* recv_len, void** meta);

int zmbus_msg_invoke(void* processor, int dstnodeid, int msgid, void* send_buf, int send_len, int ntime_out, void** recv_buf, int* recv_len, void** meta);

/*
同步投票（内部在发送request并等待接收，poll实现）;
属于有同步接收的操作，meta包含有效线程id信息，在发送回复时可以使用;
*/
int zbus_sync_vote(void* processor, groupid_t* grp, int msgid, void* send_buf, int send_len, int ndesire_count, int ntime_out, void** meta);

int zmbus_sync_vote(void* processor, groupid_t* grp, int msgid, void* send_buf, int send_len, int ndesire_count, int ntime_out, void** meta);

/*
同步条件投票;
*/
int zbus_sync_cond_vote(void* processor, groupid_t* grp, int msgid, void* send_buf, int send_len, int ndesire_count, int ntime_out, stop_cond_t cond_stop, void* cond_user_data, void** meta);

int zmbus_sync_cond_vote(void* processor, groupid_t* grp, int msgid, void* send_buf, int send_len, int ndesire_count, int ntime_out, stop_cond_t cond_stop, void* cond_user_data, void** meta);



/*
发送请求会自动新建任务号,nodeheader仅有msgid,该操作肯定没有应答，否则找不到对应的meta; 
*/
int zbus_msg_send(void* processor, int dstnodeid, int msgid, void* send_buf, int send_len); 

int zmbus_msg_send(void* processor, int dstnodeid, int msgid, void* send_buf, int send_len); 

int zibus_msg_send(void* processor, int msgid, void* send_buf, int send_len);

/*
消息中继meta信息在发送及broker上不能被修改，其实就是send，不过需要判断opt_head中的源节点id不能为0和自己的id;
*/
int zbus_msg_relay(void* processor, int dstnodeid, int msgid, void* send_buf, int send_len, void* meta); 

int zmbus_msg_relay(void* processor, int dstnodeid, int msgid, void* send_buf, int send_len, void* meta); 

/*
发送应答，当meta中包含有效的线程id信息时才将线程id+msgid(8bytes)作为nodeheader,以meta中的nthreadid（可能是无效id 0）、taskno作为该次任务的应答标识，meta必须是只含有一条消息，否则不知道是回复给哪个节点;
*/
int zbus_msg_return(void* processor, int msgid, void* send_buf, int send_len, void* meta);

int zmbus_msg_return(void* processor, int msgid, void* send_buf, int send_len, void* meta);

int zibus_msg_return(void* processor, int msgid, void* send_buf, int send_len, void* meta);

/*
异步消息交换;
*/
int zbus_msg_async_invoke (void* processor,  int dstnodeid, int msgid, void* send_buf, int send_len, int ntime_out, async_finalize_t async_finalize, void* async_user_data);

int zmbus_msg_async_invoke (void* processor,  int dstnodeid, int msgid, void* send_buf, int send_len, int ntime_out, async_finalize_t async_finalize, void* async_user_data);

int zibus_msg_async_invoke (void* processor,  int msgid, void* send_buf, int send_len, int ntime_out, async_finalize_t async_finalize, void* async_user_data);

//int zbus_async_multi_invoke (void* processor,  int* grp, int grp_size, int* msgid, void** send_buf, int* send_len, int ntime_out, stop_cond_t cond_stop, void* cond_user_data, async_finalize_t async_finalize, void* async_user_data);
int zbus_async_multi_invoke (void* processor,  int grp[], int grp_size, int msgid[], void* send_buf[], int send_len[], int ntime_out, stop_cond_t cond_stop, void* cond_user_data, async_finalize_t async_finalize, void* async_user_data);

/*
异步投票返回taskid，在内部设置定时器;
*/
int zbus_async_vote (void* processor, groupid_t* grp, int msgid, void* send_buf, int send_len, int ndesire_count, int ntime_out, async_finalize_t async_finalize, void* async_user_data);

int zmbus_async_vote (void* processor,  groupid_t* grp, int msgid, void* send_buf, int send_len, int ndesire_count, int ntime_out, async_finalize_t async_finalize, void* async_user_data);

/*
异步条件投票;
*/
int zbus_async_cond_vote (void* processor,  groupid_t* grp, int msgid, void* send_buf, int send_len, int ndesire_count, int ntime_out, stop_cond_t cond_stop, void* cond_user_data, async_finalize_t async_finalize, void* async_user_data);

int zmbus_async_cond_vote (void* processor,  groupid_t* grp, int msgid, void* send_buf, int send_len, int ndesire_count, int ntime_out, stop_cond_t cond_stop, void* cond_user_data, async_finalize_t async_finalize, void* async_user_data);

void set_mod_debug_level(void* module, int level);

void get_mod_debug_level(void* module);

void set_bus_debug_level(void* module, int level);

void get_bus_debug_level(void* module);

#endif

