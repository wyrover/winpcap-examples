#include <stdlib.h>
#include <stdio.h>

#define HAVE_REMOTE
#define WPCAP

#include<pcap.h>

void usage();

int main(int argc, char **argv)
{
    pcap_t *indesc, *outdesc;
    char errbuf[PCAP_ERRBUF_SIZE];
    char source[PCAP_BUF_SIZE];
    FILE *capfile;
    int caplen, sync;
    u_int res;
    pcap_send_queue *squeue;
    struct pcap_pkthdr *pktheader;
    const u_char *pktdata;
    float cpu_time;
    u_int npacks = 0;

    /* ��������в����ĺϷ��� */
    if (argc <= 2 || argc >= 5) {
        usage();
        return -1;
    }

    /* ��ȡ�����ļ����� */
    capfile = fopen(argv[1], "rb");

    if (!capfile) {
        printf("Capture file not found!\n");
        return -1;
    }

    fseek(capfile, 0, SEEK_END);
    caplen = ftell(capfile) - sizeof(struct pcap_file_header);
    fclose(capfile);

    /* ���ʱ����Ƿ�Ϸ� */
    if (argc == 4 && argv[3][0] == 's') {
        sync = TRUE;
    } else {
        sync = FALSE;
    }

    /* ��ʼ���� */
    /* ���� WinPcap �����﷨����һ��Դ�ַ��� */
    if (pcap_createsrcstr(
            source,         // Դ�ַ���
            PCAP_SRC_FILE,  // ����Ҫ�򿪵��ļ�
            NULL,           // Զ������
            NULL,           // Զ�������˿�
            argv[1],        // ����Ҫ�򿪵��ļ���
            errbuf  // ���󻺳�
        ) != 0) {
        fprintf(stderr, "\nError creating a source string\n");
        return -1;
    }

    printf("%s\n", source);

    /* �򿪲����ļ� */
    if ((indesc = pcap_open(source, 65536, PCAP_OPENFLAG_PROMISCUOUS,
                            1000, NULL, errbuf)) == NULL) {
        fprintf(stderr, "\nUnable to opent the file %s.\n", source);
        return -1;
    }

    /* ��Ҫ����������� */
    if ((outdesc = pcap_open(argv[2], 100, PCAP_OPENFLAG_PROMISCUOUS,
                             1000, NULL, errbuf)) == NULL) {
        fprintf(stderr, "\nUnable to open adapter %s.\n", source);
        return -1;
    }

    /* ��� MAC ���� */
    if (pcap_datalink(indesc) != pcap_datalink(outdesc)) {
        printf("Warning: the datalink of the capture differs from"
               "one of the selected interface.\n");
        printf("Press a key to continue, or CTRL+C to stop.\n");
        getchar();
    }

    /* ���䷢�Ͷ��� */
    squeue = pcap_sendqueue_alloc(caplen);

    /* ���ļ��н����ݰ���䵽���Ͷ��� */
    while ((res = pcap_next_ex(indesc, &pktheader, &pktdata)) == 1) {
        if (pcap_sendqueue_queue(squeue, pktheader, pktdata) == -1) {
            printf("Warning: packet buffer too small, not all the packets"
                   "will be sent.\n");
            break;
        }

        npacks++;
    }

    if (res == -1) {
        printf("Corrupted input file.\n");
        pcap_sendqueue_destroy(squeue);
        return -1;
    }

    /* ���Ͷ��� */
    printf("*");
    cpu_time = (float)clock();

    if ((res = pcap_sendqueue_transmit(outdesc, squeue, sync))
        < squeue->len) {
        printf("An error occured sending the packets: %s."
               "Only %d bytes were sent.\n", pcap_geterr(outdesc), res);
    }

    cpu_time = (clock() - cpu_time) / CLK_TCK;
    printf("\n\nElapsed time: %5.3f\n", cpu_time);
    printf("\nTotal packets generated = %d", npacks);
    printf("\nAverage packets per second = %d",
           (int)((double)npacks / cpu_time));
    printf("\n");
    /* �ͷŷ��Ͷ��� */
    pcap_sendqueue_destroy(squeue);
    /* �ر������ļ� */
    pcap_close(indesc);
    /*
    * �ͷ����������
    * IMPORTANT: �ǵ�һ��Ҫ�ر�����������Ȼ�Ͳ��ܱ�֤
    * ���е����ݰ����ر����ͳ�ȥ
    */
    pcap_close(outdesc);
    return 0;
}


void usage()
{
    printf("\nSendcap, sends a libpcap/tcpdump capture file to the net."
           "Copyright (C) 2002 Loris Degioanni.\n");
    printf("\nUsage:\n");
    printf("\t sendcap file_name adapter [s]\n");
    printf("\nParameters:\n");
    printf("\nfile_name: the name of the dump file that will be sent to the network\n");
    printf("\nadapter: the device to use. Use \"WinDump -D\" for a list of valid devices\n");
    printf("\ns: if present, forces the packets to be sent synchronously,"
           "i.e. respecting the timestamps in the dump file."
           "This option will work only under Windows NTx.\n\n");
    exit(0);
}