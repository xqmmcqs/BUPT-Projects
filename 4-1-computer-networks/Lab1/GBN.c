#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "protocol.h"
#include "datalink.h"

#define MAX_PKT 256
#define MAX_SEQ 63
#define DATA_MAX_TIME 1000
#define ACK_MAX_TIME 280

#define inc(k) k = (k + 1) & MAX_SEQ

typedef unsigned int seq_nr;
typedef unsigned char frame_kind;

typedef struct
{
    frame_kind kind;             // 帧的种类
    unsigned char ack;           // ACK序号：最后一个被成功接收的帧序号
    unsigned char seq;           // 帧序号
    unsigned char data[MAX_PKT]; // 数据
    unsigned char padding[4];    // CRC校验段
} Frame;                         // 帧，与物理层通信；seq,data,padding只在DATA类型帧中有效

typedef struct
{
    unsigned char data[MAX_PKT];
    size_t len;
} Packet; // 包，与网络层通信

static bool phl_ready = false;      // 物理层是否就绪
static Packet out_buf[MAX_SEQ + 1]; // 发送窗口

// 判断在窗口中是否有a<=b<c
static bool between(seq_nr a, seq_nr b, seq_nr c)
{
    return a <= b && b < c || c < a && a <= b || b < c && c < a;
}

// 添加校验段，发送DATA帧到物理层
static void put_frame(unsigned char *frame, int len)
{
    *(unsigned int *)(frame + len) = crc32(frame, len);
    send_frame(frame, len + 4);
    phl_ready = false;
}

// 根据不同类型构建帧、发送帧、处理计时器
static void send_dllayer_frame(frame_kind fk, seq_nr frame_nr, seq_nr frame_expected)
{
    Frame s;
    s.kind = fk;
    s.ack = (frame_expected + MAX_SEQ) % (MAX_SEQ + 1);
    if (fk == FRAME_DATA)
    {
        s.seq = frame_nr;
        memcpy(s.data, out_buf[frame_nr].data, out_buf[frame_nr].len);
        dbg_frame("Send DATA %d %d, ID %d\n", s.seq, s.ack, *(short *)s.data);
        put_frame((unsigned char *)&s, 3 + out_buf[frame_nr].len);
        start_timer(frame_nr, DATA_MAX_TIME);
    }
    else
    {
        dbg_frame("Send ACK %d\n", s.ack);
        send_frame((unsigned char *)&s, 3);
    }
    stop_ack_timer();
}

int main(int argc, char **argv)
{
    seq_nr ack_expected = 0;       // 期望收到ACK的帧序号，发送窗口的下界
    seq_nr next_frame_to_send = 0; // 下一个发出的帧序号，发送窗口的上界+1
    seq_nr frame_expected = 0;     // 期望收到的帧序号
    int i;
    Frame r;              // 接收到的帧
    int len;              // 接收到的帧的长度
    seq_nr nbuffered = 0; // 接收到的帧的数量
    int event;
    int arg; // 事件接收到的参数

    protocol_init(argc, argv);
    lprintf("Designed by xqmmcqs, build: " __DATE__ "  "__TIME__
            "\n");

    disable_network_layer();

    while (true)
    {
        event = wait_for_event(&arg);
        // printf("NBUFFERED %d ACK_EXPECTED %d NEXT_FRAME_TO_SEND %d\n", nbuffered, ack_expected, next_frame_to_send);

        switch (event)
        {
        case NETWORK_LAYER_READY: // 网络层就绪
            nbuffered++;
            out_buf[next_frame_to_send].len = get_packet(out_buf[next_frame_to_send].data);
            send_dllayer_frame(FRAME_DATA, next_frame_to_send, frame_expected); // 发送DATA帧
            inc(next_frame_to_send);
            break;

        case PHYSICAL_LAYER_READY:
            phl_ready = 1;
            break;

        case FRAME_RECEIVED:
            len = recv_frame((unsigned char *)&r, sizeof r);
            if (len < 5 && len != 3)
                break;
            if (len >= 5 && crc32((unsigned char *)&r, len) != 0) // 校验错误
            {
                dbg_event("**** Receiver Error, Bad CRC Checksum ****\n");
                break;
            }

            if (r.kind == FRAME_ACK)
                dbg_frame("Recv ACK %d\n", r.ack);

            if (r.kind == FRAME_DATA)
            {
                dbg_frame("Recv DATA %d %d, ID %d\n", r.seq, r.ack, *(short *)r.data);
                if (r.seq == frame_expected)
                {
                    put_packet(r.data, len - 7);
                    inc(frame_expected);
                    start_ack_timer(ACK_MAX_TIME);
                }
            }

            while (between(ack_expected, r.ack, next_frame_to_send)) // 累计确认
            {
                nbuffered--;
                stop_timer(ack_expected);
                inc(ack_expected);
            }
            break;

        case DATA_TIMEOUT:
            dbg_event("---- DATA %d timeout ----\n", arg);
            next_frame_to_send = ack_expected;
            for (int i = 1; i <= nbuffered; ++i)
            {
                send_dllayer_frame(FRAME_DATA, next_frame_to_send, frame_expected);
                inc(next_frame_to_send);
            }
            break;

        case ACK_TIMEOUT:
            dbg_event("---- ACK %d timeout ----\n", frame_expected);
            send_dllayer_frame(FRAME_ACK, 0, frame_expected);
            break;
        }

        if (nbuffered < MAX_SEQ && phl_ready)
            enable_network_layer();
        else
            disable_network_layer();
    }
}
