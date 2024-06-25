// // /*
// //  * @Author: your name
// //  * @Date: 2019-10-30 10:22:47
// //  * @LastEditTime: 2019-10-30 10:56:13
// //  * @LastEditors: Please set LastEditors
// //  * @Description: In User Settings Edit
// //  * @FilePath: \esp-idf\mine\ESP32_DNS_Server\components\src\dns_server.c
// //  */
// // #include <string.h>
// // #include <sys/socket.h>
// // #include "esp_system.h"
// // #include "esp_log.h"
// // #include "tcpip_adapter.h"
// // #include "lwip/opt.h"
// // #include "lwip/err.h"
// // #include "lwip/pbuf.h"
// // #include "lwip/netif.h"
// // #include "lwip/ip_addr.h"
// // #include "lwip/ip.h"
// // #include "lwip/udp.h"
// // #include "lwip/def.h"
// // #include "lwip/memp.h"
// // #include "lwip/ip4_addr.h"
// // #include "lwip/ip6_addr.h"
// // #include "lwip/api.h"
// // // #include "dns_server.h"

// // #define "TAG" "lwip_udp"
// // const static char http_html_hdr[] =
// //     "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";
// // const static char http_index_hml[] = "<!DOCTYPE html>"
// //       "<html>\n"
// //       "<head>\n"
// //       "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
// //       "  <style type=\"text/css\">\n"
// //       "    html, body, iframe { margin: 0; padding: 0; height: 100%; }\n"
// //       "    iframe { display: block; width: 100%; border: none; }\n"
// //       "  </style>\n"
// //       "<title>HELLO ESP32</title>\n"
// //       "</head>\n"
// //       "<body>\n"
// //       "<h1>Hello World, from ESP32!</h1>\n"
// //       "</body>\n"
// //       "</html>\n";

// // #define SIZEOF_DNSANS_HDR    12
// // #define SIZEOF_DNSANS_HDRQUE 20

// // #define DNS_SERVER_ID        0x0000
// // #define DNS_SERVER_FLAGS     0x8180
// // #define DNS_SERVER_NUMQUE    0x0001
// // #define DNS_SERVER_ANSRRS    0x0001
// // #define DNS_SERVER_AUTRRS    0x0000
// // #define DNS_SERVER_ADDRRS    0x0000
// // #define TABLENAME "mi.com"
// // #define DNS_SERVER_TYPE      0x0001
// // #define DNS_SERVER_CLASS     0x0001
// // #define DNS_POINAME          0xC00C
// // #define DNS_SERVER_ANSTYPE   0x0001
// // #define DNS_SERVER_ANSTYPEE  0x0001
// // #define DNS_SERVER_DATALEN   0x0000
// // #define DNS_SERVER_ANSTIME   0x003c0004
// // #define DNS_SERVER_ADRESS    0xc0a80401
// // uint16_t txid = 0;
// // uint16_t nquestions = 0;
// // uint16_t nanswers = 0;
// // struct udp_pcb *upcb1;
// // const ip_addr_t *addr1;
// // u16_t port1;

// // struct dns_ans_hdr
// // {
// //     PACK_STRUCT_FIELD(uint16_t id);
// //     PACK_STRUCT_FIELD(uint16_t flag);
// //     PACK_STRUCT_FIELD(uint16_t numquestions);
// //     PACK_STRUCT_FIELD(uint16_t numanswers);
// //     PACK_STRUCT_FIELD(uint16_t numauthrr);
// //     PACK_STRUCT_FIELD(uint16_t numextrarr);
// // } PACK_STRUCT_STRUCT;

// // struct dns_ans_ans
// // {
// //     uint16_t typ;
// //     uint16_t cls;
// //     uint16_t point;
// //     uint16_t antyp;
// //     uint16_t antypp;
// //     uint16_t len;
// //     uint32_t time;
// //     uint32_t addr;
// // };

// // struct dns_table_entry
// // {
// //     uint16_t txid;
// //     uint16_t flags;
// //     uint16_t numque;
// //     uint16_t ansrrs;
// //     uint16_t autrrs;
// //     uint16_t addrrs;
// //     char name[256];
// //     uint16_t type;
// //     uint16_t class;
// //     uint16_t poiname;
// //     uint16_t anstype;
// //     uint16_t anstypee;
// //     uint16_t datalen;
// //     uint32_t anstime;
// //     uint32_t adress;
// // };

// // void dns_server_send(void)
// // {
// //     struct pbuf *rp = NULL;
// //     struct dns_ans_hdr hdr;
// //     struct dns_ans_ans qry;
// //     uint8_t n;
// //     uint16_t query_idx, copy_len;
// //     const char *hostname, *hostname_part;
// //     struct dns_table_entry dns_server_table = {
// //         .txid = DNS_SERVER_ID,
// //         .flags = DNS_SERVER_FLAGS,
// //         .numque = DNS_SERVER_NUMQUE,
// //         .ansrrs = DNS_SERVER_ANSRRS,
// //         .autrrs = DNS_SERVER_AUTRRS,
// //         .addrrs = DNS_SERVER_ADDRRS,
// //         .name = {0},
// //         .type = DNS_SERVER_TYPE,
// //         .class = DNS_SERVER_CLASS,
// //         .poiname = DNS_POINAME,
// //         .anstype = DNS_SERVER_ANSTYPE,
// //         .anstypee = DNS_SERVER_ANSTYPEE,
// //         .datalen = DNS_SERVER_DATALEN,
// //         .anstime = DNS_SERVER_ANSTIME,
// //         .adress = DNS_SERVER_ADRESS};
// //     strcpy(dns_server_table.name, TABLENAME);
// //     struct dns_table_entry *entry = &dns_server_table;

// //     rp = pbuf_alloc(PBUF_TRANSPORT, 51, PBUF_RAM);
// //     if (1)
// //     {
// //         memset(&hdr, 0, SIZEOF_DNSANS_HDR);
// //         /* fill dns_ans header */
// //         hdr.id = htons(txid);
// //         hdr.flag = htons(entry->flags);
// //         hdr.numquestions = htons(entry->numque);
// //         hdr.numanswers = htons(entry->ansrrs);
// //         hdr.numauthrr = htons(entry->autrrs);
// //         hdr.numextrarr = htons(entry->addrrs);
// //         pbuf_take(rp, &hdr, SIZEOF_DNSANS_HDR);
// //         /* convert hostname into suitable query format. */
// //         hostname = entry->name;
// //         --hostname;
// //         query_idx = SIZEOF_DNSANS_HDR;
// //         do
// //         {
// //             ++hostname;
// //             hostname_part = hostname;
// //             for (n = 0; *hostname != '.' && *hostname != 0; ++hostname)
// //             {
// //                 ++n;
// //             }
// //             copy_len = (u16_t)(hostname - hostname_part);
// //             pbuf_put_at(rp, query_idx, n);
// //             pbuf_take_at(rp, hostname_part, copy_len, query_idx + 1);
// //             query_idx += n + 1;
// //         } while (*hostname != 0);
// //         pbuf_put_at(rp, query_idx, 0);
// //         query_idx++;
// //         /* fill dns ans */
// //         qry.typ = htons(entry->type);
// //         qry.cls = htons(entry->class);
// //         qry.point = htons(entry->poiname);
// //         qry.antyp = htons(entry->anstype);
// //         qry.antypp = htons(entry->anstypee);
// //         qry.len = htons(entry->datalen);
// //         qry.time = htonl(entry->anstime);
// //         qry.addr = htonl(entry->adress);
// //         printf("the query_idx is %d\n", query_idx);
// //         printf("the qry.addr is %02X\n", qry.addr);
// //         pbuf_take_at(rp, &qry, SIZEOF_DNSANS_HDRQUE, query_idx);

// //         udp_sendto(upcb1, rp, addr1, port1);
// //         //      pbuf_free(rp);
// //     }
// // }

// // void get_dns_request(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
// // {
// //     // printf("get the massage from sat\n");
// //     struct dns_ans_hdr hdr;
// //     upcb1 = upcb;
// //     addr1 = addr;
// //     port1 = port;
// //     // if (p->tot_len < (SIZEOF_DNSANS_HDR + SIZEOF_DNSANS_HDRQUE))
// //     // {
// //     //     LWIP_DEBUGF(DNS_DEBUG, ("dns_recv: pbuf too small\n"));
// //     //     printf("dns_recv: pbuf too small\n");
// //     //     /* free pbuf and return */
// //     // }
// //     // else
// //     // {
// //         pbuf_copy_partial(p, &hdr, SIZEOF_DNSANS_HDR, 0);
// //         txid = ntohs(hdr.id);
// //         nquestions = ntohs(hdr.numquestions);
// //     // }
// //     printf("the length of q: %d\n", p->tot_len);
// //     printf("the txid is: %02X and the questions number is %02X\n", txid, nquestions);
// //     pbuf_free(p); //check this
// //     dns_server_send();
// // }

// // static void web_http_server(struct netconn *conn)
// // {
// //   struct netbuf *inputbuf;
// //   char *buf;
// //   u16_t buflen;
// //   err_t err;

// //   err = netconn_recv(conn, &inputbuf);
// //   if (err == M_ERR_OK) {
// //     netbuf_data(inputbuf, (void**)&buf, &buflen);
// //     printf("the received data:\n%s\n",buf);
// //     /* Judge if this is an HTTP GET command */
// //     // if (buflen >= 5 && buf[0] == 'G' && buf[1] == 'E' && buf[2] == 'T' && buf[3] == ' ' && buf[4] == '/' ) {
// //       netconn_write(conn, http_html_hdr, sizeof(http_html_hdr)-1, NETCONN_NOCOPY);

// //     //   if(buf[5]=='h') {
// //         netconn_write(conn, http_index_hml, sizeof(http_index_hml)-1, NETCONN_NOCOPY);
// // //       }
// // //       else if(buf[5]=='l') {
// // //         netconn_write(conn, http_index_hml, sizeof(http_index_hml)-1, NETCONN_NOCOPY);
// // //       }
// // // //      else if(buf[5]=='j') {
// // // //    	  netconn_write(conn, json_unformatted, strlen(json_unformatted), NETCONN_NOCOPY);
// // // //      }
// // //       else {
// // //           netconn_write(conn, http_index_hml, sizeof(http_index_hml)-1, NETCONN_NOCOPY);
// // //       }
// //     // }
// //   }
// //   netconn_close(conn);
// //   netbuf_delete(inputbuf);
// // }

// // void my_udp_init(void* dad)
// // {
// //     struct udp_pcb *upcb;
// //     err_t err;

// //     upcb = udp_new();
// //     err = udp_bind(upcb, IP_ADDR_ANY, 53);
// //     if (err == M_ERR_OK)
// //     {
// //         udp_recv(upcb, get_dns_request, NULL);
// //     }
// //     else
// //     {
// //         udp_remove(upcb);
// //     }

// // while (1)
// // {
// //     vTaskDelay(100);
// //     /* code */
// // }

// // //      struct netconn *conn, *newconn;
// // //   conn = netconn_new(NETCONN_TCP);
// // //   netconn_bind(conn, NULL, 80);
// // //   netconn_listen(conn);
// // //   while (1) {
// // //     err = netconn_accept(conn, &newconn);
// // //     if (err == M_ERR_OK) {
// // //       web_http_server(newconn);
// // //       netconn_delete(newconn);
// // //     }
// // //     else {
// // //       netconn_close(conn);
// // //       netconn_delete(conn);
// // //       break;
// // //     }
// // //   }
// // }
// #include <string.h>
// #include <sys/param.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "esp_system.h"
// #include "esp_wifi.h"
// #include "esp_event.h"
// #include "esp_log.h"
// #include "nvs_flash.h"
// #include "esp_netif.h"
// #include "esp_event_base.h"
// // #include "esp32/wifi/conn_eth.h"
// // #include "esp32/fun/fun.h"

// // #include "tool.h"
// // #include "hot_so_fast.h"

// int sock;
// void my_udp_init(void *dad)
// {

//     // char rx_buffer[128];
//     // char addr_str[128];
//     int addr_family = AF_INET;
//     int ip_protocol = 0;
//     struct sockaddr_in6 dest_addr;
//     int len = 0;
//     uint8_t data[128];
//     // uint8_t i = 0;
//     struct sockaddr_in client = {0};
//     socklen_t client_len = sizeof(struct sockaddr_in);

//     struct sockaddr_in *dest_addr_ip4 = (struct sockaddr_in *)&dest_addr;
//     dest_addr_ip4->sin_addr.s_addr = htonl(INADDR_ANY);
//     dest_addr_ip4->sin_family = AF_INET;
//     dest_addr_ip4->sin_port = htons(53);
//     ip_protocol = IPPROTO_IP;
//     // watchdog_add(my_udp_init);

//     sock = socket(addr_family, SOCK_DGRAM, ip_protocol);

//     int err = bind(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
//     if (err < 0)
//     {
//         ESP_LOGE("TAG", "Socket unable to bind: errno %d", errno);
//     }
//     while (1)
//     {
//         feed_dogs();
//         // ESP_LOGE("TAG","ror\n");
//         len = recvfrom(sock, data, sizeof(data), 0, (struct sockaddr *)&client, &client_len); //阻塞�?
//         if ((len < 0) || (len > 100))
//         {
//             ESP_LOGE("TAG", "recvfrom error\n");
//             vTaskDelete(NULL);
//             continue;
//         }
//         // printf("%d\r\n",esp_get_free_heap_size()); //打印系统可用内存
//         //过滤掉一些乱糟糟的域�?
//         //  if (strstr((const char *)data + 0xc, "taobao") ||
//         //      strstr((const char *)data + 0xc, "qq") ||
//         //      strstr((const char *)data + 0xc, "sogou") ||
//         //      strstr((const char *)data + 0xc, "amap") ||
//         //      strstr((const char *)data + 0xc, "alipay") ||
//         //      strstr((const char *)data + 0xc, "youku") ||
//         //      strstr((const char *)data + 0xc, "iqiyi") ||
//         //      strstr((const char *)data + 0xc, "baidu"))
//         // printf("%s\r\n", data + 0xc);
//         if (!strstr((const char *)data + 0xc, "apple") || !strstr((const char *)data + 0xc, "captive"))
//         {
//             // printf("tuichule");
//             // printf("%s\r\n", data + 0xc);
//             continue;
//         }
//         //  if (strstr((const char *)data + 0xc, "taobao")
//         // printf("DNS request:");
//         // for(i = 0x4; i< len;i++)
//         // {
//         //     if((data[i] >= 'a' && data[i] <= 'z') || (data[i] >= 'A' && data[i] <= 'Z') ||(data[i] >= '0' && data[i] <= '9')  || data[i] == '.')
//         //         printf("%c",data[i]);
//         //     else
//         //         printf(".");
//         // }
//         // printf("\r\n");
//         data[2] |= 0x80;
//         data[3] |= 0x80;
//         data[7] = 1;
//         data[len++] = 0xc0;
//         data[len++] = 0x0c;
//         data[len++] = 0x00;
//         data[len++] = 0x01;
//         data[len++] = 0x00;
//         data[len++] = 0x01;
//         data[len++] = 0x00;
//         data[len++] = 0x00;
//         data[len++] = 0x00;
//         data[len++] = 0x0A;
//         data[len++] = 0x00;
//         data[len++] = 0x04;
//         data[len++] = 192;
//         data[len++] = 168;
//         data[len++] = 4;
//         data[len++] = 1;
//         /*打印客户端地址和端口号*/
//         // inet_ntop(AF_INET,&client.sin_addr,(char *)data,sizeof(data));
//         // printf("client IP is %s, port is %d\n",data,ntohs(client.sin_port));
//         sendto(sock, data, len, 0, (struct sockaddr *)&client, client_len);
//         vTaskDelay(10);
//     }
// }
// void stop_dns(void)
// {
//     lwip_close(sock);
// }
