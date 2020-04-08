#include <QStringList>
#include <QVariant>
#include <QLoggingCategory>
#include "Settings.h"

static const QString TCP_UART_CONTROLLER = "tcp_uart_controller";
static const QString TCP_UART_CONTROLLER_ID = "tcp_uart_controller_id";
static const QString SERVO = "servo";

static const QString ENCODER_ADDRESS = "encoderAddress";
static const QString ROTATOR_ADDRESS = "rotatorAddress";
static const QString NAME = "name";
static const QString TYPE = "type";
static const QString IP = "ip";
static const QString ID = "id";
static const QString PORT = "port";
static const QString MAX_WAITE_DATA = "maxWaiteData";
static const QString ADDRESS = "address";
static const QString ZERO_ANGLE = "zeroAngle";
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
  QString encoderAddress = settings.value(path+SEPARATOR+ENCODER_ADDRESS).toString();
  QString rotatorAddress = settings.value(path+SEPARATOR+ROTATOR_ADDRESS).toString();
  int zeroAngle = settings.value(path+SEPARATOR+ZERO_ANGLE, -1).toInt();

  if(encoderAddress.isEmpty())
  {
    qCritical() << path << SEPARATOR << ENCODER_ADDRESS << "is empty";
    return false;
  }  
  if(rotatorAddress.isEmpty())
  {
    qCritical() << path << SEPARATOR << ROTATOR_ADDRESS << "is empty";
    return false;
  }
  m_servoList.append(new Servo(name, QByteArray::fromHex(encoderAddress.toLatin1()), QByteArray::fromHex(rotatorAddress.toLatin1()), zeroAngle, m_tcpModule ));
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

int Settings::getAngleShiftForEncoder(unsigned short address) const
{
  foreach (Servo* e, m_servoList) {
    if(address == e->getEncoderAddress())
    {
      return e->getAngleShift();
    }
  }
}
