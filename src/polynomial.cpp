#include "polynomial.h"
#include <cmath>
#include <algorithm>
#include <map>
#include <vector>

Polynomial Polynomial::operator+(const Polynomial& other) const
{
    Polynomial result;

    auto it1 = mMonomials.begin();
    auto it2 = other.mMonomials.begin();

    while (it1 != mMonomials.end() && it2 != other.mMonomials.end())
    {
        uint64_t deg1 = (*it1).degree();
        uint64_t deg2 = (*it2).degree();

        if (deg1 > deg2)
        {
            result.mMonomials.pushBack(*it1);
            ++it1;
        } else if (deg2 > deg1)
        {
            result.mMonomials.pushBack(*it2);
            ++it2;
        } else
        {
            double coefficient = (*it1).coefficient() + (*it2).coefficient();
            if (coefficient != 0.0)
            {
                result.mMonomials.pushBack(Monomial(coefficient, (*it1).w(), (*it1).x(), (*it1).y(), (*it1).z()));
            }
            ++it1;
            ++it2;
        }
    }

    while (it1 != mMonomials.end())
    {
        result.mMonomials.pushBack(*it1);
        ++it1;
    }

    while (it2 != other.mMonomials.end())
    {
        result.mMonomials.pushBack(*it2);
        ++it2;
    }

    return result;
}

Polynomial Polynomial::operator*(const Polynomial& other) const
{
    Polynomial res{};

    for (const auto& ms : mMonomials)
    {
        Polynomial tmp{};

        for (const auto& mo : other.mMonomials)
        {
            double coef = ms.coefficient() * mo.coefficient();
            uint32_t xDeg = ms.x() + mo.x();
            uint32_t yDeg = ms.y() + mo.y();
            uint32_t zDeg = ms.z() + mo.z();
            uint32_t wDeg = ms.w() + mo.w();

            if (xDeg > UINT16_MAX || yDeg > UINT16_MAX || zDeg > UINT16_MAX || wDeg > UINT16_MAX)
            {
                throw "Overflow in multiplication occurred";
            }

            tmp.mMonomials.pushBack(Monomial(coef, wDeg, xDeg, yDeg, zDeg));
        }

        res += tmp;
    }

    return res;
}

Polynomial Polynomial::operator*(double coefficient) const
{
    Polynomial result;

    if (coefficient == 0.0) return result;

    for (auto it = mMonomials.begin(); it != mMonomials.end(); ++it)
    {
        result.mMonomials.pushBack(Monomial(coefficient * (*it).coefficient(), (*it).w(), (*it).x(), (*it).y(), (*it).z()));
    }

    return result;
}

Polynomial operator*(double coefficient, const Polynomial& p)
{
    return p * coefficient;
}

std::ostream& operator<<(std::ostream& ostr, const Polynomial& p)
{
    if (p.mMonomials.size() == 0)
    {
        ostr << 0;
        return ostr;
    }

    bool first = true;
    for (auto& v : p.mMonomials)
    {
        if (!first && v.coefficient() > 0.0) ostr << '+';
        first = false;

        ostr << v.coefficient();
        if (v.w())
        {
            ostr << "*w";
            if (v.w() > 1) ostr << '^' << static_cast<int32_t>(v.w());
        }
        if (v.x())
        {
            ostr << "*x";
            if (v.x() > 1) ostr << '^' << static_cast<int32_t>(v.x());
        }
        if (v.y())
        {
            ostr << "*y";
            if (v.y() > 1) ostr << '^' << static_cast<int32_t>(v.y());
        }
        if (v.z())
        {
            ostr << "*z";
            if (v.z() > 1) ostr << '^' << static_cast<int32_t>(v.z());
        }
    }

    return ostr;
}

Polynomial Polynomial::operator-() const
{
    return (*this) * -1.0;
}

Polynomial Polynomial::operator-(const Polynomial& other) const
{
    return (*this) + (-other);
}

Polynomial& Polynomial::operator+=(const Polynomial& other)
{
    *this = *this + other;
    return *this;
}

Polynomial& Polynomial::operator-=(const Polynomial& other)
{
    *this = *this - other;
    return *this;
}

Polynomial& Polynomial::operator*=(double coefficient)
{
    *this = *this * coefficient;
    return *this;
}

bool Polynomial::operator==(const Polynomial& other) const
{
    if (mMonomials.size() != other.mMonomials.size())
    {
        return false;
    }

    for (auto it1 = mMonomials.begin(), it2 = other.mMonomials.begin(); it1 != mMonomials.end() && it2 != other.mMonomials.end(); ++it1, ++it2)
    {
        if (*it1 != *it2)
        {
            return false;
        }
    }

    return true;
}

bool Polynomial::operator!=(const Polynomial& other) const
{
    return !operator==(other);
}

double Polynomial::evaluate(double w, double x, double y, double z) const
{
    double res = 0.0;

    for (auto it = mMonomials.begin(); it != mMonomials.end(); ++it)
    {
        res += (*it).coefficient() *
            pow(w, static_cast<double>((*it).w())) *
            pow(x, static_cast<double>((*it).x())) *
            pow(y, static_cast<double>((*it).y())) *
            pow(z, static_cast<double>((*it).z()));
    }

    return res;
}

Polynomial Polynomial::derivativeW() const
{
    Polynomial res;

    for (auto it = mMonomials.begin(); it != mMonomials.end(); ++it)
    {
        uint16_t degree = (*it).w();
        if (degree > 0)
        {
            res.mMonomials.pushBack(
                Monomial(static_cast<double>(degree) * (*it).coefficient(), (*it).w() - 1, (*it).x(), (*it).y(), (*it).z())
            );
        }
    }

    return res;
}

Polynomial Polynomial::derivativeX() const
{
    Polynomial res;

    for (auto it = mMonomials.begin(); it != mMonomials.end(); ++it)
    {
        uint16_t degree = (*it).x();
        if (degree > 0)
        {
            res.mMonomials.pushBack(
                Monomial(static_cast<double>(degree) * (*it).coefficient(), (*it).w(), (*it).x() - 1, (*it).y(), (*it).z())
            );
        }
    }

    return res;
}

Polynomial Polynomial::derivativeY() const
{
    Polynomial res;

    for (auto it = mMonomials.begin(); it != mMonomials.end(); ++it)
    {
        uint16_t degree = (*it).y();
        if (degree > 0)
        {
            res.mMonomials.pushBack(
                Monomial(static_cast<double>(degree) * (*it).coefficient(), (*it).w(), (*it).x(), (*it).y() - 1, (*it).z())
            );
        }
    }

    return res;
}

Polynomial Polynomial::derivativeZ() const
{
    Polynomial res;

    for (auto it = mMonomials.begin(); it != mMonomials.end(); ++it)
    {
        uint16_t degree = (*it).z();
        if (degree > 0)
        {
            res.mMonomials.pushBack(
                Monomial(static_cast<double>(degree) * (*it).coefficient(), (*it).w(), (*it).x(), (*it).y(), (*it).z() - 1)
            );
        }
    }

    return res;
}

Polynomial Polynomial::integralX() const
{
    Polynomial res;

    for (auto it = mMonomials.begin(); it != mMonomials.end(); ++it)
    {
        uint16_t degree = (*it).x();
        if (degree < UINT16_MAX)
        {
            res.mMonomials.pushBack(
                Monomial((*it).coefficient() / static_cast<double>(degree + 1), (*it).w(), (*it).x() + 1, (*it).y(), (*it).z())
            );
        } else
        {
            throw "Overflow in integration occurred";
        }
    }

    return res;
}

Polynomial Polynomial::integralY() const
{
    Polynomial res;

    for (auto it = mMonomials.begin(); it != mMonomials.end(); ++it)
    {
        uint16_t degree = (*it).y();
        if (degree < UINT16_MAX)
        {
            res.mMonomials.pushBack(
                Monomial((*it).coefficient() / static_cast<double>(degree + 1), (*it).w(), (*it).x(), (*it).y() + 1, (*it).z())
            );
        } else
        {
            throw "Overflow in integration occurred";
        }
    }

    return res;
}

Polynomial Polynomial::integralZ() const
{
    Polynomial res;

    for (auto it = mMonomials.begin(); it != mMonomials.end(); ++it)
    {
        uint16_t degree = (*it).z();
        if (degree < UINT16_MAX)
        {
            res.mMonomials.pushBack(
                Monomial((*it).coefficient() / static_cast<double>(degree + 1), (*it).w(), (*it).x(), (*it).y(), (*it).z() + 1)
            );
        } else
        {
            throw "Overflow in integration occurred";
        }
    }

    return res;
}

Polynomial Polynomial::integralW() const
{
    Polynomial res;

    for (auto it = mMonomials.begin(); it != mMonomials.end(); ++it)
    {
        uint16_t degree = (*it).w();
        if (degree < UINT16_MAX)
        {
            res.mMonomials.pushBack(
                Monomial((*it).coefficient() / static_cast<double>(degree + 1), (*it).w() + 1, (*it).x(), (*it).y(), (*it).z())
            );
        } else
        {
            throw "Overflow in integration occurred";
        }
    }

    return res;
}

std::variant<Polynomial::Monomial, SyntaxError> Polynomial::parseMonomial(const std::string& str, size_t& offset)
{
    std::string valid = "+-.1234567890wxyz";
    if (valid.find(str[offset]) == std::string::npos)
    {
        return SyntaxError{ offset, "Unexpected symbol" };
    }

    std::string coefficient;
    if (str[offset] == '+') ++offset;
    while (offset < str.size() && (str[offset] == '-' || str[offset] == '.' || (str[offset] >= '0' && str[offset] <= '9')))
    {
        coefficient.push_back(str[offset]);
        ++offset;
    }

    double coefd;
    if (coefficient == "-") coefd = -1.0;
    else if (coefficient.size() == 0) coefd = 1.0;
    else
    {
        size_t coefl = 0;
        try
        {
            coefd = std::stod(coefficient, &coefl);
        }
        catch (...)
        {
            return SyntaxError{ offset, "Invalid coefficient!" };
        }
        if (coefl != coefficient.size()) return SyntaxError{ offset, "Invalid coefficient!" };
    }

    uint16_t powers[4]{};

    while (offset < str.size() && (str[offset] == 'w' || str[offset] == 'x' || str[offset] == 'y' || str[offset] == 'z'))
    {
        uint16_t& p = powers[str[offset] - 'w'];
        if (p != 0)
        {
            return SyntaxError{ offset,"Variables in monomes should be mentioned no more than once." };
        }

        p = 1;
        ++offset;

        if (offset == str.size()) break;
        if (str[offset] == '^')
        {
            ++offset;
            if (offset == str.size()) break;
        }

        std::string power;
        while (offset < str.size() && (str[offset] >= '0' && str[offset] <= '9'))
        {
            power.push_back(str[offset++]);
        }

        if (power.size())
        {
            uint32_t poweri;

            try
            {
                poweri = std::stoul(power);
            }
            catch (...)
            {
                return SyntaxError{ offset, "Too big power! Maximum supported power is 65535" };
            }

            if (poweri > UINT16_MAX) return SyntaxError{ offset, "Too big power! Maximum supported power is 65535" };
            p = poweri;
        }
    }

    return Monomial(coefd, powers[0], powers[1], powers[2], powers[3]);
}

std::variant<Polynomial, SyntaxError> Polynomial::parsePolynomial(const std::string& str)
{
    std::string nospaces;
    std::vector<size_t> originalIndices;

    for (size_t i = 0; i < str.size(); ++i)
    {
        if (!isspace(str[i]))
        {
            nospaces.push_back(str[i]);
            originalIndices.push_back(i);
        }
    }
    originalIndices.push_back(str.size());

    std::map<uint64_t, Monomial> monomials;
    size_t offset = 0;
    while (offset < nospaces.size())
    {
        auto res = parseMonomial(nospaces, offset);
        if (res.index())
        {
            SyntaxError err = std::get<SyntaxError>(res);
            err.pos = originalIndices[err.pos];
            return err;
        }
        Monomial m = std::get<Monomial>(res);

        if (m.coefficient() == 0.0) continue;

        if (monomials.count(m.degree()))
        {
            return SyntaxError{ originalIndices[offset], "A polynomial must contain no more than one monomial of each degree." };
        }

        monomials[m.degree()] = m;
    }

    Polynomial p;
    for (auto it = monomials.rbegin(); it != monomials.rend(); ++it)
    {
        p.mMonomials.pushBack(it->second);
    }

    return p;
}

Polynomial::Polynomial(double num)
{
    Monomial m(num, 0, 0, 0, 0);
    mMonomials.pushBack(m);
}
