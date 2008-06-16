#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#include "base/flags.h"
#include "base/logging.h"
#include "base/util.h"

#include <string>
using namespace std;

DEFINE_FLAGS(string, server_ip, "The Server's ip:*.*.*.*.");
DEFINE_FLAGS(int, server_port, "the port of the Server.");
DEFINE_FLAGS(int, start, "the start index of the Rejudge sequence");
DEFINE_FLAGS(int, end, "the end index of the Rejudge sequence");

int main(int argc, char **argv) {  
  if (parseFlags(argc,argv)) {
    LOG(SYS_ERROR) << "Cannot parse flags!";
    return -1;
  }
  
  char command[21];
  char status_id[21];
  
  for (int i = FLAGS_start; i < FLAGS_end; i++) { 
    int sockfd;
    struct sockaddr_in serveraddr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
      LOG(SYS_ERROR) << "Cannot create socket";
      return -1;
    }

    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(FLAGS_server_port);
    if (inet_pton(AF_INET, FLAGS_server_ip.c_str(), &serveraddr.sin_addr) <= 0) {
      LOG(ERROR) << "Server ip format error";
      return -1;
    }

    if (connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == -1) {
      LOG(SYS_ERROR) << "Cannot connect to " 
                     << FLAGS_server_ip << ":" 
                     << FLAGS_server_port;
      close(sockfd);
      return -1;
    }
    
    memset(status_id, 0, sizeof(status_id));
    memset(command, 0, sizeof(command));
    sprintf(status_id, "%d", i);
    LOG(DEBUG) << "status_id:" << status_id;
    int len = strlen(status_id);
    sprintf(command, "rj%08d", len);
    LOG(DEBUG) << "command:" << command;

    if (socket_write(sockfd, command, 10) != 0) {
      LOG(SYS_ERROR) << "Cannot send header";
      close(sockfd);
      return -1;
    }

    if (socket_write(sockfd, status_id, len) != 0) {
      LOG(SYS_ERROR) << "Cannot send status_id";
      close(sockfd);
      return -1;
    }
    
    char ret;

    if (socket_read(sockfd, &ret, 1) != 1) {
      LOG(SYS_ERROR) << "Cannot read the ARK";
      close(sockfd);
      return -1;
    }
    if (ret == 'Y')
      LOG(INFO) << "System add RejudgeMission[" << i << "] suscessful";
    else
      LOG(INFO) << "System add RejudgeMission[" << i << "] failed";
    close(sockfd);
  }
  return 0;
}
