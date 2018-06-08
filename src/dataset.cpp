#include "dataset.h"
#include "assert.h"

void show_cstring(const char *str, int len) {
  for (int i = 0; i < len && str && *str; ++i)
    std::cout << str[i];
  std::cout << std::endl;
}

int UdpDataTransfer::init_client() {
  int ret = 0;
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0)
    ret = -1;

  if (!ret) {
    /* gethostbyname: get the server's DNS entry */
    hostp = gethostbyname(addr);
    if (hostp == NULL)
      ret = -2;
  }

  if (!ret) {
    /* build the server's Internet address */
    memset((char *)&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    memcpy((char *)hostp->h_addr, (char *)&serveraddr.sin_addr.s_addr, hostp->h_length);
    serveraddr.sin_port = htons(port);

    {
      int opt = 1;
      int nb = setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, (const char*)&opt, sizeof(opt));
      if (nb < 0)
        ret = -3;
    }

    /* send the message to the server */
    socklen = sizeof(serveraddr);
  }
  if (ret)
    closesocket(sockfd);

  return ret;
}

int UdpDataTransfer::start_server() {
  struct sockaddr_in clientaddr; /* client addr */
  char *hostaddrp;               /* dotted decimal host addr string */
  int n;                         /* message byte size */
  int ret = 0;

  /*
   * socket: create the parent socket
   */
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0)
    ret = -1;

  if (!ret) {
    int optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char *)&optval,
               sizeof(int));

    memset((char *)&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(port);

    socklen = sizeof(clientaddr);
  }

  if (!ret &&
      ::bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) {
    ret = -2;
  }

  if (!ret) {
    int opt = 1;
    int nb = setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, (const char*)&opt, sizeof(opt));
    if (nb < 0)
      ret = -7;
  }

  if (!ret) {
    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    if (0 != (ret = (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO,
                                (char *)&timeout, sizeof(timeout)) < 0)))
      printf("setsockopt failed\n");

    if (0 != (ret = (setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO,
                                (char *)&timeout, sizeof(timeout)) < 0)))
      printf("setsockopt failed\n");
  }

  this_run = true;
server_loop:
  while (!ret && this_run) {
    n = recvfrom(sockfd, buf, BUFSIZE, 0, (struct sockaddr *)&clientaddr,
                 (socklen_t *)&socklen);
    if (n < 0 && errno == 11)
      goto server_loop;

    ret = (n < 0) ? -3 : 0;

    if (!ret) {
      hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr,
                            sizeof(clientaddr.sin_addr.s_addr), AF_INET);
      if (hostp == NULL)
        ret = -4;
    }

    if (!ret) {
      hostaddrp = inet_ntoa(clientaddr.sin_addr);
      if (hostaddrp == NULL) {
        ret = -5;
        goto server_exit;
      }
#ifdef _DEBUG_
      printf("server received %d bytes datagram from %s (%s) \n", n,
             hostp->h_name, hostaddrp);
      show_cstring(buf, n);
#endif //_DEBUG_

      // callback
      if (!ret && reply_cb) {
        n = reply_cb(sockfd, buf, BUFSIZE, &clientaddr, socklen);
        if (n < 0)
          ret = -6;
      }
    }
  }

server_exit:
  closesocket(sockfd);
  return ret;
}

int UdpDataTransfer::close() {
  closesocket(sockfd);
  return 0;
}

int UdpDataTransfer::send(void *ptr, size_t len) {
  int n = sendto(sockfd, (const char*)ptr, (int)len, 0,
                 (struct sockaddr *)&serveraddr, socklen);
  return n;
}

int UdpDataTransfer::recv() {
  int n = recvfrom(sockfd, buf, BUFSIZE, 0, (struct sockaddr *)&serveraddr,
                   &socklen);
#ifdef _DEBUG_
  show_cstring(buf, n);
#endif //_DEBUG

  return n;
}

template <typename T, typename TInfo>
int UdpDataTransfer::send(DATASET(T) & ds) {
  return 0;
}

template <typename T, typename TInfo>
int UdpDataTransfer::send(TSDATAFRAME(T) & df) {
  return 0;
}

template <typename T, typename TInfo>
int UdpDataTransfer::recv(DATASET(T) & ds) {
  return 0;
}

template <typename T, typename TInfo>
int UdpDataTransfer::recv(TSDATAFRAME(T) & df) {
  return 0;
}

void UdpDataTransfer::stop_server() { this_run = false; }
