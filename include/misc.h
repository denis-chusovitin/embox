/**
 * \file misc.h
 *
 * \date 27.05.09
 * \author sikmir
 */

#ifndef MISC_H_
#define MISC_H_

#include "common.h"

unsigned char *ipaddr_scan(unsigned char *addr, unsigned char res[4]);
unsigned char *macaddr_scan(unsigned char *addr, unsigned char res[6]);
void ipaddr_print(const char *buf, const unsigned char *addr);
void macaddr_print(const char *buf, const unsigned char *addr);
int is_addr_from_net(const unsigned char *uip, const unsigned char *nip, unsigned char msk);

#endif //MISC_H_
