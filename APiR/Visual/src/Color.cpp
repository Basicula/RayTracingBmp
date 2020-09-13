#include <Visual/Color.h>

#include <time.h>

const Color Color::White  = Color(255, 255, 255);
const Color Color::Black  = Color(0, 0, 0);
const Color Color::Blue   = Color(0, 0, 255);
const Color Color::Green  = Color(0, 255, 0);
const Color Color::Red    = Color(255, 0, 0);
const Color Color::Yellow = Color(255, 255, 0);

Color::Color()
  : m_rgba(0)
  {};

Color::Color(std::uint32_t i_abgr)
  : m_rgba(i_abgr)
  {};

Color::Color(
  std::uint8_t i_red, 
  std::uint8_t i_green, 
  std::uint8_t i_blue,
  std::uint8_t i_alpha)
  : Color()
  {
  auto rgba_data = reinterpret_cast<std::uint8_t*>(&m_rgba);
  rgba_data[0] = i_red;
  rgba_data[1] = i_green;
  rgba_data[2] = i_blue;
  rgba_data[3] = i_alpha;
  };

Color::Color(const Color& i_other)
  : m_rgba(i_other.m_rgba)
  {}

Color& Color::operator=(const Color & i_other)
  {
  if (this == &i_other) {
    return *this;
    }
  m_rgba = i_other.m_rgba;
  return *this;
  }
;

bool Color::operator==(const Color& i_other) const
  {
  return m_rgba == i_other.m_rgba;
  }

bool Color::operator!=(const Color& i_other) const
  {
  return m_rgba != i_other.m_rgba;
  }

Color Color::operator*(double i_factor) const
  {
  if (i_factor < 0)
    return Color();
  const auto r = static_cast<std::uint8_t>(std::min(255.0, i_factor * GetRed()));
  const auto g = static_cast<std::uint8_t>(std::min(255.0, i_factor * GetGreen()));
  const auto b = static_cast<std::uint8_t>(std::min(255.0, i_factor * GetBlue()));
  return Color(r, g, b, GetAlpha());
  }

Color Color::operator*(const Vector3d& i_factor) const
  {
  const auto r = static_cast<std::uint8_t>(std::min(255.0, std::max(0.0, i_factor[0] * GetRed())));
  const auto g = static_cast<std::uint8_t>(std::min(255.0, std::max(0.0, i_factor[1] * GetGreen())));
  const auto b = static_cast<std::uint8_t>(std::min(255.0, std::max(0.0, i_factor[2] * GetBlue())));
  return Color(r, g, b, GetAlpha());
  }

Color Color::operator+(const Color& i_other) const
  {
  const auto r = static_cast<std::uint8_t>(std::min(255, int(GetRed()) + int(i_other.GetRed())));
  const auto g = static_cast<std::uint8_t>(std::min(255, int(GetGreen()) + int(i_other.GetGreen())));
  const auto b = static_cast<std::uint8_t>(std::min(255, int(GetBlue()) + int(i_other.GetBlue())));
  const auto a = static_cast<std::uint8_t>(std::min(255, int(GetAlpha()) + int(i_other.GetAlpha())));
  return Color(r, g, b);
  }

Color& Color::operator+=(const Color& i_other)
  {
  auto rgba_data = reinterpret_cast<std::uint8_t*>(&m_rgba);
  rgba_data[0] = static_cast<std::uint8_t>(std::min(255, int(GetRed()) + int(i_other.GetRed())));
  rgba_data[1] = static_cast<std::uint8_t>(std::min(255, int(GetGreen()) + int(i_other.GetGreen())));
  rgba_data[2] = static_cast<std::uint8_t>(std::min(255, int(GetBlue()) + int(i_other.GetBlue())));
  rgba_data[3] = static_cast<std::uint8_t>(std::min(255, int(GetAlpha()) + int(i_other.GetAlpha())));
  return *this;
  }

Color Color::RandomColor()
  {
  srand(rand() % RAND_MAX);
  return Color(
    rand() % UINT8_MAX, 
    rand() % UINT8_MAX, 
    rand() % UINT8_MAX);
  }