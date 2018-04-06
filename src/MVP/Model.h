#ifndef MODEL_H
#define MODEL_H

class Model
{
public:
  Model();

  float getAz() const;
  void setAz(float az);

  float getEl() const;
  void setEl(float el);

private:
  float m_az;
  float m_el;
};

#endif // MODEL_H
