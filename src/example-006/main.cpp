#include <stdlib.h>

#define HAVE_REMOTE
#include <pcap.h>

/* �ص��������� */
void packet_handler(u_char* param, const struct pcap_pkthdr* header,
                    const u_char* pktdata);

int main(int argc, char** argv)
{
    pcap_if_t* alldevs;
    pcap_if_t* d;
    int inum;
    int i = 0;
    pcap_t* adhandle;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_dumper_t* dumpfile;

    /* ������������� */
    if (argc != 2) {
        printf("usage: %s filename", argv[0]);
        return -1;
    }

    /* ��ȡ�����豸�б� */
    if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING,
                            NULL, &alldevs, errbuf) == -1) {
        fprintf(stderr, "Error in pcap_findalldevs: %s\n", errbuf);
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
        printf("\nNo interfaces found! Make sure WinPcap is installed.\n");
        return -1;
    }

    printf("Enter the interface number (1-%d):", i);
    scanf("%d", &inum);

    if (inum < 1 || inum > i) {
        printf("\nInterface number out of range.\n");
        /* �ͷ��б� */
        pcap_freealldevs(alldevs);
        return -1;
    }

    /* ��ת��ѡ�е������� */
    for (d = alldevs; i < inum - 1; d = d->next, i++);

    /* �������� */
    if ((adhandle = pcap_open(d->name, 65535,
                              PCAP_OPENFLAG_PROMISCUOUS, 1000, NULL, errbuf)) == NULL) {
        fprintf(stderr, "\nUnable to open the adapter."
                " %s is not supported by WinPcap\n", d->name);
        /* �ͷ��豸�б� */
        pcap_freealldevs(alldevs);
        return -1;
    }

    /* �򿪶��ļ� */
    dumpfile = pcap_dump_open(adhandle, argv[1]);

    if (dumpfile == NULL) {
        fprintf(stderr, "\nError opening output file.\n");
        return -1;
    }

    printf("\nListenting on %s... Press Ctrl+C to stop...\n", d->description);
    /* �ͷ��豸�б� */
    pcap_freealldevs(alldevs);
    /* ��ʼ���� */
    pcap_loop(adhandle, 0, packet_handler, (unsigned char*) dumpfile);
    return 0;
}

/* �ص������������������ݰ� */
void packet_handler(u_char* dumpfile, const struct pcap_pkthdr* header,
                    const u_char* pkt_data)
{
    /* �������ݰ������ļ� */
    pcap_dump(dumpfile, header, pkt_data);
}