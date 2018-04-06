#ifndef DATAEXCHANGE
#define DATAEXCHANGE

#include "QByteArray";

class IDataExchange
{
public:
  virtual void dataToController(QByteArray data) = 0; // slot
  virtual void dataFromController(QByteArray data) = 0;//signal
};

#endif // DATAEXCHANGE

