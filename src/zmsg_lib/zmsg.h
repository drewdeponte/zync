/*
 * Copyright 2003, 2004, 2005 Andrew De Ponte
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
 * @file zmsg.h
 * @brief A specifications file of message functions for the zsrep.
 *
 * A specifications file containing prototypes of message functions for the
 * zsrep. Some of the messages are "i got the message", "send me a message",
 * etc. The functions consit of messages that are commonley seen throughout
 * synchronization between a stock Zaurus SL-5600 and Qtopia Desktop for
 * Windows that comes with the Zaurus SL-5600.
 */

#ifndef ZMSG_H
#define ZMSG_H

#include "DesktopMessageType.h"
#include "ZaurusMessageType.h"

#include "RAYMessageType.h"
#include "AAYMessageType.h"
#include "RIGMessageType.h"
#include "AIGMessageType.h"
#include "RRLMessageType.h"
#include "RMSMessageType.h"
#include "AMGMessageType.h"
#include "RGEMessageType.h"
#include "AGEMessageType.h"
#include "RTGMessageType.h"
#include "ATGMessageType.h"
#include "ALRMessageType.h"
#include "ASYMessageType.h"
#include "AEXMessageType.h"
#include "RSYMessageType.h"
#include "RDRMessageType.h"
#include "ADRMessageType.h"
#include "RDSMessageType.h"
#include "RDDMessageType.h"
#include "RMGMessageType.h"
#include "RDWMessageType.h"
#include "ADWMessageType.h"
#include "RDIMessageType.h"
#include "ADIMessageType.h"
#include "RSSMessageType.h"

#include <stdlib.h>
#include <string.h>

int IsAckMessage(const unsigned char *buff);
int IsRqstMessage(const unsigned char *buff);
int IsAbrtMessage(const unsigned char *buff);

int RecvAck(const int sd);
int RecvRqst(const int sd);
int RecvAbrt(const int sd);
void SendAck(const int sockfd);
void SendRqst(const int sockfd);

int RecvMessage(const int sd, MessageType *pMsg);
int SendMessage(const int sd, MessageType *pMsg);

void SendRAY(const int sockfd);
void SendRIG(const int sockfd);

void SendRRL(const int sockfd, char *passwd);
void SendRMG(const int sockfd, const unsigned char type);
void SendRTG(const int sockfd);
void SendRTS(const int sockfd);
void SendRDI(const int sockfd);
void SendRSY(const int sockfd, const unsigned char type);
void SendRDR(const int sockfd, const unsigned char type,
	     const unsigned long int syncID);
void SendRDD(const int sockfd, const unsigned char type,
	     const unsigned long int syncID);
void SendRDS(const int sockfd, const unsigned char type);
void SendRQT(const int sockfd);
void SendRMS(const int sockfd);
int SendCustRMS(const int sockfd, char *data, unsigned int dsize);
void SendEmptyRMS(const int sockfd);
void SendRSS(const int sockfd, const unsigned char type);
void SendRLR(const int sockfd);
void SendRGE(const int sockfd, char *path);

void send_fin_msg(const int sockfd);

#endif
