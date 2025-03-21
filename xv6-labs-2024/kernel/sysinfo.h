#ifndef SYSINFO_H
#define SYSINFO_H

struct sysinfo {
  uint freemem;
  uint nproc;
#ifndef CHALLENGE

#else
  float loadavg;
#endif
};

#endif
