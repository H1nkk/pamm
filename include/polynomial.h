#pragma once
#include "linked_list.h"
#include "syntax_error.h"
#include <cstdint>
#include <string>
#include <variant>
#include <iostream>

class Polynomial
{
private:
    class Monomial
    {
    private:
        double mCoefficient;
        uint64_t mDegree;
    public:
        Monomial() noexcept : mCoefficient(0.0), mDegree(0) {}
        Monomial(double coefficient, uint32_t w, uint32_t x, uint32_t y, uint32_t z) noexcept : mCoefficient(coefficient),
            mDegree(((uint64_t)w << 48) | ((uint64_t)x << 32) | ((uint64_t)y << 16) | ((uint64_t)z))
        {
            if (w > UINT16_MAX || x > UINT16_MAX || y > UINT16_MAX || z > UINT16_MAX)
            {
                throw std::invalid_argument(__FUNCTION__ ": overflow occurred.");
            }
        }

        bool operator==(const Monomial& other) const { return mCoefficient == other.mCoefficient && mDegree == other.mDegree; }
        bool operator!=(const Monomial& other) const { return !operator==(other); }

        double coefficient() const noexcept { return mCoefficient; }
        void setCoefficient(double coefficient) { mCoefficient = coefficient; }
        uint64_t degree() const noexcept { return mDegree; }
        uint16_t w() const noexcept { return static_cast<uint16_t>(mDegree >> 48); }
        uint16_t x() const noexcept { return static_cast<uint16_t>(mDegree >> 32); }
        uint16_t y() const noexcept { return static_cast<uint16_t>(mDegree >> 16); }
        uint16_t z() const noexcept { return static_cast<uint16_t>(mDegree); }
    };

    LinkedList<Monomial> mMonomials;

    static std::variant<Monomial, SyntaxError> parseMonomial(const std::string& str, size_t& offset);
    static std::variant<Polynomial, SyntaxError> parsePolynomial(const std::string& str);

    explicit Polynomial(const std::string& strRepr) {}
public:
    Polynomial() {}
    Polynomial(double num);

    static std::variant<Polynomial, SyntaxError> fromString(const std::string& str)
    {
        return parsePolynomial(str);
    }

    bool operator==(const Polynomial& other) const;
    bool operator!=(const Polynomial& other) const;

    Polynomial operator+(const Polynomial& other) const;
    Polynomial& operator+=(const Polynomial& other);
    Polynomial operator-() const;
    Polynomial operator-(const Polynomial& other) const;
    Polynomial& operator-=(const Polynomial& other);
    Polynomial operator*(const Polynomial& other) const;
    Polynomial operator*(double coefficient) const;
    friend Polynomial operator*(double coefficient, const Polynomial& p);
    Polynomial& operator*=(double coefficient);
    friend std::ostream& operator<<(std::ostream& ostr, const Polynomial& p);

    double evaluate(double w, double x, double y, double z) const;
    Polynomial derivativeW() const;
    Polynomial derivativeX() const;
    Polynomial derivativeY() const;
    Polynomial derivativeZ() const;
    Polynomial integralX() const;
    Polynomial integralY() const;
    Polynomial integralZ() const;
    Polynomial integralW() const;
};