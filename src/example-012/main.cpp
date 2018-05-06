#include <stdlib.h>
#include <stdio.h>
#define HAVE_REMOTE
#include <pcap.h>

void usage();

void dispatcher_handler(u_char*, const struct pcap_pkthdr*,
                        const u_char*);

int main(int argc, char** argv)
{
    pcap_t *fp;
    char errbuf[PCAP_ERRBUF_SIZE];
    struct timeval st_ts;
    u_int netmask;
    struct bpf_program fcode;

    /* ��������в����ĺϷ��� */
    if (argc != 2) {
        usage();
        return -1;
    }

    /* ����������� */
    if ((fp = pcap_open(argv[1], 100, PCAP_OPENFLAG_PROMISCUOUS,
                        1000, NULL, errbuf)) == NULL) {
        fprintf(stderr, "\nUnable to open adapter %s.\n", errbuf);
        return -1;
    }

    /* ���ù������룬������������У������ᱻʹ�� */
    netmask = 0xffffff;

    // ���������
    if (pcap_compile(fp, &fcode, "tcp", 1, netmask) < 0) {
        fprintf(stderr, "\nUnable to compile the packet filter."
                "Check the syntax.\n");
        /* �ͷ��豸�б� */
        return -1;
    }

    // ���ù�����
    if (pcap_setfilter(fp, &fcode) < 0) {
        fprintf(stderr, "\nError setting the mode.\n");
        pcap_close(fp);
        /* �ͷ��豸�б� */
        return -1;
    }

    /* ���ӿ�����Ϊͳ��ģʽ */
    if (pcap_setmode(fp, MODE_STAT) < 0) {
        fprintf(stderr, "\nError setting the mode.\n");
        pcap_close(fp);
        /* �ͷ��豸�б� */
        return -1;
    }

    printf("TCP traffic summary\n");
    /* ��ʼ��ѭ�� */
    pcap_loop(fp, 0, dispatcher_handler, (PUCHAR)&st_ts);
    pcap_close(fp);
    return 0;
}

void dispatcher_handler(u_char *state, const struct pcap_pkthdr *header,
                        const u_char *pkt_data)
{
    struct timeval *old_ts = (struct timeval *) state;
    u_int delay;
    LARGE_INTEGER Bps, Pps;
    struct tm *ltime;
    char timestr[16];
    time_t local_tv_sec;
    /* ��΢�������һ�β�����ʱ�� */
    /* ���ֵͨ�����õ�ʱ������ */
    delay = (header->ts.tv_sec - old_ts->tv_sec) * 1000000 -
            old_ts->tv_usec + header->ts.tv_usec;
    /* ��ȡÿ��ı����� b/s */
    Bps.QuadPart = (((*(LONGLONG*)(pkt_data + 8)) *  8 * 1000000) / (delay));
    /*                                            ^          ^
                                                  |          |
                                                  |          |
                                                  |          |
                               ���ֽ�ת���ɱ��� --|          |
                                                             |
                                        ��ʱ����΢���ʾ�� --|
    */
    /* �õ�ÿ������ݰ����� */
    Pps.QuadPart = ((*((LONGLONG*)(pkt_data)) * 1000000) / (delay));
    /* ��ʱ���ת��Ϊ����ʶ��ĸ�ʽ */
    local_tv_sec = header->ts.tv_sec;
    ltime = localtime(&local_tv_sec);
    strftime(timestr, sizeof timestr, "%H:%M:%S", ltime);
    /* ��ӡʱ��� */
    printf("%s ", timestr);
    /* ��ӡ������� */
    printf("BPS = %I64u ", Bps.QuadPart);
    printf("PPS = %I64u\n", Pps.QuadPart);
    // �洢��ǰ��ʱ���
    old_ts->tv_sec = header->ts.tv_sec;
    old_ts->tv_usec = header->ts.tv_usec;
}

void usage()
{
    printf("\nShows the TCP traffic load, in bits per second and packets per second."
           "\nCopyright (C) 2002 Loris Degioanni.\n");
    printf("\nUsage:\n");
    printf("\t tcptop adapter\n");
    printf("\t You can use \"WinDump -D\" if you don't know the name of your adapters.\n");
    exit(0);
}