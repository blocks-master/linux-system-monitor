#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  //  See src/processor.cpp

  //  Declare any necessary private members
 private:
 long jiffieStart{0};
 long activeJiffieStart{0};
 long jiffieEnd{0};
 long activeJiffieEnd{0};


};

#endif