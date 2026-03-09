# 이더넷(LwIP) 연결 시 RTC 자동 시간 동기화 제안

## 1. 현재 RTC_Time.c 구조 요약

| 함수 | 역할 |
|------|------|
| `Time_GetUnixTime()` | RTC 카운터(Unix 시간) 읽기 |
| `Time_SetUnixTime(time_t t)` | RTC에 Unix 시간 쓰기 |
| `Time_GetCalendarTime()` | struct tm 형식으로 읽기 |
| `Time_SetCalendarTime(struct tm t)` | struct tm → RTC 쓰기 |
| `Time_ConvUnixToCalendar` / `Time_ConvCalendarToUnix` | 변환 유틸 |
| `RTC_Config()` | 초기화 (BKP DR1 0xA5A5 체크) |

RTC는 32.768kHz LSE 기반, Unix epoch(1970-01-01 00:00 UTC)부터 초 단위 카운트.

---

## 2. 이더넷 연결 감지 지점

`source_ethernet/app_ethernet.c`:

```c
void ethernetif_notify_conn_changed(struct netif *netif)
{
  if(netif_is_link_up(netif))
  {
    // 링크 업: IP 설정 후 netif_set_up(netif);
  }
  else
  {
    netif_set_down(netif);
  }
}
```

**훅 제안**: `netif_is_link_up(netif)` 블록 내부에서  
`RTC_NTP_Sync_OnLinkUp()` 같은 함수를 한 번 호출.

---

## 3. NTP 클라이언트 구현 옵션

### A. LwIP SNTP 모듈 사용 (권장)

- lwIP 2.x 이상에 `apps/sntp/sntp.c` 포함.
- `lwipopts.h`에 `LWIP_SNTP` 활성화 필요.
- **제약**: 현재 `NO_SYS=1`, `LWIP_NETCONN=0` → 순수 콜백 기반으로 동작해야 함.  
  SNTP는 보통 `sys_timeout`으로 주기 폴링. NO_SYS 환경에서 `LWIP_SNTP` 지원 여부 확인 필요.

### B. 최소 NTP 클라이언트 직접 구현

UDP로 NTP 서버(pool.ntp.org 또는 고정 IP)에 요청 1회 전송, 응답 파싱.

```
NTP 패킫 (RFC 5905): 48바이트
- 바이트 0: LI(2) + VN(3) + Mode(3)
- 바이트 40-43: Transmit Timestamp (seconds, big-endian)
응답의 Receive Timestamp(32-35) 또는 Transmit Timestamp 사용.
```

**주의**: DNS가 필요하면 `LWIP_DNS=1` 및 `lwip_gethostbyname` 사용.  
고정 IP(NTP 서버) 사용 시 DNS 불필요.

---

## 4. 제안 로직 흐름

```
[이더넷 링크 업]
      ↓
RTC_NTP_Sync_OnLinkUp() 호출
      ↓
NTP 요청 전송 (UDP, 비블로킹)
      ↓
(타이머/폴링으로) 응답 대기 (타임아웃 예: 5초)
      ↓
응답 수신 → Unix 시간 추출 → Time_SetUnixTime(t)
      ↓
동기화 완료 플래그 설정 (선택: BKP DR2 등에 저장)
```

---

## 5. 구현 스켈레톤 (RTC_Time.c 확장)

```c
/* RTC_Time.c에 추가 */
#include "lwip/opt.h"
#include "lwip/udp.h"
#include "lwip/netif.h"

#define NTP_SERVER_IP   "pool.ntp.org"   /* 또는 "129.6.15.28" (time.nist.gov) */
#define NTP_PORT        123
#define NTP_TIMEOUT_MS  5000

static struct udp_pcb *ntp_pcb;
static ip_addr_t ntp_server;
static uint8_t ntp_sync_pending;

void RTC_NTP_Sync_OnLinkUp(void)
{
    if (ntp_sync_pending) return;
    ntp_sync_pending = 1;
    /* UDP PCB 생성, NTP 요청 패킷 전송, recv 콜백 등록 */
}

static void ntp_recv_cb(void *arg, struct udp_pcb *pcb, struct pbuf *p,
                        ip_addr_t *addr, u16_t port)
{
    if (p && p->tot_len >= 48) {
        uint8_t *payload = (uint8_t *)p->payload;
        uint32_t secs = (payload[40]<<24) | (payload[41]<<16) |
                        (payload[42]<<8) | payload[43];
        /* NTP epoch = 1900, Unix = 1970 → 2208988800UL 보정 */
        time_t unix_secs = (time_t)secs - 2208988800UL;
        Time_SetUnixTime(unix_secs);
    }
    if (p) pbuf_free(p);
    ntp_sync_pending = 0;
}
```

---

## 6. lwipopts.h 수정 제안

- `LWIP_UDP` = 1 (이미 설정됨)
- `MEMP_NUM_UDP_PCB` 여유 확인
- DNS 사용 시: `LWIP_DNS=1`, DNS 서버(GW) 설정
- 필요 시 `LWIP_SNTP=1` (lwIP SNTP 모듈 사용 시)

---

## 7. 호출 위치 (app_ethernet.c)

```c
void ethernetif_notify_conn_changed(struct netif *netif)
{
  if(netif_is_link_up(netif))
  {
    /* 기존 IP 설정 코드 ... */
    netif_set_up(netif);

    /* RTC NTP 동기화 트리거 */
    extern void RTC_NTP_Sync_OnLinkUp(void);
    RTC_NTP_Sync_OnLinkUp();
  }
  /* ... */
}
```

---

## 8. 주기적 재동기화 (선택)

- 일정 간격(예: 24시간)마다 NTP 요청을 다시 보내서 RTC 드리프트 보정.
- `sys_timeout` 또는 메인 루프의 틱 카운터로 주기 체크.

---

## 9. 참고

- NTP 서버: pool.ntp.org, time.google.com, time.windows.com 등
- 타임존: NTP는 UTC 반환. 로컬 시간이 필요하면 `struct tm` 변환 시 오프셋 적용.
- 배터리 백업 RTC가 있으므로, 동기화는 “이더넷 접속 시 1회 + 선택적 주기”로 충분.
