

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "core/fset_lib.h"
#define DNS_PORT 53
#define MAX_BUF_SIZE 1024

#define DNS_HOST 0x01
#define DNS_CNAME 0x05
/*
**DNS报文中查询区域的查询类型
*/
#define A 1 // 查询类型，表示由域名获得IPv4地址

static int dns_addr_flag = 0;

stat_m ngethostbyname(unsigned char *host, int query_type, void *out_addr);
void ChangetoDnsNameFormat(unsigned char *, unsigned char *);

char a_chr_conf_notify[10] = {0};
/*
**DNS报文首部
**这里使用了位域
*/
struct DNS_HEADER
{
    unsigned short id;    // 会话标识
    unsigned char rd : 1; // 表示期望递归
    unsigned char tc : 1; // 表示可截断的
    unsigned char aa : 1; //  表示授权回答
    unsigned char opcode : 4;
    unsigned char qr : 1;    //  查询/响应标志，0为查询，1为响应
    unsigned char rcode : 4; // 应答码
    unsigned char cd : 1;
    unsigned char ad : 1;
    unsigned char z : 1;       // 保留值
    unsigned char ra : 1;      // 表示可用递归
    unsigned short q_count;    // 表示查询问题区域节的数量
    unsigned short ans_count;  // 表示回答区域的数量
    unsigned short auth_count; // 表示授权区域的数量
    unsigned short add_count;  // 表示附加区域的数量
};

/*
**DNS报文中查询问题区域
*/
struct QUESTION
{
    unsigned short qtype;  // 查询类型
    unsigned short qclass; // 查询类
};
typedef struct
{
    unsigned char *name;
    struct QUESTION *ques;
} QUERY;

/*
**DNS报文中回答区域的常量字段
*/
// 编译制导命令
#pragma pack(push, 1) // 保存对齐状态，设定为1字节对齐
struct R_DATA
{
    unsigned short type;     // 表示资源记录的类型
    unsigned short _class;   // 类
    unsigned int ttl;        // 表示资源记录可以缓存的时间
    unsigned short data_len; // 数据长度
};
#pragma pack(pop) // 恢复对齐状态

/*
**DNS报文中回答区域的资源数据字段
*/
struct RES_RECORD
{
    unsigned char *name;     // 资源记录包含的域名
    struct R_DATA *resource; // 资源数据
    unsigned char *rdata;
};
// DNS报文格式
struct dns_header
{
    uint16_t id;
    uint16_t flags;
    uint16_t questions;
    uint16_t answers;
    uint16_t authority;
    uint16_t additional;
};

// DNS查询问题格式
struct dns_question
{
    char *qname;
    uint16_t qtype;
    uint16_t qclass;
};

/**  */
struct sockaddr_in server_addr;
// memset(&server_addr, 0, sizeof(server_addr));

/**  */
bool init_dns = true;
extern struct timeval receiving_timeout;

// // 创建UDP套接字
// int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

// server_addr.sin_addr.s_addr = INADDR_ANY;

char dns_servers[3][16] = {"9.9.9.9", "8.8.8.8", "8.8.4.4"}; // 存放DNS服务器的IP

int dns_server_count = 0;
unsigned char buf[512] = {0};
unsigned char *qname, *reader;
int i, j, stop, mdns_sock__ = 0;

struct sockaddr_in a; // 地址

struct RES_RECORD answers[20], auth[20], addit[20]; // 回答区域、授权区域、附加区域中的资源数据字段
struct sockaddr_in dest;                            // 地址

struct DNS_HEADER *dns = NULL;
struct QUESTION *qinfo = NULL;

/**
 * @brief 测试设备有无网络
 *
 * @return stat_m
 */
stat_m m_static_drive_net_test()
{
    stat_m stat = fail_r;
    mdns_sock__ = socket(AF_INET, SOCK_STREAM, 0);
    if (mdns_sock__ < 0)
    {
        perror("socket");
    }
    else
    {
        if (connect(mdns_sock__, (struct sockaddr *)&dest, sizeof(struct sockaddr_in)) < 0)
        {
            DEBUG_TEST(DB_W, "Not Net !");
        }
        else
        {
            DEBUG_TEST(DB_W, "Net Test OK !");
            stat = succ_r;
        }
    }
    close(mdns_sock__);
    mdns_sock__ = 0;
    return stat;
}
stat_m m_static_drive_set_device_dns(char *dns_adr)
{
    // dns_addr_flag[]
    return 0;
}

struct timeval re_time_out = {10, 0};

stat_m ngethostbyname(unsigned char *host, int query_type, void *out_addr)
{
    stat_m stat = fail_r;
    long *result_adr = (uint32_t *)out_addr;

    if (answers[i].rdata > 0)
    {
        stat = m_static_drive_net_test();
        if (stat == succ_r)
        {
            *result_adr = *(long *)answers[i].rdata;
        }
        return stat; // 没有联网成功
    }
    stat = succ_r;
    mdns_sock__ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // 建立分配UDP套结字
    if (mdns_sock__ <= 0)
    {
        stat = fail_r;
    }
    if (stat == succ_r)
    {                                                                       // dest.sin_addr.s_addr = inet_addr("8.8.8.8"); // DNS服务器IP
        dest.sin_addr.s_addr = inet_addr(dns_servers[(dns_addr_flag) % 2]); // DNS服务器IP
        DEBUG_TEST(DB_I, "Dns : %s", dns_servers[(dns_addr_flag++) % 2]);
        if (init_dns && stat == succ_r)
        {
            dest.sin_family = AF_INET; // IPv4
            dest.sin_port = htons(53); // 53号端口
            // dest.sin_addr.s_addr = INADDR_ANY;
            init_dns = false;

            dns = (struct DNS_HEADER *)&buf;
            /*设置DNS报文首部*/
            dns->id = (unsigned short)htons(getpid()); // id设为进程标识符
            dns->qr = 0;                               // 查询
            dns->opcode = 0;                           // 标准查询
            dns->aa = 0;                               // 不授权回答
            dns->tc = 0;                               // 不可截断
            dns->rd = 1;                               // 期望递归
            dns->ra = 0;                               // 不可用递归
            dns->z = 0;                                // 必须为0
            dns->ad = 0;
            dns->cd = 0;
            dns->rcode = 0;          // 没有差错
            dns->q_count = htons(1); // 1个问题
            dns->ans_count = 0;
            dns->auth_count = 0;
            dns->add_count = 0;

            // qname指向查询问题区域的查询名字段
            qname = (unsigned char *)&buf[sizeof(struct DNS_HEADER)];

            ChangetoDnsNameFormat(qname, host);                                                             // 修改域名格式
            qinfo = (struct QUESTION *)&buf[sizeof(struct DNS_HEADER) + (strlen((const char *)qname) + 1)]; // qinfo指向问题查询区域的查询类型字段

            qinfo->qtype = htons(query_type); // 查询类型为A
            qinfo->qclass = htons(1);         // 查询类为1
        }

        if (setsockopt(mdns_sock__, SOL_SOCKET, SO_RCVTIMEO, &re_time_out,
                       sizeof(re_time_out)) < 0)
        {
            DEBUG_TEST(DB_E, "... failed to set socket receiving timeout");
        }
        i = sizeof dest;
        // 向DNS服务器发送DNS请求报文
    Re_send_to:
        if (sendto(mdns_sock__, (char *)buf, sizeof(struct DNS_HEADER) + (strlen((const char *)qname) + 1) + sizeof(struct QUESTION), 0, (struct sockaddr *)&dest, sizeof(dest)) < 0)
        {
        }

        // 从DNS服务器接受DNS响应报文
        // if()
        DEBUG_TEST(DB_NONE_LINE, "14 .. ");
        if (recvfrom(mdns_sock__, buf, 512, 0, (struct sockaddr *)&dest, (socklen_t *)&i) < 0)
        {
            if (stat == succ_r)
            {
                DEBUG_TEST(DB_W, "DNS Re ...  ");
                stat = fail_r;
                // close(mdns_sock__);
                goto Re_send_to;
            }
            DEBUG_TEST(DB_W, "dns fail   - !  ");
            m_callable_middle_connect_handle_set_code(M_CONNECT_ERROR_CODE_DNS_RESOLVE_FAIL);
            sprintf(a_chr_conf_notify, "%d", M_CONNECT_ERROR_CODE_SERVER_LOGIN_TIMEOUT_FAIL);
            m_ext_network_transmisson_bluetool_tx(a_chr_conf_notify, strlen(a_chr_conf_notify));
            stat = fail_r;
        }
        else
            stat = succ_r;

        dns = (struct DNS_HEADER *)buf;
    }
    if (stat == succ_r)
    {
        // 将reader指向接收报文的回答区域
        reader = &buf[sizeof(struct DNS_HEADER) + (strlen((const char *)qname) + 1) + sizeof(struct QUESTION)];

        DEBUG_TEST(DB_I, "Questions :%d Answer: %d Author %d Additional %d", ntohs(dns->q_count), ntohs(dns->ans_count), ntohs(dns->auth_count), ntohs(dns->add_count));

        /*
        **解析接收报文
        */
        reader = reader + sizeof(short); // short类型长度为32为，相当于域名字段长度，这时reader指向回答区域的查询类型字段
        answers[i].resource = (struct R_DATA *)(reader);
        reader = reader + sizeof(struct R_DATA);   // 指向回答问题区域的资源数据字段
        if (ntohs(answers[i].resource->type) == A) // 判断资源类型是否为IPv4地址
        {
            answers[i].rdata = (unsigned char *)malloc(ntohs(answers[i].resource->data_len)); // 资源数据
            for (j = 0; j < ntohs(answers[i].resource->data_len); j++)
            {
                answers[i].rdata[j] = reader[j];
            }
            answers[i].rdata[ntohs(answers[i].resource->data_len)] = '\0';
            reader = reader + ntohs(answers[i].resource->data_len);
        }

        // 显示查询结果
        if (ntohs(answers[i].resource->type) == A) // 判断查询类型IPv4地址
        {
            long *p;
            p = (long *)answers[i].rdata;
            a.sin_addr.s_addr = *p;
            DEBUG_TEST(DB_W, "one - Net Test OK !");
            *result_adr = *(long *)answers[i].rdata;
            // strcpy(out_addr ,inet_ntoa(a.sin_addr) );
            DEBUG_TEST(DB_I, "Get DNS Host IPv4 :%s\n", inet_ntoa(a.sin_addr));
            stat = succ_r;
        }
    }
    close(mdns_sock__);
    mdns_sock__ = 0;
    return stat;
}

/*
**从www.baidu.com转换到3www5baidu3com
*/
void ChangetoDnsNameFormat(unsigned char *dns, unsigned char *host)
{
    int lock = 0, i;
    strcat((char *)host, ".");

    for (i = 0; i < strlen((char *)host); i++)
    {
        if (host[i] == '.')
        {
            *dns++ = i - lock;
            for (; lock < i; lock++)
            {
                *dns++ = host[lock];
            }
            lock++;
        }
    }
    *dns++ = '\0';
}
/**
 * @brief Get the net and hostname object
 *
 * @param in_hostname 主机名
 * @param out_addr 输出地址
 * @return stat_m
 */
stat_m m_callable_drive_dns_get_net_and_hostname(unsigned char *in_hostname, void *out_addr)
{
    return ngethostbyname(in_hostname, A, out_addr);
}