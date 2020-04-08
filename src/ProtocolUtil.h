#ifndef PROTOCOLUTIL_H
#define PROTOCOLUTIL_H

#include <QByteArray>

struct EncoderData
{
  EncoderData(unsigned char encoderAddress, float deg, bool magnetIsOk, bool dataIsValid)
  {
    this->encoderAddress = encoderAddress;
    this->deg = deg;    
    this->magnetIsOk = magnetIsOk;
    this->dataIsValid = dataIsValid;
  }
  EncoderData(const EncoderData& data)
  {
    encoderAddress=data.encoderAddress;
    deg=data.deg;
    magnetIsOk=data.magnetIsOk;
    dataIsValid=data.dataIsValid;
  }

  EncoderData()
  {
    encoderAddress=0;
    deg=-1;
    magnetIsOk=false;
    dataIsValid=false;
  }

  unsigned char encoderAddress;
  float deg;
  bool magnetIsOk;
  bool dataIsValid;
};

class ProtocolUtil
{
public:
  ProtocolUtil();  

  EncoderData getEncoderData(const QByteArray& data);

  static unsigned char createCrc(const unsigned char datalen, const unsigned char address, const unsigned char data1, const unsigned char data2, const unsigned char data3);

  private:
    int getAngle(int currentAngle, int angleShift);
};

#endif // PROTOCOLUTIL_H
