#include "Model.h"

Model::Model()
{

}

float Model::getAz() const
{
  return m_az;
}

void Model::setAz(float az)
{
  m_az = az;
}

float Model::getEl() const
{
  return m_el;
}

void Model::setEl(float el)
{
  m_el = el;
}
