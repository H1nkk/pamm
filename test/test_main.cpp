#include "googletest/include/gtest/gtest.h"

#include <polynomial.h>
#include <table.h>


//int main()
//{
//	polynomial a, b, c, d;
//	a.fromString("12x2-4y");
//	b.fromString("12x3-4y");
//	c.fromString("12y2-4z");
//	d.fromString("12x3-4z5");
//	OrderedTable t;
//	t.addPolynomial("Pol1", a);
//	t.addPolynomial("Pol2", b);
//	t.addPolynomial("Pol3", c);
//	t.addPolynomial("Pol4", d);
//	t.addPolynomial("Pol5", d);
//	t.addPolynomial("Pol6", d);
//	for (int i; i < t.size(); i++)
//	{
//
//	}
//	return 0;
//}

int main(int argc, char** argv)
{
    std::cout << ("20" > "2");
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}