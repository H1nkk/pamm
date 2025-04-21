#include <gtest/gtest.h>
#include "polynomial.h"

TEST(PolynomialTest, can_create_with_defailt_constructor)
{
    Polynomial p;
    EXPECT_EQ(p.evaluate(1, 1, 1, 1), 0.0);
}

TEST(PolynomialTest, can_add_polynomials)
{
    Polynomial p1 = std::get<Polynomial>(Polynomial::fromString("2w^2 + 3x"));
    Polynomial p2 = std::get<Polynomial>(Polynomial::fromString("w^2 + x + 4"));

    Polynomial sum = p1 + p2;

    EXPECT_EQ(sum, std::get<Polynomial>(Polynomial::fromString("3w^2 + 4x + 4")));
}

TEST(PolynomialTest, can_multiply_polynomials)
{
    Polynomial p1 = std::get<Polynomial>(Polynomial::fromString("2w^2 + 3x"));
    Polynomial p2 = std::get<Polynomial>(Polynomial::fromString("w^2 + x + 4"));

    Polynomial prod = p1 * p2;

    EXPECT_EQ(prod, std::get<Polynomial>(Polynomial::fromString("2w4 + 5xw2 + 8w2 + 3x2 + 12x")));
}

TEST(PolynomialTest, can_subtract_polynomials)
{
    Polynomial p1 = std::get<Polynomial>(Polynomial::fromString("2w^2 + 3x"));
    Polynomial p2 = std::get<Polynomial>(Polynomial::fromString("w^2 + x + 4"));

    Polynomial sum = p1 - p2;

    EXPECT_EQ(sum, std::get<Polynomial>(Polynomial::fromString("w^2 + 2x - 4")));
}

TEST(PolynomialTest, can_negate_polynomial)
{
    Polynomial p1 = std::get<Polynomial>(Polynomial::fromString("2w^2 + 3x"));
    Polynomial p2 = std::get<Polynomial>(Polynomial::fromString("-2w^2 - 3x"));

    EXPECT_EQ(p1, -p2);
}

TEST(PolynomialTest, can_multiply_by_scalar)
{
    Polynomial p = std::get<Polynomial>(Polynomial::fromString("2w^2 + 3x"));
    Polynomial result = p * 2.0;

    EXPECT_EQ(result, std::get<Polynomial>(Polynomial::fromString("4w^2 + 6x")));
}

TEST(PolynomialTest, can_get_w_derivative)
{
    Polynomial p = std::get<Polynomial>(Polynomial::fromString("3w^2 + 2x + z"));
    Polynomial derivative = p.derivativeW();

    EXPECT_EQ(derivative, std::get<Polynomial>(Polynomial::fromString("6w")));
}

TEST(PolynomialTest, can_get_x_derivative)
{
    Polynomial p = std::get<Polynomial>(Polynomial::fromString("3w^2 + 2x + z"));
    Polynomial derivative = p.derivativeX();

    EXPECT_EQ(derivative, std::get<Polynomial>(Polynomial::fromString("2")));
}

TEST(PolynomialTest, can_get_y_derivative)
{
    Polynomial p = std::get<Polynomial>(Polynomial::fromString("3w^2 + 2x + z + 4y^3"));
    Polynomial derivative = p.derivativeY();

    EXPECT_EQ(derivative, std::get<Polynomial>(Polynomial::fromString("12y^2")));
}

TEST(PolynomialTest, can_get_z_derivative)
{
    Polynomial p = std::get<Polynomial>(Polynomial::fromString("3w^2 + 2x + z + 4y^3"));
    Polynomial derivative = p.derivativeZ();

    EXPECT_EQ(derivative, std::get<Polynomial>(Polynomial::fromString("1")));
}

TEST(PolynomialTest, can_get_w_integral)
{
    Polynomial p = std::get<Polynomial>(Polynomial::fromString("3w^2 + 2x + z + 4y^3"));
    Polynomial derivative = p.integralW();

    EXPECT_EQ(derivative, std::get<Polynomial>(Polynomial::fromString("w^3 + 2xw + zw + 4wy^3")));
}

TEST(PolynomialTest, can_get_x_integral)
{
    Polynomial p = std::get<Polynomial>(Polynomial::fromString("3w^2 + 2x + z + 4y^3"));
    Polynomial derivative = p.integralX();

    EXPECT_EQ(derivative, std::get<Polynomial>(Polynomial::fromString("3xw^2 + x^2 + zx + 4xy^3")));
}

TEST(PolynomialTest, can_get_y_integral)
{
    Polynomial p = std::get<Polynomial>(Polynomial::fromString("3w^2 + 2x + z + 4y^3"));
    Polynomial derivative = p.integralY();

    EXPECT_EQ(derivative, std::get<Polynomial>(Polynomial::fromString("3yw^2 + 2xy + zy + y^4")));
}

TEST(PolynomialTest, can_get_z_integral)
{
    Polynomial p = std::get<Polynomial>(Polynomial::fromString("3w^2 + 2x + z + 4y^3"));
    Polynomial derivative = p.integralZ();

    EXPECT_EQ(derivative, std::get<Polynomial>(Polynomial::fromString("3zw^2 + 2xz + 0.5z^2 + 4zy^3")));
}

TEST(PolynomialTest, can_compare)
{
    Polynomial p1 = std::get<Polynomial>(Polynomial::fromString("2w^2 + 3x"));
    Polynomial p2 = std::get<Polynomial>(Polynomial::fromString("2w^2 + 3x"));
    Polynomial p3 = std::get<Polynomial>(Polynomial::fromString("w^2 + 3x"));

    EXPECT_TRUE(p1 == p2);
    EXPECT_TRUE(p1 != p3);
}

TEST(PolynomialTest, check_power_limit)
{
    auto result = Polynomial::fromString("2w^65536");
    EXPECT_TRUE(std::holds_alternative<SyntaxError>(result));
    if (std::holds_alternative<SyntaxError>(result))
    {
        SyntaxError err = std::get<SyntaxError>(result);
        EXPECT_EQ(err.message, "Too big power! Maximum supported power is 65535");
    }
}

TEST(PolynomialTest, can_parse_with_spaces)
{
    auto result = Polynomial::fromString("  2 w ^ 2  + 3 x  + 4  ");
    EXPECT_TRUE(std::holds_alternative<Polynomial>(result));
    if (std::holds_alternative<Polynomial>(result))
    {
        Polynomial p = std::get<Polynomial>(result);
        EXPECT_EQ(p.evaluate(2, 1, 1, 1), 15.0);
    }
}

TEST(PolynomialTest, test_ostream)
{
    Polynomial p = std::get<Polynomial>(Polynomial::fromString("2w^2 + 3x + 4"));
    std::ostringstream oss;
    oss << p;
    EXPECT_EQ(oss.str(), "2*w^2+3*x+4");
}

TEST(PolynomialTest, multiplying_by_zero_gives_zero_polynomial)
{
    Polynomial p = std::get<Polynomial>(Polynomial::fromString("3w^2 + 2x + z"));
    Polynomial result = p * 0.0;
    EXPECT_EQ(result, Polynomial());
}

TEST(PolynomialTest, can_add_zero_polynomial)
{
    Polynomial p1 = std::get<Polynomial>(Polynomial::fromString("3w^2 + 2x + z"));
    Polynomial zeroPoly;
    Polynomial result = p1 + zeroPoly;
    EXPECT_EQ(result, p1);
}

TEST(PolynomialTest, returns_empty_derivative)
{
    Polynomial p = std::get<Polynomial>(Polynomial::fromString("3w^2 + 2x + z"));
    Polynomial derivative = p.derivativeY();
    EXPECT_EQ(derivative, Polynomial());
}

TEST(PolynomialTest, error_overflow_on_multiply)
{
    Polynomial p1 = std::get<Polynomial>(Polynomial::fromString("2w^65000"));
    Polynomial p2 = std::get<Polynomial>(Polynomial::fromString("w^65000"));

    EXPECT_ANY_THROW(p1 * p2);
}

TEST(PolynomialTest, error_overflow_on_int_w)
{
    Polynomial p1 = std::get<Polynomial>(Polynomial::fromString("2w^65535"));
    EXPECT_ANY_THROW(p1.integralW());
}

TEST(PolynomialTest, error_overflow_on_int_x)
{
    Polynomial p1 = std::get<Polynomial>(Polynomial::fromString("2x^65535"));
    EXPECT_ANY_THROW(p1.integralX());
}

TEST(PolynomialTest, error_overflow_on_int_y)
{
    Polynomial p1 = std::get<Polynomial>(Polynomial::fromString("2y^65535"));
    EXPECT_ANY_THROW(p1.integralY());
}

TEST(PolynomialTest, error_overflow_on_int_z)
{
    Polynomial p1 = std::get<Polynomial>(Polynomial::fromString("2z^65535"));
    EXPECT_ANY_THROW(p1.integralZ());
}

TEST(PolynomialTest, error_on_invalid_coefficient)
{
    auto result = Polynomial::fromString("3.5.2x");
    EXPECT_TRUE(std::holds_alternative<SyntaxError>(result));
    if (std::holds_alternative<SyntaxError>(result))
    {
        SyntaxError err = std::get<SyntaxError>(result);
        EXPECT_EQ(err.message, "Invalid coefficient!");
    }
}

TEST(PolynomialTest, error_on_repeated_variable)
{
    auto result = Polynomial::fromString("2xx");
    EXPECT_TRUE(std::holds_alternative<SyntaxError>(result));
    if (std::holds_alternative<SyntaxError>(result))
    {
        SyntaxError err = std::get<SyntaxError>(result);
        EXPECT_EQ(err.message, "Variables in monomes should be mentioned no more than once.");
    }
}

TEST(PolynomialTest, empty_string_gives_zero_polynomial)
{
    auto result = Polynomial::fromString("");
    EXPECT_TRUE(std::holds_alternative<Polynomial>(result));
    if (std::holds_alternative<Polynomial>(result))
    {
        Polynomial p = std::get<Polynomial>(result);
        EXPECT_EQ(p, Polynomial());
    }
}

TEST(PolynomialTest, self_subtraction_gives_zero)
{
    Polynomial p1 = std::get<Polynomial>(Polynomial::fromString("w^2 + 2x"));
    Polynomial result = p1 - p1;
    EXPECT_EQ(result, Polynomial());
}

TEST(PolynomialTest, empty_polynomial_displayed_as_zero)
{
    Polynomial p;
    std::ostringstream oss;
    oss << p;
    EXPECT_EQ(oss.str(), "0");
}

TEST(PolynomialTest, can_evaluate_polynomial_correctly)
{
    auto result = Polynomial::fromString("3.5w^2 - x^2 + 4y^3 - 2.5z");
    EXPECT_TRUE(std::holds_alternative<Polynomial>(result));
    if (std::holds_alternative<Polynomial>(result))
    {
        Polynomial p = std::get<Polynomial>(result);
        EXPECT_DOUBLE_EQ(p.evaluate(1, 2, 3, 4), 3.5 * 1.0 * 1.0 - 2.0 * 2.0 + 4.0 * 3.0 * 3.0 * 3.0 - 2.5 * 4.0);
    }
}