/**
 * @file   Md5.h
 * @author Arcila Thomas, Dallarosa Damien, Grange Benjamin, Martin Loic
 * @date   Tue Apr  6 13:50:43 2004
 * 
 * @brief  Md5 sum support 
 * 
 * $Log: Md5.h,v $
 * Revision 1.2  2004/04/20 11:16:39  pumpkins
 * gzstream
 * authors
 * models
 *
 * Revision 1.1  2004/04/06 11:57:45  pumpkins
 * support for md5sum
 *
 * 
 */

#ifndef MD5_H
#define MD5_H

#include "helpers/md5.h"

class Md5 {
 private:
  unsigned char hash[16];
  char buffer[256];
  struct md5_ctx ctx;

  
 public:
  void md5Buffer (const char *buffer, int size) {  
    md5_buffer (buffer, size, hash);    
  }

  void md5File (std::istream& ifs) {
    md5_init_ctx (&ctx);
    
    while (true) {
      ifs.read(buffer, sizeof(buffer));

      if (ifs.gcount()>0) {
	if (ifs.gcount() % 64 == 0)
	  md5_process_block (buffer, ifs.gcount(), &ctx);
	else
	  md5_process_bytes (buffer, ifs.gcount(), &ctx);
      }
      if (!ifs)
	break;
    }
    
    md5_finish_ctx (&ctx, hash);
  }

  friend std::ostream& operator<< (std::ostream& os, const Md5& s) {
    for (unsigned int i=0; i<sizeof(s.hash); ++i)
      os << std::setw(2) << std::setfill('0') << std::right 
		<< std::hex << static_cast<unsigned int>(s.hash[i]);
    return os;
  }
  
  void getMd5 (unsigned char buf[16]) {
    for (unsigned int i=0; i<sizeof(hash); ++i)
      buf[i]=hash[i];
  }
};

#endif /* MD5_H */
