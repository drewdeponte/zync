/*
 * Copyright 2003, 2004 Andrew De Ponte
 * 
 * This file is part of zsrep.
 * 
 * zsrep is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 * 
 * zsrep is distributed in the hopes that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with zsrep; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

/**
 * @file zmsg.cc
 * @brief An implimentation file containing message functions for the zsrep.
 *
 * An implimentation file containing definitions of message functions for
 * the zsrep. It also contains documentation for all of the functions it
 * defines.
 */

#include "zmsg.h"   /* This includes the prototypes of the functions */
#include "byteorder.h"

#include <unistd.h>     // write()
#include <errno.h>      // perror()
#include <stdlib.h>     // exit()
#include <time.h>       // time(), localtime()
#include <stdio.h>      // fprintf(), snprintf()
#include <string.h>     // memcpy()
#include <netinet/in.h> // ntohs()
#include <limits.h>     // USHRT_MAX


/**
 * Check if a message is an Ack message.
 *
 * Check if a message is an Acknowledgement message.
 * @return An integer representing a boolean value.
 * @retval 1 True, the message is an Ack message.
 * @retval 0 False, the message is not an Ack message.
 */
int IsAckMessage(const unsigned char *buff) {
    char msg_data[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x96, 0x06};

    if (memcmp(buff, msg_data, 7) == 0) {
        // Return 1 stating that it is an Ack message.
        return 1;
    }

    // Return 0 stating that it is not an Ack message.
    return 0;
}

/**
 * Check if a message is a Rqst message.
 *
 * Check if a message is a Request message.
 * @return An integer representing a boolean value.
 * @retval 1 True, the message is an Rqst message.
 * @retval 0 False, the message is not an Rqst message.
 */
int IsRqstMessage(const unsigned char *buff) {
    char msg_data[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x96, 0x05};

    if (memcmp(buff, msg_data, 7) == 0) {
        // Return 1 stating that it is an Ack message.
        return 1;
    }

    // Return 0 stating that it is not an Ack message.
    return 0;
}

/**
 * Check if a message is an Abrt message.
 *
 * Check if a message is an Abort message.
 * @return An integer representing a boolean value.
 * @retval 1 True, the message is an Abrt message.
 * @retval 0 False, the message is not an Abrt message.
 */
int IsAbrtMessage(const unsigned char *buff) {
    char msg_data[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x96, 0x18};

    if (memcmp(buff, msg_data, 7) == 0) {
        // Return 1 stating that it is an Ack message.
        return 1;
    }

    // Return 0 stating that it is not an Ack message.
    return 0;
}

/**
 * Receive an acknowledgement.
 *
 * Receive an acknowledgement from the opposing side of communication
 * attesting that they received the previously sent message given an open
 * socket descriptor.
 * @param sd The open socket descriptor.
 * @return An integer representing either success or failure.
 * @return 0 Successfully received an acknowledgement.
 * @return 1 Failed, 7 bytes long but didn't match an acknowledgement.
 * @return 2 Failed, was not 7 bytes long.
 */
int RecvAck(const int sd) {
    int retval;
    char msg_data[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x96, 0x06};
    char buff[7];

    retval = read(sd, buff, 7);
    if (retval == -1) {
        perror("RecvAck");
        exit(1);
    } else if (retval == 7) {
        if (memcmp(buff, msg_data, 7) == 0) {    
            // Return 0 in success stating that we received 7 bytes and
            // that those 7 bytes match the acknowledgment message.
            return 0;
        } else {
            // Return 1 signifying that the data read in was 7 bytes
            // long but didn't match.
            return 1;
        }
    } else {
        // Return 2 signifying that that we received data but it was
        // not 7 bytes od data.
        return 2;
    }
}

/**
 * Receive a request.
 *
 * Receive a request from the opposing side of communication attesting that
 * they are ready to receive a message from this side, given an open socket
 * descriptor.
 * @param sd The open socket descriptor.
 * @return An integer representing either success or failure.
 * @return 0 Successfully received an acknowledgement.
 * @return 1 Failed, 7 bytes long but didn't match a request.
 * @return 2 Failed, was not 7 bytes long.
 */
int RecvRqst(const int sd) {
    int retval;
    char msg_data[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x96, 0x05};
    char buff[7];

    retval = read(sd, buff, 7);
    if (retval == -1) {
        perror("RecvAck");
        exit(1);
        
    } else if (retval == 7) {
        if (memcmp(buff, msg_data, 7) == 0) {    
            // Return 0 in success stating that we received 7 bytes and
            // that those 7 bytes match the acknowledgment message.
            return 0;
        } else {
            // Return 1 signifying that the data read in was 7 bytes
            // long but didn't match.
            return 1;
        }
    } else {
        // Return 2 signifying that that we received data but it was
        // not 7 bytes od data.
        return 2;
    }
}

/**
 * Receive a abort.
 *
 * Receive a abort from the opposing side of communication attesting that
 * they are not happy with the prev message, given an open socket
 * descriptor.
 * @param sd The open socket descriptor.
 * @return An integer representing either success or failure.
 * @return 0 Successfully received an abort message.
 * @return 1 Failed, 7 bytes long but didn't match a request.
 * @return 2 Failed, was not 7 bytes long.
 */
int RecvAbrt(const int sd) {
    int retval;
    char msg_data[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x96, 0x18};
    char buff[7];

    retval = read(sd, buff, 7);
    if (retval == -1) {
        perror("RecvAbrt");
        exit(1);
        
    } else if (retval == 7) {
        if (memcmp(buff, msg_data, 7) == 0) {    
            // Return 0 in success stating that we received 7 bytes and
            // that those 7 bytes match the acknowledgment message.
            return 0;
        } else {
            // Return 1 signifying that the data read in was 7 bytes
            // long but didn't match.
            return 1;
        }
    } else {
        // Return 2 signifying that that we received data but it was
        // not 7 bytes od data.
        return 2;
    }
}

/**
 * Send an acknowledgement.
 *
 * Send an acknowledgement stating that data has been receieved
 * successfully.
 * @param sockfd The sockets file descriptor to send message to.
 */
void SendAck(const int sockfd) {
    int retval;
    char msg_data[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x96, 0x06};

    retval = write(sockfd, msg_data, sizeof(msg_data));
    if (retval == -1) {
        perror("SendAck");
        exit(1);
    }
}

/**
 * Send a Request.
 *
 * Send a Request to the opposing side of communication asking them for a
 * message.
 * @param sockfd The sockets file descriptor to send message to.
 */
void SendRqst(const int sockfd) {
    int retval;
    char msg_data[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x96, 0x05};

    retval = write(sockfd, msg_data, sizeof(msg_data));
    if (retval == -1) {
        perror("send_recqst_msg");
        exit(1);
    }
}

/**
 * Receive a message.
 *
 * Receive a message given an open socket descriptor and the
 * address of a MessageType object to store the message and it's content in.
 * @param sd An open socket descriptor.
 * @param pMsg Pointer to MessageType object to store message in.
 * @return An integer representing success or error.
 * @retval 0 Successfully received message.
 * @retval 1 Recieved less than 20 bytes of data, and does not match a msg.
 * @retval 2 Failed, due to failing in setting the content.
 * @retval 3 Failed, body size miss-match.
 * @retval 4 Failed, check sum miss-match.
 * @retval 5 Failed, not enough bytes read in according to wire body size.
 * @retval 6 Failed, received an Ack message instead.
 * @retval 7 Failed, received a Request message instead.
 * @retval 8 Failed, received an Abort message instead.
 * @retval 9 Failed, received 7 unknown bytes.
 */
int RecvMessage(const int sd, MessageType *pMsg) {
    int maxBuffSize;
    maxBuffSize = MSG_HDR_SIZE + 2 + USHRT_MAX + 2;
    unsigned char buff[maxBuffSize];
    int numBytesRead;
    int ret;
    int i;
    unsigned char *pContBuf;

    unsigned short int bodySize;
    unsigned short int checkSum;

    numBytesRead = read(sd, (void *)buff, maxBuffSize);

    printf("libzmsg: RecvMessage(): -----Message Beginning-----\n");

    // A little tid-bit of data that I can see being very very useful.
    printf("libzmsg: RecvMessage(): Read in %d bytes of data.\n",
       numBytesRead);

    // Here is just some extra debug info that may be of use in the process of
    // testing and reverse engineering.
    if (numBytesRead < 20) {
        printf("libzmsg: RecvMessage(): ERROR: The number of bytes read" \
            " in is less than 20.\n");
        if (numBytesRead == 7) {
            if (IsAckMessage(buff)) {
                printf("libzmsg: RecvMessage(): ERROR (Cont): Received data" \
                    " was an Ack Message.\n");
                return 6;
            } else if (IsRqstMessage(buff)) {
                printf("libzmsg: RecvMessage(): ERROR (Cont): Received data" \
                    " was an Rqst Message.\n");
                return 7;
            } else if (IsAbrtMessage(buff)) {
                printf("libzmsg: RecvMessage(): ERROR (Cont): Received data" \
                    " was an Abrt Message.\n");
                return 8;
            } else {
                printf("libzmsg: RecvMessage(): ERROR (Cont): Received data" \
                    " was 7 bytes but is an unknown message.\n");
                return 9;
            }
        } else {
            printf("libzmsg: RecvMessage(): ERROR (Cont): The number of" \
                " bytes read in is less then 20 and NOT 7. The message" \
                " if one is unknown.\n");
        }
        return 1;
    }

    // Set the message header.
    pMsg->SetHeader((void *)buff);
    
    // Get the body size out of the wire message and convert the byte order if
    // needed.
    bodySize = *((unsigned short int *)(buff + MSG_HDR_SIZE));
    if (GetHostByteOrder() == 2) {
        bodySize = htons(bodySize);
    }

    printf("libzmsg: RecvMessage(): Wire Body Size = %d.\n", bodySize);
    printf("libzmsg: RecvMessage(): Expected Message Size = %d bytes.\n",
        (MSG_HDR_SIZE + 2 + bodySize + 2));
    printf("libzmsg: RecvMessage(): Message Bytes Read In = %d bytes.\n",
       numBytesRead);

    // Check to see if the number of bytes read in match the expected number
    // of bytes based on the received message body size.
    if (numBytesRead < (MSG_HDR_SIZE + 2 + bodySize + 2)) {
        printf("libzmsg: RecvMessage(): ERROR: The number of bytes read off" \
            " the wire is NOT enough based on the body size read off" \
            " the wire.\n");
        return 5;
    }

    // Set the message type.
    pMsg->SetType((void *)(buff + MSG_HDR_SIZE + 2));

    // Print the Message type that was just read in and set.
    printf("libzmsg: RecvMessage(): Message Type = %c%c%c.\n",
        *((char *)pMsg->GetType()),
        *((char *)pMsg->GetType() + 1),
        *((char *)pMsg->GetType() + 2));

    // Get the check sum out of the wire message and convert the byte order if
    // needed.
    checkSum = *((unsigned short int *)(buff + MSG_HDR_SIZE + 2 + bodySize));
    if (GetHostByteOrder() == 2) {
        checkSum = htons(checkSum);
    }

    printf("libzmsg: RecvMessage(): Wire Check Sum = %d.\n", checkSum);

    // Set the message content and the internal check sum.
    ret = pMsg->SetContent((void *)(buff + MSG_HDR_SIZE + 2 + MSG_TYPE_SIZE),
        (bodySize - MSG_TYPE_SIZE));
    if (ret != 0) {
        printf("libzmsg: RecvMessage(): Failed to set content.\n");
        return 2;
    }

    printf("libzmsg: RecvMessage(): Set Content.\n");

    // Print the Message Content in hex.
    pContBuf = (unsigned char *)pMsg->GetContent();
    printf("libzmsg: RecvMessage(): Message Content:");
    for (i = 0; i < pMsg->GetContSize(); i++) {
        printf(" 0x%02x", pContBuf[i]);
    }
    printf(".\n");

    printf("libzmsg: RecvMessage(): Calc Body Size = %d.\n",
        pMsg->GetBodySize());
    printf("libzmsg: RecvMessage(): Calc Check Sum = %d.\n",
        pMsg->GetCheckSum());
    if (pMsg->GetBodySize() != bodySize) {
        printf("libzmsg: RecvMessage(): ERROR: Body Size Miss-Match.\n");
        return 3;
    } else if (pMsg->GetCheckSum() != checkSum) {
        printf("libzmsg: RecvMessage(): ERROR: Check Sum Miss-Match.\n");
        return 4;
    }

    // Return 0 in success.
    return 0;
}

/**
 * Send a Message over the wire.
 *
 * Send the given Message over the given open socket descriptor.
 * @return An integer representing success (zero) or failure (non-zero).
 * @retval 0 Sent the message.
 * @retval 1 Failed to allocate memory to build the message for the wire.
 * @retval 2 Failde to send message over the wire.
 */
int SendMessage(const int sd, MessageType *pMsg) {
    unsigned int msgSize;
    unsigned char *wireMsg;
    int retval;
    unsigned short int tmp;

    // Set the wireMsg to NULL by default.
    wireMsg = NULL;

    // Set the msgSize to the total number of bytes required to build the
    // version of the message to send over the wire. The 13 bytes for the
    // message header plus two bytes to hold the body size, plus the size of
    // the actual body, plus 2 more bytes to hold the check sum.
    msgSize = MSG_HDR_SIZE + 2 + pMsg->GetBodySize() + 2;

    // Allocate memory to build the wire message in.
    wireMsg = (unsigned char *)malloc(msgSize);
    if (wireMsg == NULL) {
    return 1;
    }

    // Copy the message header into its proper place.
    memcpy(wireMsg, pMsg->GetHeader(), (size_t)MSG_HDR_SIZE);

    // Copy the body size into its proper place.
    tmp = pMsg->GetBodySize();
    // Switch the byte order from big endian to little endian if the host is
    // big endian because Sharps developers used little endian as the byte
    // order for their protocol (despite that it does not follow standard for
    // nework applications).
    if (GetHostByteOrder() == 2) {
    tmp = ntohs(tmp);
    }
    memcpy(wireMsg + MSG_HDR_SIZE, (void *)&tmp, 2);

    // Copy the body itself into its proper place.
    memcpy(wireMsg + MSG_HDR_SIZE + 2, pMsg->GetType(),
       (size_t)MSG_TYPE_SIZE);
    memcpy(wireMsg + MSG_HDR_SIZE + 2 + MSG_TYPE_SIZE, pMsg->GetContent(),
       (size_t)pMsg->GetContSize());

    // Copy the message check sum into its proper place.
    tmp = pMsg->GetCheckSum();
    // If the host byte order is big-endian I want to switch it in
    // little-endian even though it is NOT the network standard because Sharps
    // developers didn't seem to follow the standard for their protocol. I can
    // use htons() to do this despite its name because htons() and ntohs() do
    // the same thing independent of the host byte order. Hence, if in one
    // byte order calling htons() or ntohs() will change it to the opposite
    // byte order.
    if (GetHostByteOrder() == 2) {
    tmp = ntohs(tmp);
    }
    memcpy(wireMsg + MSG_HDR_SIZE + 2 + pMsg->GetBodySize(), (void *)&tmp, 2);

    // At this point I have completed putting the message together and am
    // ready to send it over the passed socket.
    // Write the data to the socket descriptor.
    retval = write(sd, wireMsg, msgSize);
    if (retval == -1) {
        free(wireMsg);          // Deallocate memory used for temporary
                                // holding place for Message Format data.

        return 2;               // Return 2 signifying that it failed to
                                // write the message to the socket.
    }

    // Free the dynamically allocated memory.
    free(wireMsg);

    // Return in success.
    return 0;
}

/**
 * Send the RAY message.
 *
 * Send the RAY message to the Zaurus SL-5600 that contains the three
 * letters RAY and handle any errors appropriatley.
 * @param sockfd The sockets file descriptor to send message to.
 */
void SendRAY(const int sockfd) {
    RAYMessageType msg;
    SendMessage(sockfd, (MessageType *)&msg);
}

/**
 * Send the RIG message.
 *
 * Send the RIG message to the Zaurus SL-5600 that contains the three
 * letters RIG and handle any errors appropriatley.
 * @param sockfd The sockets file descriptor to send message to.
 */
void SendRIG(const int sockfd) {
    RIGMessageType msg;
    SendMessage(sockfd, (MessageType *)&msg);
}

/** Send the RRL message.
 *
 * Send the RRL message to the Zaurus SL-5600 given a pointer to a c-string
 * which contains the password to send to the Zaurus SL-5600 for
 * authentication.
 * @param sockfd The sockets file descriptor to send message to.
 */
void SendRRL(const int sockfd, char *passwd) {
    RRLMessageType msg(passwd);
    SendMessage(sockfd, (MessageType *)&msg);
}

/**
 * Send the RMG message.
 *
 * Send the RMG message to the Zaurus SL-5600 that contains the three
 * letters RMG and handle any errors appropriatley.
 * @param sockfd The sockets file descriptor to send message to.
 * @param type The identifier specifying the type of synchronization.
 */
void SendRMG(const int sockfd, const unsigned char type) {
    RMGMessageType msg(type);
    SendMessage(sockfd, (MessageType *)&msg);
}

/**
 * Send the RTG message.
 *
 * Send the RTG message to the Zaurus SL-5600 that contains the three
 * letters RTG and handle any errors appropriatley.
 * @param sockfd The sockets file descriptor to send message to.
 */
void SendRTG(const int sockfd) {
    RTGMessageType msg;
    SendMessage(sockfd, (MessageType *)&msg);
}

/**
 * Send the RTS message.
 *
 * Send the RTS message to the Zaurus SL-5600 that contains the three
 * letters RTS and handle any errors appropriatley.  This function grabs the
 * current time of the box this is running on and puts it into the proper
 * RTS message format and sends the RTS message.
 * @param sockfd The sockets file descriptor to send message to.
 */
void SendRTS(const int sockfd) {
    int retval;
    time_t cur_time;
    struct  tm *time_brkdwn;
    char msg_data[15];
    DesktopMessageType msg((void *)"RTS");

    /* Get the current time. */
    cur_time = time(NULL);
    if (cur_time == ((time_t)-1)) {
        perror("SendRTS");
    return;
    }

    /* Break the current time down into something that we can work with. */
    time_brkdwn = localtime(&cur_time);
    if (time_brkdwn == NULL) {
        fprintf(stderr, "SendRTS: failed to break down the local time.\n");
        return;
    }

    /* Place the time in the proper format into the message data. */
    retval = snprintf(msg_data, 15, "%.4d%.2d%.2d%.2d%.2d%.2d",
              (time_brkdwn->tm_year + 1900),
              (time_brkdwn->tm_mon + 1),
              time_brkdwn->tm_mday,
              time_brkdwn->tm_hour,
              time_brkdwn->tm_min,
              time_brkdwn->tm_sec);

    msg.SetContent((void *)msg_data, 14);

    SendMessage(sockfd, (MessageType *)&msg);
}

/**
 * Send the RDI message.
 *
 * Send the RDI message to the Zaurus SL-5600 that contains the three
 * letters RDI and handle any errors appropriatley.
 * @param sockfd The sockets file descriptor to send message to.
 */
void SendRDI(const int sockfd) {
    char msg_data[] = {0x06, 0x07};
    DesktopMessageType msg((void *)"RDI");
    msg.SetContent((void *)msg_data, 2);
    SendMessage(sockfd, (MessageType *)&msg);
}

/**
 * Send the RSY message.
 *
 * Send the RSY message to the Zaurus SL-5600 that contains the three
 * letters RSY and handle any errors appropriatley.
 * @param sockfd The sockets file descriptor to send message to.
 * @param type The identifier for the synchronization type.
 */
void SendRSY(const int sockfd, const unsigned char type) {
    RSYMessageType msg(type);
    SendMessage(sockfd, (MessageType *)&msg);
}

/**
 * Send the RDR message.
 *
 * Send the RDR message to the Zaurus SL-5600 that contains the three
 * letters RDR and handle any errors appropriatley.
 * @param sockfd The sockets file descriptor to send message to.
 * @param type An indicator of the synchronization type.
 * @param syncID The sync ID of the item requesting.
 */
void SendRDR(const int sockfd, const unsigned char type,
         const unsigned long int syncID) {
    RDRMessageType msg(type, syncID);
    SendMessage(sockfd, (MessageType *)&msg);
}

/**
 * Send the RDD message.
 *
 * Send the RDD message to the Zaurus SL-5600 that contains the three
 * letters RDD and handle any errors appropriatley.
 * @param sockfd The sockets file descriptor to send message to.
 * @param type An indicator of the synchronization type.
 * @param syncID The sync ID of the item to delete.
 */
void SendRDD(const int sockfd, const unsigned char type,
         const unsigned long int syncID) {
    RDDMessageType msg(type, syncID);
    SendMessage(sockfd, (MessageType *)&msg);
}

/**
 * Send the RDS message.
 *
 * Send the RDS message to the Zaurus SL-5600 that contains the three
 * letters RDS and handle any errors appropriatley.
 * @param sockfd The sockets file descriptor to send message to.
 * @param type The type of synchronization.
 */
void SendRDS(const int sockfd, const unsigned char type) {
    RDSMessageType msg(type);
    SendMessage(sockfd, (MessageType *)&msg);
}

/**
 * Send the RQT message.
 *
 * Send the RQT message to the Zaurus SL-5600 that contains the three
 * letters RQT and handle any errors appropriatley.
 * @param sockfd The sockets file descriptor to send message to.
 */
void SendRQT(const int sockfd) {
    char msg_data[] = {0x00, 0x00, 0x00};
    DesktopMessageType msg((void *)"RQT");
    msg.SetContent((void *)msg_data, 3);
    SendMessage(sockfd, (MessageType *)&msg);
}

/**
 * Send the RMS message.
 *
 * Send the RMS message to the Zaurus SL-5600 that contains the three
 * letters RMS along with a data length that is 2 bytes and 38 bytes of data
 * following that and handle any errors appropriatley.
 * @param sockfd The sockets file descriptor to send message to.
 */
void SendRMS(const int sockfd) {
    char msg_data[] = {0x00, 0x26, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
               0x07, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    RMSMessageType msg;
    msg.SetContent((void *)msg_data, 40);
    SendMessage(sockfd, (MessageType *)&msg);
}

/**
 * Send a custom RMS message.
 *
 * Send a RMS message with the specified contents. The contents must
 * consist of 38 or less bytes to succeed.
 * @return An integer representing success or failure.
 * @retval 0 Successfully sent the custom RMS message.
 * @retval -1 Failed to send the custom RMS message.
 */
int SendCustRMS(const int sockfd, char *data, unsigned int dsize) {
    char msg_data[40];

    if (dsize > 38)
        return -1;
    
    // Set the len of the message data content to 38 bytes (0x00 0x26)
    msg_data[0] = 0x00;
    msg_data[1] = 0x26;
    // Set the 38 bytes to an initial value of 0x00 so that the padding
    // is already done.
    memset((msg_data + 2), 0, 38);
    memcpy((msg_data + 2), data, dsize);
    // Create the message and send it.
    RMSMessageType msg;
    msg.SetContent((void *)msg_data, 40);
    SendMessage(sockfd, (MessageType *)&msg);
    return 0;
}

void SendEmptyRMS(const int sockfd) {
    char msg_data[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    RMSMessageType msg;
    msg.SetContent((void *)msg_data, 40);
    SendMessage(sockfd, (MessageType *)&msg);
}

void SendRSS(const int sockfd, const unsigned char type) {
    RSSMessageType msg(type);
    SendMessage(sockfd, (MessageType *)&msg);
}

void SendRLR(const int sockfd) {
    char msg_data[] = {0x06};
    DesktopMessageType msg((void *)"RLR");
    msg.SetContent((void *)msg_data, 1);
    SendMessage(sockfd, (MessageType *)&msg);
}

void SendRGE(const int sockfd, char *path) {
    RGEMessageType msg(path);
    SendMessage(sockfd, (MessageType *)&msg);
}


/**
 * Send the "Finished Message" message.
 *
 * Send the "Finished Message"  message to the Zaurus SL-5600 that contains
 * the three letters 96 18 and handle any errors appropriatley.
 * @param sockfd The sockets file descriptor to send message to.
 */
void send_fin_msg(const int sockfd) {
    int retval;
    char msg_data[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x96, 0x18};

    retval = write(sockfd, msg_data, sizeof(msg_data));
    if (retval == -1) {
        perror("send_fin_msg");
        exit(1);
    }
}
