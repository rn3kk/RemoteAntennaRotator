#include <QStringList>
#include <QVariant>
#include <QLoggingCategory>
#include "Encoder.h"
#include "Rotator.h"
#include "Settings.h"

static const QString TCP_UART_CONTROLLER = "tcp_uart_controller";
static const QString TCP_UART_CONTROLLER_ID = "tcp_uart_controller_id";
static const QString SERVO = "servo";

static const QString AZ_ENCODER_ADDRESS = "azEncoderAddress";
static const QString EL_ENCODER_ADDRESS = "elEncoderAddress";
static const QString ROTATOR_ADDRESS = "rotatorAddress";
static const QString NAME = "name";
static const QString TYPE = "type";
static const QString IP = "ip";
static const QString ID = "id";
static const QString PORT = "port";
static const QString MAX_WAITE_DATA = "maxWaiteData";
static const QString ADDRESS = "address";
static const QString AZ_ZERO_ANGLE = "azZeroAngle";
static const QString EL_ZERO_ANGLE = "elZeroAngle";
static const QString SEPARATOR = "/";

Settings* Settings::m_instance=0x0;

Settings::Settings()
{

}

Settings::~Settings()
{

}

Settings *Settings::getInstance()
{
  if(m_instance==0x0)
  {
    m_instance = new Settings();
    if(!m_instance->loadSettings())
    {
      delete m_instance;
      m_instance=0x0;
    }
  }
  return m_instance;
}

void Settings::resetInstance()
{
  if(m_instance)
  {
    delete m_instance;
    m_instance=0x0;
  }
}

bool Settings::loadSettings()
{
  qInfo() << "Start loadSettings()";
  bool ok(true);
  QSettings settings("Config/settings.ini", QSettings::IniFormat);
  if(!loadTcpModule(settings, TCP_UART_CONTROLLER))
  {
    qCritical()  << "Can't load tcp uart module";
    return false;
  }

  QStringList groups = settings.childGroups();
  foreach (QString groupName, groups)
  {
    if(groupName.contains(SERVO) && !loadServo(settings, groupName))
    {
      ok = false;
      break;
    }    
  }

  qInfo() << "End loadSettings() " << (ok?"":"load error");
  return ok;
}

const QVector<Servo *> &Settings::getServoList() const
{
  return m_servoList;
}


bool Settings::loadServo(const QSettings &settings, const QString &path)
{
  QString name = settings.value(path+SEPARATOR+NAME).toString();
  if(name.isEmpty())
  {
    qCritical() << path << SEPARATOR << NAME << "is empty";
    return false;
  }
  QString azEncoderAddress = settings.value(path+SEPARATOR+AZ_ENCODER_ADDRESS).toString();
  QString elEncoderAddress = settings.value(path+SEPARATOR+EL_ENCODER_ADDRESS).toString();
  QString rotatorAddress = settings.value(path+SEPARATOR+ROTATOR_ADDRESS).toString();

  int azZeroAngle = settings.value(path+SEPARATOR+AZ_ZERO_ANGLE, -1).toInt();
  int elZeroAngle = settings.value(path+SEPARATOR+EL_ZERO_ANGLE, -1).toInt();

  if(azEncoderAddress.isEmpty())
  {
    qCritical() << path << SEPARATOR << AZ_ENCODER_ADDRESS << "is empty";
    return false;
  }  
  if(elEncoderAddress.isEmpty())
  {
    qCritical() << path << SEPARATOR << EL_ENCODER_ADDRESS << "is empty";
    return false;
  }
  if(rotatorAddress.isEmpty())
  {
    qCritical() << path << SEPARATOR << ROTATOR_ADDRESS << "is empty";
    return false;
  }
  unsigned char azEncAdr = QByteArray::fromHex(azEncoderAddress.toLatin1()).at(0);
  unsigned char elEncAdr = QByteArray::fromHex(elEncoderAddress.toLatin1()).at(0);
  unsigned char rotAdr = QByteArray::fromHex(rotatorAddress.toLatin1()).at(0);

  Encoder* azEncoder = new Encoder(azEncAdr, azZeroAngle, m_tcpModule);
  Encoder* elEncoder = new Encoder(elEncAdr, azZeroAngle, m_tcpModule);
  Rotator* rotator = new Rotator(rotAdr, m_tcpModule);
  m_servoList.append(new Servo(name, azEncoder, elEncoder, rotator));
  return true;
}


bool Settings::loadTcpModule(const QSettings &settings, const QString &path)
{  
  QString ip = settings.value(path+SEPARATOR+IP).toString();
  if(ip.isEmpty())
  {
    qCritical() << path << SEPARATOR << IP << " is empty";
    return false;
  }
  int port = settings.value(path+SEPARATOR+PORT).toInt();
  if(port == 0)
  {
    qCritical() << path << SEPARATOR << PORT << " is empty";
    return false;
  }
  int maxWaiteData = settings.value(path+SEPARATOR+MAX_WAITE_DATA).toInt();
  if(maxWaiteData==0)
  {
    qCritical() << path << SEPARATOR << MAX_WAITE_DATA << " is empty";
    return false;
  }
  m_tcpModule = new TcpUartModule(ip, port, maxWaiteData);
  return true;
}
