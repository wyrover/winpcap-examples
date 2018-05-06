#include <stdlib.h>

#define HAVE_REMOTE
#include <pcap.h>

/* 4 �ֽڵ� IP ��ַ */
typedef struct ip_address {
    u_char byte1;
    u_char byte2;
    u_char byte3;
    u_char byte4;
} ip_address;

/* IPv4 �ײ� */
typedef struct ip_header {
    u_char ver_ihl;         // �汾 (4 bits) + �ײ����� (4 bits);
    u_char tos;             // �������� (Type of services)
    u_short tlen;           // �ܳ� (Total length)
    u_short identification; // ��ʶ (Identification)
    u_short flags_fo;       // ��־λ (Flags) (3 bits) +
    // ��ƫ���� (Fragment offset) (13bits)
    u_char ttl;             // ���ʱ�� (Time to live)
    u_char proto;           // Э�� (Protocol)
    u_short crc;            // �ײ������ (Header checksum)
    ip_address saddr;       // Դ��ַ (Source address)
    ip_address daddr;       // Ŀ�ĵ�ַ (Destination address)
    u_int op_pad;           // ѡ������� (Option + Padding)
} ip_header;

/* UDP �ײ� */
typedef struct udp_header {
    u_short sport;          // Դ�˿� (Source port)
    u_short dport;          // Ŀ�Ķ˿� (Destination port)
    u_short len;            // UDP ���ݰ����� (Datagram length)
    u_short crc;            // ����� (Checksum)
} udp_header;

/* �ص�����ԭ�� */
void packet_handler(u_char* param, const struct pcap_pkthdr* header,
                    const u_char* pkt_data);

int main(void)
{
    pcap_if_t* alldevs;
    pcap_if_t* d;
    int inum;
    int i = 0;
    pcap_t* adhandle;
    char errbuf[PCAP_ERRBUF_SIZE];
    u_int netmask;
    char packet_filter[] = "ip and udp";
    struct bpf_program fcode;

    /* ��ȡ�豸�б� */
    if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1) {
        fprintf(stderr, "Error in pcap_findalldevs: %s.\n", errbuf);
        exit(1);
    }

    /* ��ӡ�б� */
    for (d = alldevs; d != NULL; d = d->next) {
        printf("%d.%s", ++i, d->name);

        if (d->description) {
            printf(" (%s)\n", d->description);
        } else {
            printf(" (No description available)\n");
        }
    }

    if (i == 0) {
        printf("\nNo interfaces found! Make sure WinPcap installed.\n");
        return -1;
    }

    printf("Enter the interface number (1-%d):", i);
    scanf("%d", &inum);

    if (inum < 1 || inum > i) {
        printf("\nInterface number out of range.\n");
        /* �ͷ��豸�б� */
        pcap_freealldevs(alldevs);
        return -1;
    }

    /* ��ת����ѡ�豸 */
    for (d = alldevs, i = 0; i < inum - 1; d = d->next, i++);

    /* �������� */
    if ((adhandle = pcap_open(d->name, 65535,
                              PCAP_OPENFLAG_PROMISCUOUS, 1000, NULL, errbuf)) == NULL) {
        fprintf(stderr, "\nUnable to open the adapter."
                "%s is not supported by WinPcap.", d->name);
        /* �ͷ��豸�б� */
        pcap_freealldevs(alldevs);
        return -1;
    }

    /* ���������·�㣬Ϊ�˼�ͣ�����ֻ������̫�� */
    if (pcap_datalink(adhandle) != DLT_EN10MB) {
        fprintf(stderr, "\nThis program works only on Ethernet networks.\n");
        /* �ͷ��豸�б� */
        pcap_freealldevs(alldevs);
        return -1;
    }

    if (d->addresses != NULL) {
        /* ��ȡ�ӿڵ�һ����ַ������ */
        netmask = ((struct sockaddr_in*)
                   (d->addresses->netmask))->sin_addr.s_addr;
    } else {
        /* ����ӿ�û�е�ַ����ô���Ǽ���һ�� C ������� */
        netmask = 0xffffff;
    }

    // �༭������
    if (pcap_compile(adhandle, &fcode, packet_filter, 1, netmask) < 0) {
        fprintf(stderr, "\nUnable to compile the packet filter."
                "Check the syntex.\n");
        /* �ͷ��豸�б� */
        pcap_freealldevs(alldevs);
        return -1;
    }

    printf("\nListening on %s...\n", d->description);
    /* �ͷ��豸�б� */
    pcap_freealldevs(alldevs);
    /* ��ʼ��׽ */
    pcap_loop(adhandle, 0, packet_handler, NULL);
    return 0;
}

/* �ص����������յ�ÿһ�����ݰ�ʱ�ᱻ WinPcap ���á�*/
void packet_handler(u_char* param, const pcap_pkthdr* header,
                    const u_char* pkt_data)
{
    struct tm* ltime;
    char timestr[16];
    ip_header* ih;
    udp_header* uh;
    u_int ip_len;
    u_short sport, dport;
    time_t local_tv_sec;
    /* ��ʱ���ת���ɿ�ʶ��ĸ�ʽ */
    local_tv_sec = header->ts.tv_sec;
    ltime = localtime(&local_tv_sec);
    strftime(timestr, sizeof timestr, "%H:%M:%S", ltime);
    /* ��ӡ���ݰ���ʱ����ͳ��� */
    printf("%s.%.6ld len:%d ", timestr, header->ts.tv_usec, header->len);
    /* ��� IP ���ݰ�ͷ����λ�� */
    ih = (ip_header*)(pkt_data + 14);  // 14����̫��ͷ������
    /* ��� UDP ���ݰ�ͷ��λ�� */
    ip_len = (ih->ver_ihl & 0xf) * 4;
    uh = (udp_header*)((u_char*)ih + ip_len);
    /* �������ֽ�����ת���������ֽ����� */
    sport = ntohs(uh->sport);
    dport = ntohs(uh->dport);
    /* ��ӡ IP ��ַ�� UDP �˿� */
    printf("%d.%d.%d.%d : %d -> %d.%d.%d.%d. : %d\n",
           ih->saddr.byte1, ih->saddr.byte2,
           ih->saddr.byte3, ih->saddr.byte4, sport,
           ih->daddr.byte1, ih->daddr.byte2,
           ih->daddr.byte3, ih->daddr.byte4, dport);
}