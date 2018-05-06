#define HAVE_REMOTE
#include <stdio.h>
#include <pcap.h>

#define LINE_LEN 16

void dispatcher_handler(u_char*, const struct pcap_pkthdr*,
                        const u_char*);

int main(int argc, char** argv)
{
    pcap_t* fp;
    char errbuf[PCAP_ERRBUF_SIZE];
    char source[PCAP_BUF_SIZE];

    if (argc != 2) {
        printf("useage: %s filename", argv[0]);
        return -1;
    }

    /* ������ WinPcap �﷨����һ��Դ�ַ��� */
    if (pcap_createsrcstr(
            source,         // Դ�ַ���
            PCAP_SRC_FILE,  // ����Ҫ�򿪵�Դ�ļ�
            NULL,           // Զ������
            NULL,           // Զ�������˿�
            argv[1],        // ����Ҫ�򿪵��ļ���
            errbuf          // ���󻺳���
        ) != 0) {
        fprintf(stderr, "\nError createing a source string\n");
        return -1;
    }

    /* �򿪲����ļ� */
    if ((fp = pcap_open(
                  source,       // �豸��
                  65536,
                  // 65536 ��֤�ܲ��񵽲�ͬ������·���ϵ�ÿ�����ݰ���ȫ������
                  PCAP_OPENFLAG_PROMISCUOUS, // ����ģʽ
                  1000,             // ��ȡ��ʱʱ��
                  NULL,         // Զ��������֤
                  errbuf            // ���󻺳��
              )) == NULL) {
        fprintf(stderr, "\nUnable to opent the file %s.\n", source);
        return -1;
    }

    // ��ȡ���������ݰ���ֱ�� EOF Ϊ��
    pcap_loop(fp, 0, dispatcher_handler, NULL);
    return 0;
}

void dispatcher_handler(u_char* temp1, const struct pcap_pkthdr* header,
                        const u_char* pkt_data)
{
    u_int i = 0;
    /* ��ӡ pkt ʱ����� pkt ���� */
    printf("%ld:%ld (%u)\n", header->ts.tv_sec, header->ts.tv_usec,
           header->len);

    /* ��ӡ���ݰ� */
    for (int i = 1; (i < header->caplen + 1); i++) {
        printf("%.2x ", pkt_data[i - 1]);

        if ((i % LINE_LEN) == 0) {
            printf("\n");
        }
    }

    printf("\n\n");
}