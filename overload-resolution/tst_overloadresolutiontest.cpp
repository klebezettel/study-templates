#include <QString>
#include <QtTest>

class OverloadResolutionTest : public QObject
{
    Q_OBJECT

private slots:
    void testCase1();
};

// ----------------------------------------------------------------------

// From "C++ Templates - The Complete Guide", B.2
void combine(int, double);
void combine(double, int);

void f()
{
//    combine(1, 2); // ambiguous
}

// ----------------------------------------------------------------------
// Overload resolution vs template argument deduction
// ==> First template argument deduction happens, then overload resolution.
// During argument deduction no conversation operators are used, types must
// match perfectly!

template <typename T>
struct MyString {
    MyString() {}
    MyString(T const *) {} // converting constructor
};

template <typename T>
MyString<T> truncate(MyString<T> const &s, int)
{ return s;}

void f2()
{
    MyString<char> s1 = truncate<char>("hello", 5); // OK, no argument deduction, no conflict.
    Q_UNUSED(s1);

    // OPS, argument deduction fails (types must match without conversation),
    // so it does not even comes to overload resolution
//    MyString<char> s2 = truncate("hello", 5);
}

void OverloadResolutionTest::testCase1()
{
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(OverloadResolutionTest)

#include "tst_overloadresolutiontest.moc"
